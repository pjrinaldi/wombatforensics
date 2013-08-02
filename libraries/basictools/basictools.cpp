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

QWidget* BasicTools::setupHexTab() const
{
    // hex editor tab
    QWidget* hexTab = new QWidget();
    QVBoxLayout* hexLayout = new QVBoxLayout();
    BinViewWidget* hexWidget = new BinViewWidget();
    //BinViewModel* hexModel = 0;
    // appears the model is set to open the file, then the widget sets the model.
    hexWidget->setModel(0);
    hexLayout->addWidget(hexWidget);
    hexTab->setLayout(hexLayout);

    return hexTab;
}

QWidget* BasicTools::setupTxtTab() const
{
    QWidget* txtTab = new QWidget();

    return txtTab;
}

