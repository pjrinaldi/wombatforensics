#include "basictools.h"

QStringList BasicTools::evidenceToolboxIcons() const
{
    return QStringList() << tr(":/basictools/images/treefile") << tr(":/basictools/images/treefolder") << (":/basictools/images/treepartition") << tr(":/basictools/images/treefilemanager") << tr(":/basictools/images/treeimage");
}
/*
QStringList BasicTools::toolboxViews() const
{
    return QStringList() << tr("Directory Listing") << tr("File Extension Category");
}

QWidget* BasicTools::setupToolBoxDirectoryTree() const
{
    QTreeView* directoryTreeView = new QTreeView();
    directoryTreeView->setObjectName("bt-dirTreeView");

    return directoryTreeView;
}
QWidget* BasicTools::setupToolBoxFileExtensionTree() const
{
    QTreeView* fileExtensionTreeView = new QTreeView();
    fileExtensionTreeView->setObjectName("bt-fileExtTreeView");

    return fileExtensionTreeView;
}
*/
/*
QWidget* BasicTools::setupColumnView() const
{
    QColumnView* directoryTreeColumnView = new QColumnView();

    return directoryTreeColumnView;
}
*/
QWidget* BasicTools::setupHexTab()
{
    // hex editor tab
    QWidget* hexTab = new QWidget();
    QVBoxLayout* hexLayout = new QVBoxLayout();
    hexwidget = new BinViewWidget();
    hexwidget->setObjectName("bt-hexView");
    //BinViewModel* hexModel = 0;
    // appears the model is set to open the file, then the widget sets the model.
    hexwidget->setModel(0);
    hexLayout->setContentsMargins(0, 0, 0, 0);
    hexLayout->addWidget(hexwidget);
    hexTab->setLayout(hexLayout);

    return hexTab;
}

QWidget* BasicTools::setupTxtTab() const
{
    QWidget* txtTab = new QWidget();
    QVBoxLayout* txtLayout = new QVBoxLayout();
    QTextEdit* txtWidget = new QTextEdit();
    txtWidget->setObjectName("bt-txtView");
    txtLayout->setContentsMargins(0, 0, 0, 0);
    txtLayout->addWidget(txtWidget);
    txtTab->setLayout(txtLayout);
    
    return txtTab;
}

QWidget* BasicTools::setupDirTab()
{
    //directory tree tab
    QWidget* dirTab = new QWidget();
    QVBoxLayout* dirLayout = new QVBoxLayout();
    dirtreeview = new QTreeView();
    dirtreeview->setObjectName("bt-dirtree");
    dirLayout->SetContentsMargins(0, 0, 0, 0);
    dirLayout->addWidget(dirtreeview);
    dirTab->setLayout(dirLayout);

    return dirTab;
}

QWidget* BasicTools::setupTypTab()
{
    // file type (extension) tree tab
    QWidget* typTab = new  QWidget();
    QVBoxLayout* typLayout = new QVBoxLayout();
    typLayout->SetContentsMargins(0, 0, 0, 0);
    typtreeview = new QTreeView();
    typetreeview->setObjectName("bt-typtree");
    typLayout->addWidget(typtreeview);
    typTab->setLayout(typLayout);

    return typTab;
}

void BasicTools::LoadHexModel(QString tmpFilePath)
{
    hexmodel = new BinViewModel();
    hexmodel->open(tmpFilePath);
    hexwidget->setModel(hexmodel);
}
