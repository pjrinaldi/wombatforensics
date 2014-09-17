#include "wombatproperties.h"

WombatProperties::WombatProperties(WombatVariable* wombatvarptr)
{
    wombatptr = wombatvarptr;
    affinfo = NULL;
    ewfinfo = NULL;
    ewfvalue = (uint8_t*)malloc(sizeof(uint8_t)*64);
    uvalue8bit = 0;
    value8bit = 0;
    value32bit = 0;
    value64bit = 0;
    size64bit = 0;
    ewferror = NULL;
    ffs = NULL;
    sb1 = NULL;
    sb2 = NULL;
    ext2fs = NULL;
    /*
    TSK_FS_FILE* tmpfile = NULL;
    ntfs_sb* ntfssb = NULL;
    FATXXFS_DENTRY* tmpfatdentry = NULL;
    FATXXFS_DENTRY* curentry = NULL;
    FATFS_INFO* fatfs = NULL;
    FATXXFS_SB* fatsb = NULL;
    const TSK_FS_ATTR*tmpattr;
    TSK_DADDR_T cursector = 0;
    TSK_DADDR_T endsector = 0;
    int8_t isallocsec = 0;
    TSK_INUM_T curinum = 0;
    FATFS_DENTRY* dentry = NULL;
    ISO_INFO* iso = NULL;
    iso9660_pvd_node* p = NULL;
    iso9660_svd_node* s = NULL;
    HFS_INFO* hfs = NULL;
    hfs_plus_vh* hsb = NULL;
    char fn[HFS_MAXNAMLEN + 1];
    HFS_ENTRY* hfsentry = NULL;
    hfs_btree_key_cat key;
    hfs_thread thread;
    hfs_file_folder record;
    TSK_OFF_T off;
    char* databuffer = NULL;
    ssize_t cnt;
    ssize_t bytesread = 0;
    int a;
    uint len = 0;
    */

}

QString WombatProperties::GetFileSystemLabel(TSK_FS_INFO* curinfo)
{
    //wombatptr->bindvalues.append(QString::fromStdString(string(((EXT2FS_INFO*)tmpfsinfo)->fs->s_volume_name)));
    if(curinfo->ftype == TSK_FS_TYPE_EXT2 || curinfo->ftype == TSK_FS_TYPE_EXT3 || curinfo->ftype == TSK_FS_TYPE_EXT4 || curinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
    {
        return QString::fromStdString(string(((EXT2FS_INFO*)curinfo)->fs->s_volume_name));
    }
    else if(curinfo->ftype == TSK_FS_TYPE_FFS1 || curinfo->ftype == TSK_FS_TYPE_FFS1B)
    {
        return "ufs1";
    }
    else if(curinfo->ftype == TSK_FS_TYPE_FFS2 || curinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
    {
        qDebug() << "ffs2 volname: " << ((FFS_INFO*)curinfo)->fs.sb2->volname;
        return QString::fromUtf8(reinterpret_cast<char*>(((FFS_INFO*)curinfo)->fs.sb2->volname));
    }
    return "";
}

QStringList WombatProperties::PopulateEvidenceImageProperties()
{
    proplist.clear();
    proplist << QString("File Format") << QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)wombatptr->evidenceobject.imageinfo->itype)) << QString("File Format the evidence data is stored in. Usually it is either a raw image (.dd/.001) or an embedded image (.E01/.AFF). A raw image contains only the data from the evidence. The embedded image contains other descriptive information from the acquisition.");
    proplist << QString("Sector Size") << QString(QString::number(wombatptr->evidenceobject.imageinfo->sector_size) + " bytes") << QString("Sector size of the device. A Sector is a subdivision of a disk where data is stored. It is the smallest value used to divide the disk.");
    proplist << QString("Sector Count") << QString(QString::number((int)((float)wombatptr->evidenceobject.imageinfo->size/(float)wombatptr->evidenceobject.imageinfo->sector_size)) + " sectors") << QString("The number of sectors in the disk.");
    proplist << QString("Image Path") << QString::fromStdString(wombatptr->evidenceobject.fullpathvector[0]) << QString("Location where the evidence image is stored and read from.");
    if(TSK_IMG_TYPE_ISAFF(wombatptr->evidenceobject.imageinfo->itype)) // its AFF
    {
        affinfo = (IMG_AFF_INFO*)wombatptr->evidenceobject.imageinfo;
        proplist << "MD5" << QString::fromStdString(GetSegmentValue(affinfo, AF_MD5)) << "The MD5 hash algorithm of the uncompressed image file, stored as a 128-bit value";
        proplist << "Image GID" << QString::fromStdString(GetSegmentValue(affinfo, AF_IMAGE_GID)) << "A unique global identifier for the image";
        proplist << "Device Model" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_MODEL)) << "Acquired Drive Model number";
        proplist << "Creator" << QString::fromStdString(GetSegmentValue(affinfo, AF_CREATOR)) << "Examiner who initiated the image acquisition";
        proplist << "Case Number" << QString::fromStdString(GetSegmentValue(affinfo, AF_CASE_NUM)) << "The case number that the image is associated with";
        proplist << "SHA1" << QString::fromStdString(GetSegmentValue(affinfo, AF_SHA1)) << "The SHA1 hash algorithm of the uncompressed image file, stored as a 160-bit value";
        proplist << "Acquisition Date" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_DATE)) << "The date the acquisition was made";
        proplist << "Acquisition Notes" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_NOTES)) << "Notes regading the acquisition";
        proplist << "Acquisition Device" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_DEVICE)) << "The device used to acquire the information";
        proplist << "AFFLib Version" << QString::fromStdString(GetSegmentValue(affinfo, AF_AFFLIB_VERSION)) << "Verion of the AFFLib Library used";
        proplist << "Device Manufacturer" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_MANUFACTURER)) << "Maker of the drive";
        proplist << "Device Serial Number" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_SN)) << "Serial number of the drive";
    }
    else if(TSK_IMG_TYPE_ISEWF(wombatptr->evidenceobject.imageinfo->itype)) // its EWF
    {
        ewfinfo = (IMG_EWF_INFO*)wombatptr->evidenceobject.imageinfo;
        if(libewf_handle_get_utf8_header_value_case_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Case Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The case number the image is associated";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_description(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Description" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Description of the acquisition and or evidence item";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_examiner_name(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Examiner Name" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Name of the examiner who acquired the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_evidence_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Evidence Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Unique number identifying the evidence item";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_notes(ewfinfo->handle, ewfvalue, 255, &ewferror) == 1)
            proplist << "Notes" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Any notes related to the acquisition";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Acquisition Date" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Date the acquisition was made";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_system_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "System Date" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Date for the system acquiring the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_operating_system(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Aquisition OS" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Operating System acquiring the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_software_version(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Software Version Used" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Version of the software used to acquire the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_password(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Password" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Password to protect the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_model(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Model" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Model of the drive acquired";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_serial_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "Serial Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Serial number of the drive acquired";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_sectors_per_chunk(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << QString("Sectors Per Chunk") << QString::number(value32bit) << "Number of sectors in a image evidence chunk";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_format(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            proplist << QString("File Format");
            switch(uvalue8bit)
            {
                case LIBEWF_FORMAT_EWF:
                    proplist << QString("Original EWF") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_SMART:
                    proplist << QString("SMART") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_FTK:
                    proplist << QString("FTK Imager") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE1:
                    proplist << QString("EnCase 1") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE2:
                    proplist << QString("EnCase 2") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE3:
                    proplist << QString("EnCase 3") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE4:
                    proplist << QString("EnCase 4") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE5:
                    proplist << QString("EnCase 5") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE6:
                    proplist << QString("EnCase 6") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_LINEN5:
                    proplist << QString("Linen 5") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_LINEN6:
                    proplist << QString("Linen 6") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_EWFX:
                    proplist << QString("EWFX (extended ewf)") << QString("Extended EWF Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE5:
                    proplist << QString("LEF EnCase 5") << QString("Logical Evidence File EnCase 5 Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE6:
                    proplist << QString("LEF EnCase 6") << QString("Logical Evidence File EnCase 6 Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_UNKNOWN:
                    proplist << QString("Unknown Format") << "Format used to store the evidence image";
                    break;
            }
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_error_granularity(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << QString("Error Granularity") << QString::number(value32bit) << "Error block size";
        else
            libewf_error_fprint(ewferror, stdout);
        proplist << "Compression Method" << "Deflate" << "Method used to compress the image";
        if(libewf_handle_get_compression_values(ewfinfo->handle, &value8bit, &uvalue8bit, &ewferror) == 1)
        {
            proplist << "Compression Level";
            if(value8bit == LIBEWF_COMPRESSION_NONE)
                proplist << "No Compression";
            else if(value8bit == LIBEWF_COMPRESSION_FAST)
                proplist << "Good (Fast) Compression";
            else if(value8bit == LIBEWF_COMPRESSION_BEST)
                proplist << "Best Compression";
            else
                proplist << "Unknown Compression";
            proplist << "The more compression, the slower the acquisition but the smaller the file size";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_type(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            proplist << "Media Type";
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
            proplist << "Media type of the original evidence";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_flags(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_PHYSICAL)
                proplist << "Media Flag" << "Physical" << "Directly connected disk";
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_FASTBLOC)
                proplist << "Media Flag" << "Fastbloc Write Blocked" << "Write blocked disk using Fastbloc";
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_TABLEAU)
                proplist << "Media Flag" << "Tableau Write Blocked" << "Write blocked disk using Tableau";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_bytes_per_sector(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << "Bytes Per Sector" << QString::number(value32bit) << "Number of bytes in a sector";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_number_of_sectors(ewfinfo->handle, &value64bit, &ewferror) == 1)
            proplist << "Number of Sectors" << QString::number(value64bit) << "Number of total sectors in the original media";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_chunk_size(ewfinfo->handle, &value32bit, &ewferror) == 1)
            proplist << "Chunk Size" << QString::number(value32bit) << "The size of an image chunk";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_size(ewfinfo->handle, &size64bit, &ewferror) == 1)
            proplist << "Media Size" << QString::number(size64bit) << "The size of the media";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_md5(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "MD5" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The MD5 hash algorithm of the uncompressed image stored as a 128-bit value";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_sha1(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            proplist << "SHA1" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The SHA1 hash algorithm of the uncompressed image stored as a 160-bit value";
        else
            libewf_error_fprint(ewferror, stdout);
        free(ewfvalue);
    }
    else if(TSK_IMG_TYPE_ISRAW(wombatptr->evidenceobject.imageinfo->itype)) // is raw
    {
        // nothing i want to add for raw right now.
    }
    else // not supported...
    {
        // log error about unsupported image type.
    }
    return proplist;
}

QStringList WombatProperties::PopulatePartitionProperties()
{
    return QStringList("");
}

QStringList WombatProperties::PopulateFileSystemProperties(TSK_FS_INFO* curfsinfo)
{
    if(curfsinfo->ftype == TSK_FS_TYPE_EXT2 || curfsinfo->ftype == TSK_FS_TYPE_EXT3 || curfsinfo->ftype == TSK_FS_TYPE_EXT4 || curfsinfo->ftype == TSK_FS_TYPE_EXT_DETECT)
    {
        ext2fs = (EXT2FS_INFO*)curfsinfo;
        proplist << "File System Type";
        if(curfsinfo->ftype == TSK_FS_TYPE_EXT2)
            proplist << "ext2";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT3)
            proplist << "ext3";
        else if(curfsinfo->ftype == TSK_FS_TYPE_EXT4)
            proplist << "ext4";
        else
            proplist << "ext2";
        proplist << "";
        proplist << "Inode Count" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "Number of Inodes in the file system (0x00-0x03)";
        proplist << "Block Count" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_count)) << "Number of Blocks in the file system (0x04-0x07)";
        proplist << "Reserved Blocks" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_r_blocks_count)) << "Number of blocks reserved to prevent the file system from filling up (0x08-0x0B)";
        proplist << "Unallocated Blocks" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_blocks_count)) << "Number of unallocated blocks (0x0C-0x0F)";
        proplist << "Unallocated Inodes" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_free_inode_count)) << "Number of unnalocated inodes (0x10-0x13)";
        proplist << "First Data Block" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_first_data_block)) << "Block where block group 0 starts (0x14-0x17)";
        proplist << "Log Block Size" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_log_block_size)) << "Block size saved as the number of places to shift 1,024 to the left (0x18-0x1B)";
        proplist << "Log Fragment Size" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_log_frag_size)) << "Fragment size saved as the number of bits to shift 1,024 to the left (0x1C-0x1F)";
        proplist << "Blocks per Group" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_blocks_per_group)) << "Number of blocks in each block group (0x20-0x23)";
        proplist << "Fragments per Group" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_frags_per_group)) << "Number of fragments in each block group (0x24-0x27)";
        proplist << "Inodes per Group" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_inodes_per_group)) << "Number of inodes in each block group (0x28-0x2B)";
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_mtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_mtime), timebuf) : "empty");
        proplist << "Last Mount Time" << QString::fromStdString(string(asc)) << "Last time the file system was mounted (0x2C-0x2F)";
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_wtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_wtime), timebuf) : "empty");
        proplist << "Last Written Time" << QString::fromStdString(string(asc)) << "Last time data was written to the file system (0x30-0x33)";
        proplist << "Current Mount Count" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_mnt_count)) << "Number of times the file system has been mounted (0x34-0x35)";
        proplist << "Maximum Mount Count" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_max_mnt_count)) << "Maximum number of times the file system can be mounted (0x36-0x37)";
        proplist << "Signature" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_magic)) << "File system signature value should be 0xef53 (0x38-0x39)";
        proplist << "File System State";
        if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_state) & EXT2FS_STATE_VALID)
            proplist << "Unmounted properly";
        else if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_state) & EXT2FS_STATE_ERROR)
            proplist << "Errors Detected";
        else
            proplist << "Orphan inodes were being recovered";
        proplist << "State of the file system when it was last shut down (0x3A-0x3B)";
        proplist << "Error Handling Method";
        if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_errors) == 1)
            proplist << "Continue";
        else if(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_errors) == 2)
            proplist << "Remount as Read-Only";
        else
            proplist << "Panic";
        proplist << "Identifies what the OS should do when it encounters a file system error (0x3C-0x3D)";
        proplist << "Minor Version" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_minor_rev_level)) << "Minor Revision Level (0x3E-0x3F)";
        sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, ext2fs->fs->s_lastcheck) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_lastcheck), timebuf) : "empty");
        proplist << "Last Checked" << QString::fromStdString(string(asc)) << "Last time the consistency of the file system was checked (0x40-0x43)";
        proplist << "Interval b/w Checks" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_checkinterval)) << "Interval between forced consistency checks (0x44-0x47)";
        proplist << "Creator OS";
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
        proplist << "OS that might have created the file system (0x48-0x4B)";
        proplist << "Major Version";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_rev_level) == EXT2FS_REV_ORIG)
            proplist << "Static Structure";
        else
            proplist << "Dynamic Structure";
        proplist << "If the version is not set to dynamic, the values from bytes 84 and up might not be accurate (0x4C-0x4F)";
        proplist << "UID to Use Reserved Blocks" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_def_resuid)) << "User ID that can use reserved blocks (0x50-0x51)";
        proplist << "GID to Use Reserved Blocks" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_def_resgid)) << "Group ID that can use reserved blocks (0x52-0x53)";
        proplist << "First Non-Reserved Inode" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_first_ino)) << "First non-reserved inode in the file system (0x54-0x57)";
        proplist << "Inode Structure Size" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_inode_size)) << "Size of each inode structure (0x58-0x59)";
        proplist << "Block Group for SuperBlock" << QString::number(tsk_getu16(curfsinfo->endian, ext2fs->fs->s_block_group_nr)) << "Superblock is part of the block group (if backup copy) (0x5A-0x5B)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_DIR_PREALLOC)
            proplist << "Compatible Feature" << "Directory Pre-allocation" << "Pre-allocate directory blocks to reduce fragmentation. The OS can mount the file system as normal if it does not support a compatible feature (0x5C-0x5F)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_IMAGIC_INODES)
            proplist << "Compatible Feature" << "IMagic Inodes" << "AFS server inodes exist (0x5C-0x5F)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_HAS_JOURNAL)
            proplist << "Compatible Feature" << "Journal" << "File System has a journal (0x5C-0x5F)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_EXT_ATTR)
            proplist << "Compatible Feature" << "Extended Attributes" << "Inodes have extended attributes (0x5C-0x5F)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_RESIZE_INO)
            proplist << "Compatible Feature" << "Resizable File System" << "File system can resize itself for larger aptitions (0x5C-0x5F)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_compat) & EXT2FS_FEATURE_COMPAT_DIR_INDEX)
            proplist << "Compatible Feature" << "Directory Index" << "Directories use hash index (0x5C-0x5F)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_COMPRESSION)
            proplist << "Incompatible Feature" << "Compression" << " The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_FILETYPE)
            proplist << "Incompatible Feature" << "Filetype" << "Directory entries contain a file type field (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_RECOVER)
            proplist << "Incompatible Feature" << "Needs Recovery" << "The file systems needs to be recovered (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_JOURNAL_DEV)
            proplist << "Incompatible Feature" << "Journal Device" << "The file system uses a journal device (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_META_BG)
            proplist << "Incompatible Feature" << "Meta Block Groups" << "The file system has meta block groups (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_EXTENTS)
            proplist << "Incompatible Feature" << "Extents" << "The file system uses extents (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_64BIT)
            proplist << "Incompatible Feature" << "64-bit" << "The file system is 64-bit capable (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_MMP)
            proplist << "Incompatible Feature" << "Multiple Mount Protection" << "The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_FLEX_BG)
            proplist << "Incompatible Feature" << "Flexible Block Groups" << "The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_EA_INODE)
            proplist << "Incompatible Feature" << "Extended Attributes" << "The file system supports extended attributes (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_incompat) & EXT2FS_FEATURE_INCOMPAT_DIRDATA)
            proplist << "Incompatible Feature" << "Directory Entry Data" << "The OS should not mount the file system if it does not support this incompatible feature (0x60-0x63)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_SPARSE_SUPER)
            proplist << "Read only Feature" << "Sparse Super" << "Sparse superblocks and goup descriptor tables. The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_LARGE_FILE)
            proplist << "Read only Feature" << "Large File" << "The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_HUGE_FILE)
            proplist << "Read only Feature" << "Huge File" << "The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_BTREE_DIR)
            proplist << "Read only Feature" << "BTree Directory" << "The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)";
        if(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_feature_ro_compat) & EXT2FS_FEATURE_RO_COMPAT_EXTRA_ISIZE)
            proplist << "Read only Feature" << "Extra Inode Size" << "The OS should mount the file system as read only if it does not support this read only feature (0x64-0x67)";
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_uuid[8]), tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_uuid[0]));
        proplist << "File System ID" << QString::fromStdString(string(asc)) << "File system ID. Found in the superblock at bytes (0x68-0x77)"; 
        proplist << "File System Label" << QString::fromStdString(string(ext2fs->fs->s_volume_name)) << "File System Label. (0x78-0x87)"; 
        proplist << "Last Mounted Path" << QString::fromStdString(string(ext2fs->fs->s_last_mounted)) << "Path where the file system was last mounted (0x88-0xC7)";
        proplist << "Algorithm Usage Bitmap" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_algorithm_usage_bitmap)) << "(0xC8-0xCB)";
        proplist << "Blocks Preallocated for Files" << QString::number(ext2fs->fs->s_prealloc_blocks) << "Number of blocks to preallocate for files (0xCC-0xCC)";
        proplist << "Blocks Preallocated for Directories" << QString::number(ext2fs->fs->s_prealloc_dir_blocks) << "Number of blocks to preallocate for directories (0xCD-0xCD)";
        proplist << "Unused" << "Unused" << "Unused bytes (0xCE-0xCF)";
        sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_journal_uuid[8]), tsk_getu64(curfsinfo->endian, &(ext2fs->fs)->s_journal_uuid[0]));
        proplist << "Journal ID" << QString::fromStdString(string(asc)) << "Journal ID (0xD0-0xDF)";
        proplist << "Journal Inode" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_journal_inum)) << "Journal Inode (0xE0-0xE3)";
        proplist << "Journal Device" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_journal_dev)) << "Journal device (0xE4-0xE7)";
        proplist << "Head of Oprhan Inode List" << QString::number(tsk_getu32(curfsinfo->endian, ext2fs->fs->s_last_orphan)) << "Head of orphan inode list. (0xE8-0xEB)";
        proplist << "Unused" << "Unused" << "Unused (0xEC-0x03FF)";
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B || curfsinfo->ftype == TSK_FS_TYPE_FFS2 || curfsinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
    {
        ffs = (FFS_INFO*)curfsinfo;
        sb1 = ffs->fs.sb1;
        sb2 = ffs->fs.sb2;
        proplist << "File System Type";
        if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B)
            proplist << "UFS 1";
        else
            proplist << "UFS 2";
        if(curfsinfo->ftype == TSK_FS_TYPE_FFS1 || curfsinfo->ftype == TSK_FS_TYPE_FFS1B)
        {
            proplist << "";
            proplist << "Unused" << "Unused" << "Unused (0x00-0x07)";
            proplist << "Backup Superblock Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb1->sb_off)) << "Offset to backup superblock in cylinder group relative to a \"base\" (0x08-0x0B)";
            proplist << "Group Descriptor Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb1->gd_off)) << "Offset to group descriptor in cylinder group relative to a \"base\" (0x0C-0x0F)";
            proplist << "Inode Table Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb1->ino_off)) << "Offset to inode table in cylinder group relative to a \"base\" (0x10-0x13)";
            proplist << "Data Block Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb1->dat_off)) << "Offset to data blocks in cylinder group relative to a \"base\" (0x14-0x17)";
            proplist << "Delta Value for Staggering Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_delta)) << "Delta value for caluclating the staggering offset in cylinder group (0x18-0x1B)";
            proplist << "Mask for Staggering Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_cyc_mask)) << "Mask for calculating the staggering offset (cycle value) in cylinder group (0x1C-0x1F)";
            sprintf(asc, "%s", (tsk_getu32(curfsinfo->endian, sb1->wtime) > 0) ? tsk_fs_time_to_str(tsk_getu32(curfsinfo->endian, sb1->wtime), timebuf) : "empty");
            proplist << "Last Written Time" << QString::fromStdString(string(asc)) << "Last time data was written to the file system (0x20-0x23)";
            proplist << "Number of Fragments" << QString::number(tsk_gets32(curfsinfo->endian, sb1->frag_num)) << "Number of fragments in the file system (0x24-0x27)";
            proplist << "Number of Data Fragments" << QString::number(tsk_gets32(curfsinfo->endian, sb1->data_frag_num)) << "Number of fragments in the file system that can store file data (0x28-0x2B)";
            proplist << "Number of Cylinder Groups" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_num)) << "Number of cylinder groups in the file system (0x2C-0x2F)";
            proplist << "Block Byte Size" << QString::number(tsk_gets32(curfsinfo->endian, sb1->bsize_b)) << "Size of a block in bytes (0x30-0x33)";
            proplist << "Fragment Size" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fsize_b)) << "Size of a fragment in bytes (0x34-0x37)";
            proplist << "Block Framgent Size" << QString::number(tsk_gets32(curfsinfo->endian, sb1->bsize_frag)) << "Size of a block in fragments (0x38-0x3B)";
            proplist << "Non-Essential Values" << "Non-Essential Values" << "Non-Essential Values (0x3C-0x5F)";
            proplist << "Number of Bits Convert Block to Fragment" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_fragshift)) << "Number of bits to convert between a block address and a fragment address (0x60-0x63)";
            proplist << "Non-Essential Values" << "Non-Essential Values" << "Non-Essential Values (0x64-0x77)";
            proplist << "Inodes Per Block" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_inopb)) << "Number of inodes per block in the inode table (0x78-0x7B)";
            proplist << "Non-Essential Values" << "Non-Essential Values" << "Non-Essential Values (0x7C-0x8F)";
            sprintf(asc, "%" PRIx32 "%" PRIx32 "", tsk_getu32(curfsinfo->endian, &sb1->fs_id[4]), tsk_getu32(curfsinfo->endian, &sb1->fs_id[0]));
            proplist << "File System ID" << QString::fromStdString(string(asc)) << "File system ID (0x90-0x97)";
            proplist << "Cylinder Group Fragment Address" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_saddr)) << "Fragment address of the cylinder group summary area (0x98-0x9B)";
            proplist << "Cylinder Group Summary Area Byte Size" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_ssize_b)) << "Size of the cylinder group summary area in bytes (0x9C-0x9F)";
            proplist << "Cylinder Group Descriptor Byte Size" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_cgsize)) << "Size of the cylinder group descriptor in bytes (0xA0-0xA3)";
            proplist << "Non-Essential Values" << "Non-Essential Values" << "Non-Essential Values (0xA4-0xA5)";
            proplist << "Cylinders in File System" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_ncyl)) << "Number of cylinders in the file system (0xA6-0xB3)";
            proplist << "Cylinders per Cylinder Group" << QString::number(tsk_gets32(curfsinfo->endian, sb1->fs_cpg)) << "Number of cylinders in a cylinder group (0xB4-0xB7)";
            proplist << "Inodes per Cylinder Group" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_inode_num)) << "Number of inodes in a cylinder group (0xB8-0xBB)";
            proplist << "Fragments per Cylinder Group" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cg_frag_num)) << "Number of fragments in a cylinder group (0xBC-0xBF)";
            proplist << "Number of Directories" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.dir_num)) << "Number of directories (0xC0-0xC3)";
            proplist << "Number of Free Blocks" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.blk_free)) << "Number of free blocks (0xC4-0xC7)";
            proplist << "Number of Free Inodes" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.ino_free)) << "Number of free inodes (0xC8-0xCB)";
            proplist << "Number of Free Fragments" << QString::number(tsk_gets32(curfsinfo->endian, sb1->cstotal.frag_free)) << "Number of free fragments (0xCC-0xCF)";
            proplist << "Super Block Modified Flag" << QString::number(sb1->fs_fmod) << "Super Block Modified Flag (0xD0-0xD0)";
            proplist << "Clean File System Flag" << QString::number(sb1->fs_clean) << "File system was clean when it was mounted (0xD1-0xD1)";
            proplist << "Read Only File System Flag" << QString::number(sb1->fs_ronly) << "File system was mounted read only (0xD2-0xD2)";
            if(sb1->fs_flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags" << "Unclean" << "Set when the file system is mounted (0xD3-0xD3)";
            if(sb1->fs_flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags" << "Soft Dependencies" << "Soft dependencies are being used (0xD3-0xD3)";
            if(sb1->fs_flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags" << "Needs Check" << "Needs consistency check next time the file system is mounted (0xD3-0xD3)";
            if(sb1->fs_flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags" << "Index Directories" << "Directories are indexed using a hashtree or B-Tree (0xD3-0xD3)";
            if(sb1->fs_flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags" << "Access Control Lists" << "Access control lists are being used (0xD3-0xD3)";
            if(sb1->fs_flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags" << "TrustedBSD MAC Multi-Label" << "TrustedBSD Mandatory Access Control multi-labels are being used (0xD3-0xD3)";
            if(sb1->fs_flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags" << "Updated Flag Location" << "Flags have been moved (0xD3-0xD3)";
            proplist << "Last Mount Point" << QString::fromStdString(string(reinterpret_cast<char*>(sb1->last_mnt))) << "Last mount point (0xD4-0x02D3)";
            proplist << "Non-Essential Values" << "Non-Essential Values" << "Non-Essential Values (0x02D4-0x055B)";
            proplist << "Signature" << QString::number(tsk_gets32(curfsinfo->endian, sb1->magic)) << "File System signature value should be 0x011954 (0x055C-0x055F)";
        }
        else // FFS2
        {
            proplist << "Unused" << "Unused" << "Unused (0x00-0x07)";
            proplist << "Backup Superblock Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb2->sb_off)) << "Offset to backup superblock in cylinder group relative to a \"base\" (0x08-0x0B)";
            proplist << "Group Descriptor Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb2->gd_off)) << "Offset to group descriptor in cylinder group relative to a \"base\" (0x0C-0x0F)";
            proplist << "Inode Table Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb2->ino_off)) << "Offset to inode table in cylinder group relative to a \"base\" (0x10-0x13)";
            proplist << "First Data Block Offset" << QString::number(tsk_gets32(curfsinfo->endian, sb2->dat_off)) << "Offset to the first data block in the cylinder group relative to a \"base\" (0x14-0x17)";
            proplist << "Unused" << "Unused" << "Unused (0x18-0x2B)";
            proplist << "Number of Cylinder Groups" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_num)) << "The Number of cylinder groups in the file system (0x2C-0x2F)";
            proplist << "Block Byte Size" << QString::number(tsk_gets32(curfsinfo->endian, sb2->bsize_b)) << "Size of a block in bytes (0x30-0x33)";
            proplist << "Fragment Size" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fsize_b)) << "Size of a fragment in bytes (0x34-0x37)";
            proplist << "Block Fragment Size" << QString::number(tsk_gets32(curfsinfo->endian, sb2->bsize_frag)) << "Size of a block in fragments (0x38-0x3B)";
            proplist << "Unused" << "Unused" << "Unused (0x3C-0x5F)";
            proplist << "File System Fragment Shift" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_fragshift)) << "Number of bits to convert between a block address and a fragment address (0x60-0x63)";
            proplist << "Unused" << "Unused" << "Unused (0x64-0x77)";
            proplist << "Inodes Per Block" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_inopb)) << "Number of inodes per block in inode table (0x78-0x7B)";
            proplist << "Unused" << "Unused" << "Unused (0x7C-0x9B)";
            proplist << "Cylinder Group Summary Size" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_ssize_b)) << "Size of cylinder group summary area in bytes (0x9C-0x9F)";
            proplist << "Cylinder Group Descriptor Size" << QString::number(tsk_gets32(curfsinfo->endian, sb2->fs_cgsize)) << "Size of cylinder group descriptor in bytes (0xA0-0xA3)";
            proplist << "Unused" << "Unused" << "Unused (0xA4-0xB7)";
            proplist << "Cylinder Group Inodes" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_inode_num)) << "Inodes per cylinder group (0xB8-0xBB)";
            proplist << "Cylinder Group Fragments" << QString::number(tsk_gets32(curfsinfo->endian, sb2->cg_frag_num)) << "Fragments per cylinder group (0xBC-0xBF)";
            proplist << "Unused" << "Unused" << "Unused (0xC0-0xCF)";
            proplist << "Super Block Modified Flag" << QString::number(sb2->fs_fmod) << "Super block modified flag (0xD0-0xD0)";
            proplist << "Clean File System" << QString::number(sb2->fs_clean) << "File system was clean when it was mounted (0xD1-0xD1)";
            proplist << "Read Only Flag" << QString::number(sb2->fs_ronly) << "File system was mounted read only (0xD2-0xD2)";
            proplist << "Unused" << "Unused" << "Unused (0xD3-0xD3)";
            proplist << "Last Mount Point" << QString::fromUtf8(reinterpret_cast<char*>(sb2->last_mnt)) << "Last mount point (0xD4-0x02A7)";
            proplist << "Volume Name" << QString::fromUtf8(reinterpret_cast<char*>(sb2->volname)) << "Volume name (0x02A8-0x02C7)";
            proplist << "System UID" << QString::fromUtf8(reinterpret_cast<char*>(sb2->swuid)) << "System UID (0x02C8-0x02CF)";
            proplist << "Unused" << "Unused" << "Unused (0x02D0-0x03EF)";
            proplist << "Number of Directories" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.dir_num)) << "Number of directories (0x03F0-0x03F7)";
            proplist << "Number of Free Blocks" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.blk_free)) << "Number of free blocks (0x03F8-0x03FF)";
            proplist << "Number of Free Inodes" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.ino_free)) << "Number of free inodes (0x0400-0x0407)";
            proplist << "Number of Free Fragments" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.frag_free)) << "Number of free fragments (0x0408-0x040F)";
            proplist << "Number of Free Clusters" << QString::number(tsk_getu64(curfsinfo->endian, sb2->cstotal.clust_free)) << "Number of free clusters (0x0410-0x0417)";
            proplist << "Unused" << "Unused" << "Unused (0x0418-0x042F)";
            sprintf(asc, "%s", (tsk_getu64(curfsinfo->endian, sb1->wtime) > 0) ? tsk_fs_time_to_str(tsk_getu64(curfsinfo->endian, sb1->wtime), timebuf) : "empty");
            proplist << "Last Written Time" << QString::fromStdString(string(asc)) << "Last time data was written to the file system (0x0430-0x0437)";
            proplist << "Fragment Numbers" << QString::number(tsk_gets64(curfsinfo->endian, sb2->frag_num)) << "Number of fragments in the file system (0x0438-0x043F)";
            proplist << "Usable Fragment Numbers" << QString::number(tsk_gets64(curfsinfo->endian, sb2->blk_num)) << "Number of fragments that can store file data (0x0440-0x0447)";
            proplist << "Cylinder Group Fragment Address" << QString::number(tsk_get264(curfsinfo->endian, sb2->cg_saddr)) << "Fragment address of cylinder group summary area (0x0448-0x044F)";
            proplist << "Unused" << "Unused" << "Unused (0x0450-0x051F)";
            if(sb2->fs_flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags" << "Unclean" << "Set when the file system is mounted (0x0520-0x0523)";
            if(sb2->fs_flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags" << "Soft Dependencies" << "Soft dependencies are being used (0x0520-0x0523)";
            if(sb2->fs_flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags" << "Needs Check" << "Needs consistency check next time the file system is mounted (0x0520-0x0523)";
            if(sb2->fs_flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags" << "Index Directories" << "Directories are indexed using a hashtree or B-Tree (0x0520-0x0523)";
            if(sb2->fs_flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags" << "Access Control Lists" << "Access control lists are being used (0x0520-0x0523)";
            if(sb2->fs_flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags" << "TrustedBSD MAC Multi-Label" << "TrustedBSD Mandatory Access Control multi-labels are being used (0x0520-0x0523)";
            if(sb2->fs_flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags" << "Updated Flag Location" << "Flags have been moved (0x0520-0x0523)";
            proplist << "Unused" << "Unused" << "Unused (0x0524-0x055B)";
            proplist << "Signature" << QString::number(tsk_gets32(curfsinfo->endian, sb2->magic)) << "File system signature value should be 0x19540119 (0x055C-0x055F)";
        }
    }
    return proplist;
}

QStringList WombatProperties::PopulateFileProperties()
{
    return QStringList("");
}
// REFERENCE INFORMATION
    /*
    switch(curfsinfo->ftype)
    {
        case TSK_FS_TYPE_NTFS:
            len = roundup(sizeof(ntfs_sb), curfsinfo->img_info->sector_size);
            ntfssb = tsk_malloc(len);
            cnt = tsk_fs_read(curfsinfo, (TSK_OFF_T) 0, (char*) ntfssb, len);
            // will come back to NTFS.
            break;
        case TSK_FS_TYPE_EXFAT:


    }
        case TSK_FS_TYPE_NTFS:
                //NTFS_INFO* tmpinfo = (NTFS_INFO*)tmpfsinfo;
                if((tmpfile = tsk_fs_file_open_meta(tmpfsinfo, NULL, NTFS_MFT_VOL)) == NULL)
                {
                    // log error here...
                }
                tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, TSK_FS_ATTR_TYPE_NTFS_VNAME);
                //tmpattr = tsk_fs_attrlist_get(tmpfile->meta->attr, NTFS_ATYPE_VNAME);
                if(!tmpattr)
                {
                    // log error here...
                }
                if((tmpattr->flags & TSK_FS_ATTR_RES) && (tmpattr->size))
                {
                    UTF16* name16 = (UTF16*) tmpattr->rd.buf;
                    UTF8* name8 = (UTF8*) asc;
                    int retval;
                    retval = tsk_UTF16toUTF8(tmpfsinfo->endian, (const UTF16**)&name16, (UTF16*) ((uintptr_t) name16 + (int) tmpattr->size), &name8, (UTF8*) ((uintptr_t)name8 + sizeof(asc)), TSKlenientConversion);
                    if(retval != TSKconversionOK)
                    {
                        // log error here                                
                        *name8 = '\0';
                    }
                    else if((uintptr_t) name8 >= (uintptr_t) asc + sizeof(asc))
                        asc[sizeof(asc) - 1] = '\0';
                    else
                        *name8 = '\0';
                }
                qDebug() << "NTFS Volume Name:" << asc;
                tsk_fs_file_close(tmpfile);
                break;
            case TSK_FS_TYPE_EXFAT:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                if((tmpfile = tsk_fs_file_alloc(tmpfsinfo)) == NULL)
                {
                    // log error here
                }
                if((tmpfile->meta = tsk_fs_meta_alloc(FATFS_FILE_CONTENT_LEN)) == NULL)
                {
                    // log error here
                }
                if((databuffer = (char*)tsk_malloc(fatfs->ssize)) == NULL)
                {
                    // log error here
                }
                cursector = fatfs->rootsect;
                endsector = fatfs->firstdatasect + (fatfs->clustcnt * fatfs->csize) - 1;
                while(cursector < endsector)
                {

                }
                bytesread = tsk_fs_read_block(tmpfsinfo, cursector, databuffer, fatfs->ssize);
                if(bytesread != fatfs->ssize)
                {
                    // log error here
                }
                isallocsec = fatfs_is_sectalloc(fatfs, cursector);
                if(isallocsec == -1)
                {
                    // log error here
                }
                curinum = FATFS_SECT_2_INODE(fatfs, cursector);
                for(i = 0; i < fatfs->ssize; i+= sizeof(FATFS_DENTRY))
                {
                    dentry = (FATFS_DENTRY*)&(databuffer[i]);
                    if(exfatfs_get_enum_from_type(dentry->data[0]) == EXFATFS_DIR_ENTRY_TYPE_VOLUME_LABEL)
                    {
                        if(exfatfs_dinode_copy(fatfs, curinum, dentry, isallocsec, tmpfile) == TSK_OK)
                        {
                            qDebug() << "EXFAT Volume Name: " << tmpfile->meta->name2->name;
                            break;
                        }
                        else
                        {
                            // log error here
                        }
                    }
                }
                tsk_fs_file_close(tmpfile);
                free(databuffer);
                break;
            case TSK_FS_TYPE_FAT12:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
                qDebug() << fatsb->a.f16.vol_lab;
                printf("Volume Label (Boot Sector): %c%c%c%c%c%c%c%c%c%c%c\n", fatsb->a.f16.vol_lab[0], fatsb->a.f16.vol_lab[1], fatsb->a.f16.vol_lab[2], fatsb->a.f16.vol_lab[3], fatsb->a.f16.vol_lab[4], fatsb->a.f16.vol_lab[5], fatsb->a.f16.vol_lab[6], fatsb->a.f16.vol_lab[7], fatsb->a.f16.vol_lab[8], fatsb->a.f16.vol_lab[9], fatsb->a.f16.vol_lab[10]);
                if((databuffer = (char*) tsk_malloc(tmpfsinfo->block_size)) == NULL)
                {
                    // log error here
                }
                cnt = tsk_fs_read_block(tmpfsinfo, fatfs->rootsect, databuffer, tmpfsinfo->block_size);
                if(cnt != tmpfsinfo->block_size)
                {
                    // log error here
                }
                tmpfatdentry = NULL;
                if(fatfs->ssize <= tmpfsinfo->block_size)
                {
                    curentry = (FATXXFS_DENTRY*)databuffer;
                    for(int i=0; i < fatfs->ssize; i += sizeof(*curentry))
                    {
                        if(curentry->attrib == FATFS_ATTR_VOLUME)
                        {
                            tmpfatdentry = curentry;
                            break;
                        }
                        curentry++;
                    }
                }
                qDebug() << "FAT12 Volume Label: " << tmpfatdentry->name;
                free(databuffer);
                break;
            case TSK_FS_TYPE_FAT16:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
                qDebug() << fatsb->a.f16.vol_lab;
                printf("Volume Label (Boot Sector): %c%c%c%c%c%c%c%c%c%c%c\n", fatsb->a.f16.vol_lab[0], fatsb->a.f16.vol_lab[1], fatsb->a.f16.vol_lab[2], fatsb->a.f16.vol_lab[3], fatsb->a.f16.vol_lab[4], fatsb->a.f16.vol_lab[5], fatsb->a.f16.vol_lab[6], fatsb->a.f16.vol_lab[7], fatsb->a.f16.vol_lab[8], fatsb->a.f16.vol_lab[9], fatsb->a.f16.vol_lab[10]);
                if((databuffer = (char*) tsk_malloc(tmpfsinfo->block_size)) == NULL)
                {
                    // log error here
                }
                cnt = tsk_fs_read_block(tmpfsinfo, fatfs->rootsect, databuffer, tmpfsinfo->block_size);
                if(cnt != tmpfsinfo->block_size)
                {
                    // log error here
                }
                tmpfatdentry = NULL;
                if(fatfs->ssize <= tmpfsinfo->block_size)
                {
                    curentry = (FATXXFS_DENTRY*)databuffer;
                    for(int i=0; i < fatfs->ssize; i += sizeof(*curentry))
                    {
                        if(curentry->attrib == FATFS_ATTR_VOLUME)
                        {
                            tmpfatdentry = curentry;
                            break;
                        }
                        curentry++;
                    }
                }
                qDebug() << "FAT16 Volume Label: " << tmpfatdentry->name;
                free(databuffer);
                break;
            case TSK_FS_TYPE_FAT32:
                fatfs = (FATFS_INFO*)tmpfsinfo;
                fatsb = (FATXXFS_SB*)fatfs->boot_sector_buffer;
                qDebug() << fatsb->a.f32.vol_lab;
                printf("Volume Label (Boot Sector): %c%c%c%c%c%c%c%c%c%c%c\n", fatsb->a.f32.vol_lab[0], fatsb->a.f32.vol_lab[1], fatsb->a.f32.vol_lab[2], fatsb->a.f32.vol_lab[3], fatsb->a.f32.vol_lab[4], fatsb->a.f32.vol_lab[5], fatsb->a.f32.vol_lab[6], fatsb->a.f32.vol_lab[7], fatsb->a.f32.vol_lab[8], fatsb->a.f32.vol_lab[9], fatsb->a.f32.vol_lab[10]);
                if((databuffer = (char*) tsk_malloc(tmpfsinfo->block_size)) == NULL)
                {
                    // log error here
                }
                cnt = tsk_fs_read_block(tmpfsinfo, fatfs->rootsect, databuffer, tmpfsinfo->block_size);
                if(cnt != tmpfsinfo->block_size)
                {
                    // log error here
                }
                tmpfatdentry = NULL;
                if(fatfs->ssize <= tmpfsinfo->block_size)
                {
                    curentry = (FATXXFS_DENTRY*)databuffer;
                    for(int i=0; i < fatfs->ssize; i += sizeof(*curentry))
                    {
                        if(curentry->attrib == FATFS_ATTR_VOLUME)
                        {
                            tmpfatdentry = curentry;
                            break;
                        }
                        curentry++;
                    }
                }
                qDebug() << "FAT32 Volume Label: " << tmpfatdentry->name;
                free(databuffer);
                break;
            case TSK_FS_TYPE_FFS1:
                qDebug() << "FFS1";
                break;
            case TSK_FS_TYPE_FFS1B:
                qDebug() << "FFS1B";
                break;
            case TSK_FS_TYPE_FFS2:
                ffs = (FFS_INFO*)tmpfsinfo;
                sb1 = ffs->fs.sb1;
                sb2 = ffs->fs.sb2;
                qDebug() << "FFS2 Volume label: " << sb2->volname;
                break;
            case TSK_FS_TYPE_EXT2:
                ext2fs = (EXT2FS_INFO*)tmpfsinfo;
                sb = ext2fs->fs;
                qDebug() << "EXT2 Volume name: " << sb->s_volume_name;
                break;
            case TSK_FS_TYPE_EXT3:
                ext2fs = (EXT2FS_INFO*)tmpfsinfo;
                sb = ext2fs->fs;
                qDebug() << "EXT3 Volume name: " << sb->s_volume_name;
                break;
            case TSK_FS_TYPE_EXT4:
                ext2fs = (EXT2FS_INFO*)tmpfsinfo;
                sb = ext2fs->fs;
                qDebug() << "EXT4 Volume name: " << sb->s_volume_name;
                break;
            case TSK_FS_TYPE_RAW:
                qDebug() << "no file system. store 0, \"\", or message for respective variables";
                break;
            case TSK_FS_TYPE_ISO9660:
                a = 0;
                iso = (ISO_INFO*)tmpfsinfo;
                p = iso->pvd;
                for(p = iso->pvd; p!= NULL; p = p->next)
                {
                    a++;
                    qDebug() << "ISO9660 vol name: " << p->pvd.vol_id;
                }
                a = 0;
                for(s = iso->svd; s!= NULL; s = s->next)
                {
                    a++;
                    qDebug() << "ISO9660 vol name: " << s->svd.vol_id;
                }
                qDebug() << "ISO9660";
                break;
            case TSK_FS_TYPE_HFS:
                hfs = (HFS_INFO*)tmpfsinfo;
                hsb = hfs->fs;
                memset((char*)&key, 0, sizeof(hfs_btree_key_cat));
                cnid_to_array((uint32_t)HFS_ROOT_INUM, key.parent_cnid);
                off = hfs_cat_get_record_offset(hfs, &key);
                if(off == 0)
                {
                    // log error here
                }
                if(hfs_cat_read_thread_record(hfs, off, &thread))
                {
                    // log error here
                }

                memset((char*)&key, 0, sizeof(hfs_btree_key_cat));
                memcpy((char*)key.parent_cnid, (char*)thread.parent_cnid, sizeof(key.parent_cnid));
                memcpy((char*)&key.name, (char*)&thread.name, sizeof(key.name));
                off = hfs_cat_get_record_offset(hfs, &key);
                if(off = 0)
                {
                    // log error here
                }
                if(hfs_cat_read_file_folder_record(hfs, off, &record))
                {
                    // log error here
                }
                //if(tsk_getU16(tmpfsinfo->endian, record.file.std.rec_type) == HFS_FOLDER_RECORD)
                memcpy((char*)&entry->thread, (char*)&thread, sizeof(hfs_thread));
                entry->flags = TSK_FS_META_FLAG_ALLOC | TSK_FS_META_FLAG_USED;
                entry->inum = HFS_ROOT_INUM;
                if(follow_hard_link)
                {
                    unsigned char is_err;
                    TSK_INUM_T target_cnid = hfs_follow_hard_link(hfs, &(entry->cat), &is_err);
                    if(is_err > 1)
                    {
                        // log error here
                    }
                    if(target_cnid != HFS_ROOT_INUM)
                    {
                        uint8_t res = hfs_cat_file_lookup(hfs, target_cnid, entry, FALSE);
                        if(res != 0)
                        {
                            // log error here
                        }
                    }
                //}
                // NEED TO EXPAND THE HFS_CAT_FILE_LOOKUP() FUNCTION AND THE 
                if(hfs_cat_file_lookup(hfs, HFS_ROOT_INUM, &hfsentry, FALSE))
                {
                    // log error here
                }
                if(hfs_UTF16toUTF8(tmpfsinfo, hfsentry.thread.name.unicode, tsk_getu16(tmpfsinfo->endian, hfsentry.thread.name.length), fn, HFS_MAXNAMLEN + 1, HFS_U16U8_FLAG_REPLACE_SLASH))
                {
                    // log error here.
                }
                 //continuance
                qDebug() << "HFS Volume Name: " << fn;
                free(fn);
                break;
            case TSK_FS_TYPE_YAFFS2:
                qDebug() << "YAFFS2 no volume name, might want other properties though";
                break;
            case TSK_FS_TYPE_SWAP:
                qDebug() << "no file system. store 0, \"\", or message for respective variables";
                break;
           default:
                qDebug() << "what to do for default???";
                break;
        }
    }

     *
     */
