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
    QString htmlstr = "<html><head><title>" + index.sibling(index.row(), 11).data().toString() + "</title></head>";
    htmlstr += "<body><h5>LNK File Analysis for " + index.sibling(index.row(), 0).data().toString() + " (" + index.sibling(index.row(), 11).data().toString() + ")</h5><br/>";
    htmlstr += "<table style='border-collapse: collapse;border: 1px solid black;'><tr><th>NAME</th><th>VALUE</th></tr>";
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
	    //uint8_t tmpuint8 = 0;
	    //uint8_t* uint8ptr = NULL;
            liblnk_file_get_file_creation_time(lnkobj, &gettime, &error);
            timestr = ConvertWindowsTimeToUnixTime(gettime);
	    htmlstr += "<tr><td>Creation Time:</td><td>" + QString::fromStdString(timestr) + " UTC</td></tr>";
	    gettime = 0;
	    liblnk_file_get_file_modification_time(lnkobj, &gettime, &error);
	    timestr = ConvertWindowsTimeToUnixTime(gettime);
	    htmlstr += "<tr><td>Modification Time:</td><td>" + QString::fromStdString(timestr) + " UTC</td></tr>";
	    gettime = 0;
	    liblnk_file_get_file_access_time(lnkobj, &gettime, &error);
	    timestr = ConvertWindowsTimeToUnixTime(gettime);
	    htmlstr += "<tr><td>Access Time:</td><td>" + QString::fromStdString(timestr) + " UTC</td></tr>";
	    liblnk_file_get_file_size(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr><td>File Size:</td><td>" + QString::number(tmpuint32) + " bytes</td></tr>";
	    tmpuint32 = 0;
	    liblnk_file_get_file_attribute_flags(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr><td>File Attributes:</td><td>0x" + QString("%1").arg(tmpuint32, 8, 16, QChar('0')) + "<br/>";
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
	    htmlstr += "<tr><td>Drive Type:</td><td>";
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
	    htmlstr += "<tr><td>Drive Serial Number:</td><td>0x" + QString::number(tmpuint32, 16) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_volume_label_size(lnkobj, &tmpsize, &error);
	    uint8_t volabel[tmpsize];
	    liblnk_file_get_utf8_volume_label(lnkobj, volabel, tmpsize, &error);
	    //qDebug() << "tmpsize:" << tmpsize;
	    //qDebug() << "volabel:" << QString::fromUtf8(reinterpret_cast<char*>(volabel));
	    htmlstr += "<tr><td>Volume Label:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(volabel)) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_local_path_size(lnkobj, &tmpsize, &error);
	    uint8_t localpath[tmpsize];
	    liblnk_file_get_utf8_local_path(lnkobj, localpath, tmpsize, &error);
	    htmlstr += "<tr><td>Local Path:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(localpath)) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_working_directory_size(lnkobj, &tmpsize, &error);
	    uint8_t workdir[tmpsize];
	    liblnk_file_get_utf8_working_directory(lnkobj, workdir, tmpsize, &error);
	    htmlstr += "<tr><td>Working Directory:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(workdir)) + "</td></tr>";
	    /*
	    tmpsize = 0;
	    liblnk_file_get_link_target_identifier_data_size(lnkobj, &tmpsize, &error);
	    qDebug() << "shell item tmpsize:" << tmpsize;
	    uint8_t shelllist[tmpsize];
	    liblnk_file_copy_link_target_identifier_data(lnkobj, shelllist, tmpsize, &error);
	    */

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
