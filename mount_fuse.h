/*
 * Mount tool fuse functions
 *
 * Copyright (C) 2006-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _MOUNT_FUSE_H )
#define _MOUNT_FUSE_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
//#define FUSE_USE_VERSION	26

#if defined( HAVE_LIBFUSE )
#include <fuse.h>

#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#include "ewftools_libcerror.h"
#include "ewftools_libewf.h"
#include "mount_file_entry.h"
#include "mount_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

int mount_fuse_set_stat_info(
     struct stat *stat_info,
     size64_t size,
     uint16_t file_mode,
     int64_t access_time,
     int64_t inode_change_time,
     int64_t modification_time,
     libcerror_error_t **error );

int mount_fuse_filldir(
     void *buffer,
     fuse_fill_dir_t filler,
     const char *name,
     struct stat *stat_info,
     mount_file_entry_t *file_entry,
     libcerror_error_t **error );

int mount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_fuse_open";
	int result               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %s\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info->fh != (uint64_t) NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file information - file handle already set.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( ( file_info->flags & 0x03 ) != O_RDONLY )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		result = -EACCES;

		goto on_error;
	}
	if( mount_handle_get_file_entry_by_path(
	     ewfmount_mount_handle,
	     path,
	     (mount_file_entry_t **) &( file_info->fh ),
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry for path: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
};

int mount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_release(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_opendir(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_releasedir(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_getattr(
     const char *path,
     struct stat *stat_info );

void mount_fuse_destroy(
      void *private_data );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_FUSE_H ) */

