/*
 * Library to access the storage media (SM) RAW format
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMRAW_H )
#define _LIBSMRAW_H

#include "libsmraw/codepage.h"
#include "libsmraw/definitions.h"
#include "libsmraw/error.h"
#include "libsmraw/extern.h"
#include "libsmraw/features.h"
#include "libsmraw/types.h"

#include <stdio.h>

#if defined( LIBSMRAW_HAVE_BFIO )
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
LIBSMRAW_EXTERN \
const char *libsmraw_get_version(
             void );

/* Returns the access flags for reading
 */
LIBSMRAW_EXTERN \
int libsmraw_get_access_flags_read(
     void );

/* Returns the access flags for reading and writing
 */
LIBSMRAW_EXTERN \
int libsmraw_get_access_flags_read_write(
     void );

/* Returns the access flags for writing
 */
LIBSMRAW_EXTERN \
int libsmraw_get_access_flags_write(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_get_codepage(
     int *codepage,
     libsmraw_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_set_codepage(
     int codepage,
     libsmraw_error_t **error );

/* Globs the files according to common (split) RAW naming schemas
 * Make sure the value filenames is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_glob(
     const char *filename,
     size_t filename_length,
     char **filenames[],
     int *number_of_filenames,
     libsmraw_error_t **error );

/* Frees globbed filenames
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_glob_free(
     char *filenames[],
     int number_of_filenames,
     libsmraw_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Globs the files according to common (split) RAW naming schemas
 * Make sure the value filenames is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     wchar_t **filenames[],
     int *number_of_filenames,
     libsmraw_error_t **error );

/* Frees globbed wide filenames
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libsmraw_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBSMRAW_EXTERN \
void libsmraw_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_notify_set_stream(
     FILE *stream,
     libsmraw_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_notify_stream_open(
     const char *filename,
     libsmraw_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_notify_stream_close(
     libsmraw_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBSMRAW_EXTERN \
void libsmraw_error_free(
      libsmraw_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_error_fprint(
     libsmraw_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_error_sprint(
     libsmraw_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_error_backtrace_fprint(
     libsmraw_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_error_backtrace_sprint(
     libsmraw_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Handle functions
 * ------------------------------------------------------------------------- */

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_initialize(
     libsmraw_handle_t **handle,
     libsmraw_error_t **error );

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_free(
     libsmraw_handle_t **handle,
     libsmraw_error_t **error );

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_signal_abort(
     libsmraw_handle_t *handle,
     libsmraw_error_t **error );

/* Opens a set of storage media RAW files
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_open(
     libsmraw_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libsmraw_error_t **error );

#if defined( LIBSMRAW_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a set of storage media RAW files
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_open_wide(
     libsmraw_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libsmraw_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBSMRAW_HAVE_BFIO )

/* Opens a set of storage media RAW files using a Basic File IO (bfio) pool
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_open_file_io_pool(
     libsmraw_handle_t *handle,
     libbfio_pool_t *file_io_pool,
     int access_flags,
     libsmraw_error_t **error );

#endif /* defined( LIBSMRAW_HAVE_BFIO ) */

/* Closes the handle
 * Returns the 0 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_close(
     libsmraw_handle_t *handle,
     libsmraw_error_t **error );

/* Reads a buffer from the handle
 * Returns the number of bytes read or -1 on error
 */
LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_read_buffer(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libsmraw_error_t **error );

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_read_buffer_at_offset(
         libsmraw_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libsmraw_error_t **error );

/* Writes a buffer to the handle
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_write_buffer(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         libsmraw_error_t **error );

/* Writes (media) data at a specific offset,
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
LIBSMRAW_EXTERN \
ssize_t libsmraw_handle_write_buffer_at_offset(
         libsmraw_handle_t *handle,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libsmraw_error_t **error );

/* Seeks an offset in the handle
 * Returns the number of bytes in the buffer or -1 on error
 */
LIBSMRAW_EXTERN \
off64_t libsmraw_handle_seek_offset(
         libsmraw_handle_t *handle,
         off64_t offset,
         int whence,
         libsmraw_error_t **error );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_offset(
     libsmraw_handle_t *handle,
     off64_t *offset,
     libsmraw_error_t **error );

/* Sets the maximum number of (concurrent) open file handles
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_maximum_number_of_open_handles(
     libsmraw_handle_t *handle,
     int maximum_number_of_open_handles,
     libsmraw_error_t **error );

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libsmraw_error_t **error );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libsmraw_error_t **error );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_segment_filename(
     libsmraw_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libsmraw_error_t **error );

#if defined( LIBSMRAW_HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the segment filename size
 * The filename size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libsmraw_error_t **error );

/* Retrieves the segment filename
 * The filename size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_segment_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libsmraw_error_t **error );

/* Sets the segment filename
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_segment_filename_wide(
     libsmraw_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libsmraw_error_t **error );

#endif /* defined( LIBSMRAW_HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the maximum segment file size
 * 0 represents the maximum possible segment file size
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t *maximum_segment_size,
     libsmraw_error_t **error );

/* Sets the segment file size
 * 0 represents the maximum possible segment file size
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_maximum_segment_size(
     libsmraw_handle_t *handle,
     size64_t maximum_segment_size,
     libsmraw_error_t **error );

/* Retrieves the filename size of the segment file of the current offset
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_size(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libsmraw_error_t **error );

/* Retrieves the filename of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename(
     libsmraw_handle_t *handle,
     char *filename,
     size_t filename_size,
     libsmraw_error_t **error );

#if defined( LIBSMRAW_HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the segment file of the current offset
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_size_wide(
     libsmraw_handle_t *handle,
     size_t *filename_size,
     libsmraw_error_t **error );

/* Retrieves the filename of the segment file of the current offset
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_filename_wide(
     libsmraw_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libsmraw_error_t **error );

#endif /* defined( LIBSMRAW_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBSMRAW_HAVE_BFIO )

/* Retrieves the file IO handle of the segment file of the current offset
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_file_io_handle(
     libsmraw_handle_t *handle,
     libbfio_handle_t **file_io_handle,
     libsmraw_error_t **error );

#endif /* defined( LIBSMRAW_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Meta data functions
 * ------------------------------------------------------------------------- */

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_size(
     libsmraw_handle_t *handle,
     size64_t *media_size,
     libsmraw_error_t **error );

/* Sets the media size
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_size(
     libsmraw_handle_t *handle,
     size64_t media_size,
     libsmraw_error_t **error );

/* Retrieves the number of bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t *bytes_per_sector,
     libsmraw_error_t **error );

/* Sets the number of bytes per sector
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_bytes_per_sector(
     libsmraw_handle_t *handle,
     uint32_t bytes_per_sector,
     libsmraw_error_t **error );

/* Retrieves the media type
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_type(
     libsmraw_handle_t *handle,
     uint8_t *media_type,
     libsmraw_error_t **error );

/* Sets the media type
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_type(
     libsmraw_handle_t *handle,
     uint8_t media_type,
     libsmraw_error_t **error );

/* Retrieves the media flags
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_media_flags(
     libsmraw_handle_t *handle,
     uint8_t *media_flags,
     libsmraw_error_t **error );

/* Sets the media flags
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_media_flags(
     libsmraw_handle_t *handle,
     uint8_t media_flags,
     libsmraw_error_t **error );

/* Retrieves the number of information values
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_number_of_information_values(
     libsmraw_handle_t *handle,
     int *number_of_information_values,
     libsmraw_error_t **error );

/* Retrieves the size of a specific information value identifier
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_information_value_identifier_size(
     libsmraw_handle_t *handle,
     int information_value_index,
     size_t *identifier_size,
     libsmraw_error_t **error );

/* Retrieves a specific information value identifier
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_information_value_identifier(
     libsmraw_handle_t *handle,
     int information_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libsmraw_error_t **error );

/* Retrieves the size of an UTF-8 encoded information value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libsmraw_error_t **error );

/* Retrieves an UTF-8 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libsmraw_error_t **error );

/* Sets an UTF-8 encoded information value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf8_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libsmraw_error_t **error );

/* Retrieves the size of an UTF-16 encoded information value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_information_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libsmraw_error_t **error );

/* Retrieves an UTF-16 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libsmraw_error_t **error );

/* Sets an UTF-16 encoded information value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf16_information_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libsmraw_error_t **error );

/* Retrieves the number of integrity hash values
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_number_of_integrity_hash_values(
     libsmraw_handle_t *handle,
     int *number_of_integrity_hash_values,
     libsmraw_error_t **error );

/* Retrieves the size of a specific integrity hash value identifier
 * The identifier size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_integrity_hash_value_identifier_size(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     size_t *identifier_size,
     libsmraw_error_t **error );

/* Retrieves a specific integrity hash value identifier
 * The identifier size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_integrity_hash_value_identifier(
     libsmraw_handle_t *handle,
     int integrity_hash_value_index,
     uint8_t *identifier,
     size_t identifier_size,
     libsmraw_error_t **error );

/* Retrieves the size of an UTF-8 encoded integrity hash value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf8_string_size,
     libsmraw_error_t **error );

/* Retrieves an UTF-8 encoded integrity hash value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libsmraw_error_t **error );

/* Sets an UTF-8 encoded integrity hash value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf8_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libsmraw_error_t **error );

/* Retrieves the size of an UTF-16 encoded integrity hash value for the specific identifier
 * The value size includes the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_integrity_hash_value_size(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     size_t *utf16_string_size,
     libsmraw_error_t **error );

/* Retrieves an UTF-16 encoded integrity hash value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_get_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libsmraw_error_t **error );

/* Sets an UTF-16 encoded integrity hash value for the specific identifier
 * Returns the 1 if succesful or -1 on error
 */
LIBSMRAW_EXTERN \
int libsmraw_handle_set_utf16_integrity_hash_value(
     libsmraw_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libsmraw_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMRAW_H ) */

