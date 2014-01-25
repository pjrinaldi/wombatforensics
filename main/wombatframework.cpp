#include "wombatframework.h"

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
    for(int i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
        fprintf(stderr, "fpv: %s\n", wombatptr->evidenceobject.fullpathvector[i].c_str());
    }
    wombatptr->evidenceobject.imageinfo = tsk_img_open(wombatptr->evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    free(images);
}

void WombatFramework::OpenVolumeSystem() // open current volume system
{
    wombatptr->evidenceobject.volinfo = tsk_vs_open(wombatptr->evidenceobject.imageinfo, 0, TSK_VS_TYPE_DETECT);
    if(wombatptr->evidenceobject.volinfo == NULL)
        fprintf(stderr, "no volume, create dummy one.");
}

void WombatFramework::GetVolumeSystemName() // get the volume system name
{
    if(wombatptr->evidenceobject.volinfo == NULL)
        wombatptr->volumeobject.name = "Dummy Volume";
    else
        wombatptr->volumeobject.name = QString::fromUtf8(tsk_vs_type_todesc(wombatptr->evidenceobject.volinfo->vstype));
    wombatptr->volumeobjectvector.append(wombatptr->volumeobject); // may not need.
}

void WombatFramework::OpenPartitions() // open the partitions in the volume
{
    // MAY NEED TO CHECK IF VOLUME EXISTS, IF NOT THEN SHOULD GET FS FROM IMG INSTEAD OF VOL.
    //fprintf(stderr, "Number of Partitions: %d\n", wombatptr->evidenceobject.volinfo->part_count);
    if(wombatptr->evidenceobject.volinfo == NULL)
    {
        wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_img(wombatptr->evidenceobject.imageinfo, 0, TSK_FS_TYPE_DETECT));
    }
    else
    {
        fprintf(stderr, "partitioncount: %i\n", wombatptr->evidenceobject.volinfo->part_count);
        if(wombatptr->evidenceobject.volinfo->part_count > 0)
        {
            for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
            {
                wombatptr->evidenceobject.partinfovector.push_back(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i));
                TSK_FS_INFO* tmpfsinfo = tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT);
                if(tmpfsinfo != NULL)
                {
                    fprintf(stderr, "Not Null FS info found\n");
                    wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT));
                }
                else
                {
                    fprintf(stderr, "NULL fs info found\n");
                }
            }
        }
    }
}

void WombatFramework::OpenFileSystems() // open the filesystems in the image or partition
{
    // may not need this function since i can open them when the partitions are added w/o having to reloop it.
}

void WombatFramework::OpenEvidenceImages() // open all evidence images.
{
    for(int j = 0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        const TSK_TCHAR** images;
        images = (const char**)malloc(wombatptr->evidenceobjectvector[j].fullpathvector.size()*sizeof(char*));
        for(int i=0; i < wombatptr->evidenceobjectvector[j].fullpathvector.size(); i++)
        {
            images[i] = wombatptr->evidenceobjectvector[j].fullpathvector[i].c_str();
        }
        wombatptr->evidenceobjectvector[j].imageinfo = tsk_img_open(wombatptr->evidenceobjectvector[j].itemcount, images, TSK_IMG_TYPE_DETECT, 0);
        free(images);
    }
}

void WombatFramework::BuildEvidenceModel()
{
    // COME BACK TO QTCONCURRENT AS I GET FARTHER ALONG AND RESEARCH MORE
    /*
    QVector<ItemObject> itemvector;
    for(int i = 0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        itemvector.append(wombatptr->evidenceobjectvector[i]);
    }
    QFutureWatcher<void> watcher;
    watcher.setFuture(QtConcurrent::map(itemvector, &WombatFramework::OpenEvidenceImages));
    //watcher.setFuture(QtConcurrent::map(wombatptr->evidenceobjectvector, WombatFramework::OpenEvidenceImages));
    watcher.waitForFinished();
    */
    OpenEvidenceImages(); // PROBABLY PUT THIS IN A MULTI-THREAD ENVIRONMENT.
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++) // for each evidence image info file.
    {
        if(wombatptr->evidenceobjectvector[i].imageinfo == NULL)
            fprintf(stderr, "Image didn't open. add to log file as error.\n");
        else
            fprintf(stderr, "Image %s opened. add to log file as info.\n", wombatptr->evidenceobjectvector[i].fullpathvector[0].c_str());
    }
    // NEED TO LAUNCH THIS IN A NEW THREAD TO KEEP GUI RESPONSIVE
    // NEED TO OPEN THE IMAGE - img_open.c [tsk_img_open()]
    // NEED TO GET THE METADATA FOR THE IMAGE/VOLUMES/PARTITIONS/FILES SO I CAN POPULATE THE DIRECTORY TREE INFORMATION


    // NEED TO ADD THE EVIDENCE ITEM TO THE DATABASE
    // POPULATE THE WOMBATVARPTR FOR THE EVIDENCEOBJECT VECTOR
    // NEED TO CREATE THE EVIDENCE TSK DATABASE (EXTRACT EVIDENCE ACCORDING TO MODULES)
    // NEED TO BUILD DIRMODEL AS I GO AND POPULATE DIRTREEVIEW AS I GO WITH EACH FILE
    // FOR NOW I WON'T BUILD MODULES, I'LL JUST DESIGN A MULTI-THREADED APPROACH FOR IT AND ABSTRACT TO PLUGGABLE MODULES LATER

    /*
    QFutureWatcher<void> watcher;
    std::vector<TskSchedulerQueue::task_struct* > tmpvector;
    watcher.setFuture(QtConcurrent::map(&tmpvector, &SleuthKitPlugin::TaskMap));
    //watcher.setFuture(QtConcurrent::map(&((std::vector<TskSchedulerQueue::task_struct*>)scheduler.mapvector), &SleuthKitPlugin::TaskMap));
    watcher.waitForFinished();
    */
    // QT CONCURRENT TEST
}
