#include "apfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

bool CheckChecksum(ForImg* curimg, uint64_t blockbyteoffset, uint32_t size, uint64_t curchecksum)
{
    uint64_t lower32bit = 0;
    uint64_t upper32bit = 0;
    uint32_t value32bit = 0;
    size_t bufferoffset = 0;
    uint64_t initialvalue = 0;
    uint64_t checksum = 0;
    lower32bit = initialvalue & 0xffffffffUL;
    upper32bit = (initialvalue >> 32) & 0xffffffffUL;
    //QByteArray blockminus8 = curimg->ReadContent(blockbyteoffset + 8, 
    for(bufferoffset = 0; bufferoffset < size; bufferoffset += 4)
    {
        value32bit = qFromLittleEndian<uint32_t>(curimg->ReadContent(blockbyteoffset + bufferoffset, 4));
        lower32bit += value32bit;
        upper32bit += lower32bit;
    }
    lower32bit %= 0xffffffffUL;
    upper32bit %= 0xffffffffUL;
    value32bit = 0xffffffffUL - ((lower32bit + upper32bit) % 0xffffffffUL);
    upper32bit = 0xffffffffUL - ((lower32bit + value32bit) % 0xffffffffUL);

    checksum = (upper32bit << 32) | value32bit;
    //qDebug() << "checksum:" << checksum << "curchecksum:" << curchecksum;

    if(checksum == curchecksum)
        return true;
    else
        return false;
}

void ParseApfsVolumes(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    //qDebug() << "superblock checksum:" << CheckChecksum(curimg, curstartsector*512 + 8, qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 36, 4)) - 8, qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512, 8)));
    uint64_t nxomapoid = 0;
    uint64_t objectmapoid = 0;
    uint64_t nxoid = 0;
    uint64_t nxxid = 0;
    uint64_t roottreeoid = 0;
    uint64_t nxxpdescbase = 0;
    uint32_t nxxpdescblocks = 0;
    uint32_t blocksize = 0;
    uint8_t encryptionstatus = 0;
    QStringList volumeoidlist;
    QList<uint64_t> volofflist;
    volofflist.clear();

    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Object ID|"))
                nxoid = line.split("|").at(1).toULongLong();
            if(line.startsWith("Transaction ID|"))
                nxxid = line.split("|").at(1).toULongLong();
            if(line.startsWith("Block Size|"))
                blocksize = line.split("|").at(1).toUInt();
            if(line.startsWith("Container Object Map Object ID|"))
                nxomapoid = line.split("|").at(1).toUInt();
            if(line.startsWith("Volume Object ID List|"))
                volumeoidlist = line.split("|").at(1).split(",", Qt::SkipEmptyParts);
            /*
            if(line.startsWith("CheckPoint Descriptor Base|"))
                nxxpdescbase = line.split("|").at(1).toULongLong();
            if(line.startsWith("CheckPoint Description Blocks|"))
                nxxpdescblocks = line.split("|").at(1).toULong();
            */
        }
        propfile.close();
    }   
    // NOW TO DO THE OBJECT MAP PARSING FOR WHAT I NEED
    uint64_t nxomapoffset = curstartsector*512 + nxomapoid * blocksize;
    uint64_t omapbtreeoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxomapoffset + 48, 8));
    uint64_t omapbtreeoff = omapbtreeoid * blocksize + curstartsector*512;
    // IGNORE THIS IF/ELSE FOR NOW, SINCE IT SEEMS TO INCLUDE STUFF
    uint32_t omapobjtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(nxomapoffset + 24, 4));
    /*
    if(omapobjtype != 0x4000000b) // PHYSICAL OBJECT MAP
    {
        qDebug() << "error, not a valid object map..";
    }
    else
        qDebug() << "object map type is good, continue...";
    */
    QList<uint64_t> oidlist;
    //QList<uint64_t> xidlist;
    QList<uint64_t> blklist;
    oidlist.clear();
    //xidlist.clear();
    blklist.clear();
    // START TO PARSE THE BTREE NODE
    uint16_t btreeflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(omapbtreeoff + 32, 2));
    uint32_t keycount = qFromLittleEndian<uint32_t>(curimg->ReadContent(omapbtreeoff + 36, 4));
    uint16_t tocoff = qFromLittleEndian<uint16_t>(curimg->ReadContent(omapbtreeoff + 40, 2));
    uint16_t toclen = qFromLittleEndian<uint16_t>(curimg->ReadContent(omapbtreeoff + 42, 2));
    uint32_t valoff = blocksize - 40 - (keycount+1) * 16;
    //qDebug() << "keycount:" << keycount << "tocoff:" << tocoff << "toclen:" << toclen << "valoff:" << valoff;
    for(int i=0; i <= keycount; i++)
    {
        oidlist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(omapbtreeoff + 56 + tocoff + toclen + i*16, 8)));
        //xidlist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(omapbtreeoff + 56 + tocoff + toclen + i*16 + 8, 8)));
        blklist.prepend(qFromLittleEndian<uint64_t>(curimg->ReadContent(omapbtreeoff + valoff + i*16 + 8, 8)));
    }
    for(int i=0; i < volumeoidlist.count(); i++)
    {
        //qDebug() << "volume oid:" << volumeoidlist.at(i);
        for(int j=0; j < oidlist.count(); j++)
        {
            //qDebug() << "oidlist:" << oidlist.at(j) << "blklist:" << blklist.at(j);
            if(volumeoidlist.at(i).toULongLong() == oidlist.at(j))
                volofflist.append(blklist.at(j) * blocksize + curstartsector*512);
        }
    }
    qDebug() << "volofflist:" << volofflist;
    QString parentstr = "e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt);
    for(int i=0; i < volofflist.count(); i++)
    {
        QString volidstr = "p" + QString::number(ptreecnt) + "v" + QString::number(i);
        QString apfsvolname = "";
        quint64 encryptionstate = 0;
        quint64 apfsflags = 0;
        quint64 curoffset = volofflist.at(i);
        //quint64 curoffset = vollayoutlist.at(i).split(",").at(0).toULongLong();
        QDir dir; // current partition directory
        dir.mkpath(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/v" + QString::number(i) + "/");
        QFile contstatfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/stat");
        // copy partition stat file to the volume stat file since they are the same
        contstatfile.copy(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/v" + QString::number(i) + "/stat");
        QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/v" + QString::number(i) + "/prop");
        QTextStream out;
        if(!propfile.isOpen())
            propfile.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&propfile);
        if(propfile.isOpen())
        {
            out << "Object ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 8, 8))) << "|APFS object id." << Qt::endl;
            out << "Transaction ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 16, 8))) << "|APFS transaction id." << Qt::endl;
            // NEED TO PROCESS PROPERLY AND DO AN IF & THING FOR IT...
            //out << "Object Type|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 24, 2))) << "|APFS object type: 1 - container super block, 2 - btree, 3 btree node, 5 - spaceman, 11 - object map (OMAP), 13 - file system (volume super block), 17 - reaper." << Qt::endl;
            //out << "Object Flags|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 26, 2))) << "|APFS object flags: 0 - OBJ_VIRTUAL, 80 - OBJ_EPHEMERAL, 40 - OBJ_PHYSICAL, 20 - OBJ_NOHEADER, 10 - OBJ_ENCRYPTED, 08 - OBJ_NONPERSISTENT." << Qt::endl;
            //out << "Object SubType|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 28, 4))) << "|APFS object subtype: 0 - none, 11 - object map (OMAP), 14 - file system tree." << Qt::endl;
            out << "FileSystem Index|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 36, 4))) << "|Index number in the volume array for the APFS file system." << Qt::endl;
            // NEED TO PROCESS PROPERLY AND DO AN IF & THING FOR IT...
            out << "Features|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 40, 8))) << "|Features for the APFS volume." << Qt::endl;
            out << "Read-Only Compatible Features|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8))) << "|Read-only compatible features for the APFS volume." << Qt::endl;
            out << "Incompatible Features|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 56, 8))) << "|Incompatible APFS features." << Qt::endl;
            // NEED TO FIX THE TIME TO A STRING AND CONVERT TO UNIX TIME
            out << "Unmount Time|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 64, 8))) << "|Time when the volume was last unmounted." << Qt::endl;
            out << "FileSystem Reserve Block Count|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 72, 8))) << "|Block pre-allocated for volume." << Qt::endl;
            out << "FileSystem Quota Block Count|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 80, 8))) << "|Block quota (maximum block allocated) for volume." << Qt::endl;
            out << "FileSystem Allocation Count|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 88, 8))) << "|Number of blocks currently allocation." << Qt::endl;
            out << "Wrapped Crypto State Major Version|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 96, 2))) << "|Major version for the APFS wrapped crypto state." << Qt::endl;
            out << "Wrapped Crypto State Minor Version|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 98, 2))) << "|Minor version for the APFS wrapped crypto state." << Qt::endl;
            out << "Wrapped Crypto State Flags|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 100, 4))) << "|Flags for the APFS wrapped crypto state." << Qt::endl;
            out << "Wrapped Crypto State Persistent Class|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 104, 4))) << "|Persistent class for the APFS wrapped crypto state." << Qt::endl;
            out << "Wrapped Crypto State Key OS Version|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 108, 4))) << "|Key OS version for the APFS wrapped crypto state." << Qt::endl;
            out << "Wrapped Crypto State Key Revision|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 112, 2))) << "|Key revision for the APFS wrapped crypto state." << Qt::endl;
            out << "Wrapped Crypto State Key Length|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 114, 2))) << "|Key length for the APFS wrapped crypto state, 0 for no encryption." << Qt::endl;
            uint16_t cryptokeylength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 114, 2));
            // NEED TO FIX THIS TO DISPLAY THE KEY BASED ON KEY LENGTH ABOVE...
            //curoffset + 116 + cryptokeylength 
            //out << "APFS Wrapped Crypto State Persistent Key|" < QString::number
            // NEED TO FIX THIS TO DISPLAY THE ROOT TREE TYPE PROPERLY WITH & SWITCHES
            out << "Root Tree Type|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 116 + cryptokeylength, 4))) << "|Type of root file system tree. Object Type and Object SubType." << Qt::endl;
            out << "Extent Reference Tree Type|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + cryptokeylength + 120, 4))) << "|Type of extent reference tree." << Qt::endl;
            out << "Snapshot Metadata Tree Type|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + cryptokeylength + 124, 4))) << "|Type of snapshot metadata tree." << Qt::endl;
            out << "Object Map OID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 128, 8))) << "|Physical object ID of the APFS object map." << Qt::endl;
            out << "Root Tree OID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 136, 8))) << "|Virtual object ID of the root APFS tree." << Qt::endl;
            out << "Extent Reference Tree OID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 144, 8))) << "|Physical Object ID of the APFS extent reference tree." << Qt::endl;
            out << "Snapshot Metadata Tree OID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 152, 8))) << "|Virtual Object ID of the APFS snapshot metadata tree." << Qt::endl;
            out << "Revert To XID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 160, 8))) << "|Transaction ID that the APFS volume will revert to." << Qt::endl;
            out << "Revert to Super Block OID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 168, 8))) << "|Virtual Object ID of the Volume Super Block to revert to." << Qt::endl;
            out << "Next Object ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 176, 8))) << "|Next object ID for the APFS volume." << Qt::endl;
            out << "Number of Regular Files|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 184, 8))) << "|Number of regular files." << Qt::endl;
            out << "Number of Directories|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 192, 8))) << "|Number of directories." << Qt::endl;
            out << "Number of Symbolic Links|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 200, 8))) << "|Number of symbolic links." << Qt::endl;
            out << "Number of Other Files|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 208, 8))) << "|Number of other files." << Qt::endl;
            out << "Number of Snapshots|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 216, 8))) << "|Number of snapshots." << Qt::endl;
            out << "Total Blocks Allocated|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 224, 8))) << "|Blocks allocated by the volume." << Qt::endl;
            out << "Total Blocks Freed|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 232, 8))) << "|Blocks freed by the volume." << Qt::endl;
            // NEED TO FORMAT PROPERLY
            out << "Volume UUID|" << QString::fromStdString(curimg->ReadContent(curoffset + cryptokeylength + 240, 16).toHex().toStdString()) << "|Universal Unique Identifier for the volume." << Qt::endl;
            // NEED TO FORMAT AS HUMAN READABLE STRING
            out << "Last Modified Time|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 256, 8))) << "|Last modified time stamp for the volume." << Qt::endl;
            // NEED TO FORMAT PROPERLY
            out << "File System Flags|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 264, 8))) << "|Flags for the file system." << Qt::endl;
            out << "Formatted By ID|" << QString::fromStdString(curimg->ReadContent(curoffset + cryptokeylength + 272, 32).toStdString()) << "|Program and version that formatted the volume." << Qt::endl;
            out << "Formatted Timestamp|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 304, 8))) << "|Timestamp when the volume was formatted." << Qt::endl;
            out << "Formatted XID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 312, 8))) << "|Transaction ID for the formatting of the volume." << Qt::endl;
            out << "Last Modified ID|" << QString::fromStdString(curimg->ReadContent(curoffset + cryptokeylength + 320, 32).toStdString()) << "|Program and version that last modified the volume." << Qt::endl;
            out << "Last Modified Timestamp|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 352, 8))) << "|Timestamp when the volume was last modified." << Qt::endl;
            out << "Last Modified XID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 360, 8))) << "|Transaction ID the volume was last modified." << Qt::endl;
            // LOOP AND GET THE LAST MODIFIED ARRAY, 8 OF THEM
            for(int j=0; j < 8; j++)
            {
                out << "Last Modified " << QString::number(j+1) <<  " ID|" << QString::fromStdString(curimg->ReadContent(curoffset + cryptokeylength + 368 + j*48, 32).toStdString()) << "|Program and version that last modified the volume." << Qt::endl;
                out << "Last Modified " << QString::number(j+1) << " Timestamp|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 400 + j*48, 8))) << "|Timestamp when the volume was last modified." << Qt::endl;
                out << "Last Modified " << QString::number(j+1) << " XID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 406 + j*48, 8))) << "|Transaction ID the volume was last modified." << Qt::endl;
            }
            apfsvolname = QString::fromStdString(curimg->ReadContent(curoffset + cryptokeylength + 704, 256).toStdString());
            out << "Volume Name|" << QString::fromStdString(curimg->ReadContent(curoffset + cryptokeylength + 704, 256).toStdString()) << "|Volume name for the APFS volume." << Qt::endl;
            //qDebug() << "APFS volume name:" << apfsvolname;
            out << "Next Document ID|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + cryptokeylength + 960, 4))) << "|Next document ID." << Qt::endl;
            // NEED TO FORMAT PROPERLY
            out << "Role|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + cryptokeylength + 964, 2))) << "|APFS role. 0x00 - none, etc." << Qt::endl;
            // NEXT 2 BYTES ARE RESERVED
            out << "Root To XID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 976, 8))) << "|Transaction ID of Snapshot to Root." << Qt::endl;
            out << "Encryption State OID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 984, 8))) << "|Current State of Encryption/Decryption." << Qt::endl;
            encryptionstate = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 984, 8));
            apfsflags = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 264, 8));
            qDebug() << "encryption state:" << encryptionstate;
            if(apfsflags & 0x00000001)
            {
                encryptionstatus = 0; // not encrypted
                //qDebug() << "apfs fs unencrypted.";
                objectmapoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 128, 8));
                roottreeoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + cryptokeylength + 136, 8));
                //QString rootbtreelayout = ReturnBTreeLayout(curimg, curstartsector, ptreecnt, i, blocksize, objectmapoid, roottreeoid);
            }
            else if(apfsflags & 0x00000008)
            {
                encryptionstatus = 1; // software encrypted, can decrypt
                qDebug() << "if i go into the keybag, i can find the key entry with the password hint...";
                qDebug() << "apfs fs encrypted using VEK, please provide password...";
            }
            else // not sure what flag will tell me if it is hardware encrypted
            {
                qDebug() << "this might account for harware encrypted, not sure yet...";
                encryptionstatus = 2; // hardware encrypted, can not decrypt
            }
            //qDebug() << "encryption state:" << encryptionstate << "fsflags:" << apfsflags;
            /*
            working on apfs volume, need to build the encryption dialog...
            Hardware Encryption Detected, can't do anything... skipping...
            Software Encryption Detected, provide user password or recovery key to parse...
            No Encryption Detected, parsing...
             */ 
            /*
            // create new case here
            QInputDialog* casedialog = new QInputDialog(this);
            casedialog->setCancelButtonText("Cancel");
            casedialog->setInputMode(QInputDialog::TextInput);
            casedialog->setLabelText("Enter Case Name");
            casedialog->setOkButtonText("Create Case");
            casedialog->setTextEchoMode(QLineEdit::Normal);
            casedialog->setWindowTitle("New Case");
            if(casedialog->exec())
                wombatvariable.casename = casedialog->textValue();
            if(!wombatvariable.casename.isEmpty())
            {
                QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
                this->setWindowTitle(QString("Wombat Forensics - ") + wombatvariable.casename);
             */ 
            out.flush();
            propfile.close();
        }
        //qDebug() << "parent id:" << parentstr;
        QHash<QString, QVariant> nodedata;
        nodedata.clear();
        nodedata.insert("name", apfsvolname);
        //nodedata.insert("path", "/");
        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "v" + QString::number(i)));
        QFile statfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/v" + QString::number(i) + "/stat");
        if(!statfile.isOpen())
            statfile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString tmpstr = "";
        if(statfile.isOpen())
        {
            tmpstr = statfile.readLine(); // partition name, offset, size, partition type, id
            statfile.close();
        }
        nodedata.insert("size", tmpstr.split(",").at(1).toULongLong());
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, -1, 0);
        mutex.unlock();
        if(encryptionstatus == 0) // not encrypted
        {
            qDebug() << "objectmapoid:" << objectmapoid << "roottreeoid:" << roottreeoid;
            qInfo() << "Parse Root Directory...";
            uint64_t rootbtreeoffset = ReturnRootBTreeOffset(curimg, curstartsector, blocksize, objectmapoid, roottreeoid);
            qDebug() << "rootbtreeoffset:" << rootbtreeoffset;
            //uint64_t rootbtreelayout = ReturnBTreeLayout(curimg, curstartsector, blocksize, objectmapoid, roottreeoid);
            //QString rootbtreelayout = ReturnBTreeLayout(curimg, curstartsector, blocksize, objectmapoid, roottreeoid);
            //qDebug() << "root btree layout:" << rootbtreelayout;
            ParseApfsDirectory(curimg, curstartsector, ptreecnt, i, blocksize, QString::number(rootbtreeoffset));
        }
    }
}

uint64_t ReturnRootBTreeOffset(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, uint64_t objectmapoid, uint64_t roottreeoid)
{
    qDebug() << "roottreeoid to match:" << roottreeoid;
    QList<uint64_t> oidlist;
    //QList<uint64_t> xidlist;
    QList<uint32_t> sizlist;
    QList<uint64_t> blklist;
    uint64_t romapoff = objectmapoid * blocksize + curstartsector*512;
    uint64_t romapbtreeoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(romapoff + 48, 8));
    qDebug() << "rompabtreeoid:" << romapbtreeoid;
    uint64_t romapbtreeoff = romapbtreeoid * blocksize + curstartsector*512;
    uint32_t romapobjtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(romapoff + 24, 4));
    //qDebug() << "romapobjtype:" << QString::number(romapobjtype, 16);
    //qDebug() << "romapbtreeobjtype:" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(romapbtreeoff + 24, 4)), 16);
    //qDebug() << "romapbtreesubtype:" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(romapbtreeoff + 28, 4)), 16);
    oidlist.clear();
    sizlist.clear();
    blklist.clear();
    // START TO PARSE THE ROOT BTREE NODE
    uint16_t btreeflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(romapbtreeoff + 32, 2));
    uint32_t keycnt = qFromLittleEndian<uint32_t>(curimg->ReadContent(romapbtreeoff + 36, 4));
    uint16_t tocoff = qFromLittleEndian<uint16_t>(curimg->ReadContent(romapbtreeoff + 40, 2));
    uint16_t toclen = qFromLittleEndian<uint16_t>(curimg->ReadContent(romapbtreeoff + 42, 2));
    uint32_t valoff = blocksize - 40 - (keycnt+1) * 16;
    if(btreeflags & 0x0002)
    {
        qDebug() << "btree is leaf node";
        //valoff = valoff - 40;
    }
    qDebug() << "start table of content info:" << toclen / 8;
    for(uint16_t i=0; i < toclen / 8; i++)
    {
        uint64_t curoffset = romapbtreeoff + 56 + tocoff + i*8;
        qDebug() << "i:" << i;
        qDebug() << "key off:" << qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset, 2));
        qDebug() << "key len:" << qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 2, 2));
        qDebug() << "val off:" << qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 4, 2));
        qDebug() << "val len:" << qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 6, 2));
        if(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset, 2)) == 0 && qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 2, 2)) == 0 && qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 4, 2)) == 0 && qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 6, 2)) == 0)
            break;
    }
    qDebug() << "end table of content info";
    // VALID POTENTIAL ROOT PHYSICAL BLOCK #'S:
    // 122247 (87dd01), 122064 (d0dc01), 122419 (33de01), 122525 (9dde01), 122711 (57df01), 122768 (90df01), 122787 (a3df01),
    // 122948 (44e001), 122986 (6ae001), 123091 (d3e001), 123266 (82e101), 123297 (a1e101), 123832 (b8e301), 126706 (f2ef01),
    // 131808 (e00202), 133084 (dc0702), 133945 (390b02), 134548 (940d02), 135298 (821002), 138117 (851b02), 140733 (bd2502),
    // 146146 (e23a02), 147276 (4c3f02), 151773 (dd5002), 152223 (9f5202), 153477 (855702), 156856 (b86402), 172584 (28a202),
    // 177406 (feb402), 183394 (62cc02), 185114 (1ad302), 187421 (1ddc02), 187675 (1bdd02), 187879 (e7dd02), 189335 (97e302),
    // 189777 (51e502), 190142 (bee602), 206822 (e62703), 206954 (6a2803), 209300 (943103), 215190 (964803), 215546 (fa4903),
    // 215640 (5b4a03)
    qDebug() << "keycount:" << keycnt << "tocoff:" << tocoff << "toclen:" << toclen << "valoff:" << valoff;
    for(uint32_t i=0; i <= keycnt; i++)
    {
        oidlist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(romapbtreeoff + 56 + tocoff + toclen + i*16, 8)));
        blklist.prepend(qFromLittleEndian<uint64_t>(curimg->ReadContent(romapbtreeoff + valoff + i*16 + 8, 8)));
        sizlist.prepend(qFromLittleEndian<uint32_t>(curimg->ReadContent(romapbtreeoff + valoff + i*16 + 4, 4)));
    }
    for(int i=0; i < oidlist.count(); i++)
    {
        //qDebug() << "roottreeoid: " << roottreeoid << "oidlist:" << oidlist.at(i);
        if(roottreeoid == oidlist.at(i))
        {
            uint32_t rootbtreeobjtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(blklist.at(i) * blocksize + curstartsector*512 + 24, 4));
            uint32_t rootbtreesubtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(blklist.at(i) * blocksize + curstartsector*512 + 28, 4));
            qDebug() << "size:" << sizlist.at(i);
            qDebug() << "blk:" << blklist.at(i);
            qDebug() << "rootbtreeobjtype:" << QString::number(rootbtreeobjtype, 16);
            qDebug() << "rootbtreesubtype:" << QString::number(rootbtreesubtype, 16);
            if(rootbtreesubtype != 0x0000000e)
            {
                qDebug() << "root tree subtype isn't a FSTREE, so something else needs to be done... but what.?";
            }
            return (blklist.at(i) * blocksize + curstartsector*512);

            //return (blklist.at(i) * blocksize + curstartsector*512);
        }
    }

    return 0;
    /*
    // IGNORE THIS IF/ELSE FOR NOW, SINCE IT SEEMS TO INCLUDE STUFF
    uint32_t omapobjtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(nxomapoffset + 24, 4));
    //if(omapobjtype != 0x4000000b) // PHYSICAL OBJECT MAP
    //{
    //    qDebug() << "error, not a valid object map..";
    //}
    //else
    //    qDebug() << "object map type is good, continue...";
    // START TO PARSE THE BTREE NODE
    //qDebug() << "keycount:" << keycount << "tocoff:" << tocoff << "toclen:" << toclen << "valoff:" << valoff;
    for(int i=0; i <= keycount; i++)
    {
        oidlist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(omapbtreeoff + 56 + tocoff + toclen + i*16, 8)));
        //xidlist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(omapbtreeoff + 56 + tocoff + toclen + i*16 + 8, 8)));
        blklist.prepend(qFromLittleEndian<uint64_t>(curimg->ReadContent(omapbtreeoff + valoff + i*16 + 8, 8)));
    }
    for(int i=0; i < volumeoidlist.count(); i++)
    {
        for(int j=0; j < oidlist.count(); j++)
        {
            if(volumeoidlist.at(i).toULongLong() == oidlist.at(j))
                volofflist.append(blklist.at(j) * blocksize + curstartsector*512);
        }
    }

     */ 
}

//uint64_t ReturnBTreeLayout(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, uint64_t objectmapoid, uint64_t roottreeoid)
//{
    /*
    qDebug() << "roottreeoid to match:" << roottreeoid;
    QList<uint64_t> keylist;
    QList<uint64_t> vallist;
    //QList<uint32_t> sizlist;
    QList<uint64_t> xidlist;
    keylist.clear();
    vallist.clear();
    //sizlist.clear();
    xidlist.clear();
    uint32_t btreekeycount = 0;
    //uint16_t btreeflags = 0;
    uint64_t omapbtreeoid = 0;
    uint16_t objecttype = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * objectmapoid + 24, 2));
    uint16_t objectflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * objectmapoid + 26, 2));
    uint32_t objectsubtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + blocksize * objectmapoid + 28, 4));
    qDebug() << "object type:" << QString::number(objecttype, 16);
    qDebug() << "object falgs:" << QString::number(objectflags, 16);
    qDebug() << "object subtype:" << QString::number(objectsubtype, 16);
    qDebug() << "object map oid:" << objectmapoid;
    qDebug() << "omapoid offset:" << objectmapoid * blocksize + curstartsector*512;
    if(objecttype == 0x02 || objecttype == 0x03) // BTREE || BTREE_NODE
    {
        qDebug() << "in a btree, no need to go to an object map btree oid.";
        omapbtreeoid = objectmapoid;
    }
    else if(objecttype == 0x0b) // OMAP
    {
        omapbtreeoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512 + blocksize * objectmapoid + 48, 8));
        qDebug() << "not in a btree, need to go to the object map btree.";
    }
    qDebug() << "omap btree oid:" << omapbtreeoid;
    uint16_t btreeflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 32, 2));
    qDebug() << "btree flags:" << QString::number(btreeflags, 16);
    btreekeycount = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 36, 4));
    uint16_t btreetocoff = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 40, 2));
    uint16_t btreetoclen = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 42, 2));
    uint16_t bfreeoff = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 44, 2));
    uint16_t bfreelen = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 46, 2));
    uint16_t btreekeyoff = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 48, 2));
    uint16_t btreekeylen = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 50, 2));
    // FOR OMAP B-TREE, THE KEY AND VALUES ARE SET AND THE KEY'S START AFTER THE TOC AND THE VALUES START AT THE END DEPENDING
    // ON WHETHER THERE IS A BTREE_INFO_T
    //uint16_t btreevaloff = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 52, 2));
    //uint16_t btreevallen = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 54, 2));
    uint64_t keyoffset = 0;
    //QString keylayout = "";
    qDebug() << "btree count:" << btreekeycount;
    uint64_t keyoffstart = curstartsector*512 + blocksize * omapbtreeoid + 56 + btreetocoff + btreetoclen;
    uint64_t valoffstart = blocksize - btreekeycount * 16;
    if(btreeflags & 0x0002)
    {
        qDebug() << "btree is leaf node";
        valoffstart = valoffstart - 40;
    }
    qDebug() << "keyoffstart:" << keyoffstart << "valoffstart:" << valoffstart;
    qDebug() << "valueoffstart:" << keyoffstart + bfreeoff + bfreelen;
    for(uint i=0; i < btreekeycount; i++)
    {
        //keylist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(keyoffstart + i*16, 8)));
        //xidlist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(keyoffstart + i*16 + 8, 8)));
        //vallist.prepend(qFromLittleEndian<uint64_t>(curimg->ReadContent(valoffstart + i*16 + 8, 8)));
        keylist.append(qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 56 + btreetocoff + btreetoclen + btreekeyoff + btreekeylen + i*16, 8)));
        vallist.prepend(qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 56 + btreetocoff + btreetoclen + i*16 + bfreeoff + bfreelen + 8, 8)));
        sizlist.prepend(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + blocksize * omapbtreeoid + 56 + btreetocoff + btreetoclen + i*16 + bfreeoff + bfreelen + 4, 4)));
    }
    qDebug() << "keylist:" << keylist << "xidlist:" << xidlist << "vallist:" << vallist;
    qDebug() << "roottreeoid:" << roottreeoid << "keylist count:" << keylist.count();
    //for(int i=0 ; i < keylist.count(); i++)
    //{
    //    if(roottreeoid == keylist.at(i))
    //    {
    //        keyoffset = vallist.at(i) * blocksize + curstartsector*512;
            //keylayout = QString::number(vallist.at(i) * blocksize + curstartsector*512) + "," + QString::number(sizlist.at(i)) + ";";
    //        break;
    //    }
    //}
    //return keyoffset;
    //return keylayout;
    */
    //return 0;
//}

void ParseApfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, int volid, uint32_t blocksize, QString dirlayout)
{
    qDebug() << curimg->MountPath() << "curstartsector:" << curstartsector << "ptreecnt:" << ptreecnt << "volid:" << volid << "blocksize:" << blocksize << "dirlayout:" << dirlayout;
}
/*
qulonglong ParseHfsPlusDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    quint64 inodecnt = 0;
    //quint64 parinode = 0;
    QString layout = "";
    QHash<QString, QVariant> nodedata;
    QFile fileprop;
    QTextStream out;
    QString parentstr = "";
    uint32_t clustersize = 0;
    uint32_t curstartblock = 0;
    uint32_t curblockcount = 0;
    // ALLOCATION FILE
    quint64 allocsize = 0;
    uint32_t alloctotalblocks = 0;
    QString allocextstartblockarray = "";
    QString allocextblockcntarray = "";
    // EXTENTS OVERFLOW FILE
    quint64 extentsize = 0;
    uint32_t extenttotalblocks = 0;
    QString extentextstartblockarray = "";
    QString extentextblockcntarray = "";
    // CATALOG FILE
    quint64 catalogsize = 0;
    uint32_t catalogtotalblocks = 0;
    QString catalogextstartblockarray = "";
    QString catalogextblockcntarray = "";
    // ATTRIBUTES FILE
    quint64 attrsize = 0;
    uint32_t attrtotalblocks = 0;
    QString attrextstartblockarray = "";
    QString attrextblockcntarray = "";
    // STARTUP FILE
    quint64 startsize = 0;
    uint32_t starttotalblocks = 0;
    QString startextstartblockarray = "";
    QString startextblockcntarray = "";
    // let's figure out how to parse the root directory...
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Cluster Size|"))
                clustersize = line.split("|").at(1).toUInt();
            // ALLOCATION FILE INFORMATION
            if(line.startsWith("Allocation Logical Size|"))
                allocsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Allocation Total Blocks|"))
                alloctotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Allocation Extents Start Block Array|"))
                allocextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Allocation Extents Block Count Array|"))
                allocextblockcntarray = line.split("|").at(1);
            // EXTENTS OVERFLOW FILE INFORMATION
            if(line.startsWith("Extents Overflow Logical Size|"))
                extentsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Extents Overflow Total Blocks|"))
                extenttotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Extents Overflow Extents Start Block Array|"))
                extentextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Extents Overflow Extents Block Count Array|"))
                extentextblockcntarray = line.split("|").at(1);
            // CATALOG FILE INFORMATION
            if(line.startsWith("Catalog Logical Size|"))
                catalogsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Catalog Total Blocks|"))
                catalogtotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Catalog Extents Start Block Array|"))
                catalogextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Catalog Extents Block Count Array|"))
                catalogextblockcntarray = line.split("|").at(1);
            // ATTRIBUTES FILE INFORMATION
            if(line.startsWith("Attributes Logical Size|"))
                attrsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Attributes Total Blocks|"))
                attrtotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Attributes Extents Start Block Array|"))
                attrextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Attributes Extents Block Count Array|"))
                attrextblockcntarray = line.split("|").at(1);
            // STARTUP FILE INFORMATION
            if(line.startsWith("Startup Logical Size|"))
                startsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Startup Total Blocks|"))
                starttotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Startup Extents Start Block Array|"))
                startextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Startup Extents Block Count Array|"))
                startextblockcntarray = line.split("|").at(1);
        }
        propfile.close();
    }
    // CREATE THE RESPECTIVE RESERVED FILES HERE
    // ALLOCATION FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|6|Catalog node id for the allocation file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * alloctotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(allocsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    for(int i=0; i < allocextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = allocextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = allocextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$AllocationFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", allocsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Allocation File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // EXTENTS OVERFLOW FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|3|Catalog node id for the extents overflow file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * extenttotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(extentsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < extentextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = extentextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = extentextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$ExtentsOverflowFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", extentsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Extents Overflow File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // CATALOG FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|4|Catalog node id for the catalog file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * catalogtotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(catalogsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < catalogextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = catalogextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = catalogextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$CatalogFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", catalogsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Catalog File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // ATTRIBUTES FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|8|Catalog node id for the attributes file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * attrtotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(attrsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < attrextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = attrextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = attrextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$AttributesFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", attrsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Attributes File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // STARTUP FILE
    fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    out << "Catalog Node ID|7|Catalog node id for the startup file." << Qt::endl;
    out << "Parent Catalog Node ID|2|File's parent catalog node ID value." << Qt::endl;
    out << "Physical Size|" << QString::number(clustersize * starttotalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
    out << "Logical Size|" << QString::number(startsize) << "|Size in Bytes for the file." << Qt::endl;
    out << "Mode|r---------|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
    out << "uid / gid|0 / 0|User ID and Group ID." << Qt::endl;
    out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
    curstartblock = 0;
    curblockcount = 0;
    layout = "";
    for(int i=0; i < startextstartblockarray.split(",", Qt::SkipEmptyParts).count(); i++) // build layout
    {
        curstartblock = startextstartblockarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        curblockcount = startextblockcntarray.split(",", Qt::SkipEmptyParts).at(i).toUInt();
        layout += QString::number(curstartsector*512 + clustersize * curstartblock) + "," + QString::number(clustersize * curblockcount) + ";";
    }
    out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
    out.flush();
    fileprop.close();
    nodedata.clear();
    nodedata.insert("name", QByteArray(QString("$StartupFile").toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(QString("/").toUtf8()).toBase64());
    nodedata.insert("size", startsize);
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Startup File");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    //if(parinode > 0)
    //    parentstr += QString("-f" + QString::number(parinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, parentstr, 10, 0); // virtual file, not deleted
    mutex.unlock();
    listeditems.append(nodedata.value("id").toString());
    filesfound++;
    isignals->ProgUpd();
    inodecnt++;
    nodedata.clear();
    // FINISH THE RESPECTIVE RESERVED FILES HERE
    uint16_t nodesize = 0;
    quint64 curoffset = 0;
    quint64 curblocksize = 0;
    curoffset = curstartsector*512;
    QStringList catalogblocklist = catalogextstartblockarray.split(",", Qt::SkipEmptyParts);
    QStringList catalogblockcntlist = catalogextblockcntarray.split(",", Qt::SkipEmptyParts);
    // catalog file can be split, so i need to parse the 
    for(int i=0; i < catalogblocklist.count(); i++)
    {
        curblocksize = catalogblockcntlist.at(i).toUInt() * clustersize;
        //qDebug() << "curblocksize:" << curblocksize;
        uint32_t curpos = 0;
        // parse the catalog file by pieces here.
        if(i == 0) // header node
        {
            curoffset += catalogblocklist.at(i).toUInt() * clustersize; // curoffset at the first catalog block offset
            nodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 32, 2));
            //qDebug() << "nodesize:" << nodesize;
            // NEED TO JUMP TO CATALOG ROOT DIRECTORY NODE START, WHICH IS CATALOG NODE ID (CNID) 2, SO IT IS (CNID - 1) * NODESIZE
            // SO FOR THIS EXAMPLE, IT IS (2-1) * 4096, NEED TO SET THE OFFSET TO BE 
            curoffset += nodesize; // SHOULD PUT ME AT 4096, START OF THE CATALOG ROOT DIRECTORY NODE
            // READING THE NODE DESCRIPTOR IS ONLY IN THE i=0 PORTION OF THE CATALOG ROOT DIRECTORY
            // GET THE NUMBER OF RECORDS
            uint16_t recordcount = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2));
            //qDebug() << "recordcount:" << recordcount; // MIGHT BE ANOTHER WAY TO LOOP OVER THE RECORDS, RATHER THAN THE CUROFFSET + CURPOS METHOD...
            //qDebug() << curimg->ReadContent(curoffset, 14).toHex();
            //curoffset += 14; // THIS PUTS THE CUROFFSET INTO THE KEY/DATA LOOP FOR THE CATALOG LEAF NODE, SO ITERATING SHOULD WORK HERE
            curpos += 14;
        }
        // THIS IS WHERE THE KEY/DATA LOOP ITERATION SHOULD START... WHILE (CURPOS < CURBLOCKSIZE)
        //qDebug() << "curoffset:" << curoffset << "curpos:" << curpos << "curblocksize:" << curblocksize;
        QHash<uint32_t, QString> dirhash; // QHash<parentcnid, QString("inode, filename")>
        dirhash.clear();
        dirhash.insert(2, "0,2,/");
        while(curpos < curblocksize)
        {
            QString keyfilename = "";
            uint16_t curkeylength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            if(curkeylength == 0)
            {
                //qDebug() << "keylength is wrong, probably out of records...";
                break;
            }
            //qDebug() << "curkeylength:" << curkeylength; // in case i want to skip a record
            uint32_t parentcnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos, 4));
            //qDebug() << "parentcnid:" << parentcnid;
            curpos += 4;
            uint16_t keynamelength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            //qDebug() << "keynamelength:" << keynamelength;
            if(keynamelength > 0)
            {
                keyfilename = "";
                for(int j=0; j < keynamelength; j++)
                {
                    QChar tmpchar = QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + j*2, 2)));
                    if(tmpchar == '\x0')
                        keyfilename += QString("^");
                    else
                        keyfilename += QString(tmpchar);
                }
                curpos += 2*keynamelength;
                //qDebug() << "keyfilename:" << keyfilename;
            }
            //else // ALREADY SKIPPED KEYNAME LENGTH...
            //    curpos += 2;
            uint16_t datarecordtype = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            //qDebug() << "datarecordtype:" << datarecordtype;
            if(datarecordtype == 0x0001 || datarecordtype == 0x0002) // FOLDER RECORD 0x0001/FILE RECORD 0x0002 - 88 bytes/248 bytes
            {
                layout = "";
                QString reslayout = "";
                uint8_t itemtype = 0;
                uint8_t isdeleted = 0;
                uint16_t flags = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                uint32_t valence = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 2, 4));
                uint32_t cnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 6, 4));
                //qDebug() << "cnid:" << cnid;
                uint32_t createdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 10, 4));
                uint32_t contentmoddate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 14, 4));
                uint32_t attrmoddate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 18, 4));
                uint32_t accessdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 22, 4));
                uint32_t backupdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 26, 4));
                uint32_t ownerid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 30, 4));
                uint32_t groupid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 34, 4));
                uint8_t adminflags = qFromBigEndian<uint8_t>(curimg->ReadContent(curoffset + curpos + 38, 1));
                uint8_t ownerflags = qFromBigEndian<uint8_t>(curimg->ReadContent(curoffset + curpos + 39, 1));
                uint16_t filemode = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + 40, 2));
                uint32_t inodelinkraw = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 42, 4));
                quint64 logicalsize = 0;
                uint32_t clumpsize = 0;
                uint32_t totalblocks = 0;
                quint64 reslogicalsize = 0;
                uint32_t resclumpsize = 0;
                uint32_t restotalblocks = 0;
                //qDebug() << "createdate:" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(createdate));
                // 16 bytes for user information i currently don't care about
                // 16 bytes for finder information i currently don't care about
                // 4 bytes for text encoding i don't care about
                // 4 bytes reserved, so it doesn't matter
                if(datarecordtype == 0x0001) // FOLDER RECORD
                {
                    //qDebug() << "folder record";
                    dirhash.insert(cnid, QString(QString::number(inodecnt) + "," + QString::number(parentcnid) + "," + keyfilename));
                    curpos += 86;
                }
                else if(datarecordtype == 0x0002) // FILE RECORD
                {
                    logicalsize = qFromBigEndian<quint64>(curimg->ReadContent(curoffset + curpos + 86, 8));
                    clumpsize = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 94, 4));
                    totalblocks = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 98, 4));
                    //qDebug() << "logical size:" << logicalsize << "clumpsize:" << clumpsize << "totalblocks:" << totalblocks;
                    // PARSE DATA FORK
                    if(logicalsize > 0)
                    {
                        for(int j=0; j < 8; j++)
                        {
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 102 + j*8, 4)) > 0)
                                layout += QString::number(curstartsector*512 + clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 102 + j*8, 4))) + ",";
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 106 + j*8, 4)) > 0)
                                layout += QString::number(clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 106 + j*8, 4))) + ";";
                        }
                    }
                    // PARSE RESOURCE FORK
                    reslogicalsize = qFromBigEndian<quint64>(curimg->ReadContent(curoffset + curpos + 166, 8));
                    resclumpsize = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 174, 4));
                    restotalblocks = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 178, 4));
                    if(reslogicalsize > 0)
                    {
                        for(int j=0; j < 8; j++)
                        {
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 182 + j*8, 4)) > 0)
                                reslayout += QString::number(curstartsector*512 + clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 182 + j*8, 4))) + ",";
                            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 186 + j*8, 4)) > 0)
                                reslayout += QString::number(clustersize * qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 186 + j*8, 4))) + ";";
                        }
                        //qDebug() << "reslogicalsize:" << reslogicalsize;
                        //qDebug() << "restotalblocks:" << restotalblocks;
                        //qDebug() << "reslayout:" << reslayout;
                    }
                    curpos += 246;
                    //qDebug() << "layout:" << layout;
                }
                fileprop.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
                if(!fileprop.isOpen())
                    fileprop.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&fileprop);
                out << "Catalog Node ID|" << QString::number(cnid) << "|Catalog node id for the file/folder." << Qt::endl;
                out << "Parent Catalog Node ID|" << QString::number(parentcnid) << "|File's parent catalog node id value." << Qt::endl;
                QString filemodestr = "---------";
                if(filemode & 0xc000) // unix socket
		{
                    filemodestr.replace(0, 1, "s");
		    itemtype = 9;
		}
                if(filemode & 0xa000) // symbolic link
		{
                    filemodestr.replace(0, 1, "l");
		    itemtype = 12;
		}
                if(filemode & 0x6000) // block device
		{
                    filemodestr.replace(0, 1, "b");
		    itemtype = 7;
		}
                if(filemode & 0x2000) // char device
		{
                    filemodestr.replace(0, 1, "c");
		    itemtype = 6;
		}
                if(filemode & 0x1000) // FIFO (pipe)
		{
                    filemodestr.replace(0, 1, "p");
		    itemtype = 8;
		}
                if(filemode & 0x8000) // regular file
                {
                    filemodestr.replace(0, 1, "-");
		    itemtype = 5;
		    if(isdeleted == 1)
			itemtype = 4;
                }
                else if(filemode & 0x4000) // directory
                {
                    filemodestr.replace(0, 1, "d");
		    itemtype = 3;
		    if(isdeleted == 1)
			itemtype = 2;
                }
                if(filemode & 0x100) // user read
                    filemodestr.replace(1, 1, "r");
                if(filemode & 0x080) // user write
                    filemodestr.replace(2, 1, "w");
                if(filemode & 0x040) // user execute
                    filemodestr.replace(3, 1, "x");
                if(filemode & 0x020) // group read
                    filemodestr.replace(4, 1, "r");
                if(filemode & 0x010) // group write
                    filemodestr.replace(5, 1, "w");
                if(filemode & 0x008) // group execute
                    filemodestr.replace(6, 1, "x");
                if(filemode & 0x004) // other read
                    filemodestr.replace(7, 1, "r");
                if(filemode & 0x002) // other write
                    filemodestr.replace(8, 1, "w");
                if(filemode & 0x001) // other execute
                    filemodestr.replace(9, 1, "x");
                //qDebug() << "filemodestr:" << filemodestr;
                out << "Mode|" << filemodestr << "|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
                out << "uid / gid|" << QString::number(ownerid) << " / " << QString::number(groupid) << "|User ID and Group ID." << Qt::endl;
                out << "File Attributes|Allocated|Attributes list for the file." << Qt::endl;
                out << "Backup Date|" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(backupdate)) << "|Date time the file/folder was last backed up." << Qt::endl;
                //qDebug() << "createdate:" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(createdate));
                out << "Physical Size|" << QString::number(clustersize * totalblocks) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
                out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
                out << "Layout|" << layout << "|File layout in offset,size; format." << Qt::endl;
                out.flush();
                fileprop.close();
                nodedata.clear();
                nodedata.insert("name", QByteArray(keyfilename.toUtf8()).toBase64());
                // FIX PATH HERE BASED ON PARENTCNID AND DIRHASH
                QString parentinfo = dirhash.value(parentcnid);
                //qDebug() << "parentinfo:" << parentinfo;
                // THE PATH VARIABLE ISN'T QUITE RIGHT... IF ITS NESTED, THIS IGNORES THE NESTING...
                // SO I NEED AN ITERATIVE PATH STRING BUILDER BASED ON CHECKING DIRHASH FOR EACH CNID...
                QString parentpath = "/";
                if(parentcnid > 2)
                    BuildPath(&parentpath, &dirhash, parentcnid);
                    //nodedata.insert("path", QByteArray(QString("/" + parentinfo.split(",").at(1) + "/").toUtf8()).toBase64());
                nodedata.insert("path", QByteArray(parentpath.toUtf8()).toBase64());
                nodedata.insert("size", logicalsize);
                nodedata.insert("create", ConvertHfsTimeToUnixTime(createdate));
                nodedata.insert("access", ConvertHfsTimeToUnixTime(accessdate));
                nodedata.insert("modify", ConvertHfsTimeToUnixTime(contentmoddate));
                nodedata.insert("status", ConvertHfsTimeToUnixTime(attrmoddate));
                if(logicalsize > 0) // Get Category/Signature
                {
                    if(itemtype == 3 && isdeleted == 0)
                    {
                        nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory");
                    }
                    else
                    {
                        QString catsig = GenerateCategorySignature(curimg, keyfilename, layout.split(";").at(0).split(",").at(0).toULongLong());
                        nodedata.insert("cat", catsig.split("/").first());
                        nodedata.insert("sig", catsig.split("/").last());
                    }
                }
                else
                {
                    if(itemtype == 3 && isdeleted == 0)
                    {
                        nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory");
                    }
                    else
                    {
                        nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Zero File");
                    }
                }
                nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
                parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
                // need a list which contains cnid, inode, filename for folders...
                if(cnid != 1 && cnid != 2)
                {
                    QString parinode = parentinfo.split(",").at(0);
                    if(parentcnid > 2)
                    {
                        parentstr += QString("-f" + parinode);
                    }
                    //qDebug() << parentcnid << "parentcnid info:" << dirhash.value(parentcnid);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, parentstr, itemtype, 0); // virtual file, not deleted
                    mutex.unlock();
                    listeditems.append(nodedata.value("id").toString());
                    filesfound++;
                    isignals->ProgUpd();
                    inodecnt++;
                }
                nodedata.clear();
                if(reslogicalsize > 0) // resource fork exists
                {
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt-1));
                    nodedata.insert("name", QByteArray(QString("$RESOURCE_FORK").toUtf8()).toBase64());
                    nodedata.insert("path", QByteArray(QString(parentpath + keyfilename + "/").toUtf8()).toBase64());
                    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
                    nodedata.insert("size", reslogicalsize);
                    nodedata.insert("cat", "System File");
                    nodedata.insert("sig", "Resource Fork");
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, parentstr, 10, 0);
                    mutex.unlock();
                    listeditems.append(nodedata.value("id").toString());
                    filesfound++;
                    isignals->ProgUpd();
                    QFile adsprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
                    QTextStream adsout;
                    if(!adsprop.isOpen())
                        adsprop.open(QIODevice::Append | QIODevice::Text);
                    if(adsprop.isOpen())
                    {
                        adsout.setDevice(&adsprop);
                        adsout << "Physical Size|" << QString::number(clustersize * restotalblocks) << "|Physical size for the resource fork in bytes." << Qt::endl;
                        adsout << "Logical Size|" << QString::number(reslogicalsize) << "|Logical size of the resource fork in bytes." << Qt::endl;
                        adsout << "Layout|" << reslayout + "|Layout of the resource fork in bytes as offset,size;offset,size;." << Qt::endl;
                        adsout.flush();
                        adsprop.close();
                    }
                    inodecnt++;
                }
            }
            else if(datarecordtype == 0x0003 || datarecordtype == 0x0004) // FOLDER THREAD RECORD 0x03/ FILE THREAD RECORD 0x04 - SKIP
            {
                // keylength should be 6 bytes...
                // data length should another 6 bytes
                curpos += 6;
                uint16_t datanodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                curpos += 2;
                //qDebug() << "datanodesize:" << datanodesize;
                curpos += 2*datanodesize;
                //break;
            }
            else // SOMETHING IS WRONG PARSING CATALOG FILE
            {
                qDebug() << "error parsing catalog file!";
                break;
            }
        }
    }
}

void BuildPath(QString* path, QHash<uint32_t, QString>* dirhash, quint64 curparcnid)
{
    while(curparcnid > 2)
    {
        QString parentinfo = dirhash->value(curparcnid);
        curparcnid = parentinfo.split(",").at(1).toULongLong();
        path->prepend("/" + parentinfo.split(",").at(2));
    }
}
*/
