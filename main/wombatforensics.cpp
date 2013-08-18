#include "interfaces.h"
#include "wombatforensics.h"
#include "wombatcasedb.h"

#include <QPluginLoader>

WombatForensics::WombatForensics(QWidget *parent) : QMainWindow(parent), ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    wombatCaseData = new WombatCaseDb(this);
    wombatprogresswindow = new ProgressWindow();
    wombatprogresswindow->setModal(false);
    currentcaseid = -1;
    currentimageid = 0;
    currentanalysistype = 0;
    QString homePath = QDir::homePath();
    homePath += "/WombatForensics/";
    wombatsettingspath = homePath + "settings";
    wombatdatapath = homePath + "data/";
    wombatcasespath = homePath + "cases/";
    wombattmpfilepath = homePath + "tmpfiles/";
    bool mkPath = (new QDir())->mkpath(wombatsettingspath);
    if(mkPath == false)
        wombatCaseData->DisplayError(this, "2.0", "App Settings Folder Failed.", "App Settings Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatdatapath);
    if(mkPath == false)
        wombatCaseData->DisplayError(this, "2.1", "App Data Folder Failed.", "Application Data Folder was not created.");
    mkPath = (new QDir())->mkpath(wombatcasespath);
    if(mkPath == false)
        wombatCaseData->DisplayError(this, "2.2", "App Cases Folder Failed.", "App Cases Folder was not created.");
    mkPath = (new QDir())->mkpath(wombattmpfilepath);
    if(mkPath == false)
        wombatCaseData->DisplayError(this, "2.2", "App TmpFile Folder Failed.", "App TmpFile Folder was not created.");
    QString tmpPath = wombatdatapath + "WombatData.db";
    bool doesFileExist = wombatCaseData->FileExists(tmpPath.toStdString());
    if(!doesFileExist)
    {
        const char* errstring = wombatCaseData->CreateCaseDB(tmpPath);
        if(strcmp(errstring, "") != 0)
            wombatCaseData->DisplayError(this, "1.0", "File Error", errstring);
    }
    else
    {
        const char* errstring = wombatCaseData->OpenCaseDB(tmpPath);
        if(strcmp(errstring, "") != 0)
            wombatCaseData->DisplayError(this, "1.1", "SQL", errstring);
    }
    if(wombatCaseData->ReturnCaseCount() == 0)
    {
        ui->actionOpen_Case->setEnabled(false);
    }
    else if(wombatCaseData->ReturnCaseCount() > 0)
    {
        ui->actionOpen_Case->setEnabled(true);
    }
    else
    {
        wombatCaseData->DisplayError(this, "1.0", "Case Count", "Invalid Case Count returned.");
    }
    pluginFileNames.clear();
    pluginFileNames = locatePlugins();

    ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
    ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());
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

QObject* WombatForensics::loadPlugin(QString fileName)
{
   QPluginLoader loader(fileName);
    QObject* plugin = loader.instance();
    if(!loader.isLoaded())
        fprintf(stderr, "%s\n", loader.errorString().toStdString().c_str());
    if (plugin)
    {
        populateActions(plugin);
        populateTabWidgets(plugin);
        setupSleuthKitProperties(plugin, wombatsettingspath, "tsk-config.xml");
        setupSleuthKitLog(plugin, wombatdatapath, "tsk-log.txt", currentcaseid, currentimageid, currentanalysistype);
        setupSleuthKitSchedulerQueue(plugin);
        setupSleuthKitFileManager(plugin);
    }

    return plugin;
}

void WombatForensics::populateActions(QObject *plugin)
{
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(plugin);
    if (iEvidence)
    {
        addActions(plugin, iEvidence->evidenceActions(), iEvidence->evidenceActionIcons(), ui->mainToolBar, ui->menuEvidence, SLOT(alterEvidence()));
    }
}

void WombatForensics::populateTabWidgets(QObject *plugin)
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

void WombatForensics::addActions(QObject *plugin, const QStringList &texts, const QStringList &icons, QToolBar *toolbar, QMenu *menu, const char *member, QActionGroup *actionGroup)
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

void WombatForensics::alterEvidence()
{
    QAction *action = qobject_cast<QAction *>(sender());
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(action->parent());
    if(action->text() == tr("Add Evidence"))
    {
        QString evidenceFilePath = iEvidence->addEvidence();
        fprintf(stderr, "Evidence FilePath: %s\n", evidenceFilePath.toStdString().c_str());
        if(evidenceFilePath != "")
        {
            // MIGHT BE AN ISSUE WHEN YOU OPEN MORE THAN 1 EVIDENCE ITEM... HAVE TO TEST IT OUT AND SEE WHAT HAPPENS
            QString evidenceName = evidenceFilePath.split("/").last();
            evidenceName += ".db";
            QString tmpString = evidenceName;
            tmpString += " - ";
            tmpString += QString::fromStdString(GetTime());
            QStringList tmpList;
            tmpList << tmpString;
            wombatprogresswindow->UpdateAnalysisTree(0, new QTreeWidgetItem(tmpList));
            wombatprogresswindow->UpdateFilesFound("0");
            wombatprogresswindow->UpdateFilesProcessed("0");
            wombatprogresswindow->UpdateAnalysisState("Adding Image to Database");
            currentsleuthimages << setupSleuthKitImgDb(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath);
            setupSleuthKitBlackboard(sleuthkitplugin);
            wombatCaseData->InsertImage(evidenceName, evidenceFilePath, currentcaseid);
            sleuthKitLoadEvidence(sleuthkitplugin, evidenceFilePath, wombatprogresswindow);
            // need to populate the directory tree entries
            QStandardItem* imageNode = GetCurrentImageDirectoryTree(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath.split("/").last());
            QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
            currentroot->appendRow(imageNode);
            currenttreeview->setModel(wombatdirmodel);
       }
    }
    else if(action->text() == tr("Remove Evidence"))
        iEvidence->remEvidence(currentcaseid);
}

WombatForensics::~WombatForensics()
{
    //const char* errmsg = wombatCaseData->CloseCaseDB(); // this possibly caused glibc corrupted double-linked list
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
            currentcaseid = wombatCaseData->InsertCase(text);

            QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += text;
            this->setWindowTitle(tmpTitle); // update application window.
            // make Cases Folder
            QString userPath = wombatcasespath;
            userPath += QString::number(currentcaseid);
            userPath += "-";
            userPath += text;
            userPath += "/";
            bool mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                currentcasedirpath = userPath;
            }
            else
            {
                wombatCaseData->DisplayError(this, "2.0", "Cases Folder Creation Failed.", "New Case folder was not created.");
            }
            userPath = currentcasedirpath;
            userPath += "evidence/";
            mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                currentcaseevidencepath = userPath;
            }
            else
            {
                wombatCaseData->DisplayError(this, "2.0", "Case Evidence Folder Creation Failed", "Failed to create case evidence folder.");
            }
            if(wombatCaseData->ReturnCaseCount() > 0)
            {
                ui->actionOpen_Case->setEnabled(true);
            }
            evidenceplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libevidenceplugin.so");
            basictoolsplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libbasictoolsplugin.so");
            sleuthkitplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libsleuthkitplugin.so");
            ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
            ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());
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
        // populate case list here
        caseList = wombatCaseData->ReturnCaseNameList();
        bool ok;
        QString item = QInputDialog::getItem(this, tr("Open Existing Case"), tr("Select the Case to Open: "), caseList, 0, false, &ok);
        if(ok && !item.isEmpty()) // open selected case
        {
            currentcaseid = wombatCaseData->ReturnCaseID(item);
            QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += item;
            this->setWindowTitle(tmpTitle);
            QString userPath = wombatcasespath;
            userPath += QString::number(currentcaseid);
            userPath += "-";
            userPath += item;
            userPath += "/";
            bool mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                currentcasedirpath = userPath;
            }
            else
            {
                wombatCaseData->DisplayError(this, "2.0", "Cases Folder Check Failed.", "Existing Case folder did not exist.");
            }
            userPath = currentcasedirpath;
            userPath += "evidence/";
            mkPath = (new QDir())->mkpath(userPath);
            if(mkPath == true)
            {
                currentcaseevidencepath = userPath;
            }
            else
            {
                wombatCaseData->DisplayError(this, "2.0", "Case Evidence Folder Check Failed", "Case Evidence folder did not exist.");
            }
            evidenceplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libevidenceplugin.so");
            basictoolsplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libbasictoolsplugin.so");
            sleuthkitplugin = loadPlugin("/home/pasquale/Projects/wombatforensics/build/plugins/libsleuthkitplugin.so");
            ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
            ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());
            // POPULATE APP WITH ANY OPEN IMAGES AND MINIMAL SETTINGS
            QString caseimage;
            QStringList caseimageList = wombatCaseData->ReturnCaseImages(currentcaseid); // fullimagepath list
            foreach(caseimage, caseimageList)
            {
                OpenSleuthKitImgDb(sleuthkitplugin, currentcaseevidencepath, caseimage);
                fprintf(stderr, "Case Image: %s\n", caseimage.toStdString().c_str());
                setupSleuthKitBlackboard(sleuthkitplugin);
                QStandardItem* imageNode = GetCurrentImageDirectoryTree(sleuthkitplugin, currentcaseevidencepath, caseimage.split("/").last());
                QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
                currentroot->appendRow(imageNode);
                currenttreeview->setModel(wombatdirmodel);
            }
        }
    }
}

void WombatForensics::on_actionView_Progress_triggered()
{
    wombatprogresswindow->show();
}

void WombatForensics::setupSleuthKitProperties(QObject *plugin, QString settingsPath, QString configFileName)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemProperties(settingsPath, configFileName);
    }
}
void WombatForensics::setupSleuthKitLog(QObject *plugin, QString dataPath, QString logFileName, int64_t caseID, int64_t imageID, int analysisType)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->SetupSystemLog(dataPath, logFileName, (int)caseID, (int)imageID, analysisType);
    }
}
QString WombatForensics::setupSleuthKitImgDb(QObject *plugin, QString imgDBPath, QString evidenceFilePath)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        return iSleuthKit->SetupImageDatabase(imgDBPath, evidenceFilePath);
    }
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
void WombatForensics::sleuthKitLoadEvidence(QObject *plugin, QString evidencePath, ProgressWindow* progressWindow)
{
    SleuthKitInterface *iSleuthKit = qobject_cast<SleuthKitInterface *>(plugin);
    if(iSleuthKit)
    {
        iSleuthKit->OpenEvidence(evidencePath, progressWindow);
    }
}

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
