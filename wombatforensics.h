#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
//#include "wombatvariable.h"
//#include "wombatdatabase.h"
//#include "wombatframework.h"
#include "wombatfunctions.h"
#include "ui_wombatforensics.h"
#include "propertieswindow.h"
#include "exportdialog.h"
#include "fileviewer.h"
#include "globals.h"
#include "imgreader.h"
#include "imghexviewer.h"
#include "filterviews.h"
#include "imageviewer.h"
#include "videoviewer.h"
#include "viewermanager.h"
#include "textviewer.h"
#include "htmlviewer.h"
#include "messageviewer.h"
#include "byteconverter.h"
#include "digdeeperdialog.h"
#include "aboutbox.h"
#include "cancelthread.h"

class WombatSlider : public QSlider
{
    Q_OBJECT

public:
    WombatSlider(QWidget* parent = 0) : QSlider(parent) {};
signals:
    void ShowJumpFilter(void);

protected:
    void mousePressEvent(QMouseEvent* event)
    {
        if(event->button() == Qt::RightButton)
        {
            emit ShowJumpFilter();
            event->accept();
        }
        else
        {
            event->accept();
            QSlider::mousePressEvent(event);
        }
    };
};

class EvidenceWorker : public QObject {
    Q_OBJECT
public:
    EvidenceWorker(WombatVariable wombatvariable) {
        readimginfo = NULL;
        readvsinfo = NULL;
        readfsinfo = NULL;
        readfileinfo = NULL;
        //isrunning = true;
        wombatvariable = wombatvariable;
        //AddNewEvidence();
    };
    ~EvidenceWorker() {};
public slots:
    void process() {
        //AddNewEvidence();
        // place add new evidence code in here...
        //while(isrunning)
        {
            const TSK_TCHAR** images;
            images = (const char**)malloc(wombatvariable.evidenceobject.fullpathvector.size()*sizeof(char*));
            for(uint i=0; i < wombatvariable.evidenceobject.fullpathvector.size(); i++)
            {
                images[i] = wombatvariable.evidenceobject.fullpathvector[i].c_str();
            }
            readimginfo = tsk_img_open(wombatvariable.evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
            if(readimginfo == NULL)
            {
                LogMessage("Evidence Image acess failed");
                errorcount++;
            }
            free(images);
            fsobjectlist.clear();
            QFile evidfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".evid");
            evidfile.open(QIODevice::Append | QIODevice::Text);
            QTextStream out(&evidfile);
            out << (int)readimginfo->itype << "," << (unsigned long long)readimginfo->size << "," << (int)readimginfo->sector_size;
            for(unsigned int i=0; i < wombatvariable.evidenceobject.itemcount; i++)
                out << QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[i]) << "," << i+1;
            evidfile.close();
            readvsinfo = tsk_vs_open(readimginfo, 0, TSK_VS_TYPE_DETECT);
            QString volname = "Dummy Volume";
            int voltype = 240;
            int volsectorsize = (int)readimginfo->sector_size;
            unsigned long long voloffset = 0;
            if(readvsinfo != NULL)
            {
                voltype = (int)readvsinfo->vstype;
                volname = QString::fromUtf8(tsk_vs_type_todesc(readvsinfo->vstype));
                volsectorsize = (int)readvsinfo->block_size;
                voloffset = (unsigned long long)readvsinfo->offset;
            }

            QFile volfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".vol");
            volfile.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&volfile);
            out << voltype << "," << (unsigned long long)readimginfo->size << "," << volname << "," << volsectorsize << "," << voloffset;
            volfile.close();
            if(readvsinfo == NULL) // No volume, so a single file system is all there is in the image.
            {
                readfsinfo = tsk_fs_open_img(readimginfo, 0, TSK_FS_TYPE_DETECT);
                QFile pfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".p1");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                out << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << GetFileSystemLabel(readfsinfo) << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readfsinfo->block_count << "," << (unsigned long long)readfsinfo->block_size;
                pfile.close();
                uint8_t walkreturn;
                int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
            }
            else
            {
                QFile pfile;
                if(readvsinfo->part_count > 0)
                {
                    for(uint32_t i=0; i < readvsinfo->part_count; i++)
                    {
                        readpartinfo = tsk_vs_part_get(readvsinfo, i);
                        pfile.setFileName(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".p" + QString::number(i));
                        pfile.open(QIODevice::Append | QIODevice::Text);
                        out.setDevice(&pfile);
                        if(readpartinfo->flags == 0x02) // unallocated partition
                        {
                            out << readpartinfo->desc << "," << readpartinfo->flags << "," << (unsigned long long)readpartinfo->len * readvsinfo->block_size << "," << (unsigned long long)readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readvsinfo->block_size;
                        }
                        else if(readpartinfo->flags == 0x01) // allocated partition
                        {
                            readfsinfo = tsk_fs_open_vol(readpartinfo, TSK_FS_TYPE_DETECT);
                            if(readfsinfo != NULL)
                            {
                                out << GetFileSystemLabel(readfsinfo) << "," << readpartinfo->flags << "," << readfsinfo->ftype << "," << (unsigned long long)readfsinfo->block_size * (unsigned long long)readfsinfo->block_count << "," << (unsigned long long)readfsinfo->root_inum << "," << (unsigned long long)readfsinfo->offset << "," << (unsigned long long)readpartinfo->start << "," << (unsigned long long)readpartinfo->len << "," << (int)readfsinfo->dev_bsize << "," << (int)readfsinfo->block_size << "," << (unsigned long long)readfsinfo->block_count;
                                uint8_t walkreturn;
                                int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
                                walkreturn = tsk_fs_dir_walk(readfsinfo, readfsinfo->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
                                if(walkreturn == 1)
                                {
                                    LogMessage("Issues with traversing the file structure were encountered");
                                    errorcount++;
                                }
                            }
                        }
                        pfile.close();
                    }
                }
            }
            // finished initial partition/file system information including file info stored into the vector 
            // place InitializeQueryModel(); in here...

            emit finished();
        }
        //emit canceled();
    };
signals:
    void finished();
    void canceled();
    void error(QString err);
private:
    // add my variables here
    TSK_IMG_INFO* readimginfo;
    TSK_VS_INFO* readvsinfo;
    const TSK_VS_PART_INFO* readpartinfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    char asc[512];
    iso9660_pvd_node* p;
    HFS_INFO* hfs;
    //bool isrunning;
    WombatVariable wombatvariable;

    uint8_t hfs_cat_file_lookup(HFS_INFO* hfs, TSK_INUM_T inum, HFS_ENTRY* entry, unsigned char follow_hard_link)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        hfs_btree_key_cat key;      /* current catalog key */
        hfs_thread thread;          /* thread record */
        hfs_file_folder record;     /* file/folder record */
        TSK_OFF_T off;

        // Test if this is a special file that is not located in the catalog
        if ((inum == HFS_EXTENTS_FILE_ID) ||
            (inum == HFS_CATALOG_FILE_ID) ||
            (inum == HFS_ALLOCATION_FILE_ID) ||
            (inum == HFS_STARTUP_FILE_ID) ||
            (inum == HFS_ATTRIBUTES_FILE_ID)) {
            return 1;
        }


        /* first look up the thread record for the item we're searching for */

        /* set up the thread record key */
        memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
        cnid_to_array((uint32_t) inum, key.parent_cnid);

        /* look up the thread record */
        off = hfs_cat_get_record_offset(hfs, &key);
        if (off == 0)
        {
            // put error code here...
            return 1;
        }

        /* read the thread record */
        if (hfs_cat_read_thread_record(hfs, off, &thread))
        {
            //tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM ")", inum);
            return 1;
        }

        /* now look up the actual file/folder record */

        memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
        memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
        memcpy((char *) key.parent_cnid, (char *) thread.parent_cnid,
            sizeof(key.parent_cnid));
        memcpy((char *) &key.name, (char *) &thread.name, sizeof(key.name));

        /* look up the record */
        off = hfs_cat_get_record_offset(hfs, &key);
        if (off == 0)
        {
            // print error here
            return 1;
        }

        /* read the record */
        if (hfs_cat_read_file_folder_record(hfs, off, &record))
        {
            //tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM ")", inum);
            return 1;
        }

        /* these memcpy can be gotten rid of, really */
        if (tsk_getu16(fs->endian, record.file.std.rec_type) == HFS_FOLDER_RECORD) {
            memcpy((char *) &entry->cat, (char *) &record, sizeof(hfs_folder));
        }
        else if (tsk_getu16(fs->endian, record.file.std.rec_type) == HFS_FILE_RECORD) {
            memcpy((char *) &entry->cat, (char *) &record, sizeof(hfs_file));
        }
        /* other cases already caught by hfs_cat_read_file_folder_record */

        memcpy((char *) &entry->thread, (char *) &thread, sizeof(hfs_thread));

        entry->flags = TSK_FS_META_FLAG_ALLOC | TSK_FS_META_FLAG_USED;
        entry->inum = inum;

        if (follow_hard_link) {
            // TEST to see if this is a hard link
            unsigned char is_err;
            TSK_INUM_T target_cnid =
                hfs_follow_hard_link(hfs, &(entry->cat), &is_err);
            if (is_err > 1) {
                /*
                error_returned
                ("hfs_cat_file_lookup: error occurred while following a possible hard link for "
                "inum (cnid) =  %" PRIuINUM, inum);
                */
                return 1;
            }
            if (target_cnid != inum) {
                // This is a hard link, and we have got the cnid of the target file, so look it up.
                uint8_t res =
                    hfs_cat_file_lookup(hfs, target_cnid, entry, FALSE);
                if (res != 0) {
                    /*
                    error_returned
                    ("hfs_cat_file_lookup: error occurred while looking up the Catalog entry for "
                    "the target of inum (cnid) = %" PRIuINUM " target",
                    inum);
                    */
                }
                return 1;
            }

            // Target is NOT a hard link, so fall through to the non-hard link exit.
        }

        return 0;
    };
    uint8_t hfs_UTF16toUTF8(TSK_FS_INFO* fs, uint8_t* uni, int ulen, char* asc, int alen, uint32_t flags)
    {
        UTF8 *ptr8;
        uint8_t *uniclean;
        UTF16 *ptr16;
        int i;
        TSKConversionResult r;

        // remove nulls from the Unicode string
        // convert / to :
        uniclean = (uint8_t *) tsk_malloc(ulen * 2);
        if (!uniclean)
            return 1;

        memcpy(uniclean, uni, ulen * 2);

        for (i = 0; i < ulen; ++i) {
            uint16_t uc = tsk_getu16(fs->endian, uniclean + i * 2);


            int changed = 0;
            if (uc == UTF16_NULL) {
                uc = UTF16_NULL_REPLACE;
                changed = 1;
            }
            else if ((flags & HFS_U16U8_FLAG_REPLACE_SLASH)
                && uc == UTF16_SLASH) {
                uc = UTF16_COLON;
                changed = 1;
            }

            else if ((flags & HFS_U16U8_FLAG_REPLACE_CONTROL)
                && uc < UTF16_LEAST_PRINTABLE) {
                uc = (uint16_t) UTF16_NULL_REPLACE;
                changed = 1;
            }

            if (changed)
                *((uint16_t *) (uniclean + i * 2)) =
                    tsk_getu16(fs->endian, (uint8_t *) & uc);
        }

    // convert to UTF-8
        memset(asc, 0, alen);

        ptr8 = (UTF8 *) asc;
        ptr16 = (UTF16 *) uniclean;
        r = tsk_UTF16toUTF8(fs->endian, (const UTF16 **) &ptr16,
        (const UTF16 *) (&uniclean[ulen * 2]), &ptr8,
        (UTF8 *) & asc[alen], TSKstrictConversion);

        free(uniclean);
        if (r != TSKconversionOK) {
            tsk_error_set_errno(TSK_ERR_FS_UNICODE);
            tsk_error_set_errstr
                ("hfs_UTF16toUTF8: unicode conversion failed (%d)", (int) r);
            return 1;
        }

        return 0;
    };
    static uint8_t hfs_cat_get_record_offset_cb(HFS_INFO* hfs, int8_t level_type, const void* targ_data, const hfs_btree_key_cat* cur_key, TSK_OFF_T key_off, void* ptr)
    {
        const hfs_btree_key_cat *targ_key = (hfs_btree_key_cat *) targ_data;
        //if (tsk_verbose)
        //    tsk_fprintf(stderr,
        //        "hfs_cat_get_record_offset_cb: %s node want: %" PRIu32
        //        " vs have: %" PRIu32 "\n",
        //        (level_type == HFS_BT_NODE_TYPE_IDX) ? "Index" : "Leaf",
        //        tsk_getu32(hfs->fs_info.endian, targ_key->parent_cnid),
        //        tsk_getu32(hfs->fs_info.endian, cur_key->parent_cnid));

        if (level_type == HFS_BT_NODE_TYPE_IDX) {
            int diff = hfs_cat_compare_keys(hfs, cur_key, targ_key);
            if (diff < 0)
                return HFS_BTREE_CB_IDX_LT;
            else
                return HFS_BTREE_CB_IDX_EQGT;
        }
        else {
            int diff = hfs_cat_compare_keys(hfs, cur_key, targ_key);

            // see if this record is for our file or if we passed the interesting entries
            if (diff < 0) {
                return HFS_BTREE_CB_LEAF_GO;
            }
            else if (diff == 0) {
                TSK_OFF_T *off = (TSK_OFF_T *) ptr;
                *off =
                    key_off + 2 + tsk_getu16(hfs->fs_info.endian,
                    cur_key->key_len);
            }
            return HFS_BTREE_CB_LEAF_STOP;
        }   
    };
    static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle)
    {
        TSK_OFF_T off = 0;
        if(hfs_cat_traverse(hfs, needle, hfs_cat_get_record_offset_cb, &off))
        {
            return 0;
        }
        return off;
    };
    uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        uint16_t uni_len;
        size_t cnt;

        memset(thread, 0, sizeof(hfs_thread));
        cnt = tsk_fs_attr_read(hfs->catalog_attr, off, (char *) thread, 10, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        if (cnt != 10) {
            //tsk_error_reset();
            //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2("hfs_cat_read_thread_record: Error reading catalog offset %"PRIuOFF " (header)", off);
            return 1;
        }

        if ((tsk_getu16(fs->endian, thread->rec_type) != HFS_FOLDER_THREAD)
            && (tsk_getu16(fs->endian, thread->rec_type) != HFS_FILE_THREAD)) {
            //tsk_error_set_errno(TSK_ERR_FS_GENFS);
            //tsk_error_set_errstr("hfs_cat_read_thread_record: unexpected record type %" PRIu16,tsk_getu16(fs->endian, thread->rec_type));
            return 1;
        }

        uni_len = tsk_getu16(fs->endian, thread->name.length);

        if (uni_len > 255) {
            //tsk_error_set_errno(TSK_ERR_FS_INODE_COR);
            //tsk_error_set_errstr
            //("hfs_cat_read_thread_record: invalid string length (%" PRIu16
            //")", uni_len);
            return 1;
        }

        cnt =
            tsk_fs_attr_read(hfs->catalog_attr, off + 10, (char *) thread->name.unicode, uni_len * 2, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        if (cnt != uni_len * 2) {
                //tsk_error_reset();
            //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2
            //("hfs_cat_read_thread_record: Error reading catalog offset %"
            //PRIuOFF " (name)", off + 10);
            return 1;
        }

        return 0;
    };
    uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        size_t cnt;
        char rec_type[2];

        memset(record, 0, sizeof(hfs_file_folder));

        cnt = tsk_fs_attr_read(hfs->catalog_attr, off, rec_type, 2, (TSK_FS_FILE_READ_FLAG_ENUM)0);
        if (cnt != 2) {
            //tsk_error_reset();
            //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2
            //("hfs_cat_read_file_folder_record: Error reading record type from catalog offset %"
            //PRIuOFF " (header)", off);
            return 1;
        }

        if (tsk_getu16(fs->endian, rec_type) == HFS_FOLDER_RECORD) {
            cnt = 
                tsk_fs_attr_read(hfs->catalog_attr, off, (char *) record, sizeof(hfs_folder), (TSK_FS_FILE_READ_FLAG_ENUM)0);
            if (cnt != sizeof(hfs_folder)) {
                //tsk_error_reset();
                //tsk_error_set_errno(TSK_ERR_FS_READ);
                //tsk_error_set_errstr2
                //   ("hfs_cat_read_file_folder_record: Error reading catalog offset %"
                //    PRIuOFF " (folder)", off);
                return 1;
            }
        }
        else if (tsk_getu16(fs->endian, rec_type) == HFS_FILE_RECORD) {
            cnt =
                tsk_fs_attr_read(hfs->catalog_attr, off, (char *) record, sizeof(hfs_file), (TSK_FS_FILE_READ_FLAG_ENUM)0);
            if (cnt != sizeof(hfs_file)) {
                //tsk_error_reset();
                //tsk_error_set_errno(TSK_ERR_FS_READ);
            //tsk_error_set_errstr2
            //    ("hfs_cat_read_file_folder_record: Error reading catalog offset %"
            //    PRIuOFF " (file)", off);
            return 1;
            }
        }
        else {
            //tsk_error_set_errno(TSK_ERR_FS_GENFS);
            //tsk_error_set_errstr
            //    ("hfs_cat_read_file_folder_record: unexpected record type %"
            //    PRIu16, tsk_getu16(fs->endian, rec_type));
            return 1;
        }

        return 0;
    };
    static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2)
    {
        TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
        uint32_t cnid1, cnid2;

        cnid1 = tsk_getu32(fs->endian, key1->parent_cnid);
        cnid2 = tsk_getu32(fs->endian, key2->parent_cnid);

        if (cnid1 < cnid2)
            return -1;
        if (cnid1 > cnid2)
            return 1;

        return hfs_unicode_compare(hfs, &key1->name, &key2->name);

    };
    static uint8_t hfs_cat_traverse(HFS_INFO* hfs, const void* targ_data, TSK_HFS_BTREE_CB a_cb, void* ptr)
    {
        TSK_FS_INFO *fs = &(hfs->fs_info);
        uint32_t cur_node;          /* node id of the current node */
        char *node;

        uint16_t nodesize;
        uint8_t is_done = 0;

        //tsk_error_reset();

        nodesize = tsk_getu16(fs->endian, hfs->catalog_header.nodesize);
        if ((node = (char *) tsk_malloc(nodesize)) == NULL)
            return 1;

        /* start at root node */
        cur_node = tsk_getu32(fs->endian, hfs->catalog_header.rootNode);

        /* if the root node is zero, then the extents btree is empty */
        /* if no files have overflow extents, the Extents B-tree still
           exists on disk, but is an empty B-tree containing only
           the header node */
        if (cur_node == 0) {
            //if (tsk_verbose)
            //    tsk_fprintf(stderr, "hfs_cat_traverse: "
            //        "empty extents btree\n");
            free(node);
            return 1;
        }

        //if (tsk_verbose)
        //    tsk_fprintf(stderr, "hfs_cat_traverse: starting at "
        //        "root node %" PRIu32 "; nodesize = %"
        //        PRIu16 "\n", cur_node, nodesize);

        /* Recurse down to the needed leaf nodes and then go forward */
        is_done = 0;
        while (is_done == 0) {
            TSK_OFF_T cur_off;      /* start address of cur_node */
            uint16_t num_rec;       /* number of records in this node */
            ssize_t cnt;
            hfs_btree_node *node_desc;

            // sanity check 
            if (cur_node > tsk_getu32(fs->endian,
                    hfs->catalog_header.totalNodes)) {
         //       tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //       tsk_error_set_errstr
         //           ("hfs_cat_traverse: Node %d too large for file", cur_node);
                free(node);
                return 1;
            }

            // read the current node
            cur_off = cur_node * nodesize;
            cnt = tsk_fs_attr_read(hfs->catalog_attr, cur_off, node, nodesize, (TSK_FS_FILE_READ_FLAG_ENUM)0);
            if (cnt != nodesize) {
                if (cnt >= 0) {
         //           tsk_error_reset();
         //           tsk_error_set_errno(TSK_ERR_FS_READ);
                }
         //       tsk_error_set_errstr2
         //           ("hfs_cat_traverse: Error reading node %d at offset %"
         //           PRIuOFF, cur_node, cur_off);
                free(node);
                return 1;
            }

            // process the header / descriptor
            node_desc = (hfs_btree_node *) node;
            num_rec = tsk_getu16(fs->endian, node_desc->num_rec);

         //   if (tsk_verbose)
         //       tsk_fprintf(stderr, "hfs_cat_traverse: node %" PRIu32
         //           " @ %" PRIu64 " has %" PRIu16 " records\n",
         //           cur_node, cur_off, num_rec);

            if (num_rec == 0) {
         //       tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //       tsk_error_set_errstr("hfs_cat_traverse: zero records in node %"
         //           PRIu32, cur_node);
                free(node);
                return 1;
            }

            /* With an index node, find the record with the largest key that is smaller
             * to or equal to cnid */
            if (node_desc->type == HFS_BT_NODE_TYPE_IDX) {
                uint32_t next_node = 0;
                int rec;

                for (rec = 0; rec < num_rec; rec++) {
                    size_t rec_off;
                    hfs_btree_key_cat *key;
                    uint8_t retval;
    
                    // get the record offset in the node
                    rec_off =
                        tsk_getu16(fs->endian,
                        &node[nodesize - (rec + 1) * 2]);
                    if (rec_off > nodesize) {
           //             tsk_error_set_errno(TSK_ERR_FS_GENFS);
           //             tsk_error_set_errstr
           //                 ("hfs_cat_traverse: offset of record %d in index node %d too large (%d vs %"
           //                 PRIu16 ")", rec, cur_node, (int) rec_off,
           //                 nodesize);
                        free(node);
                        return 1;
                    }
                    key = (hfs_btree_key_cat *) & node[rec_off];

                    /*
                       if (tsk_verbose)
                       tsk_fprintf(stderr,
                       "hfs_cat_traverse: record %" PRIu16
                       " ; keylen %" PRIu16 " (%" PRIu32 ")\n", rec,
                       tsk_getu16(fs->endian, key->key_len),
                       tsk_getu32(fs->endian, key->parent_cnid));
                     */

                    /* save the info from this record unless it is too big */
                    retval =
                        a_cb(hfs, HFS_BT_NODE_TYPE_IDX, targ_data, key,
                        cur_off + rec_off, ptr);
                    if (retval == HFS_BTREE_CB_ERR) {
         //               tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //               tsk_error_set_errstr2
         //                   ("hfs_cat_traverse: Callback returned error");
                        free(node);
                        return 1;
                    }
                    // record the closest entry
                    else if ((retval == HFS_BTREE_CB_IDX_LT)
                        || (next_node == 0)) {
                        hfs_btree_index_record *idx_rec;
                        int keylen =
                            2 + hfs_get_idxkeylen(hfs, tsk_getu16(fs->endian,
                                key->key_len), &(hfs->catalog_header));
                        if (rec_off + keylen > nodesize) {
         //                   tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //                   tsk_error_set_errstr
         //                       ("hfs_cat_traverse: offset of record and keylength %d in index node %d too large (%d vs %"
         //                       PRIu16 ")", rec, cur_node,
         //                       (int) rec_off + keylen, nodesize);
                            free(node);
                            return 1;
                        }
                        idx_rec =
                            (hfs_btree_index_record *) & node[rec_off +
                            keylen];
                        next_node = tsk_getu32(fs->endian, idx_rec->childNode);
                    }
                    if (retval == HFS_BTREE_CB_IDX_EQGT) {
                        // move down to the next node
                        break;
                    }
                }
                // check if we found a relevant node
                if (next_node == 0) {
         //           tsk_error_set_errno(TSK_ERR_FS_GENFS);
         //           tsk_error_set_errstr
         //               ("hfs_cat_traverse: did not find any keys in index node %d",
         //               cur_node);
                    is_done = 1;
                    break;
                }
                cur_node = next_node;
            }

            /* With a leaf, we look for the specific record. */
            else if (node_desc->type == HFS_BT_NODE_TYPE_LEAF) {
                int rec;

            for (rec = 0; rec < num_rec; rec++) {
                size_t rec_off;
                hfs_btree_key_cat *key;
                uint8_t retval;

                // get the record offset in the node
                rec_off =
                    tsk_getu16(fs->endian,
                    &node[nodesize - (rec + 1) * 2]);
                if (rec_off > nodesize) {
      //              tsk_error_set_errno(TSK_ERR_FS_GENFS);
      //              tsk_error_set_errstr
      //                  ("hfs_cat_traverse: offset of record %d in leaf node %d too large (%d vs %"
      //                  PRIu16 ")", rec, cur_node, (int) rec_off,
      //                  nodesize);
                    free(node);
                    return 1;
                }
                key = (hfs_btree_key_cat *) & node[rec_off];

                /*
                   if (tsk_verbose)
                   tsk_fprintf(stderr,
                   "hfs_cat_traverse: record %" PRIu16
                   "; keylen %" PRIu16 " (%" PRIu32 ")\n", rec,
                   tsk_getu16(fs->endian, key->key_len),
                   tsk_getu32(fs->endian, key->parent_cnid));
                 */
                //                rec_cnid = tsk_getu32(fs->endian, key->file_id);

                retval =
                    a_cb(hfs, HFS_BT_NODE_TYPE_LEAF, targ_data, key,
                    cur_off + rec_off, ptr);
                if (retval == HFS_BTREE_CB_LEAF_STOP) {
                    is_done = 1;
                    break;
                }
                else if (retval == HFS_BTREE_CB_ERR) {
      //              tsk_error_set_errno(TSK_ERR_FS_GENFS);
      //              tsk_error_set_errstr2
      //                  ("hfs_cat_traverse: Callback returned error");
                    free(node);
                    return 1;
                }
            }

            // move right to the next node if we got this far
            if (is_done == 0) {
                cur_node = tsk_getu32(fs->endian, node_desc->flink);
                if (cur_node == 0) {
                    is_done = 1;
                }
      //          if (tsk_verbose)
      //              tsk_fprintf(stderr,
      //                  "hfs_cat_traverse: moving forward to next leaf");
              }
            }
            else {
          //      tsk_error_set_errno(TSK_ERR_FS_GENFS);
          //      tsk_error_set_errstr("hfs_cat_traverse: btree node %" PRIu32
          //          " (%" PRIu64 ") is neither index nor leaf (%" PRIu8 ")",
          //          cur_node, cur_off, node_desc->type);
                free(node);
                return 1;
            }
        }
        free(node);
        return 0;
    };
 

    QString GetFileSystemLabel(TSK_FS_INFO* curinfo)
    {
        if(curinfo != NULL)
        {
            if(curinfo->ftype == TSK_FS_TYPE_EXT2 || curinfo->ftype == TSK_FS_TYPE_EXT3 || curinfo->ftype == TSK_FS_TYPE_EXT4 || curinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
            {
                return QString::fromStdString(string(((EXT2FS_INFO*)curinfo)->fs->s_volume_name));
            }
            else if(curinfo->ftype == TSK_FS_TYPE_FFS1 || curinfo->ftype == TSK_FS_TYPE_FFS1B)
            {
                return "UFS1";
            }
            else if(curinfo->ftype == TSK_FS_TYPE_FFS2 || curinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
            {
                return QString::fromUtf8(reinterpret_cast<char*>(((FFS_INFO*)curinfo)->fs.sb2->volname));
            }
            else if(curinfo->ftype == TSK_FS_TYPE_FAT12 || curinfo->ftype == TSK_FS_TYPE_FAT16)
            {
                sprintf(asc, "%c%c%c%c%c%c%c%c%c%c%c", ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[0], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[1], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[2], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[3], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[4], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[5], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[6], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[7], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[8], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[9], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab[10]);
                return QString::fromStdString(string(asc));
            }
            else if(curinfo->ftype == TSK_FS_TYPE_FAT32)
            {
                sprintf(asc, "%c%c%c%c%c%c%c%c%c%c%c", ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[0], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[1], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[2], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[3], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[4], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[5], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[6], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[7], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[8], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[9], ((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab[10]);
                return QString::fromStdString(string(asc));
            }
            else if(curinfo->ftype == TSK_FS_TYPE_NTFS)
            {
                TSK_FS_FILE* tmpfile = NULL;
                const TSK_FS_ATTR* tmpattr;
                if((tmpfile = tsk_fs_file_open_meta(curinfo, NULL, NTFS_MFT_VOL)) != NULL)
                {
                    tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, TSK_FS_ATTR_TYPE_NTFS_VNAME);
                    if((tmpattr->flags & TSK_FS_ATTR_RES) && (tmpattr->size))
                    {
                        UTF16* name16 = (UTF16*) tmpattr->rd.buf;
                        UTF8* name8 = (UTF8*) asc;
                        int retval;
                        retval = tsk_UTF16toUTF8(curinfo->endian, (const UTF16**)&name16, (UTF16*) ((uintptr_t)name16 + (int) tmpattr->size), &name8, (UTF8*) ((uintptr_t)name8 + sizeof(asc)), TSKlenientConversion);
                        if(retval != TSKconversionOK)
                            *name8 = '\0';
                        else if((uintptr_t)name8 >= (uintptr_t)asc + sizeof(asc))
                            asc[sizeof(asc)-1] = '\0';
                        else
                            *name8 = '\0';
                        return QString::fromStdString(string(asc));
                    }
                }
            }
            else if(curinfo->ftype == TSK_FS_TYPE_EXFAT)
            {
                char* databuffer = NULL;
                TSK_DADDR_T cursector = 0;
                TSK_DADDR_T endsector = 0;
                int8_t isallocsec = 0;
                TSK_INUM_T curinum = 0;
                FATFS_DENTRY* dentry = NULL;
                TSK_FS_FILE* tmpfile = NULL;
                ssize_t bytesread = 0;
                if((tmpfile = tsk_fs_file_alloc(curinfo)) != NULL)
                {
                    if((tmpfile->meta = tsk_fs_meta_alloc(FATFS_FILE_CONTENT_LEN)) != NULL)
                    {
                        if((databuffer = (char*)tsk_malloc(((FATFS_INFO*)curinfo)->ssize)) != NULL)
                        {
                            cursector = ((FATFS_INFO*)curinfo)->rootsect;
                            endsector = (((FATFS_INFO*)curinfo)->firstdatasect + ((FATFS_INFO*)curinfo)->clustcnt * ((FATFS_INFO*)curinfo)->csize) - 1;
                            while(cursector < endsector)
                            {
                            }
                            bytesread = tsk_fs_read_block(curinfo, cursector, databuffer, ((FATFS_INFO*)curinfo)->ssize);
                            if(bytesread == ((FATFS_INFO*)curinfo)->ssize)
                            {
                                isallocsec = fatfs_is_sectalloc(((FATFS_INFO*)curinfo), cursector);
                                if(isallocsec != -1)
                                {
                                    curinum = FATFS_SECT_2_INODE(((FATFS_INFO*)curinfo), cursector);
                                    for(int i = 0; i < ((FATFS_INFO*)curinfo)->ssize; i+= sizeof(FATFS_DENTRY))
                                    {
                                        dentry = (FATFS_DENTRY*)&(databuffer[i]);
                                        if(exfatfs_get_enum_from_type(dentry->data[0]) == EXFATFS_DIR_ENTRY_TYPE_VOLUME_LABEL)
                                        {
                                            if(exfatfs_dinode_copy(((FATFS_INFO*)curinfo), curinum, dentry, isallocsec, tmpfile) == TSK_OK)
                                                return QString::fromStdString(tmpfile->meta->name2->name);
                                        }
                                    }
                                }
                            }
                        }
                        tsk_fs_file_close(tmpfile);
                        free(databuffer);
                    }
                }
            }
            else if(curinfo->ftype == TSK_FS_TYPE_ISO9660)
            {
                for(p = ((ISO_INFO*)curinfo)->pvd; p != NULL; p = p->next)
                {
                    return QString::fromUtf8(reinterpret_cast<char*>(p->pvd.vol_id));
                }
            }
            else if(curinfo->ftype == TSK_FS_TYPE_YAFFS2)
            {
                return "YAFFS2";
            }
            else if(curinfo->ftype == TSK_FS_TYPE_SWAP)
            {
                return "SWAP";
            }
            else if(curinfo->ftype == TSK_FS_TYPE_HFS)
            {
                hfs = (HFS_INFO*)curinfo;
                char fn[HFS_MAXNAMLEN + 1];
                HFS_ENTRY entry;
                if(hfs_cat_file_lookup(hfs, HFS_ROOT_INUM, &entry, FALSE))
                {
                    // log error here
                }
                if(hfs_UTF16toUTF8(curinfo, entry.thread.name.unicode, tsk_getu16(curinfo->endian, entry.thread.name.length), fn, HFS_MAXNAMLEN + 1, HFS_U16U8_FLAG_REPLACE_SLASH))
                {
                    // log error here
                }
                sprintf(asc, "%s", fn);
                return QString::fromStdString(string(asc));
            }
            return "";
        }
        return "";
    };

};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(QObject* parent = 0) : QAbstractItemModel(parent)
    {
        headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "File Signature" << "File Category";
        //headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category" << "Checked" << "MFT Attribute ID";
        rootnode = 0;
        QList<QVariant> emptyset;
        emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
        //emptyset << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
        rootnode = new Node(emptyset);
        rootnode->parent = 0;
        rootnode->childcount = 0;
    };

    ~TreeModel()
    {
        delete rootnode;
    };

    enum MyDataRoles
    {
        IndexPtrRole = Qt::UserRole + 1
    };

    QModelIndex index(int row, int col, const QModelIndex &parent) const
    {
        if(!rootnode || row < 0 || col < 0)
            return QModelIndex();
        Node* parentnode = NodeFromIndex(parent);
        Node* childnode = parentnode->children.value(row);
        if(!childnode)
            return QModelIndex();
        return createIndex(row, col, childnode);
    };

    QModelIndex parent(const QModelIndex &child) const
    {
        Node* node = NodeFromIndex(child);
        if(!node)
            return QModelIndex();
        Node* parentnode = node->parent;
        if(!parentnode)
            return QModelIndex();
        Node* grandparentnode = parentnode->parent;
        if(!grandparentnode)
            return QModelIndex();
        int row = grandparentnode->children.indexOf(parentnode);
        return createIndex(row, 0, parentnode);
    };

    int rowCount(const QModelIndex &parent) const
    {
        if(parent == QModelIndex())
            return rootnode->childcount;
        if(parent.column() > 0)
            return 0;
        Node* parentnode = rootnode; 
        if(parent.isValid())
            parentnode = NodeFromIndex(parent);
        return parentnode->childcount;
    };

    int columnCount(const QModelIndex &parent) const
    {
        return NodeFromIndex(parent)->nodevalues.count();
    };

    QVariant data(const QModelIndex &index, int role) const
    {
        if(index == QModelIndex())
            return QVariant();
        Node* node = rootnode; 
        if(index.isValid())
            node = NodeFromIndex(index);
        if(role == Qt::CheckStateRole)
        {
            if(index.column() == 0)
                return QVariant(GetCheckState(node));
            else
                return QVariant();
        }
        int nodetype = 0;
        int itemtype = 0;
        /*
        QSqlQuery dataquery(fcasedb);
        dataquery.prepare("SELECT objtype, type FROM data WHERE id = ?");
        dataquery.bindValue(0, node->nodevalues.at(0).toULongLong());
        dataquery.exec();
        dataquery.next();
        int nodetype = dataquery.value(0).toInt();
        int itemtype = dataquery.value(1).toInt();
        dataquery.finish();
        */
        //headerdata << "ID" << "Name" << "Full Path" << "Size (bytes)" << "Object Type" << "Address" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash" << "Parent ID" << "Item Type" << "Parent Image ID" << "Parent FS ID" << "Flags" << "File Signature" << "File Category" << "Checked" << "MFT Attribute ID";
        // WILL NEED TO REPLACE ALL THESE CALLS TO THE RESPECTIVE SQL QUERY RATHER THAN THE NODE SINCE I'M NOT STORING IT IN THE NODE ANYMORE...
        if(role == Qt::ForegroundRole)
        {
            if(filtervalues.maxidbool && filtervalues.minidbool == false)
            {
                if(node->nodevalues.at(0).toULongLong() <= filtervalues.maxid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.minidbool && filtervalues.maxidbool == false)
            {
                if(node->nodevalues.at(0).toULongLong() >= filtervalues.minid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxidbool && filtervalues.minidbool)
            {
                if(node->nodevalues.at(0).toULongLong() >= filtervalues.minid || node->nodevalues.at(0).toULongLong() <= filtervalues.maxid)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.namebool)
            {
                if(node->nodevalues.at(1).toString().contains(filtervalues.namefilter) == false)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.pathbool)
            {
                if(node->nodevalues.at(2).toString().contains(filtervalues.pathfilter) == false)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxsizebool && filtervalues.minsizebool == false)
            {
                if(node->nodevalues.at(3).toULongLong() <= filtervalues.maxsize)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.minsizebool && filtervalues.maxsizebool == false)
            {
                if(node->nodevalues.at(3).toULongLong() >= filtervalues.minsize)
                    return QColor(Qt::lightGray);
            }
            if(filtervalues.maxsizebool && filtervalues.minsizebool)
            {
                if(node->nodevalues.at(3).toULongLong() >= filtervalues.minsize || node->nodevalues.at(3).toULongLong() <= filtervalues.maxsize)
                    return QColor(Qt::lightGray);
            }
            //if(node->nodevalues.at(4).toInt() == 5)
            if(nodetype == 5)
            {
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool == false)
                {
                    //if(node->nodevalues.at(6).toInt() <= filtervalues.maxcreate)
                    if(node->nodevalues.at(4).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool == false && filtervalues.mincreatebool)
                {
                    if(node->nodevalues.at(4).toInt() >= filtervalues.mincreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxcreatebool && filtervalues.mincreatebool)
                {
                    if(node->nodevalues.at(4).toInt() >= filtervalues.mincreate || node->nodevalues.at(4).toInt() <= filtervalues.maxcreate)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool == false)
                {
                    //if(node->nodevalues.at(7).toInt() <= filtervalues.maxaccess)
                    if(node->nodevalues.at(5).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool == false && filtervalues.minaccessbool)
                {
                    if(node->nodevalues.at(5).toInt() >= filtervalues.minaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxaccessbool && filtervalues.minaccessbool)
                {
                    if(node->nodevalues.at(5).toInt() >= filtervalues.minaccess || node->nodevalues.at(5).toInt() <= filtervalues.maxaccess)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool == false)
                {
                    //if(node->nodevalues.at(8).toInt() <= filtervalues.maxmodify)
                    if(node->nodevalues.at(6).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool == false && filtervalues.minmodifybool)
                {
                    if(node->nodevalues.at(6).toInt() >= filtervalues.minmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxmodifybool && filtervalues.minmodifybool)
                {
                    if(node->nodevalues.at(6).toInt() >= filtervalues.minmodify || node->nodevalues.at(6).toInt() <= filtervalues.maxmodify)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool == false)
                {
                    //if(node->nodevalues.at(9).toInt() <= filtervalues.maxchange)
                    if(node->nodevalues.at(7).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool == false && filtervalues.minchangebool)
                {
                    if(node->nodevalues.at(7).toInt() >= filtervalues.minchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.maxchangebool && filtervalues.minchangebool)
                {
                    if(node->nodevalues.at(7).toInt() >= filtervalues.minchange || node->nodevalues.at(7).toInt() <= filtervalues.maxchange)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool && filtervalues.filetypebool == false)
                {
                    //if(node->nodevalues.at(16).toString().contains(filtervalues.filecategory) == false)
                    if(node->nodevalues.at(9).toString().contains(filtervalues.filecategory) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool == false && filtervalues.filetypebool)
                {
                    if(node->nodevalues.at(9).toString().contains(filtervalues.filetype) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filecategorybool && filtervalues.filetypebool)
                {
                    if(node->nodevalues.at(9).toString().contains(filtervalues.filecategory) == false || node->nodevalues.at(9).toString().contains(filtervalues.filetype) == false)
                        return QColor(Qt::lightGray);
                }
                if(filtervalues.filegroupbool)
                {
                    //if(node->nodevalues.at(17).toString().contains(filtervalues.filegroup) == false)
                    if(node->nodevalues.at(10).toString().contains(filtervalues.filegroup) == false)
                            return QColor(Qt::lightGray);
                }
                if(filtervalues.hashbool)
                {
                    for(int i=0; i < filtervalues.hashidlist.count(); i++)
                    {
                        if(node->nodevalues.at(0).toULongLong() == filtervalues.hashidlist.at(i))
                            return QColor(Qt::lightGray);
                    }
                }
            }
        }
        if(role == Qt::DisplayRole)
        {
            //if(index.column() == 2)
            //{
                //if(node->nodevalues.at(4).toInt() == 1)
                //if(nodetype == 1)
                //    return QString("");
                //else
                //    return node->nodevalues.at(index.column());
            //}
            if(index.column() >= 4 && index.column() <= 7)
            {
                char buf[128];
                QString tmpstr = QString(TskTimeToStringUTC(node->nodevalues.at(index.column()).toInt(), buf));
                return tmpstr;
            }
            else
                return node->nodevalues.at(index.column());
        }
        if(role == Qt::DecorationRole)
        {
            //int nodetype = node->nodevalues.at(4).toInt();
            QString nodename = node->nodevalues.at(1).toString();
            if(index.column() == 0)
            {
                if(nodetype == 1)
                    return QIcon(QPixmap(QString(":/basic/treeimg")));
                else if(nodetype == 2)
                    return QIcon(QPixmap(QString(":/basic/treevol")));
                else if(nodetype == 3)
                    return QIcon(QPixmap(QString(":/basic/treepart")));
                else if(nodetype == 4)
                    return QIcon(QPixmap(QString(":/basic/treefs")));
                else if(nodetype == 5)
                {
                    //int itemtype = node->nodevalues.at(12).toInt();
                    if(itemtype == 5)
                    {
                        if(nodename.compare("AttrDef") == 0 || nodename.compare("$BadClus") == 0 || nodename.compare("$Bitmap") == 0 || nodename.compare("$Boot") == 0 || nodename.compare("$ObjId") == 0 || nodename.compare("$Quota") == 0 || nodename.compare("$Reparse") == 0 || nodename.compare("$LogFile") == 0 || nodename.compare("$MFT") == 0 || nodename.compare("$MFTMirr") == 0 || nodename.compare("$Secure") == 0 || nodename.compare("$UpCase") == 0 || nodename.compare("$Volume") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfile")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefile")));
                    }
                    else if(itemtype == 3)
                    {
                        if(nodename.compare("$OrphanFiles") == 0 || nodename.compare("$Extend") == 0)
                            return QIcon(QPixmap(QString(":/basic/virtualfolder")));
                        else
                            return QIcon(QPixmap(QString(":/basic/treefolder")));
                    }
                    else if(itemtype == 10)
                        return QIcon(QPixmap(QString(":/basic/virtualfile")));
                    return QIcon(QPixmap(QString(":/basic/treefile")));
                }
                else if(nodetype == 6)
                    return QIcon(QPixmap(QString(":/basic/virtualfile")));
                return QVariant();
            }
            
            return QVariant();
        }

        return QVariant();
    };

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if(index.column() == 0 && role == Qt::CheckStateRole)
        {
            Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
            return SetCheckState(index, state);
        }
        if(role == Qt::DisplayRole)
        {
            /*
            if(value.toInt() == -15)
            {
                Node* parentnode = NodeFromIndex(index);
                QSqlQuery childupdatequery(fcasedb);
                childupdatequery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, FROM data WHERE id = ?");
                //childupdatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ?;");
                for(int i=0; i < parentnode->children.count(); i++)
                {
                    childupdatequery.addBindValue(parentnode->children.at(i)->nodevalues.at(0).toULongLong());
                    childupdatequery.exec();
                    childupdatequery.next();
                    parentnode->children.at(i)->nodevalues[0] = childupdatequery.value(0);
                    parentnode->children.at(i)->nodevalues[1] = childupdatequery.value(1);
                    parentnode->children.at(i)->nodevalues[2] = childupdatequery.value(2);
                    parentnode->children.at(i)->nodevalues[3] = childupdatequery.value(3);
                    parentnode->children.at(i)->nodevalues[4] = childupdatequery.value(4);
                    parentnode->children.at(i)->nodevalues[5] = childupdatequery.value(5);
                    parentnode->children.at(i)->nodevalues[6] = childupdatequery.value(6);
                    parentnode->children.at(i)->nodevalues[7] = childupdatequery.value(7);
                    parentnode->children.at(i)->nodevalues[8] = childupdatequery.value(8);
                    parentnode->children.at(i)->nodevalues[9] = childupdatequery.value(9);
                    parentnode->children.at(i)->nodevalues[10] = QVariant(childupdatequery.value(9).toString().split("/").at(0));
                }
                childupdatequery.finish();
                emit dataChanged(index.child(0, 0), index.child(parentnode->children.count() - 1, 0));
                return true;
            }*/
            /*else
            {*/
                Node* currentnode = NodeFromIndex(index);
                /*
                QSqlQuery updatequery(fcasedb);
                updatequery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, FROM data WHERE id = ?;");
                //updatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ?;");
                updatequery.addBindValue(currentnode->nodevalues.at(0).toULongLong());
                updatequery.exec();
                updatequery.first();
                currentnode->nodevalues[0] = updatequery.value(0);
                currentnode->nodevalues[1] = updatequery.value(1);
                currentnode->nodevalues[2] = updatequery.value(2);
                currentnode->nodevalues[3] = updatequery.value(3);
                currentnode->nodevalues[4] = updatequery.value(4);
                currentnode->nodevalues[5] = updatequery.value(5);
                currentnode->nodevalues[6] = updatequery.value(6);
                currentnode->nodevalues[7] = updatequery.value(7);
                currentnode->nodevalues[8] = updatequery.value(8);
                currentnode->nodevalues[9] = updatequery.value(9);
                currentnode->nodevalues[10] = QVariant(updatequery.value(9).toString().split("/").at(0));
                updatequery.finish();
                */
                //emit dataChanged(index, index);
                return true;
            //}
        }
        return false;
    };

    void dataChanged(const QModelIndex &topleftindex, const QModelIndex &botrightindex, const QVector<int> &roles = QVector<int>())
    {
        /*
        foreach(int role, roles)
        {
        if(role == Qt::DisplayRole)
        {
            Node* startnode = NodeFromIndex(topleftindex);
            Node* endnode = NodeFromIndex(botrightindex);
            QSqlQuery updatequery(fcasedb);
            updatequery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, FROM data WHERE id = ? OR id = ?;");
            //updatequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid == ? OR objectid == ?;");
            updatequery.addBindValue(startnode->nodevalues.at(0).toULongLong());
            updatequery.exec();
            updatequery.next();
            startnode->nodevalues[8] = updatequery.value(8).toString();
            updatequery.addBindValue(endnode->nodevalues.at(0).toULongLong());
            updatequery.exec();
            updatequery.next();
            endnode->nodevalues[8] = updatequery.value(8).toString();
            updatequery.finish();
        }
        }*/
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if(section >= 0)
                return headerdata.at(section);
        }
        if(role == Qt::DecorationRole)
        {
            if(section == 0 && (filtervalues.maxidbool || filtervalues.minidbool))
            {
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            }
            if(section == 1 && filtervalues.namebool)
            {
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            }
            if(section == 2 && filtervalues.pathbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 3 && (filtervalues.maxsizebool || filtervalues.minsizebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 4 && (filtervalues.maxcreatebool || filtervalues.mincreatebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 5 && (filtervalues.maxaccessbool || filtervalues.minaccessbool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 6 && (filtervalues.maxmodifybool || filtervalues.minmodifybool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 7 && (filtervalues.maxchangebool || filtervalues.minchangebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 9 && (filtervalues.filecategorybool || filtervalues.filetypebool))
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 8 && filtervalues.hashbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
            if(section == 10 && filtervalues.filegroupbool)
                return QIcon(QPixmap(QString(":/basic/filterimg")));
        }
        return QVariant();
    };

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);

        if(!index.isValid())
            return Qt::NoItemFlags;
        if(index == QModelIndex())
            return Qt::NoItemFlags;
        if(index.column() == 0)
        {
            flags |= Qt::ItemIsUserCheckable;
            if(index.model()->hasChildren(index))
                flags |= Qt::ItemIsTristate;
        }
        
        return flags;
    };

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent == QModelIndex())
            return true;
        parentnode = rootnode;
        if(parent.isValid())
            parentnode = NodeFromIndex(parent);
        if(rowCount(parent) > 0)
        {
            return true;
        }
        return false;
    };


    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const
    {
        if(parent == QModelIndex())
            return false;
        Node* parentnode = NodeFromIndex(parent);
        if(parentnode == rootnode)
            return false;
        if((unsigned long long)parentnode->children.count() < parentnode->childcount && parentnode->haschildren == true)
            return true;
        return false;
    };

    void fetchMore(const QModelIndex &parent = QModelIndex())
    {
        Node* parentnode = NodeFromIndex(parent);
        QList<QVariant> fetchvalues;
        fetchvalues.clear();
        if(parentnode->haschildren == true)
        {
            /*
            QSqlQuery prequery(fcasedb);
            prequery.prepare("SELECT addr, parimgid, parfsid FROM data WHERE id = ?");
            prequery.bindValue(0, parentnode->nodevalues.at(0).toULongLong());
            //qDebug() << "curid:" << parentnode->nodevalues.at(0).toULongLong();
            prequery.exec();
            prequery.first();
            unsigned long long parentaddress = prequery.value(0).toULongLong();
            unsigned long long parentimgid = prequery.value(1).toULongLong();
            unsigned long long parentfsid = prequery.value(2).toULongLong();
            if(parentfsid == 0)
                parentfsid = parentnode->nodevalues.at(0).toULongLong();
            prequery.finish();

            QSqlQuery fetchquery(fcasedb);
            fetchquery.prepare("SELECT id, name, fullpath, size, crtime, atime, mtime, ctime, md5, filemime, checked, addr, parimgid, parfsid FROM data WHERE (objtype = 5 OR objtype = 6) AND parid = ? AND parimgid = ? AND parfsid = ?;");
            //qDebug() << "parid:" << parentaddress << "parimgid:" << parentimgid << "parfsid:" << parentfsid;
            fetchquery.bindValue(0, parentaddress);
            fetchquery.bindValue(1, parentimgid);
            fetchquery.bindValue(2, parentfsid);
            if(fetchquery.exec())
            {
                beginInsertRows(parent, 0, parentnode->childcount - 1);
                while(fetchquery.next())
                {
                    fetchvalues.clear();
                    fetchvalues.append(fetchquery.value(0));
                    fetchvalues.append(fetchquery.value(1));
                    fetchvalues.append(fetchquery.value(2));
                    fetchvalues.append(fetchquery.value(3));
                    fetchvalues.append(fetchquery.value(4));
                    fetchvalues.append(fetchquery.value(5));
                    fetchvalues.append(fetchquery.value(6));
                    fetchvalues.append(fetchquery.value(7));
                    fetchvalues.append(fetchquery.value(8));
                    fetchvalues.append(fetchquery.value(9));
                    fetchvalues.append(QVariant(fetchquery.value(9).toString().split("/").at(0)));
                    Node* curchild = new Node(fetchvalues);
                    curchild->parent = parentnode;
                    if(QString(".").compare(curchild->nodevalues.at(1).toString()) == 0 || QString("..").compare(curchild->nodevalues.at(1).toString()) == 0)
                    {
                        curchild->childcount = 0;
                        curchild->haschildren = false;
                    }
                    else
                    {
                        curchild->childcount = GetChildCount(5, fetchquery.value(11).toULongLong(), fetchquery.value(12).toULongLong(), fetchquery.value(13).toULongLong());
                        curchild->haschildren = curchild->HasChildren();
                    }
                    if(fetchquery.value(10).toInt() == 0)
                        curchild->checkstate = 0;
                    else if(fetchquery.value(10).toInt() == 1)
                        curchild->checkstate = 1;
                    else
                        curchild->checkstate = 2;
                    parentnode->children.append(curchild);
                }
                endInsertRows();
                emit checkedNodesChanged();
                //setData(parent, QVariant(-15), Qt::DisplayRole);
            }
            fetchquery.finish();
            */


            /*
            QSqlQuery morequery(fcasedb);
            morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6) AND parentid = ? AND parimgid = ? AND parfsid = ?");
            //morequery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE (objecttype = 5 OR objecttype = 6) AND parentid = ? AND parimgid = ? AND parfsid = ?");
            morequery.addBindValue(parentnode->nodevalues.at(5).toULongLong());
            morequery.addBindValue(parentnode->nodevalues.at(13).toULongLong());
            if(parentnode->nodevalues.at(4).toInt() == 4)
                morequery.addBindValue(parentnode->nodevalues.at(0).toULongLong());
            else
                morequery.addBindValue(parentnode->nodevalues.at(14).toULongLong());
            //morequery.addBindValue(parentnode->nodevalues.at(0).toULongLong());
            if(morequery.exec())
            {
                beginInsertRows(parent, 0, parentnode->childcount - 1);
                while(morequery.next())
                {
                    fetchvalues.clear();
                    for(int i=0; i < morequery.record().count(); i++)
                        fetchvalues.append(morequery.value(i));
                    Node* curchild = new Node(fetchvalues);
                    curchild->parent = parentnode;
                    if(QString(".").compare(curchild->nodevalues.at(1).toString()) == 0 || QString("..").compare(curchild->nodevalues.at(1).toString()) == 0)
                    {
                        curchild->childcount = 0;
                        curchild->haschildren = false;
                    }
                    else
                    {
                        curchild->childcount = GetChildCount(5, curchild->nodevalues.at(5).toULongLong(), curchild->nodevalues.at(13).toULongLong(), curchild->nodevalues.at(14).toULongLong());
                        curchild->haschildren = curchild->HasChildren();
                    }
                    if(morequery.value(18).toInt() == 0)
                        curchild->checkstate = 0;
                    else if(morequery.value(18).toInt() == 1)
                        curchild->checkstate = 1;
                    else
                        curchild->checkstate = 2;
                    parentnode->children.append(curchild);
                }
                endInsertRows();
                emit checkedNodesChanged();
                setData(parent, QVariant(-15), Qt::DisplayRole);

            }
            */
        }
    };
    
    void GetModelCount(Node* curnode)
    {
        //if(curnode->nodevalues.at(4).toInt() == 5 || curnode->nodevalues.at(4).toInt() == 6)
        //{
            totalcount++;
            if(curnode->checkstate == 2)
                totalchecked++;
        //}
        if(curnode->haschildren)
        {
            for(int i=0; i < curnode->children.count(); i++)
            {
                GetModelCount(curnode->children[i]);
            }
        }
    };

    void RemEvidence(unsigned long long curid)
    {
        int rownumber = rootnode->GetChildRow(curid);
        beginRemoveRows(QModelIndex(), rownumber, rownumber);
        rootnode->RemoveChild(rownumber);
        rootnode->childcount--;
        endRemoveRows();
    };

    void AddEvidence(unsigned long long curid)
    {
        /*
        int filesystemcount;
        QSqlQuery addevidquery(fcasedb);
        addevidquery.prepare("SELECT id, name, fullpath, size, objtype, addr, crtime, atime, mtime, ctime, md5, parid, type, parimgid, parfsid, filemime, checked FROM data WHERE id = ? OR (objtype < 6 AND parimgid = ?)");
        //addevidquery.prepare("SELECT objectid, name, fullpath, size, objecttype, address, crtime, atime, mtime, ctime, md5, parentid, type, parimgid, parfsid, flags, filemime, filesignature, checked, mftattrid FROM data WHERE objectid = ? OR (objecttype < 5 AND parimgid = ?)");
        addevidquery.addBindValue(curid);
        addevidquery.addBindValue(curid);
        if(addevidquery.exec())
        {
            beginInsertRows(QModelIndex(), rootnode->childcount, rootnode->childcount);
            while(addevidquery.next())
            {
                currentnode = 0;
                colvalues.clear();
                colvalues.append(addevidquery.value(0));
                colvalues.append(addevidquery.value(1));
                colvalues.append(addevidquery.value(2));
                colvalues.append(addevidquery.value(3));
                colvalues.append(addevidquery.value(6));
                colvalues.append(addevidquery.value(7));
                colvalues.append(addevidquery.value(8));
                colvalues.append(addevidquery.value(9));
                colvalues.append(addevidquery.value(10));
                colvalues.append(addevidquery.value(15));
                colvalues.append(addevidquery.value(15).toString().split("/").at(0));
                //for(int i=0; i < addevidquery.record().count(); i++)
                //    colvalues.append(addevidquery.value(i));
                currentnode = new Node(colvalues);
                //if(currentnode->nodevalues.at(4).toInt() == 1) // image file
                if(addevidquery.value(4).toInt() == 1) // image file
                {
                    filesystemcount = 0;
                    rootnode->children.append(currentnode);
                    rootnode->childcount++;
                    rootnode->haschildren = rootnode->HasChildren();
                    currentnode->parent = rootnode;
                    currentnode->childcount = GetChildCount(1, currentnode->nodevalues.at(0).toULongLong());
                    currentnode->haschildren = currentnode->HasChildren();
                    parentnode = currentnode;
                }
                else if(addevidquery.value(4).toInt() == 2) // volume //else if(currentnode->nodevalues.at(4).toInt() == 2) // volume
                {
                    currentnode->parent = parentnode;
                    parentnode->children.append(currentnode);
                    currentnode->childcount = GetChildCount(2, currentnode->nodevalues.at(0).toULongLong(), curid);
                    currentnode->haschildren = currentnode->HasChildren();
                    parentnode = currentnode;
                }
                else if(addevidquery.value(4).toInt() == 3) //else if(currentnode->nodevalues.at(4).toInt() == 3) // determine if its an unallocated partition space
                {
                    //if(addevidquery.value(15).toInt() == 2) //if(currentnode->nodevalues.at(15).toInt() == 2) // unallocated partition, add to parent as a child.
                    //{
                    currentnode->parent = parentnode;
                    parentnode->children.append(currentnode); 
                    //}
                }
                // THERE SHOULD ONLY BE PARTITION OBJECTS, OBJECTTYPE == 3, FOR UNALLOCATED SPACES....
                else if(addevidquery.value(4).toInt() == 4) //else if(currentnode->nodevalues.at(4).toInt() == 4) // filesystem
                {
                    currentnode->parent = parentnode;
                    parentnode->children.append(currentnode);
                    //currentnode->childcount = GetChildCount(4, addevidquery.value(5).toULongLong(), curid, currentnode->nodevalues.at(0).toULongLong());
                    if(filesystemcount <= fsobjectlist.count())
                    {
                        currentnode->childcount = GetChildCount(4, fsobjectlist.at(filesystemcount).rootinum, curid, currentnode->nodevalues.at(0).toULongLong());
                        filesystemcount++;
                    }
                    currentnode->haschildren = currentnode->HasChildren();
                }
                else if(addevidquery.value(4).toInt() == 5 || addevidquery.value(4).toInt() == 6) // file at rootinum...
                {
                    QSqlQuery filequery(fcasedb);
                    Node* rootdirectory = 0;
                    for(int j=0; j < fsobjectlist.count(); j++)
                    {
                        filequery.prepare("SELECT parfsid FROM data WHERE (objtype = 5 OR objtype = 6) AND parimgid = ? AND parid = ? AND parfsid = ?)");
                        filequery.addBindValue(curid);
                        filequery.addBindValue(fsobjectlist.at(j).rootinum);
                        filequery.addBindValue(fsobjectlist.at(j).id);
                        if(filequery.exec())
                        {
                            while(filequery.next())
                            {
                                for(int i=0; i < parentnode->children.count(); i++)
                                {
                                    if(filequery.value(0).toULongLong() == parentnode->children.at(i)->nodevalues.at(0).toULongLong())
                                        rootdirectory = parentnode->children.at(i);
                                }
                                currentnode->parent = rootdirectory;
                                if(QString(".").compare(currentnode->nodevalues.at(1).toString()) == 0 || QString("..").compare(currentnode->nodevalues.at(1).toString()) == 0)
                                {
                                    currentnode->childcount = 0;
                                    currentnode->haschildren = false;
                                }
                                else
                                {
                                    currentnode->childcount = GetChildCount(5, addevidquery.value(5).toULongLong(), curid, addevidquery.value(15).toULongLong());
                                    //currentnode->childcount = GetChildCount(5, currentnode->nodevalues.at(5).toULongLong(), curid, currentnode->nodevalues.at(14).toULongLong());
                                    currentnode->haschildren = currentnode->HasChildren();
                                }       
                                rootdirectory->children.append(currentnode);
                            }
                        }
                        filequery.finish();
                    }
                }
                if(addevidquery.value(16).toInt() == 0)
                    currentnode->checkstate = 0;
                else if(addevidquery.value(16).toInt() == 1)
                    currentnode->checkstate = 1;
                else
                    currentnode->checkstate = 2;
            }
            endInsertRows();
            emit checkedNodesChanged();
        }
        addevidquery.finish();
        */
    };

    Node* NodeFromIndex(const QModelIndex &index) const
    {
        if(index.isValid())
            return static_cast<Node*>(index.internalPointer());
        else
            return rootnode;
    };
 
signals:
    void checkedNodesChanged();

private:
    Qt::CheckState GetCheckState(Node* curnode) const
    {
        if(curnode->checkstate == 0) // unchecked
            return Qt::Unchecked;
        else if(curnode->checkstate == 1) // partially checked
            return Qt::PartiallyChecked;
        else if(curnode->checkstate == 2) // checked
            return Qt::Checked;
        return Qt::Unchecked;
    };

    void SetParentCheckState(const QModelIndex &index)
    {
        Node* curnode = NodeFromIndex(index);
        unsigned long long checkcount = 0;
        for(int i=0; i < curnode->children.count(); i++)
        {
            if(curnode->children[i]->checkstate == 2 || curnode->children[i]->checkstate == 1)
                checkcount++;
        }
        if(curnode->childcount > checkcount && checkcount > 0)
        {
            curnode->checkstate = 1;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 1;
        }
        else if(curnode->childcount == checkcount)
        {
            curnode->checkstate = 1;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 1;
        }
        else if(checkcount == 0)
        {
            curnode->checkstate = 0;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 0;
        }
        //emit dataChanged(index, index);
        emit checkedNodesChanged();
        if(curnode->parent != 0)
            SetParentCheckState(index.parent());
    };

    void SetChildCheckState(const QModelIndex &index)
    {
        Node* curnode = NodeFromIndex(index);
        for(int i=0; i < curnode->children.count(); i++)
        {
            curnode->children[i]->checkstate = curnode->checkstate;
            //emit dataChanged(index.child(i, 0), index.child(i, 0));
            if(curnode->children[i]->haschildren)
                SetChildCheckState(index.child(i,0));
        }
        emit checkedNodesChanged();
    };

    bool SetCheckState(const QModelIndex &index, Qt::CheckState state)
    {
        Node* curnode = NodeFromIndex(index);
        if(state == Qt::Unchecked) // curnode is now unchecked...
        {
            curnode->checkstate = 0;
            if(curnode->haschildren)
                SetChildCheckState(index);
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 0;
        }
        else if(state == Qt::PartiallyChecked) // curnode is now partially checked
        {
            curnode->checkstate = 1;
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 1;
        }
        else if(state == Qt::Checked) // currentnode is now checked
        {
            curnode->checkstate = 2;
            if(curnode->haschildren)
                SetChildCheckState(index);
            checkhash[curnode->nodevalues.at(0).toULongLong()] = 2;
        }
        //emit dataChanged(index, index);
        emit checkedNodesChanged();
        if(curnode->parent != 0)
            SetParentCheckState(index.parent());
        return true;
    };

    QStringList headerdata;
};

namespace Ui {
class WombatForensics;
}

class WombatForensics : public QMainWindow
{
    Q_OBJECT

public:
    explicit WombatForensics(QWidget *parent = 0);
    ~WombatForensics();
    //WombatDatabase* wombatdatabase;
    //WombatVariable* wombatvarptr;
    TskObject tskobject;
    TskObject* tskobjptr;
    TskObject tskexternalobject;
    TskObject* tskexternalptr;
    //WombatFramework* wombatframework;
    //PropertiesWindow* propertywindow;
    ExportDialog* exportdialog;
    DigDeeperDialog* digdeeperdialog;
    FileViewer* fileviewer;
    TreeModel* treemodel;
    QMenu* treemenu;
    QMenu* selectionmenu;
    IdFilter* idfilterview;
    JumpFilter* jumpfilterview;
    NameFilter* namefilterview;
    PathFilter* pathfilterview;
    SizeFilter* sizefilterview;
    CreatedDateFilter* createfilterview;
    AccessedDateFilter* accessfilterview;
    ModifiedDateFilter* modifyfilterview;
    ChangedDateFilter* changefilterview;
    FileTypeFilter* filetypefilterview;
    FileCategoryFilter* filecategoryfilterview;
    HashFilter* hashfilterview;
    ImageViewer* imagewindow;
    VideoViewer* videowindow;
    ViewerManager* viewmanage;
    TextViewer* textviewer;
    HtmlViewer* htmlviewer;
    MessageViewer* msgviewer;
    ByteConverter* byteviewer;
    AboutBox* aboutbox;
    CancelThread* cancelthread;

signals:

private slots:
    void AddEvidence();
    void RemEvidence();
    void ExportEvidence();
    void on_actionNew_Case_triggered();
    void on_actionOpen_Case_triggered();
    void on_actionSaveState_triggered();
    void on_actionCheck_triggered();
    void on_actionExport_triggered();
    void on_actionDigDeeper_triggered();
    void on_actionView_Properties_triggered(bool checked);
    void on_actionView_File_triggered(bool checked);
    void on_actionView_Image_Gallery_triggered(bool checked);
    void on_actionViewerManager_triggered();
    void on_actionTextViewer_triggered(bool checked);
    void on_actionViewMessageLog_triggered(bool checked);
    void on_actionByteConverter_triggered(bool checked);
    void on_actionCopy_Selection_To_triggered();
    void on_actionExpandAll_triggered();
    void on_actionCollapseAll_triggered();
    void on_actionAbout_triggered();
    void UpdateProgress(unsigned long long count, unsigned long long processcount);
    void SelectionChanged(const QItemSelection &selitem, const QItemSelection &deselitem);
    void HidePropertyWindow(bool checkstate);
    void HideFileViewer(bool checkstate);
    void HideImageWindow(bool checkstate);
    void HideViewerManager(void);
    void HideTextViewer(bool checkstate);
    void HideMessageViewer(bool checkstate);
    void HideByteViewer(bool checkstate);
    void ShowExternalViewer();
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
    void ResizeColumns(void);
    void OpenParentImage(unsigned long long imgid);
    void OpenParentFileSystem(unsigned long long fsid);
    void OpenFileSystemFile(void);
    void ResizeViewColumns(const QModelIndex &index)
    {
        if(index.isValid())
            ResizeColumns();
    };
    void ExpandCollapseResize(const QModelIndex &index)
    {
        if(((TreeModel*)ui->dirTreeView->model())->canFetchMore(index))
        {
            ((TreeModel*)ui->dirTreeView->model())->fetchMore(index);
        }
        ResizeViewColumns(index);
    };
    void FileExport(FileExportData* exportdata);
    void FileDig(FileDeepData* deeperdata);
    void SetOffsetLabel(off_t pos);
    void ResetSlider(void);
    void ShowRockerToolTip(int moved);
    void SkipDown(void);
    void SkipUp(void);
    void PageUp(void);
    void PageDown(void);
    void UpdateSelectValue(const QString &txt);
    void InitializeQueryModel(void);
    void UpdateDataTable(void);
    void UpdateStatus(void);
    void UpdateDigging(void);
    void FinishExport(void);
    void FinishRemoval(void);
    void FinishThumbs(void);
    void StatusUpdate(QString tmptext)
    {
        statuslabel->setText(tmptext);
    };
    void TreeContextMenu(const QPoint &point);
    void ImgHexMenu(const QPoint &point);
    void SetFilter(int headercolumn);
    void FilterApplied()
    {
        UpdateFilterCount();
        emit ui->dirTreeView->header()->geometriesChanged();
    };
    void NextItem();
    void PreviousItem();
    void ShowItem();
    void UpdateThumbnails(int tsize);
    void SetSelectedFromImageViewer(unsigned long long selectedid);
    void ShowFile(const QModelIndex &index);
    void AddSection(void);
    void AddTextSection(void);
    void CarveFile(void);
    void AutoSaveState(void);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
private:
    Ui::WombatForensics *ui;

    void SetupHexPage(void);
    void InitializeAppStructure(void);
    void InitializeCaseStructure(void);
    void InitializeEvidenceStructure(void);
    void InitializeOpenCase(void);
    void CloseCurrentCase(void);
    void UpdateProperties(void);
    void LoadHexContents(void);
    //void SecondaryProcessing(void);
    void OpenEvidenceStructure(void);
    void StartThumbnails(void);
    void ExportFiles(FileExportData* exportdata);
    void DigFiles(FileDeepData* deepdata);
    void GetExportData(Node* curnode, FileExportData* exportdata);
    void GetDigData(Node* curnode, FileDeepData* deepdata);
    void ProcessDig(TskObject curobject, unsigned long long objectid, std::vector<FileDeepData::DigOptions> digoptions);
    void ProcessExport(TskObject curobject, std::string fullpath, std::string name);
    void UpdateFilterCount(void);
    void SaveState(void);
    void CreateAppDB(void);
    void OpenAppDB(void);
    unsigned long long ReturnCaseCount(void);
    void InsertCase(void);
    void CreateThumbDB(void);
    void OpenThumbDB(void);
    void CreateCaseDB(void);
    void OpenCaseDB(void);
    void AddNewEvidence(void);
    QString GetFileSystemLabel(TSK_FS_INFO* fsinfo);
    void RemoveTmpFiles(void);
    //void GetEvidenceObjects(void);

    uint8_t hfs_cat_file_lookup(HFS_INFO* hfs, TSK_INUM_T inum, HFS_ENTRY* entry, unsigned char follow_hard_link);
    uint8_t hfs_UTF16toUTF8(TSK_FS_INFO* fs, uint8_t* uni, int ulen, char* asc, int alen, uint32_t flags);
    static uint8_t hfs_cat_get_record_offset_cb(HFS_INFO* hfs, int8_t level_type, const void* targ_data, const hfs_btree_key_cat* cur_key, TSK_OFF_T key_off, void* ptr);
    static TSK_OFF_T hfs_cat_get_record_offset(HFS_INFO* hfs, const hfs_btree_key_cat* needle);
    uint8_t hfs_cat_read_thread_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_thread* thread);
    uint8_t hfs_cat_read_file_folder_record(HFS_INFO* hfs, TSK_OFF_T off, hfs_file_folder* record);
    static int hfs_cat_compare_keys(HFS_INFO* hfs, const hfs_btree_key_cat* key1, const hfs_btree_key_cat* key2);
    static uint8_t hfs_cat_traverse(HFS_INFO* hfs, const void* targ_data, TSK_HFS_BTREE_CB a_cb, void* ptr);
    QModelIndex selectedindex;
    QModelIndex oldselectedindex;

    QThread* evidencethread;
    EvidenceWorker* evidenceworker;
    QFuture<void> sqlfuture;
    QFutureWatcher<void> sqlwatcher;
    //QFuture<void> secondfuture;
    //QFutureWatcher<void> secondwatcher;
    QFuture<void> thumbfuture;
    QFutureWatcher<void> thumbwatcher;
    QFuture<void> exportfuture;
    QFutureWatcher<void> exportwatcher;
    QFutureWatcher<void> digwatcher;
    QFuture<void> remfuture;
    QFutureWatcher<void> remwatcher;

    QFile casesfile;
    QFile settingsfile;
    QFile viewerfile;
    QFile casedatafile;
    off_t offset() const;
    ImageHexViewer* hexwidget;
    WombatSlider* hexrocker;
    QPushButton* lineup;
    QPushButton* linedown;
    QPushButton* pageup;
    QPushButton* pagedown;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* filecountlabel;
    QLabel* filtercountlabel;
    QLabel* processcountlabel;
    QLabel* statuslabel;
    QFrame* vline1;
    QFrame* vline2;
    QVector<FileExportData> exportfilelist;
    QVector<FileDeepData> digfilelist;
    //QVector<QVariantMap> jsonstorevector;
    //QVector<SecondaryProcessObject> secondprocessvector;
    QShortcut* jumpforward;
    QShortcut* jumpbackward;
    QShortcut* showitem;
    QTimer* autosavetimer;
    //unsigned long long currentcaseid;
    WombatVariable wombatvariable; // possibly need to make this global...
    TSK_IMG_INFO* readimginfo;
    TSK_VS_INFO* readvsinfo;
    const TSK_VS_PART_INFO* readpartinfo;
    TSK_FS_INFO* readfsinfo;
    TSK_FS_FILE* readfileinfo;
    char asc[512];
    iso9660_pvd_node* p;
    HFS_INFO* hfs;
};

//void SecondaryProcessing(QVariantMap &jsonstore);
//void SecondaryProcessing(SecondaryProcessObject &secprocobj);

#endif // WOMBATFORENSICS_H
