#include "sleuthkit.h"

void SleuthKitPlugin::SetupSystemProperties(QString configFilePath) const
{
    try
    {
        systemproperties = new TskSystemPropertiesImpl();
        systemproperties->initialize(configFilePath.toStdString());
        TskServices::Instance().setSystemProperties(*systemproperties);
    }
    catch(TskException &ex)
    {
        fprintf(stderr, "Loading Config File config file: ", ex.message().c_str());
    }
}
/*
QStringList EvidencePlugin::evidenceActions() const
{
    return QStringList() << tr("Add Evidence") << tr("Remove Evidence");
}

QStringList EvidencePlugin::evidenceActionIcons() const
{
    return QStringList() << tr(":/basic/addevidence") << tr(":/basic/remevidence");
}

void EvidencePlugin::addEvidence(int currentCaseID)
{
    // add evidence here
    currentcaseid = currentCaseID;
    QString evidenceFile = QFileDialog::getOpenFileName(0, tr("Select Evidence Item"), tr("./"));
    if (evidenceFile != "")
    {
        // make respective directories to store case information
    }
}

void EvidencePlugin::remEvidence(int currentCaseID)
{
    currentcaseid = currentCaseID;
    // remove evidence.db file.
    // remove evidence_id from the cases table...
}
*/
