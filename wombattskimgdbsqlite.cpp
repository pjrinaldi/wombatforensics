#include "wombattskimgdbsqlite.h"

// CUSTOMIZE THIS FUNCTION IN UNISON WITH SQLWRAPPER TO BUILD BY WOMBAT.DB


#define IMGDB_CHUNK_SIZE 1024*1024*1 // what size chunks should the database use when growing and shrinking
#define IMGDB_MAX_RETRY_COUNT 50    // how many times will we retry a SQL statement
#define IMGDB_RETRY_WAIT 100   // how long (in milliseconds) are we willing to wait between retries

/**
 * Set the database location.  Must call
 * initialize() before the object can be used.
 * @param a_outpath Directory to store the database in. This
 * directory must already exist.
*/
//WombatTskImgDBSqlite::WombatTskImgDBSqlite(const char * a_outpath)
WombatTskImgDBSqlite::WombatTskImgDBSqlite(QString dbName)
{
    m_db = NULL;
    dbname = dbName;
}

WombatTskImgDBSqlite::~WombatTskImgDBSqlite()
{
    //(void) close();
}
int WombatTskImgDBSqlite::close()
{
    /*
    if (m_db) {
        if (sqlite3_close(m_db) == SQLITE_OK)
            m_db = NULL;
        else
            return 1;
    }
    */
    return 0;
}
/*
int WombatTskImgDBSqlite::dropTables()
{
    if (!m_db)
        return 1;

    char * errmsg;
    // Drop all the tables. No error checking just Teutonic Destruction...
    sqlite3_exec(m_db, "DROP TABLE db_info",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE image_info",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE image_names",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE vol_info",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE fs_info",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE fs_files",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE fs_blocks",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE files",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE derived_files",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE carved_files",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE carved_sectors",NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE alloc_unalloc_map", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE blackboard_artifacts", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE blackboard_attributes", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE blackboard_artifact_types", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE blackboard_attribute_types", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE file_hashes", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE modules", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE module_status", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE unalloc_img_status", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP TABLE unused_sectors", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP INDEX attrs_artifact_id", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP INDEX attrs_attribute_type", NULL, NULL, &errmsg);
    sqlite3_exec(m_db, "DROP INDEX attrs_obj_id", NULL, NULL, &errmsg);

    return 0;
}
*/

int WombatTskImgDBSqlite::initialize()
{
    sqlObject = new SqlWrapper(dbname); // create the image database

    map<int, TskArtifactNames> artTypes = TskImgDB::getAllArtifactTypes();
    for (map<int, TskArtifactNames>::iterator it = artTypes.begin(); it != artTypes.end(); it++) {
        addArtifactType(it->first, it->second.typeName, it->second.displayName);
    }
    map<int, TskAttributeNames> attrTypes = TskImgDB::getAllAttributeTypes();
    for (map<int, TskAttributeNames>::iterator it = attrTypes.begin(); it != attrTypes.end(); it++) {
        addAttributeType(it->first, it->second.typeName, it->second.displayName);
    }

    addToolInfo("DBSchema", IMGDB_SCHEMA_VERSION);

    LOGINFO(L"ImgDB Created.");
    return 0;
}

/*
 * If the database file exists this method will open it otherwise
 * it will create a new database.
 * This method also configures the chunk size and the busy handler
 * for the newly opened database.
*/
int WombatTskImgDBSqlite::open()
{
    sqlObject = new SqlWrapper(sqlStatement, "15.15", dbname);
    return 0;
}

int WombatTskImgDBSqlite::addToolInfo(const char* name, const char* version)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.16", dbname);
    sqlObject->PrepareSql("INSERT INTO db_info (name, version) VALUES(?, ?);");
    sqlObject->BindValue(1, name);
    sqlObject->BindValue(2, version);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

   return 0;
}

int WombatTskImgDBSqlite::addImageInfo(int type, int size)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.15", dbname);
    sqlObject->PrepareSql("INSERT INTO image_info (type, ssize) VALUES(?, ?);");
    sqlObject->BindValue(1, type);
    sqlObject->BindValue(2, size);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    return 0;
}

int WombatTskImgDBSqlite::addImageName(char const *imgPath)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.1", dbname);
    sqlObject->PrepareSql("INSERT INTO image_names (seq, name) VALUES(NULL, ?);");
    sqlObject->BindValue(1, imgPath);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    return 0;
}

/*
 * Adds the sector addresses of the volumes into the db.
 */
int WombatTskImgDBSqlite::addVolumeInfo(const TSK_VS_PART_INFO * vs_part)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.6", dbname);
    sqlObject->PrepareSql("INSERT INTO vol_info (vol_id, sect_start, sect_len, description, flags) VALUES(?, ?, ?, ?, ?);");
    sqlObject->BindValue(1, (int)vs_part->addr);
    sqlObject->BindValue(2, vs_part->start);
    sqlObject->BindValue(3, vs_part->len);
    sqlObject->BindValue(4, vs_part->desc);
    sqlObject->BindValue(5, vs_part->flags);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    return 0;
}

int WombatTskImgDBSqlite::addFsInfo(int volId, int fsId, const TSK_FS_INFO * fs_info)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.2", dbname);
    sqlObject->PrepareSql("INSERT INTO fs_info (fs_id, img_byte_offset, vol_id, fs_type, block_size, block_count, root_inum, first_inum, last_inum) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");
    sqlObject->BindValue(1, fsId);
    sqlObject->BindValue(2, fs_info->offset);
    sqlObject->BindValue(3, volId);
    sqlObject->BindValue(4, (int)fs_info->ftype);
    sqlObject->BindValue(5, fs_info->block_size);
    sqlObject->BindValue(6, fs_info->block_count);
    sqlObject->BindValue(7, fs_info->root_inum);
    sqlObject->BindValue(8, fs_info->first_inum);
    sqlObject->BindValue(9, fs_info->last_inum);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    return 0;
}


/**
 * Given a file system and fs_file_id, return the file_id.
 */
uint64_t WombatTskImgDBSqlite::getFileId(int a_fsId, uint64_t a_fsFileId) const
{
    uint64_t fileId = 0;
    sqlObject = new SqlWrapper(sqlStatement, "15.8", dbname);
    sqlObject->PrepareSql("SELECT file_id FROM fs_files WHERE fs_id = ? AND fs_file_id = ?;");
    sqlObject->BindValue(1, a_fsId);
    sqlObject->BindValue(2, a_fsFileId);
    int result = sqlObject->StepSql();
    if(result == SQLITE_ROW)
    {
        fileId = (uint64_t)sqlObject->ReturnInt64(0);
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();
    }
    else
    {
        LOGERROR("Error Querying fs_files table.");
    }
    return fileId;
}


int WombatTskImgDBSqlite::getFileRecord(const uint64_t fileId, TskFileRecord& fileRecord) const
{
    sqlObject = new SqlWrapper(sqlStatement, "15.8", dbname);
    sqlObject->PrepareSql("SELECT f.file_id, f.type_id, f.name, f.par_file_id, f.dir_type, f.meta_type, f.dir_flags, f.meta_flags, f.size, f.ctime, f.crtime, f.atime, f.mtime, f.mode, f.uid, f.gid, f.status, f.full_path, fh.md5, fh.sha1, fh.sha2_256, fh.sha2_512 FROM files f LEFT OUTER JOIN file_hashes fh ON f.file_id = fh.file_id WHERE f.file_id =  ?;");
    sqlObject->BindValue(1, fileId);
    int result = sqlObject->StepSql();
    if(result == SQLITE_ROW)
    {
        fileRecord.fileId = sqlObject->ReturnInt64(0);
        fileRecord.typeId = (TskImgDB::FILE_TYPES)sqlObject->ReturnInt(1);
        fileRecord.name = (char*)sqlObject->ReturnText(2);
        fileRecord.parentFileId = sqlObject->ReturnInt64(3);
        fileRecord.dirType = (TSK_FS_NAME_TYPE_ENUM)sqlObject->ReturnInt(4);
        fileRecord.metaType = (TSK_FS_META_TYPE_ENUM)sqlObject->ReturnInt(5);
        fileRecord.dirFlags = (TSK_FS_NAME_FLAG_ENUM)sqlObject->ReturnInt(6);
        fileRecord.metaFlags = (TSK_FS_META_FLAG_ENUM)sqlObject->ReturnInt(7);
        fileRecord.size = sqlObject->ReturnInt64(8);
        fileRecord.ctime = sqlObject->ReturnInt(9);
        fileRecord.crtime = sqlObject->ReturnInt(10);
        fileRecord.atime = sqlObject->ReturnInt(11);
        fileRecord.mtime = sqlObject->ReturnInt(12);
        fileRecord.mode = (TSK_FS_META_MODE_ENUM)sqlObject->ReturnInt(13);
        fileRecord.uid = sqlObject->ReturnInt(14);
        fileRecord.gid = sqlObject->ReturnInt(15);
        fileRecord.status = (TskImgDB::FILE_STATUS)sqlObject->ReturnInt(16);
        fileRecord.fullPath = (char *)sqlObject->ReturnText(17);

        if(sqlObject->ReturnColumnType(18) == SQLITE_TEXT)
            fileRecord.md5 = (char *)sqlObject->ReturnText(18);
        if(sqlObject->ReturnColumnType(19) == SQLITE_TEXT)
            fileRecord.sha1 = (char *)sqlObject->ReturnText(19);
        if(sqlObject->ReturnColumnType(20) == SQLITE_TEXT)
            fileRecord.sha2_256 = (char *)sqlObject->ReturnText(20);
        if(sqlObject->ReturnColumnType(21) == SQLITE_TEXT)
            fileRecord.sha2_512 = (char *)sqlObject->ReturnText(21);
    }
    else
        LOGERROR("Error querying files table for file id");
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return 0;
}

int WombatTskImgDBSqlite::addFsFileInfo(int fileSystemID, const TSK_FS_FILE *fileSystemFile, const char *fileName, int fileSystemAttrType, int fileSystemAttrID, uint64_t &fileID, const char *filePath)
{
    fileID = 0;
    std::string fullpath(filePath);
    fullpath.append(fileName);
    std::string fileNameAsString(fileName);
    size_t found = fileNameAsString.find("'");
    if(found != std::string::npos) // replace it and replace all its subsequent occurences
    {
        fileNameAsString.replace(found, 1, "''");
        while ((found = fileNameAsString.find("'", found+2)) != std::string::npos) // found+2 because we want to move past the newly inserted single quote.
        {
            fileNameAsString.replace(found, 1, "''");
        }
    }
    sqlObject = new SqlWrapper(sqlStatement, "15.9", dbname);
    for(int codePoint = 1; codePoint < 32; codePoint++)
    {
        char codePointAsHex[10];
        codePointAsHex[0] = codePoint;
        codePointAsHex[1] = '\0';
        std::string stringToRemove(codePointAsHex);

        found = fileNameAsString.find(stringToRemove);
        if(found != std::string::npos) // replace it and replace all subsequent occurences
        {
            fileNameAsString.replace(found, 1, "");
            while((found = fileNameAsString.find(stringToRemove, found+1)) != std::string::npos) // found+1 because the control characters are just 1 character.
            {
                fileNameAsString.replace(found, 1, "");
            }
        }
    }
    fileName = fileNameAsString.c_str();
    // Get the file size.
    TSK_OFF_T size = 0;
    const TSK_FS_ATTR *fileSystemAttribute = tsk_fs_file_attr_get_id(const_cast<TSK_FS_FILE*>(fileSystemFile), fileSystemAttrID);
    if (fileSystemAttribute)
    {
        size = fileSystemAttribute->size;
    }
    // Get the file metadata, if it's available.
    int mtime = 0;
    int crtime = 0;
    int ctime = 0;
    int atime = 0;
    int meta_type = 0;
    int meta_flags = 0;
    int meta_mode = 0;
    int gid = 0;
    int uid = 0;
    if (fileSystemFile->meta)
    {
        mtime = static_cast<int>(fileSystemFile->meta->mtime);
        atime = static_cast<int>(fileSystemFile->meta->atime);
        ctime = static_cast<int>(fileSystemFile->meta->ctime);
        crtime = static_cast<int>(fileSystemFile->meta->crtime);
        meta_type = fileSystemFile->meta->type;
        meta_flags = fileSystemFile->meta->flags;
        meta_mode = fileSystemFile->meta->mode;
        gid = fileSystemFile->meta->gid;
        uid = fileSystemFile->meta->uid;
    }
    sqlObject->PrepareSql("INSERT INTO files(file_id, type_id, status, name, par_file_id, dir_type, meta_type, dir_flags, meta_flags, size, crtime, ctime, atime, mtime, mode, gid, uid, full_path) VALUES (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    sqlObject->BindValue(1, IMGDB_FILES_TYPE_FS);
    sqlObject->BindValue(2, IMGDB_FILES_STATUS_READY_FOR_ANALYSIS);
    sqlObject->BindValue(3, fileName);
    sqlObject->BindValue(4, findParObjId(fileSystemID, fileSystemFile->name->par_addr));
    sqlObject->BindValue(5, fileSystemFile->name->type);
    sqlObject->BindValue(6, meta_type);
    sqlObject->BindValue(7, fileSystemFile->name->flags);
    sqlObject->BindValue(8, meta_flags);
    sqlObject->BindValue(9, size);
    sqlObject->BindValue(10, crtime);
    sqlObject->BindValue(11, ctime);
    sqlObject->BindValue(12, atime);
    sqlObject->BindValue(13, mtime);
    sqlObject->BindValue(14, meta_mode);
    sqlObject->BindValue(15, gid);
    sqlObject->BindValue(16, uid);
    sqlObject->BindValue(17, fullpath.c_str());
    sqlObject->StepSql();
    fileID = sqlObject->ReturnLastInsertRowID();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    sqlObject = new SqlWrapper(sqlStatement, "15.8", dbname);
    sqlObject->PrepareSql("INSERT INTO fs_files (file_id, fs_id, fs_file_id, attr_type, attr_id) VALUES (?, ?, ?, ?, ?);");
    sqlObject->BindValue(1, fileID);
    sqlObject->BindValue(2, fileSystemID);
    sqlObject->BindValue(3, fileSystemFile->name->meta_addr);
    sqlObject->BindValue(4, fileSystemAttrType);
    sqlObject->BindValue(5, fileSystemAttrID);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    if(meta_type == TSK_FS_META_TYPE_DIR) // if dir, update parent id cache
        storeParObjId(fileSystemID, fileSystemFile->name->meta_addr, fileID);
    return 0;
}

/**
 * Add block info to the database.  This table stores the run information for each file so that we
 * can map which blocks are used by what files.
 * @param a_fsId Id that the file is located in
 * @param a_fileId ID of the file
 * @param a_sequence The sequence number of this run in the file (0 for the first run, 1 for the second run, etc.)
 * @param a_blk_addr Block address (the address that the file system uses -- NOT the physical sector addr)
 * @param a_len The number of blocks in the run
 * @returns 1 on error
 */
int WombatTskImgDBSqlite::addFsBlockInfo(int a_fsId, uint64_t a_fileId, int a_sequence, uint64_t a_blk_addr, uint64_t a_len)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.9", dbname);
    sqlObject->PrepareSql("INSERT INTO fs_blocks (fs_id, file_id, seq, blk_start, blk_len) VALUES(?, ?, ?, ?, ?);");
    sqlObject->BindValue(1, a_fsId);
    sqlObject->BindValue(2, a_fileId);
    sqlObject->BindValue(3, a_sequence);
    sqlObject->BindValue(4, a_blk_addr);
    sqlObject->BindValue(5, a_len);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    return 0;
}

int WombatTskImgDBSqlite::addAllocUnallocMapInfo(int a_volID, int unallocImgID,
                                           uint64_t unallocImgStart, uint64_t length, uint64_t origImgStart)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.10", dbname);
    sqlObject->PrepareSql("INSERT INTO alloc_unalloc_map (vol_id, unalloc_img_id, unalloc_img_sect_start, sect_len, orig_img_sect_start) VALUES (?, ?, ?, ?, ?);");
    sqlObject->BindValue(1, a_volID);
    sqlObject->BindValue(2, unallocImgID);
    sqlObject->BindValue(3, unallocImgStart);
    sqlObject->BindValue(4, length);
    sqlObject->BindValue(5, origImgStart);
    return 0;
}

/**
 * Get information on all of the free sectors in an image.
 *
 * @return Info on unallocated runs (or NULL on error).  Caller must free this when done.
 */
SectorRuns * WombatTskImgDBSqlite::getFreeSectors() const
{
    SectorRuns * sr = new SectorRuns();

    LOGINFO("WombatTskImgDBSqlite::getFreeSectors - Identifying Unallocated Sectors");
    /********** FIND the unallocated volumes *************/
    sqlObject = new SqlWrapper(sqlStatement, "15.11", dbname);
    sqlObject->PrepareSql("SELECT vol_id, sect_start, sect_len, flags FROM vol_info;");
    int result = sqlObject->StepSql();
    while(result == SQLITE_ROW)
    {
        int flags = sqlObject->ReturnInt(3);
        int vol_id = sqlObject->ReturnInt(0);
        int64_t start = sqlObject->ReturnInt64(1);
        int64_t len = sqlObject->ReturnInt64(2);

        // add the unallocated volumes
        if(flags & TSK_VS_PART_FLAG_UNALLOC)
            sr->addRun(start, len, vol_id);
        // add the unallocated volumes that don't have a known file system
        else
        {
            sqlite3_stmt *sqlStatement2;
            SqlWrapper *sqlObject2 = new SqlWrapper(sqlStatement2, "15.12", dbname);
            sqlObject2->PrepareSql("SELECT fs_id FROM fs_info WHERE vol_id = ?;");
            sqlObject2->BindValue(1, vol_id);
            int result2 = sqlObject2->StepSql();
            if(result2 != SQLITE_ROW)
                sr->addRun(start, len, vol_id);
            sqlObject2->FinalizeSql();
            sqlObject2->CloseSql();
        }
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    /*************** Find the unallocated blocks in each file system *************/
    // @@@ Need to make more dynamic
    int blk_size[32];
    memset(blk_size, 0, sizeof(blk_size));
    uint64_t blk_count[32];
    memset(blk_count, 0, sizeof(blk_count));
    int vol_id[32];
    uint64_t img_offset[32];

    // get basic info on each file system
    sqlObject = new SqlWrapper(sqlStatement, "15.13", dbname);
    sqlObject->PrepareSql("SELECT fs_id, vol_id, img_byte_offset, block_size, block_count FROM fs_info;");
    LOGINFO("WombatTskImgDBSqlite::getFreeSectors - START LOOP: Find the unallocated blocks in each file system.");
    int result = sqlObject->StepSql();
    while(result == SQLITE_ROW)
    {
        int fs_id = sqlObject->ReturnInt(0);
        if(fs_id > 32)
        {
            LOGERROR("WombatTskImgDBSqlite::getFreeSectors - fs_id in fs_info is bigger than 32.");
            break;
        }
        vol_id[fs_id] = sqlObject->ReturnInt(1);
        img_offset[fs_id] = sqlObject->ReturnInt64(2) / 512;
        blk_size[fs_id] = sqlObject->ReturnInt(3) / 512;
        blk_count[fs_id] = sqlObject->ReturnInt64(4);
        std:stringstream msg;
        msg.str("");
        msg << "WombatTskImgDBSqlite::getFreeSectors - fs_id=" << fs_id << " vol_id=" << vol_id[fs_id] << " img_offset=" << img_offset[fs_id] << " blk_size=" << blk_size[fs_id] <<
            " blk_count=" << blk_count[fs_id];
        LOGINFO(msg.str().c_str());
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    LOGINFO("WombatTskImgDBSqlite::getFreeSectors - DONE: Find the unallocated blocks in each file system.");

    // see what blocks have been used and add them to a list
    TSK_LIST *seen[32];
    memset(seen, 0, 32*sizeof(TSK_LIST *));
    sqlObject = new SqlWrapper(sqlStatement, "15.14", dbname);
    sqlObject->PrepareSql("SELECT fs_id, file_id, blk_start, blk_len FROM fs_blocks;");
    int result = sqlObject->StepSql();
    while(result = SQLITE_ROW)
    {
        int fs_id = sqlObject->ReturnInt(0);
        if(fs_id > 32)
        {
            LOGERROR("WombatTskImgDBSqlite::getFreeSectors - fs_id in fs_info is bigger than 32.");
            contine;
        }
        uint64_t file_id = (uint64_t)sqlObject->ReturnInt64(1);
        int64_t addr = sqlObject->ReturnInt64(2);
        int64_t len = sqlObject->ReturnInt64(3);

        // We only want to consider the runs for files that we allocated.
        sqlite3_stmt *sqlStatement2;
        SqlWrapper *sqlObject2 = new SqlWrapper(sqlStatement2, "15.15", dbname);
        sqlObject2->PrepareSql("SELECT meta_flags from files WHERE file_id = ?");
        sqlObject2->BindValue(1, file_id);
        sqlObject2->StepSql();
        int flags = sqlObject2->ReturnInt(0);
        sqlObject2->FinalizeSql();
        sqlObject2->CloseSql();
        if(flags & TSK_FS_META_FLAG_UNALLOC)
            contine;
        // @@@ We can probably find a more effecient storage method than this...
        int error = 0;
        for(int64_t i = 0; i < len; i++)
        {
            if(tsk_list_add(&seen[fs_id], addr+i))
            {
                LOGERROR("WombatTskImgDBSqlite::getFreeSectors - Error adding seen block address to list");
                error = 1;
                break;
            }
        }
        if(error)
            break;
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    LOGINFO("WombatTskImgDBSqlite::getFreeSectors - DONE: see what blocks have been used and add them to a list.");

    // cycle through each file system to find the unused blocks
    LOGINFO("WombatTskImgDBSqlite::getFreeSectors - START LOOP: cycle through each file system to find the unused blocks.");
    for (int f = 0; f < 32; f++) {
        if (blk_count[f] == 0)
            continue;

        uint64_t st = 0;
        int len = 0;
        // we previously adjusted blk_size and img_offset to be in sectors

        msg.str("");
        msg << "blk_count[" << f << "]=" << blk_count[f];
        LOGINFO(msg.str().c_str());
        for (uint64_t a = 0; a < blk_count[f]; a++) {
            // see if this addr was used in a file
            if (tsk_list_find(seen[f], a) == 0) {
                // we already have a run being defined
                if (len) {
                    // same run, so add on to it
                    if (st + len == a) {
                        len++;
                    }
                    // different run, make a new one
                    else {
                        sr->addRun(img_offset[f]+st*blk_size[f], len*blk_size[f], vol_id[f]);
                        st = a;
                        len = 1;
                    }
                }
                // start a new run
                else {
                    st = a;
                    len = 1;
                }
            }
        }
        // add the final run
        if (len) {
            sr->addRun(img_offset[f]+st*blk_size[f], len*blk_size[f], vol_id[f]);
        }
        tsk_list_free(seen[f]);
        seen[f] = NULL;
    }
    LOGINFO("WombatTskImgDBSqlite::getFreeSectors - DONE: cycle through each file system to find the unused blocks.");

    return sr;
}

std::string WombatTskImgDBSqlite::getImageBaseName() const
{
    // There may be multiple file paths if the image is a split image. Order by sequence number to extract the file name from the first path.
    sqlObject = new SqlWrapper(sqlStatement, "15.17", dbname);
    sqlObject->PrepareSql("SELECT name FROM image_names ORDER BY seq;");
    int result = sqlObject->StepSql();
    if(result == SQLITE_ROW)
    {
        Poco::Path imagePath(reinterpret_cast<const char*>(sqlObject->ReturnText(0)));
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();
        return imagePath.getFileName();
    }
    else
    {
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();
        return "";
    }
}

std::vector<std::wstring> WombatTskImgDBSqlite::getImageNamesW() const
{
    std::vector<std::wstring> imgList;
    sqlObject = new SqlWrapper(sqlStatement, "15.18", dbname);
    int result = sqlObject->StepSql();
    while (result == SQLITE_ROW)
    {
        imgList.push_back((wchar_t *)sqlObject->ReturnText16(0));
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return imgList;
}


std::vector<std::string> WombatTskImgDBSqlite::getImageNames() const
{
    std::vector<std::string> imgList;
    sqlObject = new SqlWrapper(sqlStatement, "15.19", dbname);
    sqlObject->PrepareSql("SELECT name FROM image_names ORDER BY seq;");
    int result = sqlObject->StepSql();
    while(result == SQLITE_ROW)
    {
        imgList.push_back((char *)sqlObject->ReturnText(0));
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return imgList;
}

/**
 * @param a_fileId  File id to get information about
 * @param a_fsOffset Byte offset of start of file system that the file is located in
 * @param a_fsFileId File system-specific id of the file
 * @param a_attrType Type of attribute for this file
 * @param a_attrId The ID of the attribute for this file
 * @returns -1 on error
 */
int WombatTskImgDBSqlite::getFileUniqueIdentifiers(uint64_t a_fileId, uint64_t &a_fsOffset, uint64_t &a_fsFileId, int &a_attrType, int &a_attrId) const
{
    sqlObject = new SqlWrapper(sqlStatement, "15.19", dbname);
    sqlObject->PrepareSql("SELECT fs_file_id, attr_type, attr_id, fs_info.img_byte_offset FROM fs_files, fs_info WHERE file_id = ? AND fs_info.fs_id = fs_files.fs_id;");
    sqlObject->BindValue(1, a_fileId);
    if(sqlObject->StepSql() == SQLITE_ROW)
    {
        a_fsFileId = sqlObject->ReturnInt64(0);
        a_attrType = sqlObject->ReturnInt(1);
        a_attrId = sqlObject->ReturnInt(2);
        a_fsOffset = sqlObject->ReturnInt64(3);
    }
    else
        return -1;
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return 0;
}

/**
 * Get number of volumes in image.
 * @return Number of volumes in image or -1 on error
 */
int WombatTskImgDBSqlite::getNumVolumes() const
{
    int count = 0;
    sqlObject = new SqlWrapper(sqlStatement, "15.20", dbname);
    sqlObject->PrepareSql("SELECT count(*) from vol_info;");

    /********** Get the number of volumes *************/
    if(sqlObject->StepSql() == SQLITE_ROW)
    {
        count = (int)sqlObject->ReturnInt(0);
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return count;
}

/**
 * Get number of files in image.
 * @return Number of files in image or -1 on error
 */
int WombatTskImgDBSqlite::getNumFiles() const
{
    std::string condition("");
    return getFileCount(condition);
}

/**
 * @returns the session_id or -1 on error.
 */
int WombatTskImgDBSqlite::getSessionID() const
{
    int sessionId = -1;
    sqlObject = new  SqlWrapper(sqlStatement, "15.21", dbname);

    /********** FIND the unallocated volumes *************/
    sqlObject->PrepareSql("SELECT version from db_info WHERE name=\"SID\";");
    if(sqlObject->StepSql() == SQLITE_ROW)
    {
        sessionId = (int)sqlObject->ReturnInt(0);
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return sessionId;
}

int WombatTskImgDBSqlite::begin()
{
    char *errmsg;
    sqlObject = new SqlWrapper(sqlStatement, "15.22", dbname);
    sqlObject->PrepareSql("BEGIN");
    if(sqlObject->ExecuteSql(&errmsg) != SQLITE_OK)
    {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::begin - BEGIN Error: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlObject->Free(errmsg);
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();

        return 1;
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return 0;
}

int WombatTskImgDBSqlite::commit()
{
    char *errmsg;
    sqlObject = new SqlWrapper(sqlStatement, "15.23", dbname);
    sqlObject->PrepareSql("COMMIT");
    if(sqlObject->ExecuteSql(&errmsg) != SQLITE_OK)
    {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::commit - COMMIT Error: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlObject->Free(errmsg);
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();

        return 1;
    }
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();

    return 0;
}

UnallocRun * WombatTskImgDBSqlite::getUnallocRun(int a_unalloc_img_id, int a_file_offset) const
{
    sqlObject = new SqlWrapper(sqlStatement, "15.24", dbname);
    sqlObject->PrepareSql("SELECT vol_id, unalloc_img_sect_start, sect_len, orig_img_sect_start FROM alloc_unalloc_map WHERE unalloc_img_id = ? AND unalloc_img_sect_start <= ? ORDER BY unalloc_img_sect_start DESC;");
    sqlObject->BindValue(1, a_unalloc_img_id);
    sqlObject->BindValue(2, a_file_offset);
    char *errmsg;
    char **result;
    int nrow, ncol;
    if(sqlObject->ReturnTable(&result, &nrow, &ncol, &errmsg) != SQLITE_OK)
    {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getUnallocRun - Error fetching data from alloc_unalloc_map table: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlObject->Free(errmsg);
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();

        return new UnallocRun(-1, -1, -1, -1, -1);
    }
    else
    {
        int vol_id;
        int unalloc_img_sect_start;
        int sect_len;
        int orig_img_sect_start;
        // skip the headers
        // @@@ DO SOME ERROR CHECKING HERE to make sure that results has data...
        sscanf(result[4], "%d", &vol_id);
        sscanf(result[5], "%d", &unalloc_img_sect_start);
        sscanf(result[6], "%d", &sect_len);
        sscanf(result[7], "%d", &orig_img_sect_start);
        sqlObject->FreeTable(result);
        sqlObject->FinalizeSql();
        sqlObject->CloseSql();

        return new UnallocRun(vol_id, a_unalloc_img_id, unalloc_img_sect_start, sect_len, orig_img_sect_start);
    }
}

/**
 * Adds information about a carved file into the database.  This includes the sector layout
 * information.
 *
 * @param vol_id Volume in which the carved file was found in
 * @param name Name of the file
 * @param size Number of bytes in file
 * @param runStarts Array with starting sector (relative to start of image) for each run in file.
 * @param runLengths Array with number of sectors in each run
 * @param numRuns Number of entries in previous arrays
 * @param fileId Carved file Id (output)
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::addCarvedFileInfo(int vol_id, const char *name, uint64_t size,
                                      uint64_t *runStarts, uint64_t *runLengths, int numRuns, uint64_t & fileId)
{
    sqlObject = new SqlWrapper(sqlStatement, "15.25", dbname);
    sqlObject->PrepareSql("INSERT INTO files (file_id, type_id, name, par_file_id, dir_type, meta_type, dir_flags, meta_flags, size, ctime, crtime, atime, mtime, mode, uid, gid, status, full_path) VALUES (NULL, ?, ?, NULL, ?, ?, ?, ?, ?, 0, 0, 0, 0, NULL, NULL, NULL, ?, ?)");
    sqlObject->BindValue(1, IMGDB_FILES_TYPE_CARVED);
    sqlObject->BindValue(2, name);
    sqlObject->BindValue(3, (int)TSK_FS_NAME_TYPE_REG);
    sqlObject->BindValue(4, (int)TSK_FS_META_TYPE_REG);
    sqlObject->BindValue(5, (int)TSK_FS_NAME_FLAG_UNALLOC);
    sqlObject->BindValue(6, (int)TSK_FS_META_FLAG_UNALLOC);
    sqlObject->BindValue(7, size);
    sqlObject->BindValue(8, IMGDB_FILES_STATUS_CREATED);
    sqlObject->BindValue(9, name);
    // MAY-118 NOTE: addCarvedFileInfo insert entry into files table, but actual file on disk has not been created yet.
    sqlObject->StepSql();
    // get the assigned file_id
    fileId = (uint64_t)sqlObject->ReturnLastInsertRowID();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    // insert into the carved_files_table
    sqlObject = new SqlWrapper(sqlStatement, "15.26", dbname);
    sqlObject->PrepareSql("INSERT INTO carved_files (file_id, vol_id) VALUES (?, ?);");
    sqlObject->BindValue(1, fileId);
    sqlObject->BindValue(2, vol_id);
    sqlObject->StepSql();
    sqlObject->FinalizeSql();
    sqlObject->CloseSql();
    // insert into carved_sectors table
    sqlObject = new SqlWrapper(sqlStatement, "15.27", dbname);
    sqlObject->PrepareSql("INSERT INTO carved_sectors (file_id, seq, sect_start, sect_len) VALUES (?, ?, ?, ?);");
    for (int i = 0; i < numRuns; i++)
    {
        sqlObject->ResetSql();
        sqlObject->ClearBindings();
        sqlObject->BindValue(1, fileId);
        sqlObject->BindValue(2, i);
        sqlObject->BindValue(3, runStarts[i]);
        sqlObject->BindValue(4, runLengths[i]);
        sqlObject->StepSql();
    }

    return 0;
}

/**
 * Adds information about derived files to the database.  Derived files typically come
 * from archives and may be compressed.
 *
 * @param name The name of the file.
 * @param parentId The id of the file from which this file is derived.
 * @param isDirectory True if entry is for a directory verus a file
 * @param size The size of the file.
 * @param details This is a string that may contain extra details related
 * to the particular type of mechanism that was used to derive this file,
 * e.g. files derived from zip archives may have extra information about the
 * compressed size of the file.
 * @param ctime Time file system file entry was changed.
 * @param crtime Time the file was created.
 * @param atime Last access time.
 * @param mtime Last modified time.
 * @param fileId Reference to location where file_id for file can be assigned
 * @param path Path of file
 *
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::addDerivedFileInfo(const std::string& name, const uint64_t parentId,
                                       const bool isDirectory, const uint64_t size,
                                       const std::string& details,
                                       const int ctime, const int crtime, const int atime, const int mtime,
                                       uint64_t &fileId, std::string path)
{
    if (!m_db)
        return -1;

    char stmt[1024];
    char * errmsg;

    TSK_FS_NAME_TYPE_ENUM dirType = isDirectory ? TSK_FS_NAME_TYPE_DIR : TSK_FS_NAME_TYPE_REG;
    TSK_FS_META_TYPE_ENUM metaType = isDirectory ? TSK_FS_META_TYPE_DIR : TSK_FS_META_TYPE_REG;

    // insert into files table
    sqlite3_snprintf(1024, stmt,
        "INSERT INTO files (file_id, type_id, name, par_file_id, dir_type, meta_type, size, ctime, crtime, atime, mtime, status, full_path) "
        "VALUES (NULL, %d, '%q', %llu, %d, %d, %llu, %d, %d, %d, %d, %d, '%q')",
        IMGDB_FILES_TYPE_DERIVED, name.c_str(), parentId, dirType, metaType, size, ctime, crtime, atime, mtime, IMGDB_FILES_STATUS_CREATED, path.c_str());

    if (sqlite3_exec(m_db, stmt, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::addDerivedFileInfo - Error adding data to file table for derived file: "
            << errmsg << L" " << stmt;

        LOGERROR(msg.str());

        sqlite3_free(errmsg);
        return -1;
    }

    // get the assigned file_id
    fileId = sqlite3_last_insert_rowid(m_db);

    // insert into the derived_files table
    sqlite3_snprintf(1024, stmt, "INSERT INTO derived_files (file_id, derivation_details) "
        "VALUES (%llu, '%q')", fileId, details.c_str());
    if (sqlite3_exec(m_db, stmt, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::addDerivedFileInfo - Error adding data to derived_files table : "
            << errmsg;

        LOGERROR(msg.str());
        sqlite3_free(errmsg);
        return -1;
    }

    return 0;
}

/**
 * Fills outBuffer with file IDs that match the name fileName.
 * Returns the number of file IDs written into outBuffer or -1 on error.
 */
int WombatTskImgDBSqlite::getFileIds(char *a_fileName, uint64_t *a_outBuffer, int a_buffSize) const
{

    if (!m_db)
        return -1;

    int outIdx = 0;

    sqlite3_stmt * statement;
    std::stringstream stmt;
    stmt << "SELECT file_id FROM files WHERE name LIKE '" << a_fileName << "';";
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while(sqlite3_step(statement) == SQLITE_ROW) {
            a_outBuffer[outIdx++] = (uint64_t)sqlite3_column_int64(statement, 0);
        }
        sqlite3_finalize(statement);
    }
    else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getFileIds - Error querying files table : " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }

    return outIdx;
}

/*
 * Return the minimum file id with status = READY_FOR_ANALYSIS in minFileId.
 * Return 0 on success, -1 if failed.
 */
int WombatTskImgDBSqlite::getMinFileIdReadyForAnalysis(uint64_t & minFileId) const
{
    if (!m_db)
        return -1;

    minFileId = 0;

    sqlite3_stmt * statement;
    std::stringstream stmt;
    stmt << "SELECT min(file_id) FROM files WHERE status = " <<
        TskImgDB::IMGDB_FILES_STATUS_READY_FOR_ANALYSIS << ";";
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            minFileId = (uint64_t)sqlite3_column_int64(statement, 0);
        }
        sqlite3_finalize(statement);
    }
    else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getMinFileIdReadyForAnalysis - Error querying files table : " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }
    return 0;
}

/**
 * Given the last file ID ready for analysis, find the largest file ID ready of analysis (in maxFileId)
 * Returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::getMaxFileIdReadyForAnalysis(uint64_t a_lastFileId, uint64_t & maxFileId) const
{
    if (!m_db)
        return -1;

    maxFileId = 0;

    sqlite3_stmt * statement;
    std::stringstream stmt;
    stmt << "SELECT max(file_id) FROM files WHERE status = " <<
        TskImgDB::IMGDB_FILES_STATUS_READY_FOR_ANALYSIS <<
        " AND file_id >= " <<  a_lastFileId << ";";
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            maxFileId = (uint64_t)sqlite3_column_int64(statement, 0);
        }
        sqlite3_finalize(statement);
    }
    else {
        std::wstringstream infoMessage;
        infoMessage <<  L"WombatTskImgDBSqlite::getMaxFileIdReadyForAnalysis - Error querying files table : " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }
    return 0;
}

SectorRuns * WombatTskImgDBSqlite::getFileSectors(uint64_t a_fileId) const
{
    if (!m_db)
        return NULL;

    SectorRuns * sr = new SectorRuns();

    sqlite3_stmt * statement;
    std::stringstream stmt;
    int srCount = 0;
    stmt <<
        "SELECT fs_blocks.blk_start, fs_blocks.blk_len, "
        "fs_info.block_size, fs_info.img_byte_offset, fs_info.vol_id "
        "FROM files "
        "JOIN fs_files ON files.file_id = fs_files.file_id "
        "JOIN fs_blocks ON files.file_id = fs_blocks.file_id "
        "JOIN fs_info ON fs_blocks.fs_id = fs_info.fs_id "
        "WHERE files.file_id = " << a_fileId << " "
        "ORDER BY fs_blocks.seq;";
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while(sqlite3_step(statement) == SQLITE_ROW) {
            uint64_t blkStart = (uint64_t)sqlite3_column_int64(statement, 0);
            uint64_t blkLength = (uint64_t)sqlite3_column_int64(statement, 1);
            int blkSize = sqlite3_column_int(statement, 2);
            uint64_t imgByteOffset = (uint64_t)sqlite3_column_int64(statement, 3);
            int volId = sqlite3_column_int(statement, 4);

            uint64_t start = (imgByteOffset + blkStart * blkSize) / 512;
            uint64_t len = (blkLength * blkSize) / 512;

            sr->addRun(start, len, volId);
            srCount++;
        }

        sqlite3_finalize(statement);
    }
    else {
        std::wstringstream infoMessage;
        infoMessage <<
            L"WombatTskImgDBSqlite::getFileSectors - "
            L"Error finding block data for file_id=" << a_fileId << ": " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return NULL;
    }

    if (srCount < 1) {
        delete sr;
        sr = NULL;
    }
    return sr;
}

/**
 * This callback mechanism is registered with SQLite and is
 * called whenever an operation would result in SQLITE_BUSY.
 * Each time this method is called we will back off IMGDB_RETRY_WAIT
 * x count milliseconds. A non zero return value tells SQLite to
 * retry the statement and a zero return value tells SQLite to
 * stop retrying, in which case it will return SQLITE_BUSY or
 * SQLITE_IOERR_BLOCKED to the caller.
 *
 * @param pDB - a pointer to the sqlite3 structure
 * @param count - the number of times this handler has been
 * called for this blocking event.
 */
int WombatTskImgDBSqlite::busyHandler(void * pDB, int count)
{
    if (count < IMGDB_MAX_RETRY_COUNT)
    {
        Poco::Thread::sleep(IMGDB_RETRY_WAIT * count);
        return 1;
    }

    return 0;
}



int WombatTskImgDBSqlite::updateFileStatus(uint64_t a_file_id, FILE_STATUS a_status)
{
    if (!m_db)
        return 1;

    std::stringstream stmt;
    char * errmsg;

    stmt << "UPDATE files SET status = " << a_status << " WHERE file_id = " << a_file_id << ";";
    if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::updateFileStatus - Error UPDATE file status: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return 1;
    }

    return 0;
}


int WombatTskImgDBSqlite::updateKnownStatus(uint64_t a_file_id, KNOWN_STATUS a_status)
{
    if (!m_db)
        return 1;

    std::stringstream stmt;
    char * errmsg;

    stmt << "UPDATE file_hashes SET known = " << a_status << " WHERE file_id = " << a_file_id << ";";
    if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::updateFileStatus - Error UPDATE file status: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return 1;
    }

    return 0;
}

bool WombatTskImgDBSqlite::dbExist() const
{
    if (m_db)
        return true;
    else
        return false;
}

void WombatTskImgDBSqlite::getCarvedFileInfo(const std::string& stmt, std::map<uint64_t, std::string>& results) const
{
    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            uint64_t fileId = (uint64_t)sqlite3_column_int64(statement, 0);
            std::string fileName = (char*)sqlite3_column_text(statement, 1);
            std::string cfileName = (char*)sqlite3_column_text(statement, 2);

            // Grab the extension and append it to the cfile name
            std::string::size_type pos = fileName.rfind('.');
            if (pos != std::string::npos)
                cfileName.append(fileName.substr(pos));

            results[fileId] = cfileName;
        }
        sqlite3_finalize(statement);
    } else
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getCarvedFileInfo - Error retrieving carved file details: "
            << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
    }
}

std::map<uint64_t, std::string> WombatTskImgDBSqlite::getUniqueCarvedFiles(HASH_TYPE hashType) const
{
    if (!m_db)
        throw TskException("No database.");

    std::map<uint64_t, std::string> results;

    string hash;
    switch (hashType) {
    case TskImgDB::MD5:
        hash = "md5";
        break;
    case TskImgDB::SHA1:
        hash = "sha1";
        break;
    case TskImgDB::SHA2_256:
        hash = "sha2_256";
        break;
    case TskImgDB::SHA2_512:
        hash = "sha2_512";
        break;
    default:
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getUniqueCarvedFiles - Unsupported hashType : " << hashType;
        LOGERROR(msg.str());
        return results;
    }

    stringstream stmt;

    // If hashes have not been calculated return all carved files
    stmt << "SELECT count(*) FROM file_hashes;";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        if (sqlite3_step(statement) == SQLITE_ROW)
        {
            uint64_t counter = (uint64_t)sqlite3_column_int64(statement, 0);
            if (counter == 0)
            {
                sqlite3_finalize(statement);
                stmt.str("");
                stmt << "select c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' "
                    << "|| c.file_id from files f, carved_files c, carved_sectors cs "
                    << "where c.file_id = cs.file_id and cs.seq = 0 and f.file_id = c.file_id order by c.file_id";
                getCarvedFileInfo(stmt.str(), results);
                return results;
            }
        }
        sqlite3_finalize(statement);
    } else
    {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getUniqueCarvedFiles - Error getting file_hashes count: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }

    stmt.str("");

    // Get the set of files for which the hash has been calculated.
    stmt << "select c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' "
        << "|| c.file_id from files f, carved_files c, carved_sectors cs "
        << "where c.file_id = cs.file_id and cs.seq = 0 and f.file_id = c.file_id and c.file_id in "
        << "(select min(file_id) from file_hashes where " << hash << " != '' group by " << hash << ") order by c.file_id";

    getCarvedFileInfo(stmt.str(), results);

    // Next get the set of files for which the hash has *not* been calculated.
    stmt.str("");

    stmt << "select c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' "
        << "|| c.file_id from files f, carved_files c, carved_sectors cs "
        << "where c.file_id = cs.file_id and cs.seq = 0 and f.file_id = c.file_id and c.file_id in "
        << "(select file_id from file_hashes where " << hash << " = '') order by c.file_id";

    getCarvedFileInfo(stmt.str(), results);

    // Finally, add file info for all of the carved files for which there are no hashes of any sort.
    // All of these files must be included because without hashes there is no way to determine uniqueness.
    stmt.clear();
    stmt.str("");
    stmt << "SELECT c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' || c.file_id "
         << "FROM files f, carved_files c, carved_sectors cs "
         << "WHERE c.file_id = cs.file_id AND cs.seq = 0 AND f.file_id = c.file_id AND c.file_id NOT IN "
         << "(SELECT fh.file_id FROM file_hashes fh) ORDER BY c.file_id";
    getCarvedFileInfo(stmt.str(), results);

    return results;
}

void WombatTskImgDBSqlite::getCarvedFileInfo(const std::string &query, bool getHash, std::vector<TskCarvedFileInfo> &carvedFileInfos) const
{
    sqlite3_stmt *statement;
    executeStatement(query, statement, "WombatTskImgDBSqlite::getCarvedFileInfo");

    TskCarvedFileInfo info;
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        info.fileID = static_cast<uint64_t>(sqlite3_column_int64(statement, 0));
        std::string fileName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)); // Reinterpret from unsigned Char*
        info.cFileName = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)); // Reinterpret from unsigned Char*
        if (getHash)
        {
            info.hash = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
        }

        // Append the extension from the original file name to the constructed "cfile" name.
        std::string::size_type pos = fileName.rfind('.');
        if (pos != std::string::npos)
        {
            info.cFileName.append(fileName.substr(pos));
        }

        carvedFileInfos.push_back(info);
    }

    sqlite3_finalize(statement);
}

std::vector<TskCarvedFileInfo> WombatTskImgDBSqlite::getUniqueCarvedFilesInfo(HASH_TYPE hashType) const
{
    const std::string msgPrefix = "WombatTskImgDBSqlite::getUniqueCarvedFilesInfo : ";

    if (!m_db)
    {
        std::ostringstream msg;
        msg << msgPrefix << "no database connection";
        throw TskException(msg.str());
    }

    // Map the requested hash type to a file_hashes table column name.
    string hash;
    switch (hashType)
    {
    case TskImgDB::MD5:
        hash = "md5";
        break;
    case TskImgDB::SHA1:
        hash = "sha1";
        break;
    case TskImgDB::SHA2_256:
        hash = "sha2_256";
        break;
    case TskImgDB::SHA2_512:
        hash = "sha2_512";
        break;
    default:
        std::ostringstream msg;
        msg << msgPrefix << "unsupported hash type :" << hashType;
        throw TskException(msg.str());
    }

    std::vector<TskCarvedFileInfo> carvedFileInfos;

    // Do a quick check to see if any hashes have been calculated.
    std::ostringstream query;
    query << "SELECT COUNT(*) FROM file_hashes;";
    sqlite3_stmt *countStmt;
    executeStatement(query.str(), countStmt, "WombatTskImgDBSqlite::getUniqueCarvedFiles");
    if (sqlite3_step(countStmt) == SQLITE_ROW && static_cast<uint64_t>(sqlite3_column_int64(countStmt, 0)) != 0)
    {
        // At least one type of hash has been calculated (presumably for all files, but this is not guaranteed).
        // First, add file info for the set of unique files among the carved files for which the specified type of hash is available.
        query.clear();
        query.str("");
        query << "SELECT c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' || c.file_id, fh." << hash << " "
              << "FROM files f, carved_files c, carved_sectors cs, file_hashes fh "
              << "WHERE c.file_id = cs.file_id AND cs.seq = 0 AND f.file_id = c.file_id AND c.file_id = fh.file_id AND c.file_id IN "
              << "(SELECT MIN(file_id) FROM file_hashes WHERE " << hash << " != '' GROUP BY " << hash << ") ORDER BY c.file_id";
        getCarvedFileInfo(query.str(), true, carvedFileInfos);

         // Next, add file info for all of the carved files for which the specified hash is not available.
         // All of these files must be included because without the specified hash there is no acceptable way to determine uniqueness.
        query.clear();
        query.str("");
        query << "SELECT c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' || c.file_id "
              << "FROM files f, carved_files c, carved_sectors cs "
              << "WHERE c.file_id = cs.file_id AND cs.seq = 0 AND f.file_id = c.file_id AND c.file_id IN "
              << "(SELECT file_id FROM file_hashes WHERE " << hash << " = '') ORDER BY c.file_id";
        getCarvedFileInfo(query.str(), false, carvedFileInfos);

        // Finally, add file info for all of the carved files for which there are no hashes of any sort.
        // All of these files must be included because without hashes there is no way to determine uniqueness.
        query.clear();
        query.str("");
        query << "SELECT c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' || c.file_id "
              << "FROM files f, carved_files c, carved_sectors cs "
              << "WHERE c.file_id = cs.file_id AND cs.seq = 0 AND f.file_id = c.file_id AND c.file_id NOT IN "
              << "(SELECT fh.file_id FROM file_hashes fh) ORDER BY c.file_id";
        getCarvedFileInfo(query.str(), false, carvedFileInfos);
    }
    else
    {
        // No hashes have been calculated.
        // Return carved file info all of the carved files because without hashes there is no way to determine uniqueness.
        query.clear();
        query.str("");
        query << "SELECT c.file_id, f.name, 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' || c.file_id "
              << "FROM files f, carved_files c, carved_sectors cs "
              << "WHERE c.file_id = cs.file_id AND cs.seq = 0 AND f.file_id = c.file_id ORDER BY c.file_id";
        getCarvedFileInfo(query.str(), false, carvedFileInfos);

        std::ostringstream msg;
        msg << msgPrefix << "no hashes available, returning all carved files";
        LOGWARN(msg.str());
    }
    sqlite3_finalize(countStmt);

    return carvedFileInfos;
}

std::vector<uint64_t> WombatTskImgDBSqlite::getCarvedFileIds() const
{
    return getFileIdsWorker("carved_files");
}

std::vector<uint64_t> WombatTskImgDBSqlite::getUniqueFileIds(HASH_TYPE hashType) const
{
    if (!m_db)
        throw TskException("No database.");

    std::vector<uint64_t> results;

    string hash;
    switch (hashType) {
    case TskImgDB::MD5:
        hash = "md5";
        break;
    case TskImgDB::SHA1:
        hash = "sha1";
        break;
    case TskImgDB::SHA2_256:
        hash = "sha2_256";
        break;
    case TskImgDB::SHA2_512:
        hash = "sha2_512";
        break;
    default:
        std::wstringstream errorMsg;
        errorMsg << L"WombatTskImgDBSqlite::getUniqueFileIds - Unsupported hashType : " << hashType ;
        LOGERROR(errorMsg.str());
        return results;
    }

    stringstream stmt;

    stmt << "SELECT min(file_id) FROM file_hashes WHERE " << hash << " != '' group by " << hash ;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            uint64_t fileId = (uint64_t)sqlite3_column_int64(statement, 0);
            results.push_back(fileId);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getUniqueFileIds - Error querying file_hashes table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return results;
}

std::vector<uint64_t> WombatTskImgDBSqlite::getFileIds() const
{
    return getFileIdsWorker("files");
}

std::vector<uint64_t> WombatTskImgDBSqlite::getFileIdsWorker(std::string tableName, const std::string condition) const
{
    if (!m_db)
        throw TskException("No database.");

    std::vector<uint64_t> results;

    stringstream stmt;

    stmt << "SELECT file_id FROM " << tableName;
    if (condition.compare("") != 0)
        stmt << " WHERE " << condition;
    stmt <<  " ORDER BY file_id";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            uint64_t fileId = (uint64_t)sqlite3_column_int64(statement, 0);
            results.push_back(fileId);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getFileIdsWorker - Error getting file ids from table " <<
            tableName.c_str() << ", " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return results;
}

/**
 * Get the list of file ids that match the given criteria.
 * The given string will be appended to "select files.file_id from files".
 * See \ref img_db_schema_v1_5_page for tables and columns to include in
 * the selection criteria.
 *
 * @param condition Must be a valid SQL string defining the selection criteria.
 * @returns The collection of file ids matching the selection criteria. Throws
 * TskException if database not initialized.
 */
std::vector<uint64_t> WombatTskImgDBSqlite::getFileIds(const std::string& condition) const
{
    if (!m_db)
        throw TskException("Database not initialized.");

    std::vector<uint64_t> results;

    std::string stmt("SELECT files.file_id FROM files");

    constructStmt(stmt, condition);

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            uint64_t fileId = (uint64_t)sqlite3_column_int64(statement, 0);
            results.push_back(fileId);
        }
        sqlite3_finalize(statement);
    } else
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getFilesIds - Error getting file ids: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
    }
    return results;
}
/*
 * Get the list of file records that match the given criteria.
 * The given string will be appended to "select ... from files".
 *
 * @param condition Must be a valid SQL string defining the selection criteria.
 * @returns The collection of file records matching the selection criteria. Throws
 * TskException if database not initialized.
 */
const std::vector<TskFileRecord> WombatTskImgDBSqlite::getFileRecords(const std::string& condition) const
{
    if (!m_db)
        throw TskException("Database not initialized.");

    std::vector<TskFileRecord> results;

    std::stringstream stmtstrm;

    stmtstrm << "SELECT f.file_id, f.type_id, f.name, f.par_file_id, f.dir_type, f.meta_type, f.dir_flags, "
        << "f.meta_flags, f.size, f.ctime, f.crtime, f.atime, f.mtime, f.mode, f.uid, f.gid, f.status, f.full_path, "
        << "fh.md5, fh.sha1, fh.sha2_256, fh.sha2_512 "
        << "FROM files f LEFT OUTER JOIN file_hashes fh ON f.file_id = fh.file_id ";

    std::string stmt = stmtstrm.str();
    constructStmt(stmt, condition);

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while(sqlite3_step(statement) == SQLITE_ROW) {
            TskFileRecord fileRecord;
            fileRecord.fileId       = sqlite3_column_int64(statement, 0);
            fileRecord.typeId       = (TskImgDB::FILE_TYPES) sqlite3_column_int(statement, 1);
            fileRecord.name         = (char *)sqlite3_column_text(statement, 2);
            fileRecord.parentFileId = sqlite3_column_int64(statement, 3);
            fileRecord.dirType      = (TSK_FS_NAME_TYPE_ENUM) sqlite3_column_int(statement, 4);
            fileRecord.metaType     = (TSK_FS_META_TYPE_ENUM) sqlite3_column_int(statement, 5);
            fileRecord.dirFlags     = (TSK_FS_NAME_FLAG_ENUM) sqlite3_column_int(statement, 6);
            fileRecord.metaFlags    = (TSK_FS_META_FLAG_ENUM) sqlite3_column_int(statement, 7);
            fileRecord.size         = sqlite3_column_int64(statement, 8);
            fileRecord.ctime        = sqlite3_column_int(statement, 9);
            fileRecord.crtime       = sqlite3_column_int(statement, 10);
            fileRecord.atime        = sqlite3_column_int(statement, 11);
            fileRecord.mtime        = sqlite3_column_int(statement, 12);
            fileRecord.mode         = (TSK_FS_META_MODE_ENUM) sqlite3_column_int(statement, 13);
            fileRecord.uid          = sqlite3_column_int(statement, 14);
            fileRecord.gid          = sqlite3_column_int(statement, 15);
            fileRecord.status       = (TskImgDB::FILE_STATUS) sqlite3_column_int(statement, 16);
            fileRecord.fullPath     = (char *)sqlite3_column_text(statement, 17);

            if (sqlite3_column_type(statement, 18) == SQLITE_TEXT)
                fileRecord.md5      = (char *)sqlite3_column_text(statement, 18);
            if (sqlite3_column_type(statement, 19) == SQLITE_TEXT)
                fileRecord.sha1     = (char *)sqlite3_column_text(statement, 19);
            if (sqlite3_column_type(statement, 20) == SQLITE_TEXT)
                fileRecord.sha2_256 = (char *)sqlite3_column_text(statement, 20);
            if (sqlite3_column_type(statement, 21) == SQLITE_TEXT)
                fileRecord.sha2_512 = (char *)sqlite3_column_text(statement, 21);
            results.push_back(fileRecord);
        }
    }
    else
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getFilesRecords - Error getting file reocrds: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
    }
    return results;
}


/**
 * Get the number of files that match the given criteria.
 * The given string will be appended to "select files.file_id from files".
 *
 * @param condition Must be a valid SQL string defining the selection criteria.
 * @returns The number of files matching the selection criteria.
 */
int WombatTskImgDBSqlite::getFileCount(const std::string& condition) const
{
    if (!m_db)
        throw TskException("Database not initialized.");

    int result = 0;

    std::string stmt("SELECT COUNT(files.file_id) FROM files");

    constructStmt(stmt, condition);

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            result = (uint64_t)sqlite3_column_int(statement, 0);
        }
        sqlite3_finalize(statement);
    } else
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getFileCount - Error getting file count: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
    }
    return result;
}

int tsk_strnicmp(const char *s1, const char *s2, size_t N)
{
    if (N == 0)
        return 0;
    int diff = 0;
    if (s1 && s2) {
        while (N-- > 0 && (diff = (toupper(*s1) - toupper(*s2))) == 0 && *s1 && *s2) {
            s1++;
            s2++;
        }
    }
    else if (s1)
        return +1;
    else if (s2)
        return -1;
    return diff;
}

/* Append condition to stmt to make a single SQL query.
 */
void WombatTskImgDBSqlite::constructStmt(std::string& stmt, std::string condition) const
{
    if (!condition.empty())
    {
        // Remove leading whitespace from condition
        condition.erase(0, condition.find_first_not_of(' '));

        std::string whereClause("WHERE");
        std::string joinClause("JOIN");
        std::string leftClause("LEFT");
        std::string orderClause("ORDER");

        /* If the condition doesn't start with one of the below statements
         * (WHERE, JOIN, etc.), then
         * it is presumably extending the FROM clause with
         * one or more table names. In this case we need to add the comma to
         * the statement. */
        if (tsk_strnicmp(condition.c_str(), whereClause.c_str(), whereClause.length()) != 0 &&
            tsk_strnicmp(condition.c_str(), joinClause.c_str(), joinClause.length()) != 0 &&
            tsk_strnicmp(condition.c_str(), leftClause.c_str(), leftClause.length()) != 0 &&
            tsk_strnicmp(condition.c_str(), orderClause.c_str(), orderClause.length()) != 0 &&
            condition[0] != ',')
        {
            stmt.append(",");
        }
    }

    stmt.append(" ");
    stmt.append(condition);
}

// Set file hash for hashType for a_file_id
// Return 1 on failure, 0 on success.
int WombatTskImgDBSqlite::setHash(const uint64_t a_file_id, const TskImgDB::HASH_TYPE hashType, const std::string& hash) const
{
    if (!m_db)
        throw TskException("No database.");

    string hashTypeStr;
    switch (hashType) {
    case TskImgDB::MD5:
        hashTypeStr = "md5";
        break;
    case TskImgDB::SHA1:
        hashTypeStr = "sha1";
        break;
    case TskImgDB::SHA2_256:
        hashTypeStr = "sha2_256";
        break;
    case TskImgDB::SHA2_512:
        hashTypeStr = "sha2_512";
        break;
    default:
        std::wstringstream errorMsg;
        errorMsg << L"WombatTskImgDBSqlite::setHash - Unsupported hashType : " << hashType ;
        LOGERROR(errorMsg.str());
        return 1;
    }

    stringstream stmt;
    std::string md5, sha1, sha2_256, sha2_512;
    int known = IMGDB_FILES_UNKNOWN;
    std::stringstream stream;

    stmt << "SELECT md5, sha1, sha2_256, sha2_512, known from file_hashes WHERE file_id = " << a_file_id;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            md5 = (char *)sqlite3_column_text(statement, 0);
            sha1 = (char *)sqlite3_column_text(statement, 1);
            sha2_256 = (char *)sqlite3_column_text(statement, 2);
            sha2_512 = (char *)sqlite3_column_text(statement, 3);
            known = (int)sqlite3_column_int(statement, 4);
        }
        sqlite3_finalize(statement);
    } else {
        ; // OK if not exists
    }

    // insert new record
    stmt.str("");
    stmt << "INSERT OR REPLACE INTO file_hashes (file_id, md5, sha1, sha2_256, sha2_512, known) VALUES (" << a_file_id;
    switch (hashType) {
    case TskImgDB::MD5:
        stmt << ", '" << hash << "'";
        stmt << ", '" << sha1 << "'";
        stmt << ", '" << sha2_256 << "'";
        stmt << ", '" << sha2_512 << "'";
        stream << known;
        stmt << ", " << stream.str();
        break;
    case TskImgDB::SHA1:
        stmt << ", '" << md5 << "'";
        stmt << ", '" << hash << "'";
        stmt << ", '" << sha2_256 << "'";
        stmt << ", '" << sha2_512 << "'";
        stream << known;
        stmt << ", " << stream.str();
        break;
    case TskImgDB::SHA2_256:
        stmt << ", '" << md5 << "'";
        stmt << ", '" << sha1 << "'";
        stmt << ", '" << hash << "'";
        stmt << ", '" << sha2_512 << "'";
        stream << known;
        stmt << ", " << stream.str();
        break;
    case TskImgDB::SHA2_512:
        stmt << ", '" << md5 << "'";
        stmt << ", '" << sha1 << "'";
        stmt << ", '" << sha2_256 << "'";
        stmt << ", '" << hash << "'";
        stream << known;
        stmt << ", " << stream.str();
        break;
    }
    stmt << ")";

    char *errmsg;
    if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::setHash - Error adding hash to file_hashes table: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlite3_free(errmsg);
        return 1;
    }

    return 0;
}

std::string WombatTskImgDBSqlite::getCfileName(const uint64_t a_file_id) const
{
    if (!m_db)
        throw TskException("No database.");

    std::string cfileName;
    stringstream stmt;

    stmt << "select 'cfile_' || c.vol_id || '_' || cs.sect_start || '_' || f.file_id"
        " from files f, carved_files c, carved_sectors cs where f.file_id = c.file_id and c.file_id = cs.file_id and cs.seq = 0"
        " and f.file_id = " << a_file_id;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            cfileName = (char *)sqlite3_column_text(statement, 0);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage <<  L"WombatTskImgDBSqlite::getCfileName - Error querying tables: %S" << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }

    stmt.str("");
    stmt << "select f.name "
        " from files f, carved_files c, carved_sectors cs where f.file_id = c.file_id and c.file_id = cs.file_id and cs.seq = 0"
        " and f.file_id = " << a_file_id;

    std::string name;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            name = (char *)sqlite3_column_text(statement, 0);
        }
        sqlite3_finalize(statement);
        size_t pos = name.rfind('.');
        if (pos != string::npos)
            cfileName += name.substr(pos);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getCfileName - Error querying tables: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }

    return cfileName;
}

/**
 * Return the ImageInfo
 * @param type Image Type (output)
 * @param sectorSize Image sector size (output)
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::getImageInfo(int & type, int & sectorSize) const
{
    int rc = -1;
    if (!m_db)
        return rc;

    stringstream stmt;

    stmt << "SELECT type, ssize FROM image_info";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            type = (int)sqlite3_column_int64(statement, 0);
            sectorSize = (int)sqlite3_column_int64(statement, 1);
            rc = 0;
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage <<  L"WombatTskImgDBSqlite::getImageInfo - Error querying image_info table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }
    return rc;
}

/**
 * Return a list of TskVolumeInfoRecord
 * @param volumeInfoList A list of TskVolumeInfoRecord (output)
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::getVolumeInfo(std::list<TskVolumeInfoRecord> & volumeInfoList) const
{
    std::list<TskVolumeInfoRecord> list;

    if (!m_db)
        return -1;

    stringstream stmt;
    stmt << "SELECT vol_id, sect_start, sect_len, description, flags FROM vol_info";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            TskVolumeInfoRecord vol_info;
            vol_info.vol_id = sqlite3_column_int(statement,0);
            vol_info.sect_start = sqlite3_column_int64(statement,1);
            vol_info.sect_len = sqlite3_column_int64(statement,2);
            vol_info.description.assign((char *)sqlite3_column_text(statement, 3));
            vol_info.flags = (TSK_VS_PART_FLAG_ENUM)sqlite3_column_int(statement, 4);
            volumeInfoList.push_back(vol_info);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getVolumeInfo - Error getting from vol_info table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }
    return 0;
}

/**
 * Return a list of TskFsInfoRecord
 * @param fsInfoList A list of TskFsInfoRecord (output)
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::getFsInfo(std::list<TskFsInfoRecord> & fsInfoList) const
{
    std::list<TskFsInfoRecord> list;

    if (!m_db)
        return -1;

    stringstream stmt;
    stmt << "SELECT fs_id, img_byte_offset, vol_id, fs_type, block_size, block_count, root_inum, first_inum, last_inum FROM fs_info";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            TskFsInfoRecord fs_info;
            fs_info.fs_id = sqlite3_column_int(statement,0);
            fs_info.img_byte_offset = sqlite3_column_int64(statement,1);
            fs_info.vol_id = sqlite3_column_int(statement,2);
            fs_info.fs_type = (TSK_FS_TYPE_ENUM)sqlite3_column_int(statement,3);
            fs_info.block_size = sqlite3_column_int(statement,4);
            fs_info.block_count = sqlite3_column_int64(statement,5);
            fs_info.root_inum = sqlite3_column_int64(statement,6);
            fs_info.first_inum = sqlite3_column_int64(statement,7);
            fs_info.last_inum = sqlite3_column_int64(statement,8);
            fsInfoList.push_back(fs_info);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getFsInfo - Error getting from fs_info table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }
    return 0;
}

typedef std::map<std::string, int> FileTypeMap_t;

static std::string getFileType(const char *name)
{
    std::string filename = name;
    size_t pos = filename.rfind('.');
    if (pos != std::string::npos) {
        std::string suffix = filename.substr(pos);
        std::string result;
        for (size_t i=0; i < suffix.size(); i++) {
            result += (char)tolower(suffix[i]);
        }
        return result;
    }
    else
        return std::string("");
}

/**
 * Return a list of TskFileTypeRecord for all files.
 * @param fileTypeInfoList A list of TskFileTypeRecord (output)
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::getFileInfoSummary(std::list<TskFileTypeRecord> &fileTypeInfoList) const
{
    std::stringstream stmt;
    stmt << "SELECT name FROM files WHERE dir_type = " << TSK_FS_NAME_TYPE_REG;

    return getFileTypeRecords(stmt.str(), fileTypeInfoList);
}

/**
 * Return a list of TskFileTypeRecord for fileType
 * @param fileType FILE_TYPE to report
 * @param fileTypeInfoList A list of TskFileTypeRecord (output)
 * @returns 0 on success or -1 on error.
 */
int WombatTskImgDBSqlite::getFileInfoSummary(FILE_TYPES fileType, std::list<TskFileTypeRecord> & fileTypeInfoList) const
{
    stringstream stmt;
    stmt << "SELECT name FROM files WHERE type_id = " << fileType << " AND dir_type = " << TSK_FS_NAME_TYPE_REG;

    return getFileTypeRecords(stmt.str(), fileTypeInfoList);
}

/**
 * Return a list of TskFileTypeRecords matching the given SQL statement.
 * @param stmt The SQL statement used to match file records.
 * @param fileTypeInfoList A list of TskFileTypeRecord (output)
 * @returns 0 on success of -1 on error.
 */
int WombatTskImgDBSqlite::getFileTypeRecords(const std::string& stmt, std::list<TskFileTypeRecord>& fileTypeInfoList) const
{
    if (!m_db)
        return -1;

    std::list<TskFileTypeRecord> list;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK) {
        FileTypeMap_t fileTypeMap;
        while (sqlite3_step(statement) == SQLITE_ROW) {
            char *name = (char *)sqlite3_column_text(statement, 0);
            std::string type = getFileType(name);
            FileTypeMap_t::iterator iter = fileTypeMap.find(type);
            if (iter != fileTypeMap.end()) {
                // increment file counter
                int count = iter->second;
                fileTypeMap[type] = ++count;
            } else {
                // add a new file type
                fileTypeMap.insert(pair<std::string, int>(type, 1));
            }
        }
        for (FileTypeMap_t::const_iterator iter=fileTypeMap.begin(); iter != fileTypeMap.end(); iter++) {
            TskFileTypeRecord info;
            info.suffix.assign((*iter).first.c_str());
            info.count = (*iter).second;
            info.description.assign("File Type Description");
            fileTypeInfoList.push_back(info);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getFileTypeRecords - Error querying files table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        return -1;
    }
    return 0;
}

/**
 * Insert the Module record, if module name does not already exist in modules table.
 * Returns Module Id associated with the Module record.
 * @param name Module name
 * @param description Module description
 * @param moduleId Module Id (output)
 * @returns 0 on success, -1 on error.
 */
int WombatTskImgDBSqlite::addModule(const std::string& name, const std::string& description, int & moduleId)
{
    if (!m_db)
        return -1;

    if (name.empty())
    {
        LOGWARN(L"WombatTskImgDBSqlite::addModule - Given an empty module name.");
        return -1;
    }

    moduleId = 0;

    sqlite3_stmt * statement;
    char stmt[1024];
    sqlite3_snprintf(1024, stmt, "SELECT module_id FROM modules WHERE name = '%q';",
                     name.c_str());

    if (sqlite3_prepare_v2(m_db, stmt, -1, &statement, 0) == SQLITE_OK)
    {
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW)
        {
            // Already exists, return module_id
            moduleId = sqlite3_column_int(statement, 0);
        }
        else
        {
            // Create a new module record.
            char insertStmt[1024];
            char * errmsg;
            sqlite3_snprintf(1024, insertStmt,
                "INSERT INTO modules (module_id, name, description) VALUES (NULL, '%q', '%q');",
                name.c_str(), description.c_str());
            if (sqlite3_exec(m_db, insertStmt, NULL, NULL, &errmsg) == SQLITE_OK)
            {
                moduleId = (int)sqlite3_last_insert_rowid(m_db);
            }
            else
            {
                std::wstringstream msg;
                msg << L"WombatTskImgDBSqlite::addModule - Error adding record to modules table: " << errmsg;
                LOGERROR(msg.str());
                sqlite3_free(errmsg);
            }
        }
        sqlite3_finalize(statement);
    }
    else
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::addModule - Failed to prepare statement: " << stmt;
        LOGERROR(msg.str());
    }

    if (moduleId == 0)
        return -1;

    return 0;
}

/**
 * Insert the module status record.
 * @param file_id file_id
 * @param module_id module_id
 * @param status Status of module
 * @returns 0 on success, -1 on error.
 */
int WombatTskImgDBSqlite::setModuleStatus(uint64_t file_id, int module_id, int status)
{
    int rc = -1;

    if (!m_db)
        return rc;

    char * errmsg;
    std::stringstream stmt;
    stmt << "INSERT INTO module_status (file_id, module_id, status) VALUES (" <<
        file_id << ", " <<  module_id << ", " << status << ")";

    if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, &errmsg) == SQLITE_OK) {
        rc = 0;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::setModuleStatus - Error adding data to module_status table: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlite3_free(errmsg);
    }
    return rc;
}

/**
 * Get a list of TskModuleStatus.
 * @param moduleInfoList A list of TskModuleStatus (output)
 * @returns 0 on success, -1 on error.
*/
int WombatTskImgDBSqlite::getModuleInfo(std::vector<TskModuleInfo> & moduleInfoList) const
{
    int rc = -1;

    if (!m_db)
        return rc;

    stringstream stmt;
    stmt << "SELECT module_id, name, description FROM modules ORDER BY module_id";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        TskModuleInfo moduleInfo;
        while (sqlite3_step(statement) == SQLITE_ROW) {
            moduleInfo.module_id = (int)sqlite3_column_int64(statement, 0);
            moduleInfo.module_name = (char *)sqlite3_column_text(statement, 1);
            moduleInfo.module_description = (char *)sqlite3_column_text(statement, 2);
            moduleInfoList.push_back(moduleInfo);
        }
        sqlite3_finalize(statement);
        rc = 0;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getModuleInfo - Error querying modules table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return rc;
}

/**
 * Get a list of TskModuleStatus.
 * @param moduleStatusList A list of TskModuleStatus (output)
 * @returns 0 on success, -1 on error.
*/
int WombatTskImgDBSqlite::getModuleErrors(std::vector<TskModuleStatus> & moduleStatusList) const
{
    int rc = -1;

    if (!m_db)
        return rc;

    stringstream stmt;
    stmt << "SELECT f.file_id, m.name, ms.status FROM module_status ms, files f, modules m"
        << " WHERE ms.status != 0 AND ms.file_id = f.file_id AND m.module_id = ms.module_id"
        << " ORDER BY f.file_id";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        TskModuleStatus moduleStatus;
        while (sqlite3_step(statement) == SQLITE_ROW) {
            moduleStatus.file_id = (uint64_t)sqlite3_column_int64(statement, 0);
            moduleStatus.module_name = (char *)sqlite3_column_text(statement, 1);
            moduleStatus.status = (int)sqlite3_column_int(statement, 2);
            moduleStatusList.push_back(moduleStatus);
        }
        sqlite3_finalize(statement);
        rc = 0;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getModuleErrors - Error querying module_status table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    // Find report module errors. These have file_id = 0.
    stmt.str("");
    stmt << "SELECT 0, m.name, ms.status FROM module_status ms, modules m"
         << " WHERE ms.status != 0 AND ms.file_id = 0 AND m.module_id = ms.module_id";

    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        TskModuleStatus moduleStatus;
        while (sqlite3_step(statement) == SQLITE_ROW) {
            moduleStatus.file_id = (uint64_t)sqlite3_column_int64(statement, 0);
            moduleStatus.module_name = (char *)sqlite3_column_text(statement, 1);
            moduleStatus.status = (int)sqlite3_column_int(statement, 2);
            moduleStatusList.push_back(moduleStatus);
        }
        sqlite3_finalize(statement);
        rc = 0;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getModuleErrors - Error querying module_status table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return rc;
}

/*
 * Return a file name associated with a file_id, prefer Cfilename, otherwise name in the files table.
 * @param file_id file id
 * @returns file name as std::string
 */
std::string WombatTskImgDBSqlite::getFileName(uint64_t file_id) const
{
    std::string name;

    if (!m_db)
        return name;

    name = getCfileName(file_id);
    if (name == "") {
        TskFileRecord fileRecord;
        if (getFileRecord(file_id, fileRecord) == 0)
            name = fileRecord.name;
    }
    return name;
}


TskImgDB::KNOWN_STATUS WombatTskImgDBSqlite::getKnownStatus(const uint64_t fileId) const
{
    int retval = -1;

    if (!m_db)
        return (KNOWN_STATUS)retval;

    stringstream stmt;
    stmt << "SELECT known FROM file_hashes WHERE file_id = " << fileId;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        if(sqlite3_step(statement) == SQLITE_ROW) {
            retval = (int)sqlite3_column_int(statement, 0);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getKnownStatus - Error getting known status " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }

    return (KNOWN_STATUS)retval;
}


/**
 * Add a new row to the unalloc_img_status table, returning the unalloc_img_id.
 * @param unallocImgId unalloc_img_id (output)
 * @returns -1 on error, 0 on success.
 */
int WombatTskImgDBSqlite::addUnallocImg(int & unallocImgId)
{
    int rc = -1;

    if (!m_db)
        return rc;

    std::stringstream stmt;
    stmt << "INSERT INTO unalloc_img_status (unalloc_img_id, status) VALUES (NULL, " << TskImgDB::IMGDB_UNALLOC_IMG_STATUS_CREATED << ")";
    char * errmsg;
    if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, &errmsg) == SQLITE_OK) {
        unallocImgId = (int)sqlite3_last_insert_rowid(m_db);
        rc = 0;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::addUnallocImg - Error adding unalloc_img_status table: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlite3_free(errmsg);
    }
    return rc;
}

/**
 * Set the status in the unalloc_img_status table given the unalloc_img_id.
 * @param unallocImgId unalloc_img_id
 * @param status status of unalloc_img_id
 * @returns -1 on error, 0 on success.
 */
int WombatTskImgDBSqlite::setUnallocImgStatus(int unallocImgId, TskImgDB::UNALLOC_IMG_STATUS status)
{
    int rc = -1;

    if (!m_db)
        return rc;

    std::stringstream stmt;
    stmt << "UPDATE unalloc_img_status SET status = " << status << " WHERE unalloc_img_id = " << unallocImgId;
    char * errmsg;
    if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, &errmsg) == SQLITE_OK) {
        rc = 0;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::addUnallocImg - Error adding unalloc_img_status table: " << errmsg;
        LOGERROR(infoMessage.str());
        sqlite3_free(errmsg);
    }
    return rc;
}

/**
 * Get the status of the unalloc_img_status table given the unalloc_img_id.
 * Can throws TskException.
 * @param unallocImgId unalloc_img_id
 * @returns TskImgDB::UNALLOC_IMG_STATUS
 */
TskImgDB::UNALLOC_IMG_STATUS WombatTskImgDBSqlite::getUnallocImgStatus(int unallocImgId) const
{
    if (!m_db)
        throw TskException("Database not initialized.");

    int status = 0;
    stringstream stmt;
    stmt << "SELECT status FROM unalloc_img_status WHERE unalloc_img_id = " << unallocImgId;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            status = (int)sqlite3_column_int(statement, 0);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getUnallocImgStatus - Error getting unalloc_img_status: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return (TskImgDB::UNALLOC_IMG_STATUS)status;
}

/**
 * Get all the unalloc_img_status table.
 * @param unallocImgStatusList A vector of TskUnallocImgStatusRecord (output)
 * @returns -1 on error, 0 on success.
 */
int WombatTskImgDBSqlite::getAllUnallocImgStatus(std::vector<TskUnallocImgStatusRecord> & unallocImgStatusList) const
{
    int rc = -1;
    unallocImgStatusList.clear();

    if (!m_db)
        return rc;

    stringstream stmt;
    stmt << "SELECT unalloc_img_id, status FROM unalloc_img_status";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            TskUnallocImgStatusRecord record;
            record.unallocImgId = (int)sqlite3_column_int(statement, 0);
            record.status = (TskImgDB::UNALLOC_IMG_STATUS)sqlite3_column_int(statement, 1);
            unallocImgStatusList.push_back(record);
        }
        rc = 0;
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getAllUnallocImgStatus - Error getting unalloc_img_status: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return rc;
}

/**
 * Find and add all the unused sectors (unallocated and uncarved bytes) in the given unallocImgId
 * @param unallocImgId The unalloc image id.
 * @param unusedSectorsList A vector of TskUnusedSectorsRecord
 * @returns -1 on error, 0 on success.
 */
int WombatTskImgDBSqlite::addUnusedSectors(int unallocImgId, std::vector<TskUnusedSectorsRecord> & unusedSectorsList)
{
    assert(unallocImgId > 0);
    int rc = -1;
    if (!m_db)
        return rc;

    std::stringstream stmt;
    stmt << "SELECT vol_id, unalloc_img_sect_start, sect_len, orig_img_sect_start FROM alloc_unalloc_map "
        "WHERE unalloc_img_id = " << unallocImgId << " ORDER BY orig_img_sect_start ASC";

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        vector<TskAllocUnallocMapRecord> allocUnallocMapList;

        while (sqlite3_step(statement) == SQLITE_ROW) {
            TskAllocUnallocMapRecord record;
            record.vol_id = (int)sqlite3_column_int(statement, 0);
            record.unalloc_img_id = unallocImgId;
            record.unalloc_img_sect_start = (uint64_t)sqlite3_column_int64(statement, 1);
            record.sect_len = (uint64_t)sqlite3_column_int64(statement, 2);
            record.orig_img_sect_start = (uint64_t)sqlite3_column_int64(statement, 3);
            allocUnallocMapList.push_back(record);
        }
        sqlite3_finalize(statement);

        for (std::vector<TskAllocUnallocMapRecord>::const_iterator it = allocUnallocMapList.begin();
             it != allocUnallocMapList.end(); it++)
        {
            // Sector position tracks our position through the unallocated map record.
            uint64_t sectPos = it->orig_img_sect_start;

            uint64_t endSect = it->orig_img_sect_start + it->sect_len;

            // Retrieve all carved_sector records in range for this section of unallocated space.
            stmt.str("");
            stmt << "SELECT cs.sect_start, cs.sect_len FROM carved_files cf, carved_sectors cs"
                << " WHERE cf.file_id = cs.file_id AND cs.sect_start >= " << it->orig_img_sect_start
                << " AND cs.sect_start < " << endSect << " ORDER BY cs.sect_start ASC";

            if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
                while (sqlite3_step(statement) == SQLITE_ROW) {
                    uint64_t cfileSectStart = (uint64_t)sqlite3_column_int64(statement, 0);
                    uint64_t cfileSectLen = (uint64_t)sqlite3_column_int64(statement, 1);
                    if (cfileSectStart > sectPos)
                    {
                        // We have a block of unused sectors between this position in the unallocated map
                        // and the start of the carved file.
                        addUnusedSector(sectPos, cfileSectStart, it->vol_id, unusedSectorsList);
                    }

                    sectPos = cfileSectStart + cfileSectLen;
                }

                // Handle case where there is slack at the end of the unalloc range
                if (sectPos < endSect)
                    addUnusedSector(sectPos, endSect, it->vol_id, unusedSectorsList);

                sqlite3_finalize(statement);
            } else {
                std::wstringstream infoMessage;
                infoMessage << L"WombatTskImgDBSqlite::addUnusedSectors - Error querying carved_files, carved_sectors table: " << sqlite3_errmsg(m_db);
                LOGERROR(infoMessage.str());
            }
        }
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::addUnusedSectors - Error querying alloc_unalloc_map table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
    }
    return 0;
}

/**
 * Add one unused sector to the database, add it to the files and unused_sectors tables.
 * @param sectStart Unused sector start.
 * @param sectEnd Unused sector end.
 * @param volId Volume Id of the unused sector.
 * @param unusedSectorsList A vector of TskUnusedSectorsRecord (output)
 * @returns -1 on error, 0 on success.
 */
int WombatTskImgDBSqlite::addUnusedSector(uint64_t sectStart, uint64_t sectEnd, int volId, std::vector<TskUnusedSectorsRecord> & unusedSectorsList)
{
    assert(sectEnd > sectStart);
    int rc = -1;
    if (!m_db)
        return rc;

    std::stringstream stmt;

    std::string maxUnused = GetSystemProperty("MAX_UNUSED_FILE_SIZE_BYTES");
    const uint64_t maxUnusedFileSizeBytes = maxUnused.empty() ? (50 * 1024 * 1024) : Poco::NumberParser::parse64(maxUnused);

    uint64_t maxUnusedSectorSize = maxUnusedFileSizeBytes / 512;
    uint64_t sectorIndex = 0;
    uint64_t sectorCount = (sectEnd - sectStart) / maxUnusedSectorSize;

    while (sectorIndex <= sectorCount) {
        uint64_t thisSectStart = sectStart + (sectorIndex * maxUnusedSectorSize);
        uint64_t thisSectEnd = thisSectStart + (std::min)(maxUnusedSectorSize, sectEnd - thisSectStart);

        stmt.str("");
        stmt << "INSERT INTO files (file_id, type_id, name, par_file_id, dir_type, meta_type,"
            "dir_flags, meta_flags, size, ctime, crtime, atime, mtime, mode, uid, gid, status, full_path) "
            "VALUES (NULL, " << IMGDB_FILES_TYPE_UNUSED << ", " << "'ufile'"
            << ", NULL, " <<  TSK_FS_NAME_TYPE_REG << ", " <<  TSK_FS_META_TYPE_REG << ", "
            << TSK_FS_NAME_FLAG_UNALLOC << ", " << TSK_FS_META_FLAG_UNALLOC << ", "
            << (thisSectEnd - thisSectStart) * 512 << ", NULL, NULL, NULL, NULL, NULL, NULL, NULL, " << IMGDB_FILES_STATUS_READY_FOR_ANALYSIS << "," << "'ufile'" << ")";

        if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, NULL) == SQLITE_OK) {

            TskUnusedSectorsRecord record;

            // get the file_id from the last insert
            record.fileId = sqlite3_last_insert_rowid(m_db);
            record.sectStart = thisSectStart;
            record.sectLen = thisSectEnd - thisSectStart;

            std::stringstream name;
            name << "ufile_" << thisSectStart << "_" << thisSectEnd << "_" << record.fileId;
            stmt.str("");
            char *item;
            item = sqlite3_mprintf("%Q", name.str().c_str());
            stmt << "UPDATE files SET name = " << item << ", full_path = "
                << item << " WHERE file_id = " << record.fileId;
            sqlite3_free(item);

            if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, NULL) != SQLITE_OK) {
                std::wstringstream infoMessage;
                infoMessage << L"WombatTskImgDBSqlite::addUnusedSector - Error update into files table: " << sqlite3_errmsg(m_db);
                LOGERROR(infoMessage.str());
                rc = -1;
                break;
            }

            stmt.str("");
            stmt << "INSERT INTO unused_sectors (file_id, sect_start, sect_len, vol_id) VALUES ("
                 << record.fileId << ", " << record.sectStart << ", " << record.sectLen << ", " << volId << ")";

            if (sqlite3_exec(m_db, stmt.str().c_str(), NULL, NULL, NULL) != SQLITE_OK) {
                std::wstringstream infoMessage;
                infoMessage << L"WombatTskImgDBSqlite::addUnusedSector - Error insert into unused_sectors table: " << sqlite3_errmsg(m_db);
                LOGERROR(infoMessage.str());
                rc = -1;
                break;
            }

            unusedSectorsList.push_back(record);
            rc = 0;

        } else {
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::addUnusedSector - Error insert into files table: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            rc = -1;
            break;
        }
        sectorIndex++;
    } // while
    return rc;
}

/**
 * Get unused sector record given a file id.
 * @param fileId File id of the unused sector.
 * @param unusedSectorsRecord TskUnusedSectorsRecord (output)
 * @returns -1 on error, 0 on success.
 */
int WombatTskImgDBSqlite::getUnusedSector(uint64_t fileId, TskUnusedSectorsRecord & unusedSectorsRecord) const
{
    int rc = -1;
    if (!m_db)
        return rc;

    std::stringstream stmt;
    stmt << "SELECT sect_start, sect_len FROM unused_sectors WHERE file_id = " << fileId;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_ROW) {
            unusedSectorsRecord.fileId = fileId;
            unusedSectorsRecord.sectStart = (uint64_t)sqlite3_column_int64(statement, 0);
            unusedSectorsRecord.sectLen = (uint64_t)sqlite3_column_int64(statement, 1);
            rc = 0;
        } else {
            std::wstringstream msg;
            msg << L"TskDBSqlite::getUnusedSector - Error querying unused_sectors table for file_id "
                << fileId ;
            LOGERROR(msg.str());
        }
    } else {
        std::wstringstream msg;
        msg << L"TskDBSqlite::getUnusedSector - Error querying unused_sectors table: "
            << sqlite3_errmsg(m_db) ;
        LOGERROR(msg.str());
    }
    return rc;
}

///BLACKBOARD FUNCTIONS
/**
 * Add the given blackboard attribute to the database
 * @param attr input attribute. should be fully populated
 */
void WombatTskImgDBSqlite::addBlackboardAttribute(TskBlackboardAttribute attr)
{
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    char *item;
    sqlite3_stmt * statement;

    str << "INSERT INTO blackboard_attributes (artifact_id, source, context, attribute_type_id, value_type, "
        "value_byte, value_text, value_int32, value_int64, value_double, obj_id) VALUES (";
        str << attr.getArtifactID() << ", ";
    item = sqlite3_mprintf("%Q", attr.getModuleName().c_str()); str << item << ", ";
    sqlite3_free(item);
    item = sqlite3_mprintf("%Q", attr.getContext().c_str()); str << item << ", ";
    sqlite3_free(item);
    str << attr.getAttributeTypeID() << ", ";
    str << attr.getValueType() << ", ";
    switch (attr.getValueType()) {
        case TSK_BYTE:
            str << " ?, '', 0, 0, 0.0";
            break;
        case TSK_STRING:
            item = sqlite3_mprintf("%Q", attr.getValueString().c_str());
            str << " '', " << item << ", 0, 0, 0.0";
            sqlite3_free(item);
            break;
        case TSK_INTEGER:
            str << " '', '', " << attr.getValueInt() << ",     0, 0.0";
            break;
        case TSK_LONG:
            str << " '', '', 0, " << attr.getValueLong() << ",     0.0";
            break;
        case TSK_DOUBLE:
            str << " '', '', 0, 0, " << setprecision(20) << attr.getValueDouble();
            break;
    };
    str << ", " << attr.getObjectID();
    str << ")";

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        int result = SQLITE_OK;
        unsigned char *pBuf = 0;
        if (attr.getValueType() == TSK_BYTE) {
            // Bind the byte vector
            int a_size = attr.getValueBytes().size();
            pBuf = new unsigned char[a_size];
            for (int i = 0; i < a_size; i++) {
                pBuf[i] = attr.getValueBytes()[i];
            }
            result = sqlite3_bind_blob(statement, 1, pBuf, a_size, SQLITE_STATIC);
        }
        if (result == SQLITE_OK) {
            result = sqlite3_step(statement);
            if (!(result == SQLITE_ROW || result == SQLITE_DONE)) {
                sqlite3_finalize(statement);
                if (pBuf) delete [] pBuf;
                throw TskException("WombatTskImgDBSqlite::addBlackboardAttribute - Insert failed");
            }
        } else {
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::addBlackboardAttribute - Error in sqlite3_bind_blob: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::addBlackboardAttribute - Insert failed");
        }
        sqlite3_finalize(statement);
        if (pBuf) delete [] pBuf;
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::addBlackboardAttribute - Error adding data to blackboard table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::addBlackboardAttribute - Insert failed");
    }
}

/**
 * Get the display name for the given artifact type id
 * @param artifactTypeID artifact type id
 * @returns display name
 */
string WombatTskImgDBSqlite::getArtifactTypeDisplayName(int artifactTypeID){
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;
    std::string displayName = "";

    str << "SELECT display_name FROM blackboard_artifact_types WHERE artifact_type_id = " << artifactTypeID;

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK){
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            displayName = (char *)sqlite3_column_text(statement, 0);
        }
        else{
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::getArtifactTypeDisplayName: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::getArtifactTypeDisplayName - No artifact type with that ID");
        }
        sqlite3_finalize(statement);
        return displayName;
    }
    else{
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getArtifactTypeDisplayName: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::getArtifactTypeDisplayName - Select failed");
    }
}

/**
 * Get the artifact type id for the given artifact type string
 * @param artifactTypeString display name
 * @returns artifact type id
 */
int WombatTskImgDBSqlite::getArtifactTypeID(string artifactTypeString){
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;
    int typeID;

    str << "SELECT artifact_type_id FROM blackboard_artifact_types WHERE type_name = " << artifactTypeString;

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK){
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            typeID = (int) sqlite3_column_int(statement, 0);
        }
        else{
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::getArtifactTypeID: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::getArtifactTypeID - No artifact type with that name");
        }
        sqlite3_finalize(statement);
        return typeID;
    }
    else{
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getArtifactTypeID: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::getArtifactTypeID - Select failed");
    }
}

/**
 * Get the artifact type name for the given artifact type id
 * @param artifactTypeID id
 * @returns artifact type name
 */
string WombatTskImgDBSqlite::getArtifactTypeName(int artifactTypeID){
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;
    std::string typeName = "";

    str << "SELECT type_name FROM blackboard_artifact_types WHERE artifact_type_id = " << artifactTypeID;

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK){
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            typeName = (char *)sqlite3_column_text(statement, 0);
        }
        else{
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::getArtifactTypeName: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::getArtifactTypeName - No artifact type with that ID");
        }
        sqlite3_finalize(statement);
        return typeName;
    }
    else{
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getArtifactTypeName: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::getArtifactTypeName - Select failed");
    }
}

/**
 * Get the display name for the given attribute type id
 * @param attributeTypeID attribute type id
 * @returns display name
 */
string WombatTskImgDBSqlite::getAttributeTypeDisplayName(int attributeTypeID){
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;
    std::string displayName = "";

    str << "SELECT display_name FROM blackboard_attribute_types WHERE attribute_type_id = " << attributeTypeID;

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK){
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            displayName = (char *)sqlite3_column_text(statement, 0);
        }
        else{
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::getAttributeTypeDisplayName: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::getAttributeTypeDisplayName - No attribute type with that ID");
        }
        sqlite3_finalize(statement);
        return displayName;
    }
    else{
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getAttributeTypeDisplayName: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::getAttributeTypeDisplayName - Select failed");
    }
}

/**
 * Get the attribute type id for the given artifact type string
 * @param attributeTypeString display name
 * @returns attribute type id
 */
int WombatTskImgDBSqlite::getAttributeTypeID(string attributeTypeString){
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;
    int typeID;

    str << "SELECT attribute_type_id FROM blackboard_attribute_types WHERE type_name = " << attributeTypeString;

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK){
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            typeID = (int) sqlite3_column_int(statement, 0);
        }
        else{
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::getAttributeTypeID: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::getAttributeTypeID - No artifact type with that name");
        }
        sqlite3_finalize(statement);
        return typeID;
    }
    else{
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getAttributeTypeID: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::getAttributeTypeID - Select failed");
    }
}

/**
 * Get the attribute type name for the given artifact type id
 * @param attributeTypeID id
 * @returns attribute type name
 */
string WombatTskImgDBSqlite::getAttributeTypeName(int attributeTypeID){
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;
    std::string typeName = "";

    str << "SELECT type_name FROM blackboard_attribute_types WHERE attribute_type_id = " << attributeTypeID;

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK){
        int result = sqlite3_step(statement);
        if (result == SQLITE_ROW) {
            typeName = (char *)sqlite3_column_text(statement, 0);
        }
        else{
            std::wstringstream infoMessage;
            infoMessage << L"WombatTskImgDBSqlite::getAttributeTypeName: " << sqlite3_errmsg(m_db);
            LOGERROR(infoMessage.str());
            throw TskException("WombatTskImgDBSqlite::getAttributeTypeName - No attribute type with that ID");
        }
        sqlite3_finalize(statement);
        return typeName;
    }
    else{
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::getAttributeTypeName: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::getAttributeTypeName - Select failed");
    }
}

/**
 * Get all artifacts by performing a SQL Select statement with the given where clause.
 * @param condition The SQL select where clause that should be used in the query.
 * @returns vector of matching artifacts
 */
vector<TskBlackboardArtifact> WombatTskImgDBSqlite::getMatchingArtifacts(string condition)
{
    if (!m_db)
        throw TskException("No database.");

    vector<TskBlackboardArtifact> artifacts;
    std::string stmt("SELECT blackboard_artifacts.artifact_id, blackboard_artifacts.obj_id, blackboard_artifacts.artifact_type_id FROM blackboard_artifacts");

    constructStmt(stmt, condition);

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            int artifactTypeID = sqlite3_column_int(statement, 2);

            artifacts.push_back(TskImgDB::createArtifact(sqlite3_column_int64(statement, 0), sqlite3_column_int64(statement, 1), artifactTypeID));
        }
        sqlite3_finalize(statement);
    } else
    {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getMatchingArtifacts - Error getting artifacts: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
        throw TskException("WombatTskImgDBSqlite::getMatchingArtifacts - Select failed");
    }
    return artifacts;
}

/**
 * Get all attributes with that match the given where clause
 * @param condition where clause to use for matching
 * @returns vector of matching attributes
 */
vector<TskBlackboardAttribute> WombatTskImgDBSqlite::getMatchingAttributes(string condition)
{
    if (!m_db)
        throw TskException("No database.");

    vector<TskBlackboardAttribute> attributes;
    std::string stmt("SELECT blackboard_attributes.artifact_id, blackboard_attributes.source, blackboard_attributes.context, blackboard_attributes.attribute_type_id, blackboard_attributes.value_type, blackboard_attributes.value_byte, blackboard_attributes.value_text, blackboard_attributes.value_int32, blackboard_attributes.value_int64, blackboard_attributes.value_double, blackboard_attributes.obj_id FROM blackboard_attributes ");

    constructStmt(stmt, condition);

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            int blobSize = sqlite3_column_bytes(statement, 6);
            const unsigned char *pBlob = (const unsigned char *)sqlite3_column_blob(statement, 6);
            vector<unsigned char> bytes;
            bytes.reserve(blobSize);
            for (int i = 0; i < blobSize; i++) {
                bytes.push_back((unsigned char)pBlob[i]);
            }

            attributes.push_back(TskImgDB::createAttribute(sqlite3_column_int64(statement, 0),sqlite3_column_int(statement, 3), sqlite3_column_int64(statement, 10), std::string((char *)sqlite3_column_text(statement, 1)),
                std::string((char *)sqlite3_column_text(statement, 2)), (TSK_BLACKBOARD_ATTRIBUTE_VALUE_TYPE) sqlite3_column_int(statement, 4), sqlite3_column_int(statement, 7),
                sqlite3_column_int64(statement, 8), sqlite3_column_double(statement, 9), std::string((char *)sqlite3_column_text(statement, 6)), bytes));
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getMatchingAttributes - Error getting attributes: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
        throw TskException("WombatTskImgDBSqlite::getMatchingAttributes - Select failed");
    }
    return attributes;
}

/**
 * Create a new blackboard artifact with the given type id and file id
 * @param artifactTypeID artifact type id
 * @param file_id associated file id
 * @returns the new artifact
 */
TskBlackboardArtifact WombatTskImgDBSqlite::createBlackboardArtifact(uint64_t file_id, int artifactTypeID)
{
    if (!m_db)
        throw TskException("No database.");

    uint64_t artifactId = 0;
    std::stringstream str;
    sqlite3_stmt * statement;

    str << "INSERT INTO blackboard_artifacts (artifact_id, obj_id, artifact_type_id) VALUES (NULL, " << file_id << ", " << artifactTypeID << ")";

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        if (!(sqlite3_step(statement) == SQLITE_DONE)) {
            sqlite3_finalize(statement);
            throw TskException("WombatTskImgDBSqlite::addBlackboardInfo - Insert failed");
        }
        // select max(artifact_id) from blackboard
        str.str("");
        str << "SELECT artifact_id from blackboard_artifacts WHERE obj_id = " << file_id << " AND artifact_type_id = " << artifactTypeID;
        sqlite3_finalize(statement);
        if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
            while(sqlite3_step(statement) == SQLITE_ROW) {
                uint64_t newID = sqlite3_column_int64(statement, 0);
                if(newID > artifactId)
                    artifactId = newID;
            }
        } else {
            sqlite3_finalize(statement);
            throw TskException("WombatTskImgDBSqlite::newBlackboardArtifact - Select artifact_id failed");
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::newBlackboardArtifact - Error adding new artifact: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::newBlackboardArtifact - Insert failed");
    }

    return TskImgDB::createArtifact(artifactId, file_id, artifactTypeID);
}

/**
 * Add a new artifact type with the given name, display name and id
 * @param artifactTypeName type name
 * @param displayName display name
 * @param typeID type id
 */
void WombatTskImgDBSqlite::addArtifactType(int typeID, string artifactTypeName, string displayName)
{
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;

    str << "SELECT * FROM blackboard_artifact_types WHERE type_name = '" << artifactTypeName << "'";

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        if (!(sqlite3_step(statement) == SQLITE_ROW)) {
            sqlite3_finalize(statement);
            str.str("");
            str << "INSERT INTO blackboard_artifact_types (artifact_type_id, type_name, display_name) VALUES (" << typeID << " , '" << artifactTypeName << "', '" << displayName << "')";
            if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
                if (!(sqlite3_step(statement) == SQLITE_DONE)) {
                    sqlite3_finalize(statement);
                    std::wstringstream infoMessage;
                    infoMessage << L"WombatTskImgDBSqlite::addArtifactType - Error adding data to blackboard table: " << sqlite3_errmsg(m_db);
                    LOGERROR(infoMessage.str());
                    throw TskException("WombatTskImgDBSqlite::addArtifactType - Artifact type insert failed");
                }
            }
        }
        else{
            sqlite3_finalize(statement);
            throw TskException("WombatTskImgDBSqlite::addArtifactType - Artifact type with that name already exists");
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::addArtifactType - Error adding data to blackboard table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::addArtifactType - Insert failed");
    }
}

/**
 * Add a new attribute type with the given name, display name and id
 * @param attributeTypeName type name
 * @param displayName display name
 * @param typeID type id
 */
void WombatTskImgDBSqlite::addAttributeType(int typeID, string attributeTypeName, string displayName)
{
    if (!m_db)
        throw TskException("No database.");

    std::stringstream str;
    sqlite3_stmt * statement;

    str << "SELECT * FROM blackboard_attribute_types WHERE type_name = '" << attributeTypeName << "'";

    if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
        if (!(sqlite3_step(statement) == SQLITE_ROW)) {
            sqlite3_finalize(statement);
            str.str("");
            str << "INSERT INTO blackboard_attribute_types (attribute_type_id, type_name, display_name) VALUES (" << typeID << " , '" << attributeTypeName << "', '" << displayName << "')";
            if (sqlite3_prepare_v2(m_db, str.str().c_str(), -1, &statement, 0) == SQLITE_OK) {
                if (!(sqlite3_step(statement) == SQLITE_DONE)) {
                    sqlite3_finalize(statement);
                    std::wstringstream infoMessage;
                    infoMessage << L"WombatTskImgDBSqlite::addAttributeType - Error adding data to blackboard table: " << sqlite3_errmsg(m_db);
                    LOGERROR(infoMessage.str());
                    throw TskException("WombatTskImgDBSqlite::addAttributeType - Attribute type insert failed");
                }
            }
        } else {
            sqlite3_finalize(statement);
            throw TskException("WombatTskImgDBSqlite::addAttributeType - Attribute type with that name already exists");
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream infoMessage;
        infoMessage << L"WombatTskImgDBSqlite::addAttributeType - Error adding data to blackboard table: " << sqlite3_errmsg(m_db);
        LOGERROR(infoMessage.str());
        throw TskException("WombatTskImgDBSqlite::addAttributeType - Insert failed");
    }
}

/**
 * Get all artifacts with the given type id, type name, and file id
 * @param artifactTypeID type id
 * @param artifactTypeName type name
 * @param file_id file id
 */
vector<TskBlackboardArtifact> WombatTskImgDBSqlite::getArtifactsHelper(uint64_t file_id, int artifactTypeID, string artifactTypeName)
{
    if (!m_db)
        throw TskException("No database.");

    vector<TskBlackboardArtifact> artifacts;
    std::stringstream stmt;
    stmt << "SELECT artifact_id, obj_id, artifact_type_id FROM blackboard_artifacts WHERE obj_id = " << file_id << " AND artifact_type_id = " << artifactTypeID;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            int artifactTypeID = sqlite3_column_int(statement, 2);

            artifacts.push_back(TskImgDB::createArtifact(sqlite3_column_int64(statement, 0), file_id, artifactTypeID));
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::getArtifactsHelper - Error getting artifacts: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
        throw TskException("WombatTskImgDBSqlite::getArtifactsHelper - Select failed");
    }
    return artifacts;
}

vector<int> WombatTskImgDBSqlite::findAttributeTypes(int artifactTypeId)
{
    if (!m_db) {
        throw TskException("No database.");
    }
    vector<int> attrTypes;
    std::stringstream stmt;
    stmt << "SELECT DISTINCT(attribute_type_id) FROM blackboard_attributes JOIN blackboard_artifacts ON blackboard_attributes.artifact_id = blackboard_artifacts.artifact_id WHERE artifact_type_id = " << artifactTypeId;

    sqlite3_stmt * statement;
    if (sqlite3_prepare_v2(m_db, stmt.str().c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            int artifactTypeID = sqlite3_column_int(statement, 0);

            attrTypes.push_back(artifactTypeID);
        }
        sqlite3_finalize(statement);
    } else {
        std::wstringstream msg;
        msg << L"WombatTskImgDBSqlite::findAttributeTypes - Error finding attribute types: " << sqlite3_errmsg(m_db);
        LOGERROR(msg.str());
        throw TskException("WombatTskImgDBSqlite::findAttributeTypes - Select failed");
    }
    return attrTypes;
}

std::string WombatTskImgDBSqlite::quote(const std::string str) const
{
    char *item = sqlite3_mprintf("%Q", str.c_str());
    std::string returnStr(item);
    sqlite3_free(item);
    return returnStr;
}

void WombatTskImgDBSqlite::executeStatement(const std::string &stmtToExecute, sqlite3_stmt *&statement, const std::string &caller) const
{
    if (sqlite3_prepare_v2(m_db, stmtToExecute.c_str(), -1, &statement, 0) != SQLITE_OK)
    {
        sqlite3_finalize(statement);
        std::ostringstream msg;
        msg << caller << " : error executing " << stmtToExecute << " : " << sqlite3_errmsg(m_db);
        throw TskException(msg.str());
    }
}
