#include <QtWidgets>

#include "evidence.h"

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

void EvidencePlugin::Run(QString input)
{
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
