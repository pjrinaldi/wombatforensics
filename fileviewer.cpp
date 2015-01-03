#include "fileviewer.h"

FileViewer::FileViewer(QWidget* parent, TskObject* tskobjptr) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    tskptr = tskobjptr;
    ui->setupUi(this);
    //QHBoxLayout* hexlayout = new QHBoxLayout(ui->centralwidget);
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    filehexview = new HexViewer(ui->centralwidget, tskptr);
    filehexview->setObjectName("filehexview");
    filehexview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    ui->horizontalLayout->addWidget(filehexview);
    filehexvsb = new QScrollBar(filehexview);
    filehexvsb->setRange(0, 0);
    ui->horizontalLayout->addWidget(filehexvsb);
    connect(filehexview, SIGNAL(rangeChanged(off_t, off_t)), this, SLOT(SetScrollBarRange(off_t, off_t)));
    connect(filehexview, SIGNAL(topLeftChanged(off_t)), this, SLOT(setScrollBarValue(off_t)));
    connect(filehexview, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    connect(filehexvsb, SIGNAL(valueChanged(int)), this, SLOT(AdjustData(int)));
    connect(filehexview, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    connect(filehexview, SIGNAL(StepValues(int, int)), this, SLOT(SetStepValues(int, int)));
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

void FileViewer::LoadPage(off_t pageindex)
{
    qDebug() << "tskptr" << tskptr->offset;
    off_t retval = 0;

    /*
    if(!filereader->nFreePages())
    {
        if(abs(filereader->_firstPage - pageindex) > abs(filereader->_lastPage - pageindex))
            while(!filereader->freePage(filereader->_firstPage++));
        else
            while(!filereader->freePage(filereader->_lastPage--));
    }
    */
    filedata[pageindex] = new uchar[filereader->_pageSize];
    /*
    --filereader->nFreePages();
    */
    if(tskptr->objecttype == 5)
    {
        qDebug() << "blk id:" << tskptr->blkaddrlist.at(pageindex);
        qDebug() << "sector offset:" << tskptr->blkaddrlist.at(pageindex).toInt()*512;
        retval = tsk_fs_read_block(tskptr->readfsinfo, (tskptr->blkaddrlist.at(pageindex).toInt()-1)*filereader->_pageSize, (char*)filedata[pageindex], filereader->_pageSize);
    //retval = tsk_fs_file_read(tskptr->readfileinfo, pageindex*filereader->_pageSize, (char*)filedata[pageindex], filereader->_pageSize, TSK_FS_FILE_READ_FLAG_SLACK);
    qDebug() << "tsk fs file read bytecount:" << retval;
    }
    //retval = tsk_fs_file_read(tskptr->readfileinfo, tskptr->offset + pageindex*filereader->_pageSize, (char*)filedata[pageindex], filereader->_pageSize, TSK_FS_FILE_READ_FLAG_SLACK);
    /*
    if(retval > 0)
    {
        if(pageindex < filereader->_firstPage)
            filereader->_firstPage = pageindex;
        if(pageindex > filereader->_lastPage)
            filereader->_lastPage = pageindex;
    }
    */
}

void FileViewer::AdjustData(int topleft)
{
    int lastpageindex = 0;
    filereader->_eof = false;
    filereader->_offset = max(min((off_t)topleft, filereader->size()-1), (off_t)0);
    size_t bytesread;
    vector<uchar>& v = filehexview->_data;
    int numbytes = (int)filehexview->bytesPerPage();
    if(!filereader->_offset + numbytes >= (int)filereader->size())
    {
        filereader->_eof = true;
        if(filereader->size() == 0)
            v.erase(v.begin(), v.end());
        lastpageindex = filedata.size() - 1;
        bytesread = filereader->size() - filereader->tell();
        numbytes = bytesread;
    }
    else
    {
        lastpageindex = (filereader->_offset + numbytes)/filereader->_pageSize;
        bytesread = numbytes;
    }
    v.erase(v.begin(), v.end());
    v.reserve(v.size() + numbytes);
    for(int page = filereader->_offset/filereader->_pageSize; page <= lastpageindex; page++)
    {
        LoadPage(page);
        int start = filereader->_offset%filereader->_pageSize;
        int stop = (page == lastpageindex) ? start + numbytes : filereader->_pageSize;
        for(int i = start; i < stop; i++)
        {
            v.push_back(filedata[page][i]);
        }
        numbytes -= stop - start;
        filereader->_offset += stop - start;
    }
    filehexview->setTopLeftToPercent(topleft);
}
