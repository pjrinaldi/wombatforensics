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
    //qDebug() << "nodelist:" << treelist.at(0) << treelist.at(0).split(",").count();
    QStringList colorder = treelist.at(0).split(";").first().split(",");
    qDebug() << "colorder:" << colorder << colorder.count();
    treenodemodel->SetColumnOrder(colorder);
    // with columnorder set, i should be able to call nodedata.insert using GetColumnIndex("")
    for(int i=1; i < treelist.count(); i++)
    {
        QString parentstr = "";
        QStringList nodelist = treelist.at(i).split(";").first().split(",");
        qDebug() << "nodelist:" << nodelist << nodelist.count();
        //qDebug() << nodelist.at(11);
        QHash<QString, QVariant> nodedata;
        nodedata.clear();
        nodedata.insert("name", nodelist.at(treenodemodel->GetColumnIndex("name")));
        nodedata.insert("path", nodelist.at(treenodemodel->GetColumnIndex("path")));
        nodedata.insert("size", nodelist.at(treenodemodel->GetColumnIndex("size")).toLongLong());
        nodedata.insert("create", nodelist.at(treenodemodel->GetColumnIndex("create")).toInt());
        nodedata.insert("access", nodelist.at(treenodemodel->GetColumnIndex("access")).toInt());
        nodedata.insert("modify", nodelist.at(treenodemodel->GetColumnIndex("modify")).toInt());
        nodedata.insert("status", nodelist.at(treenodemodel->GetColumnIndex("status")).toInt());
        nodedata.insert("hash", nodelist.at(treenodemodel->GetColumnIndex("hash")));
        nodedata.insert("cat", nodelist.at(treenodemodel->GetColumnIndex("cat")));
        nodedata.insert("sig", nodelist.at(treenodemodel->GetColumnIndex("sig")));
        nodedata.insert("tag", nodelist.at(treenodemodel->GetColumnIndex("tag")));
        nodedata.insert("id", nodelist.at(treenodemodel->GetColumnIndex("id")));
	nodedata.insert("match", nodelist.at(treenodemodel->GetColumnIndex("match")));
	if(treelist.at(i).split(";").at(3).isEmpty())
	    parentstr = "-1";
	else
	    parentstr = treelist.at(i).split(";").at(3);
	qDebug() << "parentstr:" << treelist.at(i).split(";").at(3);
	mutex.lock();
	treenodemodel->AddNode(nodedata, parentstr, treelist.at(i).split(";").at(1).toInt(), treelist.at(i).split(";").at(2).toInt());
	mutex.unlock();
	//if(nodedata.value("id").toString().split("-").count() == 2)
	//    partitionlist.append(nodedata.value("id").toString() + ": " + nodedata.value("name").toString());
	if(nodedata.value("id").toString().split("-").count() == 3)
	{
	    listeditems.append(nodedata.value("id").toString());
	    filesfound++;
	    isignals->ProgUpd();
	}
	/*
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
        */
        //listeditems.append(treeout.at(11));
    }
}
