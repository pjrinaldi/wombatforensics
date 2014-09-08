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
}

QString WombatProperties::GetFileSystemLabel()
{
}

QStringList WombatProperties::PopulateEvidenceImageProperties()
{
    propertylist.clear();
    propertylist << QString("File Format") << QString(tsk_img_type_todesc((TSK_IMG_TYPE_ENUM)wombatptr->evidenceobject.imageinfo->itype)) << QString("File Format the evidence data is stored in. Usually it is either a raw image (.dd/.001) or an embedded image (.E01/.AFF). A raw image contains only the data from the evidence. The embedded image contains other descriptive information from the acquisition.");
    propertylist << QString("Sector Size") << QString(QString::number(wombatptr->evidenceobject.imageinfo->sector_size) + " bytes") << QString("Sector size of the device. A Sector is a subdivision of a disk where data is stored. It is the smallest value used to divide the disk.");
    propertylist << QString("Sector Count") << QString(QString::number((int)((float)wombatptr->evidenceobject.imageinfo->size/(float)wombatptr->evidenceobject.imageinfo->sector_size)) + " sectors") << QString("The number of sectors in the disk.");
    propertylist << QString("Image Path") << QString::fromStdString(wombatptr->evidenceobject.fullpathvector[0]) << QString("Location where the evidence image is stored and read from.");
    if(TSK_IMG_TYPE_ISAFF(wombatptr->evidenceobject.imageinfo->itype)) // its AFF
    {
        affinfo = (IMG_AFF_INFO*)wombatptr->evidenceobject.imageinfo;
        propertylist << "MD5" << QString::fromStdString(GetSegmentValue(affinfo, AF_MD5)) << "The MD5 hash algorithm of the uncompressed image file, stored as a 128-bit value";
        propertylist << "Image GID" << QString::fromStdString(GetSegmentValue(affinfo, AF_IMAGE_GID)) << "A unique global identifier for the image";
        propertylist << "Device Model" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_MODEL)) << "Acquired Drive Model number";
        propertylist << "Creator" << QString::fromStdString(GetSegmentValue(affinfo, AF_CREATOR)) << "Examiner who initiated the image acquisition";
        propertylist << "Case Number" << QString::fromStdString(GetSegmentValue(affinfo, AF_CASE_NUM)) << "The case number that the image is associated with";
        propertylist << "SHA1" << QString::fromStdString(GetSegmentValue(affinfo, AF_SHA1)) << "The SHA1 hash algorithm of the uncompressed image file, stored as a 160-bit value";
        propertylist << "Acquisition Date" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_DATE)) << "The date the acquisition was made";
        propertylist << "Acquisition Notes" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_NOTES)) << "Notes regading the acquisition";
        propertylist << "Acquisition Device" << QString::fromStdString(GetSegmentValue(affinfo, AF_ACQUISITION_DEVICE)) << "The device used to acquire the information";
        propertylist << "AFFLib Version" << QString::fromStdString(GetSegmentValue(affinfo, AF_AFFLIB_VERSION)) << "Verion of the AFFLib Library used";
        propertylist << "Device Manufacturer" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_MANUFACTURER)) << "Maker of the drive";
        propertylist << "Device Serial Number" << QString::fromStdString(GetSegmentValue(affinfo, AF_DEVICE_SN)) << "Serial number of the drive";
    }
    else if(TSK_IMG_TYPE_ISEWF(wombatptr->evidenceobject.imageinfo->itype)) // its EWF
    {
        ewfinfo = (IMG_EWF_INFO*)wombatptr->evidenceobject.imageinfo;
        if(libewf_handle_get_utf8_header_value_case_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Case Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The case number the image is associated";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_description(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Description" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Description of the acquisition and or evidence item";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_examiner_name(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Examiner Name" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Name of the examiner who acquired the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_evidence_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Evidence Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Unique number identifying the evidence item";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_notes(ewfinfo->handle, ewfvalue, 255, &ewferror) == 1)
            propertylist << "Notes" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Any notes related to the acquisition";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Acquisition Date" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Date the acquisition was made";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_system_date(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "System Date" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Date for the system acquiring the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_operating_system(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Aquisition OS" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Operating System acquiring the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_acquiry_software_version(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Software Version Used" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Version of the software used to acquire the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_password(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Password" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Password to protect the image";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_model(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Model" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Model of the drive acquired";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_header_value_serial_number(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "Serial Number" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "Serial number of the drive acquired";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_sectors_per_chunk(ewfinfo->handle, &value32bit, &ewferror) == 1)
            propertylist << QString("Sectors Per Chunk") << QString::number(value32bit) << "Number of sectors in a image evidence chunk";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_format(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            propertylist << QString("File Format");
            switch(uvalue8bit)
            {
                case LIBEWF_FORMAT_EWF:
                    propertylist << QString("Original EWF") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_SMART:
                    propertylist << QString("SMART") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_FTK:
                    propertylist << QString("FTK Imager") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE1:
                    propertylist << QString("EnCase 1") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE2:
                    propertylist << QString("EnCase 2") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE3:
                    propertylist << QString("EnCase 3") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE4:
                    propertylist << QString("EnCase 4") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE5:
                    propertylist << QString("EnCase 5") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_ENCASE6:
                    propertylist << QString("EnCase 6") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_LINEN5:
                    propertylist << QString("Linen 5") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_LINEN6:
                    propertylist << QString("Linen 6") << "Format used to store the evidence image";
                    break;
                case LIBEWF_FORMAT_EWFX:
                    propertylist << QString("EWFX (extended ewf)") << QString("Extended EWF Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE5:
                    propertylist << QString("LEF EnCase 5") << QString("Logical Evidence File EnCase 5 Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_LOGICAL_ENCASE6:
                    propertylist << QString("LEF EnCase 6") << QString("Logical Evidence File EnCase 6 Format used to store the evidence image");
                    break;
                case LIBEWF_FORMAT_UNKNOWN:
                    propertylist << QString("Unknown Format") << "Format used to store the evidence image";
                    break;
            }
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_error_granularity(ewfinfo->handle, &value32bit, &ewferror) == 1)
            propertylist << QString("Error Granularity") << QString::number(value32bit) << "Error block size";
        else
            libewf_error_fprint(ewferror, stdout);
        propertylist << "Compression Method" << "Deflate" << "Method used to compress the image";
        if(libewf_handle_get_compression_values(ewfinfo->handle, &value8bit, &uvalue8bit, &ewferror) == 1)
        {
            propertylist << "Compression Level";
            if(value8bit == LIBEWF_COMPRESSION_NONE)
                propertylist << "No Compression";
            else if(value8bit == LIBEWF_COMPRESSION_FAST)
                propertylist << "Good (Fast) Compression";
            else if(value8bit == LIBEWF_COMPRESSION_BEST)
                propertylist << "Best Compression";
            else
                propertylist << "Unknown Compression";
            propertylist << "The more compression, the slower the acquisition but the smaller the file size";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_type(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            propertylist << "Media Type";
            if(uvalue8bit == LIBEWF_MEDIA_TYPE_REMOVABLE)
                propertylist << "Removable Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_FIXED)
                propertylist << "Fixed Disk";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_SINGLE_FILES)
                propertylist << "Single Files";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_OPTICAL)
                propertylist << "Optical Disk (CD/DVD/BD)";
            else if(uvalue8bit == LIBEWF_MEDIA_TYPE_MEMORY)
                propertylist << "Memory (RAM)";
            else
                propertylist << "Unknown";
            propertylist << "Media type of the original evidence";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_flags(ewfinfo->handle, &uvalue8bit, &ewferror) == 1)
        {
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_PHYSICAL)
                propertylist << "Media Flag" << "Physical" << "Directly connected disk";
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_FASTBLOC)
                propertylist << "Media Flag" << "Fastbloc Write Blocked" << "Write blocked disk using Fastbloc";
            if(uvalue8bit == LIBEWF_MEDIA_FLAG_TABLEAU)
                propertylist << "Media Flag" << "Tableau Write Blocked" << "Write blocked disk using Tableau";
        }
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_bytes_per_sector(ewfinfo->handle, &value32bit, &ewferror) == 1)
            propertylist << "Bytes Per Sector" << QString::number(value32bit) << "Number of bytes in a sector";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_number_of_sectors(ewfinfo->handle, &value64bit, &ewferror) == 1)
            propertylist << "Number of Sectors" << QString::number(value64bit) << "Number of total sectors in the original media";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_chunk_size(ewfinfo->handle, &value32bit, &ewferror) == 1)
            propertylist << "Chunk Size" << QString::number(value32bit) << "The size of an image chunk";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_media_size(ewfinfo->handle, &size64bit, &ewferror) == 1)
            propertylist << "Media Size" << QString::number(size64bit) << "The size of the media";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_md5(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "MD5" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The MD5 hash algorithm of the uncompressed image stored as a 128-bit value";
        else
            libewf_error_fprint(ewferror, stdout);
        if(libewf_handle_get_utf8_hash_value_sha1(ewfinfo->handle, ewfvalue, 64, &ewferror) == 1)
            propertylist << "SHA1" << QString::fromUtf8(reinterpret_cast<char*>(ewfvalue)) << "The SHA1 hash algorithm of the uncompressed image stored as a 160-bit value";
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
    return propertylist;
}

QStringList WombatProperties::PopulatePartitionProperties()
{
}

QStringList WombatProperties::PopulateFileSystemProperties()
{
}

QStringList WombatProperties::PopulateFileProperties()
{
}
