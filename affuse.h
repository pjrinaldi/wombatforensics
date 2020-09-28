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

#ifndef AFFFUSE_H
#define AFFFUSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define FUSE_USE_VERSION 35

#include <afflib/afflib.h>
#include <fuse3/fuse.h>
#include <fuse3/fuse_lowlevel.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <pthread.h>
#include <sys/fsuid.h>
#include <paths.h>

static AFFILE* afimage = NULL;
static char* rawpath = NULL;
static off_t rawsize = 0;
static const char* rawext = ".raw";

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

	return 0;
};

static int affuse_open(const char *path, struct fuse_file_info *fi)
{
	if(strcmp(path, rawpath) != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	return 0;
};

static int affuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int res = 0;
	(void) fi;
	if(strcmp(path, rawpath) != 0)
		return -ENOENT;
	af_seek(afimage, (uint64_t)offset, SEEK_SET);
	errno = 0;
	res = af_read(afimage, (unsigned char*)buf, (int)size);
	return res;
};

static void affuse_destroy(void* param)
{
    af_close(afimage);
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

void* fuselooper(void *data)
{
    struct fuse* fuse = (struct fuse*) data;
    fuse_loop(fuse);
    //int ret = fuse_loop(fuse);
    //printf("fuse loop return: %d\n", ret);
    //pthread_exit(NULL);
};

struct fuse_args args;
struct fuse* affuser;
//struct fuse_session* affusersession;
pthread_t fusethread;

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
    fargv[0] = "./affuse";
    //fargv[1] = "-o";
    //fargv[2] = "auto_unmount";
    fargc = 1;
    //for(int i=0; i < fargc; i++)
    //    printf("fargv[%d]: %s\n", i, fargv[i]);
    afimage = af_open(iname, O_RDONLY|O_EXCL,0);
    afpath = xstrdup(ipath);
    printf("afpath: %s\n", afpath);
    afbasename = basename(iname);
    rawpathlen = 1 + strlen(afbasename) + strlen(rawext) + 1;
    rawpath = XCALLOC(char, rawpathlen);
    rawpath[0] = '/';
    strcat(rawpath, afbasename);
    strcat(rawpath, rawext);
    rawpath[rawpathlen - 1] = 0;
    XFREE(afpath);
    rawsize = af_get_imagesize(afimage);
    
    struct fuse_loop_config config;
    config.clone_fd = 0;
    config.max_idle_threads = 5;
    args = FUSE_ARGS_INIT(fargc, fargv);
    affuser = fuse_new(&args, &affuse_oper, sizeof(fuse_operations), NULL);
    if(affuser == NULL)
        qDebug() << "affuser new error.";
    ret = fuse_mount(affuser, imgpath.toStdString().c_str());
    int retd = fuse_daemonize(1);

    //affusersession = fuse_get_session(affuser); 
    //struct fuse_session* se = fuse_get_session(affuser);
    //int retsh = fuse_set_signal_handlers(se);
    //qDebug() << "fuse session signal handlers:" << retsh;
    //pthread_t threadId;
    int perr = pthread_create(&fusethread, NULL, fuselooper, (void *) affuser);
};

#endif // AFFUSE_H
