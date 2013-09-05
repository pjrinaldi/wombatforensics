//#include <QtWidgets>

#include "evidence.h"
/*
QList<QStringList> EvidencePlugin::PluginActions() const
{
    QList<QStringList> tmpList;
    tmpList.append(QStringList() << tr("Add Evidence") << tr("Remove Evidence"));

    return tmpList;
}

QList<QStringList> EvidencePlugin::PluginActionIcons() const
{
    QList<QStringList> tmpList;
    tmpList.append(QStringList() << tr(":/basic/addevidence") << tr(":/basic/remevidence"));
    
    return tmpList;
}
QStringList EvidencePlugin::PluginMenus() const
{
    return QStringList() << tr("Evidence");
}
*/
void EvidencePlugin::Initialize(WombatVariable wombatvariable)
{
    plugmap.clear();
    // QMenu setup
    /*
    QMenu* tmpmenu;
    QAction* tmpaction = new QAction(tr("Add Evidence"), (QWidget*)wombatvariable.parent);
    tmpaction->setIcon(QIcon(tr(":/basic/addevidence")));
    connect(tmpaction, SIGNAL(triggered()), wombatvariable.parent, SLOT(RunPlugin()));
    tmpmenu->addAction(tmpaction);
    QAction* tmpaction2 = new QAction(tr("Remove Evidence"), wombatvariable.parent);
    tmpaction2->setIcon(QIcon(tr(":/basic/remevidence")));
    connect(tmpaction2, SIGNAL(triggered()), wombatvariable.parent, SLOT(RunPlugin()));
    tmpmenu->addAction(tmpaction2);
    QVariant tmpvariant = VPtr<QMenu>::asQVariant(tmpmenu);
    //plugmap.insert("addmenu", tmpvariant);
    */
    // QToolbuttonSetup
    QAction* tmpaction3 = new QAction(QIcon(tr(":/basic/addevidence")), tr("Add Evidence"), wombatvariable.parent);
    //QVariant tmpvariant = VPtr<QMenu>::asQVariant(tmpmenu);
    QVariant tmpvariant = VPtr<QAction>::asQVariant(tmpaction3);
    plugmap.insert("addtoolbutton", tmpvariant);

    /*
        QMenu* tmpmenu = menu->addMenu(menus[i]);
        for(int j = 0; j < texts.count(); j++)
        {
            QAction* action1 = new QAction(texts[i][j], plugin);
            action1->setIcon(QIcon(icons[i][j]));
            connect(action1, SIGNAL(triggered()), this, SLOT(RunPlugin()));
            if(actionGroup)
            {
                action1->setCheckable(true);
                actionGroup->addAction(action1);
            }
            toolbar->addAction(action1);
            QAction* action2 = action1;
            tmpmenu->addAction(action2);
    plugmap.clear();
    QMap<QString, QVariant> tmpMap;
    tmpMap.insert("addevidence", ":/basic/addevidence");
    tmpMap.insert("remevidence", ":/basic/remevidence");
    plugmap.insert("icons", tmpMap);
    tmpMap.clear();
    tmpMap.insert("addevidence", tr("Add Evidence"));
    tmpMap.insert("remevidence", tr("Remove Evidence"));
    plugmap.insert("actions", tmpMap);
    tmpMap.clear();
    tmpMap.insert("evidencemenu", tr("Evidence"));
    plugmap.insert("menus", tmpMap);
    tmpMap.clear();*/
}

void EvidencePlugin::Run(QString input)
{/*
    foreach(QStringList actionList, PluginActions())
    {
        foreach(QString action, actionList)
        {
            if(input.compare(action) == 0)
            {
                fprintf(stderr, "Run with input: %s\n", input.toStdString().c_str());
            }
        }
    }
    */
}
/*
QString EvidencePlugin::addEvidence()
{
    QString evidenceFile = "";
    // add evidence here
    //currentcaseid = currentCaseID;
    evidenceFile = QFileDialog::getOpenFileName(0, tr("Select Evidence Item"), tr("./"));
    // MY OWN DIALOG - SEE IF THIS FREES UP STUFF.
    //QFileDialog *filedialog = new QFileDialog(0, "Select Evidence Item", "./");
    //filedialog->setFileMode(QFileDialog::ExistingFile);
    //filedialog->open(this, SLOT(dialogClosed(QString)));

    return evidenceFile;
}

void EvidencePlugin::remEvidence(int currentCaseID)
{
    currentcaseid = currentCaseID;
    // remove evidence.db file.
    // remove evidence_id from the cases table...
}
*/
