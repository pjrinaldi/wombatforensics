#include "wombatfunctions.h"

// Copyright 2015-2020 Pasquale J. Rinaldi, Jr.
// Distributed under the terms of the GNU General Public License version 2

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
    QString selectedid = selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("id")).data().toString();
    //QString sizestr = selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("size")).data().toString();
    //this gets the comma'd size, would have to convert to qstring number before i try to use as size
    //qDebug() << "sizestr:" << sizestr;
    for(int i=0; i < existingforimglist.count(); i++)
    {
	ForImg* curimg = existingforimglist.at(i);
	if(curimg->MountPath().endsWith(selectedid.split("-").at(0)))
	{
	    QString layout = "";
	    if(curimg->ImgType() == 15)
	    {
		layout = "0," + QString::number(curimg->Size()) + ";";
	    }
	    else
	    {
		//qDebug() << curimg->ImgPath() << curimg->MountPath();
		QString filename = selectedindex.sibling(selectedindex.row(), treenodemodel->GetColumnIndex("name")).data().toString();
		if(filename.startsWith("$FAT"))
		{
		    uint fatoffset = 0;
		    uint fatsize = 0;
		    uint bytespersector = 0;
		    QFile ppropfile(curimg->MountPath() + "/" + selectedid.split("-").at(1) + "/prop");
		    //QFile ppropfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + nodeid.split("-").at(1) + "/prop");
		    ppropfile.open(QIODevice::ReadOnly | QIODevice::Text);
		    if(ppropfile.isOpen())
		    {
			while(!ppropfile.atEnd())
			{
			    QString tmpstr = ppropfile.readLine();
			    if(tmpstr.startsWith("FAT Offset"))
				fatoffset = tmpstr.split("|").at(1).toUInt();
			    else if(tmpstr.startsWith("Bytes Per Sector"))
				bytespersector = tmpstr.split("|").at(1).toUInt();
			    else if(tmpstr.startsWith("FAT Size"))
				fatsize = tmpstr.split("|").at(1).toUInt() * bytespersector;
			}
			ppropfile.close();
		    }
		    uint fatnum = filename.right(1).toUInt();
		    layout = QString(QString::number(fatoffset + fatsize * (fatnum - 1)) + "," + QString::number(fatsize) + ";");
		    //ui->hexview->setCursorPosition((fatoffset + fatsize * (fatnum - 1)) * 2);
		    //ui->hexview->SetColor(QString(QString::number(fatoffset + fatsize * (fatnum - 1)) + "," + QString::number(fatsize) + ";"), fatsize - 1);
		}
		else if(filename == "$MBR")
		{
		    layout = "0,512;";
		    //ui->hexview->SetColor(QString("0,512;"), 512);
		}
		else
		{
		    QFile fpropfile(curimg->MountPath() + "/" + selectedid.split("-").at(1) + "/" + selectedid.split("-").at(2) + ".prop");
		    if(!fpropfile.isOpen())
			fpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
		    while(!fpropfile.atEnd())
		    {
			QString line = fpropfile.readLine();
			if(line.startsWith("Layout|"))
			{
			    layout = line.split("|").at(1);
			    break;
			}
		    }
		    fpropfile.close();
		}
	    }
	    QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
	    QDir dir;
	    dir.mkpath(wombatvariable.tmpfilepath);
	    hexstring = wombatvariable.tmpfilepath + selectedid + "-fhex";
	    QFile tmpfile(hexstring);
	    if(!tmpfile.isOpen())
		tmpfile.open(QIODevice::WriteOnly);
	    if(tmpfile.isOpen())
	    {
		for(int j=0; j < layoutlist.count(); j++)
		{
		    QByteArray filearray = curimg->ReadContent(layoutlist.at(j).split(",").at(0).toULongLong(), layoutlist.at(j).split(",").at(1).toULongLong());
		    tmpfile.write(filearray);
		}
		tmpfile.close();
	    }
        }
    }
    /*
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*-" + selectedid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
    QString evidencename = evidfiles.at(0).split("-e").first();
    QString tmpstr = "";
    QFile estatfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/stat");
    if(!estatfile.isOpen())
        estatfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(estatfile.isOpen())
    {
        tmpstr = estatfile.readLine();
        estatfile.close();
    }
    QByteArray filecontent;
    filecontent.clear();
    QString layout = "";
    QFile fpropfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/" + selectedid.split("-").at(1) + "/" + selectedid.split("-").at(2) + ".prop");
    if(!fpropfile.isOpen())
        fpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(fpropfile.isOpen())
    {
        QString line = "";
        while(!fpropfile.atEnd())
        {
            line = fpropfile.readLine();
            if(line.startsWith("Layout"))
            {
                layout = line.split("|").at(1);
                break;
            }
        }
        fpropfile.close();
    }
    */
    //QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
    /*
     * MODIFY TO BE curimg->ReadContent()
     * ALSO INTEGRATE INTO THE READ/WRITE AS ONE SO I DON'T FILL UP MEMORY FOR A BIG FILE
    QFile efile(tmpstr.split(",", Qt::SkipEmptyParts).at(1));
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        for(int i=0; i < layoutlist.count(); i++)
        {
            efile.seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
            filecontent.append(efile.read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
        }
        efile.close();
    }
    */
    /*
    QDir dir;
    dir.mkpath(wombatvariable.tmpfilepath);
    hexstring = wombatvariable.tmpfilepath + selectedid + "-fhex";
    QFile tmpfile(hexstring);
    if(!tmpfile.isOpen())
        tmpfile.open(QIODevice::WriteOnly);
    if(tmpfile.isOpen())
    {
        tmpfile.write(filecontent);
        tmpfile.close();
    }
    */
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

QString GenerateCategorySignature(QByteArray sigbuf, QString filename)
{
    QString mimestr = "";
    if(filename.startsWith("$UPCASE_TABLE"))
	mimestr = "System File/Up-case Table";
    else if(filename.startsWith("$ALLOC_BITMAP"))
	mimestr = "System File/Allocation Bitmap";
    else if(filename.startsWith("$UpCase"))
	mimestr = "Windows System/System File";
    else if(filename.startsWith("$MFT") || filename.startsWith("$MFTMirr") || filename.startsWith("$LogFile") || filename.startsWith("$Volume") || filename.startsWith("$AttrDef") || filename.startsWith("$Bitmap") || filename.startsWith("$Boot") || filename.startsWith("$BadClus") || filename.startsWith("$Secure") || filename.startsWith("$Extend"))
        mimestr = "Windows System/System File";
    else
    {
	// NON-QT WAY USING LIBMAGIC
	//QByteArray sigbuf = curimg->ReadContent(fileoffset, 1024);
	magic_t magical;
	const char* catsig;
	magical = magic_open(MAGIC_MIME_TYPE);
	magic_load(magical, NULL);
	catsig = magic_buffer(magical, sigbuf.data(), sigbuf.count());
	std::string catsigstr(catsig);
	mimestr = QString::fromStdString(catsigstr);
	magic_close(magical);
	for(int i=0; i < mimestr.count(); i++)
	{
	    if(i == 0 || mimestr.at(i-1) == ' ' || mimestr.at(i-1) == '-' || mimestr.at(i-1) == '/')
		mimestr[i] = mimestr[i].toUpper();
	}
	if(mimestr.contains("Application/Octet-Stream"))
	{
	    if(sigbuf.at(0) == '\x4c' && sigbuf.at(1) == '\x00' && sigbuf.at(2) == '\x00' && sigbuf.at(3) == '\x00' && sigbuf.at(4) == '\x01' && sigbuf.at(5) == '\x14' && sigbuf.at(6) == '\x02' && sigbuf.at(7) == '\x00') // LNK File
		mimestr = "Windows System/Shortcut";
	    else if(strcmp(filename.toStdString().c_str(), "INFO2") == 0 && (sigbuf.at(0) == 0x04 || sigbuf.at(0) == 0x05))
		mimestr = "Windows System/Recycler";
	    else if(filename.startsWith("$I") && (sigbuf.at(0) == 0x01 || sigbuf.at(0) == 0x02))
		mimestr = "Windows System/Recycle.Bin";
	    else if(filename.endsWith(".pf") && sigbuf.at(4) == 0x53 && sigbuf.at(5) == 0x43 && sigbuf.at(6) == 0x43 && sigbuf.at(7) == 0x41)
		mimestr = "Windows System/Prefetch";
	    else if(filename.endsWith(".pf") && sigbuf.at(0) == 0x4d && sigbuf.at(1) == 0x41 && sigbuf.at(2) == 0x4d)
		mimestr = "Windows System/Prefetch";
	    else if(sigbuf.at(0) == '\x72' && sigbuf.at(1) == '\x65' && sigbuf.at(2) == '\x67' && sigbuf.at(3) == '\x66') // 72 65 67 66 | regf
		mimestr = "Windows System/Registry";
	}
    }

    return mimestr;
    /*
    QString mimestr = "";
    QMimeDatabase mimedb;
    const QMimeType mimetype = mimedb.mimeTypeForData(sigbuf);
    QString geniconstr = mimetype.genericIconName();
    QString mimesignature = mimetype.comment();
    //qDebug() << "filename:" << filename << "mimesignature:" << mimesignature;
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
    {
        //mimesignature = "Unknown";
        if(sigbuf.at(0) == '\x4c' && sigbuf.at(1) == '\x00' && sigbuf.at(2) == '\x00' && sigbuf.at(3) == '\x00' && sigbuf.at(4) == '\x01' && sigbuf.at(5) == '\x14' && sigbuf.at(6) == '\x02' && sigbuf.at(7) == '\x00') // LNK File
            mimestr = "Windows System/Shortcut";
        else if(strcmp(filename.toStdString().c_str(), "INFO2") == 0 && (sigbuf.at(0) == 0x04 || sigbuf.at(0) == 0x05))
            mimestr = "Windows System/Recycler";
        else if(filename.startsWith("$I") && (sigbuf.at(0) == 0x01 || sigbuf.at(0) == 0x02))
            mimestr = "Windows System/Recycle.Bin";
        else if(filename.endsWith(".pf") && sigbuf.at(4) == 0x53 && sigbuf.at(5) == 0x43 && sigbuf.at(6) == 0x43 && sigbuf.at(7) == 0x41)
            mimestr = "Windows System/Prefetch";
        else if(filename.endsWith(".pf") && sigbuf.at(0) == 0x4d && sigbuf.at(1) == 0x41 && sigbuf.at(2) == 0x4d)
            mimestr = "Windows System/Prefetch";
        else if(sigbuf.at(0) == '\x72' && sigbuf.at(1) == '\x65' && sigbuf.at(2) == '\x67' && sigbuf.at(3) == '\x66') // 72 65 67 66 | regf
            mimestr = "Windows System/Registry";
        else if(filename.startsWith("$ALLOC_BITMAP"))
            mimestr = "System File/Allocation Bitmap";
        else if(filename.startsWith("$MFT") || filename.startsWith("$MFTMirr") || filename.startsWith("$LogFile") || filename.startsWith("$Volume") || filename.startsWith("$AttrDef") || filename.startsWith("$Bitmap") || filename.startsWith("$Boot") || filename.startsWith("$BadClus") || filename.startsWith("$Secure") || filename.startsWith("$Extend"))
            mimestr = "Windows System/System File";
    }
    if(filename.startsWith("$UPCASE_TABLE"))
        mimestr = "System File/Up-case Table";
    if(filename.startsWith("$UpCase"))
        mimestr = "Windows System/System File";
    //if(orphanlist->at(j).value("itemtype").toUInt() == 2)
    //    mimestr = "Directory/Directory";
		    //else if(sigbuf.left(4) == "FILE")
			//mimestr = "Windows System/MFT File Entry";
    if(mimestr.isEmpty())
        return QString(mimecategory + "/" + mimesignature);
    else
        return mimestr;
    */
}

QString GenerateCategorySignature(ForImg* curimg, QString filename, qulonglong fileoffset)
{
    QString mimestr = "";
    if(filename.startsWith("$UPCASE_TABLE"))
	mimestr = "System File/Up-case Table";
    else if(filename.startsWith("$ALLOC_BITMAP"))
	mimestr = "System File/Allocation Bitmap";
    else if(filename.startsWith("$UpCase"))
	mimestr = "Windows System/System File";
    else if(filename.startsWith("$MFT") || filename.startsWith("$MFTMirr") || filename.startsWith("$LogFile") || filename.startsWith("$Volume") || filename.startsWith("$AttrDef") || filename.startsWith("$Bitmap") || filename.startsWith("$Boot") || filename.startsWith("$BadClus") || filename.startsWith("$Secure") || filename.startsWith("$Extend"))
        mimestr = "Windows System/System File";
    else
    {
	// NON-QT WAY USING LIBMAGIC
	QByteArray sigbuf = curimg->ReadContent(fileoffset, 1024);
	magic_t magical;
	const char* catsig;
	//magical = magic_open(MAGIC_NONE);
	magical = magic_open(MAGIC_MIME_TYPE);
	magic_load(magical, NULL);
	catsig = magic_buffer(magical, sigbuf.data(), sigbuf.count());
	std::string catsigstr(catsig);
	mimestr = QString::fromStdString(catsigstr);
	magic_close(magical);
	for(int i=0; i < mimestr.count(); i++)
	{
	    if(i == 0 || mimestr.at(i-1) == ' ' || mimestr.at(i-1) == '-' || mimestr.at(i-1) == '/')
		mimestr[i] = mimestr[i].toUpper();
	}
	//qDebug() << "filename:" << filename << "mimestr:" << mimestr;
	if(mimestr.contains("Application/Octet-Stream"))
	{
	    if(sigbuf.at(0) == '\x4c' && sigbuf.at(1) == '\x00' && sigbuf.at(2) == '\x00' && sigbuf.at(3) == '\x00' && sigbuf.at(4) == '\x01' && sigbuf.at(5) == '\x14' && sigbuf.at(6) == '\x02' && sigbuf.at(7) == '\x00') // LNK File
		mimestr = "Windows System/Shortcut";
	    else if(strcmp(filename.toStdString().c_str(), "INFO2") == 0 && (sigbuf.at(0) == 0x04 || sigbuf.at(0) == 0x05))
		mimestr = "Windows System/Recycler";
	    else if(filename.startsWith("$I") && (sigbuf.at(0) == 0x01 || sigbuf.at(0) == 0x02))
		mimestr = "Windows System/Recycle.Bin";
	    else if(filename.endsWith(".pf") && sigbuf.at(4) == 0x53 && sigbuf.at(5) == 0x43 && sigbuf.at(6) == 0x43 && sigbuf.at(7) == 0x41)
		mimestr = "Windows System/Prefetch";
	    else if(filename.endsWith(".pf") && sigbuf.at(0) == 0x4d && sigbuf.at(1) == 0x41 && sigbuf.at(2) == 0x4d)
		mimestr = "Windows System/Prefetch";
	    else if(sigbuf.at(0) == '\x72' && sigbuf.at(1) == '\x65' && sigbuf.at(2) == '\x67' && sigbuf.at(3) == '\x66') // 72 65 67 66 | regf
		mimestr = "Windows System/Registry";
	}
        else if(mimestr.contains("Text/"))
        {
            if(filename.endsWith(".mbox"))
            {
                mimestr = "Email/MBox";
            }
        }
        else if(mimestr.contains("/Zip"))
        {
	    //qDebug() << "filename:" << filename << "mimestr:" << mimestr;
	    QByteArray officecheck = curimg->ReadContent(fileoffset, 4096);
	    //qDebug() << QString(officecheck.toHex());
	    if(officecheck.toHex().contains(QString("776f72642f").toStdString().c_str())) // "word/" (.docx)
	    {
		mimestr = "Office Document/Microsoft Word 2007+";
		//qDebug() << "it's a word document...";
	    }
	    else if(officecheck.toHex().contains(QString("78gc2f").toStdString().c_str())) // "xl/" (.xlsx)
	    {
		mimestr = "Office Document/Microsoft Excel 2007+";
	    }
	    else if(officecheck.toHex().contains(QString("7070742f").toStdString().c_str())) // "ppt/" (.pptx)
	    {
		mimestr = "Office Document/Microsoft PowerPoint 2007+";
	    }
	}
    }
	    //else
		//qDebug() << "something went wrong...";

	    // might not need the full contents, just need more hex and see if it has the:
	    // word/ folder (0x776f72642f) for .docx
	    // xl/ folder (0x786c2f) for .xlsx
	    // ppt/ folder (0x7070742f) for .pptx

    //else if(filename.startsWith("$INDEX_ROOT:") || filename.startsWith("$DATA:") || filename.startWith("$INDEX_ALLOCATION:"))

    return mimestr;
    //return QString(mimecategory + "/" + mimesignature);
    // WILL NEED TO REIMPLEMENT ALL THE BELOW BASED ON THE NEW LIBMAGIC STUFF
    //if(mimesignature.contains("text"))
//	mimecategory = "Text";
//    else
//	mimecategory = "Unknown";

//    if(mimestr.isEmpty())
//        return QString(mimecategory + "/" + mimesignature);
//    else
//        return mimestr;

    /*
    QString mimestr = "";
    // maybe a problem with file size requested being larger than the actual file size..
    QByteArray sigbuf = curimg->ReadContent(fileoffset, 1024);
    qDebug() << filename << QString::fromStdString(sigbuf.left(6).toStdString());
    QMimeDatabase mimedb;
    const QMimeType mimetype = mimedb.mimeTypeForData(sigbuf);
    QString geniconstr = mimetype.genericIconName();
    QString mimesignature = mimetype.comment();
    //qDebug() << "filename:" << filename << "mimesignature:" << mimesignature;
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
    //if(orphanlist->at(j).value("itemtype").toUInt() == 2)
    //    mimestr = "Directory/Directory";
		    //else if(sigbuf.left(4) == "FILE")
			//mimestr = "Windows System/MFT File Entry";
    if(mimestr.isEmpty())
        return QString(mimecategory + "/" + mimesignature);
    else
        return mimestr;
    */
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
    QString datetimestring = QString("%1-%2-%3 %4:%5:%6").arg(year, 4, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(day, 2, 10, QChar('0')).arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));

    return QDateTime::fromString(datetimestring, "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch();
}

qint64 ConvertExfatTimeToUnixTime(uint8_t t1, uint8_t t2, uint8_t d1, uint8_t d2, uint8_t utc)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    int offsetsecs = 0;
    QString tmpdate = QString("%1%2").arg(d1, 8, 2, QChar('0')).arg(d2, 8, 2, QChar('0'));
    QString tmptime = QString("%1%2").arg(t1, 8, 2, QChar('0')).arg(t2, 8, 2, QChar('0'));
    QString utcstr = QString("%1").arg(utc, 8, 2, QChar('0'));
    year = tmpdate.left(7).toInt(nullptr, 2) + 1980;
    month = tmpdate.mid(7, 4).toInt(nullptr, 2);
    day = tmpdate.right(5).toInt(nullptr, 2);
    hour = tmptime.left(5).toInt(nullptr, 2);
    min = tmptime.mid(5, 6).toInt(nullptr, 2);
    sec = tmptime.right(5).toInt(nullptr, 2) * 2;
    //qDebug() << "utcstr:" << utcstr << utcstr.toInt(nullptr, 2) << "utcstr right:" << utcstr.right(1) << utcstr.right(1).toInt(nullptr, 2) << "utcstr left:" << utcstr.left(7) << utcstr.left(7).toInt(nullptr, 2);
    if(utcstr.right(1).toInt(nullptr, 2) == 1) // apply utc offset
    {
        offsetsecs = utcstr.left(7).toInt(nullptr, 2) * 15 * 60; // signed decimal value * 15 to get the number of 15 minute increments * 60 to put it in seconds.
        //qDebug() << "offset seconds:" << offsetsecs;
    }
    QString datetimestring = QString("%1-%2-%3 %4:%5:%6").arg(year, 4, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(day, 2, 10, QChar('0')).arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    QDateTime tmpdatetime = QDateTime::fromString(datetimestring, "yyyy-MM-dd hh:mm:ss");
    tmpdatetime.setOffsetFromUtc(-offsetsecs);

    return tmpdatetime.toSecsSinceEpoch();
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

uint32_t ConvertNtfsTimeToUnixTime(uint64_t ntdate)
{
// (369*365 + 89) * 24 * 3600 * 10000000
#define	NSEC_BTWN_1601_1970	(uint64_t)(116444736000000000ULL)

    ntdate -= (uint64_t) NSEC_BTWN_1601_1970;
    ntdate /= (uint64_t) 10000000;

    return (uint32_t) ntdate;
}

uint32_t ConvertHfsTimeToUnixTime(uint32_t hfsdate)
{
    if (hfsdate < NSEC_BTWN_1904_1970)
        return 0;
    return (uint32_t) (hfsdate - NSEC_BTWN_1904_1970);
}

QString ConvertBlocksToExtents(QList<uint32_t> blocklist, uint blocksize)
{
    QString extentstring = "";
    int blkcnt = 1;
    uint32_t startvalue = blocklist.at(0);
    //qDebug() << "blocklist:" << blocklist;
    for(int i=1; i < blocklist.count(); i++)
    {
        //qDebug() << "curvalue:" << blocklist.at(i);
        uint32_t oldvalue = blocklist.at(i-1);
        uint32_t newvalue = blocklist.at(i);
        if(newvalue - oldvalue == 1)
            blkcnt++;
        else
        {
            extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
        if(i == blocklist.count() - 1)
        {
            extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
    }
    if(blocklist.count() == 1)
    {
        extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
    }

    return extentstring;
}

QString HashFiles(QString itemstr)
{
    QString itemid = itemstr.split(",", Qt::SkipEmptyParts).at(0);
    QString itemname = itemstr.split(",", Qt::SkipEmptyParts).at(1);
    QString hashstr = "";
    for(int i=0; i < existingforimglist.count(); i++)
    {
	ForImg* curimg = existingforimglist.at(i);
	if(curimg->MountPath().endsWith(itemid.split("-").at(0)))
	{
	    //qDebug() << "match evid:" << existingforimglist.at(i)->MountPath();
	    QString layout = "";
	    quint64 logicalsize = 0;
	    QFile fpropfile(curimg->MountPath() + "/" + itemid.split("-").at(1) + "/" + itemid.split("-").at(2) + ".prop");
	    if(!fpropfile.isOpen())
		fpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
	    if(fpropfile.isOpen())
	    {
		QString line = "";
		while(!fpropfile.atEnd())
		{
		    line = fpropfile.readLine();
		    if(line.startsWith("Layout"))
		    {
			layout = line.split("|").at(1);
			//break;
		    }
		    else if(line.startsWith("Logical Size|"))
		    {
			logicalsize = line.split("|").at(1).toULongLong();
		    }
		}
		fpropfile.close();
	    }
	    QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
	    blake3_hasher hasher;
	    blake3_hasher_init(&hasher);
	    quint64 curlogsize = 0;
	    for(int j=1; j <= layoutlist.count(); j++)
	    {
		QByteArray tmparray;
		tmparray.clear();
		quint64 curoffset = layoutlist.at(j-1).split(",", Qt::SkipEmptyParts).at(0).toULongLong();
		quint64 cursize = layoutlist.at(j-1).split(",", Qt::SkipEmptyParts).at(1).toULongLong();
		curlogsize += cursize;
		if(curlogsize <= logicalsize)
		    tmparray.append(curimg->ReadContent(curoffset, cursize));
		else
		    tmparray.append(curimg->ReadContent(curoffset, (cursize - (curlogsize - logicalsize))));
		    //tmparray.append(curimg->ReadContent(curoffset, (logicalsize - ((j-1) * cursize))));
		blake3_hasher_update(&hasher, tmparray.data(), tmparray.count());
	    }
	    uint8_t output[BLAKE3_OUT_LEN];
	    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);
	    for(size_t j=0; j < BLAKE3_OUT_LEN; j++)
		hashstr.append(QString("%1").arg(output[j], 2, 16, QChar('0')));
	    // NOT SURE HOW TO GET FILENAME OR BEST WAY TO DO THAT, WHETHER IT SHOULD BE THE ID OR THE PATH+FILENAME OR JUST FILENAME
	    // ID WILL ENABLE IT TO BE TIED BACK TO A PIECE OF EVIDENCE AND AVOID CONFUSION AS TO WHICH FILE IN WHICH EVIDENCE IMAGE IS IT SUCH AS IMAGE_100.JPG
	    // FOR NOW I WILL PUT itemid IN PLACE OF THE PATH+FILENAME
	    hashstr += "," + itemname;
	}
    }

    return hashstr;
}
