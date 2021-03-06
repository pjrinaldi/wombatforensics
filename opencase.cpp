#include "opencase.h"

// Copyright 2013-2021 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

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
        //qDebug() << nodelist.at(11);
        QList<QVariant> nodedata;
        nodedata.clear();
        nodedata << nodelist.at(0) << nodelist.at(1) << nodelist.at(2).toLongLong() << nodelist.at(3).toInt() << nodelist.at(4).toInt() << nodelist.at(5).toInt() << nodelist.at(6).toInt() << nodelist.at(7) << nodelist.at(8) << nodelist.at(9) << nodelist.at(10) << nodelist.at(11);
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
