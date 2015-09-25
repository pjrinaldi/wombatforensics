#include "fileviewer.h"

FileViewer::FileViewer(QWidget* parent, TskObject* tskobjptr) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    tskptr = tskobjptr;
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    filehexview = new FileHexViewer(ui->centralwidget, tskptr);
    filehexview->setObjectName("filehexview");
    filehexview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    ui->horizontalLayout->addWidget(filehexview);
    jumpfilterview = new FileJumpFilter(this);
    lineup = new QPushButton(QIcon(":/basic/lineup"), "", ui->centralwidget);
    linedown = new QPushButton(QIcon(":/basic/linedown"), "", ui->centralwidget);
    pageup = new QPushButton(QIcon(":/basic/pageup"), "", ui->centralwidget);
    pagedown = new QPushButton(QIcon(":/basic/pagedown"), "", ui->centralwidget);
    //jumpto = new QPushButton("J", ui->hexPage);
    linedown->setAutoRepeat(true);
    lineup->setAutoRepeat(true);
    pagedown->setAutoRepeat(true);
    pageup->setAutoRepeat(true);
    linedown->setVisible(false);
    lineup->setVisible(false);
    pageup->setVisible(false);
    pagedown->setVisible(false);
    //jumpto->setVisible(false);
    hexrocker = new FileSlider(ui->centralwidget);
    hexrocker->setRange(-100, 100);
    hexrocker->setValue(0);
    hexrocker->setSingleStep(1);
    ui->horizontalLayout->addWidget(hexrocker);

    /*
    filehexvsb = new QScrollBar(filehexview);
    filehexvsb->setRange(0, 0);
    ui->horizontalLayout->addWidget(filehexvsb);
    */
    connect(linedown, SIGNAL(clicked()), filehexview, SLOT(nextLine()));
    connect(lineup, SIGNAL(clicked()), filehexview, SLOT(prevLine()));
    connect(pagedown, SIGNAL(clicked()), filehexview, SLOT(nextPage()));
    connect(pageup, SIGNAL(clicked()), filehexview, SLOT(prevPage()));
    connect(hexrocker, SIGNAL(ShowJumpFilter()), jumpfilterview, SLOT(DisplayFilter()));
    connect(jumpfilterview, SIGNAL(SetOffset()), filehexview, SLOT(SetOffset()));
    connect(hexrocker, SIGNAL(sliderMoved(int)), filehexview, SLOT(setTopLeftToPercent(int)));
    connect(hexrocker, SIGNAL(sliderMoved(int)), this, SLOT(ShowRockerToolTip(int)));
    connect(hexrocker, SIGNAL(sliderReleased()), this, SLOT(ResetSlider()));
    connect(filehexview, SIGNAL(SkipDown()), this, SLOT(SkipDown()));
    connect(filehexview, SIGNAL(SkipUp()), this, SLOT(SkipUp()));
    connect(filehexview, SIGNAL(PageUp()), this, SLOT(PageUp()));
    connect(filehexview, SIGNAL(PageDown()), this, SLOT(PageDown()));

    //connect(filehexview, SIGNAL(rangeChanged(off_t, off_t)), this, SLOT(SetScrollBarRange(off_t, off_t)));
    //connect(filehexview, SIGNAL(topLeftChanged(off_t)), this, SLOT(setScrollBarValue(off_t)));
    connect(filehexview, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    //connect(filehexvsb, SIGNAL(valueChanged(int)), filehexview, SLOT(setTopLeftToPercent(int)));
    connect(filehexview, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    //connect(filehexview, SIGNAL(StepValues(int, int)), this, SLOT(SetStepValues(int, int)));
}

void FileViewer::ShowRockerToolTip(int moved)
{
    QToolTip::showText(QCursor::pos(), QString::number(abs(moved)), hexrocker);
}

void FileViewer::ResetSlider()
{
    if(linedown->isDown())
        linedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    hexrocker->setValue(0);
}

void FileViewer::SkipDown()
{
    if(pagedown->isDown())
        pagedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    linedown->setDown(true);
}

void FileViewer::SkipUp()
{
    if(pageup->isDown())
        pageup->setDown(false);
    if(linedown->isDown())
        linedown->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    lineup->setDown(true);
}

void FileViewer::PageUp()
{
    if(lineup->isDown())
        lineup->setDown(false);
    if(linedown->isDown())
        linedown->setDown(false);
    if(pagedown->isDown())
        pagedown->setDown(false);
    pageup->setDown(true);
}

void FileViewer::PageDown()
{
    if(linedown->isDown())
        linedown->setDown(false);
    if(lineup->isDown())
        lineup->setDown(false);
    if(pageup->isDown())
        pageup->setDown(false);
    pagedown->setDown(true);
}

FileViewer::~FileViewer()
{
    this->close();
}

void FileViewer::HideClicked()
{
    this->hide();
    emit HideFileViewer(false);
}

void FileViewer::closeEvent(QCloseEvent* event)
{
    emit HideFileViewer(false);
    event->accept();
}

void FileViewer::SetScrollBarRange(off_t low, off_t high)
{
    (void)low; (void)high;
   //filehexvsb->setRange(low, high);
}
/*
void FileViewer::setScrollBarValue(off_t pos)
{
    //filehexvsb->setValue(pos);
}
*/
void FileViewer::SetOffsetLabel(off_t pos)
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

void FileViewer::UpdateSelectValue(const QString &txt)
{
    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    /*
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    //selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    //selectedinteger->setText(tmptext);
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
    //selectedfloat->setText(tmptext);
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
    //selecteddouble->setText(tmptext);
    */
}
/*
void FileViewer::SetStepValues(int singlestep, int pagestep)
{
    //filehexvsb->setSingleStep(singlestep);
    //filehexvsb->setPageStep(pagestep);
}
*/
