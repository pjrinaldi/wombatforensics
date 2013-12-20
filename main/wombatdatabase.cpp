#include "wombatdatabase.h"

std::string WombatDatabase::GetTime()
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

WombatDatabase::WombatDatabase(WombatVariable* wombatvarptr)
{
    wombatptr = wombatvarptr;
}

bool WombatDatabase::FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
// CALLED THE CREATE WOMBATDB() TO INCLUDE CASES AND SETTINGS.
void WombatDatabase::CreateCaseDB(void)
{
    #define IMGDB_CHUNK_SIZE 1024*1024*1 // what size chunks should the database use when growing and shrinking
    #define IMGDB_MAX_RETRY_COUNT 50    // how many times will we retry a SQL statement
    #define IMGDB_RETRY_WAIT 100   // how long (in milliseconds) are we willing to wait between retries
    // set page size -- 4k is much faster on Windows than the default
    //executeStatement("PRAGMA page_size = 4096;", casestatement, "TskImgDBSqlite::initialize");
    //sqlite3_finalize(casestatement);

    // we don't have a mechanism to recover from a crash anyway
    //executeStatement("PRAGMA synchronous = 0;", casestatement, "TskImgDBSqlite::initialize");
    //sqlite3_finalize(casestatement);
    
    std::vector<const char *> wombatTableSchema;
    wombatTableSchema.clear();
    wombatTableSchema.push_back("CREATE TABLE job(jobid INTEGER PRIMARY KEY, type INTEGER, state INTEGER, filecount INTEGER, processcount INTEGER, caseid INTEGER, evidenceid INTEGER, start TEXT, end TEXT);");
    wombatTableSchema.push_back("CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INT);");
    wombatTableSchema.push_back("CREATE TABLE dataruns(id INTEGER PRIMARY KEY, objectid INTEGER, fullpath TEXT, seqnum INTEGER, start INTEGER, length INTEGER, datattype INTEGER, originalsectstart INTEGER, allocationstatus INTEGER);");
    wombatTableSchema.push_back("CREATE TABLE artifacts(id INTEGER PRIMARY KEY, objectid INTEGER, context TEXT, attrtype INTEGER, valuetype INTEGER value BLOB);");
    wombatTableSchema.push_back("CREATE TABLE msglog(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msg TEXT, datetime TEXT);");
    wombatTableSchema.push_back("CREATE TABLE data(objectid INTEGER PRIMARY KEY, type INTEGER, name TEXT, fullpath TEXT, parentid INTEGER, flags INTEGER, size INTEGER, sectstart INTEGER, sectlength INTEGER, dirtype INTEGER, metattype INTEGER, dirflags INTEGER, metaflags INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, mode INTEGER, uid INTEGER, gid INTEGER, status INTEGER, md5 TEXT, sha1 TEXT, sha_256 TEXT, sha_512 TEXT, known INTEGER, indoenumber INTEGER, mftattrid INTEGER, mftattrtype INTEGER, byteoffset INTEGER, blockcount INTEGER, rootinum INTEGER, firstinum INTEGER, lastinum INTEGER, derivationdetails TEXT);");

    if(sqlite3_open(wombatptr->caseobject.dbname.toStdString().c_str(), &casedb) == SQLITE_OK)
    {
        const char* tblschema;
        foreach(tblschema, wombatTableSchema)
        {
            if(sqlite3_prepare_v2(casedb, tblschema, -1, &casestatement, NULL) == SQLITE_OK)
            {
                int ret = sqlite3_step(casestatement);
                if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                    wombatptr->curerrmsg = QString(sqlite3_errmsg(casedb));
            }
            else
                wombatptr->curerrmsg = QString(sqlite3_errmsg(casedb));
            sqlite3_finalize(casestatement);
        }
    }
    else
        wombatptr->curerrmsg = QString(sqlite3_errmsg(casedb));

    wombatptr->curerrmsg = "";
}

void WombatDatabase::CreateAppDB()
{
    sqlite3_stmt* tmpstmt;
    if(sqlite3_open(wombatptr->wombatdbname.toStdString().c_str(), &wombatdb) == SQLITE_OK)
    {
        if(sqlite3_prepare_v2(wombatdb, "CREATE TABLE cases(caseid INTEGER PRIMARY KEY, name TEXT, creation TEXT, deleted INTEGER);", -1, &tmpstmt, NULL) == SQLITE_OK)
        {
            int ret = sqlite3_step(tmpstmt);
            if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
        }
        else
            wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
        sqlite3_finalize(tmpstmt);
    }
    else
        wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
    
    if(sqlite3_open(wombatptr->wombatdbname.toStdString().c_str(), &wombatdb) == SQLITE_OK)
    {
        if(sqlite3_prepare_v2(wombatdb, "CREATE TABLE log(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msgdatetime TEXT, msg TEXT);", -1, &tmpstmt, NULL) == SQLITE_OK)
        {
            int ret = sqlite3_step(tmpstmt);
            if(ret != SQLITE_ROW && ret != SQLITE_DONE)
            {
                wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
            }
        }
    }
    sqlite3_finalize(tmpstmt);
    //sqlite3_close(wombatdb); // I DON'T THINK I'LL CLOSE THIS CAUSE I OPEN IT JUST A LITTLE FARTHER DOWN.
}

void WombatDatabase::OpenAppDB()
{
    if(sqlite3_open(wombatptr->wombatdbname.toStdString().c_str(), &wombatdb) != SQLITE_OK)
        wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
    else
        wombatptr->curerrmsg = QString("");
}

void WombatDatabase::OpenCaseDB()
{
    if(sqlite3_open(wombatptr->caseobject.dbname.toStdString().c_str(), &casedb) != SQLITE_OK)
        wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
    else
        wombatptr->curerrmsg = "";
}

void WombatDatabase::CloseAppDB()
{
    if(sqlite3_finalize(wombatstatement) == SQLITE_OK)
    {
        if(sqlite3_close(wombatdb) == SQLITE_OK)
        {
            //fprintf(stderr, "CloseDB was successful\n");
            wombatptr->curerrmsg = "";
        }
        else
        {
            fprintf(stderr, "CloseDB: %s\n", sqlite3_errmsg(wombatdb));
            wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
        }
    }
    else
    {
        fprintf(stderr, "CloseDB: %s\n", sqlite3_errmsg(wombatdb));
        wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
    }
}

WombatDatabase::~WombatDatabase()
{
    CloseAppDB();
}

void WombatDatabase::InitializeEvidenceDatabase()
{
    /*
    #define IMGDB_CHUNK_SIZE 1024*1024*1 // what size chunks should the database use when growing and shrinking
    #define IMGDB_MAX_RETRY_COUNT 50    // how many times will we retry a SQL statement
    #define IMGDB_RETRY_WAIT 100   // how long (in milliseconds) are we willing to wait between retries

    sqlite3* cureviddencedb;
    sqlite3_stmt* curstatement;
    // set page size -- 4k is much faster on Windows than the default
    executeStatement("PRAGMA page_size = 4096;", curstatement, "TskImgDBSqlite::initialize");
    sqlite3_finalize(curstatement);

    // we don't have a mechanism to recover from a crash anyway
    executeStatement("PRAGMA synchronous = 0;", curstatement, "TskImgDBSqlite::initialize");
    sqlite3_finalize(curstatement);
    wombatptr->evidenceobject.dbname = wombatptr->evidenceobject.fullpath.split("/").last() + ".db";

    std::vector<const char *> wombatTableSchema;
    wombatTableSchema.clear();
    wombatTableSchema.push_back("CREATE TABLE job(jobid INTEGER PRIMARY KEY, type INTEGER, state INTEGER, filecount INTEGER, processcount INTEGER, caseid INTEGER, evidenceid INTEGER, start TEXT, end TEXT);");
    wombatTableSchema.push_back("CREATE TABLE evidence(evidenceid INTEGER PRIMARY KEY, fullpath TEXT, name TEXT, caseid INTEGER, creation TEXT, deleted INTEGER);");
    wombatTableSchema.push_back("CREATE TABLE settings(settingid INTEGER PRIMARY KEY, name TEXT, value TEXT, type INT);");
    wombatTableSchema.push_back("CREATE TABLE msglog(logid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, jobid INTEGER, msgtype INTEGER, msg TEXT, datetime TEXT);");
    wombatTableSchema.push_back("CREATE TABLE objects(objectid INTEGER PRIMARY KEY, caseid INTEGER, evidenceid INTEGER, fileid INTEGER, partid INTEGER, volid INTEGER, imgID INTEGER);");

    if(sqlite3_open(wombatptr->caseobject.dbname.toStdString().c_str(), &casedb) == SQLITE_OK)
    {
        const char* tblschema;
        foreach(tblschema, wombatTableSchema)
        {
            if(sqlite3_prepare_v2(casedb, tblschema, -1, &casestatement, NULL) == SQLITE_OK)
            {
                int ret = sqlite3_step(casestatement);
                if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                    wombatptr->curerrmsg = QString(sqlite3_errmsg(casedb));
            }
            else
                wombatptr->curerrmsg = QString(sqlite3_errmsg(casedb));
            sqlite3_finalize(casestatement);
        }
    }
    else
        wombatptr->curerrmsg = QString(sqlite3_errmsg(casedb));

    wombatptr->curerrmsg = "";

        QString evidenceName = evidenceFilePath.split("/").last();
        evidenceName += ".db";
    if(sqlite3_open(wombatptr->wombatdbname.toStdString().c_str(), &wombatdb) == SQLITE_OK)
    {
        if(sqlite3_prepare_v2(wombatdb, "CREATE TABLE cases(caseid INTEGER PRIMARY KEY, name TEXT, creation TEXT, deleted INTEGER);", -1, &tmpstmt, NULL) == SQLITE_OK)
        {
            int ret = sqlite3_step(tmpstmt);
            if(ret != SQLITE_ROW && ret != SQLITE_DONE)
                wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
        }
        else
            wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));
        sqlite3_finalize(tmpstmt);
    }
    else
        wombatptr->curerrmsg = QString(sqlite3_errmsg(wombatdb));

    if(sqlite3_exec(curevidencedb, "CREATE TABLE data (objectid INTEGER PRIMARY KEY, type INTEGER, name TEXT, fullpath TEXT, parentid INTEGER, size INTEGER, sectorstart INTEGER, sectorlength INTEGER)", NULL, NULL, 

    stmt = "CREATE TABLE db_info (name TEXT PRIMARY KEY, version TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating db_info table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- IMAGE_INFO
    stmt = "CREATE TABLE image_info (type INTEGER, ssize INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating image_info table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- IMAGE_NAMES
    stmt = "CREATE TABLE image_names (seq INTEGER PRIMARY KEY, name TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating image_names table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- VOL_INFO
    stmt = "CREATE TABLE vol_info (vol_id INTEGER PRIMARY KEY, sect_start INTEGER NOT NULL, "
        "sect_len INTEGER NOT NULL, description TEXT, flags INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating vol_info table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- FS_INFO
    stmt = "CREATE TABLE fs_info (fs_id INTEGER PRIMARY KEY, img_byte_offset INTEGER, "
        "vol_id INTEGER NOT NULL, fs_type INTEGER, block_size INTEGER, "
        "block_count INTEGER, root_inum INTEGER, first_inum INTEGER, last_inum INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str() , NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating fs_info table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- FILES
    stmt = "CREATE TABLE files (file_id INTEGER PRIMARY KEY, type_id INTEGER, "
        "name TEXT, par_file_id INTEGER, dir_type INTEGER, meta_type INTEGER, "
        "dir_flags INTEGER, meta_flags INTEGER, size INTEGER, ctime INTEGER, "
        "crtime INTEGER, atime INTEGER, mtime INTEGER, mode INTEGER, uid INTEGER, "
        "gid INTEGER, status INTEGER, full_path TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating files table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- FS_FILES
    stmt = "CREATE TABLE fs_files (file_id INTEGER PRIMARY KEY, fs_id INTEGER, "
        "fs_file_id INTEGER, attr_type INTEGER, attr_id INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating fs_files table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- FS_BLOCKS
    stmt = "CREATE TABLE fs_blocks (fs_id INTEGER NOT NULL, file_id INTEGER NOT NULL, seq INTEGER, "
        "blk_start INTEGER NOT NULL, blk_len INTEGER NOT NULL)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating fs_blocks table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- CARVED_FILES
    stmt = "CREATE TABLE carved_files (file_id INTEGER PRIMARY KEY, vol_id INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating carved_files table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- SECTOR_LIST
    stmt = "CREATE TABLE carved_sectors ("
        "file_id INTEGER, seq INTEGER, sect_start INTEGER, sect_len INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating carved_sectors table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- DERIVED_FILES
    stmt = "CREATE TABLE derived_files (file_id INTEGER PRIMARY KEY, derivation_details TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating derived_files table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- ALLOC_UNALLOC_MAP
    stmt = "CREATE TABLE alloc_unalloc_map (vol_id INTEGER, unalloc_img_id INTEGER, "
        "unalloc_img_sect_start INTEGER, sect_len INTEGER, orig_img_sect_start INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK)
    {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating alloc_unalloc_map table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- FILE_HASHES
    stmt = "CREATE TABLE file_hashes (file_id INTEGER PRIMARY KEY, md5 TEXT, sha1 TEXT, sha2_256 TEXT, sha2_512 TEXT, known INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating file_hashes table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- MODULES
    stmt = "CREATE TABLE modules (module_id INTEGER PRIMARY KEY, name TEXT UNIQUE NOT NULL, description TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating module table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- MODULE_STATUS
    stmt = "CREATE TABLE module_status (file_id INTEGER, module_id INTEGER, status INTEGER, PRIMARY KEY (file_id, module_id))";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating module_status table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- UNALLOC_IMG_STATUS
    stmt = "CREATE TABLE unalloc_img_status (unalloc_img_id INTEGER PRIMARY KEY, status INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating unalloc_img_status table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- UNUSED_SECTORS
    stmt = "CREATE TABLE unused_sectors (file_id INTEGER PRIMARY KEY, sect_start INTEGER, sect_len INTEGER, vol_id INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating unused_sectors table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- BLACKBOARD_ARTIFACTS
    stmt = "CREATE TABLE blackboard_artifacts (artifact_id INTEGER PRIMARY KEY, obj_id INTEGER NOT NULL, artifact_type_id INTEGER)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating blackboard_artifacts table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- BLACKBOARD_ATTRIBUTES
    stmt = "CREATE TABLE blackboard_attributes (artifact_id INTEGER NOT NULL, source TEXT, context TEXT, attribute_type_id INTEGER NOT NULL, value_type INTEGER NOT NULL, "
        "value_byte BLOB, value_text TEXT, value_int32 INTEGER, value_int64 INTEGER, value_double NUMERIC(20, 10), obj_id INTEGER NOT NULL)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating blackboard_attributes table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- BLACKBOARD_ARTIFACT_TYPES
    stmt = "CREATE TABLE blackboard_artifact_types (artifact_type_id INTEGER PRIMARY KEY, type_name TEXT, display_name TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating blackboard_artifact_types table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- BLACKBOARD_ATTRIBUTE_TYPES
    stmt = "CREATE TABLE blackboard_attribute_types (attribute_type_id INTEGER PRIMARY KEY, type_name TEXT, display_name TEXT)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating blackboard_attribute_types table: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    // ----- CREATE INDEXES
    stmt = "CREATE INDEX attrs_artifact_id ON blackboard_attributes(artifact_id)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating attrs_artifact_id index: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    stmt = "CREATE INDEX attrs_attribute_type ON blackboard_attributes(attribute_type_id)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating attrs_attribute_type index: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

    stmt = "CREATE INDEX attrs_obj_id ON blackboard_attributes(obj_id)";
    if (sqlite3_exec(m_db, stmt.c_str(), NULL, NULL, &errmsg) != SQLITE_OK) {
        infoMessage << L"TskImgDBSqlite::initialize - Error creating attrs_obj_id index: " << errmsg;
        LOGERROR(infoMessage.str());

        sqlite3_free(errmsg);
        return 1;
    }

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
 */ 
}

int WombatDatabase::ReturnCaseCount()
{
    int casecount = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT COUNT(caseid) FROM cases WHERE deleted = 0;", -1, &wombatstatement, NULL) == SQLITE_OK)
    {
        int ret = sqlite3_step(wombatstatement);
        if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            casecount = sqlite3_column_int(wombatstatement, 0);
    }
    else
        emit DisplayError("1.3", "SQL Error. ", sqlite3_errmsg(wombatdb));

    return casecount;
}

void WombatDatabase::InsertCase()
{
    wombatptr->caseobject.id = 0;
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO cases (name, creation, deleted) VALUES(?, ?, 0);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, wombatptr->caseobject.name.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(casestatement, 2, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                wombatptr->caseobject.id = sqlite3_last_insert_rowid(wombatdb);
            }
            else
                emit DisplayError("1.4", "INSERT CASE - RETURN CASEID", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.4", "INSERT CASE - BIND CASENAME", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.4", "INSERT CASE - PREPARE INSERT", sqlite3_errmsg(wombatdb));
}

void WombatDatabase::ReturnCaseNameList()
{
    if(sqlite3_prepare_v2(wombatdb, "SELECT name FROM cases WHERE deleted = 0 ORDER by caseid;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        while(sqlite3_step(casestatement) == SQLITE_ROW)
        {
            wombatptr->casenamelist << (const char*)sqlite3_column_text(casestatement, 0);
        }
    }
    else
    {
        emit DisplayError("1.5", "RETURN CASE NAMES LIST", sqlite3_errmsg(wombatdb));
    }
}

void WombatDatabase::ReturnCaseID()
{
    if(sqlite3_prepare_v2(wombatdb, "SELECT caseid FROM cases WHERE name = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, wombatptr->caseobject.name.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                wombatptr->caseobject.id = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.6", "RETURN CURRENT CASE ID", sqlite3_errmsg(wombatdb));
}

int WombatDatabase::ReturnObjectFileID(int objectid)
{
    int fileid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fileid FROM objects WHERE objectid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, objectid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                fileid = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.18", "RETURN OBJECT'S FILEID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.18", "RETURN OBJECT'S FILEID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.18", "RETURN OBJECT'S FILEID ", sqlite3_errmsg(wombatdb));

    return fileid;
}

int WombatDatabase::ReturnObjectEvidenceID(int objectid)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM objects WHERE objectid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, objectid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                evidenceid = sqlite3_column_int(casestatement, 0);
            else
                emit DisplayError("1.20", "RETURN OBJECT EVIDENCEID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.20", "RETURN OBJECT EVIDENCEID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.20", "RETURN OBJECT EVIDENCEID ", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

int WombatDatabase::ReturnObjectID(int caseid, int evidenceid, int fileid)
{
    int objectid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT objectid FROM objects WHERE caseid = ? AND evidenceid = ? AND fileid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, fileid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
                objectid = sqlite3_column_int(casestatement, 0);
            else
                emit DisplayError("1.19", "RETURN OBJECT ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.19", "RETURN OBJECT ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.19", "RETURN OBJECT ID ", sqlite3_errmsg(wombatdb));

    return objectid;
}

int WombatDatabase::InsertJob(int jobType, int caseID, int evidenceID)
{
    int jobid = 0;

    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO job (type, caseid, evidenceid, start) VALUES(?, ?, ?, ?);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobType) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, caseID) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, evidenceID) == SQLITE_OK && sqlite3_bind_text(casestatement, 4, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                jobid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
                emit DisplayError("1.4", "INSERT JOB ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.4", "INSERT JOB ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.4", "INSERT JOB ", sqlite3_errmsg(wombatdb));
    
    return jobid;
}

int WombatDatabase::InsertEvidence(QString evidenceName, QString evidenceFilePath, int caseID)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO evidence (fullpath, name, caseid, creation, deleted) VALUES(?, ?, ?, ?, 0);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, evidenceFilePath.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(casestatement, 2, evidenceName.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, caseID) == SQLITE_OK && sqlite3_bind_text(casestatement, 4, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // it was successful
                evidenceid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
                emit DisplayError("1.7", "INSERT EVIDENCE INTO CASE", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.7", "INSERT EVIDENCE INTO CASE", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.7", "INSERT EVIDENCE INTO CASE", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

int WombatDatabase::InsertObject(int caseid, int evidenceid, int itemtype, int curid)
{
    int objectid = 0;
    std::string tmpquery = "INSERT INTO objects (caseid, evidenceid, ";
    if(itemtype == 0) // item is file
        tmpquery += "fileid";
    else if(itemtype == 1) // item is partition
        tmpquery += "partid";
    else if(itemtype == 2) // item is a volume
        tmpquery += "volid";
    else if(itemtype == 3) // item is an image
        tmpquery += "imgid";
    tmpquery += ") VALUES(?, ?, ?);";
    if(sqlite3_prepare_v2(wombatdb, tmpquery.c_str(), -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, curid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // it was successful
                objectid = sqlite3_last_insert_rowid(wombatdb);
            }
            else
               DisplayError("1.17", "INSERT OBJECT INTO CASE ", sqlite3_errmsg(wombatdb));
        }
        else
            DisplayError("1.17", "INSERT OBJECT INTO CASE ", sqlite3_errmsg(wombatdb));
    }
    else
        DisplayError("1.17", "INSERT OBJECT INTO CASE ", sqlite3_errmsg(wombatdb));

    return objectid;
}

QStringList WombatDatabase::ReturnCaseActiveEvidenceID(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid,fullpath,name FROM evidence WHERE caseid = ? AND deleted = 0 ORDER BY evidenceid;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseID) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                tmpList << QString::number(sqlite3_column_int(casestatement, 0)) << (const char*)sqlite3_column_text(casestatement, 1) << (const char*)sqlite3_column_text(casestatement, 2);
            }
        }
        else
        {
            emit DisplayError("1.8", "RETURN CASE EVIDENCE", sqlite3_errmsg(wombatdb));
        }
    }
    else
        emit DisplayError("1.8", "RETURN CASE EVIDENCE", sqlite3_errmsg(wombatdb));

    return tmpList;
}

QStringList WombatDatabase::ReturnCaseActiveEvidence(int caseID)
{
    QStringList tmpList;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath FROM evidence WHERE caseid = ? AND deleted = 0 ORDER BY evidenceid;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseID) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                tmpList << (const char*)sqlite3_column_text(casestatement, 0);
            }
        }
        else
        {
            emit DisplayError("1.8", "RETURN CASE EVIDENCE", sqlite3_errmsg(wombatdb));
        }
    }
    else
        emit DisplayError("1.8", "RETURN CASE EVIDENCE", sqlite3_errmsg(wombatdb));

    return tmpList;
}

QStringList WombatDatabase::ReturnCaseEvidenceIdJobIdType(int caseid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT jobid,type,evidenceid FROM job WHERE caseid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                tmplist << QString::number(sqlite3_column_int(casestatement, 0)) << QString::number(sqlite3_column_int(casestatement, 1)) << QString::number(sqlite3_column_int(casestatement, 2));
            }
        }
        else
            emit DisplayError("1.15", "RETURN CASE EVIDENCE ADD JOB ID/TYPE ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.15", "RETURN CASE EVIDENCE ADD JOB ID/TYPE ", sqlite3_errmsg(wombatdb));

    return tmplist;
}

QStringList WombatDatabase::ReturnEvidenceData(int evidenceid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath, name FROM evidence WHERE evidenceid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                tmplist << QString((const char*)sqlite3_column_text(casestatement, 0));
                tmplist << QString((const char*)sqlite3_column_text(casestatement, 1));
            }
            else
                emit DisplayError("1.21", "RETURN EVIDENCE DATA ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.21", "RETURN EVIDENCE DATA ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.21", "RETURN EVIDENCE DATA ", sqlite3_errmsg(wombatdb));

    return tmplist;
}

QStringList WombatDatabase::ReturnMessageTableEntries(int jobid)
{
    QStringList tmpstringlist;
    QString tmptype;
    if(sqlite3_prepare_v2(wombatdb, "SELECT msgtype, msg FROM msglog WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            while(sqlite3_step(casestatement) == SQLITE_ROW)
            {
                if(sqlite3_column_int(casestatement, 0) == 0)
                    tmptype = "[ERROR]";
                else if(sqlite3_column_int(casestatement, 0) == 1)
                    tmptype = "[WARN]";
                else
                    tmptype = "[INFO]";
                tmpstringlist << tmptype << QString((const char*)sqlite3_column_text(casestatement, 1));
            }
        }
        else
            emit DisplayError("1.9", "RETURN MSGTABLE ENTIRES ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.9", "RETURN MSGTABLE ENTRIES ", sqlite3_errmsg(wombatdb));

    return tmpstringlist;
}

void WombatDatabase::InsertMsg(int caseid, int evidenceid, int jobid, int msgtype, const char* msg)
{
    if(sqlite3_prepare_v2(wombatdb, "INSERT INTO msglog (caseid, evidenceid, jobid, msgtype, msg, datetime) VALUES(?, ?, ?, ?, ?, ?);", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, caseid) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, evidenceid) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, jobid) == SQLITE_OK && sqlite3_bind_int(casestatement, 4, msgtype) == SQLITE_OK && sqlite3_bind_text(casestatement, 5, msg, -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_text(casestatement, 6, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // do nothing, it was successful
            }
            else
                emit DisplayError("1.10", "INSERT MSG ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.10", "INSERT MSG ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.10", "INSERT MSG ", sqlite3_errmsg(wombatdb));
}

QStringList WombatDatabase::ReturnJobDetails(int jobid)
{
    QStringList tmplist;
    if(sqlite3_prepare_v2(wombatdb, "SELECT end, filecount, processcount, state FROM job WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                tmplist << QString((const char*)sqlite3_column_text(casestatement, 0)) << QString::number(sqlite3_column_int(casestatement, 1)) << QString::number(sqlite3_column_int(casestatement, 2));
                if(sqlite3_column_int(casestatement, 3) == 1)
                    tmplist << "Processing Finished";
                else
                    tmplist << "Processing Finished with Errors";
            }
            else
                emit DisplayError("1.22", "RETURN JOB DETAILS ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.22", "RETURN JOB DETAILS ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.22", "RETURN JOB DETAILS ", sqlite3_errmsg(wombatdb));

    return tmplist;
}

void WombatDatabase::UpdateJobEnd(int jobid, int filecount, int processcount)
{
    if(sqlite3_prepare_v2(wombatdb, "UPDATE job SET end = ?, filecount = ?, processcount = ?, state = 1 WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, GetTime().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK && sqlite3_bind_int(casestatement, 2, filecount) == SQLITE_OK && sqlite3_bind_int(casestatement, 3, processcount) == SQLITE_OK && sqlite3_bind_int(casestatement, 4, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // do nothing, it was successful
            }
            else
            {
                emit DisplayError("1.16", "UPDATE FINISHED JOB ", sqlite3_errmsg(wombatdb));
            }
        }
        else
            emit DisplayError("1.16", "UPDATE FINISHED JOB ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.16", "UPDATE FINISHED JOB ", sqlite3_errmsg(wombatdb));
}

int WombatDatabase::ReturnJobCaseID(int jobid)
{
    int caseid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT caseid FROM job WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                caseid = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.23", "RETURN JOB CASE ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.23", "RETURN JOB CASE ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.23", "RETURN JOB CASE ID ", sqlite3_errmsg(wombatdb));

    return caseid;
}

int WombatDatabase::ReturnJobEvidenceID(int jobid)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM job WHERE jobid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, jobid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                evidenceid = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.24", "RETURN JOB EVIDENCE ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.24", "RETURN JOB EVIDENCE ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.24", "RETURN JOB EVIDENCE ID ", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

void WombatDatabase::RemoveEvidence(QString evidencename)
{
    if(sqlite3_prepare_v2(wombatdb, "UPDATE evidence SET deleted = 1 WHERE fullpath = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, evidencename.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                // do nothing, successful
            }
            else
                emit DisplayError("1.25", "REMOVE EVIDENCE ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.25", "REMOVE EVIDENCE ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.25", "REMOVE EVIDENCE ", sqlite3_errmsg(wombatdb));
}

int WombatDatabase::ReturnEvidenceID(QString evidencename)
{
    int evidenceid = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT evidenceid FROM evidence WHERE fullpath = ? and deleted = 0;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(casestatement, 1, evidencename.toStdString().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                evidenceid = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.26", "RETURN EVIDENCE ID FROM NAME ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.26", "RETURN EVIDENCE ID FROM NAME ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.26", "RETURN EVIDENCE ID FROM NAME ", sqlite3_errmsg(wombatdb));

    return evidenceid;
}

int WombatDatabase::ReturnEvidenceDeletedState(int evidenceid)
{
    int isdeleted = 0;
    if(sqlite3_prepare_v2(wombatdb, "SELECT deleted FROM evidence WHERE evidenceid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                isdeleted = sqlite3_column_int(casestatement, 0);
            }
            else
                emit DisplayError("1.28", "RETURN EVIDENCE DELETED STATE FROM ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.28", "RETURN EVIDENCE DELETED STATE FROM ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.28", "RETURN EVIDENCE DELETED STATE FROM ID ", sqlite3_errmsg(wombatdb));

    return isdeleted;
}

QString WombatDatabase::ReturnEvidencePath(int evidenceid)
{
    QString epath = "";
    if(sqlite3_prepare_v2(wombatdb, "SELECT fullpath FROM evidence WHERE evidenceid = ?;", -1, &casestatement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_int(casestatement, 1, evidenceid) == SQLITE_OK)
        {
            int ret = sqlite3_step(casestatement);
            if(ret == SQLITE_ROW || ret == SQLITE_DONE)
            {
                epath = QString((const char*)sqlite3_column_text(casestatement, 0));
            }
            else
                emit DisplayError("1.27", "RETURN EVIDENCE PATH FROM ID ", sqlite3_errmsg(wombatdb));
        }
        else
            emit DisplayError("1.27", "RETURN EVIDENCE PATH FROM ID ", sqlite3_errmsg(wombatdb));
    }
    else
        emit DisplayError("1.27", "RETURN EVIDENCE PATH FROM ID ", sqlite3_errmsg(wombatdb));

    return epath;
}
