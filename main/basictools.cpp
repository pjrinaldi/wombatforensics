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
    //ascwidget = new HexEditor(hexTab);
    //ascwidget->setObjectName("bt-ascview");
    //hexwidget->setMaximumWidth(200);
    //hexwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    //ascwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    hexLayout->addWidget(hexwidget);
    hexvsb = new QScrollBar(hexTab);
    hexLayout->addWidget(vline);
    //hexLayout->addWidget(hexvsb);
    //hexLayout->addWidget(ascwidget);
    //ascvsb = new QStrollBar(hexTab);
    hexLayout->addWidget(hexvsb);
    hexvsb->setRange(0, 0);
    //ascvsb->setRange(0, 0);

    connect(hexvsb, SIGNAL(valueChanged(int)), hexwidget, SLOT(setTopLeftToPercent(int)));
    //connect(hexvsb, SIGNAL(valueChanged(int)), ascwidget, SLOT(setTopLeftToPercent(int)));
    /*
     *
     QWidget* h = new QWidget(this);
       QHBoxLayout* l = new QHBoxLayout(h);
     
       hexEditor = new HexEditor(h);
       hexEditor->setSizePolicy( QSizePolicy( QSizePolicy::Expanding,
                         QSizePolicy::Expanding ) );
       l->addWidget(hexEditor);
       vsb = new QScrollBar(h);
       l->addWidget(vsb);
       vsb->setRange(0,0);
     
       setCentralWidget(h);
     * 
     */
    /*
    hexwidget = new BinViewWidget(hexTab);
    hexwidget->setObjectName("bt-hexview");
    // appears the model is set to open the file, then the widget sets the model.
    hexwidget->setModel(0);
    hexLayout->setContentsMargins(0, 0, 0, 0);
    hexLayout->addWidget(hexwidget);
    */
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
    //hexwidget->setBaseHex();
    hexwidget->set2BPC();
    //ascwidget->open(tmpFilePath);
    //ascwidget->setBaseASCII();
    //ascwidget->set1BPC();
    /*
    hexmodel = new BinViewModel();
    hexmodel->open(tmpFilePath);
    hexwidget->setModel(hexmodel);
    */
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
  //hexvsb->setValue(ascwidget->offsetToPercent(pos));
}
