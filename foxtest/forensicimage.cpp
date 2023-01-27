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
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 2) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
	int found = imgpath.rfind(".E");
	if(found == -1)
	    found = imgpath.rfind(".e");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	//std::cout << "Image Path: " << imgpath << std::endl;
	//std::cout << "Image Pre Match: " << imgprematch << std::endl;
	//std::cout << "imagepath: " << imagepath.string() << std::endl;
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
	libewf_error_free(&ewferror);
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 3) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.c_str());
        imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
	//std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 4) // SPLIT RAW
    {
	libsmraw_handle_t* smhandle = NULL;
	libsmraw_error_t* smerror = NULL;
	char** globfiles = NULL;
	int globfilecnt = 0;
	int found = imgpath.rfind(".0");
	if(found == -1)
	    found = imgpath.rfind(".a");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
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
	retopen = libsmraw_glob(filenames[0], strlen(filenames[0]), &globfiles, &globfilecnt, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_initialize(&smhandle, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_open(smhandle, globfiles, globfilecnt, LIBSMRAW_OPEN_READ, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_get_media_size(smhandle, &imgsize, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_close(smhandle, &smerror);
	libsmraw_handle_free(&smhandle, &smerror);
	libsmraw_glob_free(globfiles, globfilecnt, &smerror);
	libsmraw_error_free(&smerror);
        //std::cout << imgfile << " size: " << imgsize << std::endl;
    }
    else if(imgtype == 5) // WFI
    {
	FILE* wfifile = NULL;
	wfifile = fopen(imgpath.c_str(), "rb");
	fseek(wfifile, 0, SEEK_END);
	fseek(wfifile, -264, SEEK_CUR);
	fread(&wfimd, sizeof(struct wfi_metadata), 1, wfifile);
	fclose(wfifile);
        imgsize = wfimd.totalbytes;
	//std::cout << imgpath << " size: " << wfimd.totalbytes << std::endl;
	/*
	printf("\nwombatinfo v0.1\n\n");
	printf("Raw Media Size: %llu bytes\n", wfimd.totalbytes);
	printf("Case Number:\t %s\n", wfimd.casenumber);
	printf("Examiner:\t %s\n", wfimd.examiner);
	printf("Evidence Number: %s\n", wfimd.evidencenumber);
	printf("Description:\t %s\n", wfimd.description);
	printf("BLAKE3 Hash:\t ");
	for(size_t i=0; i < 32; i++)
	    printf("%02x", wfimd.devhash[i]);
	printf("\n");
	 */ 
    }
    else if(imgtype == 6) // WLI
    {
    }
    else // everything else
    {
	std::filesystem::path imagepath(imgpath);
        imgsize = std::filesystem::file_size(imgpath);
	//std::cout << imgfile << " size: " << std::filesystem::file_size(imgpath) << std::endl;
    }
}

void ForImg::ReadContent(uint8_t* buf, uint64_t pos, uint64_t size)
{
    if(imgtype == 1) // RAW
    {
        imagebuffer.open(imgpath.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(pos);
        imagebuffer.read((char*)buf, size);
        imagebuffer.close();
    }
    else if(imgtype == 2) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
        int found = imgpath.rfind(".E");
        if(found == -1)
            found = imgpath.rfind(".e");
        std::string imgprematch = imgpath.substr(0, found+2);
        std::filesystem::path imagepath(imgpath);
        imagepath.remove_filename();
        for(const auto &file : std::filesystem::directory_iterator(imagepath))
        {
            if(file.path().string().compare(0, found+2, imgprematch) == 0)
                globfilecnt++;
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
        uint64_t res = 0;
        imgoffset = libewf_handle_seek_offset(ewfhandle, pos, SEEK_SET, &ewferror);
        res = libewf_handle_read_buffer(ewfhandle, buf, size, &ewferror);
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
        libewf_error_free(&ewferror);
    }
    else if(imgtype == 3) // AFF4
    {
        AFF4_init();
        int aff4handle = AFF4_open(imgpath.c_str());
        int bytesread = AFF4_read(aff4handle, pos, buf, size);
        AFF4_close(aff4handle);
    }
    else if(imgtype == 4) // SPLIT RAW
    {
	libsmraw_handle_t* smhandle = NULL;
	libsmraw_error_t* smerror = NULL;
	char** globfiles = NULL;
	int globfilecnt = 0;
	int found = imgpath.rfind(".0");
	if(found == -1)
	    found = imgpath.rfind(".a");
	std::string imgprematch = imgpath.substr(0, found+2);
	std::filesystem::path imagepath(imgpath);
	imagepath.remove_filename();
	for(const auto &file : std::filesystem::directory_iterator(imagepath))
	{
	    if(file.path().string().compare(0, found+2, imgprematch) == 0)
		globfilecnt++;
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
	retopen = libsmraw_glob(filenames[0], strlen(filenames[0]), &globfiles, &globfilecnt, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_initialize(&smhandle, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	retopen = libsmraw_handle_open(smhandle, globfiles, globfilecnt, LIBSMRAW_OPEN_READ, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
        uint64_t res = 0;
        imgoffset = libsmraw_handle_seek_offset(smhandle, pos, SEEK_SET, &smerror);
        res = libsmraw_handle_read_buffer(smhandle, buf, size, &smerror);
	if(retopen == -1)
	    libsmraw_error_fprint(smerror, stdout);
	libsmraw_handle_close(smhandle, &smerror);
	libsmraw_handle_free(&smhandle, &smerror);
	libsmraw_glob_free(globfiles, globfilecnt, &smerror);
	libsmraw_error_free(&smerror);
    }
    else if(imgtype == 5) // WFI
    {
        FILE* fout = NULL;
        fout = fopen(imgpath.c_str(), "rb");
        size_t bufinsize = ZSTD_DStreamInSize();
        void* bufin = malloc(bufinsize);
        size_t bufoutsize = ZSTD_DStreamOutSize();
        void* bufout = malloc(bufoutsize);
        ZSTD_DCtx* dctx = ZSTD_createDCtx();
        size_t toread = bufinsize;
        size_t read;
        size_t lastret = 0;
        int isempty = 1;
        uint64_t indxstart = pos / bufoutsize;
        uint64_t relpos = pos - (indxstart * bufoutsize);
        uint64_t startingblock = pos / bufoutsize;
        uint64_t endingblock = (pos + size) / bufoutsize;
        int posodd = (pos + size) % bufoutsize;
        if(posodd > 0)
            endingblock++;
        size_t tmpbufsize = bufoutsize * (endingblock - startingblock + 1);
        char* tmpbuffer = (char*)malloc(tmpbufsize);
        int blkcnt = 0;
        int bufblkoff = 0;
        size_t readcount = 0;
        size_t outcount = 0;
        while( (read = fread(bufin, 1, toread, fout)) )
        {
            readcount = readcount + read;
            isempty = 0;

            ZSTD_inBuffer input = { bufin, read, 0 };
            while(input.pos < input.size)
            {
                ZSTD_outBuffer output = { bufout, bufoutsize, 0 };
                size_t ret = ZSTD_decompressStream(dctx, &output, &input);
                if(blkcnt >= startingblock && blkcnt <= endingblock)
                {
                    memcpy(tmpbuffer+(bufblkoff*bufoutsize), bufout, bufoutsize);
                    bufblkoff++;
                }
                outcount = outcount + output.pos;
                blkcnt++;

                lastret = ret;
            }
        }
        memcpy(buf, tmpbuffer+relpos, size);
    }
    else if(imgtype == 6) // WLI
    {
    }
    else // EVERYTHING ELSE
    {
        imagebuffer.open(imgpath.c_str(), std::ios::in|std::ios::binary);
        imagebuffer.seekg(pos);
        imagebuffer.read((char*)buf, size);
        imagebuffer.close();
    }
}
/*
QByteArray ForImg::ReadContent(qint64 pos, qint64 size)
{
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
*/

uint64_t ForImg::Size()
{
    return imgsize;
}

int8_t ForImg::ImgType()
{
    return imgtype;
}

std::string ForImg::ImagePath()
{
    return imgpath;
}

/*
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
