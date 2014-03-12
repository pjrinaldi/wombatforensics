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
    qDebug()  << "fullpathvector count: " << wombatptr->evidenceobject.fullpathvector.size();
    images = (const char**)malloc(wombatptr->evidenceobject.fullpathvector.size()*sizeof(char*));
    for(int i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        qDebug() << "fullpathvector[" << i << "] = " << QString::fromStdString(wombatptr->evidenceobject.fullpathvector[i]);
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
        qDebug() << "images[" << i << "] = " << images[i];
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
    QList<QStandardItem*> evidnode = wombatptr->dirmodel->findItems(QString::number(wombatptr->evidenceobjectvector[increment].id), Qt::MatchExactly, 0);
    QList<QStandardItem*> tmplist;
    QStandardItem* tmpnode;
    QString tmpstring = "";
    for(int j=0; j < wombatptr->volumeobjectvector.count(); j++)
    {
        if(wombatptr->evidenceobjectvector[increment].id == wombatptr->volumeobjectvector[j].parentid)
        {
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
                        int bytestart = wombatptr->partitionobjectvector[i].sectstart * wombatptr->partitionobjectvector[i].blocksize;
                        int byteend = wombatptr->partitionobjectvector[i].blocksize * (wombatptr->partitionobjectvector[i].sectstart + wombatptr->partitionobjectvector[i].sectlength) - 1;
                        QString tmpstring = wombatptr->partitionobjectvector[i].name + " [" + QString::number(bytestart) + "-" + QString::number(byteend) + "]";
                        tmplist << tmpnode << new QStandardItem(tmpstring);
                        evidnode[0]->appendRow(tmplist);
                    }
                }
            }
            else
            {
                for(int i=0; i < wombatptr->filesystemobjectvector.count(); i++)
                {
                    if(wombatptr->volumeobjectvector[j].id == wombatptr->filesystemobjectvector[i].parentid)
                    {
                        tmplist.clear();
                        tmpstring = "";
                        tmpnode = NULL;
                        tmpnode = new QStandardItem(QString::number(wombatptr->filesystemobjectvector[i].id));
                        tmpnode->setCheckable(true);
                        tmpnode->setIcon(QIcon(":/basic/treefilemanager"));
                        int byteend = wombatptr->filesystemobjectvector[i].blocksize * wombatptr->filesystemobjectvector[i].blockcount;
                        QString tmpstring = wombatptr->filesystemobjectvector[i].name + " [" + QString::number(wombatptr->filesystemobjectvector[i].byteoffset) + "-" + QString::number(byteend) + "]";
                        tmplist << tmpnode << new QStandardItem(tmpstring);
                        evidnode[0]->appendRow(tmplist);
                    }
                    else
                        qDebug() << "fs if failed";
                }
            }
        }
    }
}

/*
 * LoadChildren(qstandarditem* item, selected.id)
 * if(selected.id.childcount > 0)
 * fileobjectvector<fileobject>, fileobject has the respective file data including childcount and parentid.
 * expand a parent, children need to looped so they'yre children can be loaded.
 * need to determine what fileobject will contain.
 * loop over the nodes where parentid = selected.id
 * add nodes
 *
void WombatFramework::AddFileNodes(int curidx)
{
    QList<QStandardItem*>
}
 */ 

void WombatFramework::OpenVolumeSystem() // open current volume system
{
    wombatptr->evidenceobject.volinfo = tsk_vs_open(wombatptr->evidenceobject.imageinfo, 0, TSK_VS_TYPE_DETECT);
}

void WombatFramework::GetVolumeSystemName() // get the volume system name
{
    if(wombatptr->evidenceobject.volinfo == NULL)
        wombatptr->currentvolumename = "Dummy Volume";
    else
        wombatptr->currentvolumename = QString::fromUtf8(tsk_vs_type_todesc(wombatptr->evidenceobject.volinfo->vstype));
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

void WombatFramework::OpenFiles() // open the files and add to file info vector
{
    // tsk_fs_dir_walk the fsinfovector[i] objects to get the tsk_fs_file and tsk_fs_dir info objects and add them to the
    // fileinfovector
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

void WombatFramework::CloseInfoStructures() // close all open info structures
{
    for(int j=0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        for(int i=0; i < wombatptr->evidenceobjectvector[j].fsinfovector.size(); i++)
        {
            if(wombatptr->evidenceobjectvector[j].fsinfovector[i] != NULL)
            {
                tsk_fs_close(wombatptr->evidenceobjectvector[j].fsinfovector[i]);
                wombatptr->evidenceobjectvector[j].fsinfovector[i] = NULL;
            }
        }
        wombatptr->evidenceobjectvector[j].fsinfovector.clear();
        for(int i=0; i < wombatptr->evidenceobject.partinfovector.size(); i++)
        {
            wombatptr->evidenceobjectvector[j].partinfovector[i] = NULL;
        }
        wombatptr->evidenceobjectvector[j].partinfovector.clear();
        if(wombatptr->evidenceobjectvector[j].volinfo != NULL)
        {
            tsk_vs_close(wombatptr->evidenceobjectvector[j].volinfo);
            wombatptr->evidenceobjectvector[j].volinfo = NULL;
        }
        if(wombatptr->evidenceobjectvector[j].imageinfo != NULL)
        {
            tsk_img_close(wombatptr->evidenceobjectvector[j].imageinfo);
            wombatptr->evidenceobjectvector[j].imageinfo = NULL;
        }
    }
}

void WombatFramework::GetBootCode(int idx) // deermine boot type and populate variable if exists otherwise populate wiht negative
{
    int volidx = -1;
    for(int i=0; i < wombatptr->volumeobjectvector.count(); i++)
    {
        if(wombatptr->selectedobject.id == wombatptr->volumeobjectvector[i].parentid) // boot volume exists
            volidx = i;
    }
    if(volidx > -1) // boot volume exists
    {
        wombatptr->htmlcontent += "<tr><td class='property'>byte offset</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->volumeobjectvector[volidx].byteoffset) + "</td></tr>";
        //wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->evidenceobject.volinfo->offset)  + "</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>block size</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->volumeobjectvector[volidx].blocksize) + " bytes</td></tr>";
        //wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->evidenceobject.volinfo->block_size) + " bytes</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>endian ordering</td><td class='pvalue'>";
        /*
        if(wombatptr->evidenceobject.volinfo->endian == TSK_LIT_ENDIAN)
            wombatptr->htmlcontent += "little endian";
        else
            wombatptr->htmlcontent += "big endian";
        */
        wombatptr->htmlcontent += "</td></tr>";
        wombatptr->htmlcontent += "<tr><td class='property'>partition count</td><td class='pvalue'>";
        wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->volumeobjectvector[volidx].childcount) + "</td></tr></table>";
        //wombatptr->htmlcontent += QLocale::system().toString((int)wombatptr->evidenceobject.volinfo->part_count) + "</td></tr></table>";

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
}
// OLD STUFF THAT IS USEFULL INFO FOR HOW TO GET THINGS, BUT NOT USED IN THIS CODE RIGHT NOW
    /*
    int retval;
    //tmpelement.appendInside("<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr><tr class='odd'><td>0-2</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[0] + "</td><td class='desc'>Jump instruction to the boot code</td></tr><tr class='even'><td>3-10</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[1] + "</td><td class='desc'>OEM name string field. This field is ignored by Microsoft operating systems</td></tr><tr class='odd'><td>11-12</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[2] + " bytes</td><td class='desc'>Bytes per sector</td></tr><tr class='even'><td>13-13</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[3] + " sectors</td><td class='desc'>Seectors per cluster</td></tr><tr class='odd'><td colspan='3' class='bot'></td></tr></table>");*/
    /*
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
        delete[] bootbuffer;
        // delete bootbuffer;
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
    */

int WombatFramework::DetermineVectorIndex()
{
    int curidx = -1;
    if(wombatptr->selectedobject.type == 1) // image file
    {
        for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++)
        {
            if(wombatptr->evidenceobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else if(wombatptr->selectedobject.type == 2) // volume file (it should never be a volume since i don't add it to the image tree)
    {
        for(int i=0; i < wombatptr->volumeobjectvector.count(); i++)
        {
            if(wombatptr->volumeobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else if(wombatptr->selectedobject.type == 3) // partition file
    {
        for(int i=0; i < wombatptr->partitionobjectvector.count(); i++)
        {
            if(wombatptr->partitionobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else if(wombatptr->selectedobject.type == 4) // file system file
    {
        for(int i=0; i < wombatptr->filesystemobjectvector.count(); i++)
        {
            if(wombatptr->filesystemobjectvector[i].id == wombatptr->selectedobject.id)
                curidx = i;
        }
    }
    else // implement for files, directories etc.. as i go.
    {
    }

    return curidx;
}
/*
void WombatFramework::OpenParentImage(int imgid)
{
    int curidx = -1;
    for(int i=0; i < wombatvarptr->evidenceobjectvector.count(); i++)
    {
        if(imgid == wombatvarptr->evidenceobjectvector[i].id)
            curidx = i;
    }
    tskobjptr->imagepartspath = (const char**)malloc(wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size()*sizeof(char*));
    tskobjptr->partcount = wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size();
    for(int i=0; i < wombatvarptr->evidenceobjectvector[curidx].fullpathvector.size(); i++)
    {
        tskobjptr->imagepartspath[i] = wombatvarptr->evidenceobjectvector[curidx].fullpathvector[i].c_str();
    }
    tskobjptr->readimginfo = tsk_img_open(tskobjptr->partcount, tskobjptr->imagepartspath, TSK_IMG_TYPE_DETECT, 0);
    if(tskobjptr->readimginfo == NULL)
        qDebug() << "print image error here";
    free(tskobjptr->imagepartspath);
    hexwidget->openimage(); // need to add the offset and length to tskobject prior to calling openimage.
    hexwidget->set2BPC();
    hexwidget->setBaseHex();
}
*/
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
