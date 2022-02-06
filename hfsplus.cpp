#include "hfsplus.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

qulonglong ParseHfsPlusDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, qulonglong parinode, QString parfilename)
{
    quint64 inodecnt = 0;
    uint32_t clustersize = 0;
    quint64 catalogsize = 0;
    uint32_t catalogtotalblocks = 0;
    QString catalogextstartblockarray = "";
    QString catalogextblockcntarray = "";
    // let's figure out how to parse the root directory...
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Cluster Size|"))
                clustersize = line.split("|").at(1).toUInt();
            if(line.startsWith("Catalog Logical Size|"))
                catalogsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Catalog Total Blocks|"))
                catalogtotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Catalog Extents Start Block Array|"))
                catalogextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Catalog Extents Block Count Array|"))
                catalogextblockcntarray = line.split("|").at(1);
        }
        propfile.close();
    }
    qDebug() << "cluster size:" << clustersize;
    qDebug() << "catalogs logical size:" << catalogsize;
    qDebug() << "catalogtotalblocks:" << catalogtotalblocks;
    qDebug() << "catalogextstartblockarray:" << catalogextstartblockarray;
    qDebug() << "catalogextblockcntarray:" << catalogextblockcntarray;
    qDebug() << "start block cnt:" << catalogextstartblockarray.split(",", Qt::SkipEmptyParts).count();

    uint16_t nodesize = 0;
    // catalog file can be split, so i need to parse the 
    for(int i=0; i < catalogextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++)
    {
        // parse the catalog file by pieces here.
        if(i == 0) // header node
        {
            nodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + catalogextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt() * clustersize + 32, 2));
            qDebug() << "nodesize:" << nodesize;
            // need to clean this up a little and put the split in a variable earlier
        }
    }
}
