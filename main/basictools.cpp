#include "basictools.h"

QWidget* BasicTools::setupHexTab()
{
    // hex editor tab
    QWidget* hexTab = new QWidget();
    QHBoxLayout* hexLayout = new QHBoxLayout(hexTab);
    QFrame* vline = new QFrame();
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Sunken);
    hexwidget = new HexEditor(hexTab);
    hexwidget->setObjectName("bt-hexview");
    hexwidget->myspacer = 1;
    //hexTab->setBackground(QBrush(Qt::white));
    ascwidget = new HexEditor(hexTab);
    ascwidget->setObjectName("bt-ascview");
    ascwidget->myspacer = 2;
    fprintf(stderr, "hexspacer: %d\n", hexwidget->myspacer);
    fprintf(stderr, "ascspacer: %d\n", ascwidget->myspacer);
    //hexwidget->setMaximumWidth(200);
    hexwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    ascwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    hexLayout->addWidget(hexwidget);
    hexvsb = new QScrollBar(hexwidget);
    hexLayout->addWidget(vline);
    //hexLayout->addWidget(hexvsb);
    hexLayout->addWidget(ascwidget);
    //ascvsb = new QStrollBar(ascwidget);
    hexLayout->addWidget(hexvsb);
    hexvsb->setRange(0, 0);
    //hexLayout->setStretch(0, 1);
    //hexLayout->setStretch(1, 2);
    //hexLayout->setStretch(2, 1);
    //ascvsb->setRange(0, 0);

    connect(hexwidget, SIGNAL(rangeChanged(off_t, off_t)), this, SLOT(setScrollBarRange(off_t, off_t)));
    connect(hexwidget, SIGNAL(topLeftChanged(off_t)), this, SLOT(setScrollBarValue(off_t)));
    connect(hexwidget, SIGNAL(offsetChanged(off_t)), this, SLOT(setOffsetLabel(off_t)));
    connect(hexvsb, SIGNAL(valueChanged(int)), hexwidget, SLOT(setTopLeftToPercent(int)));
    connect(hexvsb, SIGNAL(valueChanged(int)), ascwidget, SLOT(setTopLeftToPercent(int)));
    hexTab->setLayout(hexLayout);

    return hexTab;
}

QWidget* BasicTools::setupTxtTab()
{
    QWidget* txtTab = new QWidget();
    QVBoxLayout* txtLayout = new QVBoxLayout();
    txtwidget = new QTextEdit();
    txtwidget->setObjectName("bt-txtview");
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

void BasicTools::LoadFileContents(QString filepath)
{
    if(filepath != "")
    {
        QFileInfo pathinfo(filepath);
        if(!pathinfo.isDir())
        {
            LoadHexModel(filepath);
            LoadTxtContent(filepath);
        }
        else
        {
            //hexwidget->setModel(0);
            txtwidget->setPlainText("");
        }
    }
    else
    {
        //hexwidget->setModel(0);
        txtwidget->setPlainText("");
        // load nothing here...
    }
}

void BasicTools::LoadHexModel(QString tmpFilePath)
{
    hexwidget->open(tmpFilePath);
    hexwidget->setBaseHex();
    hexwidget->set2BPC();
    ascwidget->open(tmpFilePath);
    ascwidget->setBaseASCII();
    ascwidget->set4BPC();
}
void BasicTools::LoadTxtContent(QString asciiText)
{
    QFile tmpFile(asciiText);
    tmpFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&tmpFile);
    txtwidget->setPlainText(stream.readAll());
    tmpFile.close();
}

void BasicTools::setScrollBarRange(off_t low, off_t high)
{
   (void)low;(void)high;
   // range must be contained in the space of an integer, just do 100
   // increments
   hexvsb->setRange(0,100);
}

void BasicTools::setScrollBarValue(off_t pos)
{
  // pos is the topLeft pos, set the scrollbar to the
  // location of the last byte on the page
  // Note: offsetToPercent now rounds up, so we don't
  // have to worry about if this is the topLeft or bottom right
  hexvsb->setValue(hexwidget->offsetToPercent(pos));
  hexvsb->setValue(ascwidget->offsetToPercent(pos));
}
