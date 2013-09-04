#include "wombatforensics.h"

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    threadpool = QThreadPool::globalInstance();
    wombatvariable = new WombatVariable();
    wombatcasedata = new WombatCaseDb(this);
    wombatprogresswindow = new ProgressWindow();
    connect(wombatprogresswindow, SIGNAL(HideProgressWindow(bool)), this, SLOT(HideProgressWindow(bool)), Qt::DirectConnection);
    wombatprogresswindow->setModal(false);
    wombatvariable->SetCaseID(0);
    wombatvariable->SetEvidenceID(0);
    wombatvariable->SetJobType(0);
    QString homePath = QDir::homePath();
    homePath += "/WombatForensics/";
    wombatvariable->SetSettingsPath(homePath + "settings");
    wombatvariable->SetDataPath(homePath + "data/");
    wombatvariable->SetCasesPath(homePath + "cases/");
    wombatvariable->SetTmpFilePath(homePath + "tmpfiles/");
    bool mkPath = (new QDir())->mkpath(wombatvariable->GetSettingsPath());
    if(mkPath == false)
        wombatcasedata->DisplayError(this, "2.0", "App Settings Folder Failed.", "App Settings Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvariable->GetDataPath());
    if(mkPath == false)
        wombatcasedata->DisplayError(this, "2.1", "App Data Folder Failed.", "Application Data Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvariable->GetCasesPath());
    if(mkPath == false)
        wombatcasedata->DisplayError(this, "2.2", "App Cases Folder Failed.", "App Cases Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatvariable->GetTmpFilePath());
    if(mkPath == false)
        wombatcasedata->DisplayError(this, "2.2", "App TmpFile Folder Failed.", "App TmpFile Folder was not created.");
    QString logPath = wombatvariable->GetDataPath() + "WombatLog.db";
    bool logFileExist = wombatcasedata->FileExists(logPath.toStdString());
    if(!logFileExist)
    {
        const char* errstring = wombatcasedata->CreateLogDB(logPath);
        if(strcmp(errstring, "") != 0)
            wombatcasedata->DisplayError(this, "1.0", "Log File Error", errstring);
    }
    QString tmpPath = wombatvariable->GetDataPath() + "WombatCase.db";
    bool doesFileExist = wombatcasedata->FileExists(tmpPath.toStdString());
    if(!doesFileExist)
    {
        const char* errstring = wombatcasedata->CreateCaseDB(tmpPath);
        if(strcmp(errstring, "") != 0)
            wombatcasedata->DisplayError(this, "1.0", "File Error", errstring);
    }
    else
    {
        const char* errstring = wombatcasedata->OpenCaseDB(tmpPath);
        if(strcmp(errstring, "") != 0)
            wombatcasedata->DisplayError(this, "1.1", "SQL", errstring);
    }
    if(wombatcasedata->ReturnCaseCount() == 0)
    {
        ui->actionOpen_Case->setEnabled(false);
    }
    else if(wombatcasedata->ReturnCaseCount() > 0)
    {
        ui->actionOpen_Case->setEnabled(true);
    }
    else
    {
        wombatcasedata->DisplayError(this, "1.0", "Case Count", "Invalid Case Count returned.");
    }
    //pluginFileNames.clear();
    //pluginFileNames = locatePlugins();
    //wombatplugins.clear();
    wombatvariable->SetPlugins(LoadPlugins());
    InitializeSleuthKit();
}
void WombatForensics::HideProgressWindow(bool checkedstate)
{
    ui->actionView_Progress->setChecked(checkedstate);
}
std::string WombatForensics::GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

void WombatForensics::InitializeSleuthKit()
{
    foreach(QObject* plugin, wombatvariable->GetPlugins())
    {
        SleuthKitInterface* isleuthkit = qobject_cast<SleuthKitInterface*>(plugin);
        if(isleuthkit)
        {
            fprintf(stderr, "sleuthkit exists");
        }
    }
}

/*
bool WombatForensics::isPluginLoaded(QString pluginFileName)
{
    int pluginexists = 0;
    QString fileName;
    foreach(fileName, pluginFileNames)
    {
        if(fileName.compare(pluginFileName) == 0)
        {
            loadPlugin(pluginFileName);
            pluginexists++;
        }
    }
    if(pluginexists <= 0)
    {
        fprintf(stderr, "string compare failed.");
        return false;
    }
    else
        return true;
}
QStringList WombatForensics::locatePlugins()
{
    QStringList tmpList;
    tmpList.clear();
    QString tmpPath = qApp->applicationDirPath();
    tmpPath += "/plugins/";
    QDir plugDir = QDir(tmpPath);

    foreach(QString fileName, plugDir.entryList(QDir::Files))
    {
        tmpList.append(plugDir.absoluteFilePath(fileName));
    }
    return tmpList;
}
*/
QList<QObject*> WombatForensics::LoadPlugins()
{
    QList<QObject*> tmplist;
    tmplist.clear();
    QString tmppath = qApp->applicationDirPath();
    tmppath += "/plugins/";
    QDir plugdir = QDir(tmppath);
    foreach(QString filename, plugdir.entryList(QDir::Files))
    {
        QPluginLoader loader(plugdir.absoluteFilePath(filename));
        QObject* plugin = loader.instance();
        if(plugin)
        {
            PopulateActions(plugin);
            PopulateTabWidgets(plugin);
            tmplist.append(plugin);
        }
    }
    ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
    ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());

    return tmplist;
}
/*
QObject* WombatForensics::loadPlugin(QString fileName)
{
   QPluginLoader loader(fileName);
    QObject* plugin = loader.instance();
    if(!loader.isLoaded())
        fprintf(stderr, "%s\n", loader.errorString().toStdString().c_str());
    if (plugin)
    {
        //populateActions(plugin);
        //populateTabWidgets(plugin);
        //setupSleuthKitProperties(plugin, wombatsettingspath, "tsk-config.xml");
        //setupSleuthKitLog(plugin, wombatdatapath, "tsk-log.txt", wombatvariable);
        //setupSleuthKitSchedulerQueue(plugin);
        //setupSleuthKitFileManager(plugin);
    }

    return plugin;
}
*/
void WombatForensics::PopulateActions(QObject *plugin)
{
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(plugin);
    if (iEvidence)
    {
        AddActions(plugin, iEvidence->evidenceActions(), iEvidence->evidenceActionIcons(), ui->mainToolBar, ui->menuEvidence, SLOT(alterEvidence()));
    }
}

void WombatForensics::PopulateTabWidgets(QObject *plugin)
{
    BasicToolsInterface *iBasicTools = qobject_cast<BasicToolsInterface *>(plugin);
    if(iBasicTools)
    {
        ui->fileViewTabWidget->addTab(iBasicTools->setupHexTab(), "Hex View");
        ui->fileViewTabWidget->addTab(iBasicTools->setupTxtTab(), "Text View");
        ui->fileInfoTabWidget->addTab(iBasicTools->setupDirTab(), "Directory List");
        ui->fileInfoTabWidget->addTab(iBasicTools->setupTypTab(), "File Type");
        SetupDirModel();
    }
}

void WombatForensics::SetupDirModel(void)
{
    wombatdirmodel = new QStandardItemModel();
    QStringList headerList;
    headerList << "Item ID" << "Name" << "Full Path" << "Size" << "Created" << "MD5 Hash";
    wombatdirmodel->setHorizontalHeaderLabels(headerList);
    QStandardItem *evidenceNode = wombatdirmodel->invisibleRootItem();
    currenttreeview = ui->fileInfoTabWidget->findChild<QTreeView *>("bt-dirtree");
    currenttreeview->setModel(wombatdirmodel);
    connect(currenttreeview, SIGNAL(clicked(QModelIndex)), this, SLOT(dirTreeView_selectionChanged(QModelIndex)));
}

void WombatForensics::AddActions(QObject *plugin, const QStringList &texts, const QStringList &icons, QToolBar *toolbar, QMenu *menu, const char *member, QActionGroup *actionGroup)
{
    for(int i = 0; i < texts.count(); i++)
    {
        QAction *action1 = new QAction(texts[i], plugin);
        QAction *action2 = new QAction(texts[i], plugin);
        action1->setIcon(QIcon(icons[i]));
        action2->setIcon(QIcon(icons[i]));
        connect(action1, SIGNAL(triggered()), this, member);
        connect(action2, SIGNAL(triggered()), this, member);
        toolbar->addAction(action1);
        menu->addAction(action2);

        if(actionGroup)
        {
            action1->setCheckable(true);
            action2->setCheckable(true);
            actionGroup->addAction(action1);
            actionGroup->addAction(action2);
        }
    }
}

/*
void WombatForensics::dialogClosed(QString file)
{
    QString evidenceFilePath = file;
    //wombatprogresswindow->show();
    fprintf(stderr, "Evidence FilePath: %s\n", evidenceFilePath.toStdString().c_str());
    if(evidenceFilePath != "")
    {
        wombatvariable->SetJobType(1); // add evidence
        // MIGHT BE AN ISSUE WHEN YOU OPEN MORE THAN 1 EVIDENCE ITEM... HAVE TO TEST IT OUT AND SEE WHAT HAPPENS
        QString evidenceName = evidenceFilePath.split("/").last();
        evidenceName += ".db";
        currentsleuthimages << setupSleuthKitImgDb(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath);
        setupSleuthKitBlackboard(sleuthkitplugin);
        wombatvariable->SetEvidenceID(wombatcasedata->InsertEvidence(evidenceName, evidenceFilePath, wombatvariable->GetCaseID()));
        wombatvariable->SetJobID(wombatcasedata->InsertJob(wombatvariable->GetJobType(), wombatvariable->GetCaseID(), wombatvariable->GetEvidenceID()));
        SleuthKitLogEntry(sleuthkitplugin, "Adding Evidence Started.");
        // UPDATE MESSAGETABLE(CASEID, EVIDENCEID, JOBID) WHEN JOB STARTS TO SHOW IT STARTED... WHICH SHOULD BE A SQL QUERY TO GET LOG ITEMS FOR IT.
        QString tmpString = evidenceName;
        tmpString += " - ";
        tmpString += QString::fromStdString(GetTime());
        QStringList tmpList;
        tmpList << tmpString << QString::number(wombatvariable->GetJobID());
        wombatprogresswindow->UpdateAnalysisTree(0, new QTreeWidgetItem(tmpList));
        wombatprogresswindow->UpdateFilesFound("0");
        wombatprogresswindow->UpdateFilesProcessed("0");
        wombatprogresswindow->UpdateAnalysisState("Adding Evidence to Database");
        //QList<QStringList> = wombatcasedata->ReturnLogMessages(wombatvariable->GetCaseID(), wombatvariable->GetEvidenceID(), wombatvariable->GetJobID());
        wombatprogresswindow->UpdateMessageTable("[INFO]", "Adding Evidence Started.");
        sleuthKitLoadEvidence(sleuthkitplugin, evidenceFilePath);
        //sleuthKitLoadEvidence(sleuthkitplugin, evidenceFilePath, wombatprogresswindow);
        // need to populate the directory tree entries
        QStandardItem* imageNode = GetCurrentImageDirectoryTree(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath.split("/").last());
        QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
        currentroot->appendRow(imageNode);
        currenttreeview->setModel(wombatdirmodel);
    }
}
*/

void WombatForensics::AlterEvidence()
{
    QAction *action = qobject_cast<QAction *>(sender());
    //EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(action->parent());
    if(action->text() == tr("Add Evidence"))
    {
        //QString evidenceFilePath = "/home/pasquale/Projects/TestImages/8-jpeg-search/8-jpeg-search.dd";
        QString evidenceFilePath = QFileDialog::getOpenFileName(this, tr("Select Evidence Item"), tr("./"));
        wombatprogresswindow->show();
        //QString evidenceFilePath = iEvidence->addEvidence();
        //QFileDialog *filedialog = new QFileDialog(0, "Select Evidence Item", "./");
        //filedialog->setFileMode(QFileDialog::ExistingFile);
        //filedialog->open(this, SLOT(dialogClosed(QString)));
        ///*
        fprintf(stderr, "Evidence FilePath: %s\n", evidenceFilePath.toStdString().c_str());
        if(evidenceFilePath != "")
        {
            wombatvariable->SetJobType(1); // add evidence
            // MIGHT BE AN ISSUE WHEN YOU OPEN MORE THAN 1 EVIDENCE ITEM... HAVE TO TEST IT OUT AND SEE WHAT HAPPENS
            QString evidenceName = evidenceFilePath.split("/").last();
            evidenceName += ".db";
            /*
            currentsleuthimages << setupSleuthKitImgDb(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath);
            setupSleuthKitBlackboard(sleuthkitplugin);
            wombatvariable->SetEvidenceID(wombatcasedata->InsertEvidence(evidenceName, evidenceFilePath, wombatvariable->GetCaseID()));
            wombatvariable->SetJobID(wombatcasedata->InsertJob(wombatvariable->GetJobType(), wombatvariable->GetCaseID(), wombatvariable->GetEvidenceID()));
            SleuthKitLogEntry(sleuthkitplugin, "Adding Evidence Started.");
            // UPDATE MESSAGETABLE(CASEID, EVIDENCEID, JOBID) WHEN JOB STARTS TO SHOW IT STARTED... WHICH SHOULD BE A SQL QUERY TO GET LOG ITEMS FOR IT.
            QString tmpString = evidenceName;
            tmpString += " - ";
            tmpString += QString::fromStdString(GetTime());
            QStringList tmpList;
            tmpList << tmpString << QString::number(wombatvariable->GetJobID());
            wombatprogresswindow->UpdateAnalysisTree(0, new QTreeWidgetItem(tmpList));
            wombatprogresswindow->UpdateFilesFound("0");
            wombatprogresswindow->UpdateFilesProcessed("0");
            wombatprogresswindow->UpdateAnalysisState("Adding Evidence to Database");
            //QList<QStringList> = wombatcasedata->ReturnLogMessages(wombatvariable->GetCaseID(), wombatvariable->GetEvidenceID(), wombatvariable->GetJobID());
            wombatprogresswindow->UpdateMessageTable("[INFO]", "Adding Evidence Started.");
            PluginRunner* prun = new PluginRunner(this, sleuthkitplugin, evidenceFilePath);
            prun->setAutoDelete(false);
            threadpool->start(prun);
            threadpool->waitForDone();
            //sleuthKitLoadEvidence(sleuthkitplugin, evidenceFilePath, wombatprogresswindow);
            // need to populate the directory tree entries
            //
            //QStandardItem* imageNode = GetCurrentImageDirectoryTree(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath.split("/").last());
            //QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
            //currentroot->appendRow(imageNode);
            //currenttreeview->setModel(wombatdirmodel);
            */
       }//*/
    }
    else if(action->text() == tr("Remove Evidence"))
    {
        //iEvidence->remEvidence(wombatvariable->GetCaseID());
    }
}

WombatForensics::~WombatForensics()
{
    //wombatprogresswindow->~ProgressWindow();
    //const char* errmsg = wombatcasedata->CloseCaseDB(); // this possibly caused glibc corrupted double-linked list
    //fprintf(stderr, "CloseDB: %s\n", errmsg);
    delete ui;
}

void WombatForensics::closeEvent(QCloseEvent* event)
{
    wombatprogresswindow->close();
    //const char* errmsg = wombatcasedata->CloseCaseDB();
    //fprintf(stderr, "CloseDB: %s\n", errmsg);
}

void WombatForensics::on_actionNew_Case_triggered()
{
    int ret = QMessageBox::Yes;
    // determine if a case is open
    if(wombatvariable->GetCaseID() > 0)
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
            wombatvariable->SetCaseID(wombatcasedata->InsertCase(text));

            QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += text;
            this->setWindowTitle(tmpTitle); // update application window.
            // make Cases Folder
            QString userPath = wombatvariable->GetCasesPath();
            userPath += QString::number(wombatvariable->GetCaseID());
            userPath += "-";
            userPath += text;
            userPath += "/";
            bool mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                wombatvariable->SetCaseDirPath(userPath);
                //currentcasedirpath = userPath;
            }
            else
            {
                wombatcasedata->DisplayError(this, "2.0", "Cases Folder Creation Failed.", "New Case folder was not created.");
            }
            userPath = wombatvariable->GetCaseDirPath();
            userPath += "evidence/";
            mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                wombatvariable->SetEvidenceDirPath(userPath);
                //currentcaseevidencepath = userPath;
            }
            else
            {
                wombatcasedata->DisplayError(this, "2.0", "Case Evidence Folder Creation Failed", "Failed to create case evidence folder.");
            }
            if(wombatcasedata->ReturnCaseCount() > 0)
            {
                ui->actionOpen_Case->setEnabled(true);
            }
            //evidenceplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libevidenceplugin.so");
            //basictoolsplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libbasictoolsplugin.so");
            //sleuthkitplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libsleuthkitplugin.so");
            ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
            ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());
        }
    }

}

void WombatForensics::on_actionOpen_Case_triggered()
{
    int ret = QMessageBox::Yes;
    // determine if a case is open
    if(wombatvariable->GetCaseID() > 0)
    {
        ret = QMessageBox::question(this, tr("Close Current Case"), tr("There is a case already open. Are you sure you want to close it?"), QMessageBox::Yes | QMessageBox::No);
    }
    if (ret == QMessageBox::Yes)
    {
        // open case here
        QStringList caseList;
        caseList.clear();
        // populate case list here
        caseList = wombatcasedata->ReturnCaseNameList();
        bool ok;
        QString item = QInputDialog::getItem(this, tr("Open Existing Case"), tr("Select the Case to Open: "), caseList, 0, false, &ok);
        if(ok && !item.isEmpty()) // open selected case
        {
            wombatvariable->SetCaseID(wombatcasedata->ReturnCaseID(item));
            QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += item;
            this->setWindowTitle(tmpTitle);
            QString userPath = wombatvariable->GetCasesPath();
            userPath += QString::number(wombatvariable->GetCaseID());
            userPath += "-";
            userPath += item;
            userPath += "/";
            bool mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                //currentcasedirpath = userPath;
                wombatvariable->SetCaseDirPath(userPath);
            }
            else
            {
                wombatcasedata->DisplayError(this, "2.0", "Cases Folder Check Failed.", "Existing Case folder did not exist.");
            }
            userPath = wombatvariable->GetCaseDirPath();
            //userPath = currentcasedirpath;
            userPath += "evidence/";
            mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                wombatvariable->SetEvidenceDirPath(userPath);
                //currentcaseevidencepath = userPath;
            }
            else
            {
                wombatcasedata->DisplayError(this, "2.0", "Case Evidence Folder Check Failed", "Case Evidence folder did not exist.");
            }
            //evidenceplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libevidenceplugin.so");
            //basictoolsplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libbasictoolsplugin.so");
            //sleuthkitplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libsleuthkitplugin.so");
            ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
            ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());
            // POPULATE APP WITH ANY OPEN IMAGES AND MINIMAL SETTINGS
            QString caseimage;
            QStringList caseimageList = wombatcasedata->ReturnCaseEvidence(wombatvariable->GetCaseID());
            foreach(caseimage, caseimageList)
            {
                //OpenSleuthKitImgDb(sleuthkitplugin, currentcaseevidencepath, caseimage);
                fprintf(stderr, "Case Image: %s\n", caseimage.toStdString().c_str());
                //setupSleuthKitBlackboard(sleuthkitplugin);
                //QStandardItem* imageNode = GetCurrentImageDirectoryTree(sleuthkitplugin, currentcaseevidencepath, caseimage.split("/").last());
                //QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
                //currentroot->appendRow(imageNode);
                //currenttreeview->setModel(wombatdirmodel);
            }
        }
    }
}

void WombatForensics::on_actionView_Progress_triggered(bool checked)
{
    if(!checked)
        wombatprogresswindow->hide();
    else
        wombatprogresswindow->show();
}
/*
void WombatForensics::setupSleuthKitProperties(QObject *plugin, QString settingsPath, QString configFileName)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemProperties(settingsPath, configFileName);
    }
}

void WombatForensics::SleuthKitLogEntry(QObject *plugin, QString logMsg)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
        iSleuthKit->LogEntry(logMsg);
}

void WombatForensics::setupSleuthKitLog(QObject *plugin, QString dataPath, QString logFileName, WombatVariable *wombatVariable)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemLog(dataPath, logFileName, wombatprogresswindow, wombatVariable);
    }
}
QString WombatForensics::setupSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        return iSleuthKit->SetupImageDatabase(imgDBPath, evidenceFilePath);
    }
    else
        return "";
}

void WombatForensics::OpenSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->OpenImageDatabase(imgDBPath, evidenceFilePath);
    }
}
void WombatForensics::setupSleuthKitBlackboard(QObject *plugin)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemBlackboard();
    }
}
void WombatForensics::setupSleuthKitSchedulerQueue(QObject *plugin)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemSchedulerQueue();
    }
}
void WombatForensics::setupSleuthKitFileManager(QObject *plugin)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemFileManager();
    }
}
//void WombatForensics::sleuthKitLoadEvidence(QObject *plugin, QString evidencePath, ProgressWindow* progressWindow)
void WombatForensics::sleuthKitLoadEvidence(QObject *plugin, QString evidencePath)
{
    wombatprogresswindow->show();
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
       //iSleuthKit->OpenEvidence(evidencePath, wombatprogresswindow);
    }
}
*/
/*
QStandardItem* WombatForensics::GetCurrentImageDirectoryTree(QObject *plugin, QString imageDbPath, QString imageName)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        return iSleuthKit->GetCurrentImageDirectoryTree(imageDbPath, imageName);
    }
    else
        return NULL;
}
void WombatForensics::dirTreeView_selectionChanged(const QModelIndex &index)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(sleuthkitplugin);
    if(iSleuthKit)
    {
        QString tmptext = index.sibling(index.row(), 0).data().toString();
        fprintf(stderr, "Text: %s\n", tmptext.toStdString().c_str());
        QString tmpFilePath = iSleuthKit->GetFileContents(tmptext.toInt());
        LoadHexViewer(tmpFilePath);
        QString asciiText = iSleuthKit->GetFileTxtContents(tmptext.toInt());
        LoadTxtViewer(asciiText);
    }
}
void WombatForensics::LoadHexViewer(QString tmpFilePath)
{
    BasicToolsInterface *iBasicTools = qobject_cast<BasicToolsInterface *>(basictoolsplugin);
    if(iBasicTools)
    {
        iBasicTools->LoadHexModel(tmpFilePath);
    }
}
void WombatForensics::LoadTxtViewer(QString asciiText)
{
    BasicToolsInterface *iBasicTools = qobject_cast<BasicToolsInterface *>(basictoolsplugin);
    if(iBasicTools)
    {
        iBasicTools->LoadTxtContent(asciiText);
    }
}
*/
