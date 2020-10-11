#include <stdio.h>
#include <zlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

extern "C" {
#include "utarray.h"
#include "uthash.h"
#include "zmgfs.h"
#include "utlist.h"
}

unsigned long get_file_size(const char *path) {
    unsigned long filesize = 0;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0) {
        return filesize;
    } else {
        filesize = (unsigned long) statbuff.st_size;
    }
    return filesize;
}

struct dir_entry_pos {
    char path[256];
    off_t offset;
    struct zmg_dir_entry dirent;
    UT_hash_handle hh;
};

int compress_dir_l1(const char *root, const char *path, FILE *dest, struct dir_entry_pos **poses) {

    off_t start_off = ftell(dest);
    int n_dirs = 0, n_files = 0;

    char abspath[256];
    sprintf(abspath, "%s/%s", root, path);

    DIR *d = opendir(abspath);
    if (d == NULL) { return 0; }
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_DIR
            && strcmp(dir->d_name, ".") != 0 &&
            strcmp(dir->d_name, "..") != 0) {

            char dir_relapath[256];
            memset(dir_relapath, 0, sizeof(dir_relapath));
            sprintf(dir_relapath, "%s/%s", path, dir->d_name);

            struct dir_entry_pos *curpos = (dir_entry_pos*)malloc(sizeof(struct dir_entry_pos));
            strcpy(curpos->path, dir_relapath);
            curpos->offset = ftell(dest);
            curpos->dirent.type = ENTRY_TYPE_DIR;
            strcpy(curpos->dirent.name, dir->d_name);
            curpos->dirent.n_dirs = 0;
            curpos->dirent.n_files = 0;
            curpos->dirent.off_data = 0;

            HASH_ADD_STR(*poses, path, curpos);

            fwrite(&curpos->dirent, sizeof(struct zmg_dir_entry), 1, dest);

            n_dirs++;
        }
    }
    closedir(d);

    d = opendir(abspath);
    if (d == NULL) {
        return 0;
    }
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_REG) {

            char file_abspath[256];
            memset(file_abspath, 0, sizeof(file_abspath));
            sprintf(file_abspath, "%s/%s", path, dir->d_name);
            printf("%s\n", file_abspath);
            sprintf(file_abspath, "%s/%s", abspath, dir->d_name);

            struct zmg_file_entry entry;
            entry.off_data = sizeof(entry);
            strcpy(entry.name, dir->d_name);
            entry.type = ENTRY_TYPE_FILE;
            entry.file_size = get_file_size(file_abspath);

            off_t entry_off = ftell(dest);

            //  write ziped data
            size_t data_size = 0;
            fseek(dest, sizeof(entry), SEEK_CUR);
            if (entry.file_size > 0) {
                zip_file_to_file(file_abspath, dest, &data_size);
            }

            //  seek back and write entry
            fseek(dest, entry_off, SEEK_SET);
            entry.data_size = data_size;
            fwrite(&entry, sizeof(entry), 1, dest);
            fseek(dest, data_size, SEEK_CUR);

            n_files++;
        }
    }
    closedir(d);

    struct dir_entry_pos *pos;
    HASH_FIND_STR(*poses, path, pos);
    assert(pos != NULL);
    off_t entoff = pos->offset;
    pos->dirent.off_data = (uint64_t) (start_off - entoff);
    pos->dirent.n_files = n_files;
    pos->dirent.n_dirs = n_dirs;

    off_t nowoff = ftell(dest);
    fseek(dest, entoff, SEEK_SET);
    fwrite(&pos->dirent, sizeof(pos->dirent), 1, dest);
    fseek(dest, nowoff, SEEK_SET);

    return 0;
}

void compress_dir(const char *root, const char *out_filename) {

    struct dir_path {
        char path[256];
        struct dir_path *next;
    };

    FILE *dest = fopen(out_filename, "wb+");
    fseek(dest, 0, SEEK_SET);

    struct zmg_header header;
    init_zmg_header(&header);
    fwrite(&header, sizeof(header), 1, dest);

    struct dir_entry_pos *poses = NULL;

    struct dir_entry_pos *curpos = (dir_entry_pos*)malloc(sizeof(struct dir_entry_pos));
    strcpy(curpos->path, "");
    curpos->offset = ftell(dest);
    curpos->dirent.type = ENTRY_TYPE_DIR;
    strcpy(curpos->dirent.name, "");
    curpos->dirent.n_dirs = 0;
    curpos->dirent.n_files = 0;
    curpos->dirent.off_data = sizeof(curpos->dirent);
    HASH_ADD_STR(poses, path, curpos);

    fwrite(&curpos->dirent, sizeof(curpos->dirent), 1, dest);

    struct dir_path *dirs = NULL;
    struct dir_path *dir = (dir_path*)malloc(sizeof(struct dir_path));
    strcpy(dir->path, "");
    LL_APPEND(dirs, dir);

    char abspath[256];

    int count = 0;
    LL_COUNT(dirs, dir, count);
    while (count > 0) {

        dir = dirs;
        compress_dir_l1(root, dir->path, dest, &poses);

        dirs = dirs->next;

        sprintf(abspath, "%s/%s", root, dir->path);
        DIR *d = opendir(abspath);
        assert(d != NULL);
        struct dirent *dire;
        while ((dire = readdir(d)) != NULL) {
            if (dire->d_type == DT_DIR
                && strcmp(dire->d_name, ".") != 0 &&
                strcmp(dire->d_name, "..") != 0) {

                struct dir_path *cur = (dir_path*)malloc(sizeof(struct dir_path));
                sprintf(cur->path, "%s/%s", dir->path, dire->d_name);
                cur->next = NULL;
                LL_APPEND(dirs, cur);
            }
        }
        free(dir);
        dir = NULL;

        LL_COUNT(dirs, dir, count);
    }

    struct dir_entry_pos *current, *tmp;
    HASH_ITER(hh, poses, current, tmp) {
        HASH_DEL(poses, current);  /* delete; users advances to next */
        free(current);            /* optional- if you want to free  */
    }
}

void MakeZmg(std::string zmgpath, std::string zmgfile)
{
    char* ipath = new char[zmgpath.size() + 1];
    strcpy(ipath, zmgpath.c_str());
    char* iname = new char[zmgfile.size() + 1];
    strcpy(iname, zmgfile.c_str());
    compress_dir(ipath, iname);
};

/*
int main(int argc, char **argv) {

    compress_dir(argv[1], argv[2]);

    return 0;
}
*/
