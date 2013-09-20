#include "sleuthkit.h"

SleuthKitPlugin::SleuthKitPlugin(WombatDatabase* wombatcasedata)
{
    wombatdata = wombatcasedata; 
}
void SleuthKitPlugin::Initialize(WombatVariable wombatVariable)
{
    wombatvariable = wombatVariable;
    SetupSystemProperties();
    SetupLog();
    SetupScheduler();
    SetupFileManager();
    SetupImageDatabase();
    SetupBlackboard();
    fprintf(stderr, "SleuthKit Existsi\n");
    qRegisterMetaType<WombatVariable>("WombatVariable");
    connect(this, SIGNAL(SetLogVariable(WombatVariable)), log, SLOT(LogVariable(WombatVariable)), Qt::DirectConnection);
}

void SleuthKitPlugin::SetupImageDatabase()
{
    // initialize dummy database to create copy new imagedb's from.
    try
    {
        initialdb = new TskImgDBSqlite(wombatvariable.datapath.toStdString().c_str(), "initial.db");
        if(initialdb->initialize() != 0)
            fprintf(stderr, "Error initializing StarterDB\n");
        else
        {
            fprintf(stderr, "Starter DB was Initialized Successfully!\n");
        }
        TskServices::Instance().setImgDB(*initialdb);
        fprintf(stderr, "Loading Starter ImageDB was Successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Starter ImageDB: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::OpenEvidence(WombatVariable wombatVariable)
{
    wombatvariable = wombatVariable;
    QString oldstring = wombatvariable.datapath + "initial.db";
    QString newstring = wombatvariable.evidencedirpath + wombatvariable.evidencedbname;
    if(QFile::copy(oldstring.toStdString().c_str(), newstring.toStdString().c_str()))
    {
        fprintf(stderr, "File Copy Was Successful\n");
        // copy was successful
        try
        {
            tmpdb = new TskImgDBSqlite(wombatvariable.evidencedirpath.toStdString().c_str(), wombatvariable.evidencedbname.toStdString().c_str());
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "Tsk New Exception: %s\n", ex.message().c_str());
        }
        try
        {
            if(tmpdb->initialize() != 0)
                fprintf(stderr, "Error intializing Evidence DB\n");
            else
            {
                fprintf(stderr, "Evidence DB was initialized successfully\n");
            }
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "Tsk Open Exception: %s\n", ex.message().c_str());
        }
        try
        {
            TskServices::Instance().setImgDB(*tmpdb);
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "Services Set ImageDB: %s\n", ex.message().c_str());
        }
    }
    else
    {
        fprintf(stderr, "File Copy was NOT successful\n");
        // copy was not successful
        // exit out with error that image already added...
    }
    fprintf(stderr, "Evidence ImgDB Path: %s\n", wombatvariable.evidencepath.toStdString().c_str());
    
    int filecount = 0;
    int processcount = 0;
    TskImageFileTsk imagefiletsk;
    try
    {
        imagefiletsk.open(wombatvariable.evidencepath.toStdString());
        TskServices::Instance().setImageFile(imagefiletsk);
        fprintf(stderr, "Opening Image File was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Opening Evidence: %s\n", ex.message().c_str());
    }
    try
    {
        imagefiletsk.extractFiles();
        fprintf(stderr, "Extracting Evidence was successful\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Extracting Evidence: %s\n", ex.message().c_str());
    }
    // Get Number of Files found here 
    filecount = TskServices::Instance().getImgDB().getNumFiles();
    emit UpdateStatus(filecount, processcount);
    LOGINFO("Processing Evidence Started");
    wombatdata->InsertMsg(wombatvariable.caseid, wombatvariable.evidenceid, wombatvariable.jobid, 2, "Processing Evidence Started");
    TskSchedulerQueue::task_struct *task;
    TskPipelineManager pipelinemgr;
    TskPipeline* filepipeline;
    TskSchedulerQueue::task_struct* currenttask;
    try
    {
        filepipeline = pipelinemgr.createPipeline(TskPipelineManager::FILE_ANALYSIS_PIPELINE);
    }
    catch(const TskException &ex)
    {
        fprintf(stderr, "Error creating file analysis pipeline: %s\n", ex.message().c_str());
    }
    // HOW ABOUT LOOP OF RUNNABLES...
    //

    for(int i=0; i < scheduler.mapvector.size(); i++)
    {
        currenttask = scheduler.mapvector[i];
        try
        {
            if(currenttask->task == Scheduler::FileAnalysis && filepipeline && !filepipeline->isEmpty())
            {
                TaskRunner* trunner = new TaskRunner(currenttask, filepipeline);
                QThreadPool::globalInstance()->start(trunner);
                //filepipeline->run(task->id);
            }
            else
            {
                fprintf(stderr, "Skipping task: %d\n", task->task);
            }
            //delete task;
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "TskException: %s\n", ex.message().c_str());
        }
        processcount++;
        emit UpdateStatus(filecount, processcount);
        emit UpdateMessageTable();

        //TaskRunner* trunner = new TaskRunner(scheduler.mapvector[i], filepipeline);
        //QThreadPool::globalInstance()->start(trunner);
    }
    delete currenttask;

    /*
    QFutureWatcher<void> watcher;
    std::vector<TskSchedulerQueue::task_struct* > tmpvector;
    watcher.setFuture(QtConcurrent::map(&tmpvector, &SleuthKitPlugin::TaskMap));
    //watcher.setFuture(QtConcurrent::map(&((std::vector<TskSchedulerQueue::task_struct*>)scheduler.mapvector), &SleuthKitPlugin::TaskMap));
    watcher.waitForFinished();
    */
    // QT CONCURRENT TEST
    /*
    while((task = scheduler.nextTask()) != NULL)
    {
        try
        {
            if(task->task == Scheduler::FileAnalysis && filepipeline && !filepipeline->isEmpty())
            {
                filepipeline->run(task->id);
            }
            else
            {
                fprintf(stderr, "Skipping task: %d\n", task->task);
            }
            delete task;
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "TskException: %s\n", ex.message().c_str());
        }
        processcount++;
        emit UpdateStatus(filecount, processcount);
        emit UpdateMessageTable();
    }
    */
    /*
    if(filepipeline && !filepipeline->isEmpty())
    {
        filepipeline->logModuleExecutionTimes();
    }
    */
    LOGINFO("Processing Evidence Finished");
    wombatdata->InsertMsg(wombatvariable.caseid, wombatvariable.evidenceid, wombatvariable.jobid, 2, "Processing Evidence Finished");
    fprintf(stderr, "File Count: %d - Process Count: %d\n", filecount, processcount);
    wombatdata->UpdateJobEnd(wombatvariable.jobid, filecount, processcount);
    GetImageTree(wombatvariable, 1);
    LOGINFO("Adding Evidence Completed");
    wombatdata->InsertMsg(wombatvariable.caseid, wombatvariable.evidenceid, wombatvariable.jobid, 2, "Adding Evidence Completed");
}

void SleuthKitPlugin::TaskMap(TskSchedulerQueue::task_struct* &task)
{
    TskPipelineManager pipelinemgr;
    TskPipeline* filepipeline;
    try
    {
        filepipeline = pipelinemgr.createPipeline(TskPipelineManager::FILE_ANALYSIS_PIPELINE);
    }
    catch(const TskException &ex)
    {
        fprintf(stderr, "Error creating file analysis pipeline: %s\n", ex.message().c_str());
    }
    try
    {
        if(task->task == Scheduler::FileAnalysis && filepipeline && !filepipeline->isEmpty())
        {
            filepipeline->run(task->id);
        }
        else
        {
            fprintf(stderr, "Skipping task: %d\n", task->task);
        }
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "TskException: %s\n", ex.message().c_str());
    }
    if(filepipeline && !filepipeline->isEmpty())
    {
        filepipeline->logModuleExecutionTimes();
    }
}

void SleuthKitPlugin::SetEvidenceDB(WombatVariable wombatVariable)
{
    wombatvariable = wombatVariable;
    try
    {
        tmpdb = new TskImgDBSqlite(wombatvariable.evidencedirpath.toStdString().c_str(), wombatvariable.evidencedbname.toStdString().c_str());
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Tsk New Exception: %s\n", ex.message().c_str());
    }
    try
    {
        if(tmpdb->open() != 0)
            fprintf(stderr, "Error intializing Evidence DB\n");
        else
        {
            fprintf(stderr, "Evidence DB was initialized successfully\n");
        }
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Tsk Open Exception: %s\n", ex.message().c_str());
    }
    try
    {
        TskServices::Instance().setImgDB(*tmpdb);
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Services Set ImageDB: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::ShowFile(WombatVariable wombatVariable)
{
    wombatvariable = wombatVariable;
    SetEvidenceDB(wombatvariable);
    GetFileContents(wombatvariable.fileid);
    GetFileTxtContents(wombatvariable.fileid);
    emit LoadFileContents(wombatvariable.tmpfilepath);
}

void SleuthKitPlugin::PopulateCase(WombatVariable wombatVariable)
{
    wombatvariable = wombatVariable;
    QStringList evidencepathlist = wombatdata->ReturnCaseEvidence(wombatvariable.caseid); // fullpath dd list
    QStringList evidenceidlist = wombatdata->ReturnCaseEvidenceID(wombatvariable.caseid); // evidenceid list
    QStringList evidenceaddjoblist = wombatdata->ReturnCaseEvidenceAddJobID(wombatvariable.caseid, evidenceidlist); // jobid list
    for(int i=0; i < evidenceidlist.count(); i++)
    {
        wombatvariable.evidenceid = evidenceidlist[i].toInt();
        wombatvariable.evidencepath = evidencepathlist[i];
        wombatvariable.jobid = evidenceaddjoblist[i].toInt();
        wombatvariable.evidencedbname = wombatvariable.evidencepath.split("/").last() + ".db";
        emit SetLogVariable(wombatvariable);
        SetEvidenceDB(wombatvariable);
        GetImageTree(wombatvariable, 0);
        emit PopulateProgressWindow(wombatvariable);
    }
}

void SleuthKitPlugin::SetupSystemProperties()
{
    QString tmpPath = wombatvariable.settingspath;
    tmpPath += "/tsk-config.xml";
    QFile tmpFile(tmpPath);
    fprintf(stderr, "TmpPath: %s\n", tmpPath.toStdString().c_str());
    if(!tmpFile.exists()) // if tsk-config.xml does not exist, create and write it here
    {
        if(tmpFile.open(QFile::WriteOnly | QFile::Text))
        {   
            QXmlStreamWriter xml(&tmpFile);
            xml.setAutoFormatting(true);
            xml.writeStartDocument();
            xml.writeStartElement("TSK_FRAMEWORK_CONFIG");
            xml.writeStartElement("CONFIG_DIR");
            xml.writeCharacters(wombatvariable.settingspath);
            xml.writeEndElement();
            xml.writeStartElement("MODULE_DIR");
            xml.writeCharacters("/usr/local/lib");
            xml.writeEndElement();
            xml.writeStartElement("MODULE_CONFIG_DIR");
            xml.writeCharacters("/usr/local/share/tsk");
            xml.writeEndElement();
            xml.writeStartElement("PIPELINE_CONFIG_FILE");
            xml.writeCharacters("#CONFIG_DIR#/tsk-pipe.xml");
            xml.writeEndElement();
            xml.writeEndElement();
            xml.writeEndDocument();
        }
        else
        {
            fprintf(stderr, "Could not open file to write\n");
        }
        tmpFile.close();
    }
    try
    {
        systemproperties = new TskSystemPropertiesImpl();
        systemproperties->initialize(tmpPath.toStdString());
        TskServices::Instance().setSystemProperties(*systemproperties);
        fprintf(stderr, "Configuration File Loading was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Config File config file: %s\n", ex.message().c_str());
    }
    tmpPath = wombatvariable.settingspath;
    tmpPath += "/tsk-pipe.xml";
    QFile pipeFile(tmpPath);
    fprintf(stderr, "PipPath: %s\n", tmpPath.toStdString().c_str());
    if(!pipeFile.exists()) // if tsk-pipe.xml does not exist, create and write it here
    {
        if(pipeFile.open(QFile::WriteOnly | QFile::Text))
        {
            QXmlStreamWriter pxml(&pipeFile);
            pxml.setAutoFormatting(true);
            pxml.writeStartDocument();
            pxml.writeStartElement("PIPELINE_CONFIG");
            pxml.writeStartElement("PIPELINE");
            pxml.writeAttribute("type", "FileAnalysis");
            pxml.writeStartElement("MODULE");
            pxml.writeAttribute("order", "1");
            pxml.writeAttribute("type", "plugin");
            pxml.writeAttribute("location", "tskHashCalcModule");
            pxml.writeEndElement();
            pxml.writeStartElement("MODULE");
            pxml.writeAttribute("order", "2");
            pxml.writeAttribute("type", "plugin");
            pxml.writeAttribute("location", "tskFileTypeSigModule");
            pxml.writeEndElement();
            pxml.writeEndElement();
            pxml.writeEndElement();
            pxml.writeEndDocument();
        }
        else
            fprintf(stderr, "Could not open file for writing\n");
        pipeFile.close();
    }
    try
    {
        SetSystemProperty(TskSystemProperties::PIPELINE_CONFIG_FILE, tmpPath.toStdString());
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "couldn't load pipeline: %s\n", ex.message().c_str());
    }
    try
    {
        SetSystemProperty(TskSystemProperties::OUT_DIR, "/home/pasquale/WombatForensics/tmpfiles");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Setting out dir failed: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::SetupLog()
{
    QString tmpPath = wombatvariable.datapath + "/tsk-log.txt";
    try
    {
        log = new TskLog();
        log->open(tmpPath.toStdString().c_str());
        TskServices::Instance().setLog(*log);
        fprintf(stderr, "Loading Log File was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Log File: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::SetupBlackboard()
{
    try
    {
        TskServices::Instance().setBlackboard((TskBlackboard &)TskDBBlackboard::instance());
        fprintf(stderr, "Loading Blackboard was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Blackboard: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::SetupScheduler()
{
    try
    {
        TskServices::Instance().setScheduler(scheduler);
        fprintf(stderr, "Loading Scheduler was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Scheduler: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::SetupFileManager()
{
    try
    {
        TskServices::Instance().setFileManager(fileManager->instance());
        fprintf(stderr, "Loading File Manager was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading FileManager: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::threadFinished()
{
    fprintf(stderr, "The Thread Finished. ");
}


void SleuthKitPlugin::GetImageTree(WombatVariable wombatvariable, int isAddEvidence)
{
    QString imagename = wombatvariable.evidencepath.split("/").last();
    std::vector<TskFileRecord> fileRecordVector;
    std::list<TskVolumeInfoRecord> volRecordList;
    std::list<TskFsInfoRecord> fsInfoRecordList;
    QString fullPath = "";
    QString currentVolPath = "";
    QString currentFsPath = "";
    TskFileRecord tmpRecord;
    TskVolumeInfoRecord volRecord;
    TskFsInfoRecord fsInfoRecord;
    QStandardItem *fsNode;
    QStandardItem *volNode;
    QStandardItem *imageNode = new QStandardItem(imagename);
    imageNode->setIcon(QIcon(":/basic/treeimage"));
    int ret;
    uint64_t tmpId;
    volRecordList.clear();
    fileRecordVector.clear();
    fsInfoRecordList.clear();
    // also need to get the partitions and volumes as nodes.
    ret = TskServices::Instance().getImgDB().getVolumeInfo(volRecordList);
    //fprintf(stderr, "volrecordlist count: %d\n", volRecordList.count());
    foreach(volRecord, volRecordList) // populates all vol's and fs's.
    {
        // if volflag = 0, get description
        // if volflag = 1, list as unallocated
        fprintf(stderr, "Vol Description: %s - VolFlags: %d\n", volRecord.description.c_str(), volRecord.flags);
        if(volRecord.flags >= 0 && volRecord.flags <= 2)
        {
            if(volRecord.flags == 1)
            {
                volNode = new QStandardItem(QString::fromUtf8(volRecord.description.c_str()));
                volNode->setIcon(QIcon(":/basic/treefilemanager"));
                currentVolPath = QString::fromUtf8(volRecord.description.c_str()) + "/";
            }
            else if(volRecord.flags == 0)
            {
                volNode = new QStandardItem("unallocated space");
                volNode->setIcon(QIcon(":/basic/treefilemanager"));
                currentVolPath = "unallocated space/";
            }
            else if(volRecord.flags == 2)
            {
                volNode = new QStandardItem(QString::fromUtf8(volRecord.description.c_str()));
                volNode->setIcon(QIcon(":/basic/treefilemanager"));
                currentVolPath = QString::fromUtf8(volRecord.description.c_str());
                currentVolPath += "/";
            }
            else
            {
                // don't display anything
            }
            // for each volrecord, get fsinfo list
            imageNode->appendRow(volNode);
            ret = TskServices::Instance().getImgDB().getFsInfo(fsInfoRecordList);
            foreach(fsInfoRecord, fsInfoRecordList)
            {
                if(fsInfoRecord.vol_id == volRecord.vol_id)
                {
                    QString fsType = "";
                    // NEED TO DO SWITCH/CASE HERE TO GET FSTYPE
                    if(fsInfoRecord.fs_type == 1)
                        fsType = "NTFS";
                    else if(fsInfoRecord.fs_type == 3)
                        fsType = "FAT12";
                    else if(fsInfoRecord.fs_type == 4)
                        fsType = "FAT16";
                    else if(fsInfoRecord.fs_type == 5)
                        fsType = "FAT32";
                    else if(fsInfoRecord.fs_type == 7)
                        fsType = "UFS1";
                    else if(fsInfoRecord.fs_type == 8)
                        fsType = "UFS1B";
                    else if(fsInfoRecord.fs_type == 9)
                        fsType = "UFS2";
                    else if(fsInfoRecord.fs_type == 11)
                        fsType = "EXT2";
                    else if(fsInfoRecord.fs_type == 12)
                        fsType = "EXT3";
                    else if(fsInfoRecord.fs_type == 14)
                        fsType = "SWAP";
                    else if(fsInfoRecord.fs_type == 16)
                        fsType = "RAW";
                    else if(fsInfoRecord.fs_type == 18)
                        fsType = "ISO9660";
                    else if(fsInfoRecord.fs_type == 20)
                        fsType = "HFS";
                    else if(fsInfoRecord.fs_type == 22)
                        fsType = "EXT4";
                    else if(fsInfoRecord.fs_type == 23)
                        fsType = "YAFFS2";
                    else if(fsInfoRecord.fs_type == 25)
                        fsType = "UNSUPPORTED";
                    else
                        fsType = QString::number(fsInfoRecord.fs_type);
                    fsNode = new QStandardItem(fsType);
                    fsNode->setIcon(QIcon(":/basic/treepartition"));
                    currentFsPath = fsType + "/";
                    volNode->appendRow(fsNode);
                    //BEGIN FILE ADD CODE
                    std::vector<uint64_t> fileidVector;
                    //Create custom function to access this...
                    sqlite3* tmpImgDB;
                    QString tmpImgDbPath = wombatvariable.evidencedirpath + imagename + ".db";
                    QList<int> objectidlist;
                    objectidlist.clear();
                    if(sqlite3_open(tmpImgDbPath.toStdString().c_str(), &tmpImgDB) == SQLITE_OK)
                    {
                        sqlite3_stmt* stmt;
                        if(sqlite3_prepare_v2(tmpImgDB, "SELECT file_id FROM fs_files WHERE fs_id = ? ORDER BY file_id", -1, &stmt, 0) == SQLITE_OK)
                        {
                            if(sqlite3_bind_int(stmt, 1, fsInfoRecord.fs_id) == SQLITE_OK)
                            {
                                while(sqlite3_step(stmt) == SQLITE_ROW)
                                {
                                    uint64_t fileId = (uint64_t)sqlite3_column_int(stmt, 0);
                                    fileidVector.push_back(fileId);
                                    if(isAddEvidence == 1)
                                        objectidlist.append(wombatdata->InsertObject(wombatvariable.caseid, wombatvariable.evidenceid, (int)fileId));
                                }
                                sqlite3_finalize(stmt);
                            }
                            else
                            {
                                //std::wstringstream infoMessage;
                                //infoMessage << L"Get FsFileIds Failed: " << sqlite3_errmsg(imgdb);
                                //LOGERROR(infoMessage.c_str());
    
                            }
                        }
                        else
                        {
                            //std::wstringstream infoMessage;
                            //infoMessage << L"Get FsFileIds Failed: " << sqlite3_errmsg(imgdb);
                            //LOGERROR(infoMessage.c_str());
                        }
                    }
                    sqlite3_close(tmpImgDB);
                    //end create custom function to access fileid
                    QList<QList<QStandardItem*> > treeList;
                    foreach(tmpId, fileidVector)
                    {
                        ret = TskServices::Instance().getImgDB().getFileRecord(tmpId, tmpRecord);
                        fileRecordVector.push_back(tmpRecord);
                    }
                    for(int i=0; i < (int)fileRecordVector.size(); i++)
                    {
                        fullPath = imagename + "/" + currentVolPath + currentFsPath + QString(fileRecordVector[i].fullPath.c_str());
                        // full path might contain more than i thought, to include unalloc and whatnot
                        QList<QStandardItem*> sleuthList;
                        sleuthList << new QStandardItem(QString(fileRecordVector[i].name.c_str()));
                        //fprintf(stderr, "FileRecordVectorSize: %d - ObjectIDListSize: %d\n", fileRecordVector.size(), objectidlist.count());
                        sleuthList << new QStandardItem(QString::number(wombatdata->ReturnObjectID(wombatvariable.caseid, wombatvariable.evidenceid, (int)fileRecordVector[i].fileId)));
                        //sleuthList << new QStandardItem(QString::number((int)fileRecordVector[i].fileId));
                        sleuthList << new QStandardItem(fullPath);
                        sleuthList << new QStandardItem(QString::number(fileRecordVector[i].size));
                        sleuthList << new QStandardItem(QString::number(fileRecordVector[i].crtime));
                        //sleuthList << new QStandardItem(QString(ctime(((const time_t*)fileRecordVector[i].crtime))));
                        sleuthList << new QStandardItem(QString(fileRecordVector[i].md5.c_str()));
                        treeList.append(sleuthList);
                    }
                    for(int i = 0; i < (int)fileRecordVector.size(); i++)
                    {
                        // NEED TO EXPAND FOR OTHER ICONS
                        QStandardItem* tmpItem2 = ((QStandardItem*)treeList[i].first());
                        if(((TskFileRecord)fileRecordVector[i]).dirType == 3)
                        {
                            tmpItem2->setIcon(QIcon(":/basic/treefolder"));
                        }
                        else
                        {
                            tmpItem2->setIcon(QIcon(":/basic/treefile"));
                        }
                        if(((TskFileRecord)fileRecordVector[i]).parentFileId == 1)
                        {
                            fsNode->appendRow(treeList[i]);
                        }
                    }
                    for(int i=0; i < (int)fileRecordVector.size(); i++)
                    {
                        tmpRecord = fileRecordVector[i];
                        if(tmpRecord.parentFileId > 1)
                        {
                            ((QStandardItem*)treeList[tmpRecord.parentFileId-1].first())->appendRow(treeList[i]);
                        }
                    }
                    //END FILE ADD CODE
                }
            }
        }
    }
    emit ReturnImageNode(imageNode);
}

QString SleuthKitPlugin::GetFileContents(int fileID)
{
    TskFile *tmpFile = TskServices::Instance().getFileManager().getFile((uint64_t)fileID);
    fprintf(stderr, "TskFile ID: %i :: GetSize: %i :: Name: %s\n", tmpFile->getId(), tmpFile->getSize(), tmpFile->getName().c_str());
    char buffer[32768];
    //ssize_t bytesRead = 0;
    //bytesRead = tmpFile->read(buffer, 32768);
    QByteArray ba;
    QFile qFile("/home/pasquale/WombatForensics/tmpfiles/tmp.dat");
    qFile.open(QIODevice::ReadWrite);
    qFile.write((const char*)buffer, 32768);
    qFile.close();
    return "/home/pasquale/WombatForensics/tmpfiles/tmp.dat";
}
QString SleuthKitPlugin::GetFileTxtContents(int fileID)
{
    TskFile *tmpFile = TskServices::Instance().getFileManager().getFile((uint64_t)fileID);
    char buffer[32768];
    //ssize_t bytesRead = 0;
    //bytesRead = tmpFile->read(buffer, 32768);
    QFile qFile("/home/pasquale/WombatForensics/tmpfiles/tmp.txt");
    qFile.open(QIODevice::ReadWrite);
    qFile.write((const char*)buffer, 32768);
    qFile.close();
    return "/home/pasquale/WombatForensics/tmpfiles/tmp.txt";
}
