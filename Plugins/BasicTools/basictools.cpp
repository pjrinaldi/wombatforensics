#include "basictools.h"

QStringList BasicTools::evidenceActions() const
{
    return QStringList() << tr("Add Evidence") << tr("Remove Evidence");
}

QStringList BasicTools::evidenceActionIcons() const
{
    return QStringList() << tr(":/basic/addevidence") << tr(":/basic/remevidence");
}

QStringList BasicTools::evidenceToolboxIcons() const
{
    return QStringList() << tr(":/basictools/images/treefile") << tr(":/basictools/images/treefolder") << (":/basictools/images/treepartition") << tr(":/basictools/images/treefilemanager") << tr(":/basictools/images/treeimage");
}

QStringList BasicTools::toolboxViews() const
{
    return QStringList() << tr("Directory Listing") << tr("File Signature Category");
}

void BasicTools::addEvidence(int currentCaseID)
{
    // add evidence here
    currentcaseid = currentCaseID;
    QString evidenceFile = QFileDialog::getOpenFileName(0, tr("Select Evidence Item"), tr("./"));
    if (evidenceFile != "")
    {
        // make respective directories to store case information
    }
}

void BasicTools::remEvidence(int currentCaseID)
{
    currentcaseid = currentCaseID;
    // remove evidence.db file.
    // remove evidence_id from the cases table...
}
QWidget* BasicTools::setupToolBox() const
{
    QTreeView* directoryTreeView = new QTreeView();

    return directoryTreeView;
}

QWidget* BasicTools::setupColumnView() const
{
    QColumnView* directoryTreeColumnView = new QColumnView();

    return directoryTreeColumnView;
}

QWidget* BasicTools::setupTabWidget() const
{
    QTabWidget* directoryTreeTabWidget = new QTabWidget();
    // probably add the tabs

    return directoryTreeTabWidget;
}
