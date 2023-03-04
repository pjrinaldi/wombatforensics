#include "hfsplus.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

qulonglong ParseHfsPlusDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    quint64 inodecnt = 0;
    //quint64 parinode = 0;
    QString layout = "";
    QHash<QString, QVariant> nodedata;
    QFile fileprop;
    QTextStream out;
    QString parentstr = "";
    uint32_t clustersize = 0;
    uint32_t curstartblock = 0;
    uint32_t curblockcount = 0;
    // ALLOCATION FILE
    quint64 allocsize = 0;
    uint32_t alloctotalblocks = 0;
    QString allocextstartblockarray = "";
    QString allocextblockcntarray = "";
    // EXTENTS OVERFLOW FILE
    quint64 extentsize = 0;
    uint32_t extenttotalblocks = 0;
    QString extentextstartblockarray = "";
    QString extentextblockcntarray = "";
    // CATALOG FILE
    quint64 catalogsize = 0;
    uint32_t catalogtotalblocks = 0;
    QString catalogextstartblockarray = "";
    QString catalogextblockcntarray = "";
    // ATTRIBUTES FILE
    quint64 attrsize = 0;
    uint32_t attrtotalblocks = 0;
    QString attrextstartblockarray = "";
    QString attrextblockcntarray = "";
    // STARTUP FILE
    quint64 startsize = 0;
    uint32_t starttotalblocks = 0;
    QString startextstartblockarray = "";
    QString startextblockcntarray = "";
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
            // ALLOCATION FILE INFORMATION
            if(line.startsWith("Allocation Logical Size|"))
                allocsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Allocation Total Blocks|"))
                alloctotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Allocation Extents Start Block Array|"))
                allocextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Allocation Extents Block Count Array|"))
                allocextblockcntarray = line.split("|").at(1);
            // EXTENTS OVERFLOW FILE INFORMATION
            if(line.startsWith("Extents Overflow Logical Size|"))
                extentsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Extents Overflow Total Blocks|"))
                extenttotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Extents Overflow Extents Start Block Array|"))
                extentextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Extents Overflow Extents Block Count Array|"))
                extentextblockcntarray = line.split("|").at(1);
            // CATALOG FILE INFORMATION
            if(line.startsWith("Catalog Logical Size|"))
                catalogsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Catalog Total Blocks|"))
                catalogtotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Catalog Extents Start Block Array|"))
                catalogextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Catalog Extents Block Count Array|"))
                catalogextblockcntarray = line.split("|").at(1);
            // ATTRIBUTES FILE INFORMATION
            if(line.startsWith("Attributes Logical Size|"))
                attrsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Attributes Total Blocks|"))
                attrtotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Attributes Extents Start Block Array|"))
                attrextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Attributes Extents Block Count Array|"))
                attrextblockcntarray = line.split("|").at(1);
            // STARTUP FILE INFORMATION
            if(line.startsWith("Startup Logical Size|"))
                startsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Startup Total Blocks|"))
                starttotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Startup Extents Start Block Array|"))
                startextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Startup Extents Block Count Array|"))
                startextblockcntarray = line.split("|").at(1);
        }
        propfile.close();
    }
    // CREATE THE RESPECTIVE RESERVED FILES HERE
    // ALLOCATION FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|6|Catalog node id for the allocation file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * alloctotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(allocsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    for(int i=0; i < allocextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = allocextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = allocextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$AllocationFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", allocsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Allocation File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // EXTENTS OVERFLOW FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|3|Catalog node id for the extents overflow file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * extenttotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(extentsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < extentextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = extentextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = extentextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$ExtentsOverflowFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", extentsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Extents Overflow File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // CATALOG FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|4|Catalog node id for the catalog file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * catalogtotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(catalogsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < catalogextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = catalogextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = catalogextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$CatalogFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", catalogsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Catalog File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // ATTRIBUTES FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|8|Catalog node id for the attributes file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * attrtotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(attrsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < attrextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = attrextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = attrextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$AttributesFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", attrsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Attributes File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // STARTUP FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|7|Catalog node id for the startup file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * starttotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(startsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < startextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = startextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = startextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$StartupFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", startsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Startup File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // FINISH THE RESPECTIVE RESERVED FILES HERE
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
        //qDebug() << "curblocksize:" << curblocksize;
        uint32_t curpos = 0;
        // parse the catalog file by pieces here.
        if(i == 0) // header node
        {
            curoffset += catalogblocklist.at(i).toUInt() * clustersize; // curoffset at the first catalog block offset
            nodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 32, 2));
            //qDebug() << "nodesize:" << nodesize;
            // NEED TO JUMP TO CATALOG ROOT DIRECTORY NODE START, WHICH IS CATALOG NODE ID (CNID) 2, SO IT IS (CNID - 1) * NODESIZE
            // SO FOR THIS EXAMPLE, IT IS (2-1) * 4096, NEED TO SET THE OFFSET TO BE 
            curoffset += nodesize; // SHOULD PUT ME AT 4096, START OF THE CATALOG ROOT DIRECTORY NODE
            // READING THE NODE DESCRIPTOR IS ONLY IN THE i=0 PORTION OF THE CATALOG ROOT DIRECTORY
            // GET THE NUMBER OF RECORDS
            uint16_t recordcount = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2));
            //qDebug() << "recordcount:" << recordcount; // MIGHT BE ANOTHER WAY TO LOOP OVER THE RECORDS, RATHER THAN THE CUROFFSET + CURPOS METHOD...
            //qDebug() << curimg->ReadContent(curoffset, 14).toHex();
            //curoffset += 14; // THIS PUTS THE CUROFFSET INTO THE KEY/DATA LOOP FOR THE CATALOG LEAF NODE, SO ITERATING SHOULD WORK HERE
            curpos += 14;
        }
        // THIS IS WHERE THE KEY/DATA LOOP ITERATION SHOULD START... WHILE (CURPOS < CURBLOCKSIZE)
        //qDebug() << "curoffset:" << curoffset << "curpos:" << curpos << "curblocksize:" << curblocksize;
        QHash<uint32_t, QString> dirhash; // QHash<parentcnid, QString("inode, filename")>
        dirhash.clear();
        dirhash.insert(2, "0,2,/");
        while(curpos < curblocksize)
        {
            QString keyfilename = "";
            uint16_t curkeylength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            if(curkeylength == 0)
            {
                //qDebug() << "keylength is wrong, probably out of records...";
                break;
            }
            //qDebug() << "curkeylength:" << curkeylength; // in case i want to skip a record
            uint32_t parentcnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos, 4));
            //qDebug() << "parentcnid:" << parentcnid;
            curpos += 4;
            uint16_t keynamelength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            //qDebug() << "keynamelength:" << keynamelength;
            if(keynamelength > 0)
            {
                keyfilename = "";
                for(int j=0; j < keynamelength; j++)
                {
                    QChar tmpchar = QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + j*2, 2)));
                    if(tmpchar == '\x0')
                        keyfilename += QString("^");
                    else
                        keyfilename += QString(tmpchar);
                }
                curpos += 2*keynamelength;
                //qDebug() << "keyfilename:" << keyfilename;
            }
            //else // ALREADY SKIPPED KEYNAME LENGTH...
            //    curpos += 2;
            uint16_t datarecordtype = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            //qDebug() << "datarecordtype:" << datarecordtype;
            if(datarecordtype == 0x0001 || datarecordtype == 0x0002) // FOLDER RECORD 0x0001/FILE RECORD 0x0002 - 88 bytes/248 bytes
            {
                layout = "";
                QString reslayout = "";
                uint8_t itemtype = 0;
                uint8_t isdeleted = 0;
                uint16_t flags = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                uint32_t valence = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 2, 4));
                uint32_t cnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 6, 4));
                //qDebug() << "cnid:" << cnid;
                uint32_t createdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 10, 4));
                uint32_t contentmoddate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 14, 4));
                uint32_t attrmoddate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 18, 4));
                uint32_t accessdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 22, 4));
                uint32_t backupdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 26, 4));
                uint32_t ownerid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 30, 4));
                uint32_t groupid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 34, 4));
                uint8_t adminflags = qFromBigEndian<uint8_t>(curimg->ReadContent(curoffset + curpos + 38, 1));
                uint8_t ownerflags = qFromBigEndian<uint8_t>(curimg->ReadContent(curoffset + curpos + 39, 1));
                uint16_t filemode = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + 40, 2));
                uint32_t inodelinkraw = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 42, 4));
                quint64 logicalsize = 0;
                uint32_t clumpsize = 0;
                uint32_t totalblocks = 0;
                quint64 reslogicalsize = 0;
                uint32_t resclumpsize = 0;
                uint32_t restotalblocks = 0;
                //qDebug() << "createdate:" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(createdate));
                // 16 bytes for user information i currently don't care about
                // 16 bytes for finder information i currently don't care about
                // 4 bytes for text encoding i don't care about
                // 4 bytes reserved, so it doesn't matter
                if(datarecordtype == 0x0001) // FOLDER RECORD
                {
                    //qDebug() << "folder record";
                    dirhash.insert(cnid, QString(QString::number(inodecnt) + "," + QString::number(parentcnid) + "," + keyfilename));
                    curpos += 86;
                }
                else if(datarecordtype == 0x0002) // FILE RECORD
                {
                    logicalsize = qFromBigEndian<quint64>(curimg->ReadContent(curoffset + curpos + 86, 8));
                    clumpsize = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 94, 4));
                    totalblocks = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 98, 4));
                    //qDebug() << "logical size:" << logicalsize << "clumpsize:" << clumpsize << "totalblocks:" << totalblocks;
                    // PARSE DATA FORK
                    if(logicalsize > 0)
                    {
                        for(int j=0; j < 8; j++)
                        {
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 102 + j*8, 4)) > 0)
                                layout += QString::number(curstartsector*512 + clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 102 + j*8, 4))) + ",";
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 106 + j*8, 4)) > 0)
                                layout += QString::number(clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 106 + j*8, 4))) + ";";
                        }
                    }
                    // PARSE RESOURCE FORK
                    reslogicalsize = qFromBigEndian<quint64>(curimg->ReadContent(curoffset + curpos + 166, 8));
                    resclumpsize = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 174, 4));
                    restotalblocks = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 178, 4));
                    if(reslogicalsize > 0)
                    {
                        for(int j=0; j < 8; j++)
                        {
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 182 + j*8, 4)) > 0)
                                reslayout += QString::number(curstartsector*512 + clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 182 + j*8, 4))) + ",";
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 186 + j*8, 4)) > 0)
                                reslayout += QString::number(clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 186 + j*8, 4))) + ";";
                        }
                        //qDebug() << "reslogicalsize:" << reslogicalsize;
                        //qDebug() << "restotalblocks:" << restotalblocks;
                        //qDebug() << "reslayout:" << reslayout;
                        /*

                         */ 
                    }
                    curpos += 246;
                    //qDebug() << "layout:" << layout;
                }
                fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
                if(!fileprop.isOpen())
                    fileprop.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&fileprop);
                out << "Catalog Node ID|" << QString::number(cnid) << "|Catalog node id for the file/folder." << Qt::endl;
                out << "Parent Catalog Node ID|" << QString::number(parentcnid) << "|File's parent catalog node id value." << Qt::endl;
                QString filemodestr = "---------";
                if(filemode & 0xc000) // unix socket
		{
                    filemodestr.replace(0, 1, "s");
		    itemtype = 9;
		}
                if(filemode & 0xa000) // symbolic link
		{
                    filemodestr.replace(0, 1, "l");
		    itemtype = 12;
		}
                if(filemode & 0x6000) // block device
		{
                    filemodestr.replace(0, 1, "b");
		    itemtype = 7;
		}
                if(filemode & 0x2000) // char device
		{
                    filemodestr.replace(0, 1, "c");
		    itemtype = 6;
		}
                if(filemode & 0x1000) // FIFO (pipe)
		{
                    filemodestr.replace(0, 1, "p");
		    itemtype = 8;
		}
                if(filemode & 0x8000) // regular file
                {
                    filemodestr.replace(0, 1, "-");
		    itemtype = 5;
		    if(isdeleted == 1)
			itemtype = 4;
                }
                else if(filemode & 0x4000) // directory
                {
                    filemodestr.replace(0, 1, "d");
		    itemtype = 3;
		    if(isdeleted == 1)
			itemtype = 2;
                }
                if(filemode & 0x100) // user read
                    filemodestr.replace(1, 1, "r");
                if(filemode & 0x080) // user write
                    filemodestr.replace(2, 1, "w");
                if(filemode & 0x040) // user execute
                    filemodestr.replace(3, 1, "x");
                if(filemode & 0x020) // group read
                    filemodestr.replace(4, 1, "r");
                if(filemode & 0x010) // group write
                    filemodestr.replace(5, 1, "w");
                if(filemode & 0x008) // group execute
                    filemodestr.replace(6, 1, "x");
                if(filemode & 0x004) // other read
                    filemodestr.replace(7, 1, "r");
                if(filemode & 0x002) // other write
                    filemodestr.replace(8, 1, "w");
                if(filemode & 0x001) // other execute
                    filemodestr.replace(9, 1, "x");
                //qDebug() << "filemodestr:" << filemodestr;
                out << "Mode|" << filemodestr << "|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
                out << "uid / gid|" << QString::number(ownerid) << " / " << QString::number(groupid) << "|User ID and Group ID." << Qt::endl;
                out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
                out << "Backup Date|" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(backupdate)) << "|Date time the file/folder was last backed up." << Qt::endl;
                //qDebug() << "createdate:" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(createdate));
                out << "Physical Size|" << QString::number(clustersize * totalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
                out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
                out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
                out.flush();
                fileprop.close();
                nodedata.clear();
                nodedata.insert("name", QByteArray(keyfilename.toUtf8()).toBase64());
                // FIX PATH HERE BASED ON PARENTCNID AND DIRHASH
                QString parentinfo = dirhash.value(parentcnid);
                //qDebug() << "parentinfo:" << parentinfo;
                // THE PATH VARIABLE ISN'T QUITE RIGHT... IF ITS NESTED, THIS IGNORES THE NESTING...
                // SO I NEED AN ITERATIVE PATH STRING BUILDER BASED ON CHECKING DIRHASH FOR EACH CNID...
                QString parentpath = "/";
                if(parentcnid > 2)
                    BuildPath(&parentpath, &dirhash, parentcnid);
                    //nodedata.insert("path", QByteArray(QString("/" + parentinfo.split(",").at(1) + "/").toUtf8()).toBase64());
                nodedata.insert("path", QByteArray(parentpath.toUtf8()).toBase64());
                nodedata.insert("size", logicalsize);
                nodedata.insert("create", ConvertHfsTimeToUnixTime(createdate));
                nodedata.insert("access", ConvertHfsTimeToUnixTime(accessdate));
                nodedata.insert("modify", ConvertHfsTimeToUnixTime(contentmoddate));
                nodedata.insert("status", ConvertHfsTimeToUnixTime(attrmoddate));
                if(logicalsize > 0) // Get Category/Signature
                {
                    if(itemtype == 3 && isdeleted == 0)
                    {
                        nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory");
                    }
                    else
                    {
                        QString catsig = GenerateCategorySignature(curimg, keyfilename, layout.split(";").at(0).split(",").at(0).toULongLong());
                        nodedata.insert("cat", catsig.split("/").first());
                        nodedata.insert("sig", catsig.split("/").last());
                    }
                }
                else
                {
                    if(itemtype == 3 && isdeleted == 0)
                    {
                        nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory");
                    }
                    else
                    {
                        nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Zero File");
                    }
                }
                nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
                parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
                // need a list which contains cnid, inode, filename for folders...
                if(cnid != 1 && cnid != 2)
                {
                    QString parinode = parentinfo.split(",").at(0);
                    if(parentcnid > 2)
                    {
                        parentstr += QString("-f" + parinode);
                    }
                    //qDebug() << parentcnid << "parentcnid info:" << dirhash.value(parentcnid);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, parentstr, itemtype, 0); // virtual file, not deleted
                    mutex.unlock();
                    listeditems.append(nodedata.value("id").toString());
                    filesfound++;
                    isignals->ProgUpd();
                    inodecnt++;
                }
                nodedata.clear();
                if(reslogicalsize > 0) // resource fork exists
                {
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt-1));
                    nodedata.insert("name", QByteArray(QString("$RESOURCE_FORK").toUtf8()).toBase64());
                    nodedata.insert("path", QByteArray(QString(parentpath + keyfilename + "/").toUtf8()).toBase64());
                    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
                    nodedata.insert("size", reslogicalsize);
                    nodedata.insert("cat", "System File");
                    nodedata.insert("sig", "Resource Fork");
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, parentstr, 10, 0);
                    mutex.unlock();
                    listeditems.append(nodedata.value("id").toString());
                    filesfound++;
                    isignals->ProgUpd();
                    QFile adsprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
                    QTextStream adsout;
                    if(!adsprop.isOpen())
                        adsprop.open(QIODevice::Append | QIODevice::Text);
                    if(adsprop.isOpen())
                    {
                        adsout.setDevice(&adsprop);
                        adsout << "Physical Size|" << QString::number(clustersize * restotalblocks) << "|Physical size for the resource fork in bytes." << Qt::endl;
                        adsout << "Logical Size|" << QString::number(reslogicalsize) << "|Logical size of the resource fork in bytes." << Qt::endl;
                        adsout << "Layout|" << reslayout + "|Layout of the resource fork in bytes as offset,size;offset,size;." << Qt::endl;
                        adsout.flush();
                        adsprop.close();
                    }
                    inodecnt++;
                }
            }
            else if(datarecordtype == 0x0003 || datarecordtype == 0x0004) // FOLDER THREAD RECORD 0x03/ FILE THREAD RECORD 0x04 - SKIP
            {
                // keylength should be 6 bytes...
                // data length should another 6 bytes
                curpos += 6;
                uint16_t datanodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                curpos += 2;
                //qDebug() << "datanodesize:" << datanodesize;
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

void BuildPath(QString* path, QHash<uint32_t, QString>* dirhash, quint64 curparcnid)
{
    while(curparcnid > 2)
    {
        QString parentinfo = dirhash->value(curparcnid);
        curparcnid = parentinfo.split(",").at(1).toULongLong();
        path->prepend("/" + parentinfo.split(",").at(2));
    }
}
