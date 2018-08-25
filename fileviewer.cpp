#include "fileviewer.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

//FileViewer::FileViewer(QWidget* parent, TskObject* tskobjptr) : QMainWindow(parent), ui(new Ui::FileViewer)
FileViewer::FileViewer(QWidget* parent) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    //tskptr = tskobjptr;
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(true);
    selectedoffset = new QLabel(this);
    selectedoffset->setText("Offset: 00");
    selectedhex = new QLabel(this);
    selectedhex->setText("Length: 0");
    this->statusBar()->addWidget(selectedoffset, 0);
    this->statusBar()->addWidget(selectedhex, 0);

    /*
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    filehexview = new FileHexViewer(ui->centralwidget, tskptr);
    filehexview->setObjectName("filehexview");
    filehexview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    ui->horizontalLayout->addWidget(filehexview);
    */
    //jumpfilterview = new FileJumpFilter(this);
    /*
    lineup = new QPushButton(QIcon(":/basic/lineup"), "", ui->centralwidget);
    linedown = new QPushButton(QIcon(":/basic/linedown"), "", ui->centralwidget);
    pageup = new QPushButton(QIcon(":/basic/pageup"), "", ui->centralwidget);
    pagedown = new QPushButton(QIcon(":/basic/pagedown"), "", ui->centralwidget);
    lineup->setAutoRepeat(true);
    pagedown->setAutoRepeat(true);
    pageup->setAutoRepeat(true);
    linedown->setVisible(false);
    lineup->setVisible(false);
    pageup->setVisible(false);
    pagedown->setVisible(false);
    hexrocker = new FileSlider(ui->centralwidget);
    hexrocker->setRange(-100, 100);
    hexrocker->setValue(0);
    hexrocker->setSingleStep(1);
    ui->horizontalLayout->addWidget(hexrocker);
    connect(linedown, SIGNAL(clicked()), filehexview, SLOT(nextLine()));
    connect(lineup, SIGNAL(clicked()), filehexview, SLOT(prevLine()));
    connect(pagedown, SIGNAL(clicked()), filehexview, SLOT(nextPage()));
    connect(pageup, SIGNAL(clicked()), filehexview, SLOT(prevPage()));
    connect(hexrocker, SIGNAL(ShowJumpFilter()), jumpfilterview, SLOT(DisplayFilter()));
    */


    //connect(jumpfilterview, SIGNAL(SetOffset()), filehexview, SLOT(SetOffset()));
    
    
    /*
    connect(hexrocker, SIGNAL(sliderMoved(int)), filehexview, SLOT(setTopLeftToPercent(int)));
    connect(hexrocker, SIGNAL(sliderMoved(int)), this, SLOT(ShowRockerToolTip(int)));
    connect(hexrocker, SIGNAL(sliderReleased()), this, SLOT(ResetSlider()));
    connect(filehexview, SIGNAL(SkipDown()), this, SLOT(SkipDown()));
    connect(filehexview, SIGNAL(SkipUp()), this, SLOT(SkipUp()));
    connect(filehexview, SIGNAL(PageUp()), this, SLOT(PageUp()));
    connect(filehexview, SIGNAL(PageDown()), this, SLOT(PageDown()));
    connect(filehexview, SIGNAL(offsetChanged(off_t)), this, SLOT(SetOffsetLabel(off_t)));
    connect(filehexview, SIGNAL(selectionChanged(const QString &)), this, SLOT(UpdateSelectValue(const QString&)));
    */
}

/*
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
*/
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
/*
void FileViewer::SetScrollBarRange(off_t low, off_t high)
{
    (void)low; (void)high;
}
*/
void FileViewer::SetOffsetLabel(off_t pos)
{
    QString label;
    label = "Offset: ";
    /*
    char    buffer[64];
    #if _LARGEFILE_SOURCE
    sprintf(buffer,"0x%lx",pos);
    #else
    sprintf(buffer,"0x%x",pos);
    #endif
    label += buffer;
    */
    selectedoffset->setText(label);
}

void FileViewer::UpdateSelectValue(const QString &txt)
{
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
}

/***** OLD TSK CODE TO GET CONTENT *****/
/*
 *
 *    if(tskptr->objecttype == 5 || tskptr->objecttype == 6)
    {
        if(tskptr->blkaddrlist.count() > 0)
        {
            if(tskptr->blkaddrlist.at(0).toInt() == 0)
            {
                if(tskptr->objecttype == 6)
                {
                    retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, tskptr->mftattrid, 0, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_SLACK);
                }
                else
                {
                    retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, 0, 0, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_NOID);
                }
            }
            else
            {
                if(pageIdx < tskptr->blkaddrlist.count())
                {
                    retval = tsk_fs_read_block(tskptr->readfsinfo, tskptr->blkaddrlist.at(pageIdx).toInt(), (char*)_data[pageIdx], _pageSize);
                }
            }
        }
        else
        {
            if(tskptr->objecttype == 6)
            {
                retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, tskptr->mftattrid, 0, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_SLACK);
            }
            else
            {
                retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, 0, 0, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_NOID);
            }
        }
    }
    */
