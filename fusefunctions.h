/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPLv2.
  See the file COPYING.
*/

/** @file
 *
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall hello.c `pkg-config fuse3 --cflags --libs` -o hello
 *
 * ## Source code ##
 * \include hello.c
 *
 * Modified by Pasquale J. Rinaldi, Jr. to be used for aff fuse mounting
 * as a function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define FUSE_USE_VERSION 31

#include <afflib/afflib.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
static struct options {
	const char *filename;
	const char *mntpt;
	int show_help;
} options;

#define XCALLOC(type, num) ((type *) xcalloc ((num), sizeof(type)))
#define XMALLOC(type, num) ((type *) xmalloc ((num) * sizeof(type)))
#define XFREE(stale) do { if(stale) { free ((void*) stale); stale = 0; } } while (0)

static AFFILE* afimage = NULL;
static char* rawpath = NULL;
static off_t rawsize = 0;
static const char* rawext = ".raw";

/*
#define OPTION(t, p)                           \
    { t, offsetof(struct options, p), 1 }
static const struct fuse_opt option_spec[] = {
	OPTION("--name=%s", filename),
	OPTION("--mntpt=%s", mntpt),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	FUSE_OPT_END
};*/

static void* xmalloc(size_t num)
{
    void* alloc = malloc(num);
    if(!alloc)
    {
	perror("Memory Exhausted");
	exit(EXIT_FAILURE);
    }
    return alloc;
};

static void* xcalloc(size_t num, size_t size)
{
    void* alloc = xmalloc(num*size);
    memset(alloc, 0, num*size);
    return alloc;
};

static char* xstrdup(char* string)
{
    return strcpy((char*)xmalloc(strlen(string) + 1), string);
};

static void *hello_init(struct fuse_conn_info *conn,
			struct fuse_config *cfg)
{
	(void) conn;
	cfg->kernel_cache = 1;
	return NULL;
};

static int hello_getattr(const char *path, struct stat *stbuf,
			 struct fuse_file_info *fi)
{
	(void) fi;
	int res = 0;

	printf("rawpath: %s\n", rawpath);
	//} else if (strcmp(path+1, options.filename) == 0) {
	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if(strcmp(path, rawpath) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = rawsize;
		//stbuf->st_size = strlen(options.mntpt);
	} else
		res = -ENOENT;

	return res;
};

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi,
			 enum fuse_readdir_flags flags)
{
	(void) offset;
	(void) fi;
	(void) flags;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);
	filler(buf, rawpath + 1, NULL, 0, 0);
	//filler(buf, options.filename, NULL, 0, 0);

	return 0;
};

static int hello_open(const char *path, struct fuse_file_info *fi)
{
	//if (strcmp(path+1, options.filename) != 0)
	if(strcmp(path, rawpath) != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	//af = af_open(options.filename, int flags,int mode);
	return 0;
};

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	//size_t len;
	int res = 0;
	(void) fi;
	//if(strcmp(path+1, options.filename) != 0)
	if(strcmp(path, rawpath) != 0)
		return -ENOENT;
	af_seek(afimage, (uint64_t)offset, SEEK_SET);
	errno = 0;
	res = af_read(afimage, (unsigned char*)buf, (int)size);
	return res;
	/*
	len = strlen(options.contents);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, options.contents + offset, size);
	} else
		size = 0;

	return size;
	*/
};

static void hello_destroy(void* param)
{
    af_close(afimage);
    //free(rawpath);
    XFREE(rawpath);
    return;
};

static const struct fuse_operations hello_oper = {
	.init           = hello_init,
	.getattr	= hello_getattr,
	.readdir	= hello_readdir,
	.open		= hello_open,
	.read		= hello_read,
	.destroy	= hello_destroy,
};

/*
static void show_help(const char *progname)
{
	printf("usage: %s [options] <mountpoint>\n\n", progname);
	printf("File-system specific options:\n"
	       "    --name=<s>          Name of the \"hello\" file\n"
	       "                        (default: \"hello\")\n"
	       "    --mntpt=<s>      Contents \"hello\" file\n"
	       "                        (default \"Hello, World!\\n\")\n"
	       "\n");
};
*/

