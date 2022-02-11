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
    /*
    qDebug() << "cluster size:" << clustersize;
    qDebug() << "catalogs logical size:" << catalogsize;
    qDebug() << "catalogtotalblocks:" << catalogtotalblocks;
    qDebug() << "catalogextstartblockarray:" << catalogextstartblockarray;
    qDebug() << "catalogextblockcntarray:" << catalogextblockcntarray;
    qDebug() << "start block cnt:" << catalogextstartblockarray.split(",", Qt::SkipEmptyParts).count();
    */

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
            // GET THE NUMBER OF RECORDS
            uint16_t recordcount = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2));
            qDebug() << "recordcount:" << recordcount; // MIGHT BE ANOTHER WAY TO LOOP OVER THE RECORDS, RATHER THAN THE CUROFFSET + CURPOS METHOD...
            //qDebug() << curimg->ReadContent(curoffset, 14).toHex();
            //curoffset += 14; // THIS PUTS THE CUROFFSET INTO THE KEY/DATA LOOP FOR THE CATALOG LEAF NODE, SO ITERATING SHOULD WORK HERE
            curpos += 14;
        }
        // THIS IS WHERE THE KEY/DATA LOOP ITERATION SHOULD START... WHILE (CURPOS < CURBLOCKSIZE)
        qDebug() << "curoffset:" << curoffset << "curpos:" << curpos << "curblocksize:" << curblocksize;
        while(curpos < curblocksize)
        {
            uint16_t curkeylength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            if(curkeylength == 0)
            {
                qDebug() << "keylength is wrong...";
            }
            qDebug() << "curkeylength:" << curkeylength; // in case i want to skip a record
            uint32_t parentcnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos, 4));
            qDebug() << "parentcnid:" << parentcnid;
            curpos += 4;
            uint16_t keynamelength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            qDebug() << "keynamelength:" << keynamelength;
            if(keynamelength > 0)
            {
                QString keyfilename = "";
                for(int j=0; j < keynamelength; j++)
                {
                    keyfilename += QString(QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + j*2, 2))));
                }
                curpos += 2*keynamelength;
                qDebug() << "keyfilename:" << keyfilename;
            }
            //else // ALREADY SKIPPED KEYNAME LENGTH...
            //    curpos += 2;
            uint16_t datarecordtype = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            qDebug() << "datarecordtype:" << datarecordtype;
            if(datarecordtype == 0x01) // FOLDER RECORD 0x0001 - 88 bytes
            {
                qDebug() << "folder record";
                curpos += 86;
            }
            else if(datarecordtype == 0x02) // FILE RECORD 0x0002 - 248 bytes
            {
                qDebug() << "file record";
                curpos += 246;
            }
            else if(datarecordtype == 0x03 || datarecordtype == 0x04) // FOLDER THREAD RECORD 0x03/ FILE THREAD RECORD 0x04 - SKIP
            {
                // keylength should be 6 bytes...
                // data length should another 6 bytes
                curpos += 6;
                uint16_t datanodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                curpos += 2;
                qDebug() << "datanodesize:" << datanodesize;
                curpos += 2*datanodesize;
                //break;
            }
            else // SOMETHING IS WRONG PARSING CATALOG FILE
            {
                qDebug() << "error parsing catalog file!";
                break;
            }
        }
    }
}
