#ifndef PROPERTYTEMPLATES_H
#define PROPERTYTEMPLATES_H

#include <string>
#include <map>

std::map<std::string, std::string> propertytemplates{{"forimg.pt", ""}, {"qcowimg.pt", "Format Version|Version number for the qcow format.;Media Size|Size of the qcow image in bytes.;Encryption Method|Encryption method used.;Number of Snapshots|Number of snapshots taken in the qcow image."}, {"vhdimg.pt", "File Type|File type.;Format Version|Format number.;Disk Type|Type of disk.;Media Size|Size of the media in bytes.;Bytes per Sector|Number of bytes per sector.;Identifier|16-byte GUID."}, {"vmdkimg.pt", "Disk Type|Disk type.;Media Size|Size of the media in bytes.;Content Identifier|Content indentifier.;Parent Content Identifier|Indentifier for content's parent.;Number of Extents|number of extents in the image.;"}, {"ewfimg.pt", "Case Number|Unique case identifier.;Description|description regarding the creation of the forensic image.;Examiner|name of the forensic examiner who created the forensic image.;Evidence Number|unique number assigned to the evidence.;Notes|Examiner notes.;Acquisition Date|Date of acquisition.;System Date|date of system used to create image.;Operating System Used|Operating system used to create the forensic image.;Software Version Used|Version of the software used.;File Format|EWF file format.;Sectors per chunk|Number of sectors per chunk.;Compression Method|Method of compression used.;Compression Level|level of compression applied.;Media Type|Type of media imaged.;Bytes Per Sector|Number of bytes per sector.;Sector Count|Number of sectors on disk.;Media Size|Size of the media.;Hash Information|Hash value used to validate the forensic image."}, {"fat12.pt", "Reserved Area Size|Size of the reserved area at the beginning of the file system.;Root Directory Max Files|Maximum number of root directory entries.;FAT Count|Number of FAT's in the file system.;Bytes Per Sector|Number of bytes per sector, usually 512.;Sectors Per Cluster|Number of sectors per cluster.;File System Sector Count|Total sectors in the volume.;FAT Size|Size of the FAT.;Volume Serial Number|Serial number for the volume.;Volume Label|Label for the file system volume.;FAT Offset|Byte offset to the start of the first FAT.;Root Directory Offset|Byte offset for the root directory.;Root Directory Sectors|Number of sectors for the root directory.;Root Directory Size|Size in bytes for the root directory.;Cluster Area Start|Byte offset to the start of the cluster area.;Root Directory Layout|Layout for the root directory."}, {"fat12file.pt", "Long Name|Long name for the file.;File Attributes|File attributes.;Alias Name|8.3 file name.;Layout|File offset,size layout in bytes.;Physical Size|Sector Size in Bytes for the file.;Logical Size|Size in Bytes for the file."}, {"fat16.pt", "Reserved Area Size|Size of the reserved area at the beginning of the file system.;Root Directory Max Files|Maximum number of root directory entries.;FAT Count|Number of FAT's in the file system.;Bytes Per Sector|Number of bytes per sector, usually 512.;Sectors Per Cluster|Number of sectors per cluster.;File System Sector Count|Total sectors in the volume.;FAT Size|Size of the FAT.;Volume Serial Number|Serial number for the volume.;Volume Label|Label for the file system volume.;FAT Offset|Byte offset to the start of the first FAT.;Root Directory Offset|Byte offset for the root directory.;Root Directory Sectors|Number of sectors for the root directory.;Root Directory Size|Size in bytes for the root directory.;Cluster Area Start|Byte offset to the start of the cluster area.;Root Directory Layout|Layout for the root directory."}, {"fat16file.pt", "Long Name|Long name for the file.;File Attributes|File attributes.;Alias Name|8.3 file name.;Layout|File offset,size layout in bytes.;Physical Size|Sector Size in Bytes for the file.;Logical Size|Size in Bytes for the file."}};

#endif // PROPERTYTEMPLATES_H
