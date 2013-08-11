#include "sleuthkit.h"

void SleuthKitPlugin::SetupSystemProperties(QString settingsPath, QString configFilePath)
{
    QString tmpPath = settingsPath;
    tmpPath += "/";
    tmpPath += configFilePath;
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
            xml.writeCharacters(settingsPath);
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
    tmpPath = settingsPath;
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
void SleuthKitPlugin::SetupSystemLog(QString dataPath, QString logFilePath)
{
    QString tmpPath = dataPath;
    tmpPath += logFilePath;
    try
    {
        log = std::auto_ptr<Log>(new Log());
        log->open(tmpPath.toStdString().c_str());
        TskServices::Instance().setLog(*log);
        fprintf(stderr, "Loading Log File was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Log File: %s\n", ex.message().c_str());
    }
}
QString SleuthKitPlugin::SetupImageDatabase(QString imgDBPath, QString evidenceFilePath)
{
    QString evidenceFileName = evidenceFilePath.split("/").last();
    evidenceFileName += ".db";

    try
    {
        imgdb = std::auto_ptr<TskImgDB>(new TskImgDBSqlite(imgDBPath.toStdString().c_str(), evidenceFileName.toStdString().c_str()));
        if(imgdb->initialize() != 0)
            fprintf(stderr, "Error initializing db\n");
        else
        {
            fprintf(stderr, "DB was Initialized Successfully!\n");
        }
        TskServices::Instance().setImgDB(*imgdb);
        fprintf(stderr, "Loading ImageDB was Successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading ImageDB: %s\n", ex.message().c_str());
    }
    return evidenceFilePath;
}

void SleuthKitPlugin::OpenImageDatabase(QString imgDBPath, QString evidenceFilePath)
{
    QString evidenceFileName = evidenceFilePath.split("/").last();
    evidenceFileName += ".db";

    try
    {
        imgdb = std::auto_ptr<TskImgDB>(new TskImgDBSqlite(imgDBPath.toStdString().c_str(), evidenceFileName.toStdString().c_str()));
        if(imgdb->open() != 0)
            fprintf(stderr, "Error opening db\n");
        else
        {
            fprintf(stderr, "DB was Opened Successfully!\n");
        }
        TskServices::Instance().setImgDB(*imgdb);
        fprintf(stderr, "Loading ImageDB was Successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading ImageDB: %s\n", ex.message().c_str());
    }
}

void SleuthKitPlugin::SetupSystemBlackboard()
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
void SleuthKitPlugin::SetupSystemSchedulerQueue()
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
void SleuthKitPlugin::SetupSystemFileManager()
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
void SleuthKitPlugin::OpenEvidence(QString evidencePath)
{
    TskPipelineManager pipelineMgr;
    TskPipeline *filePipeline;
    try
    {
        filePipeline = pipelineMgr.createPipeline(TskPipelineManager::FILE_ANALYSIS_PIPELINE);
    }
    catch(const TskException &ex)
    {
        fprintf(stderr, "Error creating file analysis pipeline: %s\n", ex.message().c_str());
    }

    TskImageFileTsk imagefiletsk;
    try
    {
        imagefiletsk.open(evidencePath.toStdString());
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
    TskSchedulerQueue::task_struct *task;
    while((task = scheduler.nextTask()) != NULL)
    {
        try
        {
            if(task->task == Scheduler::FileAnalysis && filePipeline && !filePipeline->isEmpty())
            {
                filePipeline->run(task->id);
            }
            else
            {
                fprintf(stderr, "Skipping task: %s\n", task->task);
            }
            //delete task;
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "TskException: %s\n", ex.message().c_str());
        }
    }
    delete task;
    if(filePipeline && !filePipeline->isEmpty())
    {
        filePipeline->logModuleExecutionTimes();
    }
}

QStandardItem* SleuthKitPlugin::GetCurrentImageDirectoryTree(QString imageName)
{
    std::vector<uint64_t> fileidVector;
    std::vector<TskFileRecord> fileRecordVector;
    std::list<TskVolumeInfoRecord> volRecordList;
    std::list<TskFsInfoRecord> fsInfoRecordList;
    fileidVector = imgdb->getFileIds();
    TskFileRecord tmpRecord;
    TskVolumeInfoRecord volRecord;
    TskFsInfoRecord fsInfoRecord;
    QStandardItem *imageNode = new QStandardItem(imageName);
    int ret;
    uint64_t tmpId;
    // also need to get the partitions and volumes as nodes.
    ret = imgdb->getVolumeInfo(volRecordList);
    foreach(volRecord, volRecordList)
    {
        // if volflag = 0, get description
        // if volflag = 1, list as unallocated
        fprintf(stderr, "Vol Description: %s - VolFlags: %d\n", volRecord.description.c_str(), volRecord.flags);
        if(volRecord.flags == 0)
        {
            QStandardItem *volNode = new QStandardItem(QString(volRecord.description));
        }
        else if(volRecord.flags == 1)
        {
            QStandardItem *volNode = new QStandardItem("unallocated space");
        }
        else if(volRecord.flags == 2)
        {
            QStandardItem *volNode = new QStandardItem(QString(volRecord.description));
        }
        else
        {
            // don't display anything
        }
    }
    QList<QList<QStandardItem*> > treeList;
    foreach(tmpId, fileidVector)
    {
        ret = imgdb->getFileRecord(tmpId, tmpRecord);
        fileRecordVector.push_back(tmpRecord);
    }
    for(int i=0; i < (int)fileRecordVector.size(); i++)
    {
        QString fullPath = "Image Name/Partition #/Volume Name[FSTYPE]/[root]/";
        fullPath += QString(fileRecordVector[i].fullPath.c_str());
        QList<QStandardItem*> sleuthList;
        sleuthList << new QStandardItem(QString::number((int)fileRecordVector[i].fileId));
        sleuthList << new QStandardItem(QString(fileRecordVector[i].name.c_str()));
        sleuthList << new QStandardItem(fullPath);
        sleuthList << new QStandardItem(QString::number(fileRecordVector[i].size));
        sleuthList << new QStandardItem(QString::number(fileRecordVector[i].crtime));
        //sleuthList << new QStandardItem(QString(ctime(((const time_t*)fileRecordVector[i].crtime))));
        sleuthList << new QStandardItem(QString(fileRecordVector[i].md5.c_str()));
        treeList.append(sleuthList);
    }
    for(int i = 0; i < (int)fileRecordVector.size(); i++)
    {
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
            imageNode->appendRow(treeList[i]);
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
    return imageNode;
}

QString SleuthKitPlugin::GetFileContents(int fileID)
{
    TskFile *tmpFile = TskServices::Instance().getFileManager().getFile((uint64_t)fileID);
    fprintf(stderr, "TskFile ID: %i :: GetSize: %i :: Name: %s\n", tmpFile->getId(), tmpFile->getSize(), tmpFile->getName().c_str());
    char buffer[32768];
    ssize_t bytesRead = 0;
    bytesRead = tmpFile->read(buffer, 32768);
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
    ssize_t bytesRead = 0;
    bytesRead = tmpFile->read(buffer, 32768);
    QFile qFile("/home/pasquale/WombatForensics/tmpfiles/tmp.txt");
    qFile.open(QIODevice::ReadWrite);
    qFile.write((const char*)buffer, 32768);
    qFile.close();
    return "/home/pasquale/WombatForensics/tmpfiles/tmp.txt";
}
