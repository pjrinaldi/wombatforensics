#include "imagefunctions.h"
//#include "makezmg.h"

void FindNextFrame(qint64 initialindex, QList<qint64>* framelist, QFile* wfi)
{
    //qDebug() << "initial index:" << initialindex;
    if(!wfi->isOpen())
        wfi->open(QIODevice::ReadOnly);
    wfi->seek(initialindex);
    QByteArray srcharray = wfi->peek(131072);
    int srchindx = srcharray.toHex().indexOf("04224d18");
    if(srchindx == -1)
    {
        //qDebug() << "this should occur after the last frame near the end of the file";
    }
    //int srchindx = srcharray.toHex().indexOf("04224d18", initialindex*2);
    wfi->seek(initialindex + srchindx/2);
    if(qFromBigEndian<qint32>(wfi->peek(4)) == 0x04224d18)
    {
        //qDebug() << "frame found:" << srchindx/2;
        framelist->append(initialindex + srchindx/2);
        FindNextFrame(initialindex + srchindx/2 + 1, framelist, wfi);
    }
    //else
    //    qDebug() << "frame error:" << srchindx/2;
}

unsigned long long GetTotalBytes(std::string instr)
{
    unsigned long long totbyt = 0;
    int infile = open(instr.c_str(), O_RDONLY);
    ioctl(infile, BLKGETSIZE64, &totbyt);
    close(infile);
    return totbyt;
}

/*
 * SHOULD INCORPORATE THE LOG INTO THE RAW DD IMAGE FILE AND CALL IT A NEW FILETYPE EXTENTSION
 * </HEADER>CASENUMBER|EVIDENCENUMBER|EXAMINER|DESCRIPTION|STARTDATE</HEADER>RAWIMAGECONTENT</FOOTER><MD5></MD5><SHA1></SHA1><SHA256></SHA256>ENDDATE</FOOTER>
 */ 
void StartImaging(std::string instring, std::string outpath, std::string outstr, std::string footstr, int radio) 
{
    if(radio == 0) // RAW
    {
        ReadBytes(instring, std::string(outpath + "/" + outstr + ".dd"), footstr);
        printf("Raw Forensic Image Finished Successfully.\n");
    }
    else if(radio == 1) // AFF4
    {
        /*
        ReadBytes(instring, std::string(outpath + "/" + outstr + ".dd"));
        std::string aff4cmd = std::string(getenv("HOME")) + std::string("/.local/share/wombatforensics/aff4imager");
        aff4cmd += " -i " + outpath + "/" + outstr + ".dd -i " + outpath + "/" + outstr + ".dd.log -o " + outpath + "/" + outstr + ".aff4";
        system(aff4cmd.c_str());
        std::remove(std::string(outpath + "/" + outstr + ".dd").c_str());
        printf("AFF4'd Forensic Image Finished Successfully.\n");
        */
    }
    else if(radio == 2) // SFS
    {
        /*
        ReadBytes(instring, std::string(outpath + "/" + outstr + ".dd"));
        std::string sqshcmd = "mksquashfs " + outpath + "/" + outstr + ".dd " + outpath + "/" + outstr + ".dd.log " + outpath + "/" + outstr + ".sfs";
        system(sqshcmd.c_str());
        std::remove(std::string(outpath + "/" + outstr + ".dd").c_str());
        std::remove(std::string(outpath + "/" + outstr + ".dd.log").c_str());
        printf("Squashfs'd Forensic Image Finished Successfully.\n");
        */
    }
    else if(radio == 3) // ZMG
    {
        /*
        ReadBytes(instring, std::string(outpath + "/" + outstr + ".zmg.raw"));
        printf("outpath: %s", outpath.c_str());
        mkdir(std::string(outpath + "/tmp").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        rename(std::string(outpath + "/" + outstr + ".zmg.raw").c_str(), std::string(outpath + "/tmp/" + outstr + ".zmg.raw").c_str());
        rename(std::string(outpath + "/" + outstr + ".zmg.raw.log").c_str(), std::string(outpath + "/tmp/" + outstr + ".dd.log").c_str());
        MakeZmg(std::string(outpath + "/tmp"), std::string(outpath + "/" + outstr + ".zmg"));
        std::remove(std::string(outpath + "/tmp/" + outstr + ".zmg.raw").c_str());
        std::remove(std::string(outpath + "/tmp/" + outstr + ".dd.log").c_str());
        std::remove(std::string(outpath + "/tmp").c_str());
        */
    }
}


void ReadBytes(std::string instring, std::string outstr, std::string footstr)
{
    std::ofstream logfile;
    std::size_t lpar = instring.find("(");
    std::size_t rpar = instring.find(")");
    std::string instr = instring.substr(lpar+1, rpar-lpar-1);
    lpar = instring.find("[");
    rpar = instring.find("]");
    std::string serialnumber = instring.substr(lpar+6, rpar-lpar-6);
    lpar = instring.find("[");
    std::string deviceinfo = instring.substr(0, lpar-1);
    std::size_t dotfnd = outstr.rfind(".");
    std::string logoutstr = outstr.substr(0, dotfnd) + ".log";
    printf("logoutstr: %s\n", logoutstr.c_str());

    //std::string zstdstr = outstr.substr(0, dotfnd) + ".wbi";

    time_t starttime = time(NULL);
    logfile.open(logoutstr, std::ofstream::out | std::ofstream::app);
    char buff[35];
    logfile << "Wombat Forensics v0.3 Forensic Image started at: " << GetDateTime(buff) << "\n";
    unsigned long long totalbytes = 0;
    unsigned int sectorsize = 512;
    unsigned long long curpos = 0;
    uint64_t errorcount = 0;
    int infile = open(instr.c_str(), O_RDONLY);
    int outfile = open(outstr.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRWXU);

    /*
    FILE* const fout = fopen(zstdstr.c_str(), "wb");
    size_t const buffinsize = ZSTD_CStreamInSize();
    void* const buffin = malloc(buffinsize);
    size_t const buffoutsize = ZSTD_CStreamOutSize();
    void* const buffout = malloc(buffoutsize);
    ZSTD_CCtx* const cctx = ZSTD_createCCtx();
    ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, 1);
    ZSTD_CCtx_setParameter(cctx, ZSTD_c_checksumFlag, 1);
    ZSTD_CCtx_setParameter(cctx, ZSTD_c_nbWorkers, 4);
    */


    ioctl(infile, BLKGETSIZE64, &totalbytes);
    ioctl(infile, BLKSSZGET, &sectorsize);
    logfile << "\nSource Device\n";
    logfile << "-------------\n";
    logfile << "Device: " << deviceinfo << "\n";
    logfile << "Device Path: " << instr << "\n";
    logfile << "Serial Number: " << serialnumber << "\n";
    logfile << "Size: " << totalbytes << " bytes\n";
    logfile << "Block Size: " << sectorsize << " bytes\n";
    /*
    time_t endtime = time(NULL);
    fprintf(filelog, "Wrote %llu out of %llu bytes\n", curpos, totalbytes);
    fprintf(filelog, "Forensic Image: %s\n", outputstr+3);
    fprintf(filelog, "Forensic Image finished at: %s\n", GetDateTime(buff));
    fprintf(filelog, "Forensic Image created in: %f seconds\n\n", difftime(endtime, starttime));
    printf("\nForensic Image Creation Finished\n");
    for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
    {
        fprintf(filelog, "%02x", sourcehash[i]);
        printf("%02x", sourcehash[i]);
    }
    fprintf(filelog, " - BLAKE3 Source Device\n");
    printf(" - BLAKE3 Source Device\n");
    for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
    {
        fprintf(filelog, "%02x", forimghash[i]);
        printf("%02x", forimghash[i]);
    }
    fprintf(filelog, " - BLAKE3 Forensic Image\n");
    printf(" - BLAKE3 Forensic Image\n");
    if(memcmp(sourcehash, forimghash, BLAKE3_OUT_LEN) == 0)
    {
        printf("\nVerification Successful\n");
        fprintf(filelog, "\nVerification Successful\n");
    }
    else
    {
        printf("\nVerification Failed\n");
        fprintf(filelog, "\nVerification Failed\n");
    }
    fprintf(filelog, "\nFinished Forensic Image Verification at: %s\n", GetDateTime(buff));
    fprintf(filelog, "Finished Forensic Image Verification in: %f seconds\n", difftime(time(NULL), starttime));
    fclose(filelog);
    close(infile);
    close(outfile);
     */ 
    lseek(infile, 0, SEEK_SET);
    lseek(outfile, 0, SEEK_SET);
    uint8_t sourcehash[BLAKE3_OUT_LEN];
    uint8_t forimghash[BLAKE3_OUT_LEN];
    int i;
    blake3_hasher srchash;
    blake3_hasher imghash;
    blake3_hasher_init(&srchash);
    blake3_hasher_init(&imghash);
    /*
    unsigned char c[MD5_DIGEST_LENGTH];
    unsigned char o[MD5_DIGEST_LENGTH];
    int i;
    MD5_CTX mdcontext;
    MD5_CTX outcontext;
    MD5_Init(&mdcontext);
    MD5_Init(&outcontext);
    */
    while (curpos < totalbytes)
    {
        char bytebuf[sectorsize];
        memset(bytebuf, 0, sizeof(bytebuf));
        char imgbuf[sectorsize];
        memset(imgbuf, 0, sizeof(imgbuf));
	ssize_t bytesread = read(infile, bytebuf, sectorsize);
        if(bytesread == -1)
        {
            memset(bytebuf, 0, sizeof(bytebuf));
            errorcount++;
            perror("Read Error, Writing zeros instead.");
        }
	ssize_t byteswrite = write(outfile, bytebuf, sectorsize);
        if(byteswrite == -1)
            perror("Write error, I haven't accounted for this yet so you probably want to use dc3dd instead.");
        /*
        if(bytesread == -1)
            perror("Read Error:");
	if(byteswrite == -1)
	    perror("Write Error:");
        */
        //MD5_Update(&mdcontext, bytebuf, bytesread);
        blake3_hasher_update(&srchash, bytebuf, bytesread);
        ssize_t byteswrote = pread(outfile, imgbuf, sectorsize, curpos);
        blake3_hasher_update(&imghash, imgbuf, byteswrote);
        //MD5_Update(&outcontext, imgbuf, byteswrote);
	curpos = curpos + sectorsize;
	printf("Wrote %lld of %lld bytes\r", curpos, totalbytes);
	fflush(stdout);
    }
    blake3_hasher_finalize(&srchash, sourcehash, BLAKE3_OUT_LEN);
    blake3_hasher_finalize(&imghash, forimghash, BLAKE3_OUT_LEN);
    //MD5_Final(c, &mdcontext);
    //MD5_Final(o, &outcontext);
    time_t endtime = time(NULL);
    logfile << "Wrote " << curpos << " of " << totalbytes << " bytes\n";
    logfile << "Forensic Image: " << outstr << "\n";
    logfile << errorcount << " blocks replaced with zero's\n";
    //fprintf(filelog, "%llu blocks replaced with zeros\n", errorcount);
    logfile << "Forensic Image finished at: " << GetDateTime(buff) << "\n";
    logfile << "Forensic Image created in: " << difftime(endtime, starttime) << " seconds\n\n";
    printf("\nForensic Image Creation Finished!\n");
    std::stringstream srcstr;
    /*
    std::stringstream srcstr;
    std::stringstream imgstr;
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        srcstr << std::hex << (int)c[i];
	printf("%02x", c[i]);
    }
    printf(" - MD5 Source Device\n");
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        imgstr << std::hex << (int)o[i];
	printf("%02x", o[i]);
    }
    printf(" - MD5 Forensic Image\n");
    std::string srcmd5 = "";
    std::string imgmd5 = "";
    srcmd5 = srcstr.str();
    imgmd5 = imgstr.str();
    logfile << srcmd5 << " - MD5 Source Device\n";
    logfile << imgmd5 << " - MD5 Forensic Image\n";
    if(srcmd5.compare(imgmd5) == 0)
    {
	printf("Verification Successful\n");
        logfile << "\nVerification Successful\n";
    }
    else
    {
	printf("Verification Failed\n");
        logfile << "Verification Failed\n";
    }
    */
    for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
    {
        srcstr << std::hex << (int)sourcehash[i];
        logfile << std::hex << (int)sourcehash[i];
        //srcstr << std::hex << sourcehash[i];
        //fprintf((logfile, "%02x", sourcehash[i]);
        printf("%02x", sourcehash[i]);
    }
    logfile << " - BLAKE3 Source Device\n";
    //fprintf(filelog, " - BLAKE3 Source Device\n");
    printf(" - BLAKE3 Source Device\n");
    for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
    {
        logfile << std::hex << (int)forimghash[i];
        //fprintf(logfile, "%02x", forimghash[i]);
        printf("%02x", forimghash[i]);
    }
    footstr += "<size>" + QString::number(totalbytes).toStdString() + "</size><blake3>" + srcstr.str() + "</blake3></imglog>";
    
    write(outfile, footstr.c_str(), sizeof(char)*footstr.size());
    logfile << " - BLAKE3 Forensic Image\n";
    //fprintf(filelog, " - BLAKE3 Forensic Image\n");
    printf(" - BLAKE3 Forensic Image\n");
    if(memcmp(sourcehash, forimghash, BLAKE3_OUT_LEN) == 0)
    {
        printf("\nVerification Successful\n");
        logfile << "\nVerification Successful\n";
        //fprintf(filelog, "\nVerification Successful\n");
    }
    else
    {
        printf("\nVerification Failed\n");
        logfile << "\nVerification Failed\n";
        //fprintf(filelog, "\nVerification Failed\n");
    }
    logfile << "\nFinished Forensic Image Verification at " << GetDateTime(buff) << "\n";
    logfile << "Finished Forensic Image Verification in: " << difftime(endtime, starttime) << " seconds\n";
    logfile.close();
    close(infile);
    close(outfile);
}

std::string Verify(QString outstr)
{
    qint64 imagesize = 0;
    uint8_t hashtype = 0;
    unsigned long long totalbytes = 0;
    quint64 curpos = 0;
    unsigned int sectorsize = 512;
    QString blake3str = "";

    QFile efile(outstr);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        QDataStream in(&efile);
        efile.seek(0);
        if(in.version() != QDataStream::Qt_5_15)
        {
            //qDebug() << "Wrong Qt Data Stream version:" << in.version();
            //return 1;
        }
        quint64 header;
        in >> header;
        if(header == 0x776f6d6261746669) // wombat forensic image
        {
            hashtype = 1; // WFI IMAGE
        }
        else if(header == 0x776f6d6261746c69) // wombat logical image
        {
            hashtype = 2; // WLI IMAGE
        }
        else
            hashtype = 0; // MD5
        efile.close();
    }

    /*
    QFile efile(outstr);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(efile.isOpen())
    {
        imagesize = efile.size();
        efile.seek(efile.size() - 9);
        if(QString(efile.read(9)) == "</imglog>")
        {
            hashtype = 1; // BLAKE3
            efile.seek(efile.size() - 1024);
            QByteArray getlog = efile.read(1024);
            int sizestart = getlog.indexOf("<size>");
            int sizeend = getlog.indexOf("</size>");
            imagesize = getlog.mid(sizestart + 6, sizeend - sizestart - 6).toInt();
            int b3start = getlog.indexOf("<blake3>");
            int b3end = getlog.indexOf("</blake3>");
            blake3str = getlog.mid(b3start + 8, b3end - b3start - 8);
            qDebug() << "blake3str:" << blake3str;
        }
        else
        {
            hashtype = 0;
        }
        efile.close();
        qDebug() << "imagesize:" << imagesize;
    }
    */

    // NEED TO UPDATE THIS TO TAKE INTO ACCOUNT E01, RAW, SPLIT RAW, AND AFF PROPERLY.
    /*
    std::string mntoutstr = "";
    if(outstr.toLower().endsWith(".zmg"))
        mntoutstr = "/tmp/wombatforensics/datamnt/" + outstr.split("/").last().toLower().split(".zmg").first().toStdString() + ".dd";
    else if(outstr.toLower().endsWith(".e01"))
        mntoutstr = "/tmp/wombatforensics/datamnt/" + outstr.split("/").last().toStdString() + ".raw";
    else if(outstr.toLower().endsWith(".aff"))
        mntoutstr = "/tmp/wombatforensics/datamnt/" + outstr.split("/").last().toStdString() + ".raw";
    else if(outstr.toLower().endsWith(".sfs"))
        mntoutstr = "/tmp/wombatforensics/datamnt/" + outstr.split("/").last().toLower().split(".sfs").first().toStdString() + ".dd";
    */
    printf("outstr: %s\n", outstr.toStdString().c_str());
    //printf("mntoutstr: %s\n", mntoutstr.c_str());
    
    std::size_t rfound = outstr.toStdString().rfind("/");
    std::string imgname = outstr.toStdString().substr(rfound+1);
    //printf("imgname: %s\n", imgname.c_str());
    //std::ifstream infile(mntoutstr + ".log");
    std::string md5hash = "";
    std::string curstr = "";
    if(hashtype == 0) // MD5
    {
        std::size_t dotfnd = imgname.rfind(".");
        std::string instr = imgname.substr(0, dotfnd);
        std::ifstream infile(instr + ".log");
        while(std::getline(infile, curstr))
        {
            std::size_t found = curstr.find(" - MD5 Forensic Image");
            if(found != std::string::npos)
                md5hash = curstr.substr(0, found);
        }
        infile.close();
    }
    /*
    else if(hashtype == 1) // BLAKE3
    {
        // get imgsize and blake3 hash
    }
    */
    if(md5hash.empty())
    {
        if(outstr.toLower().endsWith(".e01"))
        {
            // implement EWF GET MD5 here...
            libewf_handle_t* ewfhandle = NULL;
            libewf_error_t* ewferror = NULL;
            char* filenames[1] = {NULL};
            char* iname = new char[outstr.toStdString().size() + 1];
            strcpy(iname, outstr.toStdString().c_str());
            filenames[0] = (char*)iname;
            libewf_handle_initialize(&ewfhandle, &ewferror);
            libewf_handle_open(ewfhandle, filenames, 1, LIBEWF_OPEN_READ, &ewferror);
            uint8_t md5_hash[32];
            libewf_handle_get_utf8_hash_value_md5(ewfhandle, md5_hash, 32, &ewferror);
            md5hash = QString::fromUtf8(reinterpret_cast<char*>(md5_hash)).toStdString();
            libewf_handle_close(ewfhandle, &ewferror);
            libewf_handle_free(&ewfhandle, &ewferror);
            //printf("MD5 hash from E01 file: %s\n", md5hash.c_str());
        }
        else if(outstr.toLower().endsWith(".aff"))
        {
            // implement AFF GET MD5 here...
            char* iname = new char[outstr.toStdString().size() + 1];
            strcpy(iname, outstr.toStdString().c_str());
            static AFFILE* afimage = NULL;
            afimage = af_open(iname, O_RDONLY|O_EXCL,0);
	    unsigned char md5_stored[16];
	    size_t md5len = sizeof(md5_stored);
            af_get_seg(afimage, AF_MD5, 0, md5_stored, &md5len);
            std::stringstream ss;
            for(int i=0; i < 16; ++i)
                ss << std::hex << (int)md5_stored[i];
            md5hash = ss.str();
            af_close(afimage);
            //printf("MD5 hash from AFF file: %s\n", md5hash.c_str());
        }
    }
    //printf("original md5: %s\n", md5hash.c_str());
    /*
    int ofile = open(outstr.toStdString().c_str(), O_RDONLY);
    ioctl(ofile, BLKGETSIZE64, &totalbytes);
    if(hashtype == 1) // WFI
    {
        totalbytes = imagesize;
        //totalbytes = imgsize;
    }
    qDebug() << "totalbytes:" << totalbytes;
    //qDebug() << "imgsize:" << imgsize << "totalbytes:" << totalbytes;
    close(ofile);
    */
    //std::ofstream logfile;
    //time_t starttime = time(NULL);
    //char buff[35];
    //logfile.open(outstr + ".log", std::ofstream::out | std::ofstream::app);
    //logfile << "\nStarting Image Verification at " << GetDateTime(buff) << "\n";
    
    // NEED TO SWITCH THIS TO QCRYPTOGRAPHICHASH HASH(), ADDDATA(), RESULT()
    if(hashtype == 0)
    {
	QCryptographicHash tmphash(QCryptographicHash::Md5);
	quint64 curpos = 0;
	// this method worked but failed, i need to actually call ReadContent() to read the content for the img type.
	FILE* outfile = fopen(outstr.toStdString().c_str(), "rb");
	int obytes = 0;
	unsigned char odata[sectorsize];
	while((obytes = fread(odata, 1, sectorsize, outfile)) != 0)
	{
	    curpos = curpos + sectorsize;
	    tmphash.addData(reinterpret_cast<const char*>(odata), obytes);
	    printf("Bytes Read: %lld/%lld\r", curpos, totalbytes);
	    fflush(stdout);
	}
	fclose(outfile);
	QString hashstr = QString(tmphash.result().toHex()).toUpper();
	qDebug() << "MD5 hashstr:" << hashstr;
	if(md5hash.compare(hashstr.toStdString()) == 0)
	{
	    printf("Verification Successful\n");
	    imgname += "Successful";
	}
	else if(md5hash.empty())
	{
	    printf("Manual MD5 Hash Verification Comparison Required\n");
	    imgname += "Requires Manual Comparison";
	}
	else
	{
	    printf("Verification Failed\n");
	    imgname += "Failed";
	}
	/* EXAMPLE CRYPTOGRAPHICHASH CODE
		QByteArray filebytes;
		filebytes.clear();
		//filebytes = ReturnFileContent(objectid);
		QCryptographicHash tmphash((QCryptographicHash::Algorithm)hashsum); ::MD5

		//hashstr = QString(tmphash.hash(filebytes, (QCryptographicHash::Algorithm)hashsum).toHex()).toUpper();
		filebytes.clear();
	 */ 
        /*
        unsigned char o[MD5_DIGEST_LENGTH];
        int i;
        FILE* outfile = fopen(outstr.toStdString().c_str(), "rb");
        MD5_CTX outcontext;
        int obytes;
        unsigned char odata[sectorsize];
        MD5_Init(&outcontext);
        unsigned long long curpos = 0;
        while((obytes = fread(odata, 1, sectorsize, outfile)) != 0)
        {
            curpos = curpos + sectorsize;
            MD5_Update(&outcontext, odata, obytes);
            printf("Bytes Read: %lld/%lld\r", curpos, totalbytes);
            fflush(stdout);
        }
        //logfile << "Bytes Read: " << curpos << "/" << totalbytes << "\n\n";
        MD5_Final(o, &outcontext);
        std::stringstream imgstr;
        for(i = 0; i < MD5_DIGEST_LENGTH; i++)
        {
            imgstr << std::hex << (int)o[i];
            printf("%02x", o[i]);
        }
        printf(" - MD5 Forensic Image\n");
        std::string imgmd5 = "";
        imgmd5 = imgstr.str();
        //logfile << imgmd5 << " - MD5 Forensic Image\n\n";
        
        imgname += " Verification ";
        if(md5hash.compare(imgmd5) == 0)
        {
            printf("Verification Successful\n");
            //logfile << "Verification Successful\n";
            imgname += "Successful";
        }
        else if(md5hash.empty())
        {
            printf("Manual MD5 Hash Verification Comparison Required\n");
            //logfile << "Verification Comparison Required\n";
            imgname += "Requires Manual Comparison";
        }
        else
        {
            printf("Verification Failed\n");
            //logfile << "Verification Failed\n";
            imgname += "Failed";
        }
        fclose(outfile);
        */
    }
    else if(hashtype == 1) // WFI
    {
        QFile wfi(outstr);
        if(!wfi.isOpen())
            wfi.open(QIODevice::ReadOnly);
        QDataStream in(&wfi);
        if(in.version() != QDataStream::Qt_5_15)
        {
            qDebug() << "Wrong Qt Data Stream version:" << in.version();
            //return 1;
        }
        quint64 header;
        in >> header;
        if(header == 0x776f6d6261746669) // wombat forensic image
        {
            // HOW TO GET FRAME INDEX LIST OUT OF THE WFI FILE 
            QList<qint64> frameindxlist;
            frameindxlist.clear();
            FindNextFrame(0, &frameindxlist, &wfi);

            wfi.seek(0);

            uint8_t version;
            quint16 sectorsize;
            quint32 blocksize;
            quint64 totalbytes;
            QString casenumber;
            QString evidnumber;
            QString examiner;
            QString description;
            in >> header >> version >> sectorsize >> blocksize >> totalbytes >> casenumber >> evidnumber >> examiner >> description;
            if(header != 0x776f6d6261746669)
            {
                qDebug() << "Wrong file type, not a wombat forensic image.";
                //return 1;
            }
            if(version != 1)
            {
                qDebug() << "Not the correct wombat forensic image format.";
                //return 1;
            }
            
            //printf("wombatverify v0.1 started at: %s\n", QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss ap").toStdString().c_str());
            uint8_t imghash[BLAKE3_OUT_LEN];
            blake3_hasher imghasher;
            blake3_hasher_init(&imghasher);
            LZ4F_dctx* lz4dctx;
            LZ4F_errorCode_t errcode;
            errcode = LZ4F_createDecompressionContext(&lz4dctx, LZ4F_getVersion());
            //if(LZ4F_isError(errcode))
                //printf("%s\n", LZ4F_getErrorName(errcode));
            char* cmpbuf = new char[2*blocksize];
            quint64 frameoffset = 0;
            quint64 framesize = 0;
            quint64 curpos = 0;

            size_t ret = 1;
            size_t bread = 0;
            for(int i=0; i < (totalbytes / blocksize); i++)
            {
                frameoffset = frameindxlist.at(i);
                //frameoffset = indxlist.at(i).toULongLong();
                if(i == ((totalbytes / blocksize) - 1))
                    framesize = totalbytes - frameoffset;
                else
                {
                    framesize = frameindxlist.at(i+1) - frameoffset;
                    //framesize = indxlist.at(i+1).toULongLong() - frameoffset;
                }
                int bytesread = in.readRawData(cmpbuf, framesize);
                bread = bytesread;
                size_t rawbufsize = blocksize;
                char* rawbuf = new char[rawbufsize];
                size_t dstsize = rawbufsize;
                ret = LZ4F_decompress(lz4dctx, rawbuf, &dstsize, cmpbuf, &bread, NULL);
                //if(LZ4F_isError(ret))
                //    printf("decompress error %s\n", LZ4F_getErrorName(ret));
                blake3_hasher_update(&imghasher, rawbuf, dstsize);
                curpos += dstsize;
                //printf("Verifying %llu of %llu bytes\r", curpos, totalbytes);
                fflush(stdout);
            }
            blake3_hasher_finalize(&imghasher, imghash, BLAKE3_OUT_LEN);
            QString calchash = "";
            for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
            {
                //printf("%02x", imghash[i]);
                calchash.append(QString("%1").arg(imghash[i], 2, 16, QChar('0')));
            }
            //printf(" - Forensic Image Hash\n");

            //HOW TO GET HASH OUT OF THE IMAGE FOR THE WOMBATVERIFY FUNCTION...
            wfi.seek(wfi.size() - 132);
            QString readhash = "";
            in >> readhash;
            wfi.close();
            delete[] cmpbuf;

            errcode = LZ4F_freeDecompressionContext(lz4dctx);

            imgname += " Verification ";
            // VERIFY HASHES HERE...
            if(calchash == readhash)
            {
                imgname += "Successful";
                //printf("\nVerification Successful\n");
            }
            else
            {
                imgname += "Failed";
                //printf("\nVerification Failed\n");
            }
            //printf("Finished Forensic Image Verification at %s\n", QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss ap").toStdString().c_str());
        }

        /*
        int obytes = 0;
        std::stringstream srcstr;
        int infile = open(outstr.toStdString().c_str(), O_RDONLY);
        lseek(infile, 0, SEEK_SET);
        uint8_t imghash[BLAKE3_OUT_LEN];
        int i;
        blake3_hasher imghasher;
        blake3_hasher_init(&imghasher);
        while(curpos < totalbytes)
        {
            char bytebuf[sectorsize];
            memset(bytebuf, 0, sizeof(bytebuf));
            ssize_t bytesread = read(infile, bytebuf, sectorsize);
            blake3_hasher_update(&imghasher, bytebuf, bytesread);
            curpos = curpos + sectorsize;
            printf("Bytes Read: %lld/%lld\r", curpos, totalbytes);
            fflush(stdout);
        }
        blake3_hasher_finalize(&imghasher, imghash, BLAKE3_OUT_LEN);
        for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
        {
            srcstr << std::hex << (int)imghash[i];
            printf("%02x", imghash[i]);
        }
        printf(" - BLAKE3 Forensic Image\n");
        std::string imgb3 = "";
        imgb3 = srcstr.str();
        imgname += " Verification ";
        if(blake3str.toStdString().compare(imgb3) == 0)
        {
            printf("Verification Successful\n");
            imgname += "Successful";
        }
        else
        {
            printf("Verification Failed\n");
            imgname += "Failed";
        }
        close(infile);
        */
    }
    else if(hashtype == 2) // WLI
    {
        QFile wfi(outstr);
        if(!wfi.isOpen())
            wfi.open(QIODevice::ReadOnly);
        QDataStream in(&wfi);
        if(in.version() != QDataStream::Qt_5_15)
        {
            qDebug() << "Wrong Qt Data Stream version:" << in.version();
            //return 1;
        }
        quint64 header;
        in >> header;
        if(header == 0x776f6d6261746c69) // wombat logical image
        {
            //printf("wombatverify v0.1 started at: %s\n", QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss ap").toStdString().c_str());
            wfi.seek(0);
            qint64 imgsize = wfi.size() - 132;
            blake3_hasher logicalhasher;
            blake3_hasher_init(&logicalhasher);
            qint64 curpos = 0;
            while(!wfi.atEnd())
            {
                QByteArray tmparray = wfi.read(65536);
                curpos += tmparray.count();
                if(curpos > imgsize)
                    tmparray.chop(132);
                blake3_hasher_update(&logicalhasher, tmparray.data(), tmparray.count());
                //printf("Verifying %llu of %llu bytes\r", curpos, imgsize);
                //fflush(stdout);
            }
            // read existing hash
            wfi.seek(imgsize);
            QString readhash;
            in >> readhash;
            wfi.close();
            uint8_t output[BLAKE3_OUT_LEN];
            blake3_hasher_finalize(&logicalhasher, output, BLAKE3_OUT_LEN);
            QString calchash = "";
            for(size_t i=0; i < BLAKE3_OUT_LEN; i++)
                calchash.append(QString("%1").arg(output[i], 2, 16, QChar('0')));

            imgname += " Verification ";
            //printf("%s - Logical Image Hash\n", calchash.toStdString().c_str());
            if(calchash == readhash)
                imgname += "Successful";
                //printf("\nVerification Successful\n");
            else
                imgname += "Failed";
                //printf("\nVerification Failed\n");
            //printf("Finished Logical Image Verification at %s\n", QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss ap").toStdString().c_str());
        }
    }
    //time_t endtime = time(NULL);
    //logfile << "\nFinished Forensic Image Verification at " << GetDateTime(buff) << "\n";
    //logfile << "Finished Forensic Image Verification in: " << difftime(endtime, starttime) << " seconds\n";
    //logfile.close();

    return imgname;
}
/*
 *#include <stdio.h>
 #include <openssl/evp.h>

 main(int argc, char *argv[])
 {
 EVP_MD_CTX *mdctx;
 const EVP_MD *md;
 char mess1[] = "Test Message\n";
 char mess2[] = "Hello World\n";
 unsigned char md_value[EVP_MAX_MD_SIZE];
 int md_len, i;

 OpenSSL_add_all_digests();

 if(!argv[1]) {
        printf("Usage: mdtest digestname\n");
        exit(1);
 }

 md = EVP_get_digestbyname(argv[1]); // will be md = EVP_MD5();
 // may switch to input if the user wants to calculate md5, sha1, or sha256

 if(!md) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
 }

 mdctx = EVP_MD_CTX_create();
 EVP_DigestInit_ex(mdctx, md, NULL);
 EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
 EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
 EVP_DigestFinal_ex(mdctx, md_value, &md_len);
 EVP_MD_CTX_destroy(mdctx);

 printf("Digest is: ");
 for(i = 0; i < md_len; i++)
        printf("%02x", md_value[i]);
 printf("\n");

 // Call this once before exit.
 EVP_cleanup();
 exit(0);
 }
 *
 */ 
ForImg::ForImg(QString imgfile)
{
    if(imgfile.split("/").count() == 0)
        imgfile = -1;
    if(imgfile.split("/").last().toLower().endsWith(".e01"))
        imgtype = 0; // EWF
    else if(imgfile.split("/").last().toLower().endsWith(".aff"))
        imgtype = 1; // AFF
    else if(imgfile.split("/").last().toLower().endsWith(".dd") || imgfile.split("/").last().toLower().endsWith(".raw"))
        imgtype = 2; // RAW
    else if(imgfile.split("/").last().toLower().endsWith(".001") || imgfile.split("/").last().toLower().endsWith(".000") || imgfile.split("/").last().toLower().endsWith("aaa"))
	imgtype = 3; // SPLIT RAW
    else if(imgfile.split("/").last().toLower().endsWith(".zmg"))
        imgtype = 4; // ZMG
    else if(imgfile.split("/").last().toLower().endsWith(".sfs"))
	imgtype = 5; // SFS
    else if(imgfile.split("/").last().toLower().endsWith(".aff4"))
        imgtype = 6; // AFF4
    else if(imgfile.split("/").last().toLower().endsWith(".wfi"))
        imgtype = 7; // WFI
    else if(imgfile.split("/").last().toLower().endsWith(".wli")) // WLI
        imgtype = 8;
    else // any old file..
        imgtype = 15;
    imgpath = imgfile;
    //qDebug() << "imgtype at beginning of ForensicImage:" << imgtype;
    if(imgtype == 0) // EWF
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
        QString efilepath = imgfile.split(imgfile.split("/").last()).first();
        QDir edir = QDir(imgfile.split(imgfile.split("/").last()).first());
        QStringList efiles = edir.entryList(QStringList() << QString(imgfile.split("/").last().toLower().split(".e01").first() + ".e??") << QString(imgfile.split("/").last().toLower().split(".e01").first() + ".E??"), QDir::NoSymLinks | QDir::Files);
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

        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        //else
        //    printf("libewf_handle_open was successful %d\n", retopen);
        libewf_handle_get_media_size(ewfhandle, (size64_t*)&imgsize, &ewferror);
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
    }
    else if(imgtype == 1) // AFF
    {
        //char* iname = new char[imgpath.toStdString().size() + 1];
        //strcpy(iname, imgpath.toStdString().c_str());
        AFFILE* afimage = NULL;
        afimage = af_open(imgpath.toStdString().c_str(), O_RDONLY|O_EXCL, 0);
        imgsize = af_get_imagesize(afimage);
        af_close(afimage);
    }
    else if(imgtype == 2) // RAW
    {
        QFile efile(imgpath);
        if(!efile.isOpen())
            efile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(efile.isOpen())
        {
            qint64 imagesize = efile.size();
            efile.seek(efile.size() - 9);
            if(QString(efile.read(9)) == "</imglog>")
            {
                hashtype = 1; // BLAKE3
                efile.seek(efile.size() - 1024);
                QByteArray getlog = efile.read(1024);
                //QString getlog  QString(efile.read(1024));
                //qDebug() << "getlog:" << getlog;
                int sizestart = getlog.indexOf("<size>");
                int sizeend = getlog.indexOf("</size>");
                imgsize = getlog.mid(sizestart + 6, sizeend - sizestart - 6).toInt();
                //qDebug() << getlog.mid(sizestart + 6, sizeend - sizestart- 6);
                int b3start = getlog.indexOf("<blake3>");
                int b3end = getlog.indexOf("</blake3>");
                QString blake3str = getlog.mid(b3start + 8, b3end - b3start - 8);
                qDebug() << "blake3str:" << blake3str;
                //blake3hash = blake3str.toInt(nullptr, 16);
                //qDebug() << "blake3hash:" << blake3hash;
                //qDebug() << "logstart:" << logstart;
                //imgsize = efile.size() - logstart;
                //qDebug() << "imglog size:" << 1024 - logstart;
                //qDebug() << "imgsize:" << imgsize << "efile size:" << efile.size();
                //qDebug() << "imglog found!";
            }
            else
            {
                hashtype = 0; // MD5
                QFileInfo efileinfo(imgpath);
                imgsize = efileinfo.size();
                //qDebug() << "imglog not found!";
            }
            efile.close();
        }
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
        /*
        AFF4_init();
        int aff4handle = AFF4_open(imgfile.split(imgfile.split("/").last()).first().toStdString().c_str());
        qint64 imgsize = AFF4_object_size(aff4handle);
        AFF4_close(aff4handle);
        */
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
        /*
        quint64 header;
        quint8 version;
        QString casenumber;
        QString examiner;
        QString description;
        in >> header >> version >> casenumber >> examiner >> description;
         */ 
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
    char* data = new char[size];
    QByteArray tmparray;
    tmparray.clear();
    int retopen = 0;
    if(imgtype == 0)
    {
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
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

        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        qint64 res = 0;
        imgoffset = libewf_handle_seek_offset(ewfhandle, pos, SEEK_SET, &ewferror);
        res = libewf_handle_read_buffer(ewfhandle, data, size, &ewferror);
        tmparray = QByteArray::fromRawData((const char*)data, size);
        libewf_handle_close(ewfhandle, &ewferror);
        libewf_handle_free(&ewfhandle, &ewferror);
        libewf_glob_free(globfiles, globfilecnt, &ewferror);
    }
    else if(imgtype == 1) // AFF
    {
        AFFILE* afimage = NULL;
        afimage = af_open(imgpath.toStdString().c_str(), O_RDONLY|O_EXCL, 0);
        af_seek(afimage, pos, SEEK_SET);
        qint64 res = 0;
        res = af_read(afimage, (unsigned char*)data, size);
        tmparray = QByteArray::fromRawData((const char*)data, size);
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

        /*
        char* filenames[efiles.count()] = {NULL};
        for(int i=0; i < efiles.count(); i++)
            filenames[i] = QString(efilepath + efiles.at(i)).toLatin1().data();
        int retopen = 0;
        retopen = libsmraw_handle_initialize(&smhandle, &smerror);
        if(retopen == -1)
            libsmraw_error_fprint(smerror, stdout);
        retopen = libsmraw_handle_open(smhandle, filenames, efiles.count(), LIBSMRAW_OPEN_READ, &smerror);
        if(retopen == -1)
            libsmraw_error_fprint(smerror, stdout);
        qint64 res = 0;
        imgoffset = libsmraw_handle_seek_offset(smhandle, pos, SEEK_SET, &smerror);
        if(retopen == -1)
            libsmraw_error_fprint(smerror, stdout);
        res = libsmraw_handle_read_buffer(smhandle, data, size, &smerror);
        if(retopen == -1)
            libsmraw_error_fprint(smerror, stdout);
        tmparray = QByteArray::fromRawData((const char*)data, size);
        libsmraw_handle_close(smhandle, &smerror);
        libsmraw_handle_free(&smhandle, &smerror);
        libsmraw_error_free(&smerror);
        */

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
        /*
        char* data = new char[size];
        QByteArray tmparray;
        tmparray.clear();

        AFF4_init();
        int aff4handle = AFF4_open(imgpath.split(imgpath.split("/").last()).first().toStdString().c_str());
        int bytesread = AFF4_read(aff4handle, pos, data, size);
        tmparray = QByteArray::fromRawData((const char*)data, size);
        AFF4_close(aff4handle);
        */
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
        /*
	qDebug() << "framearray size:" << framearray.size();
	if(posodd == 0)
	    qDebug() << "framearray zero:" << framearray.mid(0, size).toHex();
	else
	    qDebug() << "framearray rel:" << framearray.mid(relpos, size).toHex();
        */

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
        //tmparray.append("zero");
    }

    return tmparray;
}

qint64 ForImg::Size()
{
    return imgsize;
}

int ForImg::ImgType()
{
    return imgtype;
}

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

/*
MyIODevice::MyIODevice(QString imgfile)
{
    imgpath = imgfile;
    //qDebug() << "imgtype at beginning of ForensicImage:" << imgtype;
}
MyIODevice::~MyIODevice()
{
}
qint64 MyIODevice::readData(char *data, qint64 maxSize)
{
}
qint64 MyIODevice::writeData(char* data, qint64 maxSize)
{
}
*/
