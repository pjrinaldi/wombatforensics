#include "sleuthkit.h"

void SleuthKitPlugin::SetupSystemProperties(QString settingsPath, QString configFilePath)
{
    QString tmpPath = settingsPath;
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
            xml.writeCharacters(tmpPath);
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
    try
    {
        SetSystemProperty(TskSystemProperties::OUT_DIR, settingsPath.toStdString());
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
        TskServices::Instance().setFileManager(TskFileManagerImpl::instance());
        fprintf(stderr, "Loading File Manager was successful!\n");
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading FileManager: %s\n", ex.message().c_str());
    }
}
void SleuthKitPlugin::OpenEvidence(QString evidencePath)
{
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
}

QStandardItemModel* SleuthKitPlugin::GetCurrentImageDirectoryTree()
{
    std::vector<uint64_t> fileidVector;
    std::vector<TskFileRecord> fileRecordVector;
    fileidVector = imgdb->getFileIds();
    TskFileRecord tmpRecord;
    QStandardItem *tmpItem;
    int ret;
    uint64_t tmpId;
    QStandardItemModel *model = new QStandardItemModel();
    QStandardItem *rootNode = model->invisibleRootItem();
    QList<QStandardItem*> itemList;
    foreach(tmpId, fileidVector)
    {
        ret = imgdb->getFileRecord(tmpId, tmpRecord);
        fileRecordVector.push_back(tmpRecord);
    }
    for(int i=0; i < (int)fileRecordVector.size(); i++)
    {
        itemList.append(new QStandardItem(QString(fileRecordVector[i].name.c_str())));
    }
    for(int i = 0; i < (int)fileRecordVector.size(); i++)
    {
        tmpItem = ((QStandardItem*)itemList[i]);
        if(((TskFileRecord)fileRecordVector[i]).dirType == 3)
        {
            tmpItem->setIcon(QIcon(":/treefolder"));
        }
        else
        {
            tmpItem->setIcon(QIcon(":/treefile"));
        }
        if(((TskFileRecord)fileRecordVector[i]).parentFileId == 1)
        {
            rootNode->appendRow(tmpItem);
        }
    }
    for(int i=0; i < (int)fileRecordVector.size(); i++)
    {
        tmpRecord = fileRecordVector[i];
        tmpItem = itemList[i];
        if(tmpRecord.parentFileId > 1)
        {
            //fprintf(stderr, "itemList[%d]->appendrow(itemList[%d])\n", tmpRecord.parentFileId, i);
            ((QStandardItem*)itemList[tmpRecord.parentFileId-1])->appendRow(tmpItem);
        }
    }
    return model;
}
