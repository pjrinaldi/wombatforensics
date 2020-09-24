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

#define FUSE_USE_VERSION 35

#include <afflib/afflib.h>
#include <fuse3/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <pthread.h>

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
/*
static struct options {
	const char *filename;
	const char *mntpt;
	int show_help;
} options;
*/

#define XCALLOC(type, num) ((type *) xcalloc ((num), sizeof(type)))
#define XMALLOC(type, num) ((type *) xmalloc ((num) * sizeof(type)))
#define XFREE(stale) do { if(stale) { free ((void*) stale); stale = 0; } } while (0)

static AFFILE* afimage = NULL;
static char* rawpath = NULL;
static off_t rawsize = 0;
static const char* rawext = ".raw";

/*
#define NO_TIMEOUT 500000

#define MAX_STR_LEN 128
#define TIME_FILE_NAME "current_time"
#define TIME_FILE_INO 2
#define GROW_FILE_NAME "growing"
#define GROW_FILE_INO 3
*/
//static char time_file_contents[MAX_STR_LEN];
//static size_t grow_file_size;

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

static void *affuse_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
	(void) conn;
	cfg->kernel_cache = 1;
	return NULL;
};

static int affuse_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
	(void) fi;
	int res = 0;

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

static int affuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
	(void) offset;
	(void) fi;
	(void) flags;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0, (fuse_fill_dir_flags)0);
	filler(buf, "..", NULL, 0, (fuse_fill_dir_flags)0);
	filler(buf, rawpath + 1, NULL, 0, (fuse_fill_dir_flags)0);
	//filler(buf, options.filename, NULL, 0, 0);

	return 0;
};

static int affuse_open(const char *path, struct fuse_file_info *fi)
{
	//if (strcmp(path+1, options.filename) != 0)
	if(strcmp(path, rawpath) != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	//af = af_open(options.filename, int flags,int mode);
	return 0;
};

static int affuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
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

static void affuse_destroy(void* param)
{
    af_close(afimage);
    //free(rawpath);
    XFREE(rawpath);
    return;
};

static const struct fuse_operations affuse_oper = {
	.getattr	= affuse_getattr,
	.open		= affuse_open,
	.read		= affuse_read,
	.readdir	= affuse_readdir,
	.init           = affuse_init,
	.destroy	= affuse_destroy,
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
/*
static void update_fs(void) {
	static int count = 0;
	struct tm *now;
	time_t t;
	t = time(NULL);
	now = localtime(&t);
	//assert(now != NULL);

	int time_file_size = strftime(time_file_contents, MAX_STR_LEN,
			"The current time is %H:%M:%S\n", now);
	//assert(time_file_size != 0);

	grow_file_size = count++;
};

static void* update_fs_loop(void *data) {
	struct fuse *fuse = (struct fuse*) data;

	while (1) {
		update_fs();
		/*
		if (!options.no_notify) {
			assert(invalidate(fuse, "/" TIME_FILE_NAME) == 0);
			assert(invalidate(fuse, "/" GROW_FILE_NAME) == 0);
		}
		*/
/*		sleep(1);
	}
	return NULL;
};
*/
void* fuselooper(void *data)
{
    struct fuse* fuse = (struct fuse*) data;
    int ret = fuse_loop(fuse);
    printf("fuse loop return: %d\n", ret);
};

struct fuse_args args;
struct fuse* affuser;

void AffFuser(QString imgpath, QString imgfile)
{
    int ret;
    char* afpath = NULL;
    char* afbasename = NULL;
    size_t rawpathlen = 0;
    char** fargv = NULL;
    fargv = XCALLOC(char *, 3);
    int fargc = 0;
    char* ipath = new char[imgpath.toStdString().size() + 1];
    strcpy(ipath, imgpath.toStdString().c_str());
    char* iname = new char[imgfile.toStdString().size() + 1];
    strcpy(iname, imgfile.toStdString().c_str());
    //progname = strdup("./affuse");
    fargv[0] = "./affuse";
    //fargv[1] = ipath;
    //fargv[1] = "-s";
    fargc = 1;
    for(int i=0; i < fargc; i++)
        printf("fargv[%d]: %s\n", i, fargv[i]);
    afimage = af_open(iname, O_RDONLY|O_EXCL,0);
    afpath = xstrdup(ipath);
    printf("afpath: %s\n", afpath);
    afbasename = basename(iname);
    rawpathlen = 1 + strlen(afbasename) + strlen(rawext) + 1;
    //rawpathlen = 1 + strlen(ipath) + strlen(afbasename) + strlen(rawext) + 1;
    rawpath = XCALLOC(char, rawpathlen);
    rawpath[0] = '/';
    //strcat(rawpath, ipath);
    //rawpath[0] = '/';
    strcat(rawpath, afbasename);
    strcat(rawpath, rawext);
    rawpath[rawpathlen - 1] = 0;
    printf("rawpath: %s\n", rawpath);
    XFREE(afpath);
    rawsize = af_get_imagesize(afimage);
    
    struct fuse_loop_config config;
    config.clone_fd = 0;
    config.max_idle_threads = 5;
    //struct fuse_args args = FUSE_ARGS_INIT(fargc, fargv);
    args = FUSE_ARGS_INIT(fargc, fargv);
    //struct fuse* affuser = fuse_new(&args, &hello_oper, sizeof(hello_oper), NULL);
    struct fuse* affuser = fuse_new(&args, &affuse_oper, sizeof(fuse_operations), NULL);
    if(affuser == NULL)
        qDebug() << "affuser new error.";
    ret = fuse_mount(affuser, imgpath.toStdString().c_str());
    qDebug() << "fuse mount return:" << ret;
    int retd = fuse_daemonize(1);
    qDebug() << "fuse daemonize return:" << retd;

    //pthread_t updater;     // Start thread to update file contents
    //int pret = pthread_create(&updater, NULL, update_fs_loop, (void *) affuser);
    //if (pret != 0)
        //fprintf(stderr, "pthread_create failed with %s\n", strerror(ret));
     
    struct fuse_session* se = fuse_get_session(affuser);
    int retsh = fuse_set_signal_handlers(se);
    qDebug() << "fuse session signal handlers:" << retsh;
    pthread_t threadId;
    int perr = pthread_create(&threadId, NULL, fuselooper, (void *) affuser);
};
