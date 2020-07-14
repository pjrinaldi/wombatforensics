#include "properties.h"

void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, QString attrid, QString partpath, bool isres)
{
    QString curblockstring = "";
    if(!isres)
        curblockstring = GetAdsBlockList(curfileinfo, attrid.toLongLong());
    QFile adspropfile;
    if(curfileinfo->name != NULL)
    {
        adspropfile.setFileName(partpath + "fa" + attrid + ".a" + QString::number(curfileinfo->name->meta_addr) + ".prop");
        adspropfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream proplist(&adspropfile);
        proplist << "Alternate Data Stream (ADS)||" << QString::fromStdString(std::string(curfileinfo->name->name)) << "||Alternate data stream which contains different content from what the file's standard content is." << Qt::endl;
        proplist << "Name||" << adsname << "||Name for the NTFS parent file additional $Data attribute" << Qt::endl;
        proplist << "Parent Address||" << QString::number(curfileinfo->meta->addr) << "||NTFS address ID for the parent file" << Qt::endl;
        proplist << "Parent File Name||" << QString(curfileinfo->name->name) << "||File name of the parent file" << Qt::endl;
        if(!isres)
            proplist << "Block Address||" << curblockstring << "||List of block addresses which contain the contents of the alternate data stream" << Qt::endl;
        proplist << "Attribute ID||" << attrid << "||ID for the file's ADS attribute" << Qt::endl;
        if(!isres && !adsname.contains("$BadClus:$Bad"))
            proplist << "Byte Offset||" << QString::number(curblockstring.split("^^", Qt::SkipEmptyParts).at(0).toLongLong()*curfileinfo->fs_info->block_size + curfileinfo->fs_info->offset) << "||Byte Offset for the first block of the file in bytes" << Qt::endl;
        proplist << "Data Attribute||";
        if(isres)
            proplist << "Resident";
        else
            proplist << "Non Resident";
        proplist << "||" << Qt::endl;
        proplist.flush();
        if(adspropfile.isOpen())
        {
            adspropfile.close();
        }
    }
}

void WriteFileProperties(TSK_FS_FILE* curfileinfo, QString partpath)
{
    QFile filepropfile;
    if(curfileinfo->name->meta_addr == 0 && strcmp(curfileinfo->name->name, "$MFT") != 0)
        filepropfile.setFileName(partpath + "fo" + QString::number(orphancount) + ".a" + QString::number(curfileinfo->name->par_addr) + ".prop");
    else if(curfileinfo->meta == NULL)
        filepropfile.setFileName(partpath + "fd" + QString::number(curfileinfo->name->meta_addr) + ".a" + QString::number(curfileinfo->name->par_addr) + ".prop");
    else
        filepropfile.setFileName(partpath + "f" + QString::number(curfileinfo->name->meta_addr) + ".a" + QString::number(curfileinfo->name->par_addr) + ".prop");
    filepropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream proplist(&filepropfile);
    if(curfileinfo->name != NULL) proplist << "Short Name||" << curfileinfo->name->shrt_name << "||Short Name for a file" << Qt::endl;
    bool isresident = false;
    int hfsreservetype = 0;
    if(curfileinfo->meta != NULL)
    {
        proplist << "Target File Name||" << QString(curfileinfo->meta->link) << "||Name of target file if this is a symbolic link" << Qt::endl;
        proplist << "Link Count||" << QString::number(curfileinfo->meta->nlink) << "||Number of files pointing to this file" << Qt::endl;
        proplist << "File Permissions||" << GetFilePermissions(curfileinfo->meta) << "||Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type|user|group|other - [rdlcbpv]|rw[sSx]|rw[sSx]|rw[tTx]" << Qt::endl;
        proplist << "User ID||" << QString::number(curfileinfo->meta->uid) << "||User ID" << Qt::endl;
        proplist << "Group ID||" << QString::number(curfileinfo->meta->gid) << "||Group ID" << Qt::endl;
        proplist << "Allocation Status||";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_ALLOC) == 1)
            proplist << "Allocated,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_UNALLOC) == 2)
            proplist << "Unallocated,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_USED) == 4)
            proplist << "Used,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_UNUSED) == 8)
            proplist << "Unused,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_COMP) == 16)
            proplist << "Compressed,";
        if((curfileinfo->meta->flags & TSK_FS_META_FLAG_ORPHAN) == 32)
            proplist << "Orphan,";
        proplist << "||allocation status for the file." << Qt::endl;
        proplist << "Attributes||";
        QString attrstr = "";
	QString apfsblkstring = "";
        int attrcnt = tsk_fs_file_attr_getsize(curfileinfo);
        QString rnrstr = "Data Attribute||";
        for(int i=0; i < attrcnt; i++)
        {
            const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(curfileinfo, i);
            if(fsattr->type == 0) attrstr += "Not Found,";
            else if(fsattr->type == 1 && (curfileinfo->fs_info->ftype == TSK_FS_TYPE_HFS || curfileinfo->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT))
            {
                attrstr += "Default,";
                // I SHOULD SET A BOOLEAN HERE.... THEN BELOW WHERE I WOULD POPULATE OFFSET AND BLOCKLIST CONVERT THE VALUES TO THAT...
                // I HAVE THE OPTION TO EITHER WRITE THE FILESYSTEM PROPERTIES TO A VARIABLE I CARRY THOUGH OR READ THE FS PROPERTIES FILE
                // DOWN BELOW
                if(curfileinfo->name->meta_addr == 3)
                {
                    hfsreservetype = 3;
                    //qDebug() << "$ExtentsFile";
                }
                else if(curfileinfo->name->meta_addr == 4)
                {
                    hfsreservetype = 4;
                    //qDebug() << "$CatalogFile";
                }
                else if(curfileinfo->name->meta_addr == 6)
                {
                    hfsreservetype = 6;
                    //qDebug() << "$AllocationFile";
                }
                else if(curfileinfo->name->meta_addr == 7)
                {
                    hfsreservetype = 7;
                    //qDebug() << "$StartupFile";
                }
                else if(curfileinfo->name->meta_addr == 8)
                {
                    hfsreservetype = 8;
                    //qDebug() << "$AttributesFile";
                }
                //else if(curfileinfo->name->meta_addr == 5)
                //{
                //    qDebug() << "$BadBlockFile";
                //}
            }
            else if(fsattr->type == 16) attrstr += "$STANDARD_INFOMRATION,";
            else if(fsattr->type == 32) attrstr += "$ATTRIBUTE_LIST,";
            else if(fsattr->type == 48) attrstr += "$FILE_NAME,";
            else if(fsattr->type == 64) attrstr += "$OBJECT_ID,";
            else if(fsattr->type == 80) attrstr += "SECURITY_DESCRIPTOR,";
            else if(fsattr->type == 96) attrstr += "$VOLUME_NAME,";
            else if(fsattr->type == 112) attrstr += "$VOLUME_INFORMATION,";
            else if(fsattr->type == 128 || fsattr->type == 4352)
            {
                if(fsattr->name == NULL)
                {
                    attrstr += "$DATA,";
                    if(fsattr->flags & TSK_FS_ATTR_NONRES)
                        rnrstr += "Non Resident";
                    else if(fsattr->flags & TSK_FS_ATTR_RES)
                    {
                        rnrstr += "Resident";
                        isresident = true;
                    }
                }
		//qDebug() << "fsname:" << curfileinfo->name->name << "1st blk addr:" << fsattr->nrd.run->addr << "# of blocks:" << fsattr->nrd.run->len;
                //else
                //    qDebug() << "attr name is not null:" << fsattr->name;
            }
            else if(fsattr->type == 144)
            {
                isresident = true;
                rnrstr += "Resident";
                attrstr += "$INDEX_ROOT,";
            }
            else if(fsattr->type == 160) attrstr += "$INDEX_ALLOCATION,";
            else if(fsattr->type == 176) attrstr += "$BITMAP,";
            else if(fsattr->type == 192) attrstr += "$REPARSE_POINT,";
            else if(fsattr->type == 208) attrstr += "$EA_INFORMATION,";
            else if(fsattr->type == 224) attrstr += "$EA,";
            else if(fsattr->type == 256) attrstr += "$LOGGED_UTILITY_STREAM,";
            else if(fsattr->type == 4353) attrstr += "RSRC,";
            else if(fsattr->type == 4354) attrstr += "EXT_ATTR,";
            else if(fsattr->type == 4355) attrstr += "COMP_REC,";
        }
        proplist << attrstr << "||Attributes Types" << Qt::endl;
        proplist << rnrstr << "||Whether attribute is resident or non resident. A resident attribute is stored in the $DATA attribute of the MFT entry. A non resident attribute is stored in a cluster run outside of the MFT entry." << Qt::endl;
    }
    else
    {
        proplist << "Sequence Number||" << QString::number(curfileinfo->name->meta_seq) << "||Sequence number for the metadata structure." << Qt::endl;
        proplist << "Parent Sequence||" << QString::number(curfileinfo->name->par_seq) << "||Sequence number for the parent directory." << Qt::endl;
        proplist << "Allocation Status||";
        if((curfileinfo->name->flags & TSK_FS_NAME_FLAG_ALLOC) == 1)
            proplist << "Allocated";
        else if((curfileinfo->name->flags & TSK_FS_NAME_FLAG_UNALLOC) == 2)
            proplist << "Unallocated";
        proplist << "||Allocation status for the file." << Qt::endl;

    }
    QString blockliststring = "";
    if(!isresident)
    {
        if(hfsreservetype == 3 || hfsreservetype == 4 || hfsreservetype == 6 || hfsreservetype == 7 || hfsreservetype == 8)
        {
            if(hfsreservetype == 3) // $ExtentsFile
            {
                blockliststring = hfsreserveinfo.split("E|").last().split(",").first();
                //qDebug() << "E:" << blockliststring;
            }
            else if(hfsreservetype == 4) // $CatalogFile
            {
                blockliststring = hfsreserveinfo.split("C|").last().split(",").first();
                //qDebug() << "C:" << blockliststring;
            }
            else if(hfsreservetype == 6) // $AllocationFile
            {
                blockliststring = hfsreserveinfo.split("A|").last().split(",").first();
                //qDebug() << "A:" << blockliststring;
            }
            else if(hfsreservetype == 7) // $StartupFile
            {
                blockliststring = hfsreserveinfo.split("S|").last().split(",").first();
                //qDebug() << "S:" << blockliststring;
            }
            else if(hfsreservetype == 8) // $AttributesFile
            {
                blockliststring = hfsreserveinfo.split("B|").last().split(",").first();
                //qDebug() << "B:" << blockliststring;
            }
        }
        else
            blockliststring = GetBlockList(curfileinfo);
        proplist << "Block Address||" << blockliststring << "||List of block addresses which contain the contents of the file" << Qt::endl;
	QStringList objidlist = filepropfile.fileName().split("/");
	QString faid = objidlist.last();
	QString pid = objidlist.at(objidlist.count()-2);
	QString vid = objidlist.at(objidlist.count()-3);
	QString eid = objidlist.at(objidlist.count()-4);
	QString objid = eid.split(".").last() + "-" + vid + "-" + pid + "-" + faid.split(".").first() + "-" + faid.split(".").at(1);
        if(curfileinfo->name->meta_addr == 0 && strcmp(curfileinfo->name->name, "$MFT") == 0)
        {
	    mftblockhash.insert(objid, blockliststring);
        }
    }
    if(!isresident)
    {
        if(blockliststring.split("^^", Qt::SkipEmptyParts).count() > 0)
            proplist << "Byte Offset||" << QString::number(blockliststring.split("^^", Qt::SkipEmptyParts).at(0).toLongLong()*curfileinfo->fs_info->block_size + curfileinfo->fs_info->offset) << "||Byte Offset for the first block of the file in bytes" << Qt::endl;
    }
    proplist.flush();
    if(filepropfile.isOpen())
    {
        filepropfile.close();
    }
}

void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo, QString partitionpath)
{
    FFS_INFO* ffs = NULL;
    ffs_sb1* sb1 = NULL;
    ffs_sb2* sb2 = NULL;
    FATFS_INFO* fatfs = NULL;
    FATXXFS_SB* fatsb = NULL;
    NTFS_INFO* ntfsinfo = NULL;
    EXFATFS_MASTER_BOOT_REC* exfatsb = NULL;
    HFS_INFO* hfs = NULL;
    hfs_plus_vh* hsb = NULL;
    ISO_INFO* iso = NULL;
    iso9660_pvd_node* p = NULL;
    iso9660_svd_node* s = NULL;
    YAFFSFS_INFO* yfs = NULL;
    EXT2FS_INFO* ext2fs = NULL;
    char asc[512];
    char asc128[129];
    char timebuf[128];
    QFile fspropfile(partitionpath + "prop");
    fspropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream proplist(&fspropfile);
    if(curfsinfo->ftype == TSK_FS_TYPE_EXT2 || curfsinfo->ftype == TSK_FS_TYPE_EXT3 || curfsinfo->ftype == TSK_FS_TYPE_EXT4 || curfsinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
    {
        ext2fs = (EXT2FS_INFO*)curfsinfo;
        proplist << "File System Type||";
        if(curfsinfo->ftype == TSK_FS_TYPE_EXT2)
            proplist << "ext2";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT3)
            proplist << "ext3";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT4)
            proplist << "ext4";
        else
            proplist << "ext2";
        proplist << "||" << Qt::endl;
        proplist << "Inode Count||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "||Number of Inodes in the file system (0x00-0x03)" << Qt::endl;
        proplist << "Block Count||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "||Number of Blocks in the file system (0x04-0x07)" << Qt::endl;
        proplist << "Reserved Blocks||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_r_blocks_count)) << "||Number of blocks reserved to prevent the file system from filling up (0x08-0x0B)" << Qt::endl;
        proplist << "Unallocated Blocks||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_blocks_count)) << "||Number of unallocated blocks (0x0C-0x0F)" << Qt::endl;
        proplist << "Unallocated Inodes||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_inode_count)) << "||Number of unnalocated inodes (0x10-0x13)" << Qt::endl;
        proplist << "First Data Block||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_first_data_block)) << "||Block where block group 0 starts (0x14-0x17)" << Qt::endl;
        proplist << "Log Block Size||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_log_block_size)) << "||Block size saved as the number of places to shift 1,024 to the left (0x18-0x1B)" << Qt::endl;
        proplist << "Log Fragment Size||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_log_frag_size)) << "||Fragment size saved as the number of bits to shift 1,024 to the left (0x1C-0x1F)" << Qt::endl;
        proplist << "Blocks per Group||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_blocks_per_group)) << "||Number of blocks in each block group (0x20-0x23)" << Qt::endl;
        proplist << "Fragments per Group||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_frags_per_group)) << "||Number of fragments in each block group (0x24-0x27)" << Qt::endl;
        proplist << "Inodes per Group||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_per_group)) << "||Number of inodes in each block group (0x28-0x2B)" << Qt::endl;
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_mtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_mtime), timebuf) : "empty");
        proplist << "Last Mount Time||" << QString::fromStdString(std::string(asc)) << "||Last time the file system was mounted (0x2C-0x2F)" << Qt::endl;
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_wtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_wtime), timebuf) : "empty");
        proplist << "Last Written Time||" << QString::fromStdString(std::string(asc)) << "||Last time data was written to the file system (0x30-0x33)" << Qt::endl;
        proplist << "Current Mount Count||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_mnt_count)) << "||Number of times the file system has been mounted (0x34-0x35)" << Qt::endl;
        proplist << "Maximum Mount Count||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_max_mnt_count)) << "||Maximum number of times the file system can be mounted (0x36-0x37)" << Qt::endl;
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_magic)) << "||File system signature value should be 0xef53 (0x38-0x39)" << Qt::endl;
        proplist << "File System State||";
        if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_state) & EXT2FS_STATE_VALID)
            proplist << "Unmounted properly";
        else if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_state) & EXT2FS_STATE_ERROR)
            proplist << "Errors Detected";
        else
            proplist << "Orphan inodes were being recovered";
        proplist << "||State of the file system when it was last shut down (0x3A-0x3B)" << Qt::endl;
        proplist << "Error Handling Method||";
        if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_errors) == 1)
            proplist << "Continue";
        else if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_errors) == 2)
            proplist << "Remount as Read-Only";
        else
            proplist << "Panic";
        proplist << "||Identifies what the OS should do when it encounters a file system error (0x3C-0x3D)" << Qt::endl;
        proplist << "Minor Version||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_minor_rev_level)) << "||Minor Revision Level (0x3E-0x3F)" << Qt::endl;
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_lastcheck) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_lastcheck), timebuf) : "empty");
        proplist << "Last Checked||" << QString::fromStdString(std::string(asc)) << "||Last time the consistency of the file system was checked (0x40-0x43)" << Qt::endl;
        proplist << "Interval b/w Checks||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_checkinterval)) << "||Interval between forced consistency checks (0x44-0x47)" << Qt::endl;
        proplist << "Creator OS||";
        switch(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_creator_os))
        {
            case EXT2FS_OS_LINUX:
                proplist << "Linux";
                break;
            case EXT2FS_OS_HURD:
                proplist << "HURD";
                break;
            case EXT2FS_OS_MASIX:
                proplist << "MASIX";
                break;
            case EXT2FS_OS_FREEBSD:
                proplist << "FreeBSD";
                break;
            case EXT2FS_OS_LITES:
                proplist << "LITES";
                break;
            default:
                proplist << "Unknown";
                break;
        }
        proplist << "||OS that might have created the file system (0x48-0x4B)" << Qt::endl;
        proplist << "Major Version||";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_rev_level) == EXT2FS_REV_ORIG)
            proplist << "Static Structure";
        else
            proplist << "Dynamic Structure";
        proplist << "||If the version is not set to dynamic, the values from bytes 84 and up might not be accurate (0x4C-0x4F)" << Qt::endl;
        proplist << "UID to Use Reserved Blocks||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_def_resuid)) << "||User ID that can use reserved blocks (0x50-0x51)" << Qt::endl;
        proplist << "GID to Use Reserved Blocks||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_def_resgid)) << "||Group ID that can use reserved blocks (0x52-0x53)" << Qt::endl;
        proplist << "First Non-Reserved Inode||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_first_ino)) << "||First non-reserved inode in the file system (0x54-0x57)" << Qt::endl;
        proplist << "Inode Structure Size||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_inode_size)) << "||Size of each inode structure (0x58-0x59)" << Qt::endl;
        proplist << "Block Group for SuperBlock||" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_block_group_nr)) << "||Superblock is part of the block group (if backup copy) (0x5A-0x5B)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_DIR_PREALLOC)
            proplist << "Compatible Feature||" << "Directory Pre-allocation" << "||Pre-allocate directory blocks to reduce fragmentation. The OS can mount the file system as normal if it does not support a compatible feature (0x5C-0x5F)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_IMAGIC_INODES)
            proplist << "Compatible Feature||" << "IMagic Inodes" << "||AFS server inodes exist (0x5C-0x5F)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_HAS_JOURNAL)
            proplist << "Compatible Feature||" << "Journal" << "||File System has a journal (0x5C-0x5F)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_EXT_ATTR)
            proplist << "Compatible Feature||" << "Extended Attributes" << "||Inodes have extended attributes (0x5C-0x5F)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_RESIZE_INO)
            proplist << "Compatible Feature||" << "Resizable File System" << "||File system can resize itself for larger aptitions (0x5C-0x5F)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_DIR_INDEX)
            proplist << "Compatible Feature||" << "Directory Index" << "||Directories use hash index (0x5C-0x5F)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_COMPRESSION)
            proplist << "Incompatible Feature||" << "Compression" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_FILETYPE)
            proplist << "Incompatible Feature||" << "Filetype" << "||Directory entries contain a file type field (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_RECOVER)
            proplist << "Incompatible Feature||" << "Needs Recovery" << "||The file systems needs to be recovered (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_JOURNAL_DEV)
            proplist << "Incompatible Feature||" << "Journal Device" << "||The file system uses a journal device (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_META_BG)
            proplist << "Incompatible Feature||" << "Meta Block Groups" << "||The file system has meta block groups (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_EXTENTS)
            proplist << "Incompatible Feature||" << "Extents" << "||The file system uses extents (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_64BIT)
            proplist << "Incompatible Feature||" << "64-bit" << "||The file system is 64-bit capable (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_MMP)
            proplist << "Incompatible Feature||" << "Multiple Mount Protection" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_FLEX_BG)
            proplist << "Incompatible Feature||" << "Flexible Block Groups" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_EA_INODE)
            proplist << "Incompatible Feature||" << "Extended Attributes" << "||The file system supports extended attributes (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_DIRDATA)
            proplist << "Incompatible Feature||" << "Directory Entry Data" << "||The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_SPARSE_SUPER)
            proplist << "Read only Feature||" << "Sparse Super" << "||Sparse superblocks and goup descriptor tables. The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_LARGE_FILE)
            proplist << "Read only Feature||" << "Large File" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_HUGE_FILE)
            proplist << "Read only Feature||" << "Huge File" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_BTREE_DIR)
            proplist << "Read only Feature||" << "BTree Directory" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << Qt::endl;
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_EXTRA_ISIZE)
            proplist << "Read only Feature||" << "Extra Inode Size" << "||The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)" << Qt::endl;
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_uuid[8]), tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_uuid[0]));
        proplist << "File System ID||" << QString::fromStdString(std::string(asc)) << "||File system ID. Found in the superblock at bytes (0x68-0x77)" << Qt::endl;
        proplist << "File System Label||" << QString::fromStdString(std::string(ext2fs->fs->s_volume_name)) << "||File System Label. (0x78-0x87)" << Qt::endl;
        proplist << "Last Mounted Path||" << QString::fromStdString(std::string(ext2fs->fs->s_last_mounted)) << " ||Path where the file system was last mounted (0x88-0xC7)" << Qt::endl;
        proplist << "Algorithm Usage Bitmap||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_algorithm_usage_bitmap)) << "||(0xC8-0xCB)" << Qt::endl;
        proplist << "Blocks Preallocated for Files||" << QString::number(ext2fs->fs->s_prealloc_blocks) << "||Number of blocks to preallocate for files (0xCC-0xCC)" << Qt::endl;
        proplist << "Blocks Preallocated for Directories||" << QString::number(ext2fs->fs->s_prealloc_dir_blocks) << "||Number of blocks to preallocate for directories (0xCD-0xCD)" << Qt::endl;
        proplist << "Unused||" << "Unused" << "||Unused bytes (0xCE-0xCF)" << Qt::endl;
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_journal_uuid[8]), tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_journal_uuid[0]));
        proplist << "Journal ID||" << QString::fromStdString(std::string(asc)) << "||Journal ID (0xD0-0xDF)" << Qt::endl;
        proplist << "Journal Inode||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_journal_inum)) << "||Journal Inode (0xE0-0xE3)" << Qt::endl;
        proplist << "Journal Device||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_journal_dev)) << "||Journal device (0xE4-0xE7)" << Qt::endl;
        proplist << "Head of Oprhan Inode List||" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_last_orphan)) << "||Head of orphan inode list. (0xE8-0xEB)" << Qt::endl;
        proplist << "Unused||" << "Unused" << "||Unused (0xEC-0x03FF)" << Qt::endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B || curfsinfo->ftype == TSK_FS_TYPE_FFS2 || curfsinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
    {
        ffs = (FFS_INFO*)curfsinfo;
        sb1 = ffs->fs.sb1;
        sb2 = ffs->fs.sb2;
        proplist << "File System Type||";
        if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B)
            proplist << "UFS 1";
        else
            proplist << "UFS 2";
        proplist << "" << Qt::endl;
        if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B)
        {
            proplist << "Unused||" << "Unused" << "||Unused (0x00-0x07)" << Qt::endl;
            proplist << "Backup Superblock Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->sb_off)) << "||Offset to backup superblock in cylinder group relative to a \"base\" (0x08-0x0B)" << Qt::endl;
            proplist << "Group Descriptor Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->gd_off)) << "||Offset to group descriptor in cylinder group relative to a \"base\" (0x0C-0x0F)" << Qt::endl;
            proplist << "Inode Table Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->ino_off)) << "||Offset to inode table in cylinder group relative to a \"base\" (0x10-0x13)" << Qt::endl;
            proplist << "Data Block Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->dat_off)) << "||Offset to data blocks in cylinder group relative to a \"base\" (0x14-0x17)" << Qt::endl;
            proplist << "Delta Value for Staggering Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_delta)) << "||Delta value for caluclating the staggering offset in cylinder group (0x18-0x1B)" << Qt::endl;
            proplist << "Mask for Staggering Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_cyc_mask)) << "||Mask for calculating the staggering offset (cycle value) in cylinder group (0x1C-0x1F)" << Qt::endl;
            sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, sb1->wtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, sb1->wtime), timebuf) : "empty");
            proplist << "Last Written Time||" << QString::fromStdString(std::string(asc)) << "||Last time data was written to the file system (0x20-0x23)" << Qt::endl;
            proplist << "Number of Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->frag_num)) << "||Number of fragments in the file system (0x24-0x27)" << Qt::endl;
            proplist << "Number of Data Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->data_frag_num)) << "||Number of fragments in the file system that can store file data (0x28-0x2B)" << Qt::endl;
            proplist << "Number of Cylinder Groups||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_num)) << "||Number of cylinder groups in the file system (0x2C-0x2F)" << Qt::endl;
            proplist << "Block Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->bsize_b)) << "||Size of a block in bytes (0x30-0x33)" << Qt::endl;
            proplist << "Fragment Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fsize_b)) << "||Size of a fragment in bytes (0x34-0x37)" << Qt::endl;
            proplist << "Block Framgent Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->bsize_frag)) << "||Size of a block in fragments (0x38-0x3B)" << Qt::endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x3C-0x5F)" << Qt::endl;
            proplist << "Number of Bits Convert Block to Fragment||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_fragshift)) << "||Number of bits to convert between a block address and a fragment address (0x60-0x63)" << Qt::endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x64-0x77)" << Qt::endl;
            proplist << "Inodes Per Block||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_inopb)) << "||Number of inodes per block in the inode table (0x78-0x7B)" << Qt::endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x7C-0x8F)" << Qt::endl;
            sprintf(asc, "%" PRIx32 "%" PRIx32 "", tsk_getu32(curfsinfo->endian, &sb1->fs_id[4]), tsk_getu32(curfsinfo->endian, &sb1->fs_id[0]));
            proplist << "File System ID||" << QString::fromStdString(std::string(asc)) << "||File system ID (0x90-0x97)" << Qt::endl;
            proplist << "Cylinder Group Fragment Address||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_saddr)) << "||Fragment address of the cylinder group summary area (0x98-0x9B)" << Qt::endl;
            proplist << "Cylinder Group Summary Area Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_ssize_b)) << "||Size of the cylinder group summary area in bytes (0x9C-0x9F)" << Qt::endl;
            proplist << "Cylinder Group Descriptor Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_cgsize)) << "||Size of the cylinder group descriptor in bytes (0xA0-0xA3)" << Qt::endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0xA4-0xA5)" << Qt::endl;
            proplist << "Cylinders in File System||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_ncyl)) << "||Number of cylinders in the file system (0xA6-0xB3)" << Qt::endl;
            proplist << "Cylinders per Cylinder Group||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_cpg)) << "||Number of cylinders in a cylinder group (0xB4-0xB7)" << Qt::endl;
            proplist << "Inodes per Cylinder Group||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_inode_num)) << "||Number of inodes in a cylinder group (0xB8-0xBB)" << Qt::endl;
            proplist << "Fragments per Cylinder Group||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_frag_num)) << "||Number of fragments in a cylinder group (0xBC-0xBF)" << Qt::endl;
            proplist << "Number of Directories||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.dir_num)) << "||Number of directories (0xC0-0xC3)" << Qt::endl;
            proplist << "Number of Free Blocks||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.blk_free)) << "||Number of free blocks (0xC4-0xC7)" << Qt::endl;
            proplist << "Number of Free Inodes||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.ino_free)) << "||Number of free inodes (0xC8-0xCB)" << Qt::endl;
            proplist << "Number of Free Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.frag_free)) << "||Number of free fragments (0xCC-0xCF)" << Qt::endl;
            proplist << "Super Block Modified Flag||" << QString::number(sb1->fs_fmod) << "||Super Block Modified Flag (0xD0-0xD0)" << Qt::endl;
            proplist << "Clean File System Flag||" << QString::number(sb1->fs_clean) << "||File system was clean when it was mounted (0xD1-0xD1)" << Qt::endl;
            proplist << "Read Only File System Flag||" << QString::number(sb1->fs_ronly) << "||File system was mounted read only (0xD2-0xD2)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags||" << "Unclean" << "||Set when the file system is mounted (0xD3-0xD3)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags||" << "Soft Dependencies" << "||Soft dependencies are being used (0xD3-0xD3)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags||" << "Needs Check" << "||Needs consistency check next time the file system is mounted (0xD3-0xD3)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags||" << "Index Directories" << "||Directories are indexed using a hashtree or B-Tree (0xD3-0xD3)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags||" << "Access Control Lists" << "||Access control lists are being used (0xD3-0xD3)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags||" << "TrustedBSD MAC Multi-Label" << "||TrustedBSD Mandatory Access Control multi-labels are being used (0xD3-0xD3)" << Qt::endl;
            if(sb1->fs_flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags||" << "Updated Flag Location" << "||Flags have been moved (0xD3-0xD3)" << Qt::endl;
            proplist << "Last Mount Point||" << QString::fromStdString(std::string(reinterpret_cast<char*>(sb1->last_mnt))) << "||Last mount point (0xD4-0x02D3)" << Qt::endl;
            proplist << "Non-Essential Values||" << "Non-Essential Values" << "||Non-Essential Values (0x02D4-0x055B)" << Qt::endl;
            proplist << "Signature||" << QString::number(tsk_gets32(curfsinfo->endian, sb1->magic)) << "||File System signature value should be 0x011954 (0x055C-0x055F)" << Qt::endl;
        }
        else // FFS2
        {
            proplist << "Unused||" << "Unused" << "||Unused (0x00-0x07)" << Qt::endl;
            proplist << "Backup Superblock Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->sb_off)) << "||Offset to backup superblock in cylinder group relative to a \"base\" (0x08-0x0B)" << Qt::endl;
            proplist << "Group Descriptor Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->gd_off)) << "||Offset to group descriptor in cylinder group relative to a \"base\" (0x0C-0x0F)" << Qt::endl;
            proplist << "Inode Table Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->ino_off)) << "||Offset to inode table in cylinder group relative to a \"base\" (0x10-0x13)" << Qt::endl;
            proplist << "First Data Block Offset||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->dat_off)) << "||Offset to the first data block in the cylinder group relative to a \"base\" (0x14-0x17)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x18-0x2B)" << Qt::endl;
            proplist << "Number of Cylinder Groups||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_num)) << "||The Number of cylinder groups in the file system (0x2C-0x2F)" << Qt::endl;
            proplist << "Block Byte Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->bsize_b)) << "||Size of a block in bytes (0x30-0x33)" << Qt::endl;
            proplist << "Fragment Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fsize_b)) << "||Size of a fragment in bytes (0x34-0x37)" << Qt::endl;
            proplist << "Block Fragment Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->bsize_frag)) << "||Size of a block in fragments (0x38-0x3B)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x3C-0x5F)" << Qt::endl;
            proplist << "File System Fragment Shift||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_fragshift)) << "||Number of bits to convert between a block address and a fragment address (0x60-0x63)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x64-0x77)" << Qt::endl;
            proplist << "Inodes Per Block||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_inopb)) << "||Number of inodes per block in inode table (0x78-0x7B)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x7C-0x9B)" << Qt::endl;
            proplist << "Cylinder Group Summary Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_ssize_b)) << "||Size of cylinder group summary area in bytes (0x9C-0x9F)" << Qt::endl;
            proplist << "Cylinder Group Descriptor Size||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_cgsize)) << "||Size of cylinder group descriptor in bytes (0xA0-0xA3)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0xA4-0xB7)" << Qt::endl;
            proplist << "Cylinder Group Inodes||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_inode_num)) << "||Inodes per cylinder group (0xB8-0xBB)" << Qt::endl;
            proplist << "Cylinder Group Fragments||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_frag_num)) << "||Fragments per cylinder group (0xBC-0xBF)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0xC0-0xCF)" << Qt::endl;
            proplist << "Super Block Modified Flag||" << QString::number(sb2->fs_fmod) << "||Super block modified flag (0xD0-0xD0)" << Qt::endl;
            proplist << "Clean File System||" << QString::number(sb2->fs_clean) << "||File system was clean when it was mounted (0xD1-0xD1)" << Qt::endl;
            proplist << "Read Only Flag||" << QString::number(sb2->fs_ronly) << "||File system was mounted read only (0xD2-0xD2)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0xD3-0xD3)" << Qt::endl;
            proplist << "Last Mount Point||" << QString::fromUtf8(reinterpret_cast<char*>(sb2->last_mnt)) << "||Last mount point (0xD4-0x02A7)" << Qt::endl;
            proplist << "Volume Name||" << QString::fromUtf8(reinterpret_cast<char*>(sb2->volname)) << "||Volume name (0x02A8-0x02C7)" << Qt::endl;
            proplist << "System UID||" << QString::fromUtf8(reinterpret_cast<char*>(sb2->swuid)) << "||System UID (0x02C8-0x02CF)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x02D0-0x03EF)" << Qt::endl;
            proplist << "Number of Directories||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.dir_num)) << "||Number of directories (0x03F0-0x03F7)" << Qt::endl;
            proplist << "Number of Free Blocks||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.blk_free)) << "||Number of free blocks (0x03F8-0x03FF)" << Qt::endl;
            proplist << "Number of Free Inodes||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.ino_free)) << "||Number of free inodes (0x0400-0x0407)" << Qt::endl;
            proplist << "Number of Free Fragments||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.frag_free)) << "||Number of free fragments (0x0408-0x040F)" << Qt::endl;
            proplist << "Number of Free Clusters||" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.clust_free)) << "||Number of free clusters (0x0410-0x0417)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x0418-0x042F)" << Qt::endl;
            sprintf(asc, "%s", (tsk_getu64(curfsinfo->endian, sb1->wtime) > 0) ? tsk_fs_time_to_str(tsk_getu64(curfsinfo->endian, sb1->wtime), timebuf) : "empty");
            proplist << "Last Written Time||" << QString::fromStdString(std::string(asc)) << "||Last time data was written to the file system (0x0430-0x0437)" << Qt::endl;
            proplist << "Fragment Numbers||" << QString::number(tsk_gets64(curfsinfo->endian, sb2->frag_num)) << "||Number of fragments in the file system (0x0438-0x043F)" << Qt::endl;
            proplist << "Usable Fragment Numbers||" << QString::number(tsk_gets64(curfsinfo->endian, sb2->blk_num)) << "||Number of fragments that can store file data (0x0440-0x0447)" << Qt::endl;
            proplist << "Cylinder Group Fragment Address||" << QString::number(tsk_gets64(curfsinfo->endian, sb2->cg_saddr)) << "||Fragment address of cylinder group summary area (0x0448-0x044F)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused (0x0450-0x051F)" << Qt::endl;
            int flags = tsk_getu32(curfsinfo->endian, sb2->fs_flags);
            if(flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags||" << "Unclean" << "||Set when the file system is mounted (0x0520-0x0523)" << Qt::endl;
            if(flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags||" << "Soft Dependencies" << "||Soft dependencies are being used (0x0520-0x0523)" << Qt::endl;
            if(flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags||" << "Needs Check" << "||Needs consistency check next time the file system is mounted (0x0520-0x0523)" << Qt::endl;
            if(flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags||" << "Index Directories" << "||Directories are indexed using a hashtree or B-Tree (0x0520-0x0523)" << Qt::endl;
            if(flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags||" << "Access Control Lists" << "||Access control lists are being used (0x0520-0x0523)" << Qt::endl;
            if(flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags||" << "TrustedBSD MAC Multi-Label" << "||TrustedBSD Mandatory Access Control multi-labels are being used (0x0520-0x0523)" << Qt::endl;
            if(flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags||" << "Updated Flag Location" << "||Flags have been moved (0x0520-0x0523)" << Qt::endl;
            proplist << "Unused" << "Unused||" << "||Unused (0x0524-0x055B)" << Qt::endl;
            proplist << "Signature||" << QString::number(tsk_gets32(curfsinfo->endian, sb2->magic)) << "||File system signature value should be 0x19540119 (0x055C-0x055F)" << Qt::endl;
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_FAT12 || curfsinfo->ftype == TSK_FS_TYPE_FAT16 || curfsinfo->ftype == TSK_FS_TYPE_FAT32)
    {
        fatfs = (FATFS_INFO*)curfsinfo;
        fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
        char* data_buf;
        ssize_t cnt;
        if((data_buf = (char*)tsk_malloc(curfsinfo->block_size)) != NULL)
        {
            // print error
        }
        cnt = tsk_fs_read_block(curfsinfo, fatfs->rootsect, data_buf, curfsinfo->block_size);
        if(cnt != curfsinfo->block_size)
        {
            // print error
        }
        free(data_buf);
        proplist << "File System Type||";
        if(curfsinfo->ftype == TSK_FS_TYPE_FAT12)
            proplist << "FAT12";
        else if(curfsinfo->ftype == TSK_FS_TYPE_FAT16)
            proplist << "FAT16";
        else if(curfsinfo->ftype == TSK_FS_TYPE_FAT32)
            proplist << "FAT32";
        proplist << "||File System Type" << Qt::endl;
        proplist << "Reserved||" << "Jump Code" << "||Assembly instructions to jump to boot code (0x00-0x02)" << Qt::endl;
        proplist << "OEM Name||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->oemname)) << "||OEM name in ASCII (0x03-0x0A)" << Qt::endl;
        proplist << "Bytes per Sector||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->ssize)) << "||Sector size in bytes. Allowed values include 512, 1024, 2048, and 4096 (0x0B-0x0C)" << Qt::endl;
        proplist << "Sectors per Cluster||" << QString::number(fatsb->csize) << "||Cluster size in sectors. Allowed values are powers of 2, but the cluster size must be 32KB or smaller (0x0D-0x0D)" << Qt::endl;
        proplist << "Reserved Area Size||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->reserved)) << "||Number of reserved sectors for boot sectors (0x0E-0x0F)" << Qt::endl;
        proplist << "Number of FATs||" << QString::number(fatsb->numfat) << "||Number of File Allocation Tables (FATs). Typically two for redundancy (0x10-0x10)" << Qt::endl;
        proplist << "Number of Root Directory Entries||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->numroot)) << "||Maximum number of files in the root directory for FAT12 and FAT16. This is 0 for FAT32 and typically 512 for FAT16 (0x11-0x12)" << Qt::endl;
        proplist << "Number of Sectors in File System||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->sectors16)) << "||Maximum number of sectors in the file system. If the number of sectors is larger than can be represented in this 2-byte value, a 4-byte value exists later in the data structure and this should be 0 (0x13-0x14)" << Qt::endl;
        proplist << "Media Type||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->f2)) << "||Media type. Should be 0xF8 for fixed disks and 0xF0 for removable disks (0x13-0x13)" << Qt::endl;
        proplist << "Size of FAT||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->sectperfat16)) << "||16-bit size in sectors of each FAT for FAT12 and FAT16. For FAT32, this field is 0 (0x14-0x15)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x16-0x1B)" << Qt::endl;
        proplist << "Number of Sectors Before Partition Start||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->prevsect)) << "||Number of sectors before the start of the file system partition (0x1C-0x1F)" << Qt::endl;
        proplist << "Number of Sectors in File System||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->sectors32)) << "||32-bit value of number of sectors in the file system. Either this value or the 16-bit value above must be 0 (0x20-0x23)" << Qt::endl;
        if(curfsinfo->ftype == TSK_FS_TYPE_FAT32)
        {
            proplist << "Size of FAT||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.sectperfat32)) << "||32-bit size in sectors of one FAT (0x24-0x27)" << Qt::endl;
            proplist << "Defines FAT is Written||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.ext_flag)) << "||Defines how multiple FAT structures are written to. If bit 7 is 1, only one of the FAT structures is active and its index is described in bits 0-3. Otherwise all FAT structures are mirrors of each other (0x28-0x29)" << Qt::endl;
            proplist << "Major and Minor Version||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.fs_ver)) << "||The major and minor version number (0x2A-0x2B)" << Qt::endl;
            proplist << "Root Directory Cluster||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.rootclust)) << "||Cluster where the root directory can be found (0x2C-0x2F)" << Qt::endl;
            proplist << "FSINFO Structure Sector||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.fsinfo)) << "||Sector where FSINFO structure can be found (0x30-0x31)" << Qt::endl;
            proplist << "Boot Sector Backup Copy||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.bs_backup)) << "||Sector where the backup copy of the boot sector is located, default is 6 (0x32-0x33)" << Qt::endl;
            proplist << "Reserved||" << "Reserved" << "||Reserved (0x34-0x3F)" << Qt::endl;
            proplist << "BIOS Drive Number||" << QString::number(fatsb->a.f32.drvnum) << "||BIOS INT32h drive number (0x40-0x40)" << Qt::endl;
            proplist << "Not used||" << "Not used" << "||Not used (0x41-0x42)" << Qt::endl;
            proplist << "Volume Serial Number||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.vol_id)) << "||Volume serial number, which some versions of Windows will calculate based on the creation date and time (0x43-0x46)" << Qt::endl;
            proplist << "Volume Label||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f32.vol_lab)) << "||Volume label in ASCII. The user chooses this value when creating the file system (0x47-0x51)" << Qt::endl;
            proplist << "File System Type||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f32.fs_type)) << "||File system type label in ASCII. Standard values include \"FAT32\", but nothing is required (0x52-0x59)" << Qt::endl;
            proplist << "Not Used||" << "Not Used" << "||Not Used (0x005A-0x01FD)" << Qt::endl;
        }
        else
        {
            proplist << "Reserved||" << "Reserved" << "||Reserved (0x24-0x26)" << Qt::endl;
            proplist << "Volume Serial Number||" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f16.vol_id)) << "||Volume serial number, which some versions of Windows will calculate based on the creation date and time (0x27-0x2A)" << Qt::endl;
            proplist << "Volume Label||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f16.vol_lab)) << "||Volume label in ASCII. The user chooses this value when creating the file system (0x2B-0x35)" << Qt::endl;
            proplist << "File System Type||" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f16.fs_type)) << "||File system type in ASCII. Standard values include \"FAT\", \"FAT12\", \"FAT16\", but nothing is required (0x36->0x3D)" << Qt::endl;
            proplist << "Reserved||" << "Reserved" << "||Reserved (0x3E-0x01FD)" << Qt::endl;
        }
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->magic)) << "||Signature value should be 0xAA55 (0x01FE-0x01FF)" << Qt::endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_NTFS)
    {
        ntfsinfo = (NTFS_INFO*)curfsinfo;
        proplist << "Assembly Boot Code||" << "Assembly Boot Code" << "||Assembly instruction to jump to boot code (0x00-0x02)" << Qt::endl;
        proplist << "OEM Name||" << QString::fromUtf8(ntfsinfo->fs->oemname) << "||OEM Name (0x03-0x0A)" << Qt::endl;
        proplist << "Bytes per Sector||" << QString::number(tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize)) << "||Bytes per sector (0x0B-0x0C)" << Qt::endl;
        proplist << "Sectors per Cluster||" << QString::number(ntfsinfo->fs->csize) << "||Sectors per cluster (0x0D-0x0D)" << Qt::endl;
        proplist << "Reserved Sectors||" << "Reserved Sectors" << "||Reserved and Unused Sectors (0x0E-0x27)" << Qt::endl;
        proplist << "Volume Size (sectors)||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->vol_size_s)) << "||Total Sectors in the file system (0x28-0x2F)" << Qt::endl;
        proplist << "MFT Starting Cluster Address||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mft_clust)) << "||Starting cluster address of the Master File Table (MFT) (0x30-0x37)" << Qt::endl;
        proplist << "MFT Starting Byte Address||" << QString::number( tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mft_clust) * ntfsinfo->fs->csize * tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize) ) << "||Starting byte address of the Master File Table (MFT)" << Qt::endl;
        proplist << "MFT Mirror Starting Cluster Address||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mftm_clust)) << "||Starting cluster address of the MFT Mirror (0x38-0x3F)" << Qt::endl;
        int recordsize = 0;
        if(ntfsinfo->fs->mft_rsize_c > 0)
        {
            recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize);
        }
        else
            recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
        proplist << "MFT Record Size (bytes)||" << QString::number(recordsize) << "||Size of file record in bytes (MFT Entry) (0x40-0x40)" << Qt::endl;
        proplist << "Unused||" << "Unused" << "||Unused (0x41-0x43)" << Qt::endl;
        proplist << "Size of Index Record||" << QString::number(ntfsinfo->fs->idx_rsize_c) << "||Number of clusters per index record (0x44-0x44)" << Qt::endl;
        proplist << "Unused||" << "Unused" << "||Unused (0x45-0x47)" << Qt::endl;
        proplist << "Serial Number||" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->serial)) << "||Serial Number (0x48-0x4F)" << Qt::endl;
        proplist << "Boot Code||" << "Boot Code" << "||Boot Code (0x0050-0x00FD)" << Qt::endl;
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, ntfsinfo->fs->magic)) << "||Signature value should be 0xAA55 (0x00FE-0x00FF)" << Qt::endl;
        TSK_FS_FILE* tmpfile = NULL;
        const TSK_FS_ATTR* tmpattr;
        if((tmpfile = tsk_fs_file_open_meta(curfsinfo, NULL, NTFS_MFT_VOL)) == NULL)
        {
            // log error here
        }
        tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, TSK_FS_ATTR_TYPE_NTFS_VNAME);
        if((tmpattr->flags & TSK_FS_ATTR_RES) && (tmpattr->size))
        {
            UTF16* name16 = (UTF16*) tmpattr->rd.buf;
            UTF8* name8 = (UTF8*) asc;
            int retval;
            retval = tsk_UTF16toUTF8(curfsinfo->endian, (const UTF16**)&name16, (UTF16*) ((uintptr_t)name16 + (int) tmpattr->size), &name8, (UTF8*) ((uintptr_t)name8 + sizeof(asc)), TSKlenientConversion);
            if(retval != TSKconversionOK)
            {
                // log error here
                *name8 = '\0';
            }
            else if((uintptr_t)name8 >= (uintptr_t)asc + sizeof(asc))
                asc[sizeof(asc)-1] = '\0';
            else
                *name8 = '\0';
            proplist << "Volume Name||" << QString::fromStdString(std::string(asc)) << "||Volume Name from $VOLUME_NAME attribute" << Qt::endl;
            proplist << "Version||";
            if(ntfsinfo->ver == NTFS_VINFO_NT)
                proplist << "Windows NT";
            else if(ntfsinfo->ver == NTFS_VINFO_2K)
                proplist << "Windows 2000";
            else if(ntfsinfo->ver == NTFS_VINFO_XP)
                proplist << "Windows XP";
            else
                proplist << "Newer than Windows XP";
            proplist << "||Version Information" << Qt::endl;
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_EXFAT)
    {
        fatfs = (FATFS_INFO*)curfsinfo;
        exfatsb = (EXFATFS_MASTER_BOOT_REC*)&(fatfs->boot_sector_buffer);
        proplist << "Jump Boot||" << "Jump Boot" << "||Jump boot should be 0xEB7690 (0x00-0x02)" << Qt::endl;
        proplist << "File System Name||" << QString::fromUtf8(reinterpret_cast<char*>(exfatsb->fs_name)) << "||File system name should be \"EXFAT \" (0x03-0x0A)" << Qt::endl;
        proplist << "Unused||" << "Unused" << "||Unused, must be zeros (0x0B-0x3F)" << Qt::endl;
        proplist << "Partition Offset||" << QString::number(tsk_getu64(curfsinfo->endian, exfatsb->partition_offset)) << "||Sector address for partition start (0x40-0x47)" << Qt::endl;
        proplist << "Volume Length (sectors)||" << QString::number(tsk_getu64(curfsinfo->endian, exfatsb->vol_len_in_sectors)) << "||Size of exFAT volume in sectors (0x48-0x4F)" << Qt::endl;
        proplist << "FAT Offset||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->fat_offset)) << "||FAT offset in sectors (0x50-0x53)" << Qt::endl;
        proplist << "FAT Length||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->fat_len_in_sectors)) << "||FAT length in sectors. May exceed the required space in order to align the second FAT (0x54-0x57)" << Qt::endl;
        proplist << "Cluster Heap Offset||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->cluster_heap_offset)) << "||Sector address of the data region (0x58-0x5B)" << Qt::endl;
        proplist << "Cluster Count||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->cluster_cnt)) << "||Number of clusters in the cluster heap (0x5C-0x5F)" << Qt::endl;
        proplist << "Root Directory Cluster||" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->root_dir_cluster)) << "||Cluster address of the root directory (0x60-0x63)" << Qt::endl;
        proplist << "Volume Serial Number||" << QString::fromUtf8(reinterpret_cast<char*>(exfatsb->vol_serial_no)) << "||Volume serial number (0x64-0x67)" << Qt::endl;
        proplist << "File System Revision||" << QString::number(exfatsb->fs_revision[1]) + "." + QString::number(exfatsb->fs_revision[0]) << "||File system revision as Major.Minor (0x68-0x69)" << Qt::endl;
        proplist << "Volume Flags||" << QString::number(tsk_getu16(curfsinfo->endian, exfatsb->vol_flags)) << "||Volume Flags (0x6A-0x6B)" << Qt::endl;
        proplist << "Bytes Per Sector||" << QString::number(exfatsb->bytes_per_sector) << "||Bytes per sector as a power of 2. Minimum 9 (512 bytes per sector), maximum 12 (4096 bytes per sector) (0x6C-0x6C)" << Qt::endl;
        proplist << "Sectors Per Cluster||" << QString::number(exfatsb->sectors_per_cluster) << "||Sectors per cluster as a power of 2. Minimum 0 (1 sector per cluster) up to a maximum 25 (0x6D-0x6D)" << Qt::endl;
        proplist << "Number of FATs||" << QString::QString::number(exfatsb->num_fats) << "||Number of FATs: 1 or 2, 2 is for TexFAT only) (0x6E-0x6E)" << Qt::endl;
        proplist << "Drive Select||" << QString::number(exfatsb->drive_select) << "||Used by INT 13h, typically 0x80 (0x6F-0x6F)" << Qt::endl;
        proplist << "Percent of the Heap in Use||" << QString::number(exfatsb->percent_of_cluster_heap_in_use) << "||0-100, percentage of allocated clusters rounded down to the integer; 0xFF, percentage is not available (0x70-0x70)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x71-0x77)" << Qt::endl;
        proplist << "Boot Code||" << "Boot Code" << "||Boot Code (0x78-0x01FD)" << Qt::endl;
        proplist << "Signature||" << QString::number(tsk_getu16(curfsinfo->endian, exfatsb->signature)) << "||Signature value should be 0xAA55 (0x01FE-0x01FF)" << Qt::endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_ISO9660) // Byte offset's aren't working out too well right now.
    {
        iso = (ISO_INFO*)curfsinfo;
        for(p = iso->pvd; p!= NULL; p = p->next)
        {
            proplist << "Unused||" << "Unused" << "||Unused (0x00-0x07)" << Qt::endl;
            proplist << "System Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.sys_id), 32) << "||System Identifier (0x08-0x27)" << Qt::endl;
            proplist << "Volume Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.vol_id)) << "||Volume Identifier (0x28-0x47)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused should be all zeros (0x48-0x4F)" << Qt::endl;
            proplist << "Volume Space Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.vs_sz_l)) << "||Volume Space Size in blocks (0x50-0x53)" << Qt::endl;
            proplist << "Volume Space Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.vs_sz_m)) << "||Volume Space Size in blocks (0x54-0x57)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused. All zeros (0x58-0x77)" << Qt::endl;
            proplist << "Volume Set Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_set_l)) << "||The size of the set in this logical volume (0x78-0x79)" << Qt::endl;
            proplist << "Volume Set Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_set_m)) << "||The size of the set in this logical volume (0x7A-0x7B)" << Qt::endl;
            proplist << "Volume Sequence Number (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_seq_l)) << "||The number of this disk in the volume set (0x7C-0x7D)" << Qt::endl;
            proplist << "Volume Sequence Number (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_seq_m)) << "||The number of this disk in the volume set (0x7E-0x7F)" << Qt::endl;
            proplist << "Logical Block Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.blk_sz_l)) << "||The size in bytes of a logical block (0x80-0x81)" << Qt::endl;
            proplist << "Logical Block Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.blk_sz_m)) << "||The size in bytes of a logical block (0x82-0x83)" << Qt::endl;
            proplist << "Path Table Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_size_l)) << "||The size in bytes of the path table (0x84-0x87)" << Qt::endl;
            proplist << "Path Table Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_size_m)) << "||The size in bytes of the path table (0x88-0x8B)" << Qt::endl;
            proplist << "Logical Block Address of Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_loc_l)) << "||LBA location of the path table (0x8C-0x8F)" << Qt::endl;
            proplist << "Logical Block Address of Optional Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_opt_loc_l)) << "||LBA location of the optional path table (0x90-0x93)" << Qt::endl;
            proplist << "Logical Block Address of the Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_loc_m)) << "||LBA location of the path table (0x94-0x97)" << Qt::endl;
            proplist << "Logical Block Address of the Optional Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_opt_loc_m)) << "||LBA location of the optional path table (0x98-0x9B)" << Qt::endl;
            proplist << "Directory Entry for Root Directory||" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.dir_rec.name)) << "||Directory Entry for Root Directory (0xB6-0xBD)" << Qt::endl;
            snprintf(asc128, 128, "%s", p->pvd.vol_setid);
            proplist << "Volume Set Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the volume set of which this volume is a member (0x00BE-0x013D)" << Qt::endl;
            snprintf(asc128, 128, "%s", p->pvd.pub_id);
            proplist << "Publisher Identifier||" << QString::fromStdString(std::string(asc128)) << "||Volume publisher (0x013E-0x01BD)" << Qt::endl;
            snprintf(asc128, 128, "%s", p->pvd.prep_id);
            proplist << "Data Preparer Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the person(s) who prepared the data for this volume (0x01BE-0x023D)" << Qt::endl;
            snprintf(asc128, 128, "%s", p->pvd.app_id);
            proplist << "Application Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifies how the data are recorded on this volume (0x023E-0x02BD)" << Qt::endl;
            snprintf(asc128, 38, "%s", p->pvd.copy_id);
            proplist << "Copyright File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Fielname of a file in the root directory that contains copyright information for the volume set (0x02BE-0x02E3)" << Qt::endl;
            snprintf(asc128, 37, "%s", p->pvd.abs_id);
            proplist << "Abstract File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains abstract information for the volume set (0x02E4-0x0307)" << Qt::endl;
            snprintf(asc128, 37, "%s", p->pvd.bib_id);
            proplist << "Bibliographic File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains bibliographic information for this volume set (0x0308-0x032C)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", ((char)(p->pvd.make_date.month[0])), (char)p->pvd.make_date.month[1], (char)p->pvd.make_date.day[0], (char)p->pvd.make_date.day[1], (char)p->pvd.make_date.year[0], (char)p->pvd.make_date.year[1], (char)p->pvd.make_date.year[2], (char)p->pvd.make_date.year[3], (char)p->pvd.make_date.hour[0], (char)p->pvd.make_date.hour[1], (char)p->pvd.make_date.min[0], (char)p->pvd.make_date.min[1], (char)p->pvd.make_date.sec[0], (char)p->pvd.make_date.sec[1]);
            proplist << "Volume Creation Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.make_date.gmt_off) << "||The date and time when the volume was created (0x032D-0x033D)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.mod_date.month[0], (char)p->pvd.mod_date.month[1], (char)p->pvd.mod_date.day[0], (char)p->pvd.mod_date.day[1], (char)p->pvd.mod_date.year[0], (char)p->pvd.mod_date.year[1], (char)p->pvd.mod_date.year[2], (char)p->pvd.mod_date.year[3], (char)p->pvd.mod_date.hour[0], (char)p->pvd.mod_date.hour[1], (char)p->pvd.mod_date.min[0], (char)p->pvd.mod_date.min[1], (char)p->pvd.mod_date.sec[0], (char)p->pvd.mod_date.sec[1]);
            proplist << "Volume Modification Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.mod_date.gmt_off) << "||The date and time when the volume was modified (0x033E-0x034E)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.exp_date.month[0], (char)p->pvd.exp_date.month[1], (char)p->pvd.exp_date.day[0], (char)p->pvd.exp_date.day[1], (char)p->pvd.exp_date.year[0], (char)p->pvd.exp_date.year[1], (char)p->pvd.exp_date.year[2], (char)p->pvd.exp_date.year[3], (char)p->pvd.exp_date.hour[0], (char)p->pvd.exp_date.hour[1], (char)p->pvd.exp_date.min[0], (char)p->pvd.exp_date.min[1], (char)p->pvd.exp_date.sec[0], (char)p->pvd.exp_date.sec[1]);
            proplist << "Volume Expiration Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.exp_date.gmt_off) << "||The date and time after which the volume is considered to be obsolete. If not specified, then the volume is never considered to be obsolete (0x034F-0x035F)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.ef_date.month[0], (char)p->pvd.ef_date.month[1], (char)p->pvd.ef_date.day[0], (char)p->pvd.ef_date.day[1], (char)p->pvd.ef_date.year[0], (char)p->pvd.ef_date.year[1], (char)p->pvd.ef_date.year[2], (char)p->pvd.ef_date.year[3], (char)p->pvd.ef_date.hour[0], (char)p->pvd.ef_date.hour[1], (char)p->pvd.ef_date.min[0], (char)p->pvd.ef_date.min[1], (char)p->pvd.ef_date.sec[0], (char)p->pvd.ef_date.sec[1]);
            proplist << "Volume Effective Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(p->pvd.ef_date.gmt_off) << "||The date and time after which the volume may be used. If not specified, then the volume may be used immediately (0x0360-0x0370)" << Qt::endl;
            proplist << "File Structure Version||" << QString::number(p->pvd.fs_ver) << "||The directory records and path table version, always 0x01 (0x0371-0x0371)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused, always 0x00 (0x0372-0x0372)" << Qt::endl;
            snprintf(asc, 512, "%s", p->pvd.app_use);
            proplist << "Application Used||" << QString::fromStdString(std::string(asc)) << "||Application Used. Contents not defined by ISO9660 (0x0373-0x0572)" << Qt::endl;
            proplist << "Reserved by ISO||" << "Reserved" << "||Reserved by ISO (0x0573-0x07FF)" << Qt::endl;
        }
        for(s = iso->svd; s!= NULL; s = s->next)
        {
            proplist << "Volume Flags||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.flags)) << "||Unused (0x00-0x07)" << Qt::endl;
            proplist << "System Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.sys_id), 32) << "||System Identifier (0x08-0x27)" << Qt::endl;
            proplist << "Volume Identifier||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.vol_id)) << "||Volume Identifier (0x28-0x47)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused should be all zeros (0x48-0x4F)" << Qt::endl;
            proplist << "Volume Space Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.vs_sz_l)) << "||Volume Space Size in blocks (0x50-0x53)" << Qt::endl;
            proplist << "Volume Space Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.vs_sz_m)) << "||Volume Space Size in blocks (0x54-0x57)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused. All zeros (0x58-0x77)" << Qt::endl;
            proplist << "Volume Set Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_set_l)) << "||The size of the set in this logical volume (0x78-0x79)" << Qt::endl;
            proplist << "Volume Set Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_set_m)) << "||The size of the set in this logical volume (0x7A-0x7B)" << Qt::endl;
            proplist << "Volume Sequence Number (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_seq_l)) << "||The number of this disk in the volume set (0x7C-0x7D)" << Qt::endl;
            proplist << "Volume Sequence Number (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_seq_m)) << "||The number of this disk in the volume set (0x7E-0x7F)" << Qt::endl;
            proplist << "Logical Block Size (LE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.blk_sz_l)) << "||The size in bytes of a logical block (0x80-0x81)" << Qt::endl;
            proplist << "Logical Block Size (BE)||" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.blk_sz_m)) << "||The size in bytes of a logical block (0x82-0x83)" << Qt::endl;
            proplist << "Path Table Size (LE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_size_l)) << "||The size in bytes of the path table (0x84-0x87)" << Qt::endl;
            proplist << "Path Table Size (BE)||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_size_m)) << "||The size in bytes of the path table (0x88-0x8B)" << Qt::endl;
            proplist << "Logical Block Address of Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_loc_l)) << "||LBA location of the path table (0x8C-0x8F)" << Qt::endl;
            proplist << "Logical Block Address of Optional Type-L Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_opt_loc_l)) << "||LBA location of the optional path table (0x90-0x93)" << Qt::endl;
            proplist << "Logical Block Address of the Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_loc_m)) << "||LBA location of the path table (0x94-0x97)" << Qt::endl;
            proplist << "Logical Block Address of the Optional Type-M Path Table||" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_opt_loc_m)) << "||LBA location of the optional path table (0x98-0x9B)" << Qt::endl;
            proplist << "Directory Entry for Root Directory||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.dir_rec.name)) << "||Directory Entry for Root Directory (0xB6-0xBD)" << Qt::endl;
            snprintf(asc128, 128, "%s", s->svd.vol_setid);
            proplist << "Volume Set Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the volume set of which this volume is a member (0x00BE-0x013D)" << Qt::endl;
            snprintf(asc128, 128, "%s", s->svd.pub_id);
            proplist << "Publisher Identifier||" << QString::fromStdString(std::string(asc128)) << "||Volume publisher (0x013E-0x01BD)" << Qt::endl;
            snprintf(asc128, 128, "%s", s->svd.prep_id);
            proplist << "Data Preparer Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifier of the person(s) who prepared the data for this volume (0x01BE-0x023D)" << Qt::endl;
            snprintf(asc128, 128, "%s", s->svd.app_id);
            proplist << "Application Identifier||" << QString::fromStdString(std::string(asc128)) << "||Identifies how the data are recorded on this volume (0x023E-0x02BD)" << Qt::endl;
            snprintf(asc128, 38, "%s", s->svd.copy_id);
            proplist << "Copyright File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Fielname of a file in the root directory that contains copyright information for the volume set (0x02BE-0x02E3)" << Qt::endl;
            snprintf(asc128, 37, "%s", s->svd.abs_id);
            proplist << "Abstract File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains abstract information for the volume set (0x02E4-0x0307)" << Qt::endl;
            snprintf(asc128, 37, "%s", s->svd.bib_id);
            proplist << "Bibliographic File Identifier||" << QString::fromStdString(std::string(asc128)) << "||Filename of a file in the root directory that contains bibliographic information for this volume set (0x0308-0x032C)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", ((char)(s->svd.make_date.month[0])), (char)s->svd.make_date.month[1], (char)s->svd.make_date.day[0], (char)s->svd.make_date.day[1], (char)s->svd.make_date.year[0], (char)s->svd.make_date.year[1], (char)s->svd.make_date.year[2], (char)s->svd.make_date.year[3], (char)s->svd.make_date.hour[0], (char)s->svd.make_date.hour[1], (char)s->svd.make_date.min[0], (char)s->svd.make_date.min[1], (char)s->svd.make_date.sec[0], (char)s->svd.make_date.sec[1]);
            proplist << "Volume Creation Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.make_date.gmt_off) << "||The date and time when the volume was created (0x032D-0x033D)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.mod_date.month[0], (char)s->svd.mod_date.month[1], (char)s->svd.mod_date.day[0], (char)s->svd.mod_date.day[1], (char)s->svd.mod_date.year[0], (char)s->svd.mod_date.year[1], (char)s->svd.mod_date.year[2], (char)s->svd.mod_date.year[3], (char)s->svd.mod_date.hour[0], (char)s->svd.mod_date.hour[1], (char)s->svd.mod_date.min[0], (char)s->svd.mod_date.min[1], (char)s->svd.mod_date.sec[0], (char)s->svd.mod_date.sec[1]);
            proplist << "Volume Modification Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.mod_date.gmt_off) << "||The date and time when the volume was modified (0x033E-0x034E)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.exp_date.month[0], (char)s->svd.exp_date.month[1], (char)s->svd.exp_date.day[0], (char)s->svd.exp_date.day[1], (char)s->svd.exp_date.year[0], (char)s->svd.exp_date.year[1], (char)s->svd.exp_date.year[2], (char)s->svd.exp_date.year[3], (char)s->svd.exp_date.hour[0], (char)s->svd.exp_date.hour[1], (char)s->svd.exp_date.min[0], (char)s->svd.exp_date.min[1], (char)s->svd.exp_date.sec[0], (char)s->svd.exp_date.sec[1]);
            proplist << "Volume Expiration Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.exp_date.gmt_off) << "||The date and time after which the volume is considered to be obsolete. If not specified, then the volume is never considered to be obsolete (0x034F-0x035F)" << Qt::endl;
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.ef_date.month[0], (char)s->svd.ef_date.month[1], (char)s->svd.ef_date.day[0], (char)s->svd.ef_date.day[1], (char)s->svd.ef_date.year[0], (char)s->svd.ef_date.year[1], (char)s->svd.ef_date.year[2], (char)s->svd.ef_date.year[3], (char)s->svd.ef_date.hour[0], (char)s->svd.ef_date.hour[1], (char)s->svd.ef_date.min[0], (char)s->svd.ef_date.min[1], (char)s->svd.ef_date.sec[0], (char)s->svd.ef_date.sec[1]);
            proplist << "Volume Effective Date/Time||" << QString::fromStdString(std::string(asc)) + ConvertGmtHours(s->svd.ef_date.gmt_off) << "||The date and time after which the volume may be used. If not specified, then the volume may be used immediately (0x0360-0x0370)" << Qt::endl;
            proplist << "File Structure Version||" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.fs_ver)) << "||The directory records and path table version, always 0x01 (0x0371-0x0371)" << Qt::endl;
            proplist << "Unused||" << "Unused" << "||Unused, always 0x00 (0x0372-0x0372)" << Qt::endl;
            snprintf(asc, 512, "%s", s->svd.app_use);
            proplist << "Application Used||" << QString::fromStdString(std::string(asc)) << "||Application Used. Contents not defined by ISO9660 (0x0373-0x0572)" << Qt::endl;
            proplist << "Reserved by ISO||" << "Reserved" << "||Reserved by ISO (0x0573-0x07FF)" << Qt::endl;
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_YAFFS2) // YAFFS2 file system info
    {
        yfs = (YAFFSFS_INFO*)curfsinfo;
        unsigned int objcnt;
        uint32_t objfirst, objlast, vercnt, verfirst, verlast;
        proplist << "Page Size||" << QString::number(yfs->page_size) << "||Page Size" << Qt::endl;
        proplist << "Spare Size||" << QString::number(yfs->spare_size) << "||Spare Size" << Qt::endl;
        proplist << "Spare Offset Sequence Number||" << QString::number(yfs->spare_seq_offset) << "||Spare Offset Sequence Number" << Qt::endl;
        proplist << "Spare Offset Object ID||" << QString::number(yfs->spare_obj_id_offset) << "||Spare offset object id" << Qt::endl;
        proplist << "Spare Offset Chunk ID||" << QString::number(yfs->spare_chunk_id_offset) << "||Spare offset chunk id" << Qt::endl;
        proplist << "Spare Offset Number of Bytes||" << QString::number(yfs->spare_nbytes_offset) << "||Spare offset number of bytes" << Qt::endl;
        yaffscache_objects_stats(yfs, &objcnt, &objfirst, &objlast, &vercnt, &verfirst, &verlast);
        proplist << "Number of Allocated Objects||" << QString::number(objcnt) << "||Number of Allocated Objects" << Qt::endl;
        proplist << "Object ID Range||" << QString::number(objfirst) + " - " + QString::number(objlast) << "||Object id range" << Qt::endl;
        proplist << "Number of Total Object Versions||" << QString::number(vercnt) << "||Number of total object versions" << Qt::endl;
        proplist << "Object Version Range||" << QString::number(verfirst) + " - " + QString::number(verlast) << "||Object version range" << Qt::endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_HFS) // hfs file system info (last of the tsk defined ones)
    {
        hfs = (HFS_INFO*)curfsinfo;
        hsb = hfs->fs;
        time_t mactime;
        sprintf(asc, "0x%X%X %c%c", hsb->signature[0], hsb->signature[1], hsb->signature[0], hsb->signature[1]);
        proplist << "Signature||" << QString::fromStdString(std::string(asc)) << "||Signature value 0x4244 for \"BD\", 0x482B for \"H+\" and 0x4858 for \"HX\" (0x00-0x01)" << Qt::endl;
        proplist << "Version||" << QString::number(tsk_getu16(curfsinfo->endian, hsb->version)) << "||Version number. 4 for HFS+, 5 for HFSX (0x02-0x03)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_UNMOUNTED)
            proplist << "Volume was Unmounted Properly";
        else
            proplist << "Volume was Unmounted Improperly";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 8 determines if the volume was unmounted properly (0x04-0x07)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_BADBLOCKS)
            proplist << "Volume has Bad Blocks";
        else
            proplist << "Volume does not have Bad Blocks";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 9 determines if there are bad blocks for the volume (0x04-0x07)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_NOCACHE)
            proplist << "Volume is not Cached";
        else
            proplist << "Volume is Cached";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 10 determines if the volume should not be cached (0x04-0x07)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_INCONSISTENT)
            proplist << "Volume is Inconsistent";
        else
            proplist << "Volume is Consistent";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 11 determines if the volume was unmounted properly (0x04-0x07)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_CNIDS_REUSED)
            proplist << "CNIDs are Being Reused";
        else
            proplist << "CNIDs are not Being Reused";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 12 determines if the CNIDs have wrapped around past the maximum value and are being reused (0x04-0x07)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_JOURNALED)
            proplist << "Journaling Enabled";
        else
            proplist << "Journaling Disabled";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 13 determines if journaling is enabled for the volume (0x04-0x07)" << Qt::endl;
        proplist << "Volume Attributes||";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_SOFTWARE_LOCK)
            proplist << "Volume should be Write-Protected in Software";
        else
            proplist << "Volume is not Write-Protected in Software";
        proplist << "||Attribute values, bits 0-7,14,16-31 are reserved. Bit 15 determines if the volume should be write-protected (0x04-0x07)" << Qt::endl;
        sprintf(asc, "0x%X%X%X%X %c%c%c%c", hsb->last_mnt_ver[0], hsb->last_mnt_ver[1], hsb->last_mnt_ver[2], hsb->last_mnt_ver[3], hsb->last_mnt_ver[0], hsb->last_mnt_ver[1], hsb->last_mnt_ver[2], hsb->last_mnt_ver[3]);
        proplist << "Last Mounted By||" << QString::fromStdString(std::string(asc)) << "||\"10.0\" for Mac OS X, \"HFSJ\" for journaled HFS+ on Mac OS X, \"FSK!\" for failed journal replay, \"fsck\" for fsck_hfs and \"8.10\" for Mac OS 8.1-9.2.2 (0x08-0x0B)" << Qt::endl;
        proplist << "Journal Info Block||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->jinfo_blk)) << "||Journal information block (0x0C-0x0F)" << Qt::endl;
        mactime = hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->cr_date));
        sprintf(asc, "%s", tsk_fs_time_to_str(mktime(gmtime(&mactime)), timebuf));
        proplist << "Volume Creation TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC from Local Time (0x10-0x13)" << Qt::endl;
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->m_date)), timebuf));
        proplist << "Volume Last Modified TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC (0x14-0x17)" << Qt::endl;
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->bkup_date)), timebuf));
        proplist << "Volume Backup TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC (0x18-0x1B)" << Qt::endl;
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->chk_date)), timebuf));
        proplist << "Volume Checked TimeStamp||" << QString::fromStdString(std::string(asc)) << "||MACTIME32 converted to UTC (0x1C-0x1F)" << Qt::endl;
        proplist << "File Count||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->file_cnt)) << "||Number of files on volume, not including special files (0x20-0x23)" << Qt::endl;
        proplist << "Folder Count||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->fldr_cnt)) << "||Number of folders on volume, not including the root directory (0x24-0x27)" << Qt::endl;
        proplist << "Allocation Block Size||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->blk_sz)) << "||Allocation block size in bytes (0x28-0x2B)" << Qt::endl;
        proplist << "Allocation Block Total||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->blk_cnt)) << "||Number of blocks on disk (0x2C-0x2F)" << Qt::endl;
        proplist << "Allocation Block Free||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->free_blks)) << "||Unused block count (0x30-0x33)" << Qt::endl;
        proplist << "Next Allocation||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->next_alloc)) << "||Block address to start the allocation search (0x34-0x37)" << Qt::endl;
        proplist << "Resource Clump Size||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->rsrc_clmp_sz)) << "||Default clump size for resource forks in bytes (0x38-0x3B)" << Qt::endl;
        proplist << "Data Clump Size||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->data_clmp_sz)) << "||Default clump size for data forks in bytes (0x3C-0x3F)" << Qt::endl;
        proplist << "Next Catalog ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->next_cat_id)) << "||Next catalog node id for allocation (0x40-0x43)" << Qt::endl;
        proplist << "Write Count||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->write_cnt)) << "||Write count incremented each time it is mounted and modified (0x44-0x47)" << Qt::endl;
        proplist << "Encoding Bitmap||" << QString::number(tsk_getu64(curfsinfo->endian, hsb->enc_bmp)) << "||Encoding bitmap identifies which encodings were used in the file system (0x48-0x4F)" << Qt::endl;
        proplist << "Bootable Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOT])) << "||Bootable folder id (0x50-0x53)" << Qt::endl;
        proplist << "Startup App ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_START])) << "||Startup app id (0x54-0x57)" << Qt::endl;
        proplist << "Startup Open Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_OPEN])) << "||Startup open folder id (0x58-0x5B)" << Qt::endl;
        proplist << "Mac OS 8/9 Blessed System Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOT9])) << "||Mac OS 8/9 blessed system folder id (0x5C-0x5F)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x60-0x63)" << Qt::endl;
        proplist << "Mac OS X Blessed System Folder ID||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOTX])) << "||Mac OS X blessed system folder id (0x64-0x67)" << Qt::endl;
        sprintf(asc, "%08" PRIx32 "%08" PRIx32, tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_ID1]), tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_ID2]));
        proplist << "Volume Identifier||" << QString::fromStdString(std::string(asc)) << "||Volume identifier (0x068-0x6F)" << Qt::endl;
        proplist << "Fork Data Allocation File||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->start_blk)) + "," + QString::number(tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->blk_cnt)) << "||Location and size of allocation bitmap files (0x70-0xBF)" << Qt::endl;
        hfsreserveinfo = "A|";
        //hfsreserveinfo = "A|" + QString::number((tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->start_blk);
        //hfsreserveinfo = "A|" + QString::number((tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->start_blk) * curfsinfo->block_size) + curfsinfo->offset);
        //QString tmpstring = QString::number(tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->start_blk)) + "^^";
        for(uint i=0; i < tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->blk_cnt); i++)
        {
            hfsreserveinfo += QString::number(tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->start_blk) + i) + "^^";
            //tmpstring += QString::number(tsk_getu32(curfsinfo->endian, hsb->alloc_file.extents->start_blk) + i) + "^^";
        }
        hfsreserveinfo += ",";
        proplist << "Fork Data Catalog File||" << QString::number(tsk_getu32(curfsinfo->endian, hsb->cat_file.extents->start_blk)) + "," + QString::number(tsk_getu32(curfsinfo->endian, hsb->cat_file.extents->blk_cnt)) << "||Location and size of catalog file (0x0110-0x015F)" << Qt::endl;
        hfsreserveinfo += "C|";
        for(uint i=0; i < tsk_getu32(curfsinfo->endian, hsb->cat_file.extents->blk_cnt); i++)
        {
            hfsreserveinfo += QString::number(tsk_getu32(curfsinfo->endian, hsb->cat_file.extents->start_blk) + i) + "^^";
        }
        proplist << "Fork Data Extents File||";
        if(hfs->has_extents_file)
        {
            hfsreserveinfo += ",E|";
            for(uint i=0; i < tsk_getu32(curfsinfo->endian, hsb->ext_file.extents->blk_cnt); i++)
            {
                hfsreserveinfo += QString::number(tsk_getu32(curfsinfo->endian, hsb->ext_file.extents->start_blk) + i) + "^^";
            }
            proplist << QString::number(tsk_getu32(curfsinfo->endian, hsb->ext_file.extents->start_blk)) + "," + QString::number(tsk_getu32(curfsinfo->endian, hsb->ext_file.extents->blk_cnt));
        }
        else
            proplist << "";
        proplist << "||Location and size of extents file (0x00C0-0x010F)" << Qt::endl;
        proplist << "Fork Data Attributes File||";
        if(hfs->has_attributes_file)
        {
            hfsreserveinfo += ",B|";
            for(uint i=0; i < tsk_getu32(curfsinfo->endian, hsb->attr_file.extents->blk_cnt); i++)
            {
                hfsreserveinfo += QString::number(tsk_getu32(curfsinfo->endian, hsb->attr_file.extents->start_blk) + i) + "^^";
            }
            proplist << QString::number(tsk_getu32(curfsinfo->endian, hsb->attr_file.extents->start_blk)) + "," + QString::number(tsk_getu32(curfsinfo->endian, hsb->attr_file.extents->blk_cnt));
        }
        else
            proplist << "";
        proplist << "||Location and size of attributes file (0x0160-0x01AF)" << Qt::endl;
        proplist << "Fork Data Startup File||";
        if(hfs->has_startup_file)
        {
            hfsreserveinfo += ",S|";
            for(uint i=0; i < tsk_getu32(curfsinfo->endian, hsb->start_file.extents->blk_cnt); i++)
            {
                hfsreserveinfo += QString::number(tsk_getu32(curfsinfo->endian, hsb->start_file.extents->start_blk) + i) + "^^";
            }
            proplist << QString::number(tsk_getu32(curfsinfo->endian, hsb->start_file.extents->start_blk)) + "," + QString::number(tsk_getu32(curfsinfo->endian, hsb->start_file.extents->blk_cnt));
        }
        else
            proplist << "";
        //qDebug() << "hfsreserveinfo" << hfsreserveinfo;
        proplist << "||Location and size of startup file (0x01B0-0x01FF)" << Qt::endl;
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_APFS)
    {
        IMG_POOL_INFO* pool_img = (IMG_POOL_INFO*)curfsinfo->img_info;
        const auto pool = static_cast<APFSPoolCompat*>(pool_img->pool_info->impl);
        const APFSPool* curpool = (APFSPool*)pool;
        bool isencrypted = false;
        for(int i=0; i < pool->num_vols(); i++)
        {
            TSK_POOL_VOLUME_INFO curpoolvol = pool_img->pool_info->vol_list[i];
            if(curpoolvol.block == pool_img->pvol_block)
            {
                if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                    isencrypted = true;
            }
        }
        // IF not encrypted...
        APFSFileSystem* curvol = NULL;
        //qDebug() << "partitionpath:" << partitionpath;
        QString passkey = partitionpath.split("/", Qt::SkipEmptyParts).at(partitionpath.split("/", Qt::SkipEmptyParts).count() - 3).split(".e").first() + "-" + partitionpath.split("/", Qt::SkipEmptyParts).at(partitionpath.split("/", Qt::SkipEmptyParts).count() - 2) + "-" + partitionpath.split("/", Qt::SkipEmptyParts).last();
        //qDebug() << "password key:" << QString(partitionpath.split("/", Qt::SkipEmptyParts).at(partitionpath.split("/", Qt::SkipEmptyParts).count() - 3).split(".e").first() + "-" + partitionpath.split("/", Qt::SkipEmptyParts).at(partitionpath.split("/", Qt::SkipEmptyParts).count() - 2) + "-" + partitionpath.split("/", Qt::SkipEmptyParts).last());
        //qDebug() << "passkey:" << passkey << "password:" << passwordhash.value(passkey);
        if(isencrypted)
            curvol = new APFSFileSystem(*curpool, (apfs_block_num)(pool_img->pvol_block), passwordhash.value(passkey).toStdString().c_str());
        else
            curvol = new APFSFileSystem(*curpool, (apfs_block_num)(pool_img->pvol_block));
        proplist << "Volume UUID||" << QString::fromStdString(curvol->uuid().str()) << "||The universally unique identifier for this volume." << Qt::endl;
        proplist << "APSB Block Number||" << QString::number(curvol->block_num()) << "||The block number for the APFS SuperBlock." << Qt::endl;
        proplist << "APSB oid||" << QString::number(curvol->oid()) << "||The APFS SuperBlock Object IDentifier." << Qt::endl;
        proplist << "APSB xid||" << QString::number(curvol->xid()) << "||The APFS SuperBlock Transaction IDentifier." << Qt::endl;
        proplist << "Volume Name||" << QString::fromStdString(curvol->name()) << "||The APFS Volume name." << Qt::endl;
        proplist << "Volume Role||";
        switch(curvol->role())
        {
            case APFS_VOLUME_ROLE_NONE:
                proplist << "No specific role";
                break;
            case APFS_VOLUME_ROLE_SYSTEM:
                proplist << "System";
                break;
            case APFS_VOLUME_ROLE_USER:
                proplist << "User";
                break;
            case APFS_VOLUME_ROLE_RECOVERY:
                proplist << "Recovery";
                break;
            case APFS_VOLUME_ROLE_VM:
                proplist << "Virtual Machine";
                break;
            case APFS_VOLUME_ROLE_PREBOOT:
                proplist << "Preboot";
                break;
            default:
                proplist << "Unknown";
                break;
        }
        proplist << "||Defined Role for the specified volume." << Qt::endl;
        proplist << "Capacity Consumed||" << QString::number(curvol->used()) + " bytes" << "||Capacity consumed." << Qt::endl;
        proplist << "Capacity Reserved||" << QString::number(curvol->reserved()) + " bytes" << "||Capacity reserved." << Qt::endl;
        proplist << "Capacity Quota||" << QString::number(curvol->quota()) + " bytes" << "||Capacity quota." << Qt::endl;
        proplist << "Case Sensitive||";
        if(curvol->case_sensitive())
            proplist << "Yes";
        else
            proplist << "No";
        proplist << "||File names on this volume are case (in)sensitive." << Qt::endl;
        proplist << "Encrypted||";
        if(curvol->encrypted())
        {
            QString tmpstr = "Yes";
            if(curpool->hardware_crypto())
                tmpstr += " (hardware assisted)";
            proplist << tmpstr;
        }
        else
            proplist << "No";
        proplist << "||Volume is encrypted or not encrypted and is hardware assisted or not (TPM)" << Qt::endl;
        if(curvol->encrypted() && !curpool->hardware_crypto())
        {
            const auto crypto = curvol->crypto_info();
            if(crypto.unlocked)
                proplist << "Password||" << QString::fromStdString(crypto.password) << "||APFS Volume Password." << Qt::endl;
            proplist << "Password Hint||" << QString::fromStdString(crypto.password_hint) << "||APFS Volume Password hint." << Qt::endl;

        }
        proplist << "Formatted by||" << QString::fromStdString(curvol->formatted_by()) << "||Method used to formatted volume." << Qt::endl;
        char time_buf[1024];
        tsk_fs_time_to_str_subsecs(curvol->created() / 1000000000, curvol->created() % 1000000000, time_buf);
        proplist << "Created||" << QString(time_buf) << "||APFS volume creation time." << Qt::endl;
        tsk_fs_time_to_str_subsecs(curvol->changed() / 1000000000, curvol->changed() % 1000000000, time_buf);
        proplist << "Changed||" << QString(time_buf) << "||APFS volume changed time." << Qt::endl;
    }
    proplist << "Endian Ordering||";
    if(curfsinfo->endian == TSK_UNKNOWN_ENDIAN)
        proplist << "Endianness is unknown";
    else if(curfsinfo->endian == TSK_LIT_ENDIAN)
        proplist << "Data is in Little Endian";
    else if(curfsinfo->endian == TSK_BIG_ENDIAN)
        proplist << "Data is in Big Endian";
    else
        proplist << "Endianness is unknown";
    proplist << "||Identifies the endian ordering of the data being read." << Qt::endl;
    proplist.flush();
    fspropfile.close();
}

void WriteVolumeProperties(TSK_VS_INFO* curvolinfo, QString volumepath)
{
    char asc[512];
    QFile vpropfile(volumepath + "prop");
    vpropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream proplist(&vpropfile);
    mac_part* macpart = NULL;
    bsd_disklabel* bsdpart = NULL;
    sun_dlabel_sparc* sunsparcpart = NULL;
    sun_dlabel_i386* sunx86part = NULL;
    gpt_head* gptpart = NULL;

    if(curvolinfo->vstype == TSK_VS_TYPE_DOS)
        proplist << "Signature||" << "0xAA55" << "||Signature Value should be 0xAA55 (0x1FE-0x1FF)" << Qt::endl;
    else if(curvolinfo->vstype == TSK_VS_TYPE_BSD)
    {
        char* sect_buf;
        ssize_t cnt;
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        sect_buf = (char*)tsk_malloc(curvolinfo->block_size);
        bsdpart = (bsd_disklabel*) sect_buf;
        cnt = tsk_vs_read_block(curvolinfo, BSD_PART_SOFFSET, sect_buf, curvolinfo->block_size);
        if(cnt != curvolinfo->block_size)
        {
            // print error here
        }
        free(sect_buf);
        proplist << "Signature||" << QString::number(tsk_getu32(endian, bsdpart->magic)) << "||Signature value should be 0x82564557 (0x00-0x03)" << Qt::endl;
        proplist << "Drive Type||" << QString::number(tsk_getu16(endian, bsdpart->type)) << "||Drive type (0x04-0x05)" << Qt::endl;
        proplist << "Drive Subtype||" << QString::number(tsk_getu16(endian, bsdpart->sub_type)) << "||Drive subtype (0x06-0x07)" << Qt::endl;
        proplist << "Drive Type Name||" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->type_name)) << "||The Drive type name (0x08-0x17)" << Qt::endl;
        proplist << "Pack Identifier Name||" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->packname)) << "||The Pack identifier name (0x18-0x27)" << Qt::endl;
        proplist << "Sector Size (bytes)||" << QString::number(tsk_getu32(endian, bsdpart->sec_size)) << "||Size of a sector in bytes (0x28-0x2B)" << Qt::endl;
        proplist << "Sectors per Track||" << QString::number(tsk_getu32(endian, bsdpart->sec_per_tr)) << "||Number of Sectors per track (0x2C-0x2F)" << Qt::endl;
        proplist << "Tracks per Cylinder||" << QString::number(tsk_getu32(endian, bsdpart->tr_per_cyl)) << "||Number of tracks per cylinder (0x30-0x33)" << Qt::endl;
        proplist << "Cylinders per Unit||" << QString::number(tsk_getu32(endian, bsdpart->cyl_per_unit)) << "||Number of cylinders per unit (0x34-0x37)" << Qt::endl;
        proplist << "Sectors per Cylinder||" << QString::number(tsk_getu32(endian, bsdpart->sec_per_cyl)) << "||Number of sectors per cylinder (0x38-0x3B)" << Qt::endl;
        proplist << "Sectors per Unit||" << QString::number(tsk_getu32(endian, bsdpart->sec_per_unit)) << "||Number of sectors per unit (0x3C-0x3F)" << Qt::endl;
        proplist << "Spare Sectors per Track||" << QString::number(tsk_getu16(endian, bsdpart->spare_per_tr)) << "||Number of spare sectors per track (0x40-0x41)" << Qt::endl;
        proplist << "Spare Sectors per Cylinder||" << QString::number(tsk_getu16(endian, bsdpart->spare_per_cyl)) << "||Number of spare sectors per cylinder (0x42-0x43)" << Qt::endl;
        proplist << "Alternate Cylinders Per Unit||" << QString::number(tsk_getu32(endian, bsdpart->alt_per_unit)) << "||Number of alternate cylinders per unit (0x44-0x47)" << Qt::endl;
        proplist << "Rotational Disk Speed||" << QString::number(tsk_getu16(endian, bsdpart->rpm)) << "||Rotational Speed of Disk (0x48-0x49)" << Qt::endl;
        proplist << "Hardware Sector Interleave||" << QString::number(tsk_getu16(endian, bsdpart->interleave)) << "||Hardware sector interleave (0x4A-0x4B)" << Qt::endl;
        proplist << "Track Skew||" << QString::number(tsk_getu16(endian, bsdpart->trackskew)) << "||Track skew (0x4C-0x4D)" << Qt::endl;
        proplist << "Cylinder Skew||" << QString::number(tsk_getu16(endian, bsdpart->cylskew)) << "||Cylinder skew (0x4E-0x4F)" << Qt::endl;
        proplist << "Head Switch Time||" << QString::number(tsk_getu32(endian, bsdpart->headswitch)) << "||Head switch time in microseconds (0x50-0x53)" << Qt::endl;
        proplist << "Track-to-Track Seek Time||" << QString::number(tsk_getu32(endian, bsdpart->track_seek)) << "||Track-to-Track seek time in microseconds (0x54-0x57)" << Qt::endl;
        proplist << "Flags||" << QString::number(tsk_getu32(endian, bsdpart->flags)) << "||Flags (0x58-0x5B)" << Qt::endl;
        proplist << "Drive Specific Data||" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->drivedata)) << "||Drive Specific Data (0x5C-0x6F)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x70-0x83)" << Qt::endl;
        proplist << "Signature||" << QString::number(tsk_getu32(endian, bsdpart->magic2)) << "||Signature value should be 0x82564557 (0x84-0x87)" << Qt::endl;
        proplist << "Checksum||" << QString::number(tsk_getu16(endian, bsdpart->checksum)) << "||Checksum (0x88-0x89)" << Qt::endl;
        proplist << "Number of Partitions||" << QString::number(tsk_getu16(endian, bsdpart->num_parts)) << "||Number of partitions (0x8A-0x8B)" << Qt::endl;
        proplist << "Boot Area Size||" << QString::number(tsk_getu32(endian, bsdpart->bootarea_size)) << "||Size of boot area (0x8C-0x8F)" << Qt::endl;
        proplist << "Super Block Maximum Size||" << QString::number(tsk_getu32(endian, bsdpart->sb_size)) << "||Maximum size of the file system boot super block (0x90-0x93)" << Qt::endl;
        proplist << "Unused||" << "Unused" << "||Unused (0x0194-0x01FF)" << Qt::endl;
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_SUN)
    {
        char* buf;
        ssize_t cnt;
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        buf = (char*)tsk_malloc(curvolinfo->block_size);
        cnt = tsk_vs_read_block(curvolinfo, SUN_SPARC_PART_SOFFSET, buf, curvolinfo->block_size);
        if(cnt != curvolinfo->block_size)
        {
            // print error here.
        }
        sunsparcpart = (sun_dlabel_sparc*)buf;
        sunx86part = (sun_dlabel_i386*)buf;
        if(tsk_vs_guessu16(curvolinfo, sunsparcpart->magic, SUN_MAGIC) == 0)
        {
            if(tsk_getu32(endian, sunsparcpart->sanity) == SUN_SANITY) // populate sparc
            {
                proplist << "ASCII Label||" << QString::fromUtf8(reinterpret_cast<char*>(sunsparcpart->asciilabel)) << "||ASCII label (0x00-0x7F)" << Qt::endl;
                proplist << "Version||" << QString::number(tsk_getu32(endian, sunsparcpart->version)) << "||Version (0x80-0x83)" << Qt::endl;
                proplist << "Volume Name||" << QString::fromUtf8(reinterpret_cast<char*>(sunsparcpart->vol_name)) << "||Volume Name (0x84-0x8C)" << Qt::endl;
                proplist << "Number of Partitions||" << QString::number(tsk_getu16(endian, sunsparcpart->num_parts)) << "||Number of partitions (0x8D-0x8E)" << Qt::endl;
                proplist << "Signature Sanity||" << QString::number(tsk_getu32(endian, sunsparcpart->sanity)) << "||Signature Value Sanity should be 0x600DDEEE (0xBC-0xBF)" << Qt::endl;
                proplist << "Writing Sectors to Skip||" << QString::number(tsk_getu16(endian, sunsparcpart->write_reinstruct)) << "||Sectors to skip, writing (0x0106-0x0107)" << Qt::endl;
                proplist << "Reading Sectors to Skip||" << QString::number(tsk_getu16(endian, sunsparcpart->read_reinstruct)) << "||Sectors to skip, reading (0x0108-0x0109)" << Qt::endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x010A-0x01A3)" << Qt::endl;
                proplist << "Disk Speed||" << QString::number(tsk_getu16(endian, sunsparcpart->rpm)) << "||Disk Speed (0x01A4-0x01A5)" << Qt::endl;
                proplist << "Physical Cylinder Count||" << QString::number(tsk_getu16(endian, sunsparcpart->num_ph_cyl)) << "||Number of physical cylinders (0x01A6-0x01A7)" << Qt::endl;
                proplist << "Alternates per Cylinder||" << QString::number(tsk_getu16(endian, sunsparcpart->alt_per_cyl)) << "||Alternates per cylinder (0x01A8-0x01A9)" << Qt::endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01AA-0x01AD)" << Qt::endl;
                proplist << "Interleave||" << QString::number(tsk_getu16(endian, sunsparcpart->interleave)) << "||Interleave (0x01AE-0x01AF)" << Qt::endl;
                proplist << "Data Cylinder Count||" << QString::number(tsk_getu16(endian, sunsparcpart->num_cyl)) << "||Number of data cylinders (0x01B0-0x01B1)" << Qt::endl;
                proplist << "Alternate Cylinder Count||" << QString::number(tsk_getu16(endian, sunsparcpart->num_alt_cyl)) << "||Number of alternate cylinders (0x01B2-0x01B3)" << Qt::endl;
                proplist << "Number of Heads||" << QString::number(tsk_getu16(endian, sunsparcpart->num_head)) << "||Number of heads (0x01B4-0x01B5)" << Qt::endl;
                proplist << "Sectors per Track Count||" << QString::number(tsk_getu16(endian, sunsparcpart->sec_per_tr)) << "||Number of sectors per track (0x01B6-0x01B7)" << Qt::endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01B8-0x01BB)" << Qt::endl;
                proplist << "Signature||" << QString::number(tsk_getu16(endian, sunsparcpart->magic)) << "||Signature value should be 0xDABE (0x01FC-0x01FD)" << Qt::endl;
                proplist << "Checksum||" << QString::number(tsk_getu16(endian, sunsparcpart->checksum)) << "||Checksum (0x01FE-0x01FF)" << Qt::endl;
            }
            else if(tsk_getu32(endian, sunx86part->sanity) == SUN_SANITY) // populate i386
            {
                proplist << "Signature Sanity||" << QString::number(tsk_getu32(endian, sunx86part->sanity)) << "||Signature Value Sanity should be 0x600DDEEE (0x0C-0x0F)" << Qt::endl;
                proplist << "Version||" << QString::number(tsk_getu32(endian, sunx86part->version)) << "||Version (0x10-0x13)" << Qt::endl;
                proplist << "Volume Name||" << QString::fromUtf8(reinterpret_cast<char*>(sunx86part->vol_name)) << "||Volume Name (0x14-0x1B)" << Qt::endl;
                proplist << "Sector Size||" << QString::number(tsk_getu16(endian, sunx86part->sec_size)) << "||Sector sie (0x1C-0x1D)" << Qt::endl;
                proplist << "Number of Partitions||" << QString::number(tsk_getu16(endian, sunx86part->num_parts)) << "||Number of partitions (0x1E-0x1F)" << Qt::endl;
                proplist << "Volume Label||" << QString::fromUtf8(reinterpret_cast<char*>(sunx86part->asciilabel)) << "||Volume label (0x0148-0x01C7)" << Qt::endl;
                proplist << "Physical Cylinder Count||" << QString::number(tsk_getu32(endian, sunx86part->num_ph_cyl)) << "||Number of physical cylinders (0x01C8-0x01CB)" << Qt::endl;
                proplist << "Data Cylinder Count||" << QString::number(tsk_getu32(endian, sunx86part->num_cyl)) << "||Number of data cylinders (0x01CC-0x01CF)" << Qt::endl;
                proplist << "Alternate Cylinder Count||" << QString::number(tsk_getu16(endian, sunx86part->num_alt_cyl)) << "||Number of alternate cylinders (0x01D0-0x01D1)" << Qt::endl;
                proplist << "Cylinder offset||" << QString::number(tsk_getu16(endian, sunx86part->cyl_offset)) << "||Cylinder offset (0x01D2-0x01D3)" << Qt::endl;
                proplist << "Number of Heads||" << QString::number(tsk_getu32(endian, sunx86part->num_head)) << "||Number of heads (0x01D4-0x01D7)" << Qt::endl;
                proplist << "Sectors per Track Count||" << QString::number(tsk_getu32(endian, sunx86part->sec_per_tr)) << "||Number of sectors per track (0x01D8-0x01DB)" << Qt::endl;
                proplist << "Interleave||" << QString::number(tsk_getu16(endian, sunx86part->interleave)) << "||Interleave (0x01DC-0x01DD)" << Qt::endl;
                proplist << "Skew||" << QString::number(tsk_getu16(endian, sunx86part->skew)) << "||Skew (0x01DE-0x01DF)" << Qt::endl;
                proplist << "Alternates per Cylinder||" << QString::number(tsk_getu16(endian, sunx86part->alt_per_cyl)) << "||Number of alternates per cylinder (0x01E0-0x01E1)" << Qt::endl;
                proplist << "Disk Speed||" << QString::number(tsk_getu16(endian, sunx86part->rpm)) << "||Disk Speed (0x01E2-0x01E3)" << Qt::endl;
                proplist << "Writing Sectors to Skip||" << QString::number(tsk_getu16(endian, sunx86part->write_reinstruct)) << "||Sectors to skip, writing (0x01E4-0x01E5)" << Qt::endl;
                proplist << "Reading Sectors to Skip||" << QString::number(tsk_getu16(endian, sunx86part->read_reinstruct)) << "||Sectors to skip, reading (0x01E6-0x01E7)" << Qt::endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01E8-0x01EF)" << Qt::endl;
                proplist << "Reserved||" << "Reserved" << "||Reserved (0x01F0-0x0x01FB)" << Qt::endl;
                proplist << "Signature||" << QString::number(tsk_getu16(endian, sunx86part->magic)) << "||Signature value should be 0xDABE (0x01FC-0x01FD)" << Qt::endl;
                proplist << "Checksum||" << QString::number(tsk_getu16(endian, sunx86part->checksum)) << "||Checksum (0x01FE-0x01FF)" << Qt::endl;
            }
            free(buf);
        }
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_MAC)
    {
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        macpart = (mac_part*)curvolinfo;
        proplist << "Signature||" << "0x504D" << "||Signature Value should be 0x504D (0x00-0x01)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x02-0x03)" << Qt::endl;
        proplist << "Number of Partitions||" << QString::number(tsk_getu32(endian, macpart->pmap_size)) << "||Total Number of Partitions (0x08-0x0B)" << Qt::endl;
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_GPT)
    {
        char* sect_buf;
        ssize_t cnt;
        TSK_ENDIAN_ENUM endian = curvolinfo->endian;
        sect_buf = (char*)tsk_malloc(curvolinfo->block_size);
        gptpart = (gpt_head*)sect_buf;
        cnt = tsk_vs_read_block(curvolinfo, GPT_PART_SOFFSET + 1, sect_buf, curvolinfo->block_size);
        if(cnt != curvolinfo->block_size)
        {
            // print error here
        }
        free(sect_buf);
        proplist << "Signature||" << QString::number(tsk_getu64(endian, gptpart->signature)) << "||Signature Value should be EFI PART, 0x5452415020494645 (0x00-0x07)" << Qt::endl;
        proplist << "Version||" << QString::number(tsk_getu32(endian, gptpart->version)) << "||Version (0x08-0x0B)" << Qt::endl;
        proplist << "GPT Header Size (bytes)||" << QString::number(tsk_getu32(endian, gptpart->head_size_b)) << "||Size of GPT header in bytes (0x0C-0x0F)" << Qt::endl;
        proplist << "GPT Header Checksum||" << QString::number(tsk_getu32(endian, gptpart->head_crc)) << "||CRC32 checksum of GPT header (0x10-0x13)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x14-0x17)" << Qt::endl;
        proplist << "Current GPT Header LBA||" << QString::number(tsk_getu64(endian, gptpart->head_lba)) << "||Logical Block Address of the current GPT header structure (0x18-0x1F)" << Qt::endl;
        proplist << "Other GPT Header LBA||" << QString::number(tsk_getu64(endian, gptpart->head2_lba)) << "||Logical Block Address of the other GPT header structure (0x20-0x27)" << Qt::endl;
        proplist << "Partition Area Start LBA||" << QString::number(tsk_getu64(endian, gptpart->partarea_start)) << "||Logical Block Address of the start of the partition area (0x28-0x2F)" << Qt::endl;
        proplist << "Partition End Area LBA||" << QString::number(tsk_getu64(endian, gptpart->partarea_end)) << "||Logical Block Address of the end of the partition area (0x30-0x37)" << Qt::endl;
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(endian, &(gptpart->guid[8])), tsk_getu64(endian, &(gptpart->guid[0])));
        proplist << "Disk GUID||" << QString::fromStdString(std::string(asc)) << "||Disk GUID (0x38-0x47)" << Qt::endl;
        proplist << "LBA of Start of the Partition Table||" << QString::number(tsk_getu64(endian, gptpart->tab_start_lba)) << "||Logical Block Address of the start of the partition table (0x48-0x4F)" << Qt::endl;
        proplist << "Number of Partition Table Entries||" << QString::number(tsk_getu32(endian, gptpart->tab_num_ent)) << "||Number of entries in the partition table (0x50-0x53)" << Qt::endl;
        proplist << "Partition Table Entry Size||" << QString::number(tsk_getu32(endian, gptpart->tab_size_b)) << "||Size of each entry in the partition table (0x54-0x57)" << Qt::endl;
        proplist << "Partition Table Checksum||" << QString::number(tsk_getu32(endian, gptpart->tab_crc)) << "||CRC32 checksum of the partition table (0x58-0x5B)" << Qt::endl;
        proplist << "Reserved||" << "Reserved" << "||Reserved (0x5C-0x01FF)" << Qt::endl;
    }
    else if(curvolinfo->vstype == TSK_VS_TYPE_APFS)
    {
        qDebug() << "apfs stuff here...";
    }
    proplist.flush();
    vpropfile.close();
}

void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo, QString evidencepath, QString imgfullpath)
{
    QFile epropfile(evidencepath + "prop");
    epropfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream proplist(&epropfile);
    IMG_EWF_INFO* ewfinfo = NULL;
    uint8_t* ewfvalue = (uint8_t*)malloc(sizeof(uint8_t)*64);
    uint8_t uvalue8bit = 0;
    int8_t value8bit = 0;
    uint32_t value32bit = 0;
    uint64_t value64bit = 0;
    size64_t size64bit = 0;
    libewf_error_t* ewferror = NULL;
    proplist << QString("File Format||") << QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)curimginfo->itype)) << QString("||File Format the evidence data is stored in. Usually it is either a raw image (.dd/.001) or an embedded image (.E01/.AFF). A raw image contains only the data from the evidence. The embedded image contains other descriptive information from the acquisition.") << Qt::endl;
    proplist << QString("Sector Size||") << QString(QString::number(curimginfo->sector_size) + " bytes||") << QString("Sector size of the device. A Sector is a subdivision of a disk where data is stored. It is the smallest value used to divide the disk.") << Qt::endl;
    proplist << QString("Sector Count||") << QString(QString::number((int)((float)curimginfo->size/(float)curimginfo->sector_size)) + " sectors||") << QString("The number of sectors in the disk.") << Qt::endl;
    proplist << QString("Image Path||") << imgfullpath << QString("||Location where the evidence image is stored and read from.") << Qt::endl;
    if(TSK_IMG_TYPE_ISAFF(curimginfo->itype)) // its AFF
    {
    }
    else if(TSK_IMG_TYPE_ISEWF(curimginfo->itype)) // its EWF
    {
        ewfinfo = (IMG_EWF_INFO*)curimginfo;
        if(libewf_handle_get_utf8_header_value_case_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Case Number||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||The case number the image is associated" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_description(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Description||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Description of the acquisition and or evidence item" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_examiner_name(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Examiner Name||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Name of the examiner who acquired the image" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_evidence_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Evidence Number||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Unique number identifying the evidence item" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_notes(ewfinfo->handle, ewfvalue, 255, &ewferror) == 1)
            proplist << "Notes||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Any notes related to the acquisition" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Acquisition Date||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Date the acquisition was made" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_system_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "System Date||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Date for the system acquiring the image" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_operating_system(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Aquisition OS||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Operating System acquiring the image" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_software_version(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Software Version Used||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Version of the software used to acquire the image" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_password(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Password||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Password to protect the image" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_model(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Model||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Model of the drive acquired" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_serial_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Serial Number||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||Serial number of the drive acquired" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_sectors_per_chunk(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << QString("Sectors Per Chunk||") << QString::number(value32bit) << "||Number of sectors in a image evidence chunk" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_format(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            proplist << QString("File Format||");
            switch(uvalue8bit)
            {
                case LIBEWF_FORMAT_EWF:
                    proplist << QString("Original EWF") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_SMART:
                    proplist << QString("SMART") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                //case LIBEWF_FORMAT_FTK:
                //    proplist << QString("FTK Imager") << "||Format used to store the evidence image" << Qt::endl;
                //    break;
                case LIBEWF_FORMAT_ENCASE1:
                    proplist << QString("EnCase 1") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_ENCASE2:
                    proplist << QString("EnCase 2") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_ENCASE3:
                    proplist << QString("EnCase 3") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_ENCASE4:
                    proplist << QString("EnCase 4") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_ENCASE5:
                    proplist << QString("EnCase 5") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_ENCASE6:
                    proplist << QString("EnCase 6") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_LINEN5:
                    proplist << QString("Linen 5") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_LINEN6:
                    proplist << QString("Linen 6") << "||Format used to store the evidence image" << Qt::endl;
                    break;
                case LIBEWF_FORMAT_EWFX:
                    proplist << QString("EWFX (extended ewf)") << QString("||Extended EWF Format used to store the evidence image") << Qt::endl;
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE5:
                    proplist << QString("LEF EnCase 5") << QString("||Logical Evidence File EnCase 5 Format used to store the evidence image") << Qt::endl;
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE6:
                    proplist << QString("LEF EnCase 6") << QString("||Logical Evidence File EnCase 6 Format used to store the evidence image") << Qt::endl;
                    break;
                case LIBEWF_FORMAT_UNKNOWN:
                    proplist << QString("Unknown Format") << "||Format used to store the evidence image" << Qt::endl;
                    break;
            }
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_error_granularity(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << QString("Error Granularity||") << QString::number(value32bit) << "||Error block size" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        proplist << "Compression Method||" << "Deflate" << "||Method used to compress the image" << Qt::endl;
        if(libewf_handle_get_compression_values(ewfinfo->handle, &value8bit, &uvalue8bit, &ewferror) == 1)
        {
            proplist << "Compression Level||";
            if(value8bit == LIBEWF_COMPRESSION_NONE)
                proplist << "No Compression";
            else if(value8bit == LIBEWF_COMPRESSION_FAST)
                proplist << "Good (Fast) Compression";
            else if(value8bit == LIBEWF_COMPRESSION_BEST)
                proplist << "Best Compression";
            else
                proplist << "Unknown Compression";
            proplist << "||The more compression, the slower the acquisition but the smaller the file size" << Qt::endl;
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_type(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            proplist << "Media Type||";
            if(uvalue8bit == LIBEWF_MEDIA_TYPE_REMOVABLE)
                proplist << "Removable Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_FIXED)
                proplist << "Fixed Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_SINGLE_FILES)
                proplist << "Single Files";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_OPTICAL)
                proplist << "Optical Disk (CD/DVD/BD)";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_MEMORY)
                proplist << "Memory (RAM)";
            else
                proplist << "Unknown";
            proplist << "||Media type of the original evidence" << Qt::endl;
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_flags(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_PHYSICAL)
                proplist << "Media Flag||" << "Physical" << "||Directly connected disk" << Qt::endl;
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_FASTBLOC)
                proplist << "Media Flag||" << "Fastbloc Write Blocked" << "||Write blocked disk using Fastbloc" << Qt::endl;
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_TABLEAU)
                proplist << "Media Flag||" << "Tableau Write Blocked" << "||Write blocked disk using Tableau" << Qt::endl;
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_bytes_per_sector(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << "Bytes Per Sector||" << QString::number(value32bit) << "||Number of bytes in a sector" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_number_of_sectors(ewfinfo->handle, &value64bit, &ewferror) == 1)
            proplist << "Number of Sectors||" << QString::number(value64bit) << "||Number of total sectors in the original media" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_chunk_size(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << "Chunk Size||" << QString::number(value32bit) << "||The size of an image chunk" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_size(ewfinfo->handle, &size64bit, &ewferror) == 1)
            proplist << "Media Size||" << QString::number(size64bit) << "||The size of the media" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_md5(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "MD5||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||The MD5 hash algorithm of the uncompressed image stored as a 128-bit value" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_sha1(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "SHA1||" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "||The SHA1 hash algorithm of the uncompressed image stored as a 160-bit value" << Qt::endl;
        else
            libewf_error_fprint(ewferror, stdout);
        free(ewfvalue);
    }
    else if(TSK_IMG_TYPE_ISRAW(curimginfo->itype)) // is raw
    {
        // nothing i want to add for raw right now.
    }
    else // not supported...
    {
        // log error about unsupported image type.
    } 
    proplist.flush();
    epropfile.close();
}


