#ifndef PROPERTIES_H
#define PROPERTIES_H

void WriteEvidenceProperties(TSK_IMG_INFO* curimginfo, QString evidencepath, QString imgfullpath);
void WriteVolumeProperties(TSK_VS_INFO* curvolinfo, QString volumepath);
void WriteFileSystemProperties(TSK_FS_INFO* curfsinfo, QString partitionpath);
void WriteFileProperties(TSK_FS_FILE* curfileinfo, QString partpath);
void WriteAlternateDataStreamProperties(TSK_FS_FILE* curfileinfo, QString adsname, QString attrid, QString partpath, bool isres);

#endif // properties.h
