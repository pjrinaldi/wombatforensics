#ifndef PROPERTYTEMPLATES_H
#define PROPERTYTEMPLATES_H

#include <string>
#include <map>

std::map<std::string, std::string> propertytemplates{{"forimg.pt", ""}, {"qcowimg.pt", "Format Version|Version number for the qcow format.;Media Size|Size of the qcow image in bytes.;Encryption Method|Encryption method used.;Number of Snapshots|Number of snapshots taken in the qcow image."}, {"vhdimg.pt", "File Type|File type.;Format Version|Format number.;Disk Type|Type of disk.;Media Size|Size of the media in bytes.;Bytes per Sector|Number of bytes per sector.;Identifier|16-byte GUID."}, {"vmdkimg.pt", "Disk Type|Disk type.;Media Size|Size of the media in bytes.;Content Identifier|Content indentifier.;Parent Content Identifier|Indentifier for content's parent.;Number of Extents|number of extents in the image.;"}, {"ewfimg.pt", "Case Number|Unique case identifier.;Description|description regarding the creation of the forensic image.;Examiner|name of the forensic examiner who created the forensic image.;Evidence Number|unique number assigned to the evidence.;Notes|Examiner notes.;Acquisition Date|Date of acquisition.;System Date|date of system used to create image.;Operating System Used|Operating system used to create the forensic image.;Software Version Used|Version of the software used.;File Format|EWF file format.;Sectors per chunk|Number of sectors per chunk.;Compression Method|Method of compression used.;Compression Level|level of compression applied.;Media Type|Type of media imaged.;Bytes Per Sector|Number of bytes per sector.;Sector Count|Number of sectors on disk.;Media Size|Size of the media.;Hash Information|Hash value used to validate the forensic image."}, {"wfiimg.pt", "Image Size|Size of the image."}, {"sfsimg.pt", "Image Size|Size of the image."}, {"unalloc.pt", "Offset|Volume offset;Size|Volume Size"}, {"fat12.pt", "Reserved Area Size|Size of the reserved area at the beginning of the file system.;Root Directory Max Files|Maximum number of root directory entries.;FAT Count|Number of FAT's in the file system.;Bytes Per Sector|Number of bytes per sector, usually 512.;Sectors Per Cluster|Number of sectors per cluster.;File System Sector Count|Total sectors in the volume.;FAT Size|Size of the FAT.;Volume Serial Number|Serial number for the volume.;Volume Label|Label for the file system volume.;FAT Offset|Byte offset to the start of the first FAT.;Root Directory Offset|Byte offset for the root directory.;Root Directory Sectors|Number of sectors for the root directory.;Root Directory Size|Size in bytes for the root directory.;Cluster Area Start|Byte offset to the start of the cluster area.;Root Directory Layout|Layout for the root directory."}, {"fat12file.pt", "Long Name|Long name for the file.;File Attributes|File attributes.;Alias Name|8.3 file name.;Layout|File offset,size layout in bytes.;Physical Size|Sector Size in Bytes for the file.;Logical Size|Size in Bytes for the file."}, {"fat16.pt", "Reserved Area Size|Size of the reserved area at the beginning of the file system.;Root Directory Max Files|Maximum number of root directory entries.;FAT Count|Number of FAT's in the file system.;Bytes Per Sector|Number of bytes per sector, usually 512.;Sectors Per Cluster|Number of sectors per cluster.;File System Sector Count|Total sectors in the volume.;FAT Size|Size of the FAT.;Volume Serial Number|Serial number for the volume.;Volume Label|Label for the file system volume.;FAT Offset|Byte offset to the start of the first FAT.;Root Directory Offset|Byte offset for the root directory.;Root Directory Sectors|Number of sectors for the root directory.;Root Directory Size|Size in bytes for the root directory.;Cluster Area Start|Byte offset to the start of the cluster area.;Root Directory Layout|Layout for the root directory."}, {"fat16file.pt", "Long Name|Long name for the file.;File Attributes|File attributes.;Alias Name|8.3 file name.;Layout|File offset,size layout in bytes.;Physical Size|Sector Size in Bytes for the file.;Logical Size|Size in Bytes for the file."}, {"fat32.pt", "Reserved Area Size|Size of the reserved area at the beginning of the file system.;FAT Count|Number of FAT's in the file system.;Bytes Per Sector|Number of bytes per sector, usually 512.;Sectors Per Cluster|Number of sectors per cluster.;File System Sector Count|Total sectors in the volume.;FAT Size|Size of the FAT.;Volume Serial Number|Serial number for the volume.;Volume Label|Label for the file system volume.;FAT Offset|Byte offset to the start of the first FAT.;Root Directory Offset|Byte offset for the root directory.;Root Directory Cluster|Cluster offset to the start of the root directory.;Cluster Area Start|Byte offset to the start of the cluster area.;Root Directory Layout|Layout for the root directory."}, {"fat32file.pt", "Long Name|Long name for the file.;File Attributes|File attributes.;Alias Name|8.3 file name.;Layout|File offset,size layout in bytes.;Physical Size|Sector Size in Bytes for the file.;Logical Size|Size in Bytes for the file."}, {"exfat.pt", "Reserved Area Size|Size of the reserved area at the beginning of the file system.;Root Directory Max Files|Maximum number of root directory entries.;FAT Size|Size of the FAT.;Volume Label|Label for the file system volume.;Bytes Per Sector|Number of bytes per sector.;Sector Count|Number of sectors on disk.;FAT Count|Number of FAT's in the file system.;FAT Offset|Byte offset to the start of the first FAT.;Cluster Area Start|Sector offset to the start of the cluster area.;Root Directory Offset|Byte offset for the root directory.;Root Directory Size|Size in bytes for the root directory.;Root Directory Layout|Layout for the root directory."}, {"exfatfile.pt", "File Attributes|File attributes.;Layout|File offset,size layout in bytes.;Physical Size|Sector Size in Bytes for the file.;Logical Size|Size in Bytes for the file."}, {"extfs.pt", "Volume Label|Volume Label for the file system.;Created Time|Creation time for the file system.;Mount Time|Mount time for the file system.;Write Time|Write time for the file system.;Last Check Time|Last check time for the file system.;Current State|Condition of the file system at lsat unmount.;Compatible Features|File system compatible feature set.;Incompatible Features|File system incompatible Feature set.;Read Only Compatible Features|File system read only compatible feature set.;File System Type|File system type string.;Block Group Descriptor Size|Size in bytes of the block group descriptor table entry.;File System Inode Count|Number of inodes within the file system.;File System Block Count|Number of blocks within the file system.;Block Group 0 Start Block|Starting block number for block group 0.;Block Size|Block size in bytes.;Fragment Size|Fragment size in bytes.;Block Group Block Count|Number of blocks within a block group.;Block Group Fragment Count|Number of fragments within a block group.;Block Group Inode Count|Number of inodes within a block group.;Creator OS|Operating System used to create the file system.;Inode Size|Size of an inode in bytes.;Last Mounted Path|Path where file system was last mounted.;Inode Address Table|Table of the Inode addresses for a block group.;Root Inode Table Address|Starting address for the Root Directory Inode Table.;Revision Level|File system revision level.;"}, {"extfsfile.pt", "EXTFS Inode|EXTFS inode value to locate file in the filesystem.;Mode|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool, r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx].;uid / gid|User ID and Group ID.;Deleted Time|Deleted time for the file.;Link Count|Number of files pointing to this file.;File Attributes|Attributes list for the file.;Layout|File layout in offset,size format.;Physical Size|Size of the blocks the file takes up in bytes.;Logical Size|Size in Bytes for the file.;"}, {"ntfs.pt", "Bytes Per Sector|Number of bytes per sector, usually 512.;Sectors Per Cluster|Number of sectors per cluster.;Total Sectors|Number of sectors in the file system.;Bytes Per Cluster|Number of bytes per cluster.;MFT Starting Cluster|Starting cluster number for the MFT.;MFT Starting Offset|Starting byte for the MFT.;MFT Entry Size|Entry size in clusters for an MFT Entry.;MFT Entry Bytes|Entry size in bytes for an MFT Entry.;Serial Number|Serial number for the file system volume.;Index Record Size|Index record size for an index record.;MFT Layout|Layout for the MFT in starting offset, size format.;Max MFT Entries|Max MFT Entries allowed in the MFT.;Volume Label|Volume Label for the file system.;"}, {"ntfsfile.pt", "NT Inode|NTFS file inode value.;Parent NT Inode|File's parent NTFS inode value.;MFT Entry Layout|Offset and size to the MFT entry record for the file.;MFT Sequence ID|Sequence number for the MFT entry.;$I30 Sequence ID|Sequence number for the file from the $I30 attribute.;$I30 Create Date|File Creation time as recorded in the $I30 entry.;$I30 Modify Date|File Modification time as recorded in the $I30 entry.;$I30 Status Changed Date|File Status Changed time as recorded in the $I30 entry,;$I30 Accessed Date|File Accessed time as recorded in the $I30 entry.;File Attributes|Attributes list for the file.;$FILE_NAME Create|File creation time as recorded in the $FILE_NAME attribute.;$FILE_NAME Modify|File modification time as recorded in the $FILE_NAME attribute.;$FILE_NAME Status Changed|File status changed time as recorded in the $FILE_NAME attribute.;$FILE_NAME Accessed|File accessed time as recorded in the $FILE_NAME attribute.;Physical Size|Physical size in bytes for the file.;Logical Size|Size in Bytes for the file.;Layout|File layout in bytes and formatted as offset,size entries.;Object ID|128-bit Global Object Identifier that can be used to address the file instead of the name.;Security ID|Security identifier for the file/directory."}};

#endif // PROPERTYTEMPLATES_H
