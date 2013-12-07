#include "basictools.h"

QWidget* BasicTools::setupHexTab()
{
    // hex editor tab
    QWidget* hexTab = new QWidget();
    QHBoxLayout* hexLayout = new QHBoxLayout();
    QVBoxLayout* vlayout = new QVBoxLayout();
    QWidget* dumwidget = new QWidget();
    hstatus = new QStatusBar(hexTab);
    hstatus->setSizeGripEnabled(false);
    selectedoffset = new QLabel("Offset: 00");
    selectedhex = new QLabel("Length: 0");
    selectedascii = new QLabel("Ascii: ");
    selectedinteger = new QLabel("Integer: ");
    selectedfloat = new QLabel("Float: ");
    selecteddouble = new QLabel("Double: ");
    //selectedoffset->setFrameStyle(QFrame::NoFrame || QFrame::VLine || QFrame::Plain);
    hstatus->addWidget(selectedoffset);
    hstatus->addWidget(selectedhex);
    hstatus->addWidget(selectedascii);
    hstatus->addWidget(selectedinteger);
    hstatus->addWidget(selectedfloat);
    hstatus->addWidget(selecteddouble);
    hexwidget = new HexEditor(dumwidget);
    hexwidget->setObjectName("bt-hexview");
    //hexTab->setBackground(QBrush(Qt::white));
    hexwidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    hexLayout->addWidget(hexwidget);
    hexvsb = new QScrollBar(hexwidget);
    hexLayout->addWidget(hexvsb);
    hexvsb->setRange(0, 0);
    dumwidget->setLayout(hexLayout);
    vlayout->addWidget(dumwidget);
    vlayout->addWidget(hstatus);

    hexTab->setLayout(vlayout);

    return hexTab;
}

void BasicTools::UpdateSelectValue(const QString &txt)
{
    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    selectedinteger->setText(tmptext);
    // update float entry;
    float fvalue;
    ucharPtr = (uchar*)(&fvalue);
    if(bytes.size() < sizeof(float) )
    {
        for(unsigned int i= 0; i < sizeof(float); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&fvalue,&bytes.begin()[0],sizeof(float));
    }
    strvalue.setNum( fvalue );
    tmptext = "Float: " + strvalue;
    selectedfloat->setText(tmptext);
    // update double
    double dvalue;
    ucharPtr = (uchar*)&dvalue;
    if(bytes.size() < sizeof(double) )
    {
        for(unsigned int i= 0; i < sizeof(double); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&dvalue,&bytes.begin()[0],sizeof(double));
    }
    strvalue.setNum( dvalue );
    tmptext = "Double: " + strvalue;
    selecteddouble->setText(tmptext);
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

QWidget* BasicTools::setupOmniTab()
{
    QWidget* omnitab = new QWidget();
    QWidget* webpage = new QWidget();
    QWidget* picpage = new QWidget();
    QWidget* vidpage = new QWidget();
    QLabel* piclabel = new QLabel("Pic View");
    QLabel* vidlabel = new QLabel("Vid View");
    QVBoxLayout* weblayout = new QVBoxLayout();
    QVBoxLayout* piclayout = new QVBoxLayout();
    QVBoxLayout* vidlayout = new QVBoxLayout();
    QWebView* webview = new QWebView(omnitab);
    webview->setObjectName("bt-webview");
    //webview->setHtml("<b>hello</b>");
    //webview->show();
    weblayout->addWidget(webview);
    weblayout->setContentsMargins(0, 0, 0, 0);
    piclayout->addWidget(piclabel);
    piclayout->setContentsMargins(0, 0, 0, 0);
    vidlayout->addWidget(vidlabel);
    vidlayout->setContentsMargins(0, 0, 0, 0);
    webpage->setLayout(weblayout);
    picpage->setLayout(piclayout);
    vidpage->setLayout(vidlayout);
    QStackedLayout* omnistack = new QStackedLayout();
    omnistack->setObjectName("bt-omnistack");
    omnistack->addWidget(webpage);
    omnistack->addWidget(picpage);
    omnistack->addWidget(vidpage);
    //omnistack->setCurrentWidget(webpage);
    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addLayout(omnistack);
    omnitab->setLayout(mainlayout);

    return omnitab;
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
            LoadOmniContent(filepath); // possibly add a view type here: 1 - web, 2 - pic, 3 - vid
        }
        else
        {
            txtwidget->setPlainText("");
        }
    }
    else
    {
        txtwidget->setPlainText("");
        // load nothing here...
    }
}

void BasicTools::LoadHexModel(QString tmpFilePath)
{
    hexwidget->open(tmpFilePath);
    hexwidget->set2BPC();
    hexwidget->setBaseHex();
}
void BasicTools::LoadTxtContent(QString asciiText)
{
    QFile tmpFile(asciiText);
    tmpFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&tmpFile);
    txtwidget->setPlainText(stream.readAll());
    tmpFile.close();
}

void BasicTools::LoadOmniContent(QString filePath)
{
}

void BasicTools::setOffsetLabel(off_t pos)
{
  QString label;
  label = "Offset: ";
  char    buffer[64];
#if _LARGEFILE_SOURCE
  sprintf(buffer,"0x%lx",pos);
#else
  sprintf(buffer,"0x%x",pos);
#endif
  label += buffer;
  selectedoffset->setText(label);
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
}
