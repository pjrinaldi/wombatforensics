#include "wombatfunctions.h"

// Copyright 2015-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

std::string GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

// Sleep for 'ms' milliseconds without freezing the UI
void SleepLoop(const int ms)
{
    QEventLoop idleloop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &idleloop, &QEventLoop::quit);
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, &idleloop, &QEventLoop::quit);
    timer.start(ms);
    idleloop.exec();
}
/*
void LogMessage(QString logmsg)
{
    QByteArray ba;
    ba.clear();
    QString tmpstring = QDateTime::currentDateTime().toString(QString("MM/dd/yyyy hh:mm:ss t"));
    ba.append(tmpstring + "\t" + logmsg  +"\n");
    msglog->append(QString(tmpstring + ": " + logmsg));
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    logfile.write(ba);
    logfile.write(QString(tmpstring + "\t" + logmsg + "\n").toStdString().c_str());
    logfile.close();
}
*/

qint64 GetChildCount(QString filefilter)
{
    qint64 tmpcount = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList files = eviddir.entryList(QStringList(filefilter), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    tmpcount = files.count();
    return tmpcount;
}

QStringList GetChildFiles(QString filefilter)
{
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    return eviddir.entryList(QStringList(filefilter), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
}

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int SegmentDigits(int number)
{
    if(number < 10)
        return 1;
    int count = 0;
    while(number > 0)
    {
        number /= 10;
        count++;
    }
    return count;
}

void GenerateWombatCaseFile(void)
{
    // BEGIN TAR METHOD
    QString tmptar = casepath + "/" + wombatvariable.casename + ".wfc";
    QString oldtmptar = tmptar + ".old";
    if(FileExists(tmptar.toStdString()))
    {
        rename(tmptar.toStdString().c_str(), oldtmptar.toStdString().c_str());
    }
    QByteArray tmparray = tmptar.toLocal8Bit();
    QByteArray tmparray2 = wombatvariable.tmpmntpath.toLocal8Bit();
    QByteArray tmparray3 = QString("./" + wombatvariable.casename).toLocal8Bit();
    TAR* casehandle;
    tar_open(&casehandle, tmparray.data(), NULL, O_WRONLY | O_CREAT, 0644, TAR_GNU);
    tar_append_tree(casehandle, tmparray2.data(), tmparray3.data());
    tar_close(casehandle);
    std::remove(oldtmptar.toStdString().c_str());
    // END TAR METHOD
}

void RewriteSelectedIdContent(QModelIndex selectedindex)
{
    QString selectedid = selectedindex.sibling(selectedindex.row(), 11).data().toString();
    TSK_IMG_INFO* imginfo = NULL;
    std::vector<std::string> pathvector;
    pathvector.clear();

    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QString tmpstr = "";
    QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split(".e").first();
    QString estring = selectedid.split("-", Qt::SkipEmptyParts).at(0);
    QString vstring = selectedid.split("-", Qt::SkipEmptyParts).at(1);
    QString pstring = selectedid.split("-", Qt::SkipEmptyParts).at(2);
    QString fstring = selectedid.split("-", Qt::SkipEmptyParts).at(3);
    QString astring = selectedid.split("-", Qt::SkipEmptyParts).at(4);
    //qDebug() << "astring:" << astring;
    qint64 curaddr = 0;
    QString zipid = "";
    if(fstring.contains("a"))
        curaddr = astring.mid(1).toLongLong();
    else if(fstring.contains("z"))
    {
        zipid = selectedid;
        selectedid = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString();
        fstring = selectedid.split("-", Qt::SkipEmptyParts).at(3);
        curaddr = astring.mid(1).toLongLong();
        //qDebug() << "curaddr:" << curaddr;
        //qDebug() << "selected parent id:" << selectedid;
    }
    else if(fstring.contains("d") || fstring.contains("o"))
        curaddr = fstring.mid(2).toLongLong();
    else
        curaddr = fstring.mid(1).toLongLong();
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    if(!evidfile.isOpen())
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    evidfile.close();
    QString imgfile = tmpstr.split(",").at(3).split("|").at(0);
    if(imgfile.endsWith(".zmg"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + imgfile.split("/").last() + "/" + imgfile.split("/").last().split(".zmg").first() + ".dd").toStdString());
    else if(imgfile.toLower().endsWith(".e01"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + imgfile.split("/").last() + "/" + imgfile.split("/").last() + ".raw").toStdString());
    else if(imgfile.toLower().endsWith(".aff") || imgfile.toLower().endsWith(".000") || imgfile.toLower().endsWith(".001"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + imgfile.split("/").last() + "/" + imgfile.split("/").last() + ".raw").toStdString());
    else if(imgfile.toLower().endsWith(".sfs"))
        pathvector.push_back(QString(wombatvariable.imgdatapath + imgfile.split("/").last() + "/" + imgfile.split("/").last().split(".sfs").first() + ".dd").toStdString());
    else
        pathvector.push_back(tmpstr.split(",").at(3).split("|").at(0).toStdString());
    const TSK_TCHAR** images;
    images = (const char**)malloc(pathvector.size()*sizeof(char*));
    images[0] = pathvector[0].c_str();
    int partcount = tmpstr.split(",").at(3).split("|").size();
    imginfo = tsk_img_open(partcount, images, TSK_IMG_TYPE_DETECT, 0);
    
    if(imginfo == NULL)
    {
        qDebug() << tsk_error_get_errstr();
        //LogMessage("Image opening error");
    }
    tmpstr = "";
    QStringList partlist;
    partlist.clear();
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    if(!partfile.isOpen())
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(partfile.isOpen())
        tmpstr = partfile.readLine();
    partlist = tmpstr.split(",");
    tmpstr = "";
    // NEED TO IMPLEMENT APFS FS OPEN'S HERE
    int fstype = partlist.at(0).toInt();
    TSK_FS_INFO* fsinfo = NULL;
    if(fstype == TSK_FS_TYPE_APFS_DETECT)
    {
        const TSK_POOL_INFO* poolinfo = nullptr;
        poolinfo = tsk_pool_open_img_sing(imginfo, partlist.at(4).toULongLong(), TSK_POOL_TYPE_DETECT);
        if(poolinfo != nullptr)
        {
            if(poolinfo->num_vols > 0)
            {
                for(int i=0; i < poolinfo->num_vols; i++)
                {
                    TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[i];
                    if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                    {
                        //fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidname + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)).toStdString().c_str());
                        fsinfo = tsk_fs_open_img_decrypt(imginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_APFS, passwordhash.value(evidencename + "-" + vstring + "-" + pstring).toStdString().c_str());
                        //fsinfo = tsk_fs_open_pool_decrypt(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT, "encrypted");
                    }
                    else
                    {
                        fsinfo = tsk_fs_open_img(imginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_APFS_DETECT);
                        //fsinfo = tsk_fs_open_pool(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT);
                    }
                }
            }
        }
    }
    else
        fsinfo = tsk_fs_open_img(imginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
    TSK_FS_FILE* fsfile = NULL;
    fsfile = tsk_fs_file_open_meta(fsinfo, NULL, curaddr);
    char* imgbuf = new char[0];
    ssize_t imglen = 0;
    if(fsfile != NULL)
    {
    if(fsfile->meta != NULL)
    {
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
        {
            if(fstring.contains("a")) // IF ADS
            {
                imgbuf = new char[selectedindex.sibling(selectedindex.row(), 2).data().toULongLong()];
		imglen = tsk_fs_file_read_type(fsfile, TSK_FS_ATTR_TYPE_NTFS_DATA, fstring.mid(2).toInt(), 0, imgbuf, selectedindex.sibling(selectedindex.row(), 2).data().toULongLong(), TSK_FS_FILE_READ_FLAG_SLACK);
            }
            else // IF NOT ADS
            {
		imgbuf = new char[fsfile->meta->size];
		imglen = tsk_fs_file_read(fsfile, 0, imgbuf, fsfile->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
            }
        }
        else
        {
	    imgbuf = new char[fsfile->meta->size];
	    imglen = tsk_fs_file_read(fsfile, 0, imgbuf, fsfile->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
        }
    }
    }
    tsk_fs_file_close(fsfile);
    fsfile = NULL;
    tsk_fs_close(fsinfo);
    fsinfo = NULL;
    tsk_img_close(imginfo);
    imginfo = NULL;

    QDir dir;
    dir.mkpath(wombatvariable.tmpfilepath);
    hexstring = wombatvariable.tmpfilepath + selectedid + "-fhex";
    QFile tmpfile(hexstring);
    if(!tmpfile.isOpen())
        tmpfile.open(QIODevice::WriteOnly);
    if(tmpfile.isOpen())
    {
        QDataStream outbuffer(&tmpfile);
        outbuffer.writeRawData(imgbuf, imglen);
    }
    tmpfile.close();
    delete[] imgbuf;
    if(zipid.contains("z"))
    {
        //qDebug() << "zipid addr:" << zipid.split("-").at(3).mid(2).toLongLong();
        int err = 0;
        //qDebug() << "zipid:" << zipid << "hexstring:" << hexstring;
        zip* zfile = zip_open(hexstring.toStdString().c_str(), ZIP_RDONLY, &err);
        struct zip_stat zstat;
        zip_stat_init(&zstat);
        zip_stat_index(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0, &zstat);
        zip_file_t* curfile = NULL;
        if(zstat.encryption_method == ZIP_EM_NONE)
            curfile = zip_fopen_index(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0); // IF NOT ENCRYPTED
        else
        {
            // PROMPT USER FOR PASSWORD HERE....
            curfile = zip_fopen_index_encrypted(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0, "password"); // IF ENCRYPTED (PROMPT USER FOR PASSWORD)...
        }
        QString zhexstring = wombatvariable.tmpfilepath + zipid + "-fhex";
        if(curfile != NULL)
        {
            char* zfbuf = new char[zstat.size];
            qint64 zcnt = zip_fread(curfile, zfbuf, zstat.size);
            zip_fclose(curfile);
            QDir zdir;
            zdir.mkpath(wombatvariable.tmpfilepath);
            QFile ztmp(zhexstring);
            if(!ztmp.isOpen())
                ztmp.open(QIODevice::WriteOnly);
            if(ztmp.isOpen())
            {
                QDataStream zbuffer(&ztmp);
                zbuffer.writeRawData(zfbuf, zcnt);
                ztmp.close();
            }
            delete[] zfbuf;
        }
        zip_close(zfile);
        hexstring = zhexstring;
        //qDebug() << "extract zip content here...";
    }
}

void InitializePasswordList(void)
{
    passwordhash.clear();
    QString tmpstr = "";
    QFile hfile(wombatvariable.tmpmntpath + "passlist");
    QByteArray ba;
    if(hfile.exists())
    {
        if(!hfile.isOpen())
            hfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(hfile.isOpen())
            tmpstr = hfile.readLine();
        hfile.close();
        QStringList hlist = tmpstr.split(",", Qt::SkipEmptyParts);
        for(int i=0; i < hlist.count(); i++)
        {
            ba.clear();
            ba.append(hlist.at(i).split("|", Qt::SkipEmptyParts).at(1).toUtf8());
            passwordhash.insert(hlist.at(i).split("|", Qt::SkipEmptyParts).at(0), QByteArray::fromBase64(ba));
        }
    }
}

void SavePasswordList(void)
{
    QFile hfile(wombatvariable.tmpmntpath + "passlist");
    if(!hfile.isOpen())
        hfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(hfile.isOpen())
    {
        QHashIterator<QString, QString> i(passwordhash);
        while(i.hasNext())
        {
            QByteArray ba;
            ba.clear();
            i.next();
            ba.append(i.value().toUtf8());
            hfile.write(i.key().toStdString().c_str());
            hfile.write("|");
            hfile.write(ba.toBase64().toStdString().c_str());
        }
    }
    hfile.close();
}

QString GenerateCategorySignature(const QMimeType mimetype)
{
    QString geniconstr = mimetype.genericIconName();
    QString mimesignature = mimetype.comment();
    QString mimecategory = "";
    if(geniconstr.contains("document")) // Document
        mimecategory = "Document";
    else if(geniconstr.contains("video")) // Video
        if(mimesignature.contains("WPL"))
            mimecategory = "HTML";
        else
            mimecategory = "Video";
    else if(geniconstr.contains("image")) // Image
        mimecategory = "Image";
    else if(geniconstr.contains("package")) // Archive
        mimecategory = "Archive";
    else if(geniconstr.contains("font")) // Font
        mimecategory = "Font";
    else if(geniconstr.contains("text")) // Text
        if(mimesignature.contains("email") || mimesignature.contains("mail") || mimesignature.contains("reference to remote file"))
            mimecategory = "Email";
        else if(mimesignature.contains("html", Qt::CaseInsensitive))
            mimecategory = "HTML";
        else
            mimecategory = "Text";
    else if(geniconstr.contains("audio")) // Audio
        mimecategory = "Audio";
    else if(geniconstr.contains("spreadsheet")) // Office Spreadsheet
        mimecategory = "Spreadsheet";
    else if(geniconstr.contains("presentation")) // Office Presentation
        mimecategory = "Presentation";
    else if(geniconstr.contains("multipart")) // MultiPart
        mimecategory = "MultiPart";
    else if(geniconstr.contains("inode")) // Inode
        mimecategory = "Inode";
    else if(geniconstr.contains("model")) // Binary
        mimecategory = "Binary";
    else if(geniconstr.contains("application-x")) // Try iconName() database, java, document, text, image, executable, certificate, bytecode, library, Data, Trash, zerosize, 
    {
       if(mimesignature.contains("certificate") || mimesignature.contains("private key") || mimesignature.contains("keystore")) 
            mimecategory = "Certificate";
       else if(mimesignature.contains("Metafile") || mimesignature.contains("profile"))
            mimecategory = "Metafile";
       else if(mimesignature.contains("video"))
           mimecategory = "Video";
       else if(mimesignature.contains("TNEF message") || mimesignature.contains("email"))
           mimecategory = "Email";
       else if(mimesignature.contains("Microsoft Word Document") || mimesignature.contains("OpenDocument Master Document Template") || mimesignature.contains("MIF"))
           mimecategory = "Document";
       else if(mimesignature.contains("ROM") || mimesignature.contains("Atari") || mimesignature.contains("Thomson"))
           mimecategory = "ROM";
       else if(mimesignature.contains("database") || mimesignature.contains("Database") || mimesignature.contains("SQL"))
           mimecategory = "Database";
       else if(mimesignature.contains("filesystem") || mimesignature.contains("disk image") || mimesignature.contains("AppImage") || mimesignature.contains("WiiWare"))
           mimecategory = "Disk Image";
       else if(mimesignature.contains("executable") || mimesignature.contains("Windows Intaller") || mimesignature.contains("library"))
           mimecategory = "Executable";
       else if(mimesignature.contains("Internet shortcut") || mimesignature.contains("backup file") || mimesignature.contains("VBScript") || mimesignature.contains("RDF") || mimesignature.contains("Streaming playlist") || mimesignature.contains("cache file") || mimesignature.contains("Remmina") || mimesignature.contains("GML") || mimesignature.contains("GPX") || mimesignature.contains("MathML") || mimesignature.contains("Metalink") || mimesignature.contains("XML") || mimesignature.contains("RDF") || mimesignature.contains("KML") || mimesignature.contains("FictionBook") || mimesignature.contains("NewzBin"))
           mimecategory = "Text";
       else if(mimesignature.contains("Windows animated cursor"))
           mimecategory = "Image";
       else if(mimesignature.contains("SPSS") || mimesignature.contains("MHTML"))
           mimecategory = "Archive";
       else if(mimesignature.contains("empty"))
           mimecategory = "Empty File";
       else
           mimecategory = "Binary";
    }
    else if(geniconstr.contains("x-content-x-generic")) 
    {
        if(mimesignature.contains("audio"))
            mimecategory = "Audio";
        else if(mimesignature.contains("blank"))
            mimecategory = "Disk Image";
        else if(mimesignature.contains("e-book"))
            mimecategory = "Document";
        else if(mimesignature.contains("photos") || mimesignature.contains("Picture"))
            mimecategory = "Image";
        else if(mimesignature.contains("software"))
            mimecategory = "Executable";
        else if(mimesignature.contains("video") || mimesignature.contains("Video"))
            mimecategory = "Video";
    }
    if(mimesignature.contains("unknown"))
        mimesignature = "Unknown";

    return QString(mimecategory + "/" + mimesignature);
}


QString ConvertGmtHours(int gmtvar)
{
    int tmpmin = gmtvar - 48;
    int gmthr = tmpmin / 4;
    int gmtmin = (tmpmin % 4)*15;
    QString tmpstring = " GMT ";
    if(abs(gmthr) < 10)
        tmpstring += "0";
    tmpstring += QString::number(gmthr) + QString::number(gmtmin);
    if(gmtmin == 0)
        tmpstring += "0";

    return tmpstring;

}

qint64 ConvertDosTimeToUnixTime(uint8_t t1, uint8_t t2, uint8_t d1, uint8_t d2)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    QString tmpdate = QString("%1%2").arg(d1, 8, 2, QChar('0')).arg(d2, 8, 2, QChar('0'));
    QString tmptime = QString("%1%2").arg(t1, 8, 2, QChar('0')).arg(t2, 8, 2, QChar('0'));
    year = tmpdate.left(7).toInt(nullptr, 2) + 1980;
    month = tmpdate.mid(7, 4).toInt(nullptr, 2);
    day = tmpdate.right(5).toInt(nullptr, 2);
    hour = tmptime.left(5).toInt(nullptr, 2);
    min = tmptime.mid(5, 6).toInt(nullptr, 2);
    sec = tmptime.right(5).toInt(nullptr, 2) * 2;
    //qDebug()  << QString::number(year) + "-" + QString::number(month) + "-" + QString::number(day) + " " + QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec);
    QString datetimestring = QString("%1-%2-%3 %4:%5:%6").arg(year, 4, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(day, 2, 10, QChar('0')).arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    return QDateTime::fromString(datetimestring, "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch();
    // return QDateTime::fromString("datetimestring", "string format").toSecsSinceEpoch(); // use QString.arg() since DateTime is greedy when reading...
    //QString timestr = "";
    // if t1 and t2 == 0, then it's a date only... so set time var's as 00:00:00 to put into string and get the unixepoch integer
    //QString datetest = QString("%1%2").arg(rootdirbuf.at(i*32 + 17), 8, 2, QChar('0')).arg(rootdirbuf.at(i*32 + 16), 8, 2, QChar('0'));
    //qDebug() << datetest;
    //qDebug() << "year:" << 1980 + datetest.left(7).toInt(nullptr, 2) << "month:" << datetest.mid(7, 4).toInt(nullptr, 2) << "day:" << datetest.right(5).toInt(nullptr, 2);
    //QString timetest = QString("%1%2").arg(rootdirbuf.at(i*32 + 15), 8, 2, QChar('0')).arg(rootdirbuf.at(i*32 + 14), 8, 2, QChar('0'));
    //qDebug() << "hour:" << timetest.left(5).toInt(nullptr, 2) << "min:" << timetest.mid(5, 6).toInt(nullptr, 2) << "sec:" << timetest.right(5).toInt(nullptr, 2) * 2;
    //return 0;
}
QString ConvertWindowsTimeToUnixTime(uint64_t input)
{
    QTimeZone tmpzone = QTimeZone(reporttimezone);
    uint64_t temp;
    temp = input / TICKS_PER_SECOND; //convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  //subtract number of seconds between epochs
    time_t crtimet = (time_t)temp;
    QString timestr = "";
    timestr = QDateTime::fromSecsSinceEpoch(crtimet, tmpzone).toString("MM/dd/yyyy hh:mm:ss AP");

    return timestr;
}

QString ConvertWindowsTimeToUnixTimeUTC(uint64_t input)
{
    //QTimeZone tmpzone = QTimeZone(reporttimezone);
    uint64_t temp;
    temp = input / TICKS_PER_SECOND; //convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  //subtract number of seconds between epochs
    time_t crtimet = (time_t)temp;
    QString timestr = "";
    timestr = QDateTime::fromSecsSinceEpoch(crtimet, QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");

    return timestr;
}

QString ConvertUnixTimeToString(uint32_t input)
{
    //QTimeZone tmpzone = QTimeZone(reporttimezone);
    time_t crtimet = (time_t)input;
    QString timestr = QDateTime::fromSecsSinceEpoch(crtimet, QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");

    return timestr;
}
