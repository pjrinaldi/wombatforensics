/*
 * Definitions for libewf
 *
 * Copyright (C) 2006-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBEWF_DEFINITIONS_H )
#define _LIBEWF_DEFINITIONS_H

#include "types.h"

#define LIBEWF_VERSION						20230212

/* The version string
 */
#define LIBEWF_VERSION_STRING					"20230212"

/* The access flags definitions
 * bit 1							set to 1 for read access
 * bit 2							set to 1 for write access
 * bit 3-4							not used
 * bit 5        set to 1 to resume write
 * bit 6-8							not used
 */
enum LIBEWF_ACCESS_FLAGS
{
	LIBEWF_ACCESS_FLAG_READ					= 0x01,
	LIBEWF_ACCESS_FLAG_WRITE				= 0x02,

	LIBEWF_ACCESS_FLAG_RESUME				= 0x10
};

/* The file access macros
 */
#define LIBEWF_OPEN_READ					( LIBEWF_ACCESS_FLAG_READ )
#define LIBEWF_OPEN_WRITE					( LIBEWF_ACCESS_FLAG_WRITE )
#define LIBEWF_OPEN_WRITE_RESUME				( LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME )

/* The file formats
 */
enum LIBEWF_FORMAT
{
	LIBEWF_FORMAT_UNKNOWN					= 0x00,
	LIBEWF_FORMAT_ENCASE1					= 0x01,
	LIBEWF_FORMAT_ENCASE2					= 0x02,
	LIBEWF_FORMAT_ENCASE3					= 0x03,
	LIBEWF_FORMAT_ENCASE4					= 0x04,
	LIBEWF_FORMAT_ENCASE5					= 0x05,
	LIBEWF_FORMAT_ENCASE6					= 0x06,
	LIBEWF_FORMAT_ENCASE7					= 0x07,

	LIBEWF_FORMAT_SMART					= 0x0e,
	LIBEWF_FORMAT_FTK_IMAGER				= 0x0f,

	LIBEWF_FORMAT_LOGICAL_ENCASE5				= 0x10,
	LIBEWF_FORMAT_LOGICAL_ENCASE6				= 0x11,
	LIBEWF_FORMAT_LOGICAL_ENCASE7				= 0x12,

	LIBEWF_FORMAT_LINEN5					= 0x25,
	LIBEWF_FORMAT_LINEN6					= 0x26,
	LIBEWF_FORMAT_LINEN7					= 0x27,

	LIBEWF_FORMAT_V2_ENCASE7				= 0x37,

	LIBEWF_FORMAT_V2_LOGICAL_ENCASE7			= 0x47,

	/* The format as specified by Andrew Rosen
	 */
	LIBEWF_FORMAT_EWF					= 0x70,

	/* Libewf eXtended EWF format
	 */
	LIBEWF_FORMAT_EWFX					= 0x71
};

/* The default segment file size
 */
#define LIBEWF_DEFAULT_SEGMENT_FILE_SIZE			( 1500 * 1024 * 1024 )

/* The compression methods definitions
 */
enum LIBEWF_COMPRESSION_METHODS
{
	LIBEWF_COMPRESSION_METHOD_NONE				= 0,
	LIBEWF_COMPRESSION_METHOD_DEFLATE			= 1,
	LIBEWF_COMPRESSION_METHOD_BZIP2				= 2,
};

/* The compression level definitions
 */
enum LIBEWF_COMPRESSION_LEVELS
{
	LIBEWF_COMPRESSION_LEVEL_DEFAULT			= -1,
	LIBEWF_COMPRESSION_LEVEL_NONE				= 0,
	LIBEWF_COMPRESSION_LEVEL_FAST				= 1,
	LIBEWF_COMPRESSION_LEVEL_BEST				= 2,
};

#define LIBEWF_COMPRESSION_DEFAULT				LIBEWF_COMPRESSION_LEVEL_DEFAULT
#define LIBEWF_COMPRESSION_NONE					LIBEWF_COMPRESSION_LEVEL_NONE
#define LIBEWF_COMPRESSION_FAST					LIBEWF_COMPRESSION_LEVEL_FAST
#define LIBEWF_COMPRESSION_BEST					LIBEWF_COMPRESSION_LEVEL_BEST

/* The compression flags
 * bit 1							set to 1 for empty block compression
 *              detects empty blocks and stored them compressed, the compression
 *              is only done once
 * bit 2							set to 1 for pattern fill compression
 *              this implies empty block compression using the pattern fill method
 *              used internally only
 * bit 3-8							not used
 */
enum LIBEWF_COMPRESSION_FLAGS
{
	LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION	= (uint8_t) 0x01,
	LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION	= (uint8_t) 0x10,
};

/* The media type definitions
 */
enum LIBEWF_MEDIA_TYPES
{
	LIBEWF_MEDIA_TYPE_REMOVABLE				= 0x00,
	LIBEWF_MEDIA_TYPE_FIXED					= 0x01,
	LIBEWF_MEDIA_TYPE_OPTICAL				= 0x03,
	LIBEWF_MEDIA_TYPE_SINGLE_FILES				= 0x0e,
	LIBEWF_MEDIA_TYPE_MEMORY				= 0x10
};

/* The media flags definitions
 */
enum LIBEWF_MEDIA_FLAGS
{
	LIBEWF_MEDIA_FLAG_PHYSICAL				= 0x02,
	LIBEWF_MEDIA_FLAG_FASTBLOC				= 0x04,
	LIBEWF_MEDIA_FLAG_TABLEAU				= 0x08
};

/* The date representation formats
 */
enum LIBEWF_DATE_FORMATS
{
	LIBEWF_DATE_FORMAT_DAYMONTH				= 0x01,
	LIBEWF_DATE_FORMAT_MONTHDAY				= 0x02,
	LIBEWF_DATE_FORMAT_ISO8601				= 0x03,
	LIBEWF_DATE_FORMAT_CTIME				= 0x04
};

/* The header value compression levels definitions
 */
#define LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_NONE		"n"
#define LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_FAST		"f"
#define LIBEWF_HEADER_VALUE_COMPRESSION_LEVEL_BEST		"b"

/* The segment file type definitions
 */
enum LIBEWF_SEGMENT_FILE_TYPES
{
	LIBEWF_SEGMENT_FILE_TYPE_UNDEFINED			= 0,
	LIBEWF_SEGMENT_FILE_TYPE_EWF1				= 1,
	LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART			= 2,
	LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL			= 3,
	LIBEWF_SEGMENT_FILE_TYPE_EWF2				= 4,
	LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL			= 5
};

/* The chunk IO (read/write) flags
 */
enum LIBEWF_CHUNK_IO_FLAGS
{
	/* Indicates the checksum should be read or written
	 */
	LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET			= 0x01,

	/* Indicates the padding should be read or written
	 */
	LIBEWF_CHUNK_IO_FLAG_PADDING_SET			= 0x02,

	/* Indicates the chunk is packed
	 */
	LIBEWF_CHUNK_IO_FLAG_IS_PACKED				= 0x04,
};

/* The chunk data item flags definitions
 */
enum LIBEWF_CHUNK_DATA_ITEM_FLAGS
{
        /* The data is not managed by the chunk data item
	 */
	LIBEWF_CHUNK_DATA_ITEM_FLAG_NON_MANAGED_DATA		= 0x00,

        /* The data is managed by the chunk data item
	 */
	LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA		= 0x01
};

/* The (single) file entry types
 */
enum LIBEWF_FILE_ENTRY_TYPES
{
	LIBEWF_FILE_ENTRY_TYPE_DIRECTORY			= (uint8_t) 'd',
	LIBEWF_FILE_ENTRY_TYPE_FILE				= (uint8_t) 'f'
};

#define LIBEWF_FILE_ENTRY_TYPE_FOLDER				LIBEWF_FILE_ENTRY_TYPE_DIRECTORY

/* The (single) file entry flags
 */
enum LIBEWF_FILE_ENTRY_FLAGS
{
	LIBEWF_FILE_ENTRY_FLAG_ARCHIVE				= 0x00000008UL,

	LIBEWF_FILE_ENTRY_FLAG_SPARSE_DATA			= 0x04000000UL,
};

/* The (single) file entry name separator
 */
#define LIBEWF_SEPARATOR					'\\'

#endif /* !defined( _LIBEWF_DEFINITIONS_H ) */

