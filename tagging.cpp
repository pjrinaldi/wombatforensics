#include "tagging.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void AddTag(QString artifact, QString tagstring)
{
    QFile artifactfile;
    QStringList artifactlist;
    artifactfile.setFileName(wombatvariable.tmpmntpath + artifact);
    if(!FileExists(QString(wombatvariable.tmpmntpath + artifact).toStdString()))
    {
        artifactfile.open(QIODevice::WriteOnly | QIODevice::Text);
        artifactfile.close();
    }
    artifactlist.clear();
    artifactfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(artifactfile.isOpen())
        artifactlist = QString(artifactfile.readLine()).split(",", Qt::SkipEmptyParts);
    artifactfile.close();
    /*
    QStringList bookitemlist;
    bookitemlist.clear();
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        bookitemlist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();

            bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
            if(bookmarkfile.isOpen())
                bookmarkfile.write(tmpstr.toStdString().c_str());
            bookmarkfile.close();

    QString selectedtag = ui->listWidget->currentItem()->text();
    QString tmpstr = "";
    for(int i = 0; i < taglist.count(); i++)
    {
        if(taglist.at(i).contains(selectedtag))
            taglist.removeAt(i);
    }
    for(int i=0; i < taglist.count(); i++)
        tmpstr += taglist.at(i) + ",";
    bookmarkfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
        bookmarkfile.write(tmpstr.toStdString().c_str());
    bookmarkfile.close();
    */
    qDebug() << "artifact:" << artifact << "tagstring:" << tagstring;
}
void RemTag(QString artifact, QString tagstring)
{
    qDebug() << "artifact:" << artifact << "tagstring:" << tagstring;
}
