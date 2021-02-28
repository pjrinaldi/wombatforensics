#include "artifactparser.h"
#include <QtEndian>

#include <libfwnt.h>

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

QString ParseLnkArtifact(QString lnkname, QString lnkid)
{
    QString htmlstr = "";
    QFile initfile(":/html/artifactprephtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    htmlstr += "<div id='infotitle'>LNK File Analysis for " + lnkname + " (" + lnkid + ")</div><br/>";
    htmlstr += "<table width='100%'><tr><th>NAME</th><th>Value</th></tr>";
    QString lnkfile = wombatvariable.tmpfilepath + lnkid + "-fhex";
    liblnk_error_t* error = NULL;
    liblnk_file_t* lnkobj = NULL;
    liblnk_file_initialize(&lnkobj, &error);
    liblnk_file_open(lnkobj, lnkfile.toStdString().c_str(), liblnk_get_access_flags_read(), &error);
    if(liblnk_check_file_signature(lnkfile.toStdString().c_str(), &error))
    {
        if(liblnk_file_link_refers_to_file(lnkobj, &error))
        {
            uint64_t gettime = 0;
	    uint32_t tmpuint32 = 0;
	    size_t tmpsize = 0;
            liblnk_file_get_file_creation_time(lnkobj, &gettime, &error);
	    htmlstr += "<tr class='odd'><td class='aval'>Creation Time:</td><td>" + ConvertWindowsTimeToUnixTime(gettime) + "</td></tr>";
	    gettime = 0;
	    liblnk_file_get_file_modification_time(lnkobj, &gettime, &error);
	    htmlstr += "<tr class='even'><td class='aval'>Modification Time:</td><td>" + ConvertWindowsTimeToUnixTime(gettime) + "</td></tr>";
	    gettime = 0;
	    liblnk_file_get_file_access_time(lnkobj, &gettime, &error);
	    htmlstr += "<tr class='odd'><td class='aval'>Access Time:</td><td>" + ConvertWindowsTimeToUnixTime(gettime) + "</td></tr>";
	    liblnk_file_get_file_size(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr class='even'><td class='aval'>File Size:</td><td>" + QString::number(tmpuint32) + " bytes</td></tr>";
	    tmpuint32 = 0;
	    liblnk_file_get_file_attribute_flags(lnkobj, &tmpuint32, &error);
	    htmlstr += "<tr class='odd'><td class='aval vtop'>File Attributes:</td><td class='vtop'>0x" + QString("%1").arg(tmpuint32, 8, 16, QChar('0')) + "<br/>";
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
	    htmlstr += "<tr class='even'><td class='aval'>Drive Type:</td><td>";
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
	    htmlstr += "<tr class='odd'><td class='aval'>Drive Serial Number:</td><td>0x" + QString::number(tmpuint32, 16) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_volume_label_size(lnkobj, &tmpsize, &error);
	    uint8_t volabel[tmpsize];
	    liblnk_file_get_utf8_volume_label(lnkobj, volabel, tmpsize, &error);
	    htmlstr += "<tr class='even'><td class='aval'>Volume Label:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(volabel)) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_local_path_size(lnkobj, &tmpsize, &error);
	    uint8_t localpath[tmpsize];
	    liblnk_file_get_utf8_local_path(lnkobj, localpath, tmpsize, &error);
	    htmlstr += "<tr class='odd'><td class='aval'>Local Path:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(localpath)) + "</td></tr>";
	    tmpsize = 0;
	    liblnk_file_get_utf8_working_directory_size(lnkobj, &tmpsize, &error);
	    uint8_t workdir[tmpsize];
	    liblnk_file_get_utf8_working_directory(lnkobj, workdir, tmpsize, &error);
	    htmlstr += "<tr class='even'><td class='aval'>Working Directory:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(workdir)) + "</td></tr>";
        }
    }
    liblnk_file_close(lnkobj, &error);
    liblnk_file_free(&lnkobj, &error);
    liblnk_error_free(&error);
    htmlstr += "</table></body></html>";

    return htmlstr;
}

QString ParseI30Artifact(QString i30name, QString i30id)
{

    // FROM PROPERTIES FILES, I WILL NEED I CAN GET MFTRECORDLAYOUT AND ESTRING TO ACCESS DD...
    	/*
	 * USE DIRENTRY $I30 TO PARSE TEH FILE SYSTEM, NOW I WILL TRY THE MFT PARSING METHOD, WHICH IS WHAT I SHOULD USE???
	 *
	 *
        QByteArray curmftentryarray = mftarray.mid(curmftentry * fsinfo->value("mftentrybytes").toUInt(), fsinfo->value("mftentrybytes").toUInt());
        int curoffset = 0;
        uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curmftentryarray.mid(20, 2)); // offset to first attribute
        uint16_t attrcount = qFromLittleEndian<uint16_t>(curmftentryarray.mid(40, 2)); // next attribute id
        uint32_t attrlength = 0;
	QByteArray indxrootbuf;
	QByteArray indxallocbuf;
	indxrootbuf.clear();
	indxallocbuf.clear();
        curoffset = firstattroffset;
        for(int i=0; i < attrcount; i++)
        {
            uint32_t attrtype = qFromLittleEndian<uint32_t>(curmftentryarray.mid(curoffset, 4)); // attribute type
            //uint8_t namelength = qFromLittleEndian<uint8_t>(curmftentryarray.mid(curoffset + 9, 1)); // length of name
            attrlength = qFromLittleEndian<uint32_t>(curmftentryarray.mid(curoffset + 4, 4)); // attribute length
	    uint16_t attroffset = qFromLittleEndian<uint16_t>(curmftentryarray.mid(curoffset + 20, 2));
            //qDebug() << "attr type:" << attrtype << "curoffset:" << curoffset << "attrlength:" << attrlength;
            if(attrtype == 0x90) // $INDEX_ROOT - ALWAYS RESIDENT
	    {
                //qDebug() << "process index root here";
		uint32_t atlength = qFromLittleEndian<uint32_t>(curmftentryarray.mid(curoffset + 16, 4));
		indxrootbuf = curmftentryarray.mid(curoffset + attroffset, atlength);
	    }
            else if(attrtype == 0xa0) // $INDEX_ALLOCATION - ALWAYS NON-RESIDENT
	    {
                //qDebug() << "process index allocation here";
		//uint64_t vcnstart = qFromLittleEndian<uint64_t>(curmftentryarray.mid(curoffset + 16, 8));
		//uint64_t vcnend = qFromLittleEndian<uint64_t>(curmftentryarray.mid(curoffset + 24, 8));
		uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentryarray.mid(curoffset + 32, 2));
		//uint64_t actualattrcontentsize = qFromLittleEndian<uint64_t>(curmftentryarray.mid(curoffset + 48, 8));
		curoffset = curoffset + runlistoff;
		int j=0;
		QStringList runlist;
		runlist.clear();
		while(curoffset < fsinfo->value("mftentrybytes").toUInt())
		{
		    if(curmftentryarray.at(curoffset) > 0)
		    {
			int runlengthbytes = QString(QString::number(curmftentryarray.at(curoffset), 16).at(1)).toInt();
			int runlengthoffset = QString(QString::number(curmftentryarray.at(curoffset), 16).at(0)).toInt();
			//qDebug() << "runlengthbytes:" << runlengthbytes << "runlengthoffset:" << runlengthoffset;
			if(runlengthbytes == 0 && runlengthoffset == 0)
			    break;
			curoffset++;
			int runlength = qFromLittleEndian<int>(curmftentryarray.mid(curoffset, runlengthbytes));
			//qDebug() << "runlength:" << runlength;
			int runoffset = qFromLittleEndian<int>(curmftentryarray.mid(curoffset + runlengthbytes, runlengthoffset));
			if(j > 0)
			    runoffset = runoffset + runlist.at(j-1).split(",").at(0).toUInt();
			//qDebug() << "runoffset:" << runoffset << "runlength:" << runlength;
			runlist.append(QString::number((fsinfo->value("partoffset").toUInt() * 512) + runoffset * fsinfo->value("bytespercluster").toUInt()) + "," + QString::number(runlength * fsinfo->value("bytespercluster").toUInt()));
			j++;
			curoffset += runlengthbytes + runlengthoffset;
		    }
		    else
			break;
		}
		//qDebug() << "runlist:" << runlist;
		if(!efile.isOpen())
		    efile.open(QIODevice::ReadOnly);
		if(efile.isOpen())
		{
		    for(j=0; j < runlist.count(); j++)
		    {
			efile.seek(runlist.at(j).split(",").at(0).toUInt());
			indxallocbuf.append(efile.read(runlist.at(j).split(",").at(1).toUInt()));
		    }
		    efile.close();
		}
	    }
            else if(attrtype == 4294967295)
                break;
            curoffset += attrlength;
        }
	qDebug() << "indxallocbuf count:" << indxallocbuf.count();
	//uint32_t leroothdr = qFromLittleEndian<uint32_t>(indxrootbuf.left(4)); // uint 4 bytes (0-3)
	//qDebug() << "leroothdr:" << QString::number(leroothdr, 16); // should be 0x30
	// bytes 4-7 are collation sorting rule, which enforces filename sorting and not needed to parse.
	uint32_t indxrecordsize = qFromLittleEndian<uint32_t>(indxrootbuf.mid(8, 4)); // INDEX RECORD SIZE (BYTES)
	// byte 12 is index record cluster size
	// bytes 13-15 are always unused
	uint32_t indxentryoffset = qFromLittleEndian<uint32_t>(indxrootbuf.mid(16, 4));
	uint32_t indxentrylistsize = qFromLittleEndian<uint32_t>(indxrootbuf.mid(20, 4));
	uint32_t indxentrylistallocated = qFromLittleEndian<uint32_t>(indxrootbuf.mid(24, 4));
        //if((indexentrylistallocated - indexentrylistsize) > 0)
        //    isindxrootslack = true;
	uint8_t indxentrylistflags = indxrootbuf.at(28);
	qDebug() << "indxentrylistflags:" << QString::number(indxentrylistflags, 16);
	if(indxentrylistflags == 0x01) // $INDEX_ALLOC EXISTS
	{
	    int indxrecordcount = indxallocbuf.count() / indxrecordsize; // NUMBER OF INDEX RECORDS IN ALLOCATION
	    uint curpos = 0;
	    for(int i=0; i < indxrecordcount; i++)
	    {
		QString indxrecordheader = QString::fromStdString(indxallocbuf.mid(curpos, 4).toStdString());
		uint32_t indxentrystartoffset = qFromLittleEndian<uint32_t>(indxallocbuf.mid(curpos + 24, 4));
		uint32_t indxentryendoffset = qFromLittleEndian<uint32_t>(indxallocbuf.mid(curpos + 28, 4));
		uint32_t indxentryallocoffset = qFromLittleEndian<uint32_t>(indxallocbuf.mid(curpos + 32, 4));
		curpos = curpos + 24 + indxentrystartoffset;
		while(curpos < indxentryallocoffset)
		{
		    //qDebug() << "curpos at start of next index entry:" << curpos;
		    uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxallocbuf.mid(curpos + 8, 2));
		    uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxallocbuf.mid(curpos + 10, 2));
		    //qDebug() << "filenamelength:" << filenamelength << "indexentrylength:" << indxentrylength;
		    if(indxentrylength == 0 || filenamelength == 0)
		    {
			i = indxrecordcount;
			break;
		    }
		    QByteArray filenamebuf = indxallocbuf.mid(curpos + 16, filenamelength);
		    uint8_t fnametype = filenamebuf.at(65);
		    qDebug() << "fnametype:" << fnametype;
		    if(fnametype != 0x02)
		    {
		    qulonglong parentnode = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 6));
		    //qDebug() << "parentnode:" << parentnode << "pnode:" << pnode;
		    uint64_t parfileref = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 8));
		    uint64_t createdate = qFromLittleEndian<uint64_t>(filenamebuf.mid(8, 8));
		    uint64_t modifydate = qFromLittleEndian<uint64_t>(filenamebuf.mid(16, 8));
		    uint64_t changedate = qFromLittleEndian<uint64_t>(filenamebuf.mid(24, 8));
		    uint64_t accessdate = qFromLittleEndian<uint64_t>(filenamebuf.mid(32, 8));
		    uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenamebuf.mid(40, 8));
		    uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenamebuf.mid(48, 8));
		    uint32_t fileflags = qFromLittleEndian<uint32_t>(filenamebuf.mid(56, 4));
		    uint32_t filereparse = qFromLittleEndian<uint32_t>(filenamebuf.mid(60, 4));
		    uint8_t fnamelength = filenamebuf.at(64);
		    QString filename = "";
		    for(int j=0; j < fnamelength; j++)
			filename += QString(QChar(qFromLittleEndian<uint16_t>(filenamebuf.mid(66 + j*2, 2))));
		    qDebug() << "filename:" << filename;
		    //filename += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid((i+j)*32 + k*2, 2))));
                    if(curpos > indxentryendoffset)
			qDebug() << "deleted file/dir";
		    }
                    curpos = curpos + indxentrylength;
		}
	    }
	}
	else // 0x00 NO $INDEX_ALLOC
	{

            qDebug() << "no index allocation...";
            // bytes 29-31 are often blank, always unused. bytes 32 starts the 1st index entry...
            // byte 32 because header started at byte 16, and offset to 1st index entry was 16, 16+16=32
            uint curpos = 32;
            int a=1;
            //if((indexentrylistallocated - indexentrylistsize) > 0)
            //    isindxrootslack = true;
            //MIGHT WANT TO USE THE INDEXENTRYLISTALLOCATED INSTAED OF INDEXENTRYLISTSIZE AND THEN IMPLEMENT THE TWO ZERO CHECKS TO BREAK...
            //while(curpos < indexentrylistsize - 32) // LOOPS OVER THE INDEX ENTRIES
            while(curpos < indexentrylistallocated - 32) // LOOPS OVER THE INDEX ENTRIES
            {
                if(a % 2 == 0)
                    htmlstr += "<tr class=even>";
                else
                    htmlstr += "<tr class=odd>";
                //uint64_t indxentrymftref = qFromLittleEndian<uint64_t>(indxrootba.mid(curpos, 8));
                uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 8, 2));
                uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 10, 2));
                //uint16_t indxentryflags = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 12, 4));
                QByteArray filenameattribute = indxrootba.mid(curpos + 16, filenamelength);
                uint64_t createdtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(8, 8));
                uint64_t modifiedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(16, 8));
                uint64_t changedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(24, 8));
                uint64_t accessedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(32, 8));
                uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(40, 8));
                uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(48, 8));
                //uint64_t filenameflags = qFromLittleEndian<uint64_t>(filenameattribute.mid(56, 4));
                // 60-63 is reparse value
                uint8_t fnamelength = qFromLittleEndian<uint8_t>(filenameattribute.mid(64, 1));
                //uint8_t filenamespace = qFromLittleEndian<uint8_t>(filenameattribute.mid(65, 1));
                if(indxentrylength == 0 || filenamelength == 0)
                    break;
                QString filename = "";
                for(int i=0; i < fnamelength*2; i++)
                {
                    if(i % 2 == 0)
                        filename.append(filenameattribute.mid(66+i, 1));
                }
                htmlstr += "<td>" + filename + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(createdtime) + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(modifiedtime) + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(changedtime) + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(accessedtime) + "</td>";
                htmlstr += "<td>" + QString::number(logicalsize) + "</td>";
                htmlstr += "<td>" + QString::number(physicalsize) + "</td>";
                htmlstr += "<td>&nbsp;</td>";
                htmlstr += "</tr>";
                curpos = curpos + indxentrylength;
                //curpos = curpos + 16 + indxentrylength;
                a++;
            }
	}
	*/
    //}
    //qDebug() << "fs type str:" << fsinfo->value("typestr").toString();
    //qDebug() << "cur mft entry:" << curmftentry;


    QString htmlstr = "";
    QFile initfile(":/html/artifactprephtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    htmlstr += "<div id='infotitle'>$I30 File Analysis for " + i30name + " (" + i30id + ")</div><br/>";
    htmlstr += "<table><tr><th colspan=8>$I30 Entries</th></tr>";
    htmlstr += "<tr><td class=subhead>File Name</td><td class=subhead>Created</td><td class=subhead>Modified</td><td class=subhead>Changed</td><td class=subhead>Accessed</td><td class=subhead>Logical Size (bytes)</td><td class=subhead>Physical Size (bytes)</td><td class=subhead>Recovered</td></tr>";
    // uint8 = 1 byte - uchar
    // uint16 = 2 bytes - ushort
    // uint32 = 4 bytes - uint
    // uint64 = 8 bytes - ulonglong
    //bool isindxrootslack = false;
    QString indxrootstr = wombatvariable.tmpfilepath + i30id + "-fhex";
    QByteArray indxrootba;
    indxrootba.clear();
    QFile indxrootfile(indxrootstr);
    if(!indxrootfile.isOpen())
        indxrootfile.open(QIODevice::ReadOnly);
    if(indxrootfile.isOpen())
        indxrootba = indxrootfile.readAll();
    indxrootfile.close();
    QString evidid = i30id.split("-").first();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-*" + evidid)), QDir::NoSymLinks | QDir::Dirs);
    QFile fsfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + i30id.split("-").at(1) + "/prop");
    fsfile.open(QIODevice::ReadOnly | QIODevice::Text);
    uint maxmftentries = 0;
    if(fsfile.isOpen())
    {
        QString line = "";
        while(!fsfile.atEnd())
        {
            line = fsfile.readLine();
            if(line.startsWith("Max MFT Entries"))
            {
                maxmftentries = line.split("|").at(1).toUInt();
                break;
            }
        }
        fsfile.close();
    }
    uint32_t leroothdr = qFromLittleEndian<uint32_t>(indxrootba.left(4));
    uint32_t indxrecordsize = qFromLittleEndian<uint32_t>(indxrootba.mid(8, 4)); // INDEX RECORD SIZE (Bytes)
    if(leroothdr == 0x30)
    {
        uint8_t indxrootflags = indxrootba.at(28);
        if(indxrootflags == 0x01) // $INDEX_ALLOCATION EXISTS
        {
            QString tmpstr = "";
            QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
            evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
            if(evidfile.isOpen())
            {
                tmpstr = evidfile.readLine();
                evidfile.close();
            }
            QString layout = "";
            QFile indxpropfile(wombatvariable.tmpmntpath + evidfiles.first() + "/" + i30id.split("-").at(1) + "/" + i30id.split("-").at(2) + ".prop");
            indxpropfile.open(QIODevice::ReadOnly | QIODevice::Text);
            if(indxpropfile.isOpen())
            {
                QString line = "";
                while(!indxpropfile.atEnd())
                {
                    line = indxpropfile.readLine();
                    if(line.startsWith("MFT Record Layout"))
                    {
                        layout = line.split("|").at(1);
                        break;
                    }
                }
                indxpropfile.close();
            }
            QByteArray mftentry;
            mftentry.clear();
            QFile imgfile(tmpstr.split(",", Qt::SkipEmptyParts).at(1));
            imgfile.open(QIODevice::ReadOnly);
            if(imgfile.isOpen())
            {
                imgfile.seek(layout.split(";").at(0).split(",").at(0).toUInt());
                mftentry = imgfile.read(layout.split(";").at(0).split(",").at(1).toUInt());
                imgfile.close();
            }
        }
        else // NO $INDEX_ALLOCATION
        {
            uint32_t startoffset = qFromLittleEndian<uint32_t>(indxrootba.mid(16, 4));
            uint32_t endoffset = qFromLittleEndian<uint32_t>(indxrootba.mid(20, 4));
            uint32_t allocoffset = qFromLittleEndian<uint32_t>(indxrootba.mid(24, 4));
            uint curpos = 16 + startoffset;
            int a = 1;
            while(curpos < allocoffset)
            {
                uint64_t ntinode = qFromLittleEndian<uint64_t>(indxrootba.mid(curpos, 6));
                uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 8, 2));
                uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 10, 2));
                if(indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0)
                {
                    QByteArray filenamebuf = indxrootba.mid(curpos + 16, filenamelength);
                    uint8_t fnametype = filenamebuf.at(65);
                    if(fnametype != 0x02)
                    {
                        uint8_t fnamelength = filenamebuf.at(64);
                        QString filename = "";
                        for(int j=0; j < fnamelength; j++)
                            filename += QString(QChar(qFromLittleEndian<uint16_t>(filenamebuf.mid(66 + j*2, 2))));
                        if( a % 2 == 0)
                            htmlstr += "<tr class=even>";
                        else
                            htmlstr += "<tr class=odd>";
                        htmlstr += "<td>" + filename + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(8, 8))) + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(16, 8))) + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(24, 8))) + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(32, 8))) + "</td>";
                        htmlstr += "<td>" + QString::number(qFromLittleEndian<uint64_t>(filenamebuf.mid(40, 8))) + "</td>";
                        htmlstr += "<td>" + QString::number(qFromLittleEndian<uint64_t>(filenamebuf.mid(48, 8))) + "</td>";
                        htmlstr += "<td>&nbsp;</td>";
                        htmlstr += "</tr>";
                    }
                    curpos = curpos + indxentrylength;
                }
                else
                    curpos = curpos + 4;
            }
            /* 
        while(curpos < allocoffset)
        {
	    if(indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0)
            {
                QByteArray filenamebuf = indxroot.mid(curpos + 16, filenamelength);
                uint8_t fnametype = filenamebuf.at(65);
                if(fnametype != 0x02)
                {
                    uint8_t fnamelength = filenamebuf.at(64);
                    QString filename = "";
                    for(int j=0; j < fnamelength; j++)
                        filename += QString(QChar(qFromLittleEndian<uint16_t>(filenamebuf.mid(66 + j*2, 2))));
                    if(filename != ".")
                    {
	                fileinfo.insert("i30sequenceid", QVariant(qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 6, 2)))); // sequence number for entry
                        uint64_t parntinode = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 6)); // parent nt inode for the entry
                        fileinfo.insert("i30parentsequenceid", QVariant(qFromLittleEndian<uint16_t>(filenamebuf.mid(6, 2)))); // parent sequence number for entry
                        fileinfo.insert("ntinode", QVariant((qulonglong)ntinode)); // current nt inode
                        fileinfo.insert("parntinode", QVariant((qulonglong)parntinode)); // current parent nt inode
                        fileinfo.insert("i30create", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(8, 8)))));
                        fileinfo.insert("i30modify", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(16, 8)))));
                        fileinfo.insert("i30change", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(24, 8)))));
                        fileinfo.insert("i30access", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(32, 8)))));
                        //uint64_t parfileref = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 8));
                        //uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenamebuf.mid(40, 8));
                        //uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenamebuf.mid(48, 8));
                        //uint32_t fileflags = qFromLittleEndian<uint32_t>(filenamebuf.mid(56, 4));
                        //uint32_t filereparse = qFromLittleEndian<uint32_t>(filenamebuf.mid(60, 4));
                        QList<QHash<QString, QVariant>> adsinfolist;
                        adsinfolist.clear();
                        fileinfo.insert("filename", QVariant(filename));
                        fileinfo.insert("inode", QVariant(curinode));
                        //qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                        GetMftEntryContent(estring, ntinode, &fileinfo, fsinfo, &adsinfolist);
                        uint16_t attrflags = fileinfo.value("attrflags").toUInt();
                        uint32_t accessflags = fileinfo.value("accessflags").toUInt();
                        if(curpos > endoffset)
                        {
                            //qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                            if(attrflags == 0x00) // unalloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(4));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                            else if(attrflags == 0x02) // unalloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(2));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                            else
                            {
                                fileinfo.insert("itemtype", QVariant(4));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                        }
                        else if(parfileinfo != NULL)
                        {
                            if(parfileinfo->value("ntinode").toUInt() != parntinode)
                            {
                                //qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                                if(attrflags == 0x00) // unalloc file
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(4));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                                else if(attrflags == 0x02) // unalloc dir
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(2));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                                else
                                {
                                    fileinfo.insert("itemtype", QVariant(4));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                            }
                            else
                            {
                                if(attrflags == 0x01) //alloc file
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(5));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                                else if(attrflags == 0x03) // alloc dir
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(3));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                                else
                                {
                                    fileinfo.insert("itemtype", QVariant(5));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                            }
                        }
                        else
                        {
                            if(attrflags == 0x01) // alloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(5));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                            else if(attrflags == 0x03) // alloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(3));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                            else
                            {
                                fileinfo.insert("itemtype", QVariant(5));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                        }
                        //qDebug() << "filename:" << fileinfo.value("filename").toString() << "itemtype:" << fileinfo.value("itemtype").toUInt();
                        if(parfileinfo == NULL)
                        {
                            fileinfo.insert("path", QVariant("/"));
                            fileinfo.insert("parentinode", QVariant(-1));
                        }
                        else
                        {
                            fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString() + parfileinfo->value("filename").toString() + "/")));
                            fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toULongLong()));
                        }
                        fileinfolist->append(fileinfo);
                        curinode++;
                        if(fileinfo.value("itemtype").toUInt() == 2 || fileinfo.value("itemtype").toUInt() == 3) // directory
                        {
                            ParseNtfsDirectory(estring, fsinfo, fileinfolist, orphanlist, &fileinfo, ntinode, curinode); // should be able to get rid of mftentries...
                            curinode = fileinfolist->count();
                        }
                        if(adsinfolist.count() > 0)
                        {
                            for(int j=0; j < adsinfolist.count(); j++)
                            {
                                QHash<QString, QVariant> curadsinfo = adsinfolist.at(j);
                                curadsinfo.insert("inode", QVariant(curinode));
                                fileinfolist->append(curadsinfo);
                                curinode++;
                            }
                        }
                        /*
                        if(curpos > indxentrylistsize)
                        {
                            qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                            qDebug() << "deleted file/dir";
                        }
                        */
                    /*}
                }
                curpos = curpos + indxentrylength;
            }
            else
                curpos = curpos + 4;
        }

             */ 
        }
    }

        /*
        // bytes 4-7 are collation sorting rule, which enforces filename sorting and not needed to parse.
	uint32_t indxrecordsize = qFromLittleEndian<uint32_t>(indxrootba.mid(8, 4)); // INDEX RECORD SIZE
        //uint8_t indxrecordclustersize = qFromLittleEndian<uint8_t>(indxrootba.mid(12, 1));
        // bytes 13-15 are often blank, always unused.
        //uint32_t indexentryoffset = qFromLittleEndian<uint32_t>(indxrootba.mid(16, 4)); // INDEX NODE HEADER
        //uint32_t indexentrylistsize = qFromLittleEndian<uint32_t>(indxrootba.mid(20, 4)); // USED SIZE FOR ALL INDEX ENTRIES
        uint32_t indexentrylistallocated = qFromLittleEndian<uint32_t>(indxrootba.mid(24, 4)); // ALLOC SIZE FOR ALL INDEX ENTRIES
        uint8_t indxentrylistflags = qFromLittleEndian<uint8_t>(indxrootba.mid(28, 1)); // WHETHER INDEX ALLOCATION EXISTS
        //if((indexentrylistallocated - indexentrylistsize) > 0)
        //    isindxrootslack = true;
        if(indxentrylistflags == 0x01) // $INDEX_ALLOCATION EXISTS
        {
            QByteArray indxrootcontent;
            QByteArray indxalloccontent;
            //QByteArray bitmapcontent;
            indxrootcontent.clear();
            indxalloccontent.clear();
            //bitmapcontent.clear();
	    qDebug() << "tmpmntpath:" << wombatvariable.tmpmntpath;
	    // GET CORRECT EVIDENCE DIRECTORY
	    QString evidid = i30id.split("-").first();
	    QDir eviddir = QDir(wombatvariable.tmpmntpath);
	    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-*" + evidid)), QDir::NoSymLinks | QDir::Dirs);
	    qDebug() << "evidfile:" << evidfiles.at(0);
            qDebug() << "propfile:" << QString(i30id + ".prop");
	    /*
	    QString evidid = nodeid.split("-").first();
	    QDir eviddir = QDir(wombatvariable.tmpmntpath);
	    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-*" + evidid)), QDir::NoSymLinks | QDir::Dirs);
	    QString evidname = evidfiles.first().split(QString("-" + evidid)).first();
	    QString tmpstr = "";
	    QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
	    evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
	    if(evidfile.isOpen())
		tmpstr = evidfile.readLine(); // original evidence filename, evidence mount string, imgsize, id
	    evidfile.close();
	    */

            /*
            TSK_IMG_INFO* imginfo = NULL;
            std::vector<std::string> pathvector;
            pathvector.clear();
            QDir eviddir = QDir(wombatvariable.tmpmntpath);
            QString tmpstr = "";
            QStringList evidfiles = eviddir.entryList(QStringList("*." + i30id.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
            QString evidencename = evidfiles.at(0).split(".e").first();
            QString estring = i30id.split("-", Qt::SkipEmptyParts).at(0);
            QString vstring = i30id.split("-", Qt::SkipEmptyParts).at(1);
            QString pstring = i30id.split("-", Qt::SkipEmptyParts).at(2);
            QString fstring = i30id.split("-", Qt::SkipEmptyParts).at(3);
            QString astring = i30id.split("-", Qt::SkipEmptyParts).at(4);
            qint64 curaddr = 0;
            if(fstring.contains("a") || fstring.contains("d") || fstring.contains("o") || fstring.contains("z"))
                curaddr = astring.mid(2).toLongLong();
            else
                curaddr = fstring.mid(1).toLongLong();
            QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
            if(!evidfile.isOpen())
                evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
            if(evidfile.isOpen())
                tmpstr = evidfile.readLine();
            evidfile.close();
            pathvector.push_back(tmpstr.split(",").at(3).split("|").at(0).toStdString());
            const TSK_TCHAR** images;
            images = (const char**)malloc(pathvector.size()*sizeof(char*));
            images[0] = pathvector[0].c_str();
            int partcount = tmpstr.split(",").at(3).split("|").size();
            imginfo = tsk_img_open(partcount, images, TSK_IMG_TYPE_DETECT, 0);
            if(imginfo != NULL)
            {
                QStringList partlist;
                tmpstr = "";
                partlist.clear();
                QFile partfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/" + vstring + "/" + pstring + "/stat");
                if(!partfile.isOpen())
                    partfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(partfile.isOpen())
                    tmpstr = partfile.readLine();
                partlist = tmpstr.split(",");
                tmpstr = "";
                int fstype = partlist.at(0).toInt();
                TSK_FS_INFO* fsinfo = NULL;
                if(fstype == TSK_FS_TYPE_NTFS_DETECT)
                {
                    fsinfo = tsk_fs_open_img(imginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
                    if(fsinfo != NULL)
                    {
                        TSK_FS_FILE* fsfile = NULL;
                        fsfile = tsk_fs_file_open_meta(fsinfo, NULL, curaddr);
                        if(fsfile != NULL)
                        {
                            if(fsfile->meta != NULL)
                            {
                                int attrcnt = tsk_fs_file_attr_getsize(fsfile);
                                char* fscontent = new char[0];
                                ssize_t fslength = 0;
                                for(int i=0; i < attrcnt; i++)
                                {
                                    const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(fsfile, i);
                                    /*
                                    if(fsattr->type == 144) // $INDEX_ROOT
                                    {
                                        if(isindxrootslack)
                                        {
                                            fscontent = new char[fsattr->size];
                                            fslength = tsk_fs_attr_read(fsattr, 0, fscontent, fsattr->size, TSK_FS_FILE_READ_FLAG_SLACK);
                                            indxrootcontent.append(QByteArray(fscontent, fslength));
                                        }
                                    }
                                    */
            /*
                                    if(fsattr->type == 160) // $INDEX_ALLOCATION
                                    {
                                        fscontent = new char[fsattr->size];
                                        fslength = tsk_fs_attr_read(fsattr, 0, fscontent, fsattr->size, TSK_FS_FILE_READ_FLAG_SLACK);
                                        indxalloccontent.append(QByteArray(fscontent, fslength));
                                    }
                                }
                                delete[] fscontent;
                            }
                        }
                        tsk_fs_file_close(fsfile);
                        fsfile = NULL;
                    }
                    tsk_fs_close(fsinfo);
                    fsinfo = NULL;
                }
            }
            tsk_img_close(imginfo);
            imginfo = NULL;
            */
        /*
            // CONTENT FOR ROOT SLACK, INDEXALLOC AND BITMAP HAS BEEN OBTAINED, SO GO BACK TO PROCESSING INDEX ENTRIES NOW...
            int indxrecordcnt = indxalloccontent.count() / indxrecordsize; // NUMBER OF INDEX RECORDS IN ALLOCATION
            uint curpos = 0;
            for(int i=0; i < indxrecordcnt; i++) // loop over index records
            {
                QString indxrecordheader = QString::fromStdString(indxalloccontent.mid(curpos, 4).toStdString());
                if(indxrecordheader.contains("INDX")) // moving correctly.
                {
                    //uint16_t indxrecordfixupoffset = qFromLittleEndian<uint16_t>(indxalloccontent.mid(curpos + 4, 2));
                    //uint16_t indxrecordfixupcount = qFromLittleEndian<uint16_t>(indxalloccontent.mid(curpos + 6, 2));
                    // Bytes 8-15 provide $Logfile sequence number which i don't need for this.
                    //uint64_t indxrecordvcn = qFromLittleEndian<uint64_t>(indxalloccontent.mid(curpos + 16, 8));
                    uint32_t indxentrystartoffset = qFromLittleEndian<uint32_t>(indxalloccontent.mid(curpos + 24, 4));
                    uint32_t indxentryendoffset = qFromLittleEndian<uint32_t>(indxalloccontent.mid(curpos + 28, 4));
                    uint32_t indxentryallocoffset = qFromLittleEndian<uint32_t>(indxalloccontent.mid(curpos + 32, 4));
                    // byte 36-40 are flags that i probably don't use...
                    // SET CURPOS TO THE START OF THE 1ST INDEX ENTRY.. INDXENTRYSTARTOFFSET + 24 (START OF THE NODE HEADER)
                    curpos = curpos + 24 + indxentrystartoffset;
                    //curpos = 80;
                    int a = 1;
                    while(curpos < indxentryallocoffset) // get alloc and deleted, will need to tell apart maybe curpos > endoffset, the deleted...
                    {
                        if( a % 2 == 0)
                            htmlstr += "<tr class=even>";
                        else
                            htmlstr += "<tr class=odd>";
                        uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxalloccontent.mid(curpos + 8, 2));
                        uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxalloccontent.mid(curpos + 10, 2));
                        // 12-15 are flags and can be ignored for now...
                        if(indxentrylength == 0 || filenamelength == 0)
                            break;
                        QByteArray filenameattribute = indxalloccontent.mid(curpos + 16, filenamelength);
                        uint64_t createdtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(8, 8));
                        uint64_t modifiedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(16, 8));
                        uint64_t changedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(24, 8));
                        uint64_t accessedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(32, 8));
                        uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(40, 8));
                        uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(48, 8));
                        uint8_t fnamelength = qFromLittleEndian<uint8_t>(filenameattribute.mid(64, 1));
                        QString filename = "";
                        for(int i=0; i < fnamelength*2; i++)
                        {
                            if(i % 2 == 0)
                                filename.append(filenameattribute.mid(66+i, 1));
                        }
                        htmlstr += "<td>" + filename + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(createdtime) + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(modifiedtime) + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(changedtime) + "</td>";
                        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(accessedtime) + "</td>";
                        htmlstr += "<td>" + QString::number(logicalsize) + "</td>";
                        htmlstr += "<td>" + QString::number(physicalsize) + "</td>";
                        if(curpos > indxentryendoffset)
                            htmlstr += "<td>TRUE</td>";
                        else
                            htmlstr += "<td>&nbsp;</td>";
                        htmlstr += "</tr>";
                        curpos = curpos + indxentrylength;
                        a++;
                    }
                }
            }
         }
        else // 0x00 NO $INDEX_ALLOCATION
        {
            /*
        uint32_t startoffset = qFromLittleEndian<uint32_t>(indxroot.mid(16, 4));
        uint32_t endoffset = qFromLittleEndian<uint32_t>(indxroot.mid(20, 4));
        uint32_t allocoffset = qFromLittleEndian<uint32_t>(indxroot.mid(24, 4));
        uint curpos = 16 + startoffset;
        //uint curpos = 32;
        //if(indxentrylistalloc - indxentrylistsize > 0)
        //    isindxrootslack = true;
        while(curpos < allocoffset)
        {
            uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 8, 2));
            uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 10, 2));
            uint64_t ntinode = qFromLittleEndian<uint64_t>(indxroot.mid(curpos, 6)); 
	    if(indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0)
            {
                QByteArray filenamebuf = indxroot.mid(curpos + 16, filenamelength);
                uint8_t fnametype = filenamebuf.at(65);
                if(fnametype != 0x02)
                {
                    uint8_t fnamelength = filenamebuf.at(64);
                    QString filename = "";
                    for(int j=0; j < fnamelength; j++)
                        filename += QString(QChar(qFromLittleEndian<uint16_t>(filenamebuf.mid(66 + j*2, 2))));
                    if(filename != ".")
                    {
	                fileinfo.insert("i30sequenceid", QVariant(qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 6, 2)))); // sequence number for entry
                        uint64_t parntinode = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 6)); // parent nt inode for the entry
                        fileinfo.insert("i30parentsequenceid", QVariant(qFromLittleEndian<uint16_t>(filenamebuf.mid(6, 2)))); // parent sequence number for entry
                        fileinfo.insert("ntinode", QVariant((qulonglong)ntinode)); // current nt inode
                        fileinfo.insert("parntinode", QVariant((qulonglong)parntinode)); // current parent nt inode
                        fileinfo.insert("i30create", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(8, 8)))));
                        fileinfo.insert("i30modify", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(16, 8)))));
                        fileinfo.insert("i30change", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(24, 8)))));
                        fileinfo.insert("i30access", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(32, 8)))));
                        //uint64_t parfileref = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 8));
                        //uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenamebuf.mid(40, 8));
                        //uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenamebuf.mid(48, 8));
                        //uint32_t fileflags = qFromLittleEndian<uint32_t>(filenamebuf.mid(56, 4));
                        //uint32_t filereparse = qFromLittleEndian<uint32_t>(filenamebuf.mid(60, 4));
                        QList<QHash<QString, QVariant>> adsinfolist;
                        adsinfolist.clear();
                        fileinfo.insert("filename", QVariant(filename));
                        fileinfo.insert("inode", QVariant(curinode));
                        //qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                        GetMftEntryContent(estring, ntinode, &fileinfo, fsinfo, &adsinfolist);
                        uint16_t attrflags = fileinfo.value("attrflags").toUInt();
                        uint32_t accessflags = fileinfo.value("accessflags").toUInt();
                        if(curpos > endoffset)
                        {
                            //qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                            if(attrflags == 0x00) // unalloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(4));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                            else if(attrflags == 0x02) // unalloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(2));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                            else
                            {
                                fileinfo.insert("itemtype", QVariant(4));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                        }
                        else if(parfileinfo != NULL)
                        {
                            if(parfileinfo->value("ntinode").toUInt() != parntinode)
                            {
                                //qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                                if(attrflags == 0x00) // unalloc file
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(4));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                                else if(attrflags == 0x02) // unalloc dir
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(2));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                                else
                                {
                                    fileinfo.insert("itemtype", QVariant(4));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                            }
                            else
                            {
                                if(attrflags == 0x01) //alloc file
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(5));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                                else if(attrflags == 0x03) // alloc dir
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(3));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                                else
                                {
                                    fileinfo.insert("itemtype", QVariant(5));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                            }
                        }
                        else
                        {
                            if(attrflags == 0x01) // alloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(5));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                            else if(attrflags == 0x03) // alloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(3));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                            else
                            {
                                fileinfo.insert("itemtype", QVariant(5));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                        }
                        //qDebug() << "filename:" << fileinfo.value("filename").toString() << "itemtype:" << fileinfo.value("itemtype").toUInt();
                        if(parfileinfo == NULL)
                        {
                            fileinfo.insert("path", QVariant("/"));
                            fileinfo.insert("parentinode", QVariant(-1));
                        }
                        else
                        {
                            fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString() + parfileinfo->value("filename").toString() + "/")));
                            fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toULongLong()));
                        }
                        fileinfolist->append(fileinfo);
                        curinode++;
                        if(fileinfo.value("itemtype").toUInt() == 2 || fileinfo.value("itemtype").toUInt() == 3) // directory
                        {
                            ParseNtfsDirectory(estring, fsinfo, fileinfolist, orphanlist, &fileinfo, ntinode, curinode); // should be able to get rid of mftentries...
                            curinode = fileinfolist->count();
                        }
                        if(adsinfolist.count() > 0)
                        {
                            for(int j=0; j < adsinfolist.count(); j++)
                            {
                                QHash<QString, QVariant> curadsinfo = adsinfolist.at(j);
                                curadsinfo.insert("inode", QVariant(curinode));
                                fileinfolist->append(curadsinfo);
                                curinode++;
                            }
                        }
                        /*
                        if(curpos > indxentrylistsize)
                        {
                            qDebug() << "filename:" << filename << "nt inode:" << ntinode << "parent nt node:" << parntinode;
                            qDebug() << "deleted file/dir";
                        }
                        */
          /*          }
                }
                curpos = curpos + indxentrylength;
            }
            else
                curpos = curpos + 4;
        }

             */
            /*
            qDebug() << "no index allocation...";
	    qDebug() << "tmpmntpath:" << wombatvariable.tmpmntpath;
            // bytes 29-31 are often blank, always unused. bytes 32 starts the 1st index entry...
            // byte 32 because header started at byte 16, and offset to 1st index entry was 16, 16+16=32
            uint curpos = 32;
            int a=1;
            //if((indexentrylistallocated - indexentrylistsize) > 0)
            //    isindxrootslack = true;
            //MIGHT WANT TO USE THE INDEXENTRYLISTALLOCATED INSTAED OF INDEXENTRYLISTSIZE AND THEN IMPLEMENT THE TWO ZERO CHECKS TO BREAK...
            //while(curpos < indexentrylistsize - 32) // LOOPS OVER THE INDEX ENTRIES
            while(curpos < indexentrylistallocated - 32) // LOOPS OVER THE INDEX ENTRIES
            {
                if(a % 2 == 0)
                    htmlstr += "<tr class=even>";
                else
                    htmlstr += "<tr class=odd>";
                //uint64_t indxentrymftref = qFromLittleEndian<uint64_t>(indxrootba.mid(curpos, 8));
                uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 8, 2));
                uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 10, 2));
                //uint16_t indxentryflags = qFromLittleEndian<uint16_t>(indxrootba.mid(curpos + 12, 4));
                QByteArray filenameattribute = indxrootba.mid(curpos + 16, filenamelength);
                uint64_t createdtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(8, 8));
                uint64_t modifiedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(16, 8));
                uint64_t changedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(24, 8));
                uint64_t accessedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(32, 8));
                uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(40, 8));
                uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(48, 8));
                //uint64_t filenameflags = qFromLittleEndian<uint64_t>(filenameattribute.mid(56, 4));
                // 60-63 is reparse value
                uint8_t fnamelength = qFromLittleEndian<uint8_t>(filenameattribute.mid(64, 1));
                //uint8_t filenamespace = qFromLittleEndian<uint8_t>(filenameattribute.mid(65, 1));
                if(indxentrylength == 0 || filenamelength == 0)
                    break;
                QString filename = "";
                for(int i=0; i < fnamelength*2; i++)
                {
                    if(i % 2 == 0)
                        filename.append(filenameattribute.mid(66+i, 1));
                }
                htmlstr += "<td>" + filename + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(createdtime) + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(modifiedtime) + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(changedtime) + "</td>";
                htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(accessedtime) + "</td>";
                htmlstr += "<td>" + QString::number(logicalsize) + "</td>";
                htmlstr += "<td>" + QString::number(physicalsize) + "</td>";
                htmlstr += "<td>&nbsp;</td>";
                htmlstr += "</tr>";
                curpos = curpos + indxentrylength;
                //curpos = curpos + 16 + indxentrylength;
                a++;
            }
            /*
            uint64_t indxentrymftref = qFromLittleEndian<uint64_t>(indxrootba.mid(32, 8));
            qDebug() << "$MFT file reference:" << indxentrymftref;
            uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxrootba.mid(40, 2));
            qDebug() << "indx entry length:" << indxentrylength;
            uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxrootba.mid(42, 2));
            qDebug() << "$FILE_NAME attribute length:" << filenamelength;
            uint16_t indxentryflags = qFromLittleEndian<uint16_t>(indxrootba.mid(44, 4));
            qDebug() << "indxentryflags (0, 1, 2):" << QString::number(indxentryflags, 16);
            // 1st filename attribute 0x48
            QByteArray filenameattribute = indxrootba.mid(48, filenamelength);
            qDebug() << "filenameattribute size:" << filenameattribute.count();
            uint64_t createdtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(8, 8));
            qDebug() << "createdtime:" << createdtime << ConvertWindowsTimeToUnixTime(createdtime);
            uint64_t modifiedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(16, 8));
            uint64_t changedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(24, 8));
            uint64_t accessedtime = qFromLittleEndian<uint64_t>(filenameattribute.mid(32, 8));
            uint64_t logicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(40, 8));
            uint64_t physicalsize = qFromLittleEndian<uint64_t>(filenameattribute.mid(48, 8));
            uint64_t filenameflags = qFromLittleEndian<uint64_t>(filenameattribute.mid(56, 8));
            uint8_t fnamelength = qFromLittleEndian<uint8_t>(filenameattribute.mid(64, 1));
            qDebug() << "fnamelength:" << fnamelength;
            uint8_t filenamespace = qFromLittleEndian<uint8_t>(filenameattribute.mid(65, 1));
            QString filename = "";
            for(int i=0; i < fnamelength*2; i++)
            {
                if(i % 2 == 0)
                    filename.append(filenameattribute.mid(66+i, 1));
            }
            qDebug() << "filename:" << filename << "end of indexentry:" << 66 + fnamelength*2 << "indxroot size:" << indxrootba.count();
            */

            /*
             *
                uint8_t parentreference[6];         // 0x00 parent directory $MFT Record number
                uint8_t parentsequencenumber[2];    // 0x06 parent directory sequence number
                uint8_t createdtime[8];             // 0x08 created (birth) time
                uint8_t modifiedtime[8];            // 0x10 last written time
                uint8_t changedtime[8];             // 0x18 record modified
                uint8_t accessedtime[8];            // 0x20 last accessed time
                uint8_t logicalsize[8];             // 0x28 logical size
                uint8_t physicalsize[8];            // 0x30 physical size
                uint8_t flags[8];                   // 0x38 flags (read only, hidden, etc)
                uint8_t filenamelength;             // 0x40 file name length
                uint8_t filenamenamespace;          // 0x41 namespace
                uint8_t filename;                   // 0x42 file name variable based on 0x40
	        uint8_t workdir[tmpsize];
	        liblnk_file_get_utf8_working_directory(lnkobj, workdir, tmpsize, &error);
	        htmlstr += "<tr class='even'><td class='aval'>Working Directory:</td><td>" + QString::fromUtf8(reinterpret_cast<char*>(workdir)) + "</td></tr>";
             */ 
        //}
    //}
    htmlstr += "</table></body></html>";
    
    return htmlstr;
}

QString ParseInfo2Artifact(QString info2name, QString info2id)
{
    QString htmlstr = "";
    QFile initfile(":/html/artifactprephtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    htmlstr += "<div id='infotitle'>INFO2 File Analysis for " + info2name + " (" + info2id + ")</div><br/>";
    htmlstr += "<table width='100%'><tr><th>FILE NAME</th><th>DELETED</th></tr>";
    QString info2file = wombatvariable.tmpfilepath + info2id + "-fhex";
    QByteArray info2content;
    info2content.clear();
    QFile i2file(info2file);
    if(!i2file.isOpen())
        i2file.open(QIODevice::ReadOnly);
    if(i2file.isOpen())
        info2content = i2file.readAll();
    i2file.close();
    uint32_t fileentrysize = qFromLittleEndian<uint32_t>(info2content.mid(12, 4));
    int curpos = 20; // content starts after offset
    int a = 1;
    while(curpos < info2content.count())
    {
        if(a % 2 == 0)
            htmlstr += "<tr class=even>";
        else
            htmlstr += "<tr class=odd>";
	QString filenamestring = QString::fromStdString(QByteArray(info2content.mid(curpos + 3, 260).toStdString().c_str(), -1).toStdString());
        uint64_t deleteddate = qFromLittleEndian<uint64_t>(info2content.mid(curpos + 268, 8));
        htmlstr += "<td>" + filenamestring + "</td>";
        htmlstr += "<td>" + ConvertWindowsTimeToUnixTime(deleteddate) + "</td>";
        htmlstr += "</tr>";
        curpos = curpos + fileentrysize;
    }

    htmlstr += "</table></body></html>";
    
    return htmlstr;
}

QString ParseIDollarArtifact(QString idollarname, QString idollarid)
{
    QString htmlstr = "";
    QFile initfile(":/html/artifactprephtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    htmlstr += "<div id='infotitle'>$I File Analysis for " + idollarname + " (" + idollarid + ")</div><br/>";
    htmlstr += "<table width='100%'><tr><th>NAME</th><th>Value</th></tr>";
    QString idollarfilestr = wombatvariable.tmpfilepath + idollarid + "-fhex";
    QByteArray idollarcontent;
    idollarcontent.clear();
    QFile idollarfile(idollarfilestr);
    if(!idollarfile.isOpen())
        idollarfile.open(QIODevice::ReadOnly);
    if(idollarfile.isOpen())
        idollarcontent = idollarfile.readAll();
    idollarfile.close();
    uint64_t versionformat = qFromLittleEndian<uint64_t>(idollarcontent.left(8));
    uint64_t filesize = qFromLittleEndian<uint64_t>(idollarcontent.mid(8, 8));
    uint64_t deleteddate = qFromLittleEndian<uint64_t>(idollarcontent.mid(16, 8));
    QString filenamestring = "";
    if(versionformat == 0x01)
    {
        filenamestring = QString::fromStdString(idollarcontent.mid(24, 520).trimmed().toStdString());
    }
    else if(versionformat == 0x02)
    {
        uint32_t filenamesize = qFromLittleEndian<uint32_t>(idollarcontent.mid(24, 4));
        filenamestring = QString::fromStdString(idollarcontent.mid(24, filenamesize).trimmed().toStdString());
    }
    htmlstr += "<tr class=odd><td class=aval>File Name:</td><td>" + filenamestring + "</td></tr>";
    htmlstr += "<tr class=even><td class=aval>Deleted:</td><td>" + ConvertWindowsTimeToUnixTime(deleteddate) + "</td></tr>";
    htmlstr += "<tr class=odd><td class=aval>File Size:</td><td>" + QString::number(filesize) + " bytes</td></tr>";

    htmlstr += "</table></body></html>";
    
    return htmlstr;
}

QString ParsePrefetchArtifact(QString pfname, QString pfid)
{
    QString htmlstr = "";
    QFile initfile(":/html/artifactprephtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    htmlstr += "<div id='infotitle'>Prefetch File Analysis for " + pfname + " (" + pfid + ")</div><br/>";
    htmlstr += "<table width='100%'><tr><th>NAME</th><th>Value</th></tr>";
    QString pffilestr = wombatvariable.tmpfilepath + pfid + "-fhex";
    QByteArray pfcontent;
    pfcontent.clear();
    QFile pffile(pffilestr);
    if(!pffile.isOpen())
        pffile.open(QIODevice::ReadOnly);
    if(pffile.isOpen())
        pfcontent = pffile.readAll();
    pffile.close();
    uint32_t tmpuint32 = 0;
    QString pfheader = QString::fromStdString(pfcontent.left(4).toStdString());
    if(pfheader.startsWith("MAM")) // WIN10
    {
        uint32_t datasize = qFromLittleEndian<uint32_t>(pfcontent.mid(4, 4));
        size_t compressedsize = pfcontent.count() - 8;
        size_t uncompressedsize = datasize;
        libfwnt_error_t* fwnterror = NULL;
        uint8_t compresseddata[compressedsize];
        uint8_t uncompresseddata[uncompressedsize];
        for(uint i=0; i < compressedsize; i++)
            compresseddata[i] = pfcontent.at(8+i);
	// libfwnt decompresses the data, and then i can just store that as the new pfcontent and then run the below to display it... 
        libfwnt_lzxpress_huffman_decompress(compresseddata, compressedsize, uncompresseddata, &uncompressedsize, &fwnterror);
	pfcontent.clear();
	for(uint i=0; i < uncompressedsize; i++)
	    pfcontent.append(uncompresseddata[i]);
    }
    pfheader = QString::fromStdString(pfcontent.mid(4, 4).toStdString());
    if(pfheader.contains("SCCA"))
    {
	uint32_t pfversion = qFromLittleEndian<uint32_t>(pfcontent.left(4));
	htmlstr += "<tr class=odd><td class=aval>Format Version:</td><td>" + QString::number(pfversion) + "</td></tr>";
	QString filenamestring = "";
	for(int i=16; i < 76; i++)
	{
	    if(i % 2 == 0)
	    {
		if(pfcontent.at(i) == '\u0000')
		    break;
		filenamestring.append(pfcontent.at(i));
	    }
	}
	htmlstr += "<tr class=even><td class=aval>Executable File Name:</td><td>" + filenamestring + "</td></tr>";
	tmpuint32 = qFromLittleEndian<uint32_t>(pfcontent.mid(76, 4));
	htmlstr += "<tr class=odd><td class=aval>Prefetch Hash:</td><td>0x" + QString::number(tmpuint32, 16) + "</td></tr>";
	tmpuint32 = 0;
	uint32_t fnamestringsoffset = 0;
	uint32_t fnamestringssize = 0;
	uint32_t volinfooffset = 0;
	uint32_t volinfosize = 0;
	uint32_t volinfocount = 0;
	uint32_t metricsoffset = 0;
	QByteArray fileinformation;
	QByteArray filenamestrings;
	QByteArray volinfocontent;
	fileinformation.clear();
	metricsoffset = qFromLittleEndian<uint32_t>(pfcontent.mid(84, 4));
	if(pfversion == 17) // WINXP, WIN2003
	    fileinformation = pfcontent.mid(84, 68);
	else if(pfversion == 23) // VISTA, WIN7
	    fileinformation = pfcontent.mid(84, 156);
	else if(pfversion == 26 || (pfversion == 30 && metricsoffset == 0x130)) // WIN8.1/WIN10
	    fileinformation = pfcontent.mid(84, 224);
	else if(pfversion == 30 && metricsoffset == 0x128) // WIN10
	    fileinformation = pfcontent.mid(84, 216);
	fnamestringsoffset = qFromLittleEndian<uint32_t>(fileinformation.mid(16, 4));
	fnamestringssize = qFromLittleEndian<uint32_t>(fileinformation.mid(20, 4));
	volinfooffset = qFromLittleEndian<uint32_t>(fileinformation.mid(24, 4));
	volinfocount = qFromLittleEndian<uint32_t>(fileinformation.mid(28, 4));
	volinfosize = qFromLittleEndian<uint32_t>(fileinformation.mid(32, 4));
	if(pfversion == 17)
	    htmlstr += "<tr class=even><td class=aval>Run Count:</td><td>" + QString::number(qFromLittleEndian<uint32_t>(fileinformation.mid(60, 4))) + "</td></tr>";
	else if(pfversion == 23)
	    htmlstr += "<tr class=even><td class=aval>Run Count:</td><td>" + QString::number(qFromLittleEndian<uint32_t>(fileinformation.mid(68, 4))) + "</td></tr>";
	else if(pfversion == 26 || (pfversion == 30 && metricsoffset == 0x130))
	    htmlstr += "<tr class=even><td class=aval>Run Count:</td><td>" + QString::number(qFromLittleEndian<uint32_t>(fileinformation.mid(124, 4))) + "</td></tr>";
	else if(pfversion == 30 && metricsoffset == 0x128)
	    htmlstr += "<tr class=even><td class=aval>Run Count:</td><td>" + QString::number(qFromLittleEndian<uint32_t>(fileinformation.mid(116, 4))) + "</td></tr>";
	if(pfversion == 17)
	    htmlstr += "<tr class=odd><td class=aval>Last Run Time:</td><td>" + ConvertWindowsTimeToUnixTime(qFromLittleEndian<uint64_t>(fileinformation.mid(36, 8))) + "</td></tr>";
	else if(pfversion == 23)
	    htmlstr += "<tr class=odd><td class=aval>Last Run Time:</td><td>" + ConvertWindowsTimeToUnixTime(qFromLittleEndian<uint64_t>(fileinformation.mid(44, 8))) + "</td></tr>";
	else if(pfversion == 26 || pfversion == 30)
	{
	    for(int i=0; i < 8; i++)
	    {
		uint64_t lastruntime = qFromLittleEndian<uint64_t>(fileinformation.mid(44+(i*8), 8));
		if(lastruntime == 0)
		    htmlstr += "<tr class=odd><td class=aval>Last Run Time" + QString::number(i+1) + "</td><td>Not Set (0)</td></tr>";
		else
		    htmlstr += "<tr class=odd><td class=aval>Last Run Time" + QString::number(i+1) + "</td><td>" + ConvertWindowsTimeToUnixTime(lastruntime) + "</td></tr>";
	    }
	}
	filenamestrings.clear();
	filenamestrings = pfcontent.mid(fnamestringsoffset, fnamestringssize);
	QStringList tmpstrlist;
	tmpstrlist.clear();
	QString tmpstr = "";
	for(uint i=0; i < fnamestringssize; i++)
	{
	    if(i % 2 == 0)
	    {
		if(filenamestrings.at(i) == '\u0000')
		{
		    tmpstrlist.append(tmpstr);
		    tmpstr = "";
		}
		else
		    tmpstr += filenamestrings.at(i);
	    }
	}
	for(int i=0; i < tmpstrlist.count(); i++)
	    htmlstr += "<tr class=even><td class=aval>File Name " + QString::number(i+1) + ":</td><td>" + tmpstrlist.at(i) + "</td></tr>";
	volinfocontent.clear();
	volinfocontent = pfcontent.mid(volinfooffset, volinfosize);
	int curpos = 0;
	for(uint i=0; i < volinfocount; i++)
	{
	    if(pfversion == 17)
		curpos = 40*i;
	    else if(pfversion == 23 || pfversion == 26)
		curpos = 104*i;
	    else if(pfversion == 30)
		curpos = 96*i;
	    uint32_t volpathoffset = qFromLittleEndian<uint32_t>(volinfocontent.mid(curpos, 4));
	    uint32_t volpathsize = qFromLittleEndian<uint32_t>(volinfocontent.mid(curpos+4, 4));
	    QByteArray volpath = volinfocontent.mid(curpos + volpathoffset, volpathsize*2);
	    QString volpathstr = "";
	    for(int j=0; j < volpath.count(); j++)
	    {
		if(j % 2 == 0)
		    volpathstr += volpath.at(j);
	    }
	    uint64_t tmpuint64 = qFromLittleEndian<uint64_t>(volinfocontent.mid(curpos+8, 8));
	    uint32_t volserial = qFromLittleEndian<uint32_t>(volinfocontent.mid(curpos+16, 4));
	    htmlstr += "<tr class=odd><td class=aval>Volume " + QString::number(i+1) + " Path:</td><td>" + volpathstr + "</td></tr>";
	    htmlstr += "<tr class=odd><td class=aval>Volume " + QString::number(i+1) + " Serial Number:</td><td>0x" + QString::number(volserial, 16) + "</td></tr>";
	    htmlstr += "<tr class=odd><td class=aval>Volume " + QString::number(i+1) + " Creation Time:</td><td>" + ConvertWindowsTimeToUnixTime(tmpuint64) + "</td></tr>";
	}
    }
    htmlstr += "</table></body></html>";

    return htmlstr;
}

QString ParseArchiveArtifact(QString archivename, QString archiveid)
{
    QString htmlstr = "";
    QFile initfile(":/html/artifactprephtml");
    initfile.open(QIODevice::ReadOnly);
    if(initfile.isOpen())
        htmlstr = initfile.readAll();
    initfile.close();
    htmlstr += "<div id='infotitle'>Archive File Analysis for " + archivename + " (" + archiveid + ")</div><br/>";
    //htmlstr += "<table width='100%'><tr><th>NAME</th><th>Value</th></tr>";
    htmlstr += "<table width='100%'><tr><th>File Name</th><th>Uncompressed Size</th><th>Compressed Size</th><th>Modified Time</th><th>Compression Method</th><th>Encryption Method</th></tr>";
    QString archivefilestr = wombatvariable.tmpfilepath + archiveid + "-fhex";
    int err = 0;
    zip* curzip = zip_open(archivefilestr.toStdString().c_str(), ZIP_RDONLY, &err);
    qint64 zipentrycount = zip_get_num_entries(curzip, 0);
    for(int i=0; i < zipentrycount; i++)
    {
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        zip_stat_index(curzip, i, 0, &zipstat);
        htmlstr += "<tr class=";
        if(i % 2 == 0)
            htmlstr += "odd";
        else
            htmlstr += "even";
        time_t modtime = zipstat.mtime;
        uint64_t temp = (uint64_t)modtime;
        temp = temp + EPOCH_DIFFERENCE;
        temp = temp * TICKS_PER_SECOND;
        htmlstr += "><td>" + QString::fromStdString(std::string(zipstat.name)) + "</td><td>" + QString::number(zipstat.size) + "</td><td>" + QString::number(zipstat.comp_size) + "</td><td>" + ConvertWindowsTimeToUnixTime(temp) + "</td><td>";
        //if(zipstat.comp_method == )
        switch(zipstat.comp_method)
        {
            case ZIP_CM_STORE:
                htmlstr += "UNCOMPRESSED";
                break;
            case ZIP_CM_SHRINK:
                htmlstr += "SHRUNK";
                break;
            case ZIP_CM_REDUCE_1:
                htmlstr += "FACTOR 1 REDUCED";
                break;
            case ZIP_CM_REDUCE_2:
                htmlstr += "FACTOR 2 REDUCED";
                break;
            case ZIP_CM_REDUCE_3:
                htmlstr += "FACTOR 3 REDUCED";
                break;
            case ZIP_CM_REDUCE_4:
                htmlstr += "FACTOR 4 REDUCED";
                break;
            case ZIP_CM_IMPLODE:
                htmlstr += "IMPLODED";
                break;
            case ZIP_CM_DEFLATE:
                htmlstr += "DEFLATE";
                break;
            case ZIP_CM_DEFLATE64:
                htmlstr += "DEFLATE64";
                break;
            case ZIP_CM_PKWARE_IMPLODE:
                htmlstr += "PKWARE IMPLODE";
                break;
            case ZIP_CM_BZIP2:
                htmlstr += "BZIP2";
                break;
            case ZIP_CM_LZMA:
                htmlstr += "LZMA";
                break;
            case ZIP_CM_TERSE:
                htmlstr += "IBM TERSE";
                break;
            case ZIP_CM_LZ77:
                htmlstr += "IBM LZ77";
                break;
            case ZIP_CM_LZMA2:
                htmlstr += "LZMA2";
                break;
            case ZIP_CM_XZ:
                htmlstr += "XZ";
                break;
            case ZIP_CM_JPEG:
                htmlstr += "COMPRESSED JPEG";
                break;
            case ZIP_CM_WAVPACK:
                htmlstr += "COMPRESSED WAVPACK";
                break;
            case ZIP_CM_PPMD:
                htmlstr += "PPMD I";
                break;
            default:
                htmlstr += "DEFLATE";
                break;
        }
        htmlstr += "</td><td>";
        //if(zipstat.encryption_method == )
        switch(zipstat.encryption_method)
        {
            case ZIP_EM_NONE:
                htmlstr += "NOT ENCRYPTED";
                break;
            case ZIP_EM_TRAD_PKWARE:
                htmlstr += "PKWARE ENCRYPTED";
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
                htmlstr += "AES 128 ENCRYPTED";
                break;
            case ZIP_EM_AES_192:
                htmlstr += "AES 192 ENCRYPTED";
                break;
            case ZIP_EM_AES_256:
                htmlstr += "AES 256 ENCRYPTED";
                break;
            case ZIP_EM_UNKNOWN:
                htmlstr += "UNKNOWN ALGORITHM";
                break;
            default:
                htmlstr += "NOT ENCRYPTED";
                break;
        }
        htmlstr += "</td></tr>";
    }
    htmlstr += "</table></body></html>";

    return htmlstr;
}

void TransferThumbnails(QString thumbid, QString reppath)
{
    if(QFile::exists(wombatvariable.tmpmntpath + "thumbs/" + thumbid + ".png"))
        QFile::copy(wombatvariable.tmpmntpath + "thumbs/" + thumbid + ".png", reppath + "thumbs/" + thumbid + ".png");
}

void TransferFiles(QString thumbid, QString reppath)
{
    QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), 1, Qt::MatchFlags(Qt::MatchExactly | Qt:: MatchRecursive));
    TreeNode* curnode = static_cast<TreeNode*>(indexlist.at(0).internalPointer());
    qulonglong filesize = curnode->Data(2).toULongLong();
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*-" + thumbid.split("-").at(0)), QDir::NoSymLinks | QDir::Dirs);
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
    QString typestr = "";
    QByteArray filecontent;
    filecontent.clear();
    QString layout = "";
    if(thumbid.contains("-z"))
    {
        int err = 0;
        RewriteSelectedIdContent(indexlist.at(0).parent()); // writes parent content to use to load zip content
        QString fnamestr = wombatvariable.tmpfilepath + thumbid.split("-z").at(0) + "-fhex";
        zip* curzip = zip_open(fnamestr.toStdString().c_str(), ZIP_RDONLY, &err);
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        int zipid = thumbid.split("-z").at(1).toInt();
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
    }
    else if(thumbid.contains("-c"))
    {
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + thumbid + ".prop");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
        {
            layout = cfile.readLine();
            cfile.close();
        }
    }
    else if(thumbid.split("-").count() == 3)
    {
        QFile fpropfile(wombatvariable.tmpmntpath + evidfiles.at(0) + "/" + thumbid.split("-").at(1) + "/" + thumbid.split("-").at(2) + ".prop");
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
    if(!thumbid.contains("-z"))
    {
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
    }
    QString tmppath = reppath + "files/";
    if(curnode->Data(8).toString() == "Directory")
    {
        // NEED TO DETERMINE IF THE FS IS NTFS, THEN I CAN WRITE $I30 AS WELL...
        // CURRENTLY USING THE FIRST 2 OF ID DON'T WORK FOR CARVED, SINCE IT DOESN'T HAVE PARTITION...
        // MAYBE I SHOULD INCLUDE PARTITION IN CARVED ID...
        QDir dir;
        bool tmpdir = dir.mkpath(QString(tmppath + thumbid));
    }
    else
    {
        QDir dir;
        bool tmpdir = dir.mkpath(QDir::cleanPath(tmppath));
        if(tmpdir)
        {
            QFile tmpfile(tmppath + thumbid);
            if(!tmpfile.isOpen())
                tmpfile.open(QIODevice::WriteOnly);
            if(tmpfile.isOpen())
            {
                if(curnode->Data(9).toString().contains("Shortcut"))
                {
                    QString lnkstr = ParseLnkArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                    tmpfile.write(lnkstr.toStdString().c_str());
                }
                else if(curnode->Data(9).toString().contains("Recycler"))
                {
                    QString info2str = ParseInfo2Artifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                    tmpfile.write(info2str.toStdString().c_str());
                }
                else if(curnode->Data(9).toString().contains("Recycle.Bin"))
                {
                    QString idollarstr = ParseIDollarArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                    tmpfile.write(idollarstr.toStdString().c_str());
                }
                else if(curnode->Data(9).toString().contains("Prefetch"))
                {
                    QString pfstr = ParsePrefetchArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                    tmpfile.write(pfstr.toStdString().c_str());
                }
                /*
                else if(curnode->Data(9).toString().contains("Zip archive"))
                {
                    QString zipstr = ParseArchiveArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                    tmpfile.write(zipstr.toStdString().c_str());
                }
                */
                else
                {
                    tmpfile.write(filecontent);
                }
                tmpfile.close();
            }
        }
    }
    /*
     *
        if(curnode->itemtype == 2 || curnode->itemtype == 11) // directory
        {
            //if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
            // EXPORT I30 PARSED CONTENT TO THE NEW PATH AS WELL.
            QDir dir;
            bool tmpdir = dir.mkpath(QString(tmppath + thumbid));
            if(!tmpdir)
            {
                qWarning() << "Creation of export directory tree for file:" << tmppath << "failed";
                //LogMessage(QString("Creation of export directory tree for file: " + tmppath + " failed"));
                errorcount++;
            }
            if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
            {
                QString i30str = ParseI30Artifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                QFile tmpfile(tmppath + thumbid);
                if(!tmpfile.isOpen())
                    tmpfile.open(QIODevice::WriteOnly);
                if(tmpfile.isOpen())
                {
                    tmpfile.write(i30str.toStdString().c_str());
                    tmpfile.close();
                }
            }
        }
        else
        {
            QDir dir;
            bool tmpdir = dir.mkpath(QDir::cleanPath(tmppath));
            if(tmpdir == true)
            {
                QFile tmpfile(tmppath + thumbid);
                if(!tmpfile.isOpen())
                    tmpfile.open(QIODevice::WriteOnly);
                if(tmpfile.isOpen())
                {
                    if(curnode->Data(9).toString().contains("Shortcut"))
                    {
                        QString lnkstr = ParseLnkArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(lnkstr.toStdString().c_str());
                    }
                    else if(curnode->Data(9).toString().contains("Recycler"))
                    {
                        QString info2str = ParseInfo2Artifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(info2str.toStdString().c_str());
                    }
                    else if(curnode->Data(9).toString().contains("Recycle.Bin"))
                    {
                        QString idollarstr = ParseIDollarArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(idollarstr.toStdString().c_str());
                    }
                    else if(curnode->Data(9).toString().contains("Prefetch"))
                    {
                        QString pfstr = ParsePrefetchArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(pfstr.toStdString().c_str());
                    }
                    else
                    {
                        QDataStream outbuffer(&tmpfile);
                        outbuffer.writeRawData(filebuffer, bufferlength);
                    }
                    tmpfile.close();
                }
            }
        }

     */ 
    /*
    if(thumbid.contains("-c"))
    {
        QString estring = thumbid.split("-").at(0);
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList(QString("*." + estring)), QDir::NoSymLinks | QDir::Dirs);
        QFile evidfile(wombatvariable.tmpmntpath + evidfiles.first() + "/stat");
        QStringList evidlist;
        evidlist.clear();
        if(!evidfile.isOpen())
            evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
        {
            evidlist = QString(evidfile.readLine()).split(",", Qt::SkipEmptyParts);
            evidfile.close();
        }
        QString rawevidencepath = wombatvariable.imgdatapath;
        if(TSK_IMG_TYPE_ISAFF((TSK_IMG_TYPE_ENUM)evidlist.at(0).toInt()))
            rawevidencepath += evidlist.at(3).split("/").last() + ".raw";
        else if(TSK_IMG_TYPE_ISEWF((TSK_IMG_TYPE_ENUM)evidlist.at(0).toInt()))
            rawevidencepath += evidlist.at(3).split("/").last() + "/ewf1";
        else if(TSK_IMG_TYPE_ISRAW((TSK_IMG_TYPE_ENUM)evidlist.at(0).toInt()))
        {
            QString imgext = evidlist.at(3).split("/").last().split("/").last();
            if(imgext.contains("000"))
                rawevidencepath += evidlist.at(3).split("/").last() + ".raw";
            else
                rawevidencepath = evidlist.at(3);
        }
        QString tmpstr;
        QFile cfile(wombatvariable.tmpmntpath + "carved/" + thumbid + ".prop");
        if(!cfile.isOpen())
            cfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(cfile.isOpen())
            tmpstr = cfile.readLine();
        cfile.close();
        QByteArray carvedcontent;
        carvedcontent.clear();
        QFile rawfile(rawevidencepath);
        if(!rawfile.isOpen())
            rawfile.open(QIODevice::ReadOnly);
        if(rawfile.isOpen())
        {
            bool isseek = rawfile.seek(tmpstr.split(",").at(16).toLongLong());
            if(isseek)
                carvedcontent = rawfile.read(tmpstr.split(",").at(8).toLongLong());
            rawfile.close();
        }
        QString tmppath = reppath + "files/";
        QDir dir;
        bool tmpdir = dir.mkpath(QDir::cleanPath(tmppath));
        if(tmpdir)
        {
            QFile tmpfile(tmppath + thumbid);
            if(!tmpfile.isOpen())
                tmpfile.open(QIODevice::WriteOnly);
            if(tmpfile.isOpen())
            {
                // IF I CARVE FOR SPECIAL ARTIFACTS (I30, INFO2, $I, PREFETCH, ETC), I NEED TO WRITE THAT CONTENT LIKE BELOW RATHER THAN RAW CONTENT...
                tmpfile.write(carvedcontent);
            }
            tmpfile.close();
        }
    }
    else
    {
        // IMPLEMENT SIMILAR FUNCTIONALITY AS EXPORTFILES FOR ZIP ARCHIVE CHILD FILES.
        QString zipid = "";
        if(thumbid.contains("z")) // transferring a child of a zip file
        {
            QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            zipid = thumbid;
            thumbid = indxlist.first().parent().sibling(indxlist.first().parent().row(), 11).data().toString();
        }
        QString tmpstr = "";
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QString estring = thumbid.split("-", Qt::SkipEmptyParts).at(0);
        QString vstring = thumbid.split("-", Qt::SkipEmptyParts).at(1);
        QString pstring = thumbid.split("-", Qt::SkipEmptyParts).at(2);
        QString fstring = thumbid.split("-", Qt::SkipEmptyParts).at(3);
        QString astring = thumbid.split("-", Qt::SkipEmptyParts).at(4);
        QStringList evidfiles = eviddir.entryList(QStringList(QString("*." + estring)), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.at(0).split(".e").first();
        TSK_IMG_INFO* imginfo = NULL;
        std::vector<std::string> pathvector;
        pathvector.clear();
        QFile evidfile(wombatvariable.tmpmntpath + evidencename + "." + estring + "/stat");
        if(!evidfile.isOpen())
            evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            tmpstr = evidfile.readLine();
        evidfile.close();
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
        TSK_FS_INFO* fsinfo = NULL;
        fsinfo = tsk_fs_open_img(imginfo, partlist.at(4).toULongLong(), TSK_FS_TYPE_DETECT);
        qint64 curaddr = 0;
        if(fstring.contains("a"))
            curaddr = astring.mid(1).toLongLong();
        else if(fstring.contains("d") || fstring.contains("o") || fstring.contains("z"))
            curaddr = fstring.mid(2).toLongLong();
        else
            curaddr = fstring.mid(1).toLongLong();
        char* filebuffer = new char[0];
        ssize_t bufferlength = 0;
        TSK_FS_FILE* fsfile = NULL;
        fsfile = tsk_fs_file_open_meta(fsinfo, NULL, curaddr);
        if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
        {
            if(fstring.contains("a")) // IF ADS
            {
                int attrid = fstring.mid(2).toInt();
                const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_id(fsfile, attrid);
                filebuffer = new char[fsattr->size];
                bufferlength = tsk_fs_file_read_type(fsfile, TSK_FS_ATTR_TYPE_NTFS_DATA, attrid, 0, filebuffer, fsattr->size, TSK_FS_FILE_READ_FLAG_SLACK);
                fsattr = NULL;
            }
            else // IF NOT ADS
            {
                filebuffer = new char[fsfile->meta->size];
                bufferlength = tsk_fs_file_read(fsfile, 0, filebuffer, fsfile->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
            }
        }
        else
        {
            filebuffer = new char[fsfile->meta->size];
            bufferlength = tsk_fs_file_read(fsfile, 0, filebuffer, fsfile->meta->size, TSK_FS_FILE_READ_FLAG_SLACK);
        }
        tsk_fs_file_close(fsfile);
        fsfile = NULL;
        tsk_fs_close(fsinfo);
        fsinfo = NULL;
        tsk_img_close(imginfo);
        imginfo = NULL;
        if(zipid.contains("z"))
        {
            QString parstr = wombatvariable.tmpfilepath + thumbid + "-fhex";
            QFile parfile(parstr);
            if(!parfile.isOpen())
                parfile.open(QIODevice::WriteOnly);
            if(parfile.isOpen())
            {
                QDataStream outbuf(&parfile);
                outbuf.writeRawData(filebuffer, bufferlength);
                parfile.close();
            }
            int err = 0;
            zip* zfile = zip_open(parstr.toStdString().c_str(), ZIP_RDONLY, &err);
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
            if(curfile != NULL)
            {
                filebuffer = new char[zstat.size];
                bufferlength = zip_fread(curfile, filebuffer, zstat.size);
                zip_fclose(curfile);
            }
            zip_close(zfile);
            thumbid = zipid;
        }
        QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(thumbid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
        TreeNode* curnode = static_cast<TreeNode*>(indexlist.first().internalPointer());
        QString tmppath = reppath + "files/";
        if(curnode->itemtype == 2 || curnode->itemtype == 11) // directory
        {
            //if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
            // EXPORT I30 PARSED CONTENT TO THE NEW PATH AS WELL.
            QDir dir;
            bool tmpdir = dir.mkpath(QString(tmppath + thumbid));
            if(!tmpdir)
            {
                qWarning() << "Creation of export directory tree for file:" << tmppath << "failed";
                //LogMessage(QString("Creation of export directory tree for file: " + tmppath + " failed"));
                errorcount++;
            }
            if(partlist.at(0).toInt() == TSK_FS_TYPE_NTFS_DETECT) // IF NTFS
            {
                QString i30str = ParseI30Artifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                QFile tmpfile(tmppath + thumbid);
                if(!tmpfile.isOpen())
                    tmpfile.open(QIODevice::WriteOnly);
                if(tmpfile.isOpen())
                {
                    tmpfile.write(i30str.toStdString().c_str());
                    tmpfile.close();
                }
            }
        }
        else
        {
            QDir dir;
            bool tmpdir = dir.mkpath(QDir::cleanPath(tmppath));
            if(tmpdir == true)
            {
                QFile tmpfile(tmppath + thumbid);
                if(!tmpfile.isOpen())
                    tmpfile.open(QIODevice::WriteOnly);
                if(tmpfile.isOpen())
                {
                    if(curnode->Data(9).toString().contains("Shortcut"))
                    {
                        QString lnkstr = ParseLnkArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(lnkstr.toStdString().c_str());
                    }
                    else if(curnode->Data(9).toString().contains("Recycler"))
                    {
                        QString info2str = ParseInfo2Artifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(info2str.toStdString().c_str());
                    }
                    else if(curnode->Data(9).toString().contains("Recycle.Bin"))
                    {
                        QString idollarstr = ParseIDollarArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(idollarstr.toStdString().c_str());
                    }
                    else if(curnode->Data(9).toString().contains("Prefetch"))
                    {
                        QString pfstr = ParsePrefetchArtifact(indexlist.first().sibling(indexlist.first().row(), 0).data().toString(), thumbid);
                        tmpfile.write(pfstr.toStdString().c_str());
                    }
                    else
                    {
                        QDataStream outbuffer(&tmpfile);
                        outbuffer.writeRawData(filebuffer, bufferlength);
                    }
                    tmpfile.close();
                }
            }
        }
        delete[] filebuffer;
    }
    */
}

void TransferArtifacts(QString artifact, QString reppath)
{
    QDir pdir;
    pdir.mkpath(reppath + "/" + artifact + "/");
    QDir artdir = QDir(wombatvariable.tmpmntpath + artifact + "/");

    //QDir eviddir = QDir(wombatvariable.tmpmntpath);
    //QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
    QStringList filelist = artdir.entryList(QDir::NoSymLinks | QDir::Files);
    for(int i=0; i < filelist.count(); i++)
    {
        //qDebug() << "filelist.at(" << i << "):" << filelist.at(i);
        QFile::copy(wombatvariable.tmpmntpath + artifact + "/" + filelist.at(i), reppath + "/" + artifact + "/" + filelist.at(i));
        //QFile::copy(wombatvariable.tmpmntpath + "thumbs/" + thumbid + ".png", reppath + "thumbs/" + thumbid + ".png");
    }
}
