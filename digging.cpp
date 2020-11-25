#include "digging.h"

// IMAGEMAGICK HEADER
#include <Magick++.h>
// LIBFFMPEG THUMBNAILER HEADERS
#include <filmstripfilter.h>
#include <videothumbnailer.h>
// LIBPOPPLER-QT5 HEADER
#include <poppler-qt5.h>

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void GenerateArchiveExpansion(QString objectid)
{
    // NEED TO FIGURE OUT HOW I WANT TO RECORD ZIP FILES.... UNDER THE NEW ID PARADIGM

    // NEED TO DISTINGUISH BETWEEN ZIP AND OTHER ARCHIVE FORMATS AND PROCESS ACCORDINGLY...
    if(!isclosing)
    {
        //QString estring = "";
        //QString vstring = "";
        //QString pstring = "";
        QString astring = "";
        QString idstring = "";
        QString parstr = "";
        // SHOULD JUST HAVE TO IF/ELSE IF CONTAINS "-" 2 FOR COUNT FOR MANUAL COUNT OTHERWISE EXPANSION SEEMS LIKE IT WILL WORK FOR PROCESSED CARVED
        // NEED TO LINK THE FZ BACK TO THE CORRECT PARENT, SO I'LL NEED MORE THAN JUST THE COUNT OF 2, MIGHT NEED A SEPARATE BIT FOR CARVED ALLTOGETHER.
        if(objectid.split("-").count() == 2)
        {
            idstring = objectid.split("-").at(0);
            astring = objectid.split("-").at(1);
            parstr = objectid;
            //idstring = estring;
        }
        else if(objectid.split("-").count() == 4)
        {
            idstring = objectid.split("-c").at(0);
            astring = objectid.split("-").at(3);
            parstr = objectid;
            //estring = objectid.split("-").at(0);
        }
        else
        {
            idstring = objectid.split("-f").at(0);
            //estring = objectid.split("-").at(0);
            //vstring = objectid.split("-").at(1);
            //pstring = objectid.split("-").at(2);
            astring = objectid.split("-").at(3);
            parstr = objectid.split("-a").first();
            //idstring = estring + "-" + vstring + "-" + pstring;
        }
        QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        QString filename = indxlist.first().sibling(indxlist.first().row(), 0).data().toString();
        QString filepath = indxlist.first().sibling(indxlist.first().row(), 1).data().toString();
        int err = 0;
	QByteArray filebytes;
	filebytes.clear();
        filebytes = ReturnFileContent(objectid);
        QString fnamestr = wombatvariable.tmpfilepath + objectid + "-fhex";
        QFile tmpfile(fnamestr);
        if(!tmpfile.isOpen())
            tmpfile.open(QIODevice::WriteOnly);
        if(tmpfile.isOpen())
        {
            tmpfile.write(filebytes);
            tmpfile.close();
        }
        zip* curzip = zip_open(fnamestr.toStdString().c_str(), ZIP_RDONLY, &err);
        qint64 zipentrycount = zip_get_num_entries(curzip, 0);
        for(int i=0; i < zipentrycount; i++)
        {
            //QString newzipid = estring + "-" + vstring + "-" + pstring + "-" + "-fz" + QString::number(i) + "-a" + astring.mid(1);
            // CURRENT METHOD DOESN'T ACCOUNT FOR DIRECTORIES IN ARCHIVES, JUST LEAVES THEM AS THE PATH...
            // ALSO DOESN'T ACCOUNT FOR ENCRYPTED ZIP..
            QString statstr = wombatvariable.tmpmntpath + "archives/" + idstring + "-fz" + QString::number(i) + "-a" + astring.mid(1) + ".stat";
            QString propstr = wombatvariable.tmpmntpath + "archives/" + idstring + "-fz" + QString::number(i) + "-a" + astring.mid(1) + ".prop";
	    if(!QFile::exists(statstr) || !QFile::exists(propstr))
	    {
		struct zip_stat zipstat;
		zip_stat_init(&zipstat);
		zip_stat_index(curzip, i, 0, &zipstat);
		char* magicbuffer = new char[0];
		qint64 sigsize = 1024;
		if(zipstat.size < 1024)
		    sigsize = zipstat.size;
		magicbuffer = new char[sigsize];
		QByteArray tmparray("intro");
		tmparray.clear();
		mutex.lock();
		zip_file_t* curfile = NULL;
		if(zipstat.encryption_method == ZIP_EM_NONE)
		    curfile = zip_fopen_index(curzip, i, 0); // IF NOT ENCRYPTED
		else
		{
		    // PROMPT USER FOR PASSWORD HERE....
		    curfile = zip_fopen_index_encrypted(curzip, i, 0, "password"); // IF ENCRYPTED (PROMPT USER FOR PASSWORD)...
		}
		if(curfile != NULL)
		{
		    zip_fread(curfile, magicbuffer, sigsize);
		    zip_fclose(curfile);
		}
		mutex.unlock();
		tmparray = QByteArray::fromRawData(magicbuffer, sigsize);
		QMimeDatabase mimedb;
		const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
		QString mimestr = GenerateCategorySignature(mimetype);
		if(mimestr.contains("Unknown")) // generate further analysis
		{
		    if(tmparray.at(0) == '\x4c' && tmparray.at(1) == '\x00' && tmparray.at(2) == '\x00' && tmparray.at(3) == '\x00' && tmparray.at(4) == '\x01' && tmparray.at(5) == '\x14' && tmparray.at(6) == '\x02' && tmparray.at(7) == '\x00') // LNK File
			mimestr = "Windows System/Shortcut";
		    else if(strcmp(filename.toStdString().c_str(), "INFO2") == 0 && (tmparray.at(0) == 0x04 || tmparray.at(0) == 0x05))
			mimestr = "Windows System/Recycler";
		    else if(filename.startsWith("$I") && (tmparray.at(0) == 0x01 || tmparray.at(0) == 0x02))
			mimestr = "Windows System/Recycle.Bin";
		    else if(filename.endsWith(".pf") && tmparray.at(4) == 0x53 && tmparray.at(5) == 0x43 && tmparray.at(6) == 0x43 && tmparray.at(7) == 0x41)
			mimestr = "Windows System/Prefetch";
		    else if(filename.endsWith(".pf") && tmparray.at(0) == 0x4d && tmparray.at(1) == 0x41 && tmparray.at(2) == 0x4d)
			mimestr = "Windows System/Prefetch";
		    //else if(astring.contains("fa"
		}
		//qDebug() << "mimestr:" << mimestr;
		delete[] magicbuffer;
		//qDebug() << outstr;
		QByteArray ba;
		ba.clear();
		ba.append(QString::fromStdString(std::string(zipstat.name)).toUtf8());
		QByteArray ba2;
		ba2.clear();
		ba2.append(QString(filepath + filename + "/").toUtf8());
		QString outstr = ba.toBase64() + ",5," + astring.mid(1) + "," + ba2.toBase64() + ",0,0,0," + QString::number(zipstat.mtime) + "," + QString::number(zipstat.size) + "," + QString::number(i) + "," + mimestr + ",0," + QString(idstring + "-fz" + QString::number(i) + "-a" + astring.mid(1)) + ",0,0,0";
		QStringList treeout;
		treeout.clear();
		treeout << ba.toBase64() << ba2.toBase64() << QString::number(zipstat.size) << "0" << "0" << QString::number(zipstat.mtime) << "0" << "0" << mimestr.split("/").first() << mimestr.split("/").last() << "0" << QString(idstring + "-fz" + QString::number(i) + "-a" + astring.mid(1));
		QList<QVariant> nodedata;
		nodedata.clear();
		for(int i=0; i < 12; i++)
		    nodedata << treeout.at(i);
		if(!QFile::exists(statstr))
		{
		    mutex.lock();
		    treenodemodel->AddNode(nodedata, parstr, 1, 0); 
		    mutex.unlock();
		    filesfound++;
		    listeditems.append(treeout.at(11));
		    QFile statfile(statstr);
		    if(!statfile.isOpen())
			statfile.open(QIODevice::WriteOnly | QIODevice::Text);
		    if(statfile.isOpen())
		    {
			statfile.write(outstr.toStdString().c_str());
			statfile.close();
		    }
		}
		if(!QFile::exists(propstr))
		{
		    QFile propfile(propstr);
		    if(!propfile.isOpen())
			propfile.open(QIODevice::WriteOnly | QIODevice::Text);
		    if(propfile.isOpen())
		    {
			QTextStream proplist(&propfile);
			proplist << "Compressed Size||" << zipstat.comp_size << " bytes||Compressed size of the file in bytes." << Qt::endl;
			proplist << "Compression Method||";
			switch(zipstat.comp_method)
			{
			    case ZIP_CM_STORE:
				proplist << "UNCOMPRESSED";
				break;
			    case ZIP_CM_SHRINK:
				proplist << "SHRUNK";
				break;
			    case ZIP_CM_REDUCE_1:
				proplist << "FACTOR 1 REDUCED";
				break;
			    case ZIP_CM_REDUCE_2:
				proplist << "FACTOR 2 REDUCED";
				break;
			    case ZIP_CM_REDUCE_3:
				proplist << "FACTOR 3 REDUCED";
				break;
			    case ZIP_CM_REDUCE_4:
				proplist << "FACTOR 4 REDUCED";
				break;
			    case ZIP_CM_IMPLODE:
				proplist << "IMPLODED";
				break;
			    case ZIP_CM_DEFLATE:
				proplist << "DEFLATE";
				break;
			    case ZIP_CM_DEFLATE64:
				proplist << "DEFLATE64";
				break;
			    case ZIP_CM_PKWARE_IMPLODE:
				proplist << "PKWARE IMPLODE";
				break;
			    case ZIP_CM_BZIP2:
				proplist << "BZIP2";
				break;
			    case ZIP_CM_LZMA:
				proplist << "LZMA";
				break;
			    case ZIP_CM_TERSE:
				proplist << "IBM TERSE";
				break;
			    case ZIP_CM_LZ77:
				proplist << "IBM LZ77";
				break;
			    case ZIP_CM_LZMA2:
				proplist << "LZMA2";
				break;
			    case ZIP_CM_XZ:
				proplist << "XZ";
				break;
			    case ZIP_CM_JPEG:
				proplist << "COMPRESSED JPEG";
				break;
			    case ZIP_CM_WAVPACK:
				proplist << "COMPRESSED WAVPACK";
				break;
			    case ZIP_CM_PPMD:
				proplist << "PPMD I";
				break;
			    default:
				proplist << "DEFLATE";
				break;
			}
			proplist << "||Compression Method used to add the files to the archive." << Qt::endl;
			proplist << "Encryption Method||";
			switch(zipstat.encryption_method)
			{
			    case ZIP_EM_NONE:
				proplist << "NOT ENCRYPTED";
				break;
			    case ZIP_EM_TRAD_PKWARE:
				proplist << "PKWARE ENCRYPTED";
				break;
			    /*
			    case ZIP_EM_DES:
				htmlstr += "DES ENCRYPTED";
				break;
			    case ZIP_EM_RC2_OLD:
				htmlstr += "RC2 < v5.2 ENCRYPTED";
				break;
			    case ZIP_EM_3DES_168:
				htmlstr += "3DES 168 ENCRYPTED";
				break;
			    case ZIP_EM_3DES_112:
				htmlstr += "3DES 112 ENCRYPTED";
				break;
			    case ZIP_EM_PKZIP_AES_128:
				htmlstr += "PKZIP AES 128 ENCRYPTED";
				break;
			    case ZIP_EM_PKZIP_AES_192:
				htmlstr += "PKZIP AES 192 ENCRYPTED";
				break;
			    case ZIP_EM_PKZIP_AES_256:
				htmlstr += "PKZIP AES 256 ENCRYPTED";
				break;
			    case ZIP_EM_RC2:
				htmlstr += "RC2 > V5.2 ENCRYPTED";
				break;
			    case ZIP_EM_RC4:
				htmlstr += "RC4 ENCRYPTED";
				break;
			    */
			    case ZIP_EM_AES_128:
				proplist << "AES 128 ENCRYPTED";
				break;
			    case ZIP_EM_AES_192:
				proplist << "AES 192 ENCRYPTED";
				break;
			    case ZIP_EM_AES_256:
				proplist << "AES 256 ENCRYPTED";
				break;
			    case ZIP_EM_UNKNOWN:
				proplist << "UNKNOWN ALGORITHM";
				break;
			    default:
				proplist << "NOT ENCRYPTED";
				break;
			}
			proplist << "||Encryption Method used to protect the contents of the files added to the archive." << Qt::endl;
			proplist.flush();
			propfile.close();
		    }
		}
            }
            // FILE STAT CONTENTS - filename, filetype, par addr, dir path, atime, ctime, crtime, mtime, size, addr, mime cat/sig, 0, ID, hash, deleted, bookmark
            // FILE PROP CONTENTS - block address of parent, maybe zip properties i display in html double click for zip parent...
            // TREENODE CONTENTS - name, path, size, crtime, atime, mtime, ctime, md5, category, signature, tag, id
        }
        zip_close(curzip);
        digarchivecount++;
        isignals->DigUpd(5, digarchivecount);
    }
}

void GenerateHash(QString objectid)
{
    // update Hash header: 32 = md5, 40 = sha1, 64 = sha256
    //if((objectid.split("-").count() == 5 || objectid.contains("-c")) && !isclosing)
    if((objectid.split("-").count() == 3 || objectid.contains("-c")) && !isclosing)
    {
        QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        QString objectname = indxlist.first().sibling(indxlist.first().row(), 0).data().toString();
	int curhashcount = indxlist.first().sibling(indxlist.first().row(), 7).data().toString().count();
	if((hashsum == 1 && curhashcount != 32) || (hashsum == 2 && curhashcount != 40) || (hashsum == 3 && curhashcount != 64))
	{
	    TreeNode* curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
	    qint64 filesize = curitem->Data(2).toLongLong();
	    //if(objectid.contains("z"))
            //  qDebug() << objectid << filesize;
	    QString hashstr = "";
	    if(filesize > 0 && !objectname.endsWith("$Bad") && !isclosing) // speed up hashing if we ignore the sparse file $Bad which doesn't contain relevant information anyway
	    {
		QByteArray filebytes;
		filebytes.clear();
		filebytes = ReturnFileContent(objectid);
		QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum);
		hashstr = QString(tmphash.hash(filebytes, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper();
		filebytes.clear();
	    }
	    else
	    {
		if(hashsum == 1)
		    hashstr = QString("d41d8cd98f00b204e9800998ecf8427e").toUpper(); // MD5 zero file
		else if(hashsum == 2)
		    hashstr = QString("da39a3ee5e6b4b0d3255bfef95601890afd80709").toUpper(); // SHA1 zero file
		else if(hashsum == 4)
		    hashstr = QString("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855").toUpper(); // SHA256 zero file
	    }
	    if(!isclosing)
	    {
		hashlist.insert(objectid, hashstr);
		mutex.lock();
		treenodemodel->UpdateNode(objectid, 7, hashstr);
		mutex.unlock();
		dighashcount++;
	    }
	    int hashtype = 1;
	    if(hashsum == 1) // MD5
		hashtype = 1;
	    else if(hashsum == 2) // SHA1
		hashtype = 2;
	    else if(hashsum == 4) // SHA256
		hashtype = 3;
	    if(!isclosing)
		isignals->DigUpd(hashtype, dighashcount);
	}
	else
	    qInfo() << "File Hash:" << objectid << "already exists. Skipping...";
    }
}

void GenerateVidThumbnails(QString thumbid)
{
    QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    QString thumbtestpath = genthmbpath + "thumbs/" + thumbid + ".png";
    QImage* testimage = new QImage();
    bool imgbool = testimage->load(thumbtestpath);
    if(imgbool && (testimage->height() <= thumbsize+10 && testimage->height() >= thumbsize-10))
    {
	qInfo() << "Video Thumbnail:" << thumbtestpath << "already exists. Skipping...";
    }
    else
    {
	TreeNode* curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
	qint64 filesize = curitem->Data(2).toLongLong();
	if(filesize > 0 && !isclosing)
	{
	    // IMPLEMENT QBYTEARRAY RETURN FUNCTION HERE
	    QByteArray filebytes;
	    filebytes.clear();
	    filebytes = ReturnFileContent(thumbid);
	    QDir dir;
	    dir.mkpath(wombatvariable.tmpfilepath);
	    QString tmpstring = wombatvariable.tmpfilepath + thumbid + "-tmp";
	    QFile tmpfile(tmpstring);
	    if(tmpfile.open(QIODevice::WriteOnly))
	    {
		tmpfile.write(filebytes);
		tmpfile.close();
	    }
	    else
		qDebug() << "Item:" << thumbid << "couldn't open file for writing contents.";
	    QByteArray ba;
	    QString fullpath = curitem->Data(1).toString() + curitem->Data(0).toString();
	    ba.clear();
	    ba.append(fullpath.toUtf8());
	    if(!isclosing)
		imageshash.insert(thumbid, QString(ba.toBase64()));
	    QStringList tlist;
	    try
	    {
		if(!isclosing)
		{
		    ffmpegthumbnailer::VideoThumbnailer videothumbnailer(0, true, true, 8, false);
		    videothumbnailer.setThumbnailSize(QString::number(thumbsize).toStdString());
		    std::unique_ptr<ffmpegthumbnailer::FilmStripFilter> filmstripfilter;
		    filmstripfilter.reset(new ffmpegthumbnailer::FilmStripFilter());
		    videothumbnailer.addFilter(filmstripfilter.get());
		    videothumbnailer.setPreferEmbeddedMetadata(false);
		    int vtcnt = 100 / vidcount;
		    tlist.clear();
		    for(int i=0; i <= vtcnt; i++)
		    {
			int seekpercentage = i * vidcount;
			if(seekpercentage == 0)
			    seekpercentage = 5;
			if(seekpercentage == 100)
			    seekpercentage = 95;
			QString tmpoutfile = wombatvariable.tmpfilepath + thumbid + ".t" + QString::number(seekpercentage) + ".png";
			tlist.append(tmpoutfile);
			videothumbnailer.setSeekPercentage(seekpercentage);
			videothumbnailer.generateThumbnail(tmpstring.toStdString(), Png, tmpoutfile.toStdString());
		    }
		    try
		    {
			std::list<Magick::Image> thmbimages;
			std::list<Magick::Image> montage;
			Magick::Image image;
			for(int i=0; i < tlist.count(); i++)
			{
			    image.read(tlist.at(i).toStdString());
			    thmbimages.push_back(image);
			}
			QString thumbout = genthmbpath + "thumbs/" + thumbid + ".png";
			Magick::Montage montageopts;
			Magick::Color color("rgba(0,0,0,0)");
			montageopts.geometry(QString(QString::number(thumbsize) + "x" + QString::number(thumbsize) + "+1+1").toStdString());
			montageopts.tile(QString(QString::number(tlist.count()) + "x1").toStdString());
			montageopts.backgroundColor(color);
			montageopts.fileName(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
			Magick::montageImages(&montage, thmbimages.begin(), thmbimages.end(), montageopts); 
			if(montage.size() == 1)
			{
			    std::string mstring = thumbout.toStdString();
			    Magick::Image& montageimage = montage.front();
			    montageimage.magick("png");
			    montageimage.write(mstring);
			}
			else
			{
			    qDebug() << "Item:" << thumbid << "issue with montage" << montage.size() << ". Missing video thumbnail will be used.";
			    try
			    {
				if(!isclosing)
				{
				    QPixmap pixmap(":/videoerror", "PNG");
				    QByteArray iarray;
				    QBuffer buffer(&iarray);
				    buffer.open(QIODevice::WriteOnly);
				    pixmap.save(&buffer, "PNG");
				    Magick::Blob blob(static_cast<const void*>(iarray.data()), iarray.size());
				    Magick::Image master(blob);
				    master.quiet(false);
				    master.resize(QString(QString::number(thumbsize) + "x" + QString::number(thumbsize)).toStdString());
				    master.magick("PNG");
				    master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
				}
			    }
			    catch(Magick::Exception &error)
			    {
				qDebug() << "Item:" << thumbid << "magick error:" << error.what() << ".";
			    }
			}
		    }
		    catch(Magick::Exception &error)
		    {
			qDebug() << "Item:" << thumbid << "caught exception during montage operation:" << error.what() << ". Missing video thumbnail will be used.";
			try
			{
			    if(!isclosing)
			    {
				QPixmap pixmap(":/videoerror", "PNG");
				QByteArray iarray;
				QBuffer buffer(&iarray);
				buffer.open(QIODevice::WriteOnly);
				pixmap.save(&buffer, "PNG");
				Magick::Blob blob(static_cast<const void*>(iarray.data()), iarray.size());
				Magick::Image master(blob);
				master.quiet(false);
				master.resize(QString(QString::number(thumbsize) + "x" + QString::number(thumbsize)).toStdString());
				master.magick("PNG");
				master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
			    }
			}
			catch(Magick::Exception &error)
			{
			    qDebug() << "Item:" << thumbid << "magick error:" << error.what() << ".";
			}
		    }
		}
	    }
	    catch(std::exception& e)
	    {
		qDebug() << "Item:" << thumbid << "libffmpegthumbnailer error:" << e.what() << ". Missing video thumbnail used instead.";
		try
		{
		    if(!isclosing)
		    {
			QPixmap pixmap(":/videoerror", "PNG");
			QByteArray iarray;
			QBuffer buffer(&iarray);
			buffer.open(QIODevice::WriteOnly);
			pixmap.save(&buffer, "PNG");
			Magick::Blob blob(static_cast<const void*>(iarray.data()), iarray.size());
			Magick::Image master(blob);
			master.quiet(false);
			master.resize(QString(QString::number(thumbsize) + "x" + QString::number(thumbsize)).toStdString());
			master.magick("PNG");
			master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
		    }
		}
		catch(Magick::Exception &error)
		{
		    qDebug() << "Item:" << thumbid << "magick error:" << error.what() << ".";
		}
	    }
	}
	else if(filesize == 0 && !isclosing)// video was 0 length
	{
	    try
	    {
		if(!isclosing)
		{
		    QPixmap pixmap(":/videoerror", "PNG");
		    QByteArray iarray;
		    QBuffer buffer(&iarray);
		    buffer.open(QIODevice::WriteOnly);
		    pixmap.save(&buffer, "PNG");
		    Magick::Blob blob(static_cast<const void*>(iarray.data()), iarray.size());
		    Magick::Image master(blob);
		    master.quiet(false);
		    master.resize(QString(QString::number(thumbsize) + "x" + QString::number(thumbsize)).toStdString());
		    master.magick("PNG");
		    master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
		}
	    }
	    catch(Magick::Exception &error)
	    {
		qDebug() << "Item:" << thumbid << "magick error:" << error.what() << ".";
	    }
	}
    }
    if(!isclosing)
    {
        digvidthumbcount++;
        isignals->DigUpd(4, digvidthumbcount);
    }
}

void GeneratePreDigging(QString thumbid)
{
    TreeNode* curitem = NULL;
    QModelIndexList indxlist;
    QString category = "";
    if(!isclosing)
    {
        indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
        category = curitem->Data(8).toString();
    }
    bool isarchive = false;
    isarchive = category.contains("Archive");
    if(hasarchive && isarchive && !isclosing)
        GenerateArchiveExpansion(thumbid);
}

void GenerateDigging(QString thumbid)
{
    TreeNode* curitem = NULL;
    QModelIndexList indxlist;
    QString category = "";
    if(!isclosing)
    {
        indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
        category = curitem->Data(8).toString();
    }
    bool isvid = false;
    bool isimg = false;
    isvid = category.contains("Video");
    isimg = category.contains("Image");
    if(hashash && !isclosing)
        GenerateHash(thumbid);
    if(hasvid && isvid && !isclosing)
        GenerateVidThumbnails(thumbid);
    if(hasimg && isimg && !isclosing)
        GenerateThumbnails(thumbid);
    //if(hasarchive && !isclosing)
    //    GenerateArchiveExpansion(thumbid);
}

void GenerateThumbnails(QString thumbid)
{
    QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    QString thumbtestpath = genthmbpath + "thumbs/" + thumbid + ".png";
    QImage* testimage = new QImage();
    bool imgbool = testimage->load(thumbtestpath);
    if(imgbool && (testimage->width() == thumbsize || testimage->height() == thumbsize))
    {
	qInfo() << "Thumbnail:" << thumbtestpath << "already exists. Skipping.";
    }
    else
    {
	TreeNode* curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
	qint64 filesize = curitem->Data(2).toLongLong();
	Magick::Geometry thumbgeometry(thumbsize, thumbsize);
	if(filesize > 0 && !isclosing)
	{
	    // IMPLEMENT QBYTEARRAY RETURN FUNCTION HERE
	    QByteArray filebytes;
	    filebytes.clear();
	    filebytes = ReturnFileContent(thumbid);
	    QByteArray ba;
	    ba.clear();
	    QString fullpath = curitem->Data(1).toString() + curitem->Data(0).toString();
	    ba.append(fullpath.toUtf8());
	    imageshash.insert(thumbid, QString(ba.toBase64()));
	    try
	    {
		if(!isclosing)
		{
		    Magick::Blob blob(static_cast<const void*>(filebytes.data()), filebytes.size());
		    Magick::Image master(blob);
		    master.quiet(false);
		    master.resize(thumbgeometry);
		    master.magick("PNG");
		    master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
		}
	    }
	    catch(Magick::Exception &error)
	    {
		qDebug() << "Item:" << thumbid << "magick resize exception:" << error.what() << ". Missing image thumbnail used.";
		try
		{
		    if(!isclosing)
		    {
			QPixmap pixmap(":/missingimage", "PNG");
			QByteArray iarray;
			QBuffer buffer(&iarray);
			buffer.open(QIODevice::WriteOnly);
			pixmap.save(&buffer, "PNG");
			Magick::Blob blob(static_cast<const void*>(iarray.data()), iarray.size());
			Magick::Image master(blob);
			master.quiet(false);
			master.resize(thumbgeometry);
			master.magick("PNG");
			master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
		    }
		}
		catch(Magick::Exception &error)
		{
		    qDebug() << "Item:" << thumbid << "magick error:" << error.what() << ".";
		}
	    }
	}
	else if(filesize == 0 && !isclosing)
	{
	    qDebug() << "Item:" << thumbid << "has zero filesize, so missing image thumbnail is used.";
	    try
	    {
		if(!isclosing)
		{
		    QPixmap pixmap(":/missingimage", "PNG");
		    QByteArray iarray;
		    QBuffer buffer(&iarray);
		    buffer.open(QIODevice::WriteOnly);
		    pixmap.save(&buffer, "PNG");
		    Magick::Blob blob(static_cast<const void*>(iarray.data()), iarray.size());
		    Magick::Image master(blob);
		    master.quiet(false);
		    master.resize(thumbgeometry);
		    master.magick("PNG");
		    master.write(QString(genthmbpath + "thumbs/" + thumbid + ".png").toStdString());
		}
	    }
	    catch(Magick::Exception &error)
	    {
		qDebug() << "Item:" << thumbid << "magick error:" << error.what() << ".";
	    }
	}
    }
    if(!isclosing)
    {
        digimgthumbcount++;
        isignals->DigUpd(0, digimgthumbcount);
    }
}

QByteArray ReturnFileContent(QString objectid)
{
    // STILL NEED TO HANDLE ZIP AND CARVED
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    TreeNode* curnode = static_cast<TreeNode*>(indexlist.first().internalPointer());
    qulonglong filesize = curnode->Data(2).toULongLong();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*-" + objectid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
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
    if(objectid.split("-").count() == 3)
    {
        QFile fpropfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/" + objectid.split("-").at(1) + "/" + objectid.split("-").at(2) + ".prop");
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
    }
    else if(objectid.contains("-c"))
    {
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + objectid + ".prop");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
        {
            layout = cfile.readLine();
            cfile.close();
        }
    }
    QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
    QFile efile(tmpstr.split(",", Qt::SkipEmptyParts).at(1));
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        for(uint i=1; i <= (uint)layoutlist.count(); i++)
        {
            efile.seek(layoutlist.at(i-1).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
            if(i * layoutlist.at(i-1).split(",", Qt::SkipEmptyParts).at(1).toULongLong() <= filesize)
                filecontent.append(efile.read(layoutlist.at(i-1).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
            else
                filecontent.append(efile.read(filesize - ((i-1) * layoutlist.at(i-1).split(",", Qt::SkipEmptyParts).at(1).toULongLong())));
        }
        efile.close();
    }

    return filecontent;

    /*
    // TSK FREE METHOD IMPLEMENTATION
    QString zipid = "";
    if(objectid.contains("z")) // exporting a child of a zip file
    {
        QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        zipid = objectid;
        objectid = indxlist.first().parent().sibling(indxlist.first().parent().row(), 11).data().toString();
    }
    QString estring = objectid.split("-", Qt::SkipEmptyParts).at(0);
    QString vstring = objectid.split("-", Qt::SkipEmptyParts).at(1);
    QString pstring = objectid.split("-", Qt::SkipEmptyParts).at(2);
    QString fstring = objectid.split("-", Qt::SkipEmptyParts).at(3);
    QString astring = "";
    if(!objectid.contains("-c"))
        astring = objectid.split("-", Qt::SkipEmptyParts).at(4);
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    TreeNode* curnode = static_cast<TreeNode*>(indexlist.first().internalPointer());
    QString fname = indexlist.first().sibling(indexlist.first().row(), 0).data().toString();
    qint64 filesize = curnode->Data(2).toLongLong();
    QString tmpstr = "";
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QString evidencename = eviddir.entryList(QStringList("*." + estring), QDir::NoSymLinks | QDir::Dirs).first().split(".e").first();
    QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
    evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(evidfile.isOpen())
        tmpstr = evidfile.readLine();
    evidfile.close();
    QString datastring = wombatvariable.imgdatapath;
    if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
        datastring += tmpstr.split(",").at(3).split("/").last() + ".raw";
    else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
        datastring += tmpstr.split(",").at(3).split("/").last() + "/ewf1";
    else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)tmpstr.split(",").at(0).toInt()))
    {
        QString imgext = tmpstr.split(",").at(3).split("/").last().split(".").last();
        if(imgext.contains(".000"))
            datastring += tmpstr.split(",").at(3).split("/").last() + ".raw";
        else
            datastring = tmpstr.split(",").at(3);
    }
    tmpstr = "";
    QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
    if(!partfile.isOpen())
	partfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(partfile.isOpen())
	tmpstr = partfile.readLine();
    partfile.close();
    QStringList partlist = tmpstr.split(",");
    tmpstr = "";
    qint64 fsoffset = partlist.at(4).toLongLong();
    int blocksize = partlist.at(6).toInt();
    int fstype = partlist.at(0).toInt();
    partlist.clear();
    int mftaddress = 0;
    bool isresident = false;
    QString blockstring = "";
    QString residentstring = "";
    QString bytestring = "";
    if(fstring.contains("a")) // ADS ATTRIBUTE
        mftaddress = astring.mid(2).toInt();
    else if(fstring.contains("o")) // ORPHAN
        mftaddress = fstring.mid(2).toInt();
    else if(fstring.contains("d")) // DELETED RECOVERED
        mftaddress = fstring.mid(2).toInt();
    else
        mftaddress = fstring.mid(1).toInt();
    if(!objectid.contains("-c"))
    {
        QFile fileprop(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/" + fstring + "." + astring + ".prop");
        if(!fileprop.isOpen())
            fileprop.open(QIODevice::ReadOnly | QIODevice::Text);
        if(fileprop.isOpen())
        {
            while(!fileprop.atEnd())
            {
                QString tmpstring = fileprop.readLine();
                if(tmpstring.contains("Block Address"))
                    blockstring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Byte Offset"))
                    bytestring = tmpstring.split("||").at(1);
                else if(tmpstring.contains("Data Attribute"))
                    residentstring = tmpstring.split("||").at(1);
            }
        }
        isresident = true;
        if(residentstring.contains("Non"))
            isresident = false;
        fileprop.close();
    }
    QByteArray filebytes;
    filebytes.clear();
    QFile imgfile(datastring);
    
    // ALTERNATIVE IF/ELSE METHOD TO SHORTEN CODE
    bool isres = isresident;
    bool isntfs = false;
    bool isads = false;
    bool isdir = false;
    bool iscarved = false;
    if(objectid.contains("-c"))
        iscarved = true;
    if(fstype == TSK_FS_TYPE_NTFS_DETECT)
	isntfs = true;
    if(fstring.contains("a"))
        isads = true;
    if(curnode->itemtype == 3 || curnode->itemtype == 11) // IF DIRECTORY (ALWAYS RESIDENT)
	isdir = true;
    if(fname.contains("$OrphanFiles"))
        isres = false;

    if(isntfs && isres && !iscarved) // NTFS & RESIDENT
    {
    	unsigned int curoffset = 0;
        uint16_t mftoffset = 0;
        uint32_t atrtype = 0;
        uint8_t namelength = 0;
        uint32_t attrlength = 0;
        uint16_t resoffset = 0;
	uint32_t contentlength = 0;
	QStringList mftblocklist;
        mftblocklist.clear();
	QString mftid = objectid.split("-").first() + "-" + objectid.split("-").at(1) + "-" + objectid.split("-").at(2) + "-f0-a5";
	mftblocklist = mftblockhash.value(mftid).split("^^", Qt::SkipEmptyParts);
         // NEW RESIDENT OFFSET CALCULATION
        qint64 residentoffset = 0;
        if((mftaddress * 1024/blocksize) % 2 == 0) // even number, get the starting block.
        {
            //float mftblock = mftaddress * 1024.0/blocksize;
            //int mftblockint = floor(mftblock);
            */
            /*
            qDebug() << "mftid:" << mftid;
            qDebug() << "mftblocklist count:" << mftblocklist.count();
            qDebug() << "objectid:" << objectid;
            qDebug() << "mftaddress * 1024 / blocksize:" << mftaddress << mftaddress * 1024/blocksize;
            */
            //residentoffset = (mftblocklist.at(mftblockint).toLongLong() * blocksize) + fsoffset + (blocksize * (mftblock - mftblockint));
            /*
            residentoffset = (mftblocklist.at(mftaddress * 1024/blocksize).toLongLong() * blocksize) + fsoffset;
        }
        else // odd number, get starting block and jump the fractional amount to get to the correct entry.
        {
            float mftblock = mftaddress * 1024.0/blocksize;
            int mftblockint = floor(mftblock);
            residentoffset = (mftblocklist.at(mftblockint).toLongLong() * blocksize) + fsoffset + (blocksize * (mftblock - mftblockint));
        }
	QByteArray resbuffer;
	resbuffer.clear();
        if(!imgfile.isOpen())
	    imgfile.open(QIODevice::ReadOnly);
	imgfile.seek(0);
	imgfile.seek(residentoffset);
	resbuffer.append(imgfile.read(1024)); // MFT ENTRY
        if(imgfile.isOpen())
	    imgfile.close();
	if(resbuffer.count() > 0)
	{
            curoffset = 0;
            mftoffset = qFromLittleEndian<uint16_t>(resbuffer.mid(20, 2)); // offset to first attribute
            uint16_t attrcnt = qFromLittleEndian<uint16_t>(resbuffer.mid(40, 2)); // next attribute id
            curoffset += mftoffset;
            for(int i = 0; i < attrcnt; i++)
            {
                atrtype = qFromLittleEndian<uint32_t>(resbuffer.mid(curoffset, 4)); // attribute type
                namelength = qFromLittleEndian<uint8_t>(resbuffer.mid(curoffset + 9, 1)); // length of name
                attrlength = qFromLittleEndian<uint32_t>(resbuffer.mid(curoffset + 4, 4)); // attribute length
		if(isdir && atrtype == 144)
                {
		    break;
                }
		if(!isdir && isads && namelength > 0 && atrtype == 128)
                {
		    break;
                }
		else if(!isdir && !isads && namelength == 0 && atrtype == 128)
                {
	            break;
                }
                if(atrtype == 4294967295)
                {
                    break;
                }
                curoffset += attrlength;
            }
	    resoffset = qFromLittleEndian<uint16_t>(resbuffer.mid(curoffset + 20, 2)); // resident attribute content offset
	    filebytes.append(resbuffer.mid(curoffset + resoffset, contentlength));
        }
    }
    else if(iscarved)
    {
        QString tmpstr;
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + objectid + ".stat");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
            tmpstr = cfile.readLine();
        cfile.close();
        if(!imgfile.isOpen())
            imgfile.open(QIODevice::ReadOnly);
        if(imgfile.isOpen())
        {
            bool isseek = imgfile.seek(tmpstr.split(",").at(16).toLongLong());
            if(isseek)
                filebytes = imgfile.read(tmpstr.split(",").at(8).toLongLong());
            imgfile.close();
        }
    }
    else // NTFS NON-RESIDENT or ALTERNATIVE FILE SYSTEM
    {
        if(!imgfile.isOpen())
	    imgfile.open(QIODevice::ReadOnly);
	for(int i=1; i <= blockstring.split("^^", Qt::SkipEmptyParts).count(); i++)
	{
            if(blockstring.split("^^", Qt::SkipEmptyParts).at(i-1).toLongLong() != 0)
            {
		imgfile.seek(0);
                int blkoffset = fsoffset + blockstring.split("^^", Qt::SkipEmptyParts).at(i-1).toLongLong() * blocksize;
    	        imgfile.seek(blkoffset);
        	if((i * blocksize) <= filesize)
	            filebytes.append(imgfile.read(blocksize));
		else
		    filebytes.append(imgfile.read(filesize - ((i-1)*blocksize)));
            }
	}
        if(imgfile.isOpen())
            imgfile.close();
    }
    if(zipid.contains("z"))
    {
        // WRITE FILEBYTES to TMPFILE for ZIP* to import...
        QString zipstr = wombatvariable.tmpfilepath + objectid + "-fhex";
        QFile tmpfile(zipstr);
        if(!tmpfile.isOpen())
            tmpfile.open(QIODevice::WriteOnly);
        if(tmpfile.isOpen())
        {
            tmpfile.write(filebytes);
            tmpfile.close();
        }
        int err = 0;
        zip* zfile = zip_open(zipstr.toStdString().c_str(), ZIP_RDONLY, &err);
        qDebug() << "zipstr:" << zipstr << "zipid:" << zipid;
        struct zip_stat zstat;
        zip_stat_init(&zstat);
        // zip issue at below line, start to debug this...
        zip_stat_index(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0, &zstat);
        zip_file_t* curfile = NULL;
        if(zstat.encryption_method == ZIP_EM_NONE)
            curfile = zip_fopen_index(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0); // IF NOT ENCRYPTED
        else
        {
            // PROMPT USER FOR PASSWORD HERE....
            curfile = zip_fopen_index_encrypted(zfile, zipid.split("-").at(3).mid(2).toLongLong(), 0, "password"); // IF ENCRYPTED (PROMPT USER FOR PASSWORD)...
        }
        if(curfile != NULL)
        {
            char* zfbuf = new char[zstat.size];
            qint64 zcnt = zip_fread(curfile, zfbuf, zstat.size);
            zip_fclose(curfile);
            filebytes.clear();
            filebytes.append(QByteArray::fromRawData(zfbuf, zcnt));
            delete[] zfbuf;
        }
        zip_close(zfile);
    }
    */
}

void SaveHashList(void)
{
    QFile hfile(wombatvariable.tmpmntpath + "hashlist");
    if(!hfile.isOpen())
        hfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(hfile.isOpen())
    {
        QHashIterator<QString, QString> i(hashlist);
        while(i.hasNext())
        {
            i.next();
            hfile.write(i.key().toStdString().c_str());
            hfile.write("|");
            hfile.write(i.value().toStdString().c_str());
            hfile.write(",");
        }
    }
    hfile.close();
}

void InitializeHashList(void)
{
    hashlist.clear();
    QString tmpstr = "";
    QFile hashfile(wombatvariable.tmpmntpath + "hashlist");
    if(hashfile.exists())
    {
        if(!hashfile.isOpen())
            hashfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(hashfile.isOpen())
            tmpstr = hashfile.readLine();
        hashfile.close();
        QStringList hlist = tmpstr.split(",", Qt::SkipEmptyParts);
        for(int i=0; i < hlist.count(); i++)
            hashlist.insert(hlist.at(i).split("|", Qt::SkipEmptyParts).at(0), hlist.at(i).split("|", Qt::SkipEmptyParts).at(1));
    }
    QHashIterator<QString, QString> i(hashlist);
    while(i.hasNext())
    {
        i.next();
        treenodemodel->UpdateNode(i.key(), 7, i.value());
    }
}

void PopulateArchiveFiles(QString afilestr)
{
    afilestr = wombatvariable.tmpmntpath + "archives/" + afilestr;
    QFile afile(afilestr);
    QString tmpstr;
    if(!afile.isOpen())
        afile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(afile.isOpen())
        tmpstr = afile.readLine();
    afile.close();
    QStringList slist = tmpstr.split(",");
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << slist.at(0); // name
    nodedata << slist.at(3); // path
    nodedata << slist.at(8); // size
    nodedata << slist.at(6); // crtime
    nodedata << slist.at(4); // atime
    nodedata << slist.at(7); // mtime
    nodedata << slist.at(5); // ctime
    nodedata << slist.at(13); // hash
    nodedata << QString(slist.at(10)).split("/").first(); // category
    nodedata << QString(slist.at(10)).split("/").last(); // signature
    nodedata << slist.at(15); // tag
    nodedata << slist.at(12); // id
    QString parentstr = afilestr.split("/").last().split("-fz").first() + "-f" + afilestr.split("/").last().split("-a").last().split(".stat").first();
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 1, 0);
    mutex.unlock();
    listeditems.append(slist.at(12));
}

void SaveImagesHash()
{
    QFile thumbfile;
    thumbfile.setFileName(wombatvariable.tmpmntpath + "thumbs/thumbpathlist");
    thumbfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(thumbfile.isOpen())
    {
        QHashIterator<QString, QString> i(imageshash);
        while(i.hasNext())
        {
            i.next();
            thumbfile.write(i.key().toStdString().c_str());
            thumbfile.write("|");
            thumbfile.write(i.value().toStdString().c_str());
            thumbfile.write(",");
        }
    }
    thumbfile.close();
}

void LoadImagesHash()
{
    QFile thumbfile;
    QString tmpstr = "";
    thumbfile.setFileName(wombatvariable.tmpmntpath + "thumbs/thumbpathlist");
    thumbfile.open(QIODevice::ReadOnly);
    if(thumbfile.isOpen())
        tmpstr = thumbfile.readLine();
    thumbfile.close();
    for(int i = 0; i < tmpstr.split(",", Qt::SkipEmptyParts).count(); i++)
        imageshash.insert(tmpstr.split(",", Qt::SkipEmptyParts).at(i).split("|").at(0), tmpstr.split(",", Qt::SkipEmptyParts).at(i).split("|").at(1));
}

