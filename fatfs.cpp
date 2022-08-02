#include "fatfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

qulonglong ParseFatDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, qulonglong parinode, QString parfilename, QString dirlayout)
{
    qulonglong inodecnt = 0;
    //uint32_t fatsize = 0;
    qulonglong fatoffset = 0;
    uint16_t bytespersector = 0;
    uint8_t sectorspercluster = 0;
    uint8_t fstype = 0;
    qulonglong clusterareastart = 0;
    QString rootdirlayout = "";
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Bytes Per Sector|"))
                bytespersector = line.split("|").at(1).toUInt();
            else if(line.startsWith("Sectors Per Cluster|"))
                sectorspercluster = line.split("|").at(1).toUInt();
            else if(line.startsWith("Root Directory Layout|"))
                rootdirlayout = line.split("|").at(1);
            else if(line.startsWith("FAT Offset|"))
                fatoffset = line.split("|").at(1).toULongLong();
            //else if(line.startsWith("FAT Size|"))
            //    fatsize = line.split("|").at(1).toUInt();
            else if(line.startsWith("Cluster Area Start|"))
                clusterareastart = line.split("|").at(1).toULongLong();
	    else if(line.startsWith("File System Type Int|"))
		fstype = line.split("|").at(1).toUInt();
        }
        propfile.close();
    }
    if(!dirlayout.isEmpty())
	rootdirlayout = dirlayout;
    //qDebug() << "rootdirlayout:" << rootdirlayout;
    //qDebug() << "bps:" << bytespersector << "fo:" << fatoffset << "fs:" << fatsize << "rdl:" << rootdirlayout;
    for(int i=0; i < rootdirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
        //qDebug() << "root dir entry:" << i;
	qulonglong rootdiroffset = 0;
	qulonglong rootdirsize = 0;
	if(i == 0)
	{
	    if(dirlayout.isEmpty()) // root directory
	    {
	        rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
		rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
	    }
	    else // sub directory
	    {
		rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong() + 64; // skip . and .. directories
		rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() - 64; // adjust read size for the 64 byte skip
	    }
	}
        uint rootdirentrycount = rootdirsize / 32;
        //qDebug() << "current rootdirentrycount:" << rootdirentrycount;
	if(parinode == 0)
	    inodecnt = 0;
	else
	    inodecnt = parinode + 1;
	//qDebug() << "inodecnt at start of parent comparison:" << inodecnt;
        QString longnamestring = "";
        for(uint j=0; j < rootdirentrycount; j++)
        {
	    QString longname = "";
            QTextStream out;
            QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
            if(!fileprop.isOpen())
                fileprop.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&fileprop);

            uint8_t firstchar = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32, 1));
            if(firstchar == 0x00) // entry is free and all remaining are free
                break;
            uint8_t fileattr = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 11, 1));
            //qDebug() << "firstchar:" << QString::number(firstchar, 16) << "fileattr:" << QString::number(fileattr, 16);
            if(fileattr != 0x0f && fileattr != 0x00 && fileattr != 0x3f) // need to process differently // 0x3f is ATTR_LONG_NAME_MASK which is a long name entry sub component
            {
                if(!longnamestring.isEmpty())
                {
                    out << "Long Name|" << longnamestring << "|Long name for the file." << Qt::endl;
		    longname = longnamestring;
                    longnamestring = "";
                }
                else
                    out << "Long Name| |Long name for the file." << Qt::endl;
                out << "File Attributes|";
                if(fileattr & 0x01)
                    out << "Read Only,";
                if(fileattr & 0x02)
                    out << "Hidden File,";
                if(fileattr & 0x04)
                    out << "System File,";
                if(fileattr & 0x08)
                    out << "Volume ID,";
                if(fileattr & 0x10)
                    out << "SubDirectory,";
                if(fileattr & 0x20)
                    out << "Archive File,";
                out << "|File attributes." << Qt::endl;
                uint8_t isdeleted = 0;
                if(firstchar == 0xe5 || firstchar == 0x05) // was allocated but now free
                    isdeleted = 1;
                QString restname = QString::fromStdString(curimg->ReadContent(rootdiroffset + j*32 + 1, 7).toStdString()).replace(" ", "");
                QString extname = QString::fromStdString(curimg->ReadContent(rootdiroffset + j*32 + 8, 3).toStdString()).replace(" ", "");
                QString aliasname = QString(char(firstchar));
                out << "Alias Name|";
                if(extname.count() > 0)
                {
                    aliasname += QString(restname.toUtf8() + "." + extname.toUtf8());
                }
                else
                {
                    aliasname += QString(restname.toUtf8());
                }
                out << aliasname << "|8.3 file name." << Qt::endl;
		//uint8_t createtenth = rootdirbuf.at(i*32 + 13); // NOT GOING TO USE RIGHT NOW...
                qint64 createdate = ConvertDosTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 15, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 17, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 1)));
                qint64 accessdate = ConvertDosTimeToUnixTime(0x00, 0x00, qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 19, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 1)));
                qint64 modifydate = ConvertDosTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 23, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 25, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 1)));
                uint16_t hiclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)); // always zero for fat12/16
                uint16_t loclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 26, 2));
                uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
                QList<uint> clusterlist;
                clusterlist.clear();
                if(clusternum >= 2)
                {
                    clusterlist.append(clusternum);
                    GetNextCluster(curimg, clusternum, fstype, fatoffset, &clusterlist);
                }
                QString layout = "";
                if(clusterlist.count() > 0)
		    layout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
		out << "Layout|" << layout << "|File offset,size; layout in bytes." << Qt::endl;
                qulonglong physicalsize = clusterlist.count() * sectorspercluster * bytespersector;
		out << "Physical Size|" << QString::number(physicalsize) << "|Sector Size in Bytes for the file." << Qt::endl;
                clusterlist.clear();
		uint32_t logicalsize = 0;
		uint8_t itemtype = 0;
		if(fileattr & 0x10) // sub directory attribute
		{
		    if(firstchar == 0xe5 || firstchar == 0x05) // deleted directory
			itemtype = 2;
		    else // directory
			itemtype = 3;
                    qulonglong curdirsize = 0;
                    int lastdirentry = 0;
                    int curdircnt = 0;
                    for(int k = 0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                    {
                        curdirsize += layout.split(";").at(k).split(",").at(1).toULongLong();
                    }
                    for(int k=0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                    {
                        for(uint m=0; m < curdirsize / 32; m++)
                        {
                            uint8_t curfirstchar = qFromLittleEndian<uint8_t>(curimg->ReadContent(layout.split(";").at(k).split(",").at(0).toULongLong() + m*32, 1));
                            if(curfirstchar == 0x00) // entry is free and all remaining are free
                            {
                                lastdirentry = curdircnt;
                                break;
                            }
                            curdircnt++;
                        }
                    }
                    logicalsize = lastdirentry * 32;
		}
		else
		{
		    if(firstchar == 0xe5 || firstchar == 0x05) // deleted file
			itemtype = 4;
		    else
			itemtype = 5;
		    logicalsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 4));
		}
		out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
		//qDebug() << "logicalsize:" << logicalsize;
		// ADD FILE INFO TO THE NODE TREE...
		QString filepath = "";
		QString filename = "";
		if(longname.isEmpty())
		    filename = aliasname;
		else
		    filename = longname;
		QHash<QString, QVariant> nodedata;
		nodedata.clear();
		nodedata.insert("name", QByteArray(filename.toStdString().c_str()).toBase64());
		if(parfilename.isEmpty())
		    filepath = "/";
		else
		    filepath = parfilename;
		nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
                nodedata.insert("size", logicalsize);
                nodedata.insert("create", createdate);
                nodedata.insert("access", accessdate);
                nodedata.insert("modify", modifydate);
                //nodedata.insert("status", "0");
                //nodedata.insert("hash", "0");
		if(logicalsize > 0) // Get Category/Signature
		{
		    if(itemtype == 3 && isdeleted == 0)
                    {
			nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory"); // category << signature
                    }
		    else
		    {
			//QString catsig = GenerateCategorySignature(curimg, filename, layout); might not need this method, could just read more and see if it has word/ in hex.
			QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			//qDebug() << filename.left(20) << catsig;
                        nodedata.insert("cat", catsig.split("/").first());
                        nodedata.insert("sig", catsig.split("/").last());
		    }
		}
                else
                {
                    nodedata.insert("cat", "Empty");
                    nodedata.insert("sig", "Zero File");
                }
                //nodedata.insert("tag", "0");
                nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
		//nodedata.insert("match", 0);
                QString parentstr = "";
                if(parinode == 0)
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
                else
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(parinode));
                mutex.lock();
                treenodemodel->AddNode(nodedata, parentstr, itemtype, isdeleted);
                mutex.unlock();
                if(nodedata.value("id").toString().split("-").count() == 3)
                {
                    listeditems.append(nodedata.value("id").toString());
                    filesfound++;
                    isignals->ProgUpd();
                }
                inodecnt++;
                out.flush();
                fileprop.close();
                nodedata.clear();
		//qDebug() << "inodecnt:" << inodecnt << filename << layout;
                if(fileattr & 0x10 && logicalsize > 0) // sub directory
                {
                    if(firstchar != 0xe5 && firstchar != 0x05) // not deleted
			inodecnt = ParseFatDirectory(curimg, curstartsector, ptreecnt, inodecnt-1, QString(filepath + filename + "/"), layout);
                }
            }
            else if(fileattr == 0x0f || 0x3f) // long directory entry for succeeding short entry...
            {
                if(firstchar & 0x40)
                {
                    if(!longnamestring.isEmpty()) // orphan long entry
                    {
			QHash<QString, QVariant> nodedata;
			nodedata.clear();
			nodedata.insert("name", QByteArray(longnamestring.toUtf8()).toBase64());
                        nodedata.insert("path", QByteArray("/orphans/").toBase64());
                        nodedata.insert("size", 0);
                        nodedata.insert("create", "0");
                        nodedata.insert("access", "0");
                        nodedata.insert("modify", "0");
                        nodedata.insert("status", "0");
                        nodedata.insert("hash", "0");
                        nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Empty File");
                        nodedata.insert("tag", "0");
                        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
			nodedata.insert("match", 0);
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o"), 4, 1);
			mutex.unlock();
			nodedata.clear();
			inodecnt++;
                        //qDebug() << "orphan:" << longnamestring;
                        //orphaninfo.clear();
                        //orphaninfo.insert("filename", QVariant(longnamestring));
                        //orphanlist->append(orphaninfo);
                        //orphanlist->append(longnamestring);
                        longnamestring = "";
                    }
                }
                QString l3 = "";
                QString l2 = "";
                QString l1 = "";
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 2)) < 0xFFFF)
                    l3 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 30, 2)) < 0xFFFF)
                    l3 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 30, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 3, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 3, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 5, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 5, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 7, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 7, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 2))));
                longnamestring.prepend(QString(l1 + l2 + l3).toUtf8());
            }
        }
    }
    return inodecnt;
}

