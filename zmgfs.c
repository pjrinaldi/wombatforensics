//
// Created by rainm on 17-6-9.
//

#include <string.h>
#include <stdio.h>
#include <zlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

#include "zmgfs.h"

void init_zmg_header(struct zmg_header *header) {
    strcpy(header->magic, "zmg");
    header->version = ZMG_VERSION;
}

int zip_buffer_to_file(const char *buffer, size_t size, FILE *dest, size_t *outsize, int level) {

    *outsize = 0;

    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK_SIZE] = {0};

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    size_t pos = 0;

    /* compress until end of file */
    do {
        int blk = (int) ((size - pos) > CHUNK_SIZE ? CHUNK_SIZE : (size - pos));
        if (blk == 0) {
            break;
        }
        strm.avail_in = (uInt) blk;
        flush = pos + blk < size ? Z_NO_FLUSH : Z_FINISH;
        strm.next_in = (Bytef *) (buffer + pos);

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK_SIZE - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void) deflateEnd(&strm);
                return Z_ERRNO;
            }
            *outsize += have;
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        pos += blk;
        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void) deflateEnd(&strm);
    return Z_OK;
}

int unzip_buffer_to_buffer(const char *input, size_t input_size, char *output, size_t *output_size) {

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return ret;
    }

    size_t pos = 0;
    size_t outpos = 0;
    /* decompress until deflate stream ends or end of file */
    do {
        int blk = (int) ((input_size - pos) > CHUNK_SIZE ? CHUNK_SIZE : (input_size - pos));
        if (blk == 0) {
            break;
        }
        strm.avail_in = (uInt) blk;
        strm.next_in = (Bytef *) (input + pos);

        /* run inflate() on input until output input not full */
        do {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void) inflateEnd(&strm);
                    return ret;
            }
            have = CHUNK_SIZE - strm.avail_out;
            memcpy(&output[outpos], out, have);
            outpos += have;
        } while (strm.avail_out == 0);

        pos += blk;

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void) inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int unzip_buffer_to_file(const char *buffer, size_t size, FILE *dest) {

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return ret;
    }

    size_t pos = 0;
    /* decompress until deflate stream ends or end of file */
    do {
        int blk = (int) ((size - pos) > CHUNK_SIZE ? CHUNK_SIZE : (size - pos));
        if (blk == 0) {
            break;
        }
        strm.avail_in = (uInt) blk;
        strm.next_in = (Bytef *) (buffer + pos);

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void) inflateEnd(&strm);
                    return ret;
            }
            have = CHUNK_SIZE - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void) inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        pos += blk;

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void) inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int zip_file_to_file(const char *filename, FILE *dest, size_t *outsize) {

    int _fd = open(filename, O_RDONLY);

    struct stat st;
    fstat(_fd, &st);
    size_t _size = (size_t) st.st_size;
    char *_buf = mmap(NULL, _size, PROT_READ, MAP_PRIVATE, _fd, 0);

    int ret = zip_buffer_to_file(_buf, _size, dest, outsize, Z_DEFAULT_COMPRESSION);

    munmap(_buf, _size);

    close(_fd);
    return ret;
}


const char *last_name_of(const char *path) {
    int len = (int) strlen(path);
    int i = len - 1;
    while (i >= 0) {
        if (path[i] == '/') {
            return &(path[i + 1]);
        }
        i--;
    }

    return path;
}

struct zmg_dir_entry *find_dir_entry_from_dir(const char *name, struct zmg_dir_entry *dentry) {

    struct zmg_dir_entry *entries = (struct zmg_dir_entry *) (((char *) dentry) + dentry->off_data);
    for (int i = 0; i < dentry->n_dirs; i++) {
        if (strcmp(entries[i].name, name) == 0) {
            return &entries[i];
        }
    }
    return NULL;
}

struct zmg_dir_entry *find_dir_entry_at(const char *path, struct zmg_dir_entry *root) {

    if (path[0] == '/') {
        path++;
    }
    if (path[0] == '\0') {
        return root;
    }

    char *pathname = strdup(path);

    struct zmg_dir_entry *dentry = root;

    char *restpath = pathname;
    char *dirname = restpath;
    restpath = strchr(pathname, '/');
    while (restpath != NULL) {
        *restpath = '\0';
        restpath++;

        dentry = find_dir_entry_from_dir(dirname, dentry);
        if (dentry == NULL) {
            goto out;
        }

        dirname = restpath;
        restpath = strchr(restpath, '/');
    }

    dentry = find_dir_entry_from_dir(dirname, dentry);

    out:
    free(pathname);
    return dentry;
}

struct zmg_file_entry *find_file_entry_with_filename_at(const char *name, struct zmg_dir_entry *dentry) {

    struct zmg_dir_entry *entries = (struct zmg_dir_entry *) (((char *) dentry) + dentry->off_data);
    struct zmg_file_entry *fentry = (struct zmg_file_entry *) ((char *) (entries + dentry->n_dirs));

    int nfiles = dentry->n_files;
    while (nfiles-- > 0) {

        if (strcmp(fentry->name, name) == 0) {
            return fentry;
        }
        fentry = (struct zmg_file_entry *) (((char *) fentry) + fentry->off_data + fentry->data_size);
    }
    return NULL;
}


struct zmg_file_entry *find_file_entry_at(const char *path, struct zmg_dir_entry *direntry) {
    char filepath[256];
    strcpy(filepath, path);
    char *filename = (char *) last_name_of(filepath);
    filename[-1] = '\0';

    struct zmg_dir_entry *dentry = find_dir_entry_at(filepath, direntry);
    if (dentry != NULL) {
        struct zmg_file_entry *fentry = find_file_entry_with_filename_at(filename, dentry);
        if (fentry != NULL) {
            return fentry;
        }
    }
    return NULL;
}
