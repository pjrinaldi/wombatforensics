#include "interfaces.h"
#include "wombatforensics.h"
#include "wombatcasedb.h"

#include <QPluginLoader>

WombatForensics::WombatForensics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WombatForensics)
{
    ui->setupUi(this);
    wombatCaseData = new WombatCaseDb(this);
    currentcaseid = -1;
    QDir testDir = QDir(qApp->applicationDirPath());
    testDir.mkdir("data");
    QString tmpPath = testDir.currentPath();
    tmpPath += "/data/WombatData.db";
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
        ui->actionOpen_Case_2->setEnabled(false);
    }
    else if(wombatCaseData->ReturnCaseCount() > 0)
    {
        ui->actionOpen_Case->setEnabled(true);
        ui->actionOpen_Case_2->setEnabled(true);
    }
    else
    {
        wombatCaseData->DisplayError(this, "1.0", "Case Count", "Invalid Case Count returned.");
    }
    pluginFileNames = locatePlugins();

    ui->menuEvidence->setEnabled(!ui->menuEvidence->actions().isEmpty());
    ui->menuSettings->setEnabled(!ui->menuSettings->actions().isEmpty());
}

bool WombatForensics::isPluginLoaded(QString pluginFileName)
{
    QString fileName;
    foreach(fileName, pluginFileNames)
    {
        if(pluginFileName == fileName)
            loadPlugin(pluginFileName);
        else
            fprintf(stderr, "string compare failed.");
    }
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

void WombatForensics::loadPlugin(QString fileName)
{
   QPluginLoader loader(fileName);
    QObject* plugin = loader.instance();
    if(!loader.isLoaded())
        fprintf(stderr, "%s\n", loader.errorString().toStdString().c_str());
    if (plugin)
    {
        populateActions(plugin);
        populateToolBox(plugin);
    }
}

void WombatForensics::populateActions(QObject *plugin)
{
    EvidenceInterface *iEvidence = qobject_cast<EvidenceInterface *>(plugin);
    if (iEvidence)
    {
        addActions(plugin, iEvidence->evidenceActions(), iEvidence->evidenceActionIcons(), ui->mainToolBar, ui->menuEvidence, SLOT(alterEvidence()));
    }
}

void WombatForensics::populateToolBox(QObject *plugin)
{
    /*
    BasicToolsInterface *iBasicTools = qobject_cast<BasicToolsInterface *>(plugin);
    if(iBasicTools)
    {
        ui->toolBox->addItem(iBasicTools->setupToolBoxDirectoryTree(), ((QStringList)iBasicTools->toolboxViews())[0]);
        ui->toolBox->addItem(iBasicTools->setupToolBoxFileExtensionTree(), ((QStringList)iBasicTools->toolboxViews())[1]);
    }
    */
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
        iEvidence->addEvidence(currentcaseid);
    else if(action->text() == tr("Remove Evidence"))
        iEvidence->remEvidence(currentcaseid);
}

WombatForensics::~WombatForensics()
{
    const char* errmsg = wombatCaseData->CloseCaseDB();
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
            this->setWindowTitle(tmpTitle);

            if(wombatCaseData->ReturnCaseCount() > 0)
            {
                ui->actionOpen_Case->setEnabled(true);
                ui->actionOpen_Case_2->setEnabled(true);
            }
            isPluginLoaded("/home/pasquale/Projects/wombatforensics/build/plugins/libevidenceplugin.so"); // manually load evidence plugin
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
        bool ok;
        QString item = QInputDialog::getItem(this, tr("Open Existing Case"), tr("Select the Case to Open: "), caseList, 0, false, &ok);
        if(ok && !item.isEmpty())
        {
           QString tmpTitle = "Wombat Forensics - ";
            tmpTitle += item;
            this->setWindowTitle(tmpTitle);
        }
    }
}

