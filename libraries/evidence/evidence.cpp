#include <QtWidgets>

#include "evidence.h"

QStringList EvidencePlugin::evidenceActions() const
{
    return QStringList() << tr("Add Evidence") << tr("Remove Evidence");
}

QStringList EvidencePlugin::evidenceActionIcons() const
{
    return QStringList() << tr(":/basic/addevidence") << tr(":/basic/remevidence");
}

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
