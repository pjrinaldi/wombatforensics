//
// Created by rainm on 17-6-9.
//

#ifndef ZMG_UTILS_ZMGFS_H
#define ZMG_UTILS_ZMGFS_H

#include <stdint.h>

#define ZMG_VERSION     1

#define MAX_FILE_NAME   64

#define CHUNK_SIZE  (16384)

#define ENTRY_TYPE_DIR  0
#define ENTRY_TYPE_FILE 1

struct zmg_header {
    char magic[4];
    int version;
};

struct zmg_entry {
    int type;
};

struct zmg_dir_entry {
    int type;
    char name[MAX_FILE_NAME];
    int n_dirs;
    int n_files;
    //  offset of data(relative to start of this entry)
    uint64_t off_data;
};

struct zmg_file_entry {
    int type;
    char name[MAX_FILE_NAME];
    uint64_t data_size;
    uint64_t file_size;
    //  offset of data(relative to start of this entry)
    uint64_t off_data;
};

void init_zmg_header(struct zmg_header *header);

int zip_buffer_to_file(const char *buffer, size_t size, FILE *dest, size_t *outsize, int level);

int unzip_buffer_to_buffer(const char *input, size_t input_size, char *output, size_t *output_size);

int unzip_buffer_to_file(const char *buffer, size_t size, FILE *dest);

int zip_file_to_file(const char *filename, FILE *dest, size_t *outsize);

const char *last_name_of(const char *path);

struct zmg_dir_entry *find_dir_entry_from_dir(const char *name, struct zmg_dir_entry *dentry);

struct zmg_dir_entry *find_dir_entry_at(const char *path, struct zmg_dir_entry *root);

struct zmg_file_entry *find_file_entry_with_filename_at(const char *name, struct zmg_dir_entry *dentry);

struct zmg_file_entry *find_file_entry_at(const char *path, struct zmg_dir_entry *dentry);

#endif //ZMG_UTILS_ZMGFS_H
