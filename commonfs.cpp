#include "commonfs.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2


QString ConvertBlocksToExtents(QList<uint> blocklist, uint blocksize, qulonglong rootdiroffset)
{
    // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
    //QString rootdirlayout = QString::number(rootdiroffset + ((rootdircluster - 2) * sectorspercluster * bytespersector)) + "," + QString::number(sectorspercluster * bytespersector) + ";";
    QString extentstring = "";
    int blkcnt = 1;
    uint startvalue = blocklist.at(0);
    for(int i=1; i < blocklist.count(); i++)
    {
        uint oldvalue = blocklist.at(i-1);
        uint newvalue = blocklist.at(i);
        if(newvalue - oldvalue == 1)
            blkcnt++;
        else
        {
            if(rootdiroffset > 0)
                extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
            else
                extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
        if(i == blocklist.count() - 1)
        {
            if(rootdiroffset > 0)
                extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
            else
                extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
    }
    if(blocklist.count() == 1)
    {
        if(rootdiroffset > 0)
            extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
        else
            extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
    }

    return extentstring;
}

void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, qulonglong fatoffset, QList<uint>* clusterlist)
{
    uint32_t curcluster = 0;
    int fatbyte1 = 0;
    if(fstype == 1) // FAT12
    {
        fatbyte1 = clusternum + (clusternum / 2);
        if(clusternum & 0x0001) // ODD
            curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) >> 4);
        else // EVEN
            curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) & 0x0FFF);
        //if(curcluster != 0x0FF7)
        //    clusterlist->append(curcluster);
        //if(curcluster < 0x0FF8 && curcluster >= 2)
        if(curcluster < 0x0FF7 && curcluster >= 2)
        {
            clusterlist->append(curcluster);
            GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
        }
    }
    else if(fstype == 2) // FAT16
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 2;
            curcluster = qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2));
            //if(curcluster != 0xFFF7)
            //    clusterlist->append(curcluster);
            //if(curcluster < 0xFFF8 && curcluster >= 2)
            if(curcluster < 0xFFF7 && curcluster >= 2)
            {
                clusterlist->append(curcluster);
                GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if(fstype == 3) // FAT32
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
            curcluster = (qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4)) & 0x0FFFFFFF);
	    //if(curcluster != 0x0FFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster < 0x0FFFFFF7 && curcluster >= 2)
            {
                clusterlist->append(curcluster);
		GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if(fstype == 4) // EXFAT
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
            curcluster = qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4));
	    //if(curcluster != 0xFFFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster < 0xFFFFFFF7 && curcluster >= 2)
            {
                clusterlist->append(curcluster);
		GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
            }
        }
    }
}

