#include "wombatfunctions.h"

std::string GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

char* TskTimeToStringUTC(time_t time, char buf[128])
{
    buf[0] = '\0';
    if (time <= 0) {
        strncpy(buf, "0000-00-00 00:00:00", 128);
    }
    else {
        //struct tm *tmTime = localtime(&time);
        struct tm *tmTime = gmtime(&time);

        snprintf(buf, 128, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", (int) tmTime->tm_year + 1900, (int) tmTime->tm_mon + 1, (int) tmTime->tm_mday, tmTime->tm_hour, (int) tmTime->tm_min, (int) tmTime->tm_sec);//, tzname[(tmTime->tm_isdst == 0) ? 0 : 1]);
    }
    return buf;
}

//Node* FindParentNode(Node* curnode, Node* parentnode)
int FindParentNode(Node* curnode, Node* parentnode, int rootinum)
{
    //int nodefound = 0;
    if(curnode->nodevalues.at(11).toInt() == rootinum)
    {
        parentnode->children.append(curnode);
        curnode->parent = parentnode;
        qDebug() << "curnode parent id == rootinum";
        return 1;
    }
    else
    {
    if(parentnode->nodevalues.at(5).toInt() == curnode->nodevalues.at(11).toInt()) // parent address == cur parentid
    {
        qDebug() << "parent node with objid: " << parentnode->nodevalues.at(0).toInt() << "found for objid: " << curnode->nodevalues.at(0).toInt() << "!";
        qDebug() << "parent addr == cur parid" << parentnode->nodevalues.at(5).toInt() << " == " << curnode->nodevalues.at(11).toInt();
        parentnode->children.append(curnode);
        curnode->parent = parentnode;
        //nodefound = 1;
        return 1;
        //return parentnode;
    }
    else if(parentnode->HasChildren())
    {
        for(int i=0; i < parentnode->children.count(); i++)
        {
            qDebug() << "re-iterate with new parent " << parentnode->children.at(i)->nodevalues.at(0).toInt();
            FindParentNode(curnode, parentnode->children.at(i), rootinum);
        }
    }
    return 0;
    }
    //return nodefound;
    /*
    qDebug() << "parentnode objectid: " << parentnode->nodevalues.at(0).toInt();
    qDebug() << "parentnode address || currentnode parentid: " << parentnode->nodevalues.at(5).toInt() << " || " << curnode->nodevalues.at(11).toInt();
    int parentid = curnode->nodevalues.at(11).toInt();
    //qDebug() << "curnode parid: " << parentid;
    if(parentnode->nodevalues.at(5).toInt() == parentid)
    {
        qDebug() << "findparentnode-parent objid: " << curnode->nodevalues.at(0).toInt();
        return parentnode;
    }
    else
    {
        foreach(Node* childnode, parentnode->children)
        {
            //if(childnode->children.count() > 0)
            //{
            qDebug() << "findparentnode-child child start objid: " << curnode->nodevalues.at(0).toInt();
            FindParentNode(curnode, childnode);
           // }
            //else
            //{
                //if(childnode->nodevalues.at(5).toInt() == parentid)
                //{
                    //qDebug() << "findparentnode->childless child objid: " << curnode->nodevalues.at(0).toInt();
                    //return childnode;
                //}
            //}
        }
    }*/
}

/*
bool ParentNodeExists(Node* curnode, Node* parentnode)
{
    bool isparent = false;
    foreach(Node* childnode, parentnode->children)
    {
        if(childnode->children.count() > 0)
        {
            foreach(Node* grandchildnode, childnode->children)
            {
                ParentNodeExists(curnode, grandchildnode);
            }
        }
        else
        {
            if(curnode->nodevalues.at(11).toInt() == childnode->nodevalues.at(5).toInt())
            {
                isparent = true;
                childnode->children.append(curnode);
            }
        }
    }
    return isparent;
}*/

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

bool ProcessingComplete()
{
    bool processingcomplete = false;
    for(int i = 0; i < threadvector.count(); i++)
    {
        processingcomplete = threadvector[i].isFinished();
    }
    
    return processingcomplete;
}

void ProcessFile(QVector<QString> tmpstrings, QVector<int> tmpints)
{

    if(fcasedb.isValid() && fcasedb.isOpen())
    {
        QSqlQuery fquery(fcasedb);
        fquery.prepare("INSERT INTO data(objecttype, type, name, parentid, fullpath, atime, ctime, crtime, mtime, size, address, md5, parimgid) VALUES(5, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        fquery.addBindValue(tmpints[0]);
        fquery.addBindValue(tmpstrings[0]);
        fquery.addBindValue(tmpints[1]);
        fquery.addBindValue(tmpstrings[1]);
        fquery.addBindValue(tmpints[2]);
        fquery.addBindValue(tmpints[3]);
        fquery.addBindValue(tmpints[4]);
        fquery.addBindValue(tmpints[5]);
        fquery.addBindValue(tmpints[6]);
        fquery.addBindValue(tmpints[7]);
        fquery.addBindValue(tmpstrings[2]);
        fquery.addBindValue(currentevidenceid);
        
        fquery.exec();
        fquery.finish();
        filesprocessed++;
        isignals->ProgUpd();
        
    }
    else
    {
        //qDebug() << fcasedb.lastError().text();
    }
}

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    //char buf[128];
    TSK_FS_HASH_RESULTS hashresults;
    //qDebug() << "Accessed Time (readable): " << tsk_fs_time_to_str(tmpfile->meta->atime, buf); // need to call this function for
    // dates and times when i get the variable and want to display it.
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    char sbuf[17];
    int sint = 0;
    for(int i=0; i < 16; i++)
    {
        sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
    }
    QString tmpstring = QString(sbuf);

    QVector<QString> filestrings;
    if(tmpfile->name != NULL) filestrings.append(QString(tmpfile->name->name));
    else filestrings.append(QString("unknown.wbt"));
    filestrings.append(QString("/") + QString(tmppath));
    filestrings.append(tmpstring);

    QVector<int> fileints;

    if(tmpfile->name != NULL)
    {
        fileints.append((int)tmpfile->name->type);
        fileints.append((int)tmpfile->name->par_addr);
    }
    else
    {
        fileints.append(0);
        fileints.append(0);
    }
    if(tmpfile->meta != NULL)
    {
        fileints.append((int)tmpfile->meta->atime);
        fileints.append((int)tmpfile->meta->ctime);
        fileints.append((int)tmpfile->meta->crtime);
        fileints.append((int)tmpfile->meta->mtime);
        fileints.append((int)tmpfile->meta->size);
        fileints.append((int)tmpfile->meta->addr);
    }
    else
    {
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
        fileints.append(0);
    }

    //QFutureWatcher<void> tmpwatcher;
    QFuture<void> tmpfuture = QtConcurrent::run(ProcessFile, filestrings, fileints);
    filewatcher.setFuture(tmpfuture);
    threadvector.append(tmpfuture);
    filesfound++;

    return TSK_WALK_CONT;
}
