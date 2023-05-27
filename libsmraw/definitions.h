/*
 * Definitions for libsmraw
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

#if !defined( _LIBSMRAW_DEFINIONS_H )
#define _LIBSMRAW_DEFINIONS_H

#include "types.h"

#define LIBSMRAW_VERSION			20230320

/* The version string
 */
#define LIBSMRAW_VERSION_STRING			"20230320"

/* The access flags definitions
 * bit 1					set to 1 for read access
 * bit 2					set to 1 for write access
 * bit 3        set to 1 to truncate an existing file on write
 * bit 4-8					not used
 */
enum LIBSMRAW_ACCESS_FLAGS
{
	LIBSMRAW_ACCESS_FLAG_READ		= 0x01,
	LIBSMRAW_ACCESS_FLAG_WRITE		= 0x02,
	LIBSMRAW_ACCESS_FLAG_TRUNCATE		= 0x04
};

/* The file access macros
 */
#define LIBSMRAW_OPEN_READ			( LIBSMRAW_ACCESS_FLAG_READ )
#define LIBSMRAW_OPEN_READ_WRITE		( LIBSMRAW_ACCESS_FLAG_READ | LIBSMRAW_ACCESS_FLAG_WRITE )
#define LIBSMRAW_OPEN_WRITE			( LIBSMRAW_ACCESS_FLAG_WRITE )
#define LIBSMRAW_OPEN_WRITE_TRUNCATE		( LIBSMRAW_ACCESS_FLAG_WRITE | LIBSMRAW_ACCESS_FLAG_TRUNCATE )
#define LIBSMRAW_OPEN_READ_WRITE_TRUNCATE	( LIBSMRAW_ACCESS_FLAG_READ | LIBSMRAW_ACCESS_FLAG_WRITE | LIBSMRAW_ACCESS_FLAG_TRUNCATE )

/* The default maximum segment size
 */
#define LIBSMRAW_DEFAULT_MAXIMUM_SEGMENT_SIZE	( 1500 * 1024 * 1024 )

/* The media type definitions
 */
enum LIBSMRAW_MEDIA_TYPES
{
	LIBSMRAW_MEDIA_TYPE_UNKNOWN		= 0,
	LIBSMRAW_MEDIA_TYPE_FIXED,
	LIBSMRAW_MEDIA_TYPE_MEMORY,
	LIBSMRAW_MEDIA_TYPE_OPTICAL,
	LIBSMRAW_MEDIA_TYPE_REMOVABLE
};

/* The media flags definitions
 */
enum LIBSMRAW_MEDIA_FLAGS
{
	LIBSMRAW_MEDIA_FLAG_PHYSICAL		= 0x01
};

#endif /* !defined( _LIBSMRAW_DEFINITIONS_H ) */

