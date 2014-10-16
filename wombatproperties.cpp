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
    macpart = NULL;
    bsdpart = NULL;
    sunsparcpart = NULL;
    sunx86part = NULL;
    gptpart = NULL;
    fatfs = NULL;
    fatsb = NULL;
    ntfsinfo = NULL;
    exfatsb = NULL;
    hfs = NULL;
    hsb = NULL;
    iso = NULL;
    p = NULL;
    s = NULL;
    yfs = NULL;
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
        return "UFS1";
    }
    else if(curinfo->ftype == TSK_FS_TYPE_FFS2 || curinfo->ftype == TSK_FS_TYPE_FFS_DETECT)
    {
        return QString::fromUtf8(reinterpret_cast<char*>(((FFS_INFO*)curinfo)->fs.sb2->volname));
    }
    else if(curinfo->ftype == TSK_FS_TYPE_FAT12 || curinfo->ftype == TSK_FS_TYPE_FAT16)
    {
        return QString::fromUtf8(reinterpret_cast<char*>(((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f16.vol_lab));
    }
    else if(curinfo->ftype == TSK_FS_TYPE_FAT32)
    {
        return QString::fromUtf8(reinterpret_cast<char*>(((FATXXFS_SB*)((FATFS_INFO*)curinfo)->boot_sector_buffer)->a.f32.vol_lab));
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
        // print_inode_name(((HFS_INFO*)fs), HFS_ROOT_INUM);
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

QStringList WombatProperties::PopulateVolumeProperties()
{
    proplist.clear();
    if(wombatptr->evidenceobject.volinfo != NULL) // valid volume object
    {
        if(wombatptr->evidenceobject.volinfo->vstype == TSK_VS_TYPE_DOS)
            proplist << "Signature" << "0xAA55" << "Signature Value should be 0xAA55 (0x1FE-0x1FF)";
        else if(wombatptr->evidenceobject.volinfo->vstype == TSK_VS_TYPE_BSD)
        {
            char* sect_buf;
            //uint32_t idx = 0;
            ssize_t cnt;
            //char* table_str;
            TSK_ENDIAN_ENUM endian = wombatptr->evidenceobject.volinfo->endian;
            sect_buf = (char*)tsk_malloc(wombatptr->evidenceobject.volinfo->block_size);
            bsdpart = (bsd_disklabel*) sect_buf;
            cnt = tsk_vs_read_block(wombatptr->evidenceobject.volinfo, BSD_PART_SOFFSET, sect_buf, wombatptr->evidenceobject.volinfo->block_size);
            if(cnt != wombatptr->evidenceobject.volinfo->block_size)
            {
                // print error here
            }
            free(sect_buf);
            proplist << "Signature" << QString::number(tsk_getu32(endian, bsdpart->magic)) << "Signature value should be 0x82564557 (0x00-0x03)";
            proplist << "Drive Type" << QString::number(tsk_getu16(endian, bsdpart->type)) << "Drive type (0x04-0x05)";
            proplist << "Drive Subtype" << QString::number(tsk_getu16(endian, bsdpart->sub_type)) << "Drive subtype (0x06-0x07)";
            proplist << "Drive Type Name" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->type_name)) << "The Drive type name (0x08-0x17)";
            proplist << "Pack Identifier Name" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->packname)) << "The Pack identifier name (0x18-0x27)";
            proplist << "Sector Size (bytes)" << QString::number(tsk_getu32(endian, bsdpart->sec_size)) << "Size of a sector in bytes (0x28-0x2B)";
            proplist << "Sectors per Track" << QString::number(tsk_getu32(endian, bsdpart->sec_per_tr)) << "Number of Sectors per track (0x2C-0x2F)";
            proplist << "Tracks per Cylinder" << QString::number(tsk_getu32(endian, bsdpart->tr_per_cyl)) << "Number of tracks per cylinder (0x30-0x33)";
            proplist << "Cylinders per Unit" << QString::number(tsk_getu32(endian, bsdpart->cyl_per_unit)) << "Number of cylinders per unit (0x34-0x37)";
            proplist << "Sectors per Cylinder" << QString::number(tsk_getu32(endian, bsdpart->sec_per_cyl)) << "Number of sectors per cylinder (0x38-0x3B)";
            proplist << "Sectors per Unit" << QString::number(tsk_getu32(endian, bsdpart->sec_per_unit)) << "Number of sectors per unit (0x3C-0x3F)";
            proplist << "Spare Sectors per Track" << QString::number(tsk_getu16(endian, bsdpart->spare_per_tr)) << "Number of spare sectors per track (0x40-0x41)";
            proplist << "Spare Sectors per Cylinder" << QString::number(tsk_getu16(endian, bsdpart->spare_per_cyl)) << "Number of spare sectors per cylinder (0x42-0x43)";
            proplist << "Alternate Cylinders Per Unit" << QString::number(tsk_getu32(endian, bsdpart->alt_per_unit)) << "Number of alternate cylinders per unit (0x44-0x47)";
            proplist << "Rotational Disk Speed" << QString::number(tsk_getu16(endian, bsdpart->rpm)) << "Rotational Speed of Disk (0x48-0x49)";
            proplist << "Hardware Sector Interleave" << QString::number(tsk_getu16(endian, bsdpart->interleave)) << "Hardware sector interleave (0x4A-0x4B)";
            proplist << "Track Skew" << QString::number(tsk_getu16(endian, bsdpart->trackskew)) << "Track skew (0x4C-0x4D)";
            proplist << "Cylinder Skew" << QString::number(tsk_getu16(endian, bsdpart->cylskew)) << "Cylinder skew (0x4E-0x4F)";
            proplist << "Head Switch Time" << QString::number(tsk_getu32(endian, bsdpart->headswitch)) << "Head switch time in microseconds (0x50-0x53)";
            proplist << "Track-to-Track Seek Time" << QString::number(tsk_getu32(endian, bsdpart->track_seek)) << "Track-to-Track seek time in microseconds (0x54-0x57)";
            proplist << "Flags" << QString::number(tsk_getu32(endian, bsdpart->flags)) << "Flags (0x58-0x5B)";
            proplist << "Drive Specific Data" << QString::fromUtf8(reinterpret_cast<char*>(bsdpart->drivedata)) << "Drive Specific Data (0x5C-0x6F)";
            proplist << "Reserved" << "Reserved" << "Reserved (0x70-0x83)";
            proplist << "Signature" << QString::number(tsk_getu32(endian, bsdpart->magic2)) << "Signature value should be 0x82564557 (0x84-0x87)";
            proplist << "Checksum" << QString::number(tsk_getu16(endian, bsdpart->checksum)) << "Checksum (0x88-0x89)";
            proplist << "Number of Partitions" << QString::number(tsk_getu16(endian, bsdpart->num_parts)) << "Number of partitions (0x8A-0x8B)";
            proplist << "Boot Area Size" << QString::number(tsk_getu32(endian, bsdpart->bootarea_size)) << "Size of boot area (0x8C-0x8F)";
            proplist << "Super Block Maximum Size" << QString::number(tsk_getu32(endian, bsdpart->sb_size)) << "Maximum size of the file system boot super block (0x90-0x93)";
            proplist << "Unused" << "Unused" << "Unused (0x0194-0x01FF)";
        }
        else if(wombatptr->evidenceobject.volinfo->vstype == TSK_VS_TYPE_SUN)
        {
            char* buf;
            ssize_t cnt;
            TSK_ENDIAN_ENUM endian = wombatptr->evidenceobject.volinfo->endian;
            buf = (char*)tsk_malloc(wombatptr->evidenceobject.volinfo->block_size);
            cnt = tsk_vs_read_block(wombatptr->evidenceobject.volinfo, SUN_SPARC_PART_SOFFSET, buf, wombatptr->evidenceobject.volinfo->block_size);
            if(cnt != wombatptr->evidenceobject.volinfo->block_size)
            {
                // print error here.
            }
            sunsparcpart = (sun_dlabel_sparc*)buf;
            sunx86part = (sun_dlabel_i386*)buf;
            if(tsk_vs_guessu16(wombatptr->evidenceobject.volinfo, sunsparcpart->magic, SUN_MAGIC) == 0)
            {
                if(tsk_getu32(endian, sunsparcpart->sanity) == SUN_SANITY) // populate sparc
                {
                    proplist << "ASCII Label" << QString::fromUtf8(reinterpret_cast<char*>(sunsparcpart->asciilabel)) << "ASCII label (0x00-0x7F)";
                    proplist << "Version" << QString::number(tsk_getu32(endian, sunsparcpart->version)) << "Version (0x80-0x83)";
                    proplist << "Volume Name" << QString::fromUtf8(reinterpret_cast<char*>(sunsparcpart->vol_name)) << "Volume Name (0x84-0x8C)";
                    proplist << "Number of Partitions" << QString::number(tsk_getu16(endian, sunsparcpart->num_parts)) << "Number of partitions (0x8D-0x8E)";
                    proplist << "Signature Sanity" << QString::number(tsk_getu32(endian, sunsparcpart->sanity)) << "Signature Value Sanity should be 0x600DDEEE (0xBC-0xBF)";
                    proplist << "Writing Sectors to Skip" << QString::number(tsk_getu16(endian, sunsparcpart->write_reinstruct)) << "Sectors to skip, writing (0x0106-0x0107)";
                    proplist << "Reading Sectors to Skip" << QString::number(tsk_getu16(endian, sunsparcpart->read_reinstruct)) << "Sectors to skip, reading (0x0108-0x0109)";
                    proplist << "Reserved" << "Reserved" << "Reserved (0x010A-0x01A3)";
                    proplist << "Disk Speed" << QString::number(tsk_getu16(endian, sunsparcpart->rpm)) << "Disk Speed (0x01A4-0x01A5)";
                    proplist << "Physical Cylinder Count" << QString::number(tsk_getu16(endian, sunsparcpart->num_ph_cyl)) << "Number of physical cylinders (0x01A6-0x01A7)";
                    proplist << "Alternates per Cylinder" << QString::number(tsk_getu16(endian, sunsparcpart->alt_per_cyl)) << "Alternates per cylinder (0x01A8-0x01A9)";
                    proplist << "Reserved" << "Reserved" << "Reserved (0x01AA-0x01AD)";
                    proplist << "Interleave" << QString::number(tsk_getu16(endian, sunsparcpart->interleave)) << "Interleave (0x01AE-0x01AF)";
                    proplist << "Data Cylinder Count" << QString::number(tsk_getu16(endian, sunsparcpart->num_cyl)) << "Number of data cylinders (0x01B0-0x01B1)";
                    proplist << "Alternate Cylinder Count" << QString::number(tsk_getu16(endian, sunsparcpart->num_alt_cyl)) << "Number of alternate cylinders (0x01B2-0x01B3)";
                    proplist << "Number of Heads" << QString::number(tsk_getu16(endian, sunsparcpart->num_head)) << "Number of heads (0x01B4-0x01B5)";
                    proplist << "Sectors per Track Count" << QString::number(tsk_getu16(endian, sunsparcpart->sec_per_tr)) << "Number of sectors per track (0x01B6-0x01B7)";
                    proplist << "Reserved" << "Reserved" << "Reserved (0x01B8-0x01BB)";
                    proplist << "Signature" << QString::number(tsk_getu16(endian, sunsparcpart->magic)) << "Signature value should be 0xDABE (0x01FC-0x01FD)";
                    proplist << "Checksum" << QString::number(tsk_getu16(endian, sunsparcpart->checksum)) << "Checksum (0x01FE-0x01FF)";
                }
                else if(tsk_getu32(endian, sunx86part->sanity) == SUN_SANITY) // populate i386
                {
                    proplist << "Signature Sanity" << QString::number(tsk_getu32(endian, sunx86part->sanity)) << "Signature Value Sanity should be 0x600DDEEE (0x0C-0x0F)";
                    proplist << "Version" << QString::number(tsk_getu32(endian, sunx86part->version)) << "Version (0x10-0x13)";
                    proplist << "Volume Name" << QString::fromUtf8(reinterpret_cast<char*>(sunx86part->vol_name)) << "Volume Name (0x14-0x1B)";
                    proplist << "Sector Size" << QString::number(tsk_getu16(endian, sunx86part->sec_size)) << "Sector sie (0x1C-0x1D)";
                    proplist << "Number of Partitions" << QString::number(tsk_getu16(endian, sunx86part->num_parts)) << "Number of partitions (0x1E-0x1F)";
                    proplist << "Volume Label" << QString::fromUtf8(reinterpret_cast<char*>(sunx86part->asciilabel)) << "Volume label (0x0148-0x01C7)";
                    proplist << "Physical Cylinder Count" << QString::number(tsk_getu32(endian, sunx86part->num_ph_cyl)) << "Number of physical cylinders (0x01C8-0x01CB)";
                    proplist << "Data Cylinder Count" << QString::number(tsk_getu32(endian, sunx86part->num_cyl)) << "Number of data cylinders (0x01CC-0x01CF)";
                    proplist << "Alternate Cylinder Count" << QString::number(tsk_getu16(endian, sunx86part->num_alt_cyl)) << "Number of alternate cylinders (0x01D0-0x01D1)";
                    proplist << "Cylinder offset" << QString::number(tsk_getu16(endian, sunx86part->cyl_offset)) << "Cylinder offset (0x01D2-0x01D3)";
                    proplist << "Number of Heads" << QString::number(tsk_getu32(endian, sunx86part->num_head)) << "Number of heads (0x01D4-0x01D7)";
                    proplist << "Sectors per Track Count" << QString::number(tsk_getu32(endian, sunx86part->sec_per_tr)) << "Number of sectors per track (0x01D8-0x01DB)";
                    proplist << "Interleave" << QString::number(tsk_getu16(endian, sunx86part->interleave)) << "Interleave (0x01DC-0x01DD)";
                    proplist << "Skew" << QString::number(tsk_getu16(endian, sunx86part->skew)) << "Skew (0x01DE-0x01DF)";
                    proplist << "Alternates per Cylinder" << QString::number(tsk_getu16(endian, sunx86part->alt_per_cyl)) << "Number of alternates per cylinder (0x01E0-0x01E1)";
                    proplist << "Disk Speed" << QString::number(tsk_getu16(endian, sunx86part->rpm)) << "Disk Speed (0x01E2-0x01E3)";
                    proplist << "Writing Sectors to Skip" << QString::number(tsk_getu16(endian, sunx86part->write_reinstruct)) << "Sectors to skip, writing (0x01E4-0x01E5)";
                    proplist << "Reading Sectors to Skip" << QString::number(tsk_getu16(endian, sunx86part->read_reinstruct)) << "Sectors to skip, reading (0x01E6-0x01E7)";
                    proplist << "Reserved" << "Reserved" << "Reserved (0x01E8-0x01EF)";
                    proplist << "Reserved" << "Reserved" << "Reserved (0x01F0-0x0x01FB)";
                    proplist << "Signature" << QString::number(tsk_getu16(endian, sunx86part->magic)) << "Signature value should be 0xDABE (0x01FC-0x01FD)";
                    proplist << "Checksum" << QString::number(tsk_getu16(endian, sunx86part->checksum)) << "Checksum (0x01FE-0x01FF)";
                }
                free(buf);
            }
        }
        else if(wombatptr->evidenceobject.volinfo->vstype == TSK_VS_TYPE_MAC)
        {
            TSK_ENDIAN_ENUM endian = wombatptr->evidenceobject.volinfo->endian;
            macpart = (mac_part*)wombatptr->evidenceobject.volinfo;
            proplist << "Signature" << "0x504D" << "Signature Value should be 0x504D (0x00-0x01)";
            proplist << "Reserved" << "Reserved" << "Reserved (0x02-0x03)";
            proplist << "Number of Partitions" << QString::number(tsk_getu32(endian, macpart->pmap_size)) << "Total Number of Partitions (0x08-0x0B)";
        }
        else if(wombatptr->evidenceobject.volinfo->vstype == TSK_VS_TYPE_GPT)
        {
            char* sect_buf;
            ssize_t cnt;
            TSK_ENDIAN_ENUM endian = wombatptr->evidenceobject.volinfo->endian;
            sect_buf = (char*)tsk_malloc(wombatptr->evidenceobject.volinfo->block_size);
            cnt = tsk_vs_read_block(wombatptr->evidenceobject.volinfo, GPT_PART_SOFFSET + 1, sect_buf, wombatptr->evidenceobject.volinfo->block_size);
            if(cnt != wombatptr->evidenceobject.volinfo->block_size)
            {
                // print error here
            }
            free(sect_buf);
            proplist << "Signature" << QString::number(tsk_getu64(endian, gptpart->signature)) << "Signature Value should ve EFI PART (0x00-0x07)";
            proplist << "Version" << QString::number(tsk_getu32(endian, gptpart->version)) << "Version (0x08-0x0B)";
            proplist << "GPT Header Size (bytes)" << QString::number(tsk_getu32(endian, gptpart->head_size_b)) << "Size of GPT header in bytes (0x0C-0x0F)";
            proplist << "GPT Header Checksum" << QString::number(tsk_getu32(endian, gptpart->head_crc)) << "CRC32 checksum of GPT header (0x10-0x13)";
            proplist << "Reserved" << "Reserved" << "Reserved (0x14-0x17)";
            proplist << "Current GPT Header LBA" << QString::number(tsk_getu64(endian, gptpart->head_lba)) << "Logical Block Address of the current GPT header structure (0x18-0x1F)";
            proplist << "Other GPT Header LBA" << QString::number(tsk_getu64(endian, gptpart->head2_lba)) << "Logical Block Address of the other GPT header structure (0x20-0x27)";
            proplist << "Partition Area Start LBA" << QString::number(tsk_getu64(endian, gptpart->partarea_start)) << "Logical Block Address of the start of the partition area (0x28-0x2F)";
            proplist << "Partition End Area LBA" << QString::number(tsk_getu64(endian, gptpart->partarea_end)) << "Logical Block Address of the end of the partition area (0x30-0x37)";
            sprintf(asc, "%" PRIx64 "%" PRIx64 "", tsk_getu64(endian, &(gptpart->guid[8])), tsk_getu64(endian, &(gptpart->guid[0])));
            proplist << "Disk GUID" << QString::fromStdString(string(asc)) << "Disk GUID (0x38-0x47)";
            proplist << "LBA of Start of the Partition Table" << QString::number(tsk_getu64(endian, gptpart->tab_start_lba)) << "Logical Block Address of the start of the partition table (0x48-0x4F)";
            proplist << "Number of Partition Table Entries" << QString::number(tsk_getu32(endian, gptpart->tab_num_ent)) << "Number of entries in the partition table (0x50-0x53)";
            proplist << "Partition Table Entry Size" << QString::number(tsk_getu32(endian, gptpart->tab_size_b)) << "Size of each entry in the partition table (0x54-0x57)";
            proplist << "Partition Table Checksum" << QString::number(tsk_getu32(endian, gptpart->tab_crc)) << "CRC32 checksum of the partition table (0x58-0x5B)";
            proplist << "Reserved" << "Reserved" << "Reserved (0x5C-0x01FF)";
        }
    }
    return proplist;
}

QStringList WombatProperties::PopulateFileSystemProperties(TSK_FS_INFO* curfsinfo)
{
    proplist.clear();
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
            proplist << "Cylinder Group Fragment Address" << QString::number(tsk_gets64(curfsinfo->endian, sb2->cg_saddr)) << "Fragment address of cylinder group summary area (0x0448-0x044F)";
            proplist << "Unused" << "Unused" << "Unused (0x0450-0x051F)";
            int flags = tsk_getu32(curfsinfo->endian, sb2->fs_flags);
            if(flags & FFS_SB_FLAG_UNCLEAN)
                proplist << "General Flags" << "Unclean" << "Set when the file system is mounted (0x0520-0x0523)";
            if(flags & FFS_SB_FLAG_SOFTDEP)
                proplist << "General Flags" << "Soft Dependencies" << "Soft dependencies are being used (0x0520-0x0523)";
            if(flags & FFS_SB_FLAG_NEEDFSCK)
                proplist << "General Flags" << "Needs Check" << "Needs consistency check next time the file system is mounted (0x0520-0x0523)";
            if(flags & FFS_SB_FLAG_INDEXDIR)
                proplist << "General Flags" << "Index Directories" << "Directories are indexed using a hashtree or B-Tree (0x0520-0x0523)";
            if(flags & FFS_SB_FLAG_ACL)
                proplist << "General Flags" << "Access Control Lists" << "Access control lists are being used (0x0520-0x0523)";
            if(flags & FFS_SB_FLAG_MULTILABEL)
                proplist << "General Flags" << "TrustedBSD MAC Multi-Label" << "TrustedBSD Mandatory Access Control multi-labels are being used (0x0520-0x0523)";
            if(flags & FFS_SB_FLAG_UPDATED)
                proplist << "General Flags" << "Updated Flag Location" << "Flags have been moved (0x0520-0x0523)";
            proplist << "Unused" << "Unused" << "Unused (0x0524-0x055B)";
            proplist << "Signature" << QString::number(tsk_gets32(curfsinfo->endian, sb2->magic)) << "File system signature value should be 0x19540119 (0x055C-0x055F)";
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
        proplist << "File System Type";
        if(curfsinfo->ftype == TSK_FS_TYPE_FAT12)
            proplist << "FAT12";
        else if(curfsinfo->ftype == TSK_FS_TYPE_FAT16)
            proplist << "FAT16";
        else if(curfsinfo->ftype == TSK_FS_TYPE_FAT32)
            proplist << "FAT32";
        proplist << "File System Type";
        proplist << "Reserved" << "Jump Code" << "Assembly instructions to jump to boot code (0x00-0x02)";
        proplist << "OEM Name" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->oemname)) << "OEM name in ASCII (0x03-0x0A)";
        proplist << "Bytes per Sector" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->ssize)) << "Sector size in bytes. Allowed values include 512, 1024, 2048, and 4096 (0x0B-0x0C)";
        proplist << "Sectors per Cluster" << QString::number(fatsb->csize) << "Cluster size in sectors. Allowed values are powers of 2, but the cluster size must be 32KB or smaller (0x0D-0x0D)";
        proplist << "Reserved Area Size" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->reserved)) << "Number of reserved sectors for boot sectors (0x0E-0x0F)";
        proplist << "Number of FATs" << QString::number(fatsb->numfat) << "Number of File Allocation Tables (FATs). Typically two for redundancy (0x10-0x10)";
        proplist << "Number of Root Directory Entries" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->numroot)) << "Maximum number of files in the root directory for FAT12 and FAT16. This is 0 for FAT32 and typically 512 for FAT16 (0x11-0x12)";
        proplist << "Number of Sectors in File System" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->sectors16)) << "Maximum number of sectors in the file system. If the number of sectors is larger than can be represented in this 2-byte value, a 4-byte value exists later in the data structure and this should be 0 (0x13-0x14)";
        proplist << "Media Type" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->f2)) << "Media type. Should be 0xF8 for fixed disks and 0xF0 for removable disks (0x13-0x13)";
        proplist << "Size of FAT" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->sectperfat16)) << "16-bit size in sectors of each FAT for FAT12 and FAT16. For FAT32, this field is 0 (0x14-0x15)";
        proplist << "Reserved" << "Reserved" << "Reserved (0x16-0x1B)";
        proplist << "Number of Sectors Before Partition Start" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->prevsect)) << "Number of sectors before the start of the file system partition (0x1C-0x1F)";
        proplist << "Number of Sectors in File System" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->sectors32)) << "32-bit value of number of sectors in the file system. Either this value or the 16-bit value above must be 0 (0x20-0x23)";
        if(curfsinfo->ftype == TSK_FS_TYPE_FAT32)
        {
            proplist << "Size of FAT" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.sectperfat32)) << "32-bit size in sectors of one FAT (0x24-0x27)";
            proplist << "Defines FAT is Written" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.ext_flag)) << "Defines how multiple FAT structures are written to. If bit 7 is 1, only one of the FAT structures is active and its index is described in bits 0-3. Otherwise all FAT structures are mirrors of each other (0x28-0x29)";
            proplist << "Major and Minor Version" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.fs_ver)) << "The major and minor version number (0x2A-0x2B)";
            proplist << "Root Directory Cluster" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.rootclust)) << "Cluster where the root directory can be found (0x2C-0x2F)";
            proplist << "FSINFO Structure Sector" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.fsinfo)) << "Sector where FSINFO structure can be found (0x30-0x31)";
            proplist << "Boot Sector Backup Copy" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->a.f32.bs_backup)) << "Sector where the backup copy of the boot sector is located, default is 6 (0x32-0x33)";
            proplist << "Reserved" << "Reserved" << "Reserved (0x34-0x3F)";
            proplist << "BIOS Drive Number" << QString::number(fatsb->a.f32.drvnum) << "BIOS INT32h drive number (0x40-0x40)";
            proplist << "Not used" << "Not used" << "Not used (0x41-0x42)";
            proplist << "Volume Serial Number" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f32.vol_id)) << "Volume serial number, which some versions of Windows will calculate based on the creation date and time (0x43-0x46)";
            proplist << "Volume Label" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f32.vol_lab)) << "Volume label in ASCII. The user chooses this value when creating the file system (0x47-0x51)";
            proplist << "File System Type" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f32.fs_type)) << "File system type label in ASCII. Standard values include \"FAT32\", but nothing is required (0x52-0x59)";
            proplist << "Not Used" << "Not Used" << "Not Used (0x005A-0x01FD)";
        }
        else
        {
            proplist << "Reserved" << "Reserved" << "Reserved (0x24-0x26)";
            proplist << "Volume Serial Number" << QString::number(tsk_getu32(curfsinfo->endian, fatsb->a.f16.vol_id)) << "Volume serial number, which some versions of Windows will calculate based on the creation date and time (0x27-0x2A)";
            proplist << "Volume Label" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f16.vol_lab)) << "Volume label in ASCII. The user chooses this value when creating the file system (0x2B-0x35)";
            proplist << "File System Type" << QString::fromUtf8(reinterpret_cast<char*>(fatsb->a.f16.fs_type)) << "File system type in ASCII. Standard values include \"FAT\", \"FAT12\", \"FAT16\", but nothing is required (0x36->0x3D)";
            proplist << "Reserved" << "Reserved" << "Reserved (0x3E-0x01FD)";
        }
        proplist << "Signature" << QString::number(tsk_getu16(curfsinfo->endian, fatsb->magic)) << "Signature value should be 0xAA55 (0x01FE-0x01FF)";
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_NTFS)
    {
        ntfsinfo = (NTFS_INFO*)curfsinfo;
        proplist << "Assembly Boot Code" << "Assembly Boot Code" << "Assembly instruction to jump to boot code (0x00-0x02)";
        proplist << "OEM Name" << QString::fromUtf8(ntfsinfo->fs->oemname) << "OEM Name (0x03-0x0A)";
        proplist << "Bytes per Sector" << QString::number(tsk_getu16(curfsinfo->endian, ntfsinfo->fs->ssize)) << "Bytes per sector (0x0B-0x0C)";
        proplist << "Sectors per Cluster" << QString::number(ntfsinfo->fs->csize) << "Sectors per cluster (0x0D-0x0D)";
        proplist << "Reserved Sectors" << "Reserved Sectors" << "Reserved and Unused Sectors (0x0E-0x27)";
        proplist << "Volume Size (sectors)" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->vol_size_s)) << "Total Sectors in the file system (0x28-0x2F)";
        proplist << "MFT Starting Cluster Address" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mft_clust)) << "Starting cluster address of the Master File Table (MFT) (0x30-0x37)";
        proplist << "MFT Mirror Starting Cluster Address" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->mftm_clust)) << "Starting cluster address of the MFT Mirror (0x38-0x3F)";
        proplist << "MFT Record Size (clusters)" << QString::number(ntfsinfo->fs->mft_rsize_c) << "Size of file record (MFT Entry) (0x40-0x40)";
        proplist << "Unused" << "Unused" << "Unused (0x41-0x43)";
        proplist << "Size of Index Record" << QString::number(ntfsinfo->fs->idx_rsize_c) << "Number of clusters per index record (0x44-0x44)";
        proplist << "Unused" << "Unused" << "Unused (0x45-0x47)";
        proplist << "Serial Number" << QString::number(tsk_getu64(curfsinfo->endian, ntfsinfo->fs->serial)) << "Serial Number (0x48-0x4F)";
        proplist << "Boot Code" << "Boot Code" << "Boot Code (0x0050-0x00FD)";
        proplist << "Signature" << QString::number(tsk_getu16(curfsinfo->endian, ntfsinfo->fs->magic)) << "Signature value should be 0xAA55 (0x00FE-0x00FF)";
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
            proplist << "Volume Name" << QString::fromStdString(string(asc)) << "Volume Name from $VOLUME_NAME attribute";
            proplist << "Version";
            if(ntfsinfo->ver == NTFS_VINFO_NT)
                proplist << "Windows NT";
            else if(ntfsinfo->ver == NTFS_VINFO_2K)
                proplist << "Windows 2000";
            else if(ntfsinfo->ver == NTFS_VINFO_XP)
                proplist << "Windows XP";
            else
                proplist << "Newer than Windows XP";
            proplist << "Version Information";
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_EXFAT)
    {
        fatfs = (FATFS_INFO*)curfsinfo;
        exfatsb = (EXFATFS_MASTER_BOOT_REC*)&(fatfs->boot_sector_buffer);
        proplist << "Jump Boot" << "Jump Boot" << "Jump boot should be 0xEB7690 (0x00-0x02)";
        proplist << "File System Name" << QString::fromUtf8(reinterpret_cast<char*>(exfatsb->fs_name)) << "File system name should be \"EXFAT \" (0x03-0x0A)";
        proplist << "Unused" << "Unused" << "Unused, must be zeros (0x0B-0x3F)";
        proplist << "Partition Offset" << QString::number(tsk_getu64(curfsinfo->endian, exfatsb->partition_offset)) << "Sector address for partition start (0x40-0x47)";
        proplist << "Volume Length (sectors)" << QString::number(tsk_getu64(curfsinfo->endian, exfatsb->vol_len_in_sectors)) << "Size of exFAT volume in sectors (0x48-0x4F)";
        proplist << "FAT Offset" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->fat_offset)) << "FAT offset in sectors (0x50-0x53)";
        proplist << "FAT Length" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->fat_len_in_sectors)) << "FAT length in sectors. May exceed the required space in order to align the second FAT (0x54-0x57)";
        proplist << "Cluster Heap Offset" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->cluster_heap_offset)) << "Sector address of the data region (0x58-0x5B)";
        proplist << "Cluster Count" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->cluster_cnt)) << "Number of clusters in the cluster heap (0x5C-0x5F)";
        proplist << "Root Directory Cluster" << QString::number(tsk_getu32(curfsinfo->endian, exfatsb->root_dir_cluster)) << "Cluster address of the root directory (0x60-0x63)";
        proplist << "Volume Serial Number" << QString::fromUtf8(reinterpret_cast<char*>(exfatsb->vol_serial_no)) << "Volume serial number (0x64-0x67)";
        proplist << "File System Revision" << QString::number(exfatsb->fs_revision[1]) + "." + QString::number(exfatsb->fs_revision[0]) << "File system revision as Major.Minor (0x68-0x69)";
        proplist << "Volume Flags" << QString::number(tsk_getu16(curfsinfo->endian, exfatsb->vol_flags)) << "Volume Flags (0x6A-0x6B)";
        proplist << "Bytes Per Sector" << QString::number(exfatsb->bytes_per_sector) << "Bytes per sector as a power of 2. Minimum 9 (512 bytes per sector), maximum 12 (4096 bytes per sector) (0x6C-0x6C)";
        proplist << "Sectors Per Cluster" << QString::number(exfatsb->sectors_per_cluster) << "Sectors per cluster as a power of 2. Minimum 0 (1 sector per cluster) up to a maximum 25 (0x6D-0x6D)";
        proplist << "Number of FATs" << QString::QString::number(exfatsb->num_fats) << "Number of FATs: 1 or 2, 2 is for TexFAT only) (0x6E-0x6E)";
        proplist << "Drive Select" << QString::number(exfatsb->drive_select) << "Used by INT 13h, typically 0x80 (0x6F-0x6F)";
        proplist << "Percent of the Heap in Use" << QString::number(exfatsb->percent_of_cluster_heap_in_use) << "0-100, percentage of allocated clusters rounded down to the integer; 0xFF, percentage is not available (0x70-0x70)";
        proplist << "Reserved" << "Reserved" << "Reserved (0x71-0x77)";
        proplist << "Boot Code" << "Boot Code" << "Boot Code (0x78-0x01FD)";
        proplist << "Signature" << QString::number(tsk_getu16(curfsinfo->endian, exfatsb->signature)) << "Signature value should be 0xAA55 (0x01FE-0x01FF)";
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_ISO9660) // Byte offset's aren't working out too well right now.
    {
        iso = (ISO_INFO*)curfsinfo;
        for(p = iso->pvd; p!= NULL; p = p->next)
        {
            proplist << "Unused" << "Unused" << "Unused (0x00-0x07)";
            proplist << "System Identifier" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.sys_id), 32) << "System Identifier (0x08-0x27)";
            proplist << "Volume Identifier" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.vol_id)) << "Volume Identifier (0x28-0x47)";
            proplist << "Unused" << "Unused" << "Unused should be all zeros (0x48-0x4F)";
            proplist << "Volume Space Size (LE)" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.vs_sz_l)) << "Volume Space Size in blocks (0x50-0x53)";
            proplist << "Volume Space Size (BE)" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.vs_sz_m)) << "Volume Space Size in blocks (0x54-0x57)";
            proplist << "Unused" << "Unused" << "Unused. All zeros (0x58-0x77)";
            proplist << "Volume Set Size (LE)" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_set_l)) << "The size of the set in this logical volume (0x78-0x79)";
            proplist << "Volume Set Size (BE)" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_set_m)) << "The size of the set in this logical volume (0x7A-0x7B)";
            proplist << "Volume Sequence Number (LE)" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_seq_l)) << "The number of this disk in the volume set (0x7C-0x7D)";
            proplist << "Volume Sequence Number (BE)" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.vol_seq_m)) << "The number of this disk in the volume set (0x7E-0x7F)";
            proplist << "Logical Block Size (LE)" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.blk_sz_l)) << "The size in bytes of a logical block (0x80-0x81)";
            proplist << "Logical Block Size (BE)" << QString::number(tsk_getu16(curfsinfo->endian, p->pvd.blk_sz_m)) << "The size in bytes of a logical block (0x82-0x83)";
            proplist << "Path Table Size (LE)" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_size_l)) << "The size in bytes of the path table (0x84-0x87)";
            proplist << "Path Table Size (BE)" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_size_m)) << "The size in bytes of the path table (0x88-0x8B)";
            proplist << "Logical Block Address of Type-L Path Table" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_loc_l)) << "LBA location of the path table (0x8C-0x8F)";
            proplist << "Logical Block Address of Optional Type-L Path Table" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_opt_loc_l)) << "LBA location of the optional path table (0x90-0x93)";
            proplist << "Logical Block Address of the Type-M Path Table" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_loc_m)) << "LBA location of the path table (0x94-0x97)";
            proplist << "Logical Block Address of the Optional Type-M Path Table" << QString::number(tsk_getu32(curfsinfo->endian, p->pvd.pt_opt_loc_m)) << "LBA location of the optional path table (0x98-0x9B)";
            proplist << "Directory Entry for Root Directory" << QString::fromUtf8(reinterpret_cast<char*>(p->pvd.dir_rec.name)) << "Directory Entry for Root Directory (0xB6-0xBD)";
            snprintf(asc128, 128, "%s", p->pvd.vol_setid);
            proplist << "Volume Set Identifier" << QString::fromStdString(string(asc128)) << "Identifier of the volume set of which this volume is a member (0x00BE-0x013D)";
            snprintf(asc128, 128, "%s", p->pvd.pub_id);
            proplist << "Publisher Identifier" << QString::fromStdString(string(asc128)) << "Volume publisher (0x013E-0x01BD)";
            snprintf(asc128, 128, "%s", p->pvd.prep_id);
            proplist << "Data Preparer Identifier" << QString::fromStdString(string(asc128)) << "Identifier of the person(s) who prepared the data for this volume (0x01BE-0x023D)";
            snprintf(asc128, 128, "%s", p->pvd.app_id);
            proplist << "Application Identifier" << QString::fromStdString(string(asc128)) << "Identifies how the data are recorded on this volume (0x023E-0x02BD)";
            snprintf(asc128, 38, "%s", p->pvd.copy_id);
            proplist << "Copyright File Identifier" << QString::fromStdString(string(asc128)) << "Fielname of a file in the root directory that contains copyright information for the volume set (0x02BE-0x02E3)";
            snprintf(asc128, 36, "%s", p->pvd.abs_id);
            proplist << "Abstract File Identifier" << QString::fromStdString(string(asc128)) << "Filename of a file in the root directory that contains abstract information for the volume set (0x02E4-0x0307)";
            snprintf(asc128, 37, "%s", p->pvd.bib_id);
            proplist << "Bibliographic File Identifier" << QString::fromStdString(string(asc128)) << "Filename of a file in the root directory that contains bibliographic information for this volume set (0x0308-0x032C)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", ((char)(p->pvd.make_date.month[0])), (char)p->pvd.make_date.month[1], (char)p->pvd.make_date.day[0], (char)p->pvd.make_date.day[1], (char)p->pvd.make_date.year[0], (char)p->pvd.make_date.year[1], (char)p->pvd.make_date.year[2], (char)p->pvd.make_date.year[3], (char)p->pvd.make_date.hour[0], (char)p->pvd.make_date.hour[1], (char)p->pvd.make_date.min[0], (char)p->pvd.make_date.min[1], (char)p->pvd.make_date.sec[0], (char)p->pvd.make_date.sec[1]);
            proplist << "Volume Creation Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(p->pvd.make_date.gmt_off) << "The date and time when the volume was created (0x032D-0x033D)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.mod_date.month[0], (char)p->pvd.mod_date.month[1], (char)p->pvd.mod_date.day[0], (char)p->pvd.mod_date.day[1], (char)p->pvd.mod_date.year[0], (char)p->pvd.mod_date.year[1], (char)p->pvd.mod_date.year[2], (char)p->pvd.mod_date.year[3], (char)p->pvd.mod_date.hour[0], (char)p->pvd.mod_date.hour[1], (char)p->pvd.mod_date.min[0], (char)p->pvd.mod_date.min[1], (char)p->pvd.mod_date.sec[0], (char)p->pvd.mod_date.sec[1]);
            proplist << "Volume Modification Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(p->pvd.mod_date.gmt_off) << "The date and time when the volume was modified (0x033E-0x034E)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.exp_date.month[0], (char)p->pvd.exp_date.month[1], (char)p->pvd.exp_date.day[0], (char)p->pvd.exp_date.day[1], (char)p->pvd.exp_date.year[0], (char)p->pvd.exp_date.year[1], (char)p->pvd.exp_date.year[2], (char)p->pvd.exp_date.year[3], (char)p->pvd.exp_date.hour[0], (char)p->pvd.exp_date.hour[1], (char)p->pvd.exp_date.min[0], (char)p->pvd.exp_date.min[1], (char)p->pvd.exp_date.sec[0], (char)p->pvd.exp_date.sec[1]);
            proplist << "Volume Expiration Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(p->pvd.exp_date.gmt_off) << "The date and time after which the volume is considered to be obsolete. If not specified, then the volume is never considered to be obsolete (0x034F-0x035F)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)p->pvd.ef_date.month[0], (char)p->pvd.ef_date.month[1], (char)p->pvd.ef_date.day[0], (char)p->pvd.ef_date.day[1], (char)p->pvd.ef_date.year[0], (char)p->pvd.ef_date.year[1], (char)p->pvd.ef_date.year[2], (char)p->pvd.ef_date.year[3], (char)p->pvd.ef_date.hour[0], (char)p->pvd.ef_date.hour[1], (char)p->pvd.ef_date.min[0], (char)p->pvd.ef_date.min[1], (char)p->pvd.ef_date.sec[0], (char)p->pvd.ef_date.sec[1]);
            proplist << "Volume Effective Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(p->pvd.ef_date.gmt_off) << "The date and time after which the volume may be used. If not specified, then the volume may be used immediately (0x0360-0x0370)";
            proplist << "File Structure Version" << QString::number(p->pvd.fs_ver) << "The directory records and path table version, always 0x01 (0x0371-0x0371)";
            proplist << "Unused" << "Unused" << "Unused, always 0x00 (0x0372-0x0372)";
            snprintf(asc, 512, "%s", p->pvd.app_use);
            proplist << "Application Used" << QString::fromStdString(string(asc)) << "Application Used. Contents not defined by ISO9660 (0x0373-0x0572)";
            proplist << "Reserved by ISO" << "Reserved" << "Reserved by ISO (0x0573-0x07FF)";
        }
        for(s = iso->svd; s!= NULL; s = s->next)
        {
            proplist << "Volume Flags" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.flags)) << "Unused (0x00-0x07)";
            proplist << "System Identifier" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.sys_id), 32) << "System Identifier (0x08-0x27)";
            proplist << "Volume Identifier" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.vol_id)) << "Volume Identifier (0x28-0x47)";
            proplist << "Unused" << "Unused" << "Unused should be all zeros (0x48-0x4F)";
            proplist << "Volume Space Size (LE)" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.vs_sz_l)) << "Volume Space Size in blocks (0x50-0x53)";
            proplist << "Volume Space Size (BE)" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.vs_sz_m)) << "Volume Space Size in blocks (0x54-0x57)";
            proplist << "Unused" << "Unused" << "Unused. All zeros (0x58-0x77)";
            proplist << "Volume Set Size (LE)" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_set_l)) << "The size of the set in this logical volume (0x78-0x79)";
            proplist << "Volume Set Size (BE)" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_set_m)) << "The size of the set in this logical volume (0x7A-0x7B)";
            proplist << "Volume Sequence Number (LE)" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_seq_l)) << "The number of this disk in the volume set (0x7C-0x7D)";
            proplist << "Volume Sequence Number (BE)" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.vol_seq_m)) << "The number of this disk in the volume set (0x7E-0x7F)";
            proplist << "Logical Block Size (LE)" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.blk_sz_l)) << "The size in bytes of a logical block (0x80-0x81)";
            proplist << "Logical Block Size (BE)" << QString::number(tsk_getu16(curfsinfo->endian, s->svd.blk_sz_m)) << "The size in bytes of a logical block (0x82-0x83)";
            proplist << "Path Table Size (LE)" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_size_l)) << "The size in bytes of the path table (0x84-0x87)";
            proplist << "Path Table Size (BE)" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_size_m)) << "The size in bytes of the path table (0x88-0x8B)";
            proplist << "Logical Block Address of Type-L Path Table" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_loc_l)) << "LBA location of the path table (0x8C-0x8F)";
            proplist << "Logical Block Address of Optional Type-L Path Table" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_opt_loc_l)) << "LBA location of the optional path table (0x90-0x93)";
            proplist << "Logical Block Address of the Type-M Path Table" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_loc_m)) << "LBA location of the path table (0x94-0x97)";
            proplist << "Logical Block Address of the Optional Type-M Path Table" << QString::number(tsk_getu32(curfsinfo->endian, s->svd.pt_opt_loc_m)) << "LBA location of the optional path table (0x98-0x9B)";
            proplist << "Directory Entry for Root Directory" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.dir_rec.name)) << "Directory Entry for Root Directory (0xB6-0xBD)";
            snprintf(asc128, 128, "%s", s->svd.vol_setid);
            proplist << "Volume Set Identifier" << QString::fromStdString(string(asc128)) << "Identifier of the volume set of which this volume is a member (0x00BE-0x013D)";
            snprintf(asc128, 128, "%s", s->svd.pub_id);
            proplist << "Publisher Identifier" << QString::fromStdString(string(asc128)) << "Volume publisher (0x013E-0x01BD)";
            snprintf(asc128, 128, "%s", s->svd.prep_id);
            proplist << "Data Preparer Identifier" << QString::fromStdString(string(asc128)) << "Identifier of the person(s) who prepared the data for this volume (0x01BE-0x023D)";
            snprintf(asc128, 128, "%s", s->svd.app_id);
            proplist << "Application Identifier" << QString::fromStdString(string(asc128)) << "Identifies how the data are recorded on this volume (0x023E-0x02BD)";
            snprintf(asc128, 38, "%s", s->svd.copy_id);
            proplist << "Copyright File Identifier" << QString::fromStdString(string(asc128)) << "Fielname of a file in the root directory that contains copyright information for the volume set (0x02BE-0x02E3)";
            snprintf(asc128, 36, "%s", s->svd.abs_id);
            proplist << "Abstract File Identifier" << QString::fromStdString(string(asc128)) << "Filename of a file in the root directory that contains abstract information for the volume set (0x02E4-0x0307)";
            snprintf(asc128, 37, "%s", s->svd.bib_id);
            proplist << "Bibliographic File Identifier" << QString::fromStdString(string(asc128)) << "Filename of a file in the root directory that contains bibliographic information for this volume set (0x0308-0x032C)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", ((char)(s->svd.make_date.month[0])), (char)s->svd.make_date.month[1], (char)s->svd.make_date.day[0], (char)s->svd.make_date.day[1], (char)s->svd.make_date.year[0], (char)s->svd.make_date.year[1], (char)s->svd.make_date.year[2], (char)s->svd.make_date.year[3], (char)s->svd.make_date.hour[0], (char)s->svd.make_date.hour[1], (char)s->svd.make_date.min[0], (char)s->svd.make_date.min[1], (char)s->svd.make_date.sec[0], (char)s->svd.make_date.sec[1]);
            proplist << "Volume Creation Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(s->svd.make_date.gmt_off) << "The date and time when the volume was created (0x032D-0x033D)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.mod_date.month[0], (char)s->svd.mod_date.month[1], (char)s->svd.mod_date.day[0], (char)s->svd.mod_date.day[1], (char)s->svd.mod_date.year[0], (char)s->svd.mod_date.year[1], (char)s->svd.mod_date.year[2], (char)s->svd.mod_date.year[3], (char)s->svd.mod_date.hour[0], (char)s->svd.mod_date.hour[1], (char)s->svd.mod_date.min[0], (char)s->svd.mod_date.min[1], (char)s->svd.mod_date.sec[0], (char)s->svd.mod_date.sec[1]);
            proplist << "Volume Modification Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(s->svd.mod_date.gmt_off) << "The date and time when the volume was modified (0x033E-0x034E)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.exp_date.month[0], (char)s->svd.exp_date.month[1], (char)s->svd.exp_date.day[0], (char)s->svd.exp_date.day[1], (char)s->svd.exp_date.year[0], (char)s->svd.exp_date.year[1], (char)s->svd.exp_date.year[2], (char)s->svd.exp_date.year[3], (char)s->svd.exp_date.hour[0], (char)s->svd.exp_date.hour[1], (char)s->svd.exp_date.min[0], (char)s->svd.exp_date.min[1], (char)s->svd.exp_date.sec[0], (char)s->svd.exp_date.sec[1]);
            proplist << "Volume Expiration Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(s->svd.exp_date.gmt_off) << "The date and time after which the volume is considered to be obsolete. If not specified, then the volume is never considered to be obsolete (0x034F-0x035F)";
            sprintf(asc, "%c%c/%c%c/%c%c%c%c %c%c:%c%c:%c%c", (char)s->svd.ef_date.month[0], (char)s->svd.ef_date.month[1], (char)s->svd.ef_date.day[0], (char)s->svd.ef_date.day[1], (char)s->svd.ef_date.year[0], (char)s->svd.ef_date.year[1], (char)s->svd.ef_date.year[2], (char)s->svd.ef_date.year[3], (char)s->svd.ef_date.hour[0], (char)s->svd.ef_date.hour[1], (char)s->svd.ef_date.min[0], (char)s->svd.ef_date.min[1], (char)s->svd.ef_date.sec[0], (char)s->svd.ef_date.sec[1]);
            proplist << "Volume Effective Date/Time" << QString::fromStdString(string(asc)) + ConvertGmtHours(s->svd.ef_date.gmt_off) << "The date and time after which the volume may be used. If not specified, then the volume may be used immediately (0x0360-0x0370)";
            proplist << "File Structure Version" << QString::fromUtf8(reinterpret_cast<char*>(s->svd.fs_ver)) << "The directory records and path table version, always 0x01 (0x0371-0x0371)";
            proplist << "Unused" << "Unused" << "Unused, always 0x00 (0x0372-0x0372)";
            snprintf(asc, 512, "%s", s->svd.app_use);
            proplist << "Application Used" << QString::fromStdString(string(asc)) << "Application Used. Contents not defined by ISO9660 (0x0373-0x0572)";
            proplist << "Reserved by ISO" << "Reserved" << "Reserved by ISO (0x0573-0x07FF)";
        }
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_YAFFS2) // YAFFS2 file system info
    {
        yfs = (YAFFSFS_INFO*)curfsinfo;
        unsigned int objcnt;
        uint32_t objfirst, objlast, vercnt, verfirst, verlast;
        proplist << "Page Size" << QString::number(yfs->page_size) << "Page Size";
        proplist << "Spare Size" << QString::number(yfs->spare_size) << "Spare Size";
        proplist << "Spare Offset Sequence Number" << QString::number(yfs->spare_seq_offset) << "Spare Offset Sequence Number";
        proplist << "Spare Offset Object ID" << QString::number(yfs->spare_obj_id_offset) << "Spare offset object id";
        proplist << "Spare Offset Chunk ID" << QString::number(yfs->spare_chunk_id_offset) << "Spare offset chunk id";
        proplist << "Spare Offset Number of Bytes" << QString::number(yfs->spare_nbytes_offset) << "Spare offset number of bytes";
        yaffscache_objects_stats(yfs, &objcnt, &objfirst, &objlast, &vercnt, &verfirst, &verlast);
        proplist << "Number of Allocated Objects" << QString::number(objcnt) << "Number of Allocated Objects";
        proplist << "Object ID Range" << QString::number(objfirst) + " - " + QString::number(objlast) << "Object id range";
        proplist << "Number of Total Object Versions" << QString::number(vercnt) << "Number of total object versions";
        proplist << "Object Version Range" << QString::number(verfirst) + " - " + QString::number(verlast) << "Object version range";
    }
    else if(curfsinfo->ftype == TSK_FS_TYPE_HFS) // hfs file system info (last of the tsk defined ones)
    {
        hfs = (HFS_INFO*)curfsinfo;
        hsb = hfs->fs;
        time_t mactime;
        sprintf(asc, "0x%X%X %c%c", hsb->signature[0], hsb->signature[1], hsb->signature[0], hsb->signature[1]);
        proplist << "Signature" << QString::fromStdString(string(asc)) << "Signature value 0x4244 for \"BD\", 0x482B for \"H+\" and 0x4858 for \"HX\" (0x00-0x01)";
        proplist << "Version" << QString::number(tsk_getu16(curfsinfo->endian, hsb->version)) << "Version number. 4 for HFS+, 5 for HFSX (0x02-0x03)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_UNMOUNTED)
            proplist << "Volume was Unmounted Properly";
        else
            proplist << "Volume was Unmounted Improperly";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 8 determines if the volume was unmounted properly (0x04-0x07)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_BADBLOCKS)
            proplist << "Volume has Bad Blocks";
        else
            proplist << "Volume does not have Bad Blocks";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 9 determines if there are bad blocks for the volume (0x04-0x07)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_NOCACHE)
            proplist << "Volume is not Cached";
        else
            proplist << "Volume is Cached";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 10 determines if the volume should not be cached (0x04-0x07)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_INCONSISTENT)
            proplist << "Volume is Inconsistent";
        else
            proplist << "Volume is Consistent";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 11 determines if the volume was unmounted properly (0x04-0x07)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_CNIDS_REUSED)
            proplist << "CNIDs are Being Reused";
        else
            proplist << "CNIDs are not Being Reused";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 12 determines if the CNIDs have wrapped around past the maximum value and are being reused (0x04-0x07)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_JOURNALED)
            proplist << "Journaling Enabled";
        else
            proplist << "Journaling Disabled";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 13 determines if journaling is enabled for the volume (0x04-0x07)";
        proplist << "Volume Attributes";
        if(tsk_getu32(curfsinfo->endian, hsb->attr) & HFS_VH_ATTR_SOFTWARE_LOCK)
            proplist << "Volume should be Write-Protected in Software";
        else
            proplist << "Volume is not Write-Protected in Software";
        proplist << "Attribute values, bits 0-7,14,16-31 are reserved. Bit 15 determines if the volume should be write-protected (0x04-0x07)";
        sprintf(asc, "0x%X%X%X%X %c%c%c%c", hsb->last_mnt_ver[0], hsb->last_mnt_ver[1], hsb->last_mnt_ver[2], hsb->last_mnt_ver[3], hsb->last_mnt_ver[0], hsb->last_mnt_ver[1], hsb->last_mnt_ver[2], hsb->last_mnt_ver[3]);
        proplist << "Last Mounted By" << QString::fromStdString(string(asc)) << "\"10.0\" for Mac OS X, \"HFSJ\" for journaled HFS+ on Mac OS X, \"FSK!\" for failed journal replay, \"fsck\" for fsck_hfs and \"8.10\" for Mac OS 8.1-9.2.2 (0x08-0x0B)";
        proplist << "Journal Info Block" << QString::number(tsk_getu32(curfsinfo->endian, hsb->jinfo_blk)) << "Journal information block (0x0C-0x0F)";
        mactime = hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->cr_date));
        sprintf(asc, "%s", tsk_fs_time_to_str(mktime(gmtime(&mactime)), timebuf));
        proplist << "Volume Creation TimeStamp" << QString::fromStdString(string(asc)) << "MACTIME32 converted to UTC from Local Time (0x10-0x13)";
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->m_date)), timebuf));
        proplist << "Volume Last Modified TimeStamp" << QString::fromStdString(string(asc)) << "MACTIME32 converted to UTC (0x14-0x17)";
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->bkup_date)), timebuf));
        proplist << "Volume Backup TimeStamp" << QString::fromStdString(string(asc)) << "MACTIME32 converted to UTC (0x18-0x1B)";
        sprintf(asc, "%s", tsk_fs_time_to_str(hfs_convert_2_unix_time(tsk_getu32(curfsinfo->endian, hsb->chk_date)), timebuf));
        proplist << "Volume Checked TimeStamp" << QString::fromStdString(string(asc)) << "MACTIME32 converted to UTC (0x1C-0x1F)";
        proplist << "File Count" << QString::number(tsk_getu32(curfsinfo->endian, hsb->file_cnt)) << "Number of files on volume, not including special files (0x20-0x23)";
        proplist << "Folder Count" << QString::number(tsk_getu32(curfsinfo->endian, hsb->fldr_cnt)) << "Number of folders on volume, not including the root directory (0x24-0x27)";
        proplist << "Allocation Block Size" << QString::number(tsk_getu32(curfsinfo->endian, hsb->blk_sz)) << "Allocation block size in bytes (0x28-0x2B)";
        proplist << "Allocation Block Total" << QString::number(tsk_getu32(curfsinfo->endian, hsb->blk_cnt)) << "Number of blocks on disk (0x2C-0x2F)";
        proplist << "Allocation Block Free" << QString::number(tsk_getu32(curfsinfo->endian, hsb->free_blks)) << "Unused block count (0x30-0x33)";
        proplist << "Next Allocation" << QString::number(tsk_getu32(curfsinfo->endian, hsb->next_alloc)) << "Block address to start the allocation search (0x34-0x37)";
        proplist << "Resource Clump Size" << QString::number(tsk_getu32(curfsinfo->endian, hsb->rsrc_clmp_sz)) << "Default clump size for resource forks in bytes (0x38-0x3B)";
        proplist << "Data Clump Size" << QString::number(tsk_getu32(curfsinfo->endian, hsb->data_clmp_sz)) << "Default clump size for data forks in bytes (0x3C-0x3F)";
        proplist << "Next Catalog ID" << QString::number(tsk_getu32(curfsinfo->endian, hsb->next_cat_id)) << "Next catalog node id for allocation (0x40-0x43)";
        proplist << "Write Count" << QString::number(tsk_getu32(curfsinfo->endian, hsb->write_cnt)) << "Write count incremented each time it is mounted and modified (0x44-0x47)";
        proplist << "Encoding Bitmap" << QString::number(tsk_getu64(curfsinfo->endian, hsb->enc_bmp)) << "Encoding bitmap identifies which encodings were used in the file system (0x48-0x4F)";
        proplist << "Bootable Folder ID" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOT])) << "Bootable folder id (0x50-0x53)";
        proplist << "Startup App ID" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_START])) << "Startup app id (0x54-0x57)";
        proplist << "Startup Open Folder ID" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_OPEN])) << "Startup open folder id (0x58-0x5B)";
        proplist << "Mac OS 8/9 Blessed System Folder ID" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOT9])) << "Mac OS 8/9 blessed system folder id (0x5C-0x5F)";
        proplist << "Reserved" << "Reserved" << "Reserved (0x60-0x63)";
        proplist << "Mac OS X Blessed System Folder ID" << QString::number(tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_BOOTX])) << "Mac OS X blessed system folder id (0x64-0x67)";
        sprintf(asc, "%08" PRIx32 "%08" PRIx32, tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_ID1]), tsk_getu32(curfsinfo->endian, hsb->finder_info[HFS_VH_FI_ID2]));
        proplist << "Volume Identifier" << QString::fromStdString(string(asc)) << "Volume identifier (0x068-0x6F)";
        proplist << "Fork Data Allocation File" << "" << "Location and size of allocation bitmap files (0x70-0xBF)";
        proplist << "Fork Data Extents File" << "" << "Location and size of extents file (0x00C0-0x010F)";
        proplist << "Fork Data Catalog File" << "" << "Location and size of catalog file (0x0110-0x015F)";
        proplist << "Fork Data Attributes File" << "" << "Location and size of attributes file (0x0160-0x01AF)";
        proplist << "Fork Data Startup File" << "" << "Location and size of startup file (0x01B0-0x01FF)";
    }
    return proplist;
}

QString WombatProperties::ConvertGmtHours(int gmtvar)
{
    int tmpmin = gmtvar - 48;
    int gmthr = tmpmin / 4;
    int gmtmin = (tmpmin % 4)*15;
    QString tmpstring = " GMT ";
    if(abs(gmthr) < 10)
        tmpstring += "0";
    tmpstring += QString::number(gmthr) + QString::number(gmtmin);
    if(gmtmin == 0)
        tmpstring += "0";

    return tmpstring;

}

void WombatProperties::yaffscache_objects_stats(YAFFSFS_INFO* yfs, unsigned int* objcnt, uint32_t* objfirst, uint32_t* objlast, uint32_t* vercnt, uint32_t* verfirst, uint32_t* verlast)
{
    YaffsCacheObject* obj;
    YaffsCacheVersion* ver;

    *objcnt = 2;
    *objfirst = 0xffffffff;
    *objlast = 0;
    *vercnt = 0;
    *verfirst = 0xffffffff;
    *verlast = 0;

    for(obj = yfs->cache_objects; obj != NULL; obj = obj->yco_next)
    {
        objcnt += 1;
        if(obj->yco_obj_id < *objfirst)
            *objfirst = obj->yco_obj_id;
        if(obj->yco_obj_id > *objlast)
            *objlast = obj->yco_obj_id;
        for(ver = obj->yco_latest; ver != NULL; ver = ver->ycv_prior)
        {
            vercnt += 1;
            if(ver->ycv_seq_number < *verfirst)
                *verfirst = ver->ycv_seq_number;
            if(ver->ycv_seq_number > *verlast)
                *verlast = ver->ycv_seq_number;
        }
    }
}

uint32_t WombatProperties::hfs_convert_2_unix_time(uint32_t hfsdate)
{
    if(hfsdate < NSEC_BTWN_1904_1970)
        return 0;
    return (uint32_t)(hfsdate - NSEC_BTWN_1904_1970);
}

QStringList WombatProperties::PopulateFileProperties()
{
    proplist.clear();
    return QStringList("");
}
