#include "basictools.h"

/*QStringList BasicTools::evidenceToolboxIcons() const
{
    return QStringList() << vtr(":/basictools/images/treefile") << tr(":/basictools/images/treefolder") << (":/basictools/images/treepartition") << tr(":/basictools/images/treefilemanager") << tr(":/basictools/images/treeimage");
}*/
QVariantMap BasicTools::Initialize()
{
    QVariantMap tmpmap;
    QWidget* tmpwidget = setupTxtTab();
    QVariant v = VPtr<QWidget>::asQVariant(tmpwidget);
    tmpmap.clear();
    tmpmap.insert("Text View", v);

    return tmpmap;
    /*
    PluginMap mainmap;
    plugmap.clear();
    plugmap.insert("actions", QStringList() << "loadhex" << "loadtxt");
    //plugmap.insert("icons", QStringList() << tr(":/basictools/images/treefile") << tr(":/basictools/images/treefolder") << (":/basictools/images/treepartition") << tr(":/basictools/images/treefilemanager") << tr(":/basictools/images/treeimage"));
    QVariant hexvariant = VPtr<QWidget>::asQVariant(setupTxtTab());
    plugmap.insert("viewtab", hexvariant);
    mainmap.map = plugmap;
    mastermap.map = plugmap;

    return mainmap;
    */
}

void BasicTools::Run(QString input)
{
    fprintf(stderr, "Map Size: %d\n", mastermap.map.size());
    QStringList tmplist = mastermap.map["actions"].toStringList();
    foreach(QString tmpstring, tmplist)
    {
        if(input.compare(tmpstring) == 0)
        {
            fprintf(stderr, "Run with input: %s\n", input.toStdString().c_str());
        }
    }
}

QWidget* BasicTools::setupHexTab()
{
    // hex editor tab
    QWidget* hexTab = new QWidget();
    QVBoxLayout* hexLayout = new QVBoxLayout();
    hexwidget = new BinViewWidget(hexTab);
    hexwidget->setObjectName("bt-hexView");
    // appears the model is set to open the file, then the widget sets the model.
    hexwidget->setModel(0);
    hexLayout->setContentsMargins(0, 0, 0, 0);
    hexLayout->addWidget(hexwidget);
    hexTab->setLayout(hexLayout);

    return hexTab;
}

QWidget* BasicTools::setupTxtTab()
{
    QWidget* txtTab = new QWidget();
    QVBoxLayout* txtLayout = new QVBoxLayout();
    txtwidget = new QTextEdit();
    txtwidget->setObjectName("bt-txtView");
    txtLayout->setContentsMargins(0, 0, 0, 0);
    txtLayout->addWidget(txtwidget);
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
    dirLayout->setContentsMargins(0, 0, 0, 0);
    dirLayout->addWidget(dirtreeview);
    dirTab->setLayout(dirLayout);

    return dirTab;
}

QWidget* BasicTools::setupTypTab()
{
    // file type (extension) tree tab
    QWidget* typTab = new  QWidget();
    QVBoxLayout* typLayout = new QVBoxLayout();
    typLayout->setContentsMargins(0, 0, 0, 0);
    typtreeview = new QTreeView();
    typtreeview->setObjectName("bt-typtree");
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
void BasicTools::LoadTxtContent(QString asciiText)
{
    QFile tmpFile(asciiText);
    tmpFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&tmpFile);
    txtwidget->setPlainText(stream.readAll());
    tmpFile.close();
}
