#include "wombatframework.h"

WombatFramework::WombatFramework(WombatVariable* wombatvariable)
{
    wombatptr = wombatvariable;
}
WombatFramework::~WombatFramework()
{
}

void WombatFramework::OpenEvidenceImage() // open current evidence image
{
    const TSK_TCHAR** images;
    qDebug()  << "fullpathvector count: " << wombatptr->evidenceobject.fullpathvector.size();
    images = (const char**)malloc(wombatptr->evidenceobject.fullpathvector.size()*sizeof(char*));
    for(int i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        qDebug() << "fullpathvector[" << i << "] = " << QString::fromStdString(wombatptr->evidenceobject.fullpathvector[i]);
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
        qDebug() << "images[" << i << "] = " << images[i];
    }
    wombatptr->evidenceobject.imageinfo = tsk_img_open(wombatptr->evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(wombatptr->evidenceobject.imageinfo == NULL)
        qDebug() << "print image error here";
    free(images);
}

void WombatFramework::AddEvidenceNodes() // add evidence node to the dirmodel
{
    wombatptr->dirmodel->clear();
    QStringList headerList;
    headerList << "Name" << "Unique ID" << "Full Path" << "Size (Bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    wombatptr->dirmodel->setHorizontalHeaderLabels(headerList);
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        QList<QStandardItem*> tmplist;
        QStandardItem* tmpnode = new QStandardItem(QString::number(wombatptr->evidenceobjectvector[i].id));
        tmpnode->setCheckable(true);
        tmpnode->setIcon(QIcon(":/basic/treeimage"));
        tmplist << tmpnode << new QStandardItem(wombatptr->evidenceobjectvector[i].name);
        wombatptr->dirmodel->invisibleRootItem()->appendRow(tmplist);
        AddPartitionNodes(i);
    }
}

void WombatFramework::AddPartitionNodes(int increment) // add partition/fs nodes to the image node
{
    QList<QStandardItem*> evidnode = wombatptr->dirmodel->findItems(QString::number(wombatptr->evidenceobjectvector[increment].id), Qt::MatchExactly, 0);
    QList<QStandardItem*> tmplist;
    QStandardItem* tmpnode;
    QString tmpstring = "";
    for(int j=0; j < wombatptr->volumeobjectvector.count(); j++)
    {
        if(wombatptr->evidenceobjectvector[increment].id == wombatptr->volumeobjectvector[j].parentid)
        {
            if(wombatptr->volumeobjectvector[j].childcount > 0)
            {
                for(int i=0; i < wombatptr->partitionobjectvector.count(); i++)
                {
                    if(wombatptr->volumeobjectvector[j].id == wombatptr->partitionobjectvector[i].parentid)
                    {
                        tmplist.clear();
                        tmpstring = "";
                        tmpnode = NULL;
                        tmpnode = new QStandardItem(QString::number(wombatptr->partitionobjectvector[i].id));
                        tmpnode->setCheckable(true);
                        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
                        int bytestart = wombatptr->partitionobjectvector[i].sectstart * wombatptr->partitionobjectvector[i].blocksize;
                        int byteend = wombatptr->partitionobjectvector[i].blocksize * (wombatptr->partitionobjectvector[i].sectstart + wombatptr->partitionobjectvector[i].sectlength) - 1;
                        QString tmpstring = wombatptr->partitionobjectvector[i].name + " [" + QString::number(bytestart) + "-" + QString::number(byteend) + "]";
                        tmplist << tmpnode << new QStandardItem(tmpstring);
                        evidnode[0]->appendRow(tmplist);
                    }
                }
            }
            else
            {
                for(int i=0; i < wombatptr->filesystemobjectvector.count(); i++)
                {
                    if(wombatptr->volumeobjectvector[j].id == wombatptr->filesystemobjectvector[i].parentid)
                    {
                        tmplist.clear();
                        tmpstring = "";
                        tmpnode = NULL;
                        tmpnode = new QStandardItem(QString::number(wombatptr->filesystemobjectvector[i].id));
                        tmpnode->setCheckable(true);
                        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
                        int byteend = wombatptr->filesystemobjectvector[i].blocksize * wombatptr->filesystemobjectvector[i].blockcount;
                        QString tmpstring = wombatptr->filesystemobjectvector[i].name + " [" + QString::number(wombatptr->filesystemobjectvector[i].byteoffset) + "-" + QString::number(byteend) + "]";
                        tmplist << tmpnode << new QStandardItem(tmpstring);
                        evidnode[0]->appendRow(tmplist);
                    }
                    else
                        qDebug() << "fs if failed";
                }
            }
        }
    }
}

/*
 * LoadChildren(qstandarditem* item, selected.id)
 * if(selected.id.childcount > 0)
 * fileobjectvector<fileobject>, fileobject has the respective file data including childcount and parentid.
 * expand a parent, children need to looped so they'yre children can be loaded.
 * need to determine what fileobject will contain.
 * loop over the nodes where parentid = selected.id
 * add nodes
 *
void WombatFramework::AddFileNodes(int curidx)
{
    QList<QStandardItem*>
}
 */ 

void WombatFramework::OpenVolumeSystem() // open current volume system
{
    wombatptr->evidenceobject.volinfo = tsk_vs_open(wombatptr->evidenceobject.imageinfo, 0, TSK_VS_TYPE_DETECT);
}

void WombatFramework::GetVolumeSystemName() // get the volume system name
{
    if(wombatptr->evidenceobject.volinfo == NULL)
        wombatptr->currentvolumename = "Dummy Volume";
    else
        wombatptr->currentvolumename = QString::fromUtf8(tsk_vs_type_todesc(wombatptr->evidenceobject.volinfo->vstype));
}

void WombatFramework::OpenPartitions() // open the partitions in the volume
{
    wombatptr->evidenceobject.fsinfovector.clear();
    // CHECK IF VOLUME EXISTS, IF NOT THEN SHOULD GET FS FROM IMG INSTEAD OF VOL.
    if(wombatptr->evidenceobject.volinfo == NULL)
    {
        wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_img(wombatptr->evidenceobject.imageinfo, 0, TSK_FS_TYPE_DETECT));
    }
    else
    {
        if(wombatptr->evidenceobject.volinfo->part_count > 0)
        {
            for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
            {
                wombatptr->evidenceobject.partinfovector.push_back(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i));
                TSK_FS_INFO* tmpfsinfo = tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT);
                if(tmpfsinfo != NULL)
                {
                    wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT));
                }
                else
                {
                }
            }
        }
    }
}

void WombatFramework::OpenFiles() // open the files and add to file info vector
{
    // tsk_fs_dir_walk(TSK_FS_INFO* fsinfovector, TSK_INUM_T a_addr, TSK_FS_DIR_WALK_FLAG_ENUM, TSK_FS_DIR_WALK_CB callfunction, void* ptr to data that is passed to the callback
    //
    // tsk_fs_dir_walk the fsinfovector[i] objects to get the tsk_fs_file and tsk_fs_dir info objects and add them to the
    // fileinfovector
}

void WombatFramework::OpenEvidenceImages() // open all evidence images.
{
    for(int j = 0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        const TSK_TCHAR** images;
        images = (const char**)malloc(wombatptr->evidenceobjectvector[j].fullpathvector.size()*sizeof(char*));
        for(int i=0; i < wombatptr->evidenceobjectvector[j].fullpathvector.size(); i++)
        {
            images[i] = wombatptr->evidenceobjectvector[j].fullpathvector[i].c_str();
        }
        wombatptr->evidenceobjectvector[j].imageinfo = tsk_img_open(wombatptr->evidenceobjectvector[j].itemcount, images, TSK_IMG_TYPE_DETECT, 0);
        free(images);
    }
}

void WombatFramework::CloseInfoStructures() // close all open info structures
{
    for(int j=0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        for(int i=0; i < wombatptr->evidenceobjectvector[j].fsinfovector.size(); i++)
        {
            if(wombatptr->evidenceobjectvector[j].fsinfovector[i] != NULL)
            {
                tsk_fs_close(wombatptr->evidenceobjectvector[j].fsinfovector[i]);
                wombatptr->evidenceobjectvector[j].fsinfovector[i] = NULL;
            }
        }
        wombatptr->evidenceobjectvector[j].fsinfovector.clear();
        for(int i=0; i < wombatptr->evidenceobject.partinfovector.size(); i++)
        {
            wombatptr->evidenceobjectvector[j].partinfovector[i] = NULL;
        }
        wombatptr->evidenceobjectvector[j].partinfovector.clear();
        if(wombatptr->evidenceobjectvector[j].volinfo != NULL)
        {
            tsk_vs_close(wombatptr->evidenceobjectvector[j].volinfo);
            wombatptr->evidenceobjectvector[j].volinfo = NULL;
        }
        if(wombatptr->evidenceobjectvector[j].imageinfo != NULL)
        {
            tsk_img_close(wombatptr->evidenceobjectvector[j].imageinfo);
            wombatptr->evidenceobjectvector[j].imageinfo = NULL;
        }
    }
}

void WombatFramework::GetBootCode(int idx) // deermine boot type and populate variable if exists otherwise populate wiht negative
{
    int volidx = -1;
    for(int i=0; i < wombatptr->volumeobjectvector.count(); i++)
    {
        if(wombatptr->selectedobject.id == wombatptr->volumeobjectvector[i].parentid) // boot volume exists
            volidx = i;
    }
    if(volidx > -1) // boot volume exists
    {
        wombatptr->htmlcontent += "<tr><td class='property'>byte offset</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->volumeobjectvector[volidx].byteoffset) + "</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>block size</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->volumeobjectvector[volidx].blocksize) + " bytes</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>endian ordering</td><td class='pvalue'>";
        /*
        if(wombatptr->evidenceobject.volinfo->endian == TSK_LIT_ENDIAN)
            wombatptr->htmlcontent += "little endian";
        else
            wombatptr->htmlcontent += "big endian";
        */
        wombatptr->htmlcontent += "</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>partition count</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->volumeobjectvector[volidx].childcount) + "</td></tr></table>";
        //wombatptr->htmlcontent += "<br/><br/><div class='tabletitle'>boot sector</div>";
        //wombatptr->htmlcontent += "<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr>";
        //wombatptr->htmlcontent += "<tr class='odd'><td>0-0</td><td class='bvalue'></td><td class='desc'></td></tr>";
        //wombatptr->htmlcontent += "<tr class='odd'><td colspan='3' class='bot'></td></tr></table>";
        // populate the boot sector table
        // PARTITION INFORMATION I NEED TO RECORD: ACTIVE/BOOTABLE, PARTITION TYPE, LBA OF FIRST ABSOLUTE SECTOR OF PARTITION,# OF SECTORS IN PARTITION
        // PARTITION INFORMATION FROM TSK INFO   : FLAGS, LEN (# OF SECTORS), START, (FIRST SECTOR), DESC, SLOT_NUM, TABLE_NUM
    }
    else // not a bootable volume
    {
        wombatptr->htmlcontent += "</table><br/><br/><div class='tabletitle'>not a bootable volume</div>";
        // block size, partition count, offset where volume system begins, endian - probably go in a table similar to the above one, not a byte offset one.
        // not sure if i want to include the minimal amount of sector info since there are so few and it'll be repetative with the possible partition
        // layout in the tree view
    }
}

int WombatFramework::DetermineVectorIndex()
{
    int curidx = -1;
    if(wombatptr->selectedobject.type == 1) // image file
    {
        for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
        {
            if(wombatptr->evidenceobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else if(wombatptr->selectedobject.type == 2) // volume file (it should never be a volume since i don't add it to the image tree)
    {
        for(int i=0; i < wombatptr->volumeobjectvector.count(); i++)
        {
            if(wombatptr->volumeobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else if(wombatptr->selectedobject.type == 3) // partition file
    {
        for(int i=0; i < wombatptr->partitionobjectvector.count(); i++)
        {
            if(wombatptr->partitionobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else if(wombatptr->selectedobject.type == 4) // file system file
    {
        for(int i=0; i < wombatptr->filesystemobjectvector.count(); i++)
        {
            if(wombatptr->filesystemobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else // implement for files, directories etc.. as i go.
    {
    }

    return curidx;
}
