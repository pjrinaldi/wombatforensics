/*
 * Library to access the Parallels Hard Disk image format
 *
 * Copyright (C) 2015-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBPHDI_H )
#define _LIBPHDI_H

#include <libphdi/codepage.h>
#include <libphdi/definitions.h>
#include <libphdi/error.h>
#include <libphdi/extern.h>
#include <libphdi/features.h>
#include <libphdi/types.h>

#include <stdio.h>

#if defined( LIBPHDI_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBPHDI_EXTERN \
const char *libphdi_get_version(
             void );

/* Returns the access flags for reading
 */
LIBPHDI_EXTERN \
int libphdi_get_access_flags_read(
     void );

/* Returns the access flags for reading and writing
 */
LIBPHDI_EXTERN \
int libphdi_get_access_flags_read_write(
     void );

/* Returns the access flags for writing
 */
LIBPHDI_EXTERN \
int libphdi_get_access_flags_write(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_get_codepage(
     int *codepage,
     libphdi_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_set_codepage(
     int codepage,
     libphdi_error_t **error );

/* Determines if a file contains a PHDI file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_check_file_signature(
     const char *filename,
     libphdi_error_t **error );

#if defined( LIBPHDI_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains a PHDI file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_check_file_signature_wide(
     const wchar_t *filename,
     libphdi_error_t **error );

#endif /* defined( LIBPHDI_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBPHDI_HAVE_BFIO )

/* Determines if a file is a PHDI file (check for the PHDI file signature) using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libphdi_error_t **error );

#endif /* defined( LIBPHDI_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBPHDI_EXTERN \
void libphdi_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_notify_set_stream(
     FILE *stream,
     libphdi_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_notify_stream_open(
     const char *filename,
     libphdi_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_notify_stream_close(
     libphdi_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBPHDI_EXTERN \
void libphdi_error_free(
      libphdi_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_error_fprint(
     libphdi_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_error_sprint(
     libphdi_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_error_backtrace_fprint(
     libphdi_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_error_backtrace_sprint(
     libphdi_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Handle functions
 * ------------------------------------------------------------------------- */

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_initialize(
     libphdi_handle_t **handle,
     libphdi_error_t **error );

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_free(
     libphdi_handle_t **handle,
     libphdi_error_t **error );

/* Signals a handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_signal_abort(
     libphdi_handle_t *handle,
     libphdi_error_t **error );

/* Opens a handle
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_open(
     libphdi_handle_t *handle,
     const char *filename,
     int access_flags,
     libphdi_error_t **error );

#if defined( LIBPHDI_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a handle
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_open_wide(
     libphdi_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libphdi_error_t **error );

#endif /* defined( LIBPHDI_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBPHDI_HAVE_BFIO )

/* Opens a handle using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_open_file_io_handle(
     libphdi_handle_t *handle,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libphdi_error_t **error );

#endif /* defined( LIBPHDI_HAVE_BFIO ) */

/* Opens the extent data files
 * If the extent data filenames were not set explicitly this function assumes the extent data files
 * are in the same location as the descriptor file
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_open_extent_data_files(
     libphdi_handle_t *handle,
     libphdi_error_t **error );

#if defined( LIBPHDI_HAVE_BFIO )

/* Opens the extent data files using a Basic File IO (bfio) pool
 * This function assumes the extent data files are in same order as defined by the descriptor file
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_open_extent_data_files_file_io_pool(
     libphdi_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     libphdi_error_t **error );

#endif /* defined( LIBPHDI_HAVE_BFIO ) */

/* Closes a handle
 * Returns 0 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_close(
     libphdi_handle_t *handle,
     libphdi_error_t **error );

/* Reads (media) data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBPHDI_EXTERN \
ssize_t libphdi_handle_read_buffer(
         libphdi_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libphdi_error_t **error );

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBPHDI_EXTERN \
ssize_t libphdi_handle_read_buffer_at_offset(
         libphdi_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libphdi_error_t **error );

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
LIBPHDI_EXTERN \
off64_t libphdi_handle_seek_offset(
         libphdi_handle_t *handle,
         off64_t offset,
         int whence,
         libphdi_error_t **error );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_offset(
     libphdi_handle_t *handle,
     off64_t *offset,
     libphdi_error_t **error );

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_set_maximum_number_of_open_handles(
     libphdi_handle_t *handle,
     int maximum_number_of_open_handles,
     libphdi_error_t **error );

/* Sets the path to the extend data files
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_set_extent_data_files_path(
     libphdi_handle_t *handle,
     const char *path,
     size_t path_length,
     libphdi_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Sets the path to the extend data files
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_set_extent_data_files_path_wide(
     libphdi_handle_t *handle,
     const wchar_t *path,
     size_t path_length,
     libphdi_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_media_size(
     libphdi_handle_t *handle,
     size64_t *media_size,
     libphdi_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_utf8_name_size(
     libphdi_handle_t *handle,
     size_t *utf8_string_size,
     libphdi_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_utf8_name(
     libphdi_handle_t *handle,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libphdi_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_utf16_name_size(
     libphdi_handle_t *handle,
     size_t *utf16_string_size,
     libphdi_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_utf16_name(
     libphdi_handle_t *handle,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libphdi_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_number_of_extents(
     libphdi_handle_t *handle,
     int *number_of_extents,
     libphdi_error_t **error );

/* Retrieves a specific extent descriptor
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_extent_descriptor_by_index(
     libphdi_handle_t *handle,
     int extent_index,
     libphdi_extent_descriptor_t **extent_descriptor,
     libphdi_error_t **error );

/* Retrieves the number of snapshots
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_number_of_snapshots(
     libphdi_handle_t *handle,
     int *number_of_snapshots,
     libphdi_error_t **error );

/* Retrieves a specific snapshot
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_handle_get_snapshot_by_index(
     libphdi_handle_t *handle,
     int snapshot_index,
     libphdi_snapshot_t **snapshot,
     libphdi_error_t **error );

/* -------------------------------------------------------------------------
 * Extent descriptor functions
 * ------------------------------------------------------------------------- */

/* Frees an extent descriptor
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_extent_descriptor_free(
     libphdi_extent_descriptor_t **extent_descriptor,
     libphdi_error_t **error );

/* Retrieves the extent range (offset and size)
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_extent_descriptor_get_range(
     libphdi_extent_descriptor_t *extent_descriptor,
     off64_t *offset,
     size64_t *size,
     libphdi_error_t **error );

/* Retrieves the number of images
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_extent_descriptor_get_number_of_images(
     libphdi_extent_descriptor_t *extent_descriptor,
     int *number_of_images,
     libphdi_error_t **error );

/* Retrieves a specific image descriptor
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_extent_descriptor_get_image_descriptor_by_index(
     libphdi_extent_descriptor_t *extent_descriptor,
     int image_index,
     libphdi_image_descriptor_t **image_descriptor,
     libphdi_error_t **error );

/* -------------------------------------------------------------------------
 * Image descriptor functions
 * ------------------------------------------------------------------------- */

/* Frees an image descriptor
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_image_descriptor_free(
     libphdi_image_descriptor_t **image_descriptor,
     libphdi_error_t **error );

/* Retrieves the image type
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_image_descriptor_get_type(
     libphdi_image_descriptor_t *image_descriptor,
     int *type,
     libphdi_error_t **error );

/* Retrieves the size of the UTF-8 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_image_descriptor_get_utf8_filename_size(
     libphdi_image_descriptor_t *image_descriptor,
     size_t *utf8_string_size,
     libphdi_error_t **error );

/* Retrieves the UTF-8 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_image_descriptor_get_utf8_filename(
     libphdi_image_descriptor_t *image_descriptor,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libphdi_error_t **error );

/* Retrieves the size of the UTF-16 encoded filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_image_descriptor_get_utf16_filename_size(
     libphdi_image_descriptor_t *image_descriptor,
     size_t *utf16_string_size,
     libphdi_error_t **error );

/* Retrieves the UTF-16 encoded filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_image_descriptor_get_utf16_filename(
     libphdi_image_descriptor_t *image_descriptor,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libphdi_error_t **error );

/* -------------------------------------------------------------------------
 * Snapshot functions
 * ------------------------------------------------------------------------- */

/* Frees a snapshot
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_snapshot_free(
     libphdi_snapshot_t **snapshot,
     libphdi_error_t **error );

/* Retrieves the identifier
 * The identifier is a big-endian GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_snapshot_get_identifier(
     libphdi_snapshot_t *snapshot,
     uint8_t *guid_data,
     size_t guid_data_size,
     libphdi_error_t **error );

/* Retrieves the parent identifier
 * The identifier is a big-endian GUID and is 16 bytes of size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBPHDI_EXTERN \
int libphdi_snapshot_get_parent_identifier(
     libphdi_snapshot_t *snapshot,
     uint8_t *guid_data,
     size_t guid_data_size,
     libphdi_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPHDI_H ) */

