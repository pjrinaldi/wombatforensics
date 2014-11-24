#include "fileviewer.h"

FileViewer::FileViewer(QWidget* parent, TskObject* tskobjptr) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    tskptr = tskobjptr;
    ui->setupUi(this);
    QHBoxLayout* hexlayout = new QHBoxLayout();
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);
    tskptr->readimginfo = NULL;
    tskptr->readfsinfo = NULL;
    tskptr->readfileinfo = NULL;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    filehexview = new HexEditor(ui->hexpage, tskptr);
    filehexview->setObjectName("filehexview");
    filehexview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    hexlayout->addWidget(filehexview);
    filehexvsb = new QScrollBar(filehexview);
    filehexvsb->setRange(0, 0);
    hexlayout->addWidget(filehexvsb);
    connect(filehexview, SIGNAL(rangeChanged(off_t, off_t)), this, SLOT(SetScrollBarRange(off_t, off_t)));
    connect(filehexview, SIGNAL(topLeftChanged(off_t)), this, SLOT(setScrollBarValue(off_t)));
    connect(filehexview, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    connect(filehexvsb, SIGNAL(valueChanged(int)), filehexview, SLOT(setTopLeftToPercent(int)));
    connect(filehexview, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    connect(filehexview, SIGNAL(StepValues(int, int)), this, SLOT(SetStepValues(int, int)));
}

FileViewer::~FileViewer()
{
    this->close();
}

void FileViewer::SetScrollBarRange(off_t low, off_t high)
{
   filehexvsb->setRange(low, high);
}

void FileViewer::setScrollBarValue(off_t pos)
{
    filehexvsb->setValue(pos);
}

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

void FileViewer::SetStepValues(int singlestep, int pagestep)
{
    filehexvsb->setSingleStep(singlestep);
    filehexvsb->setPageStep(pagestep);
}
