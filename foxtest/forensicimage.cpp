#include "forensicimage.h"

ForImg::ForImg(std::string imgfile)
{
    // SET IMGPATH - PATH TO THE FORENSIC IMAGE
    imgpath = imgfile;
    int rfound = imgfile.rfind(".");
    // SET IMGTYPE - DETERMINES HOW READ THE RAW CONTENT
    std::string imgext = imgfile.substr(rfound+1);
    if(imgext.compare("dd") == 0 || imgext.compare("DD") == 0) // RAW
        imgtype = 1;
    else if(imgext.compare("e01") == 0 || imgext.compare("E01") == 0) // EWF
        imgtype = 2;
    else if(imgext.compare("aff4") == 0 || imgext.compare("AFF4") == 0) // AFF4
        imgtype = 3;
    else if(imgext.compare("000") == 0 || imgext.compare("001") == 0) // SPLIT RAW
        imgtype = 4;
    else if(imgext.compare("wfi") == 0) // WFI
        imgtype = 5;
    else if(imgext.compare("wli") == 0) // WLI
        imgtype = 6;
    else // ANY OLD FILE
        imgtype = 0;
    // SET IMGSIZE - GET THE SIZE OF THE RAW CONTENT
    if(imgtype == 1) // RAW
    {
        imagebuffer.open(imgfile.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(0, imagebuffer.beg);
        imagebuffer.seekg(0, imagebuffer.end);
        imgsize = imagebuffer.tellg();
        imagebuffer.close();
        std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 2) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
	//std::cout << "Image Path: " << imgpath << std::endl;
	int found = imgpath.rfind(".E");
	if(found == -1)
	    found = imgpath.rfind(".e");
	std::string imgprematch = imgpath.substr(0, found+2);
	//std::cout << "Image Pre Match: " << imgprematch << std::endl;
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	//std::cout << "imagepath: " << imagepath.string() << std::endl;
	//int filecount = 0;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
		//std::cout << "match: " << file.path().string() <<std::endl;
	}
	char* filenames[globfilecnt] = {NULL};
	filenames[0] = (char*)imgpath.c_str();
	int i = 1;
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
	    {
		if(file.path().string().compare(imgpath) != 0)
		{
		    filenames[i] = (char*)file.path().string().c_str();
		    i++;
		}
	    }
	}
        int retopen = 0;
        retopen = libewf_glob(filenames[0], strlen(filenames[0]), LIBEWF_FORMAT_UNKNOWN, &globfiles, &globfilecnt, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        libewf_handle_get_media_size(ewfhandle, &imgsize, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
        std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 3) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.c_str());
        imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
	std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 4) // SPLIT RAW
    {
    }
    else if(imgtype == 5) // WFI
    {
    }
    else if(imgtype == 6) // WLI
    {
    }
    else // everything else
    {
    }
}

/*
    else if(imgtype == 1) // AFF
    {
        //char* iname = new char[imgpath.toStdString().size() + 1];
        //strcpy(iname, imgpath.toStdString().c_str());
        AFFILE* afimage = NULL;
        afimage = af_open(imgpath.toStdString().c_str(), O_RDONLY|O_EXCL, 0);
        imgsize = af_get_imagesize(afimage);
        af_close(afimage);
    }
    else if(imgtype == 3) // SPLIT RAW
    {
	QString efilepath = imgfile.split(imgfile.split("/").last()).first();
	QString fileprefix = "";
	if(imgfile.toLower().endsWith(".000"))
	    fileprefix = imgfile.split("/").last().split(".000").first();
	else if(imgfile.toLower().endsWith(".001"))
	    fileprefix = imgfile.split("/").last().split(".001").first();
	else if(imgfile.toLower().endsWith(".aaa"))
	    fileprefix = imgfile.split("/").last().split(".aaa", Qt::SkipEmptyParts, Qt::CaseInsensitive).first();
	//qDebug() << "fileprefix:" << fileprefix;
	//qDebug() << "efilepath:" << efilepath;
	QDir edir = QDir(efilepath);
	QStringList efiles = edir.entryList(QStringList() << QString(fileprefix + ".???"), QDir::NoSymLinks | QDir::Files);
	//qDebug() << "efiles count:" << efiles.count();
        //qDebug() << "efiles:" << efiles;
        // SMRAW isn't working, so i'm gonna try to implement it myself...
        for(int i=0; i < efiles.count(); i++)
        {
            QFileInfo segmentfile(efilepath + efiles.at(i));
            imgsize += segmentfile.size();
            //qDebug() << "segment size:" << segmentfile.size();
        }
        //qDebug() << "final imagesize:" << imgsize;
    }
    else if(imgtype == 6) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgfile.toStdString().c_str());
        imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
    }
    else if(imgtype == 7) // WFI
    {
        QFile wfile(imgpath);
        if(!wfile.isOpen())
            wfile.open(QIODevice::ReadOnly);
        QDataStream in(&wfile);
        if(in.version() != QDataStream::Qt_5_15)
        {
            qDebug() << "Wrong Qt Data Stream version:" << in.version();
        }
        quint64 header;
        uint8_t version;
        quint16 sectorsize;
        quint32 blocksize;
        quint64 totalbytes;
        in >> header >> version >> sectorsize >> blocksize >> totalbytes;
        imgsize = totalbytes;
        wfile.seek(0);
        // HOW TO GET FRAME INDEX LIST OUT OF THE WFI FILE 
        
        //QList<qint64> frameindxlist;
        framelist.clear();
        FindNextFrame(0, &framelist, &wfile);
        wfile.close();
    }
    else if(imgtype == 8) // WLI
    {
        QFile wfile(imgpath);
        if(!wfile.isOpen())
            wfile.open(QIODevice::ReadOnly);
        QDataStream in(&wfile);
        if(in.version() != QDataStream::Qt_5_15)
        {
            qDebug() << "Wrong Qt Data Stream Version:" << in.version();
        }
        imgsize = wfile.size();
        wfile.close();
        //quint64 header;
        //quint8 version;
        //QString casenumber;
        //QString examiner;
        //QString description;
        //in >> header >> version >> casenumber >> examiner >> description;
    }
    else if(imgtype == 15) // everything else
    {
        //QFile efile(imgpath);
        //if(!efile.isOpen())
        //    efile.open(QIDevice::ReadOnly);
        //if(efile.isOpen())
        //{
            //hashtype = 0; // MD5
        QFileInfo efileinfo(imgpath);
        imgsize = efileinfo.size();
        //efile.close();
        //}

    }
    else if(imgtype = -1)
    {
        imgsize = 0;
    }
    else
	imgsize = 0;
}

ForImg::~ForImg()
{
    //if(imgtype == 0) // EWF
    //{
        //libewf_handle_close(ewfhandle, &ewferror);
        //libewf_handle_free(&ewfhandle, &ewferror);
        //libewf_glob_free(globfiles, globfilecnt, &ewferror);
    //}
}

QByteArray ForImg::ReadContent(qint64 pos, qint64 size)
{
    //qDebug() << "pos:" << pos << "size:" << size;
    //qDebug() << "imgtype in ReadContent:" << imgtype;
    //char* data = new char[size];
    QByteArray tmparray;
    tmparray.clear();
    int retopen = 0;
    if(imgtype == 0) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
        if(imgpath == "")
            return tmparray;
        QString efilepath = imgpath.split(imgpath.split("/").last()).first();
        QDir edir = QDir(imgpath.split(imgpath.split("/").last()).first());
        QStringList efiles = edir.entryList(QStringList() << QString(imgpath.split("/").last().toLower().split(".e01").first() + ".e??") << QString(imgpath.split("/").last().toLower().split(".e01").first() + ".E??"), QDir::NoSymLinks | QDir::Files);
        char* filenames[efiles.count()] = {NULL};
        for(int i=0; i < efiles.count(); i++)
        {
            filenames[i] = QString(efilepath + efiles.at(i)).toLatin1().data();
            //printf("filenames[%d] = %s\n", i, filenames[i]);
        }
        globfilecnt = efiles.count();
        //printf("globfilecnt: %d\n", globfilecnt);
        int retopen = 0;

        retopen = libewf_glob(filenames[0], strlen(filenames[0]), LIBEWF_FORMAT_UNKNOWN, &globfiles, &globfilecnt, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        //else
        //    printf("libewf glob was successful: %d\n", retopen);

        char data[size];
        //char bytebuf[sectorsize];
        //char* data = new char[size];
        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        qint64 res = 0;
        imgoffset = libewf_handle_seek_offset(ewfhandle, pos, SEEK_SET, &ewferror);
        res = libewf_handle_read_buffer(ewfhandle, data, size, &ewferror);
        tmparray = QByteArray::fromRawData((const char*)data, size);
        //delete[] data;
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
    }
    else if(imgtype == 1) // AFF
    {
        char data[size];
        //char* data = new char[size];
        AFFILE* afimage = NULL;
        if(imgpath == "")
            return tmparray;
        afimage = af_open(imgpath.toStdString().c_str(), O_RDONLY|O_EXCL, 0);
        af_seek(afimage, pos, SEEK_SET);
        qint64 res = 0;
        res = af_read(afimage, (unsigned char*)data, size);
        tmparray = QByteArray::fromRawData((const char*)data, size);
        //delete[] data;
        af_close(afimage);
    }
    else if(imgtype == 2) // RAW
    {
	QFile tmpfile(imgpath);
	if(!tmpfile.isOpen())
	    tmpfile.open(QIODevice::ReadOnly);
	if(tmpfile.isOpen())
	{
	    tmpfile.seek(pos);
	    tmparray = tmpfile.read(size);
	    tmpfile.close();
	}
    }
    else if(imgtype == 3) // SPLIT RAW
    {
        //libsmraw_handle_t* smhandle = NULL;
        //libsmraw_error_t* smerror = NULL;

	QString efilepath = imgpath.split(imgpath.split("/").last()).first();
	QString fileprefix = "";
	if(imgpath.toLower().endsWith(".000"))
	    fileprefix = imgpath.split("/").last().split(".000").first();
	else if(imgpath.toLower().endsWith(".001"))
	    fileprefix = imgpath.split("/").last().split(".001").first();
	else if(imgpath.toLower().endsWith(".aaa"))
	    fileprefix = imgpath.split("/").last().split(".aaa", Qt::SkipEmptyParts, Qt::CaseInsensitive).first();
	QDir edir = QDir(efilepath);
	QStringList efiles = edir.entryList(QStringList() << QString(fileprefix + ".???"), QDir::NoSymLinks | QDir::Files);

        //char* filenames[efiles.count()] = {NULL};
        //for(int i=0; i < efiles.count(); i++)
        //    filenames[i] = QString(efilepath + efiles.at(i)).toLatin1().data();
        //int retopen = 0;
        //retopen = libsmraw_handle_initialize(&smhandle, &smerror);
        //if(retopen == -1)
        //    libsmraw_error_fprint(smerror, stdout);
        //retopen = libsmraw_handle_open(smhandle, filenames, efiles.count(), LIBSMRAW_OPEN_READ, &smerror);
        //if(retopen == -1)
        //    libsmraw_error_fprint(smerror, stdout);
        //qint64 res = 0;
        //imgoffset = libsmraw_handle_seek_offset(smhandle, pos, SEEK_SET, &smerror);
        //if(retopen == -1)
        //    libsmraw_error_fprint(smerror, stdout);
        //res = libsmraw_handle_read_buffer(smhandle, data, size, &smerror);
        //if(retopen == -1)
        //    libsmraw_error_fprint(smerror, stdout);
        //tmparray = QByteArray::fromRawData((const char*)data, size);
        //libsmraw_handle_close(smhandle, &smerror);
        //libsmraw_handle_free(&smhandle, &smerror);
        //libsmraw_error_free(&smerror);

        QFileInfo segmentfile(efilepath + efiles.at(0));
        off64_t segsize = segmentfile.size();
        uint16_t segstart = pos / segsize;
        off64_t relpos = pos - segstart * segsize;
        off64_t relsize = segsize - pos;
        if(pos + size > (segstart + 1) * segsize)
        {
            // will need to figure this out and then account for the extra size for the next segment and add to the tmparray
            qDebug() << "goes into next segment... and maybe more...";
        }
        QFile tmpfile(efilepath + efiles.at(segstart));
        if(!tmpfile.isOpen())
            tmpfile.open(QIODevice::ReadOnly);
        if(tmpfile.isOpen())
        {
            tmpfile.seek(relpos);
            tmparray = tmpfile.read(size);
            tmpfile.close();
        }
    }
    else if(imgtype == 6) // AFF4
    {
        char* data = new char[size];
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.toStdString().c_str());
        int bytesread = AFF4_read(aff4handle, pos, data, size);
        tmparray = QByteArray::fromRawData((const char*)data, size);
        AFF4_close(aff4handle);
    }
    else if(imgtype == 7) // WFI
    {
        QFile wfi(imgpath);
        if(!wfi.isOpen())
            wfi.open(QIODevice::ReadOnly);
        else
        {
            qDebug() << "wfi file failed to open";
        }
        QDataStream in(&wfi);
        if(in.version() != QDataStream::Qt_5_15)
        {
            qDebug() << "Wrong Qt Data Stream version:" << in.version();
        }

        wfi.seek(0);
        quint64 header;
        uint8_t version;
        quint16 sectorsize;
        quint32 blocksize;
        quint64 totalbytes;
        QString casenumber;
        QString evidnumber;
        QString examiner;
        QString description;
        in >> header >> version >> sectorsize >> blocksize >> totalbytes >> casenumber >> evidnumber >> examiner >> description;
	
	qint64 lz4start = wfi.pos();
        //qDebug() << "lz4start:" << lz4start; // IF IT IS OFF BY 35, THEN THIS IS WHAT IS SCREWING UP THE HEXVIEWER DISPLAY BYTES...
        qint64 lz4size = wfi.size() - wfi.pos() - 132; // wfi file size - position aat start of 1st frame - blake3 hash at the end

        if(header != 0x776f6d6261746669)
        {
            qDebug() << "Wrong file type, not a wombat forensic image.";
        }
        if(version != 1)
        {
            qDebug() << "Not the correct wombat forensic image format.";
        }
        LZ4F_dctx* lz4dctx;
        LZ4F_errorCode_t errcode;
        errcode = LZ4F_createDecompressionContext(&lz4dctx, LZ4F_getVersion());
        char* cmpbuf = new char[2*blocksize];
        QByteArray framearray;
        framearray.clear();
        quint64 frameoffset = 0;
        quint64 framesize = 0;
        size_t ret = 1;
        size_t bread = 0;
        size_t rawbufsize = blocksize;
        size_t dstsize = rawbufsize;
        char* rawbuf = new char[rawbufsize];

	qint64 indxstart = pos / blocksize;
	qint8 posodd = pos % blocksize;
	qint64 relpos = pos - (indxstart * blocksize);
	qint64 indxcnt = size / blocksize;
	if(indxcnt == 0)
	    indxcnt = 1;
	if(posodd != 0 && (relpos + size) > blocksize)
	    indxcnt++;
        qint64 indxend = indxstart + indxcnt;
        if(indxend > totalbytes / blocksize) // this if clause causes fuse to fail, so maybe check here and see...
        {
            qDebug() << "indxend is larger than totalbytes / sectorsize";
        }
	//qDebug() << "requested pos:" << pos << "relpos:" << relpos << "requested size:" << size;
	//qDebug() << "indxstart:" << indxstart << "posodd:" << posodd << "indxcnt:" << indxcnt;
        //QStringList indxlist = getindx.split(",", Qt::SkipEmptyParts);
	for(int i=indxstart; i < indxstart + indxcnt; i++)
	{
            frameoffset = framelist.at(i);
            //frameoffset = indxlist.at(i).toULongLong();
	    if(i == ((totalbytes / blocksize) - 1))
		framesize = lz4size - frameoffset;
	    else
                framesize = framelist.at(i+1) - frameoffset;
                //framesize = indxlist.at(i+1).toULongLong() - frameoffset;
	    wfi.seek(frameoffset);
	    //wfi.seek(lz4start + frameoffset);
	    int bytesread = in.readRawData(cmpbuf, framesize);
	    bread = bytesread;
	    ret = LZ4F_decompress(lz4dctx, rawbuf, &dstsize, cmpbuf, &bread, NULL);
	    QByteArray blockarray(rawbuf, dstsize);
	    framearray.append(blockarray);
	}
        //qDebug() << "framecnt:" << totalbytes / sectorsize;
        //if(ret > 0)
        //    qDebug() << "ret value:" << ret;

	//qDebug() << "size requested:" << size << "size read:" << dstsize;
        errcode = LZ4F_freeDecompressionContext(lz4dctx);
        delete[] cmpbuf;
        delete[] rawbuf;
        wfi.close();
	tmparray.clear();
	if(posodd == 0)
            tmparray = framearray.mid(0, size);
	else
            tmparray = framearray.mid(relpos, size);
	//qDebug() << "tmparray:" << tmparray.toHex();
    }
    else if(imgtype == 8) // WLI
    {
        QFile wfile(imgpath);
        if(!wfile.isOpen())
            wfile.open(QIODevice::ReadOnly);
        wfile.seek(pos);
        tmparray = wfile.read(size);
        wfile.close();
    }
    else if(imgtype == 15) // everything else
    {
	QFile tmpfile(imgpath);
	if(!tmpfile.isOpen())
	    tmpfile.open(QIODevice::ReadOnly);
	if(tmpfile.isOpen())
	{
	    tmpfile.seek(pos);
	    tmparray = tmpfile.read(size);
	    tmpfile.close();
	}
    }
    else if(imgtype == -1) // no file loaded yet
    {
	tmparray.clear();
        //tmparray.append("zero");
    }
    else
	tmparray.clear();
    //delete[] data;

    return tmparray;
}

qint64 ForImg::Size()
{
    return imgsize;
}

int8_t ForImg::ImgType()
{
    return imgtype;
}
*/

std::string ForImg::ImagePath()
{
    return imgpath;
}

/*
QString ForImg::ImgPath()
{
    return imgpath;
}

QString ForImg::MountPath()
{
    return mntpath;
}

void ForImg::SetMountPath(QString mountpath)
{
    mntpath = mountpath;
    mntpath.chop(1);
}
*/
