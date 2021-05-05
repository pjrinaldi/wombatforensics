#include "reporting.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

int UpdateBookmarkItems(QString tagname)
{
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QStringList bookitemlist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    if(bookitemlist.indexOf(tagname) == -1)
    {
        bookitemlist.append(tagname);
        bookitemlist.removeDuplicates();
        bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&bookmarkfile);
        //qDebug() << "bookitemlist count:" << bookitemlist.count();
        for(int i=0; i < bookitemlist.count(); i++)
        {
            out << bookitemlist.at(i) << ",";
        }
        bookmarkfile.close();
        return bookitemlist.count() - 1;
    }
    else
        return -15;
}

void UpdateEvidenceList()
{
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString("*-e*")), QDir::Dirs | QDir::NoSymLinks);
    existingevidence.clear();
    existingevid.clear();
    //QStringList tmplist;
    for(int i=0; i < evidfiles.count(); i++)
    {
        //ForensicImage* tmpimage = new ForensicImage(evidfiles.at(i).split("-e" + QString::number(i)).first());
        //existingevid.append(tmpimage);
        existingevidence.append(evidfiles.at(i).split("-e" + QString::number(i)).first());
        //tmplist.clear();
        /*
        QFile evidfile(wombatvariable.tmpmntpath + evidfiles.at(i) + "/stat");
        evidfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(evidfile.isOpen())
            tmplist = QString(evidfile.readLine()).split(",");
        existingevidence.append(tmplist.at(3));
        evidfile.close();
        */
    }
}

void AddTagItem(int tagid, QString tagname)
{
    QString origstr = "";
    QString tagstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttag-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttag-->";
    QString curcontent = beginsplit.last().split("<!--lasttag-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttag-->").last();
    postcontent = "<!--lasttag-->" + postcontent;
    QStringList taglist = curcontent.split("\n", Qt::SkipEmptyParts);
    tagstr = "";
    bool tagexists = false;
    if(taglist.count() > 0)
    {
        for(int i=0; i < taglist.count(); i++)
        {
            if(taglist.at(i).contains(tagname))
                tagexists = true;
        }
        if(!tagexists)
            curcontent += "<div id='t" + QString::number(tagid) + "'><h3>" + tagname + "</h3><br/><br/><table><tr><!--firstfile--><!--lastfile--></tr></table></div><br/>\n";
    }
    else
        curcontent += "<div id='t" + QString::number(tagid) + "'><h3>" + tagname + "</h3><br/><br/><table><tr><!--firstfile--><!--lastfile--></tr></table></div><br/>\n";
    curcontent += tagstr;
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + curcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void AddEvidItem(QString content)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firstevid-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firstevid-->";
    QString curcontent = beginsplit.last().split("<!--lastevid-->").first();
    QString postcontent = beginsplit.last().split("<!--lastevid-->").last();
    postcontent = "<!--lastevid-->" + postcontent;
    QStringList evidlist = curcontent.split("\n", Qt::SkipEmptyParts);
    bool elinkexists = false;
    if(evidlist.count() > 0)
    {
        for(int i=0; i < evidlist.count(); i++)
        {
            if(evidlist.at(i).contains(content))
                elinkexists = true;
        }
        if(!elinkexists)
            curcontent += content;
    }
    else
        curcontent += content;
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + curcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void AddELinkItem(int evidid, QString evidname)
{
    QString origstr = "";
    QString linkstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firstelink-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firstelink-->";
    QString curcontent = beginsplit.last().split("<!--lastelink-->").first();
    QString postcontent = beginsplit.last().split("<!--lastelink-->").last();
    postcontent = "<!--lastelink-->" + postcontent;
    QStringList linklist = curcontent.split("\n", Qt::SkipEmptyParts);
    linkstr = "";
    bool elinkexists = false;
    if(linklist.count() > 0)
    {
        for(int i=0; i < linklist.count(); i++)
        {
            if(linklist.at(i).contains(evidname))
                elinkexists = true;
        }
        if(!elinkexists)
            linkstr += "<span id='l" + QString::number(evidid) + "'><a href='#e" + QString::number(evidid) + "'>" + evidname + "</a></span><br/>\n";
    }
    else
        linkstr = "<span id='l0'><a href='#e0'>" + evidname + "</a></span><br/>\n";
    curcontent += linkstr;
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + curcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void AddTLinkItem(int tagid, QString tagname)
{
    QString origstr = "";
    QString linkstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttlink-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttlink-->";
    QString curcontent = beginsplit.last().split("<!--lasttlink-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttlink-->").last();
    postcontent = "<!--lasttlink-->" + postcontent;
    QStringList linklist = curcontent.split("\n", Qt::SkipEmptyParts);
    linkstr = "";
    if(tagid == linklist.count())
        linkstr += "<span id='l" + QString::number(tagid) + "'><a href='#t" + QString::number(tagid) + "'>" + tagname + "</a></span><br/>\n";
    curcontent += linkstr;
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + curcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void AddFileItem(QString tagname, QString content)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttag-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttag-->";
    QString curcontent = beginsplit.last().split("<!--lasttag-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttag-->").last();
    postcontent = "<!--lasttag-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts); // tag list
    QString midstr = "";
    midstr += precontent;
    if(curlist.count() > 0)
    {
        for(int i=0; i < curlist.count(); i++)
        {
            if(curlist.at(i).contains(tagname))
            {
                QString origsub = curlist.at(i);
                QStringList subsplit = origsub.split("<!--lastfile-->", Qt::SkipEmptyParts);
                midstr += subsplit.first();
                midstr += content;
                midstr += "<!--lastfile-->";
                midstr += subsplit.last() + "\n";
            }
            else
                midstr += curlist.at(i) + "\n";
        }
    }
    midstr += postcontent;
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(midstr.toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void RemoveFileItem(QString fileid)
{
    QString itemstr = "<td class='fitem' id='" + fileid + "'>";
    QString origstr = "";
    QString postcontent = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QString finalstr = "";
    QStringList filelist = origstr.split(itemstr, Qt::SkipEmptyParts);
    if(filelist.count() > 1)
    {
        finalstr += filelist.first();
        QString laststr = filelist.last();
        int tagend = laststr.indexOf("</table></td>");
        tagend = tagend + 13;
        finalstr += laststr.remove(0, tagend);
        if(!previewfile.isOpen())
            previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(previewfile.isOpen())
            previewfile.write(finalstr.toStdString().c_str());
        previewfile.close();
        isignals->ActivateReload();
    }
}

void UpdateTLinkItem(int tagid, QString oldname, QString newname)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttlink-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttlink-->";
    QString curcontent = beginsplit.last().split("<!--lasttlink-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttlink-->").last();
    postcontent = "<!--lasttlink-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts);
    QString updatedcontent = "";
    if(curlist.count() > 0)
    {
        for(int i=0; i < curlist.count(); i++)
        {
            if(i == tagid)
            {
                int startindex = curlist.at(tagid).indexOf(oldname);
                int oldlength = oldname.count();
                QString tmpstr = curlist.at(tagid);
                updatedcontent += tmpstr.replace(startindex, oldlength, newname) + "\n";
            }
            else
                updatedcontent += curlist.at(i) + "\n";
        }
    }
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + updatedcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void UpdateTagItem(int tagid, QString oldname, QString newname)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttag-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttag-->";
    QString curcontent = beginsplit.last().split("<!--lasttag-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttag-->").last();
    postcontent = "<!--lasttag-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts);
    QString updatedcontent = "";
    if(curlist.count() > 0)
    {
        for(int i=0; i < curlist.count(); i++)
        {
            if(i == tagid)
            {
                int startindex = curlist.at(tagid).indexOf(oldname);
                int oldlength = oldname.count();
                QString tmpstr = curlist.at(tagid);
                updatedcontent += tmpstr.replace(startindex, oldlength, newname) + "\n";
            }
            else
                updatedcontent += curlist.at(i) + "\n";
        }
    }
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + updatedcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void RemoveTLinkItem(QString tagname)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttlink-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttlink-->";
    QString curcontent = beginsplit.last().split("<!--lasttlink-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttlink-->").last();
    postcontent = "<!--lasttlink-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts);
    QString updatedcontent = "";
    for(int i=0; i < curlist.count(); i++)
    {
        if(!curlist.at(i).contains(tagname))
            updatedcontent += curlist.at(i) + "\n";
    }
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + updatedcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void RemoveELinkItem(QString evidname)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firstelink-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firstelink-->";
    QString curcontent = beginsplit.last().split("<!--lastelink-->").first();
    QString postcontent = beginsplit.last().split("<!--lastelink-->").last();
    postcontent = "<!--lastelink-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts);
    QString updatedcontent = "";
    for(int i=0; i < curlist.count(); i++)
    {
        if(!curlist.at(i).contains(evidname))
            updatedcontent += curlist.at(i) + "\n";
    }
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + updatedcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void RemoveEvidItem(QString evidname)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firstevid-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firstevid-->";
    QString curcontent = beginsplit.last().split("<!--lastevid-->").first();
    QString postcontent = beginsplit.last().split("<!--lastevid-->").last();
    postcontent = "<!--lastevid-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts);
    QString updatedcontent = "";
    for(int i=0; i < curlist.count(); i++)
    {
        if(!curlist.at(i).contains(evidname))
            updatedcontent += curlist.at(i) + "\n";
    }
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + updatedcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}

void RemoveTagItem(QString tagname)
{
    QString origstr = "";
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(previewfile.isOpen())
        origstr = previewfile.readAll();
    previewfile.close();
    QStringList beginsplit = origstr.split("<!--firsttag-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firsttag-->";
    QString curcontent = beginsplit.last().split("<!--lasttag-->").first();
    QString postcontent = beginsplit.last().split("<!--lasttag-->").last();
    postcontent = "<!--lasttag-->" + postcontent;
    QStringList curlist = curcontent.split("\n", Qt::SkipEmptyParts);
    QString updatedcontent = "";
    for(int i=0; i < curlist.count(); i++)
    {
        if(!curlist.at(i).contains(tagname))
            updatedcontent += curlist.at(i) + "\n";
    }
    if(!previewfile.isOpen())
        previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
        previewfile.write(QString(precontent + updatedcontent + postcontent).toStdString().c_str());
    previewfile.close();
    isignals->ActivateReload();
}
// should be able to remove
void InitializeTaggedList(void)
{
    taggedhash.clear();
    QString tmpstr = "";
    QFile hashfile(wombatvariable.tmpmntpath + "taggedlist");
    if(hashfile.exists())
    {
        if(!hashfile.isOpen())
            hashfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(hashfile.isOpen())
            tmpstr = hashfile.readLine();
        QStringList hlist = tmpstr.split(",", Qt::SkipEmptyParts);
        for(int i=0; i < hlist.count(); i++)
            taggedhash.insert(hlist.at(i).split("|", Qt::SkipEmptyParts).at(0), hlist.at(i).split("|", Qt::SkipEmptyParts).at(1));
        hashfile.close();
    }
    QHashIterator<QString, QString> i(taggedhash);
    while(i.hasNext())
    {
        i.next();
        treenodemodel->UpdateNode(i.key(), 10, i.value());
    }
}
// should be able to remove
void SaveTaggedList(void)
{
    QFile hfile(wombatvariable.tmpmntpath + "taggedlist");
    if(!hfile.isOpen())
        hfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(hfile.isOpen())
    {
        QHashIterator<QString, QString> i(taggedhash);
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

