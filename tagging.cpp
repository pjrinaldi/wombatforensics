#include "tagging.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void AddTag(QString artifact, QString tagstring)
{
    QFile artifactfile;
    QStringList artifactlist;
    QString artifactstring = "";
    artifactfile.setFileName(wombatvariable.tmpmntpath + artifact + "tags");
    if(!FileExists(QString(wombatvariable.tmpmntpath + artifact + "tags").toStdString()))
    {
        artifactfile.open(QIODevice::WriteOnly | QIODevice::Text);
        artifactfile.close();
    }
    artifactlist.clear();
    artifactfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(artifactfile.isOpen())
	artifactstring = artifactfile.readLine();
    artifactfile.close();
    artifactstring += tagstring + ",";
    artifactfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(artifactfile.isOpen())
	artifactfile.write(artifactstring.toStdString().c_str());
    artifactfile.close();
    // add htmlentry to preview report.
    // write htmlvalue to tmpmntpath + "artifact/" converted tagstring |->. and \\->-
}

void RemTag(QString artifact, QString tagstring)
{
    QFile artifactfile;
    QStringList artifactlist;
    QString artifactstring = "";
    artifactfile.setFileName(wombatvariable.tmpmntpath + artifact + "tags");
    artifactlist.clear();
    artifactfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(artifactfile.isOpen())
	artifactlist = QString(artifactfile.readLine()).split(",", Qt::SkipEmptyParts);
    artifactfile.close();
    artifactlist = artifactstring.split(",", Qt::SkipEmptyParts);
    for(int i=0; i < artifactlist.count(); i++)
    {
	if(artifactlist.at(i).contains(tagstring))
	    artifactlist.removeAt(i);
    }
    for(int i=0; i < artifactlist.count(); i++)
	artifactstring += artifactlist.at(i) + ",";
    artifactfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(artifactfile.isOpen())
	artifactfile.write(artifactstring.toStdString().c_str());
    artifactfile.close();
    // convert tagstring to filename and remove file from tmpmntpath + "/artifact/filename"
}

void CreateArtifactFile(QString artifact, QString idkeyvalue, QString htmlcontent)
{
    QDir pdir;
    pdir.mkpath(wombatvariable.tmpmntpath + artifact);
    QString filepath = wombatvariable.tmpmntpath + artifact + "/" + idkeyvalue.replace("|", ".").replace("\\", "-");
    QFile artfile;
    artfile.setFileName(filepath);
    artfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(artfile.isOpen())
        artfile.write(htmlcontent.toStdString().c_str());
    artfile.close();
    //QString filename
    //qDebug() << artifact << idkeyvalue << htmlcontent;
}

/*
void WombatForensics::TagFile(QModelIndex curindex, QString tagname)
{
    if(curindex.sibling(curindex.row(), 11).data().toString().split("-").count() == 5 || curindex.sibling(curindex.row(), 11).data().toString().contains("-c"))
    {
        QTimeZone tmpzone = QTimeZone(reporttimezone);
        taggedhash.insert(curindex.sibling(curindex.row(), 11).data().toString(), tagname);
        treenodemodel->UpdateNode(curindex.sibling(curindex.row(), 11).data().toString(), 10, tagname);
        QString filestr = "<td class='fitem' id='" + curindex.sibling(curindex.row(), 11).data().toString() + "'>";
        filestr += "<table width='300px'><tr><th colspan='2'>" + curindex.sibling(curindex.row(), 0).data().toString() + "</th></tr>";
        filestr += "<tr class='odd vtop'><td class='pvalue'>File Path:</td><td class='property'><span style='word-wrap:break-word;'>" + curindex.sibling(curindex.row(), 1).data().toString() + "</span></td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>File Size:</td><td class='property'>" + curindex.sibling(curindex.row(), 2).data().toString() + " bytes</td></tr>";
        if(!curindex.sibling(curindex.row(), 3).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Created:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 3).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 4).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Accessed:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 4).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 5).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Modified:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 5).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 6).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Changed:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 6).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        if(!curindex.sibling(curindex.row(), 7).data().toString().isEmpty())
        {
            filestr += "<tr class='odd'><td class='pvalue'>";
            if(hashsum == 1)
                filestr += "MD5";
            else if(hashsum == 2)
                filestr += "SHA1";
            else if(hashsum == 4)
                filestr += "SHA256";
            filestr += " Hash:</td><td class='property'>" + curindex.sibling(curindex.row(), 7).data().toString() + "</td></tr>";
        }
        filestr += "<tr class='even'><td class='pvalue'>Category:</td><td class='property'>" + curindex.sibling(curindex.row(), 8).data().toString() + "</td></tr>";
        filestr += "<tr class='odd'><td class='pvalue'>Signature:</td><td class='property'>" + curindex.sibling(curindex.row(), 9).data().toString() + "</td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>ID:</td><td class='property'>" + curindex.sibling(curindex.row(), 11).data().toString() + "</td></tr>";
	// IF HTMLVIEWER WON'T OPEN FILE WITHOUT .HTML EXTENSION, THEN I WILL NEED TO ADD THE SHORTCUT AND OTHER HTML PARSED ARTIFACTS TO THIS IF/ELSE STATEMENT...
        if(curindex.sibling(curindex.row(), 8).data().toString().contains("Image") || curindex.sibling(curindex.row(), 8).data().toString().contains("Video"))
            filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'><img src='./thumbs/" + curindex.sibling(curindex.row(), 11).data().toString() + ".png'/></a></td></tr>";
        else
            filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'>Link</a></td></tr>";
        filestr += "</table></td>";
        RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
        AddFileItem(tagname, filestr);
        emit treenodemodel->layoutChanged(); // this resolves the issues with the add evidence not updating when you add it later
    }
    else
        qInfo() << "Can only tag files and directories, not evidence images, volumes or partitions.";
}
void WombatForensics::RemoveTag()
{
    QAction* tagaction = qobject_cast<QAction*>(sender());
    if(QString(tagaction->iconText()).contains("Selected")) // single file
    {
        QString paridstr = selectedindex.parent().sibling(selectedindex.parent().row(), 11).data().toString().split("-f").last();
        QDir eviddir = QDir(wombatvariable.tmpmntpath);
        QStringList evidfiles = eviddir.entryList(QStringList("*." + selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
        QString evidencename = evidfiles.first();
        QString estring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(0);
        QString vstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(1);
        QString pstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(2);
        QString fstring = selectedindex.sibling(selectedindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(3);
        QStringList partlist;
        partlist.clear();
        QFile partfile(wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/stat");
        partfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(partfile.isOpen())
            partlist = QString(partfile.readLine()).split(",");
        partfile.close();
        qint64 rootinum = partlist.at(3).toLongLong();
        if(paridstr.contains("-"))
            paridstr = QString::number(rootinum);
        QFile filefile;
        QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/" + fstring;
        //if(fstring.split(":").count() > 1)
        //    filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
        //else
        //    filefilepath += fstring.split(":").first();
        filefilepath += ".a" + paridstr + ".stat";
        filefile.setFileName(filefilepath);
        filefile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString tmpstr = "";
        QStringList tmplist;
        tmplist.clear();
        if(filefile.isOpen())
            tmpstr = filefile.readLine();
        filefile.close();
        RemoveFileItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
        if(tmpstr.split(",").count() > 15)
            tmplist = tmpstr.split(",", Qt::SkipEmptyParts);
        tmplist[15] = "0";
        tmpstr = "";
        for(int i = 0; i < tmplist.count(); i++)
        {
            tmpstr += tmplist.at(i);
            if(i < tmplist.count() - 1)
                tmpstr += ",";
        }
        filefile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(filefile.isOpen())
            filefile.write(tmpstr.toStdString().c_str());
        filefile.close();
        treenodemodel->UpdateNode(selectedindex.sibling(selectedindex.row(), 11).data().toString(), 10, "0");
    }
    else if(QString(tagaction->iconText()).contains("Checked")) // single file
    {
        QStringList checkeditems = GetFileLists(1);
        for(int i=0; i < checkeditems.count(); i++)
        {
            QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(checkeditems.at(i)), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
            if(indexlist.count() > 0)
            {
                QModelIndex curindex = ((QModelIndex)indexlist.first());
                QModelIndex parindex = ((QModelIndex)indexlist.first()).parent();
                QString paridstr = parindex.sibling(parindex.row(), 11).data().toString().split("-f").last();
                QDir eviddir = QDir(wombatvariable.tmpmntpath);
                QStringList evidfiles = eviddir.entryList(QStringList("*." + curindex.sibling(curindex.row(), 11).data().toString().split("-").first()), QDir::NoSymLinks | QDir::Dirs);
                QString evidencename = evidfiles.first();
                QString estring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(0);
                QString vstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(1);
                QString pstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(2);
                QString fstring = curindex.sibling(curindex.row(), 11).data().toString().split("-", Qt::SkipEmptyParts).at(3);
                QStringList partlist;
                partlist.clear();
                QFile partfile(wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/stat");
                partfile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(partfile.isOpen())
                    partlist = QString(partfile.readLine()).split(",");
                partfile.close();
                qint64 rootinum = partlist.at(3).toLongLong();
                if(paridstr.contains("-"))
                    paridstr = QString::number(rootinum);
                QFile filefile;
                QString filefilepath = wombatvariable.tmpmntpath + evidencename + "/" + vstring + "/" + pstring + "/" + fstring;
                //if(fstring.split(":").count() > 1)
                //    filefilepath += fstring.split(":").first() + "-" + fstring.split(":").last();
                //else
                //    filefilepath += fstring.split(":").first();
                filefilepath += ".a" + paridstr + ".stat";
                QString tmpstr = "";
                QStringList tmplist;
                tmplist.clear();
                filefile.setFileName(filefilepath);
                filefile.open(QIODevice::ReadOnly | QIODevice::Text);
                if(filefile.isOpen())
                    tmpstr = filefile.readLine();
                filefile.close();
                RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
                if(tmpstr.split(",").count() > 15)
                    tmplist = tmpstr.split(",", Qt::SkipEmptyParts);
                tmplist[15] = "0";
                tmpstr = "";
                for(int i = 0; i < tmplist.count(); i++)
                {
                    tmpstr += tmplist.at(i);
                    if(i < tmplist.count() - 1)
                        tmpstr += ",";
                }
                filefile.open(QIODevice::WriteOnly | QIODevice::Text);
                if(filefile.isOpen())
                    filefile.write(tmpstr.toStdString().c_str());
                filefile.close();
                treenodemodel->UpdateNode(curindex.sibling(curindex.row(), 11).data().toString(), 10, "0");
            }
        }
    }
}
*/
