#ifndef ZMGFUSE_H
#define ZMGFUSE_H
//
// Created by rainm on 17-6-9.
//
// Modified by Pasquale J. Rinaldi, Jr. 2020-10-09

#define FUSE_USE_VERSION 35

#include <fuse3/fuse.h>
#include <fuse3/fuse_lowlevel.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

extern "C" {
#include "utarray.h"
#include "uthash.h"
#include "utlist.h"
#include "zmgfs.h"
}

//#define DEBUG

#ifndef DEBUG
#   define log(format, ...)
#else
#   define log(format, ...) \
{ \
    FILE *FH = fopen( "/tmp/zmgmnt.log", "a" ); \
    if( FH ) { \
        fprintf( FH, "line:%d: " format "\n", __LINE__, ##__VA_ARGS__ ); \
        fclose( FH ); \
    } \
}
#endif

static struct fuse_opt zmg_opts[] =
        {
                FUSE_OPT_END
        };

struct file_buffer_cache {
    char path[256];
    char *buffer;
    UT_hash_handle hh;
};

char *zmgfile = NULL;
char *mtpt = NULL;
int zmgfd = 0;
const char *zmgmap = NULL;

pthread_mutex_t cache_mutex;
struct file_buffer_cache *buffer_cache = NULL;

int is_file_cached(const char *path) {
    struct file_buffer_cache *cache;

    pthread_mutex_lock(&cache_mutex);
    HASH_FIND_STR(buffer_cache, path, cache);
    pthread_mutex_unlock(&cache_mutex);

    return (cache != NULL);
};

char *get_cached_buffer(const char *path) {
    struct file_buffer_cache *cache;

    pthread_mutex_lock(&cache_mutex);
    HASH_FIND_STR(buffer_cache, path, cache);
    pthread_mutex_unlock(&cache_mutex);

    if (cache != NULL) {
        return cache->buffer;
    }
    return NULL;
};

char *remove_cache_if_cached(const char *path) {
    char *ret = NULL;
    struct file_buffer_cache *cache;

    pthread_mutex_lock(&cache_mutex);
    HASH_FIND_STR(buffer_cache, path, cache);
    if (cache != NULL) {
        HASH_DEL(buffer_cache, cache);
        char *buffer = cache->buffer;
        free(cache);
        ret = buffer;
    }
    pthread_mutex_unlock(&cache_mutex);

    return ret;
};

int put_cache_if_not_cached(const char *path, char *buffer) {

    assert(buffer != NULL);

    int ret = 0;
    struct file_buffer_cache *cache = NULL;

    pthread_mutex_lock(&cache_mutex);
    HASH_FIND_STR(buffer_cache, path, cache);
    if (cache == NULL) {
        cache = (struct file_buffer_cache *) malloc(sizeof(struct file_buffer_cache));
        strcpy(cache->path, path);
        cache->buffer = buffer;
        HASH_ADD_STR(buffer_cache, path, cache);
        ret = 1;
    }
    pthread_mutex_unlock(&cache_mutex);

    return ret;
};

struct zmg_dir_entry *open_root(const char *zmgmap) {
    return (struct zmg_dir_entry *) (zmgmap + sizeof(struct zmg_header));
}
//void (*getattr) (fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
static int zmgfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {

    struct zmg_dir_entry *root = open_root(zmgmap);
    struct zmg_dir_entry *dentry = find_dir_entry_at(path, root);

    memset(stbuf, 0, sizeof(struct stat));
    if (dentry != NULL) {
        stbuf->st_mode = S_IFDIR | 0555;
        stbuf->st_nlink = 2;
        return 0;
    }

    char filepath[256];
    strcpy(filepath, path);
    char *filename = (char *) last_name_of(filepath);
    filename[-1] = '\0';

    dentry = find_dir_entry_at(filepath, root);
    if (dentry != NULL) {
        struct zmg_file_entry *fentry = find_file_entry_with_filename_at(filename, dentry);
        if (fentry != NULL) {
            stbuf->st_mode = S_IFREG | 0444;
            stbuf->st_nlink = 1;
            stbuf->st_size = fentry->file_size;
            return 0;
        }
    }
    return -ENOENT;
};

static int zmgfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    (void) offset;
    (void) fi;

    struct zmg_dir_entry *root = open_root(zmgmap);
    struct zmg_dir_entry *dentry = find_dir_entry_at(path, root);

    if (dentry == NULL) {
        return -ENOENT;
    }

    //filler(buf, ".", NULL, 0);
    //filler(buf, "..", NULL, 0);

    struct zmg_dir_entry *dentries = (struct zmg_dir_entry *) (((char *) dentry) + dentry->off_data);
    struct zmg_file_entry *fentry = (struct zmg_file_entry *) (dentries + dentry->n_dirs);

    //filler(buf, rawpath + 1, NULL, 0, (fuse_fill_dir_flags)0);
    for (int i = 0; i < dentry->n_dirs; i++) {
        filler(buf, dentries[i].name, NULL, 0, (fuse_fill_dir_flags)0);
    }

    for (int i = 0; i < dentry->n_files; i++) {
        filler(buf, fentry->name, NULL, 0, (fuse_fill_dir_flags)0);
        fentry = (struct zmg_file_entry *) (((char *) fentry) + fentry->off_data + fentry->data_size);
    }
    return 0;
};

static int zmgfs_open(const char *path, struct fuse_file_info *fi) {

    log("zmgfs: open %s", path);

    struct zmg_dir_entry *root = open_root(zmgmap);
    struct zmg_file_entry *fentry = find_file_entry_at(path, root);

    if (fentry != NULL) {
        if ((fi->flags & 3) != O_RDONLY) {
            return -EACCES;
        }
        return 0;
    }
    return -ENOENT;
};

static int zmgfs_release(const char *path, struct fuse_file_info *fi) {

    log("zmgfs: release %s", path);

    char *buffer = remove_cache_if_cached(path);
    if (buffer != NULL) {
        free(buffer);
    }
    return 0;
};

static int zmgfs_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {

    log("zmgfs: read %s", path);

    struct zmg_dir_entry *root = open_root(zmgmap);
    struct zmg_file_entry *fentry = find_file_entry_at(path, root);

    size_t filesize;
    if (fentry != NULL) {
        filesize = fentry->file_size;
        if (offset < filesize) {

            char *buffer = get_cached_buffer(path);
            if (buffer != NULL) {
                if (offset + size > filesize) {
                    size = filesize - offset;
                }
                memcpy(buf, buffer + offset, size);
            } else {
                char *zipbuf = ((char *) fentry) + fentry->off_data;
                buffer = (char*)malloc(fentry->file_size);
                size_t sz;
                unzip_buffer_to_buffer(zipbuf, fentry->data_size, buffer, &sz);

                if (offset + size > filesize) {
                    size = filesize - offset;
                }
                memcpy(buf, buffer + offset, size);

                if (!put_cache_if_not_cached(path, buffer)) {
                    free(buffer);
                }
            }
        } else {
            size = 0;
        }
    }
    log("zmgfs: end read %s", path);
    return (int) size;
};

static int
zmg_opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs) {
    (void) data;

    switch (key) {
        case FUSE_OPT_KEY_OPT:
            return 1;

        case FUSE_OPT_KEY_NONOPT:
            if (!zmgfile) {
                zmgfile = strdup(arg);
                return 0;
            } else if (!mtpt) {
                mtpt = strdup(arg);
            }
            return 1;
        default:
            fprintf(stderr, "internal error\n");
            abort();
    }
};

//static struct fuse_lowlevel_ops zmgfs_oper = {
static struct fuse_operations zmgfs_oper = {
        .getattr     = zmgfs_getattr,
        .open        = zmgfs_open,
        .read        = zmgfs_read,
        .readdir     = zmgfs_readdir,
        //.release    = zmgfs_release,
};

void* zmgfuselooper(void *data)
{
    struct fuse* fuse = (struct fuse*) data;
    fuse_loop(fuse);
    //struct fuse_session* fuse = (struct fuse_session*) data;
    //fuse_session_loop(fuse);
};

//struct fuse_args zmgargs;
//struct fuse* zmgfuser;
//pthread_t zmgfusethread;

//void ZmgFuser(std::string imgpath, std::string imgfile)
//struct fuse* ZmgFuser(std::string imgpath, std::string imgfile)
//struct fuse_session* ZmgFuser(std::string imgpath, std::string imgfile)
void ZmgFuser(std::string imgpath, std::string imgfile)
{
    QString zprog = "zmgmnt";
    QStringList args;
    args << QString::fromStdString(imgfile) << QString::fromStdString(imgpath);
    QProcess* zprocess = new QProcess();
    zprocess->waitForFinished(-1);
    zprocess->start(zprog, args);
    /*
    struct fuse_args zmgargs;
    struct fuse_session* zmgfuser;
    struct fuse_cmdline_opts opts;
    //struct fuse_loop_config config;
    //struct lo_data lo = { .debug = 0, .writeback = 0 };

    char** fargv = NULL;
    fargv = (char**)calloc(3, sizeof(char*));
    fargv[0] = "./zmgfuse";
    int fargc = 1;
    zmgargs = FUSE_ARGS_INIT(fargc, fargv);
    char* ipath = new char[imgpath.size() + 1];
    strcpy(ipath, imgpath.c_str());
    char* iname = new char[imgfile.size() + 1];
    strcpy(iname, imgfile.c_str());
    umask(0);
    //pthread_mutex_t zmgfusethread;
    //pthread_mutex_init(&zmgfusethread, NULL);
    //lo.root.next = lo.root.prev = &lo.root;
    //lo.root.fd = -1;
    //lo.cache = CACHE_NORMAL;
    //fuse_opt_parse(&args, &lo, lo_opts, NULL);
    //lo.root.refcount = 2;
    //if(lo.source)
    //{
    //    struct stat stat;
    //    int res;
    //    res = lstat(lo.source, &stat);
    //lo.source = iname;
    //lo.timeout = 86400.0;
    //lo.root.fd = open(lo.source, O_PATH);
    pthread_t zmgfusethread;
    pthread_create(&zmgfusethread, NULL, zmgfuselooper, (void *) zmgfuser);
    zmgfuser = fuse_session_new(&zmgargs, &zmgfs_oper, sizeof(fuse_operations), NULL);
    fuse_set_signal_handlers(zmgfuser);
    fuse_session_mount(zmgfuser, imgpath.c_str());
    fuse_daemonize(1);

    //fuse_session_loop(zmgfuser);

    return zmgfuser;
    // STORE FUSE_SESSION (ZMGFUSER) BY RETURNING IT

    //fuse_session_unmount(zmgfuser);
    //fuse_session_destroy(zmgfuser);
    //close(lo.root.fd);

    //}
    */

    /*
    char** fargv = NULL;
    fargv = (char**)calloc(3, sizeof(char*));
    int fargc = 3;
    char* iname = new char[imgfile.size() + 1];
    strcpy(iname, imgfile.c_str());
    char* ipath = new char[imgpath.size() + 1];
    strcpy(ipath, imgpath.c_str());
    fargv[0] = "./zmgfuse";
    fargv[1] = iname;
    fargv[2] = ipath;
    struct fuse_args zmgargs = FUSE_ARGS_INIT(fargc, fargv);
    struct fuse* fuse = fuse_new(&zmgargs, &zmgfs_oper, sizeof(fuse_operations), NULL);
    fuse_mount(fuse, imgpath.c_str());
    fuse_daemonize(1);
    //struct fuse_session* se = fuse_get_session(fuse);
    //fuse_set_signal_handlers(se);
    fuse_loop(fuse);
    //fuse_remove_signal_handlers(se);
    */
    /*
    struct fuse_args zmgargs;
    pthread_t zmgfusethread;
    struct fuse* zmgfuser;
    char** fargv = NULL;
    fargv = (char**)calloc(3, sizeof(char*));
    //fargv[0] = "./zmgfuse";
    int fargc = 2;
    int ret = 0;
    char* iname = new char[imgfile.size() + 1];
    strcpy(iname, imgfile.c_str());
    char* ipath = new char[imgpath.size() + 1];
    strcpy(ipath, imgpath.c_str());
    fargv[0] = iname;
    fargv[1] = ipath;
    // PROBABLY WANT TO MOVE THE BELOW STAT STUFF TO THE FUNCTION CALL BASED ON PASED VARIABLES???
    zmgfd = open(iname, O_RDONLY);
    struct stat st;
    fstat(zmgfd, &st);
    size_t size = (size_t) st.st_size;
    zmgmap = (const char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, zmgfd, 0);
    zmgargs = FUSE_ARGS_INIT(fargc, fargv);
    //zmgfuser = fuse_new(&zmgargs, &zmgfs_oper, sizeof(fuse_operations), NULL);
    //ret = fuse_mount(zmgfuser, imgpath.c_str());
    //int retd = fuse_daemonize(1);
    //int perr = pthread_create(&zmgfusethread, NULL, zmgfuselooper, (void *)zmgfuser);
    
    //fuse_daemonize(1);
    fuse_main(zmgargs.argc, zmgargs.argv, &zmgfs_oper, NULL);
    //return fuse_get_session(zmgfuser);

    //system(std::string("zmgmnt " + imgfile + " " + imgpath).c_str());
    //system(QString("zmgmnt " + newevidence.at(i) + " " + emntpath).toStdString().c_str());
    
    //struct fuse_session *fuse_get_session(struct fuse *f);
    */
    
    /*
    struct fuse_args zmgargs;
    pthread_t zmgfusethread;
    struct fuse* zmgfuser;
    //printf("imgpath: %s\n", imgpath.c_str());
    //printf("imgfile: %s\n", imgfile.c_str());
    char** fargv = NULL;
    fargv = (char**)calloc(3, sizeof(char*));
    fargv[0] = "./zmgfuse";
    int fargc = 1;
    int ret = 0;
    char* ipath = new char[imgpath.size() + 1];
    strcpy(ipath, imgpath.c_str());
    char* iname = new char[imgfile.size() + 1];
    strcpy(iname, imgfile.c_str());
    //printf("imgpath:  %s\n", ipath);
    //printf("imgfile: %s\n", iname);
    zmgfd = open(iname, O_RDONLY);
    struct stat st;
    fstat(zmgfd, &st);
    size_t size = (size_t) st.st_size;
    zmgmap = (const char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, zmgfd, 0);
    zmgargs = FUSE_ARGS_INIT(fargc, fargv);
    zmgfuser = fuse_new(&zmgargs, &zmgfs_oper, sizeof(fuse_operations), NULL);
    ret = fuse_mount(zmgfuser, imgpath.c_str());
    int retd = fuse_daemonize(1);
    int perr = pthread_create(&zmgfusethread, NULL, zmgfuselooper, (void *) zmgfuser);

    return zmgfuser;
    */
};
#endif // ZMGFUSE_H
