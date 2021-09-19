#include "opencase.h"

// Copyright 2013-2021 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

// NEED TO FIX THIS TO ACCOUNT FOR THE MOVEABLE COLUMNS
void LoadTreeModel(void)
{
    QStringList treelist;
    treelist.clear();
    QFile treefile(wombatvariable.tmpmntpath + "treemodel");
    if(!treefile.isOpen())
        treefile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(treefile.isOpen())
    {
        QTextStream in(&treefile);
        while(!in.atEnd())
        {
            treelist.append(in.readLine());
            //qDebug() << treefile.readLine();
        }
        treefile.close();
    }
    //qDebug() << "treelist count:" << treelist.count();
    for(int i=0; i < treelist.count(); i++)
    {
        QString parentstr = "";
        QStringList nodelist = treelist.at(i).split(",");
        qDebug() << "nodelist:" << nodelist << nodelist.count();
        //qDebug() << nodelist.at(11);
        QHash<QString, QVariant> nodedata;
        nodedata.clear();
        nodedata.insert("name", nodelist.at(0));
        nodedata.insert("path", nodelist.at(1));
        nodedata.insert("size", nodelist.at(2).toLongLong());
        nodedata.insert("create", nodelist.at(3).toInt());
        nodedata.insert("access", nodelist.at(4).toInt());
        nodedata.insert("modify", nodelist.at(5).toInt());
        nodedata.insert("status", nodelist.at(6).toInt());
        nodedata.insert("hash", nodelist.at(7));
        nodedata.insert("cat", nodelist.at(8));
        nodedata.insert("sig", nodelist.at(9));
        nodedata.insert("tag", nodelist.at(10));
        nodedata.insert("id", nodelist.at(11));
        if(nodelist.at(14).isEmpty())
            parentstr = "-1";
        else
            parentstr = nodelist.at(14);
        //qDebug() << nodelist.at(11) << parentstr;
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, nodelist.at(12).toInt(), nodelist.at(13).toInt());
        mutex.unlock();
        if(nodelist.at(11).split("-").count() == 2)
        {
            partitionlist.append(nodelist.at(11) + ": " + nodelist.at(0));
	    //partitionlist.append(plist.at(10) + ": " + QString(GetFileSystemLabel(fsinfo)) + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
        }
        if(nodelist.at(11).split("-").count() == 3)
        {
            listeditems.append(nodelist.at(11));
            filesfound++;
            isignals->ProgUpd();
        }
        //listeditems.append(treeout.at(11));
    }
}
