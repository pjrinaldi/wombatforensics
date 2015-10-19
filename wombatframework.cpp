#include "wombatframework.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatFramework::WombatFramework(WombatVariable* wombatvariable)
{
    wombatptr = wombatvariable;
}
WombatFramework::~WombatFramework()
{
}

void WombatFramework::OpenEvidenceImage() // open current evidence image
{
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatptr->evidenceobject.fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
    }
    wombatptr->evidenceobject.imageinfo = tsk_img_open(wombatptr->evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(wombatptr->evidenceobject.imageinfo == NULL)
    {
        LogMessage("Evidence Image access failed");
        errorcount++;
    }
    free(images);
}

void WombatFramework::OpenVolumeSystem() // open current volume system
{
    wombatptr->evidenceobject.volinfo = tsk_vs_open(wombatptr->evidenceobject.imageinfo, 0, TSK_VS_TYPE_DETECT);
}

void WombatFramework::GetVolumeSystemName() // get the volume system name
{
    if(wombatptr->evidenceobject.volinfo == NULL)
        wombatptr->currentvolumename = "Dummy Volume";
    else
        wombatptr->currentvolumename = QString::fromUtf8(tsk_vs_type_todesc(wombatptr->evidenceobject.volinfo->vstype));
}

void WombatFramework::OpenPartitions() // open the partitions in the volume
{
    wombatptr->evidenceobject.fsinfovector.clear();
    // CHECK IF VOLUME EXISTS, IF NOT THEN SHOULD GET FS FROM IMG INSTEAD OF VOL.
    if(wombatptr->evidenceobject.volinfo == NULL)
    {
        wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_img(wombatptr->evidenceobject.imageinfo, 0, TSK_FS_TYPE_DETECT));
    }
    else
    {
        if(wombatptr->evidenceobject.volinfo->part_count > 0)
        {
            for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
            {
                if(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i)->flags == 0x02) // if its an unallocated partition
                {
                }
                wombatptr->evidenceobject.partinfovector.push_back(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i));
            }
        }
    }
}

void WombatFramework::OpenFiles() // open the files and add to file info vector
{
    uint8_t walkreturn;
    int walkflags = TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE;
    for(uint i=0; i < wombatptr->evidenceobject.fsinfovector.size(); i++)
    {
        currentfilesystemid = wombatptr->evidenceobject.fsidvector[i];
            blockstring = "";
            walkreturn = tsk_fs_dir_walk(wombatptr->evidenceobject.fsinfovector[i], wombatptr->evidenceobject.fsinfovector[i]->root_inum, (TSK_FS_DIR_WALK_FLAG_ENUM)walkflags, FileEntries, NULL);
    }
    if(walkreturn == 1)
    {
        LogMessage("Issues with traversing the file structure were encountered");
        errorcount++;
    }
    if(fcasedb.isValid() && fcasedb.isOpen())
    {
        if(fcasedb.transaction())
        {
            QSqlQuery fquery(fcasedb);
            fquery.prepare("INSERT INTO data(objecttype, type, name, parentid, fullpath, atime, ctime, crtime, mtime, size, address, parimgid, parfsid, blockaddress, filemime, filesignature, md5, mftattrid) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, '', '', '', '', ?);");
            for(int i=0; i < filedatavector.count(); i++)
            {
                if(filedatavector.at(i).mftattrid == 0)
                    fquery.bindValue(0, 5);
                else
                    fquery.bindValue(0, 6);
                fquery.bindValue(1, filedatavector.at(i).type);
                fquery.bindValue(2, filedatavector.at(i).name);
                fquery.bindValue(3, filedatavector.at(i).paraddr);
                fquery.bindValue(4, filedatavector.at(i).path);
                fquery.bindValue(5, filedatavector.at(i).atime);
                fquery.bindValue(6, filedatavector.at(i).ctime);
                fquery.bindValue(7, filedatavector.at(i).crtime);
                fquery.bindValue(8, filedatavector.at(i).mtime);
                fquery.bindValue(9, filedatavector.at(i).size);
                fquery.bindValue(10, filedatavector.at(i).addr);
                fquery.bindValue(11, filedatavector.at(i).evid);
                fquery.bindValue(12, filedatavector.at(i).fsid);
                fquery.bindValue(13, filedatavector.at(i).mftattrid);
                fquery.exec();
            }
            fcasedb.commit();
            fquery.finish();
            filedatavector.clear();
        }
    }
}

void WombatFramework::CloseInfoStructures() // close all open info structures
{
    for(int j=0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        for(uint i=0; i < wombatptr->evidenceobjectvector[j].fsinfovector.size(); i++)
        {
            if(wombatptr->evidenceobjectvector[j].fsinfovector[i] != NULL)
            {
                tsk_fs_close(wombatptr->evidenceobjectvector[j].fsinfovector[i]);
                wombatptr->evidenceobjectvector[j].fsinfovector[i] = NULL;
            }
        }
        wombatptr->evidenceobjectvector[j].fsinfovector.clear();
        for(uint i=0; i < wombatptr->evidenceobject.partinfovector.size(); i++)
        {
            wombatptr->evidenceobjectvector[j].partinfovector[i] = NULL;
        }
        wombatptr->evidenceobjectvector[j].partinfovector.clear();
        if(wombatptr->evidenceobjectvector[j].volinfo != NULL)
        {
            tsk_vs_close(wombatptr->evidenceobjectvector[j].volinfo);
            wombatptr->evidenceobjectvector[j].volinfo = NULL;
        }
        if(wombatptr->evidenceobjectvector[j].imageinfo != NULL)
        {
            tsk_img_close(wombatptr->evidenceobjectvector[j].imageinfo);
            wombatptr->evidenceobjectvector[j].imageinfo = NULL;
        }
    }
}
