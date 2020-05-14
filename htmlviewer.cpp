#include "htmlviewer.h"
//#include <shell_items.h>

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

HtmlViewer::HtmlViewer(QWidget* parent) : QDialog(parent), ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
    this->close();
}

void HtmlViewer::HideClicked()
{
    this->close();
}

void HtmlViewer::ShowHtml(const QModelIndex &index)
{
    //curobjaddr = index.sibling(index.row(), 11).data().toString().toLongLong();
    QString oldfile = wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex";
    QString newfile = oldfile + ".html";
    QFile::copy(oldfile, newfile);
    ui->webView->setUrl(QUrl::fromLocalFile(newfile));
    this->show();
}

void HtmlViewer::ShowLnk(const QModelIndex &index)
{
    /*
     *
     * 1. create a function which returns the htmlstring content....
     * 2. when tag file, write the return htmlstring to the file at showcontent...
     * 3. store time as qdatetime so i can convert timezone... look into what i can import to qdatetime, time_t, tm, etc...
     *
    HTML JS FUNCTION: ShowContent('\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'
     */  
    QString htmlstr = "";
    QFile initfile(":/html/initialhtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    //QString htmlstr = "<html><head><title>" + index.sibling(index.row(), 11).data().toString() + "</title></head>";
    htmlstr += "<div id='infotitle'>LNK File Analysis for " + index.sibling(index.row(), 0).data().toString() + " (" + index.sibling(index.row(), 11).data().toString() + ")</div><br/>";
    htmlstr += "<table width='600px'><tr><th colspan='1'>NAME</th><th>VALUE</th></tr>";
    QString lnkfile = wombatvariable.tmpfilepath + index.sibling(index.row(), 11).data().toString() + "-fhex";
    liblnk_error_t* error = NULL;
    liblnk_file_t* lnkobj = NULL;
    liblnk_file_initialize(&lnkobj, &error);
    liblnk_file_open(lnkobj, lnkfile.toStdString().c_str(), liblnk_get_access_flags_read(), &error);
    if(liblnk_check_file_signature(lnkfile.toStdString().c_str(), &error))
    {
        if(liblnk_file_link_refers_to_file(lnkobj, &error))
        {
	    std::string timestr = "";
            uint64_t gettime = 0;
	    uint32_t tmpuint32 = 0;
	    size_t tmpsize = 0;
            liblnk_file_get_file_creation_time(lnkobj, &gettime, &error);
            timestr = ConvertWindowsTimeToUnixTime(gettime);
	    htmlstr += "<tr class='odd vtop'><td class='pvalue'>Creation Time:</td><td class='property'>" + QString::fromStdString(timestr) + " UTC</td></tr>";
	    gettime = 0;
	    liblnk_file_get_file_modification_time(lnkobj, &gettime, &error);
	    timestr = ConvertWindowsTimeToUnixTime(gettime);
	    htmlstr += "<tr class='even'><td class='pvalue'>Modification Time:</td><td class='property'>" + QString::fromStdString(timestr) + " UTC</td></tr>";
	    gettime = 0;
	    liblnk_file_get_file_access_time(lnkobj, &gettime, &error);
	    timestr = ConvertWindowsTimeToUnixTime(gettime);
	    htmlstr += "<tr class='odd'><td class='pvalue'>Access Time:</td><td class='property'>" + QString::fromStdString(timestr) + " UTC</td></tr>";
	    liblnk_file_get_file_size(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr class='even'><td class='pvalue'>File Size:</td><td class='property'>" + QString::number(tmpuint32) + " bytes</td></tr>";
	    tmpuint32 = 0;
	    liblnk_file_get_file_attribute_flags(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr class='odd'><td class='pvalue'>File Attributes:</td><td class='property'>0x" + QString("%1").arg(tmpuint32, 8, 16, QChar('0')) + "<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_READ_ONLY) != 0)
		htmlstr += "Read Only (FILE_ATTRIBUTE_READ_ONLY)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_HIDDEN) != 0)
		htmlstr += "Hidden (FILE_ATTRIBUTE_HIDDEN)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_SYSTEM) != 0)
		htmlstr += "System (FILE_ATTRIBUTE_SYSTEM)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_DIRECTORY) != 0)
		htmlstr += "Directory (FILE_ATTRIBUTE_DIRECTORY)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_ARCHIVE) != 0)
		htmlstr += "Archived (FILE_ATTRIBUTE_ARCHIVE)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_DEVICE) != 0)
		htmlstr += "Device (FILE_ATTRIBUTE_DEVICE)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_NORMAL) != 0)
		htmlstr += "Normal (FILE_ATTRIBUTE_NORMAL)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_TEMPORARY) != 0)
		htmlstr += "Temporary (FILE_ATTRIBUTE_TEMPORARY)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_SPARSE_FILE) != 0)
		htmlstr += "Sparse File (FILE_ATTRIBUTE_SPARSE)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_REPARSE_POINT) != 0)
		htmlstr += "Reparse Point (FILE_ATTRIBUTE_REPARSE_POINT)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_COMPRESSED) != 0)
		htmlstr += "Compressed (FILE_ATTRIBUTE_COMPRESSED)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_OFFLINE) != 0)
		htmlstr += "Offline (FILE_ATTRIBUTE_OFFLINE)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED) != 0)
		htmlstr += "Content should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_ENCRYPTED) != 0)
		htmlstr += "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)<br/>";
	    if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_VIRTUAL) != 0)
		htmlstr += "Virtual (FILE_ATTRIBUTE_VIRTUAL)<br/>";
	    htmlstr += "</td></tr>";
	    tmpuint32 = 0;
	    liblnk_file_get_drive_type(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr class='even'><td class='pvalue'>Drive Type:</td><td class='property'>";
	    switch(tmpuint32)
	    {
		case LIBLNK_DRIVE_TYPE_UNKNOWN:
		    htmlstr += "Unknown)";
		    break;
		case LIBLNK_DRIVE_TYPE_NO_ROOT_DIR:
		    htmlstr += "No root directory";
		    break;
		case LIBLNK_DRIVE_TYPE_REMOVABLE:
		    htmlstr += "Removable";
		    break;
		case LIBLNK_DRIVE_TYPE_FIXED:
		    htmlstr += "Fixed";
		    break;
		case LIBLNK_DRIVE_TYPE_REMOTE:
		    htmlstr += "Remote";
		    break;
		case LIBLNK_DRIVE_TYPE_CDROM:
		    htmlstr += "CDROM";
		    break;
		case LIBLNK_DRIVE_TYPE_RAMDISK:
		    htmlstr += "Ram disk";
		    break;
		default:
		    htmlstr += "Unknown";
		    break;
	    }
	    htmlstr += " (" + QString::number(tmpuint32) + ")</td></tr>";
	    tmpuint32 = 0;
	    liblnk_file_get_drive_serial_number(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr class='odd'><td class='pvalue'>Drive Serial Number:</td><td class='property'>0x" + QString::number(tmpuint32, 16) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_volume_label_size(lnkobj, &tmpsize, &error);
	    uint8_t volabel[tmpsize];
	    liblnk_file_get_utf8_volume_label(lnkobj, volabel, tmpsize, &error);
	    //qDebug() << "tmpsize:" << tmpsize;
	    //qDebug() << "volabel:" << QString::fromUtf8(reinterpret_cast<char*>(volabel));
	    htmlstr += "<tr class='even'><td class='pvalue'>Volume Label:</td><td class='property'>" + QString::fromUtf8(reinterpret_cast<char*>(volabel)) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_local_path_size(lnkobj, &tmpsize, &error);
	    uint8_t localpath[tmpsize];
	    liblnk_file_get_utf8_local_path(lnkobj, localpath, tmpsize, &error);
	    htmlstr += "<tr class='odd'><td class='pvalue'>Local Path:</td><td class='property'>" + QString::fromUtf8(reinterpret_cast<char*>(localpath)) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_working_directory_size(lnkobj, &tmpsize, &error);
	    uint8_t workdir[tmpsize];
	    liblnk_file_get_utf8_working_directory(lnkobj, workdir, tmpsize, &error);
	    htmlstr += "<tr class='even'><td class='pvalue'>Working Directory:</td><td class='property'>" + QString::fromUtf8(reinterpret_cast<char*>(workdir)) + "</td></tr>";
        }
    }
    liblnk_file_close(lnkobj, &error);
    liblnk_file_free(&lnkobj, &error);
    htmlstr += "</table></body></html>";
    ui->webView->setHtml(htmlstr);
    this->show();
    //curobjaddr = index.sibling(index.row(), 11).data().toString().toULongLong()
}

void HtmlViewer::LoadHtml(QString filepath)
{
    // populate html here
    ui->webView->setUrl(QUrl::fromLocalFile(filepath));
    this->show();
}

void HtmlViewer::Reload()
{
    ui->webView->reload();
}

void HtmlViewer::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
        isignals->ActivateReload();
}

void HtmlViewer::closeEvent(QCloseEvent* e)
{
    emit HideReportPreviewWindow(false);
    e->accept();
}
