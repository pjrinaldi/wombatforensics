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
        //QString parstr = "";
        // SHOULD JUST HAVE TO IF/ELSE IF CONTAINS "-" 2 FOR COUNT FOR MANUAL COUNT OTHERWISE EXPANSION SEEMS LIKE IT WILL WORK FOR PROCESSED CARVED
        // NEED TO LINK THE FZ BACK TO THE CORRECT PARENT, SO I'LL NEED MORE THAN JUST THE COUNT OF 2, MIGHT NEED A SEPARATE BIT FOR CARVED ALLTOGETHER.
        if(objectid.split("-").count() == 2)
        {
            idstring = objectid.split("-").at(0);
            astring = objectid.split("-").at(1);
            //parstr = objectid;
            //idstring = estring;
        }
        else if(objectid.split("-").count() == 3)
        {
            if(objectid.contains("-c"))
                idstring = objectid.split("-c").at(0);
            else
                idstring = objectid.split("-f").at(0);
            astring = objectid.split("-").at(2);
            //parstr = objectid;
            //estring = objectid.split("-").at(0);
        }
        /*
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
        */
        QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        QString filename = indxlist.first().sibling(indxlist.first().row(), 0).data().toString();
        QString filepath = indxlist.first().sibling(indxlist.first().row(), 1).data().toString();
        int err = 0;
	QByteArray filebytes;
	filebytes.clear();
        filebytes = ReturnFileContent(objectid);
        //get parent layout property.
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*-" + objectid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
        //QString evidencename = evidfiles.at(0).split("-e").first();
        //QString tmpstr = "";
        /*
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
        */
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
            //QString statstr = wombatvariable.tmpmntpath + "archives/" + idstring + "-fz" + QString::number(i) + "-a" + astring.mid(1) + ".stat";
            QString propstr = wombatvariable.tmpmntpath + "archives/" + objectid + "-z" + QString::number(i) + ".prop";
            //QString propstr = wombatvariable.tmpmntpath + "archives/" + idstring + "-fz" + QString::number(i) + "-a" + astring.mid(1) + ".prop";
	    //if(!QFile::exists(statstr) || !QFile::exists(propstr))
            if(!QFile::exists(propstr))
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
                    // MIGHT NEED TO MOVE THIS SOONER IN THE ZIP SO I DON'T WASTE TIME FAILING W/O A PASSWORD
		    curfile = zip_fopen_index_encrypted(curzip, i, 0, "password"); // IF ENCRYPTED (PROMPT USER FOR PASSWORD)...
		}
		if(curfile != NULL)
		{
		    zip_fread(curfile, magicbuffer, sigsize);
		    zip_fclose(curfile);
		}
		mutex.unlock();
		tmparray = QByteArray::fromRawData(magicbuffer, sigsize);
		QString mimestr = GenerateCategorySignature(tmparray, QString::fromStdString(std::string(zipstat.name)));
		//QString mimestr = GenerateCategorySignature(mimetype, QString::fromStdString(std::string(zipstat.name)));
                /*
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
                if(QString::fromStdString(std::string(zipstat.name)).endsWith("/"))
                    mimestr = "Directory/Directory";
                */
                //qDebug() << "it is a directory, update mime values";
		//qDebug() << "mimestr:" << mimestr;
		delete[] magicbuffer;
		//qDebug() << outstr;
                /*
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
                */
                QHash<QString, QVariant> nodedata;
                nodedata.clear();
                nodedata.insert("name", QByteArray(QString::fromStdString(std::string(zipstat.name)).toUtf8()).toBase64());
                nodedata.insert("path", QByteArray(QString(filepath + filename + "/").toUtf8()).toBase64());
                nodedata.insert("size", (quint64)zipstat.size);
                nodedata.insert("create", "0");
                nodedata.insert("access", "0");
                nodedata.insert("modify", QString::number(zipstat.mtime));
                nodedata.insert("status", "0");
                nodedata.insert("hash", "0");
                nodedata.insert("cat", mimestr.split("/").at(0));
                nodedata.insert("sig", mimestr.split("/").at(1));
                nodedata.insert("tag", "");
                nodedata.insert("id", QString(objectid + "-z" + QString::number(i)));
                mutex.lock();
                treenodemodel->AddNode(nodedata, objectid, 1, 0);
                mutex.unlock();
                filesfound++;
                listeditems.append(QString(objectid + "-z" + QString::number(i)));
                /*
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
                */
		if(!QFile::exists(propstr))
		{
		    QFile propfile(propstr);
		    if(!propfile.isOpen())
			propfile.open(QIODevice::WriteOnly | QIODevice::Text);
		    if(propfile.isOpen())
		    {
			QTextStream proplist(&propfile);
			proplist << "Compressed Size|" << zipstat.comp_size << " bytes|Compressed size of the file in bytes." << Qt::endl;
			proplist << "Compression Method|";
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
			proplist << "|Compression Method used to add the files to the archive." << Qt::endl;
			proplist << "Encryption Method|";
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
			proplist << "|Encryption Method used to protect the contents of the files added to the archive." << Qt::endl;
                        proplist << "Layout|" << layout << "|File Content Layout based on byte offset and size." << Qt::endl;
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
        QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        QString objectname = indxlist.first().sibling(indxlist.first().row(), treenodemodel->GetColumnIndex("name")).data().toString();
	int curhashcount = indxlist.first().sibling(indxlist.first().row(), treenodemodel->GetColumnIndex("hash")).data().toString().count();
        // CAN REMOVE HASH IF
	if((hashsum == 1 && curhashcount != 32) || (hashsum == 2 && curhashcount != 40) || (hashsum == 4 && curhashcount != 64) || (hashsum == 11 && curhashcount != 64))
	{
	    TreeNode* curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
	    qint64 filesize = curitem->Data("size").toLongLong();
	    //if(objectid.contains("z"))
            //  qDebug() << objectid << filesize;
	    QString hashstr = "";
	    if(filesize > 0 && !objectname.endsWith("$Bad") && !isclosing) // speed up hashing if we ignore the sparse file $Bad which doesn't contain relevant information anyway
	    {
		QByteArray filebytes;
		filebytes.clear();
		filebytes = ReturnFileContent(objectid);
                if(hashsum == 11)
                {
                    /*
                    */
                    blake3_hasher filehash;
                    blake3_hasher_init(&filehash);
                    blake3_hasher_update(&filehash, filebytes.data(), filebytes.count());
                    uint8_t blake3hash[BLAKE3_OUT_LEN];
                    blake3_hasher_finalize(&filehash, blake3hash, BLAKE3_OUT_LEN);
                    std::stringstream b3str;
                    for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
                        b3str << std::hex << (int)blake3hash[i];
                    std::string b3hash = "";
                    hashstr = QString::fromStdString(b3str.str());
                    qDebug() << "blake3 hash:" << hashstr;
                }
                else
                {
		    QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum);
		    hashstr = QString(tmphash.hash(filebytes, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper();
                }
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
                else if(hashsum == 11)
                    hashstr = QString("af1349b9f5f9a1a6a0404dea36dcc9499bcb25c9adc112b7cc9a93cae41f3262").toUpper(); // BLAKE3 zero file
	    }
	    if(!isclosing)
	    {
		hashlist.insert(objectid, hashstr);
		mutex.lock();
		treenodemodel->UpdateNode(objectid, "hash", hashstr);
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
            else if(hashsum == 11) // BLAKE3
                hashtype = 4;
	    if(!isclosing)
		isignals->DigUpd(hashtype, dighashcount);
	}
	else
	    qInfo() << "File Hash:" << objectid << "already exists. Skipping...";
    }
}

void GenerateVidThumbnails(QString thumbid)
{
    QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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
	qint64 filesize = curitem->Data("size").toLongLong();
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
	    QString fullpath = curitem->Data("path").toString() + curitem->Data("name").toString();
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
    //QString category = "";
    QString signature = "";
    if(!isclosing)
    {
        indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
        //category = curitem->Data("cat").toString();
        signature = curitem->Data("sig").toString();
    }
    bool isarchive = false;
    //isarchive = category.contains("Archive");
    isarchive = signature.contains("Zip");
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
        indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        curitem = static_cast<TreeNode*>(indxlist.first().internalPointer());
        category = curitem->Data("cat").toString();
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
    QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
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
	qint64 filesize = curitem->Data("size").toLongLong();
	Magick::Geometry thumbgeometry(thumbsize, thumbsize);
	if(filesize > 0 && !isclosing)
	{
	    // IMPLEMENT QBYTEARRAY RETURN FUNCTION HERE
	    QByteArray filebytes;
	    filebytes.clear();
	    filebytes = ReturnFileContent(thumbid);
	    QByteArray ba;
	    ba.clear();
	    QString fullpath = curitem->Data("path").toString() + curitem->Data("name").toString();
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
    ForImg* curimg = NULL;
    for(int i=0; i < existingforimglist.count(); i++)
    {
        if(existingforimglist.at(i)->MountPath().endsWith(objectid.split("-").at(0)))
        {
            curimg = existingforimglist.at(i);
            break;
        }
    }
    QString layout = "";
    quint64 logicalsize = 0;
    if(curimg->ImgType() == 15)
        layout = "0," + QString::number(curimg->Size()) + ";";
    else
    {
        QFile fpropfile(curimg->MountPath() + "/" + objectid.split("-").at(1) + "/" + objectid.split("-").at(2) + ".prop");
        if(!fpropfile.isOpen())
            fpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
        while(!fpropfile.atEnd())
        {
            QString line = fpropfile.readLine();
            if(line.startsWith("Layout|"))
                layout = line.split("|").at(1);
            else if(line.startsWith("Logical Size|"))
                logicalsize = line.split("|").at(1).toULongLong();
        }
        fpropfile.close();
    }
    QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
    QByteArray filecontent;
    filecontent.clear();
    quint64 curlogsize = 0;
    for(int i=1; i <= layoutlist.count(); i++)
    {
        quint64 curoffset = layoutlist.at(i-1).split(",", Qt::SkipEmptyParts).at(0).toULongLong();
        quint64 cursize = layoutlist.at(i-1).split(",", Qt::SkipEmptyParts).at(1).toULongLong();
        curlogsize += cursize;
        if(curlogsize <= logicalsize)
            filecontent.append(curimg->ReadContent(curoffset, cursize));
        else
            filecontent.append(curimg->ReadContent(curoffset, (logicalsize - ((i-1) * cursize))));
    }

    return filecontent;
    /*
    // STILL NEED TO HANDLE ZIP AND CARVED
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, treenodemodel->GetColumnIndex("id"), QModelIndex()), Qt::DisplayRole, QVariant(objectid), 1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
    TreeNode* curnode = static_cast<TreeNode*>(indexlist.first().internalPointer());
    qulonglong filesize = curnode->Data("size").toULongLong();
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
    if(objectid.contains("-z"))
    {
        int err = 0;
        RewriteSelectedIdContent(indexlist.at(0).parent()); // writes parent content to use to load zip content
        QString fnamestr = wombatvariable.tmpfilepath + objectid.split("-z").at(0) + "-fhex";
        zip* curzip = zip_open(fnamestr.toStdString().c_str(), ZIP_RDONLY, &err);
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        int zipid = objectid.split("-z").at(1).toInt();
        zip_stat_index(curzip, zipid, 0, &zipstat);
        char* zipbuf = new char[zipstat.size];
        zip_file_t* curfile = NULL;
        if(zipstat.encryption_method == ZIP_EM_NONE)
            curfile = zip_fopen_index(curzip, zipid, 0);
        if(curfile != NULL)
        {
            zip_fread(curfile, zipbuf, zipstat.size);
            zip_fclose(curfile);
        }
        filecontent.append(zipbuf, zipstat.size);
        delete[] zipbuf;

        return filecontent;
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
    else if(objectid.split("-").count() == 3)
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
        treenodemodel->UpdateNode(i.key(), "hash", i.value());
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
    QHash<QString, QVariant> nodedata;
    nodedata.clear();
    nodedata.insert("name", slist.at(0)); // name
    nodedata.insert("path", slist.at(3)); // path
    nodedata.insert("size", slist.at(8)); // size
    nodedata.insert("create", slist.at(6)); // crtime
    nodedata.insert("access", slist.at(4)); // atime
    nodedata.insert("modify", slist.at(7)); // mtime
    nodedata.insert("status", slist.at(5)); // ctime
    nodedata.insert("hash", slist.at(13)); // hash
    nodedata.insert("cat", QString(slist.at(10)).split("/").first()); // category
    nodedata.insert("sig", QString(slist.at(10)).split("/").last()); // signature
    nodedata.insert("tag", slist.at(15)); // tag
    nodedata.insert("id", slist.at(12)); // id
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

