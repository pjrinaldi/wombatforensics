#include "wombatforensics.h"
#include "ui_wombatforensics.h"
#include <QPluginLoader>

// static plugins are always available, dynamic are not.

WombatForensics::WombatForensics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    currentcaseid = -1;
    //loadPlugins();
    QDir testDir = QDir(qApp->applicationDirPath());
    testDir.mkdir("data");
    wombatCaseData = new WombatCaseDb("WombatData.db"); // create db.
    if(wombatCaseData->ReturnCaseCount() == 0)
    {
        ui->actionOpen_Case->setEnabled(FALSE);
        ui->actionOpen_Case_2->setEnabled(FALSE);
    }
    else if(wombatCaseData->ReturnCaseCount() > 0)
    {
        ui->actionOpen_Case->setEnabled(TRUE);
        ui->actionOpen_Case_2->setEnabled(TRUE);
    }
    else
    {
        fprintf(stderr, "Case count is < 0.");
    }
    //wInstance->set;
    //TskFunctions *testcase = new TskFunctions();
    //testcase->SetupTskFramework();
    // determine if a cases db exist and if any cases are open, otherwise disable open existing case
}

void WombatForensics::loadPlugins()
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
    {
        populateMenus(plugin);
        populateToolBox(plugin);
    }

    pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#endif
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            populateMenus(plugin);
            pluginFileNames += fileName;
            populateToolBox(plugin);
        }
    }
    ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
    // enable menu's here
    /*
        brushMenu->setEnabled(!brushActionGroup->actions().isEmpty());
        shapesMenu->setEnabled(!shapesMenu->actions().isEmpty());
        filterMenu->setEnabled(!filterMenu->actions().isEmpty());
     */
}

void WombatForensics::populateMenus(QObject *plugin)
{
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(plugin);
    if (iEvidence)
    {
        addToMenu(plugin, iEvidence->evidenceMenuActions(), ui->menuEvidence, SLOT(alterEvidence()));
        // add items to the toolbar as well
    }
    /*
        BrushInterface *iBrush = qobject_cast<BrushInterface *>(plugin);
        if (iBrush)
            addToMenu(plugin, iBrush->brushes(), brushMenu, SLOT(changeBrush()),
                      brushActionGroup);

        ShapeInterface *iShape = qobject_cast<ShapeInterface *>(plugin);
        if (iShape)
            addToMenu(plugin, iShape->shapes(), shapesMenu, SLOT(insertShape()));

        FilterInterface *iFilter = qobject_cast<FilterInterface *>(plugin);
        if (iFilter)
            addToMenu(plugin, iFilter->filters(), filterMenu, SLOT(applyFilter()));
     */
}

void WombatForensics::populateToolBox(QObject *plugin)
{
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(plugin);
    if (iEvidence)
    {
        ui->toolBox->addItem(iEvidence->setupToolBox(), iEvidence->toolboxViews()[0]);
    }
}

void WombatForensics::addToMenu(QObject *plugin, const QStringList &texts, QMenu *menu, const char *member, QActionGroup *actionGroup)
{
    foreach (QString text, texts)
    {
        QAction *action = new QAction(text, plugin);
        connect(action, SIGNAL(triggered()), this, member);
        menu->addAction(action);

        if (actionGroup)
        {
            action->setCheckable(true);
            actionGroup->addAction(action);
        }
    }
}

void WombatForensics::alterEvidence()
{
    QAction *action = qobject_cast<QAction *>(sender());
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(action->parent());
    if(action->text() == tr("Add Evidence"))
        iEvidence->addEvidence(currentcaseid);
    else if(action->text() == tr("Remove Evidence"))
        iEvidence->remEvidence(currentcaseid);
    /*
    FilterInterface *iFilter =
            qobject_cast<FilterInterface *>(action->parent());

    const QImage image = iFilter->filterImage(action->text(), paintArea->image(),
                                              this);
    paintArea->setImage(image);
     */
}

WombatForensics::~WombatForensics()
{
    delete ui;
}

void WombatForensics::on_actionNew_Case_triggered()
{
    int ret = QMessageBox::Yes;
    // determine if a case is open
    if(currentcaseid > 0) // a case is open, provide action dialog first
    {
        ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
    }
    if (ret == QMessageBox::Yes)
    {
        // create new case here
        bool ok;
        QString text = QInputDialog::getText(this, tr("New Case Creation"), "Enter Case Name: ", QLineEdit::Normal, "", &ok);
        if(ok && !text.isEmpty())
        {
            SqlWrapper *sqlObject = new SqlWrapper(sqlStatement, "2.1", "WombatData.db");
            sqlObject->PrepareSql("INSERT INTO cases (casename) VALUES(?);");
            sqlObject->BindValue(1, text.toStdString().c_str());
            sqlObject->StepSql();
            currentcaseid = sqlObject->ReturnLastInsertRowID();
            sqlObject->FinalizeSql();
            sqlObject->CloseSql();

            QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += text;
            this->setWindowTitle(tmpTitle);

            if(wombatCaseData->ReturnCaseCount() > 0)
            {
                ui->actionOpen_Case->setEnabled(TRUE);
                ui->actionOpen_Case_2->setEnabled(TRUE);
            }
        }
    }

}

void WombatForensics::on_actionOpen_Case_triggered()
{
    int ret = QMessageBox::Yes;
    // determine if a case is open
    if(currentcaseid > 0) // a case is open, provide action dialog first
    {
        ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
    }
    if (ret == QMessageBox::Yes)
    {
        // open case here
        QStringList caseList;
        caseList.clear();
        SqlWrapper *sqlObject = new SqlWrapper(sqlStatement, "2.2", "WombatData.db");
        sqlObject->PrepareSql("SELECT casename FROM cases ORDER BY caseid;");
        while(sqlObject->StepSql() == SQLITE_ROW) // step through the sql
        {
            caseList.push_back(sqlObject->ReturnText(0));
        }
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();

        bool ok;
        QString item = QInputDialog::getItem(this, tr("Open Existing Case"), tr("Select the Case to Open: "), caseList, 0, false, &ok);
        if(ok && !item.isEmpty())
        {
            // open case here (store case id)
            SqlWrapper *sqlObject = new SqlWrapper(sqlStatement, "2.3", "WombatData.db");
            sqlObject->PrepareSql("SELECT caseid FROM cases WHERE casename = ?;");
            sqlObject->BindValue(1, item.toStdString().c_str());
            sqlObject->StepSql();
            currentcaseid = sqlObject->ReturnInt(0);
            sqlObject->FinalizeSql();
            sqlObject->CloseSql();

            QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += item;
            this->setWindowTitle(tmpTitle);
        }
    }
}
/*
    // open case here
    QString evidenceFile = QFileDialog::getOpenFileName(this, "Select Evidence Item", "./");
    ui->testLabel->setText(evidenceFile);

    // BEGIN FRAMEWORK TEST CODE
    try
    {
        TskSystemPropertiesImpl *systemProperties = new TskSystemPropertiesImpl();
        systemProperties->initialize("./framework_config.xml");
        TskServices::Instance().setSystemProperties(*systemProperties);
    }
    catch (TskException& ex)
    {
        fprintf(stderr, "Loading Framework Config: %s\n", ex.message().c_str());
    }
    QString outDirPath;
    outDirPath = "./tmpoutput";
    SetSystemProperty(TskSystemProperties::OUT_DIR, outDirPath.toStdString().c_str());
    makeDir(outDirPath.toStdString().c_str());
    makeDir(GetSystemProperty(TskSystemProperties::SYSTEM_OUT_DIR).c_str());
    makeDir(GetSystemProperty(TskSystemProperties::MODULE_OUT_DIR).c_str());
    std::string logDir = GetSystemProperty(TskSystemProperties::LOG_DIR);
    makeDir(logDir.c_str());
    // LOG TEST
    struct tm * newtime;
    time_t aclock;

    time(&aclock);
    newtime = localtime(&aclock);
    char filename[MAX_BUFF_LENGTH];
    snprintf(filename, MAX_BUFF_LENGTH, "/log_%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.txt", newtime->tm_year + 1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    logDir.append(filename);
    std::auto_ptr<Log> log(NULL);

    log = std::auto_ptr<Log>(new Log());

    log->open(logDir.c_str());
    TskServices::Instance().setLog(*log);
    //SetSystemProperty(TskSystemProperties::OUT_DIR, outDirPath.toStdString().c_str());
    std::auto_ptr<TskImgDB> pImgDB(NULL);
    pImgDB = std::auto_ptr<TskImgDB>(new TskImgDBSqlite(outDirPath.toStdString().c_str()));
    if(pImgDB->initialize() != 0)
    {
        fprintf(stderr, "Sqlite Initialization failed.");
    }

    TskServices::Instance().setImgDB(*pImgDB);
    TskServices::Instance().setBlackboard((TskBlackboard &) TskDBBlackboard::instance());
    SetSystemProperty(TskSystemProperties::PIPELINE_CONFIG_FILE, "./pipeline_config.xml");
    TskSchedulerQueue scheduler;
    TskServices::Instance().setScheduler(scheduler);
    TskServices::Instance().setFileManager(TskFileManagerImpl::instance());
    TskImageFileTsk imageFileTsk;
    TskArchiveExtraction::ExtractorPtr containerExtractor = TskArchiveExtraction::createExtractor(evidenceFile.toStdWString());
    if(containerExtractor.isNull())
    {
        if(imageFileTsk.open(evidenceFile.toStdString()))
        {
            fprintf(stderr, "Error opening image at: %s\n", evidenceFile.toStdString().c_str());
        }
        TskServices::Instance().setImageFile(imageFileTsk);
    }
    TskPipelineManager pipelineMgr;
    TskPipeline *filePipeline;
    try
    {
        filePipeline = pipelineMgr.createPipeline(TskPipelineManager::FILE_ANALYSIS_PIPELINE);
    }
    catch (const TskException &e)
    {
        fprintf(stderr, "Error creating file analysis pipeline: %s\n", e.message().c_str());
    }
    TskPipeline *reportPipeline;
    try
    {
        reportPipeline = pipelineMgr.createPipeline(TskPipelineManager::POST_PROCESSING_PIPELINE);
    }
    catch(const TskException &e)
    {
        reportPipeline = NULL;
    }
    if ((filePipeline == NULL) && (reportPipeline == NULL))
    {
        std::stringstream msg;
        msg << "No pipelines configured.  Stopping";
        LOGERROR(msg.str());
        exit(1);
    }
// THIS IS WHERE THE DB IS POPULATED WITH THE IMAGE FILES, VOLUME, FS, ETC... NO PROCESSING DONE
    if(!containerExtractor.isNull())
    {
        if(containerExtractor->extractFiles() != 0)
        {
            std::wstringstream msg;
            msg << L"Error adding archived file info to db";
            LOGERROR(msg.str());
        }
    }
    else
    {
        if(imageFileTsk.extractFiles() != 0)
        {
            std::wstringstream msg;
            msg << L"Error adding file system info to db";
            LOGERROR(msg.str());
        }
    }
    // THIS IS THE ANALYSIS PIPELINE USING THE MODULES
/*
    TskSchedulerQueue::task_struct *task;
    while ((task = scheduler.nextTask()) != NULL)
    {
        try
        {
            if (task->task == Scheduler::FileAnalysis && filePipeline && !filePipeline->isEmpty())
            {
                filePipeline->run(task->id);
            }
            else
            {
                std::stringstream msg;
                msg << "WARNING: Skipping Task: " << task->task;
                LOGWARN(msg.str());
            }
            delete task;
        }
        catch(...)
        {
            // Error has been logged already
        }
    }
*/
    // END FRAMEWORK TEST CODE
/*
    // BEGIN POPULATE DIRECTORY TREE
    std::vector<uint64_t> fileidVector;
    std::vector<TskFileRecord> fileRecordVector;
    fileidVector = pImgDB->getFileIds();
    TskFileRecord tmpRecord;
    QStandardItem *tmpItem;
    //fileRecordVector = pImgDB->getFileRecords(";");
    int ret;
    u_int64_t tmpId;
    //QList<QTreeWidgetItem*> rootItemList;
    QStandardItemModel *model = new QStandardItemModel();
    QStandardItem *rootNode = model->invisibleRootItem();
    QList<QStandardItem*> itemList;
    foreach(tmpId, fileidVector)
    {
        //fprintf(stderr, "FileId: %d\n", tmpId);
        ret = pImgDB->getFileRecord(tmpId, tmpRecord);
        fileRecordVector.push_back(tmpRecord);
        if(ret == 0)
        {
        }
        //fprintf(stderr, "FileId: %d - FileName: %s - Parent FileId: %d - dirtype: %d\n", tmpRecord.fileId, tmpRecord.name.c_str(), tmpRecord.parentFileId, tmpRecord.dirType);
    }
    //fprintf(stderr, "record vector size: %d\n", fileRecordVector.size());
    for(int i = 0; i < (int)fileRecordVector.size(); i++)
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
    ui->directoryTreeView->setModel(model);

    // BEGIN FILE TYPE SIGNATURE TREE CREATION
    // vector<int> attrTypeVector;
    // attrTypeVector = pImgDB->findAttributeTypes(62); // return unique artif

    // END FILE TYPE SIGNATURE TREE CREATION
    */
// FRAMEWORK TEST MAKE DIR CODE
/*
void WombatForensics::makeDir(const char *dir)
{
    Poco::File path(dir);
    try
    {
        if (!path.createDirectory())
        {
            fprintf(stderr, "Error creating directory: %s\n", dir);
            //return 1;
        }
    }
    catch (const Poco::Exception &ex)
    {
        //std::stringstream msg;
        //msg << "Error creating directory: " << dir << " Poco exception: " << ex.displayText();
        //fprintf(stderr, "%s\n", msg.str().c_str());
        //return 1;
        fprintf(stderr, "Error Creating Directory");
    }
    //return 0;
}
/**
 * Logs all messages to a log file and prints
 * error messages to STDERR
 */
/*
class StderrLog : public Log
{
public:
    StderrLog() : Log() {
    }

    ~StderrLog() {
    }

    void log(Channel a_channel, const std::wstring &a_msg)
    {
        Log::log(a_channel, a_msg);
        if (a_channel != Error) {
            return;
        }
        fprintf(stderr, "%S\n", a_msg.c_str());
    }
};

void WombatForensics::on_toolBox_currentChanged(int index)
{
    if (index == 0)
    {
        fprintf(stderr, "Directory Listing Selected");
    }
    else if(index == 1)
    {
        fprintf(stderr, "File Type Selected");
    }
}*/
