#include "basictools.h"

QStringList BasicTools::evidenceToolboxIcons() const
{
    return QStringList() << tr(":/basictools/images/treefile") << tr(":/basictools/images/treefolder") << (":/basictools/images/treepartition") << tr(":/basictools/images/treefilemanager") << tr(":/basictools/images/treeimage");
}

QStringList BasicTools::toolboxViews() const
{
    return QStringList() << tr("Directory Listing") << tr("File Extension Category");
}

QWidget* BasicTools::setupToolBoxDirectoryTree() const
{
    QTreeView* directoryTreeView = new QTreeView();

    return directoryTreeView;
}
QWidget* BasicTools::setupToolBoxFileExtensionTree() const
{
    QTreeView* fileExtensionTreeView = new QTreeView();

    return fileExtensionTreeView;
}

QWidget* BasicTools::setupColumnView() const
{
    QColumnView* directoryTreeColumnView = new QColumnView();

    return directoryTreeColumnView;
}

QWidget* BasicTools::setupTabWidget() const
{
    QTabWidget* directoryTreeTabWidget = new QTabWidget();
    // probably add the generic tabs as well

    return directoryTreeTabWidget;
}

