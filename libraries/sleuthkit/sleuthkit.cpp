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
void SleuthKitPlugin::SetupImageDatabase(QString imgDBPath, QString evidenceFilePath)
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
            delete task;
        }
        catch(TskException &ex)
        {
            fprintf(stderr, "TskException: %s\n", ex.message().c_str());
        }
    }
    if(filePipeline && !filePipeline->isEmpty())
    {
        filePipeline->logModuleExecutionTimes();
    }
}

QStandardItemModel* SleuthKitPlugin::GetCurrentImageDirectoryTree()
{
    std::vector<uint64_t> fileidVector;
    std::vector<TskFileRecord> fileRecordVector;
    fileidVector = imgdb->getFileIds();
    TskFileRecord tmpRecord;
    // implement custom qstandarditem which has a fileid field
    //QStandardItem *tmpItem;
    SleuthFileItem *tmpItem2;
    int ret;
    uint64_t tmpId;
    QStandardItemModel *model = new QStandardItemModel();
    QStandardItem *rootNode = model->invisibleRootItem();
    //QList<QStandardItem*> itemList;
    QList<SleuthFileItem*> sleuthList;
    foreach(tmpId, fileidVector)
    {
        ret = imgdb->getFileRecord(tmpId, tmpRecord);
        //fprintf(stderr, "FileId: %d\n", tmpId);
        fileRecordVector.push_back(tmpRecord);
        //fprintf(stderr, "File Id: %d - FileName: %s - Parent FileId: %d - dirtype: %d\n", tmpRecord.fileId, tmpRecord.name.c_str(), tmpRecord.parentFileId, tmpRecord.dirType);
    }
    //fprintf(stderr, "record vector size: %d\n", fileRecordVector.size());
    for(int i=0; i < (int)fileRecordVector.size(); i++)
    {
        //itemList.append(new QStandardItem(QString(fileRecordVector[i].name.c_str())));
        sleuthList.append(new SleuthFileItem(QString(fileRecordVector[i].name.c_str()), (int)fileRecordVector[i].fileId));
    }
    for(int i = 0; i < (int)fileRecordVector.size(); i++)
    {
        //tmpItem = ((QStandardItem*)itemList[i]);
        tmpItem2 = ((SleuthFileItem*)sleuthList[i]);

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
            rootNode->appendRow(tmpItem2);
        }
    }
    for(int i=0; i < (int)fileRecordVector.size(); i++)
    {
        tmpRecord = fileRecordVector[i];
        //tmpItem = itemList[i];
        tmpItem2 = sleuthList[i];
        if(tmpRecord.parentFileId > 1)
        {
            //fprintf(stderr, "itemList[%d]->appendrow(itemList[%d])\n", tmpRecord.parentFileId-1, i);
            //fprintf(stderr, "sleuthListId %d\n", sleuthList[i]->GetSleuthFileID());
            //((QStandardItem*)itemList[tmpRecord.parentFileId-1])->appendRow(tmpItem);
            ((SleuthFileItem*)sleuthList[tmpRecord.parentFileId-1])->appendRow(tmpItem2);
        }
    }
    return model;
}
char SleuthKitPlugin::GetFileContents(SleuthFileItem* fileItem)
{
    // TskFile *pFile
    //  uint8_t byte = 0;
    //  long byteCounts[256];
    //  memset(byteCounts, 0, sizeof(long) * 256);
    //  long totalBytes = 0;
    //  char buffer[FILE_BUFFER_SIZE];
    //  ssize_t bytesRead = 0;
    //  do
    //  {
    //      memset(buffer, 0, FILE_BUFFER_SIZE);
    //      bytesRead = pFile->read(buffer, FILE_BUFFER_SIZE);
    //      if (bytesRead > 0)
    //      {
    //          for (int i = 0; i < bytesRead; ++i)
    //          {
    //              byte = static_cast<uint8_t>(buffer[i]);
    //              byteCounts[byte]++;
    //          }
    //          totalBytes += bytesRead;
    //      }
    //  }
    //
    //
    //
    //fileManager = (TskFileManagerImpl*)TskServices::Instance().getFileManager();
    TskFileTsk *tmpFile = (TskFileTsk*)TskServices::Instance().getFileManager().getFile((uint64_t)fileItem->GetSleuthFileID());
    //TskFile *tmpFile = TskServices::Instance().getFileManager().getFile((uint64_t)fileItem->GetSleuthFileID());
    fprintf(stderr, "fileItem ID: %i\n", fileItem->GetSleuthFileID());
    fprintf(stderr, "TskFile ID: %i :: GetSize: %i :: Name: %s\n", tmpFile->getId(), tmpFile->getSize(), tmpFile->getName().c_str());
    //tmpFile->save();
    //tmpFile->save();
    //tmpFile->open();
    //TskServices::Instance().getFileManager().saveFile(tmpFile);
    char buffer[8192];
    return buffer[8192];
    //tmpFile->open();
    //tmpFile->save();
    //tmpFile->close();
    //fileManager->saveFile(tmpFile);
    //TskFile *tmpFile = TskServices::Instance().getFileManager().getFile((uint64_t)fileItem->GetSleuthFileID());
    //TskServices::Instance().getFileManager().saveFile(tmpFile);
    //uint8_t byte = 0;
    //long byteCounts[256];
    //memset(byteCounts, 0, sizeof(long) *256);
    //long totalBytes = 0;
    // TskImageFileTsk tmpfile;
    //int ret = (TskServices::Instance().getImageFile()).openFile((uint64_t)fileItem->GetSleuthFileID());
    //tmpfile = TskServices::Instance().getImageFile();
    //std::auto_ptr<TskImageFile> tmpFile = TskServices::Instance().getImageFile();
    //char buffer[8192];
    //ssize_t bytesRead = 0;
    //memset(buffer, 0, 8192);
    //bytesRead = tmpFile->read(buffer, 8192);
    //return buffer[8192];
    //int ret2 = (TskServices::Instance().getImageFile()).readFile(ret, 
    //QString
    //int tmpId = imgdb->getFileIds(" where name = ? limit 1;")
    // get fileid using filename.
    // openfile(fileid)
    // readfile(fileid)
    // closefile(fileid)
    // return buffe
}
