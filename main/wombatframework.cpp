#include "wombatframework.h"

WombatFramework::WombatFramework(WombatVariable* wombatvariable)
{
    wombatptr = wombatvariable;
}
WombatFramework::~WombatFramework()
{
}

void WombatFramework::OpenEvidenceImage() // open current evidence image
{
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatptr->evidenceobject.fullpathvector.size()*sizeof(char*));
    for(int i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
        //fprintf(stderr, "fpv: %s\n", wombatptr->evidenceobject.fullpathvector[i].c_str());
    }
    wombatptr->evidenceobject.imageinfo = tsk_img_open(wombatptr->evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(wombatptr->evidenceobject.imageinfo == NULL)
        qDebug() << "print image error here";
    free(images);
}

void WombatFramework::AddEvidenceNodes() // add evidence node to the dirmodel
{
    wombatptr->dirmodel->clear();
    QStringList headerList;
    headerList << "Name" << "Unique ID" << "Full Path" << "Size (Bytes)" << "Created (UTC)" << "Accessed (UTC)" << "Modified (UTC)" << "Status Changed (UTC)" << "MD5 Hash";
    wombatptr->dirmodel->setHorizontalHeaderLabels(headerList);
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        QList<QStandardItem*> tmplist;
        QStandardItem* tmpnode = new QStandardItem(QString::number(wombatptr->evidenceobjectvector[i].id));
        tmpnode->setCheckable(true);
        tmpnode->setIcon(QIcon(":/basic/treeimage"));
        tmplist << tmpnode << new QStandardItem(wombatptr->evidenceobjectvector[i].name);
        wombatptr->dirmodel->invisibleRootItem()->appendRow(tmplist);
        AddPartitionNodes(i);
    }
}

void WombatFramework::AddPartitionNodes(int increment) // add partition/fs nodes to the image node
{
    qDebug() << "current evidenceobjectvector item: " << increment;
    QList<QStandardItem*> evidnode = wombatptr->dirmodel->findItems(QString::number(wombatptr->evidenceobjectvector[increment].id), Qt::MatchExactly, 0);
    QList<QStandardItem*> tmplist;
    QStandardItem* tmpnode;
    QString tmpstring = "";
    for(int j=0; j < wombatptr->volumeobjectvector.count(); j++)
    {
        if(wombatptr->evidenceobjectvector[increment].id == wombatptr->volumeobjectvector[j].parentid)
        {
            qDebug() << "vol part children count: " << wombatptr->volumeobjectvector[j].childcount;
            if(wombatptr->volumeobjectvector[j].childcount > 0)
            {
                for(int i=0; i < wombatptr->partitionobjectvector.count(); i++)
                {
                    if(wombatptr->volumeobjectvector[j].id == wombatptr->partitionobjectvector[i].parentid)
                    {
                        tmplist.clear();
                        tmpstring = "";
                        tmpnode = NULL;
                        tmpnode = new QStandardItem(QString::number(wombatptr->partitionobjectvector[i].id));
                        tmpnode->setCheckable(true);
                        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
                        int sectend = wombatptr->partitionobjectvector[i].sectstart + wombatptr->partitionobjectvector[i].sectlength - 1;
                        QString tmpstring = wombatptr->partitionobjectvector[i].name + " [" + QString::number(wombatptr->partitionobjectvector[i].sectstart) + "-" + QString::number(sectend) + "]";
                        tmplist << tmpnode << new QStandardItem(tmpstring);
                        evidnode[0]->appendRow(tmplist);
                    }
                }
            }
            else
            {
                qDebug() << "fs count: " << wombatptr->filesystemobjectvector.count();
                for(int i=0; i < wombatptr->filesystemobjectvector.count(); i++)
                {
                    qDebug() << "into fs loop";
                    qDebug() << "vol id: " << wombatptr->volumeobjectvector[j].id;
                    qDebug() << "fs parent id: " << wombatptr->filesystemobjectvector[i].parentid;
                    if(wombatptr->volumeobjectvector[j].id == wombatptr->filesystemobjectvector[i].parentid)
                    {
                        qDebug() << "into fs if statement";
                        tmplist.clear();
                        tmpstring = "";
                        tmpnode = NULL;
                        tmpnode = new QStandardItem(QString::number(wombatptr->filesystemobjectvector[i].id));
                        tmpnode->setCheckable(true);
                        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
                        int sectend = wombatptr->filesystemobjectvector[i].blocksize * wombatptr->filesystemobjectvector[i].blockcount;
                        QString tmpstring = wombatptr->filesystemobjectvector[i].name + " [" + QString::number(wombatptr->filesystemobjectvector[i].byteoffset) + "-" + QString::number(sectend) + "]";
                        tmplist << tmpnode << new QStandardItem(tmpstring);
                        evidnode[0]->appendRow(tmplist);
                    }
                    else
                        qDebug() << "fs if failed";
                }
            }
        }
    }
    /*
    for(int i=0; i < wombatptr->partitionobjectvector.count(); i++)
    {
        tmplist.clear();
        tmpstring = "";
        tmpnode = NULL;
        tmpnode = new QStandardItem(QString::number(wombatptr->partitionobjectvector[i].id));
        tmpnode->setCheckable(true);
        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
        int sectend = wombatptr->partitionobjectvector[i].sectstart + wombatptr->partitionobjectvector[i].sectlength - 1;
        QString tmpstring = wombatptr->partitionobjectvector[i].name + " [" + QString::number(wombatptr->partitionobjectvector[i].sectstart) + "-" + QString::number(sectend) + "]";
        tmplist << tmpnode << new QStandardItem(tmpstring);
        evidnode[0]->appendRow(tmplist);
    }
    for(int i=0; i < wombatptr->filesystemobjectvector.count(); i++)
    {
        tmplist.clear();
        tmpstring = "";
        tmpnode = NULL;
        tmpnode = new QStandardItem(QString::number(wombatptr->filesystemobjectvector[i].id));
        tmpnode->setCheckable(true);
        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
        int sectend = wombatptr->filesystemobjectvector[i].blocksize * wombatptr->filesystemobjectvector[i].blockcount;
        QString tmpstring = wombatptr->filesystemobjectvector[i].name + " [" + QString::number(wombatptr->filesystemobjectvector[i].byteoffset) + "-" + QString::number(sectend) + "]";
        tmplist << tmpnode << new QStandardItem(tmpstring);
        evidnode[0]->appendRow(tmplist);
    }*/
}

void WombatFramework::OpenVolumeSystem() // open current volume system
{
    wombatptr->evidenceobject.volinfo = tsk_vs_open(wombatptr->evidenceobject.imageinfo, 0, TSK_VS_TYPE_DETECT);
    //if(wombatptr->evidenceobject.volinfo == NULL)
        //fprintf(stderr, "no volume, create dummy one.");
}

void WombatFramework::GetVolumeSystemName() // get the volume system name
{
    if(wombatptr->evidenceobject.volinfo == NULL)
        wombatptr->currentvolumename = "Dummy Volume";
    else
        wombatptr->currentvolumename = QString::fromUtf8(tsk_vs_type_todesc(wombatptr->evidenceobject.volinfo->vstype));
    //wombatptr->volumeobjectvector.append(wombatptr->volumeobject); // may not need.
}

void WombatFramework::OpenPartitions() // open the partitions in the volume
{
    wombatptr->evidenceobject.fsinfovector.clear();
    // CHECK IF VOLUME EXISTS, IF NOT THEN SHOULD GET FS FROM IMG INSTEAD OF VOL.
    if(wombatptr->evidenceobject.volinfo == NULL)
    {
        wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_img(wombatptr->evidenceobject.imageinfo, 0, TSK_FS_TYPE_DETECT));
    }
    else
    {
        fprintf(stderr, "partitioncount: %i\n", wombatptr->evidenceobject.volinfo->part_count);
        if(wombatptr->evidenceobject.volinfo->part_count > 0)
        {
            for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
            {
                wombatptr->evidenceobject.partinfovector.push_back(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i));
                TSK_FS_INFO* tmpfsinfo = tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT);
                if(tmpfsinfo != NULL)
                {
                    wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT));
                }
                else
                {
                }
            }
        }
    }
}
// DEFINITELY DON'T NEED THIS FUNCTION RIGHT NOW.
void WombatFramework::OpenFileSystems() // open the filesystems in the image or partition
{
    // may not need this function since i can open them when the partitions are added w/o having to reloop it.
}

void WombatFramework::OpenEvidenceImages() // open all evidence images.
{
    for(int j = 0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        const TSK_TCHAR** images;
        images = (const char**)malloc(wombatptr->evidenceobjectvector[j].fullpathvector.size()*sizeof(char*));
        for(int i=0; i < wombatptr->evidenceobjectvector[j].fullpathvector.size(); i++)
        {
            images[i] = wombatptr->evidenceobjectvector[j].fullpathvector[i].c_str();
        }
        wombatptr->evidenceobjectvector[j].imageinfo = tsk_img_open(wombatptr->evidenceobjectvector[j].itemcount, images, TSK_IMG_TYPE_DETECT, 0);
        free(images);
    }
}

void WombatFramework::GetBootCode() // deermine boot type and populate variable if exists otherwise populate wiht negative
{
    // while this byte reading and converting is great... to find out the boot information, i can pull it from my volume system information such as...
    //
    if(wombatptr->evidenceobject.volinfo != NULL)
    {
        wombatptr->htmlcontent += "<tr><td class='property'>byte offset</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->evidenceobject.volinfo->offset)  + "</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>block size</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->evidenceobject.volinfo->block_size) + " bytes</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>endian ordering</td><td class='pvalue'>";
        if(wombatptr->evidenceobject.volinfo->endian == TSK_LIT_ENDIAN)
            wombatptr->htmlcontent += "little endian";
        else
            wombatptr->htmlcontent += "big endian";
        wombatptr->htmlcontent += "</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>partition count</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->evidenceobject.volinfo->part_count) + "</td></tr></table>";
        //wombatptr->htmlcontent += "<br/><br/><div class='tabletitle'>boot sector</div>";
        //wombatptr->htmlcontent += "<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr>";
        //wombatptr->htmlcontent += "<tr class='odd'><td>0-0</td><td class='bvalue'></td><td class='desc'></td></tr>";
        //wombatptr->htmlcontent += "<tr class='odd'><td colspan='3' class='bot'></td></tr></table>";
        // populate the boot sector table
        // PARTITION INFORMATION I NEED TO RECORD: ACTIVE/BOOTABLE, PARTITION TYPE, LBA OF FIRST ABSOLUTE SECTOR OF PARTITION,# OF SECTORS IN PARTITION
        // PARTITION INFORMATION FROM TSK INFO   : FLAGS, LEN (# OF SECTORS), START, (FIRST SECTOR), DESC, SLOT_NUM, TABLE_NUM 
    }
    else // not a bootable volume
    {
        wombatptr->htmlcontent += "</table><br/><br/><div class='tabletitle'>not a bootable volume</div>";
        // block size, partition count, offset where volume system begins, endian - probably go in a table similar to the above one, not a byte offset one.
        // not sure if i want to include the minimal amount of sector info since there are so few and it'll be repetative with the possible partition
        // layout in the tree view
    }
    int retval;
    //tmpelement.appendInside("<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr><tr class='odd'><td>0-2</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[0] + "</td><td class='desc'>Jump instruction to the boot code</td></tr><tr class='even'><td>3-10</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[1] + "</td><td class='desc'>OEM name string field. This field is ignored by Microsoft operating systems</td></tr><tr class='odd'><td>11-12</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[2] + " bytes</td><td class='desc'>Bytes per sector</td></tr><tr class='even'><td>13-13</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[3] + " sectors</td><td class='desc'>Seectors per cluster</td></tr><tr class='odd'><td colspan='3' class='bot'></td></tr></table>");
    QString tmpstr = "";
    char* bootbuffer = NULL;
    wombatptr->rawbyteintvector.clear();
    bootbuffer = new char[wombatptr->evidenceobject.imageinfo->sector_size];
    retval = tsk_img_read(wombatptr->evidenceobject.imageinfo, 0, bootbuffer, wombatptr->evidenceobject.imageinfo->sector_size);
    if(retval > 0)
    {
        wombatptr->rawbyteintvector.resize(wombatptr->evidenceobject.imageinfo->sector_size);
        for(int i=0; i < retval; i++)
        {
            wombatptr->rawbyteintvector[i] = bootbuffer[i];
        }
        //qDebug() << "Byte to Hex: " << Translate::ByteToHex(wombatptr->rawbyteintvector[510]);
        //qDebug() << "Byte to Int: " << wombatptr->rawbyteintvector[510];
        vector<uchar> subchar;
        subchar.push_back(wombatptr->rawbyteintvector[510]);
        subchar.push_back(wombatptr->rawbyteintvector[511]);
        Translate::ByteToHex(tmpstr, subchar);
        if(QString::compare("55aa", tmpstr) == 0) // its a boot sector
        {
                // now to determine if its got a partition table
        }
        //Translate::ByteToBinary(tmpstr, subchar);
        //qDebug() << "Byte to Bin: " << tmpstr;
    }
        //wombatptr->bootbytearray = QByteArray::fromRawData(wombatptr->bootbuffer, wombatptr->evidenceobject.imageinfo->sector_size);
        /*
        qDebug() << ByteArrayToHex(wombatptr->bootbytearray.mid(510,2)); // nets my signature value to compare
        if(QString::compare("55aa", ByteArrayToHex(wombatptr->bootbytearray.mid(510,2))) == 0) // its a boot sector
        {
            qDebug() << "byte to examine: " << ByteArrayToHex(wombatptr->bootbytearray.mid(510,1));
	    qDebug() << "1. byte to hex: " << SingleByteToString(wombatptr->bootbytearray.mid(510,1), 16);
	    qDebug() << "2. byte to Int: " << SingleByteToString(wombatptr->bootbytearray.mid(510,1), 10);
	    qDebug() << "3. byte to Binary: " << SingleByteToString(wombatptr->bootbytearray.mid(510,1), 2);
	    qDebug() << "4. bytearray number: " << QByteArray::number(10, 16);
	    qDebug() << "5. byte (0x10) to binary: " << SingleByteToString(QByteArray::number(10, 16), 2);
            // determine if its got a partition table
            // NEED TO CHECK THE 64 BYTE PARTITION TABLE FOR THE 1ST BYTE OF EACH 16 BYTE PARTITION ENTRY. IF ITS 80 THEN ITS BOOTABLE AND OLD, IF THE
            // SEVENTH BIT IS ACTIVE (CHECK BYTE FUNCTION) THEN ITS A BOOT PARAMETER... SO NEED TO CHECK EACH AND ALSO CHECK THE PARTITION TYPE AGAINST
            // A VALID ENTRY IN MY PARTITION TABLE MASK.
        //}
        //wombatptr->bootsectorlist << ByteArrayToHexDisplay(wombatptr->bootbytearray.mid(0,3));
        */
    /*
     *
        if(wombatvarptr->selectedobject.type == 1)
        {
            QWebElement tmpelement = ui->webView->page()->currentFrame()->documentElement().lastChild();
            // if no boot code/partition table, then simply return info to say no boot code present.
            // OR POSSIBLY STATE WHAT THE IMAGE CONTAINS (PARTITION TABLE/SINGLE PARTITION ONLY)
            // if DOS partition table...
            // if boot code exists...
            // check for signature value at offset 510-511 to determine if its a dosmbr or something else. if its 0x55AA then its dos mbr
            // check for partition table and populate the values accordingly.
            // the fs stuff i find at jump and oem and the others are for the filesystem/partition boot sector. this isn't valid when there is an mbr.
            // need to determine if there is an mbr and then pull the partition table information from it. otherwise simply display the image info
            // and have no mbr present in first sector.
            // when you click on the partition, this is where the partition boot sector information will go.
            // MOVE ALL OF THIS CODE INTO WOMBATFRAMEWORK...
            GetDosBootCode(); // move into getbootcode above
            tmpelement.appendInside("<br/><br/><div class='tabletitle'>boot sector</div>");
            tmpelement.appendInside("<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr><tr class='odd'><td>0-2</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[0] + "</td><td class='desc'>Jump instruction to the boot code</td></tr><tr class='even'><td>3-10</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[1] + "</td><td class='desc'>OEM name string field. This field is ignored by Microsoft operating systems</td></tr><tr class='odd'><td>11-12</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[2] + " bytes</td><td class='desc'>Bytes per sector</td></tr><tr class='even'><td>13-13</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[3] + " sectors</td><td class='desc'>Seectors per cluster</td></tr><tr class='odd'><td colspan='3' class='bot'></td></tr></table>");
void WombatForensics::GetDosBootCode()
{
    int retval;
    wombatvarptr->bootbuffer = NULL;
    wombatvarptr->bootbuffer = new char[wombatvarptr->evidenceobject.imageinfo->sector_size];
    retval = tsk_img_read(wombatvarptr->evidenceobject.imageinfo, 0, wombatvarptr->bootbuffer, wombatvarptr->evidenceobject.imageinfo->sector_size);
    if(retval > 0)
    {
        wombatvarptr->bootbytearray = QByteArray::fromRawData(wombatvarptr->bootbuffer, wombatvarptr->evidenceobject.imageinfo->sector_size);
        fprintf(stderr, "oem from byte array: %s\n", QString::fromUtf8(wombatvarptr->bootbytearray.mid(3,8)).toStdString().c_str());
        wombatvarptr->bootsectorlist << ByteArrayToHexDisplay(wombatvarptr->bootbytearray.mid(0,3));
        wombatvarptr->bootsectorlist << QString::fromUtf8(wombatvarptr->bootbytearray.mid(3,8));
        wombatvarptr->bootsectorlist << ByteArrayToShortDisplay(wombatvarptr->bootbytearray.mid(11,2));
        wombatvarptr->bootsectorlist << ByteArrayToShortDisplay(wombatvarptr->bootbytearray.mid(13,1));
    }
    else
        fprintf(stderr, "filling bootbuffer failed\n");

 *
    uint64_t bytelen = 512*(seclength - 1);
    uint64_t bytestart = 512*secstart;
    char* volbuffer = NULL;
    volbuffer = new char[bytelen+512];

    {
        // need to figure out why this fails... and returns -1
        retval = TskServices::Instance().getImageFile().getSectorData(secstart, seclength-1, volbuffer);
        fprintf(stderr, "sector data return value: %i\n", retval);
        if (retval == -1)
        {
        }
    }
    if(retval > 0)
    {
        QFile tmpfile("/home/pasquale/WombatForensics/tmpfiles/volbyte.dat");
        tmpfile.open(QIODevice::WriteOnly);
        tmpfile.write(volbuffer, bytelen);
        tmpfile.close();
    }
    delete[] volbuffer;
    //delete volbuffer;

    return "/home/pasquale/WombatForensics/tmpfiles/volbyte.dat";

 *
 *
 *    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    selectedinteger->setText(tmptext);
    // update float entry;
    float fvalue;
    ucharPtr = (uchar*)(&fvalue);
    if(bytes.size() < sizeof(float) )
    {
        for(unsigned int i= 0; i < sizeof(float); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&fvalue,&bytes.begin()[0],sizeof(float));
    }
    strvalue.setNum( fvalue );
    tmptext = "Float: " + strvalue;
    selectedfloat->setText(tmptext);
    // update double
    double dvalue;
    ucharPtr = (uchar*)&dvalue;
    if(bytes.size() < sizeof(double) )
    {
        for(unsigned int i= 0; i < sizeof(double); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&dvalue,&bytes.begin()[0],sizeof(double));
    }
    strvalue.setNum( dvalue );
    tmptext = "Double: " + strvalue;
    selecteddouble->setText(tmptext);

 */ 
}

// BELOW FUNCTION CURRENTLY NOT USED
void WombatFramework::BuildEvidenceModel()
{
    // COME BACK TO QTCONCURRENT AS I GET FARTHER ALONG AND RESEARCH MORE
    /*
    QVector<ItemObject> itemvector;
    for(int i = 0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        itemvector.append(wombatptr->evidenceobjectvector[i]);
    }
    QFutureWatcher<void> watcher;
    watcher.setFuture(QtConcurrent::map(itemvector, &WombatFramework::OpenEvidenceImages));
    //watcher.setFuture(QtConcurrent::map(wombatptr->evidenceobjectvector, WombatFramework::OpenEvidenceImages));
    watcher.waitForFinished();
    */
    OpenEvidenceImages(); // PROBABLY PUT THIS IN A MULTI-THREAD ENVIRONMENT.
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++) // for each evidence image info file.
    {
        if(wombatptr->evidenceobjectvector[i].imageinfo == NULL)
            fprintf(stderr, "Image didn't open. add to log file as error.\n");
        else
            fprintf(stderr, "Image %s opened. add to log file as info.\n", wombatptr->evidenceobjectvector[i].fullpathvector[0].c_str());
    }
    // NEED TO LAUNCH THIS IN A NEW THREAD TO KEEP GUI RESPONSIVE
    // NEED TO OPEN THE IMAGE - img_open.c [tsk_img_open()]
    // NEED TO GET THE METADATA FOR THE IMAGE/VOLUMES/PARTITIONS/FILES SO I CAN POPULATE THE DIRECTORY TREE INFORMATION


    // NEED TO ADD THE EVIDENCE ITEM TO THE DATABASE
    // POPULATE THE WOMBATVARPTR FOR THE EVIDENCEOBJECT VECTOR
    // NEED TO CREATE THE EVIDENCE TSK DATABASE (EXTRACT EVIDENCE ACCORDING TO MODULES)
    // NEED TO BUILD DIRMODEL AS I GO AND POPULATE DIRTREEVIEW AS I GO WITH EACH FILE
    // FOR NOW I WON'T BUILD MODULES, I'LL JUST DESIGN A MULTI-THREADED APPROACH FOR IT AND ABSTRACT TO PLUGGABLE MODULES LATER

    /*
    QFutureWatcher<void> watcher;
    std::vector<TskSchedulerQueue::task_struct* > tmpvector;
    watcher.setFuture(QtConcurrent::map(&tmpvector, &SleuthKitPlugin::TaskMap));
    //watcher.setFuture(QtConcurrent::map(&((std::vector<TskSchedulerQueue::task_struct*>)scheduler.mapvector), &SleuthKitPlugin::TaskMap));
    watcher.waitForFinished();
    */
    // QT CONCURRENT TEST
}
