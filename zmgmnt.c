//
// Created by rainm on 17-6-9.
//

#define FUSE_USE_VERSION 26

#include <fuse.h>
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

#include "uthash.h"

#include "zmgfs.h"

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
}

char *get_cached_buffer(const char *path) {
    struct file_buffer_cache *cache;

    pthread_mutex_lock(&cache_mutex);
    HASH_FIND_STR(buffer_cache, path, cache);
    pthread_mutex_unlock(&cache_mutex);

    if (cache != NULL) {
        return cache->buffer;
    }
    return NULL;
}

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
}

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
}

struct zmg_dir_entry *open_root(const char *zmgmap) {
    return (struct zmg_dir_entry *) (zmgmap + sizeof(struct zmg_header));
}

static int zmgfs_getattr(const char *path, struct stat *stbuf) {

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
}

static int zmgfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    struct zmg_dir_entry *root = open_root(zmgmap);
    struct zmg_dir_entry *dentry = find_dir_entry_at(path, root);

    if (dentry == NULL) {
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    struct zmg_dir_entry *dentries = (struct zmg_dir_entry *) (((char *) dentry) + dentry->off_data);
    struct zmg_file_entry *fentry = (struct zmg_file_entry *) (dentries + dentry->n_dirs);

    for (int i = 0; i < dentry->n_dirs; i++) {
        filler(buf, dentries[i].name, NULL, 0);
    }

    for (int i = 0; i < dentry->n_files; i++) {
        filler(buf, fentry->name, NULL, 0);
        fentry = (struct zmg_file_entry *) (((char *) fentry) + fentry->off_data + fentry->data_size);
    }
    return 0;
}

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
}

static int zmgfs_release(const char *path, struct fuse_file_info *fi) {

    log("zmgfs: release %s", path);

    char *buffer = remove_cache_if_cached(path);
    if (buffer != NULL) {
        free(buffer);
    }
    return 0;
}

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
                buffer = malloc(fentry->file_size);
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
}

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
}

static struct fuse_operations zmgfs_oper = {
        .getattr    = zmgfs_getattr,
        .readdir    = zmgfs_readdir,
        .open        = zmgfs_open,
        .read        = zmgfs_read,
        .release    = zmgfs_release,
};

int main(int argc, char *argv[]) {

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    if (fuse_opt_parse(&args, NULL, zmg_opts, zmg_opt_proc) == -1) {
        return -1;
    }

    if (zmgfile == NULL || mtpt == NULL) {
        printf("Invalid arguments.\n");
        exit(-1);
    }

    printf("zmg file: %s\n", zmgfile);
    printf("mount point: %s\n", mtpt);

    pthread_mutex_init(&cache_mutex, NULL);

    zmgfd = open(argv[1], O_RDONLY);
    struct stat st;
    fstat(zmgfd, &st);
    size_t size = (size_t) st.st_size;
    zmgmap = mmap(NULL, size, PROT_READ, MAP_PRIVATE, zmgfd, 0);

    int ret = fuse_main(args.argc, args.argv, &zmgfs_oper, NULL);

    munmap((void *) zmgmap, size);
    close(zmgfd);

    return ret;
}
