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
    quint64 curoffset = 0;
    quint64 curblocksize = 0;
    curoffset = curstartsector*512;
    QStringList catalogblocklist = catalogextstartblockarray.split(",", Qt::SkipEmptyParts);
    QStringList catalogblockcntlist = catalogextblockcntarray.split(",", Qt::SkipEmptyParts);
    // catalog file can be split, so i need to parse the 
    for(int i=0; i < catalogblocklist.count(); i++)
    {
        curblocksize = catalogblockcntlist.at(i).toUInt() * clustersize;
        qDebug() << "curblocksize:" << curblocksize;
        uint32_t curpos = 0;
        // parse the catalog file by pieces here.
        if(i == 0) // header node
        {
            curoffset += catalogblocklist.at(i).toUInt() * clustersize; // curoffset at the first catalog block offset
            nodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 32, 2));
            qDebug() << "nodesize:" << nodesize;
            // NEED TO JUMP TO CATALOG ROOT DIRECTORY NODE START, WHICH IS CATALOG NODE ID (CNID) 2, SO IT IS (CNID - 1) * NODESIZE
            // SO FOR THIS EXAMPLE, IT IS (2-1) * 4096, NEED TO SET THE OFFSET TO BE 
            curoffset += nodesize; // SHOULD PUT ME AT 4096, START OF THE CATALOG ROOT DIRECTORY NODE
            // READING THE NODE DESCRIPTOR IS ONLY IN THE i=0 PORTION OF THE CATALOG ROOT DIRECTORY
            qDebug() << curimg->ReadContent(curoffset, 14).toHex();
            cufoffset += 14; // THIS PUTS THE CUROFFSET INTO THE KEY/DATA LOOP FOR THE CATALOG LEAF NODE, SO ITERATING SHOULD WORK HERE
        }
        // THIS IS WHERE THE KEY/DATA LOOP ITERATION SHOULD START... WHILE (CURPOS < CURBLOCKSIZE
    }
}
