/*
 * Definitions for libphdi
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

#if !defined( _LIBPHDI_DEFINITIONS_H )
#define _LIBPHDI_DEFINITIONS_H

#include <libphdi/types.h>

#define LIBPHDI_VERSION			20221025

/* The version string
 */
#define LIBPHDI_VERSION_STRING		"20221025"

/* The access flags definitions
 * bit 1        set to 1 for read access
 * bit 2        set to 1 for write access
 * bit 3-8      not used
 */
enum LIBPHDI_ACCESS_FLAGS
{
	LIBPHDI_ACCESS_FLAG_READ	= 0x01,
/* Reserved: not supported yet */
	LIBPHDI_ACCESS_FLAG_WRITE	= 0x02
};

/* The file access macros
 */
#define LIBPHDI_OPEN_READ		( LIBPHDI_ACCESS_FLAG_READ )
/* Reserved: not supported yet */
#define LIBPHDI_OPEN_WRITE		( LIBPHDI_ACCESS_FLAG_WRITE )
/* Reserved: not supported yet */
#define LIBPHDI_OPEN_READ_WRITE		( LIBPHDI_ACCESS_FLAG_READ | LIBPHDI_ACCESS_FLAG_WRITE )

/* The disk type definitions
 */
enum LIBPHDI_DISK_TYPES
{
	LIBPHDI_DISK_TYPE_UNKNOWN	= 0,
	LIBPHDI_DISK_TYPE_EXPANDING	= 1,
	LIBPHDI_DISK_TYPE_FIXED		= 2
};

/* TODO remove */
/* The extent (storage image) type definitions
 */
enum LIBPHDI_EXTENT_TYPE
{
	LIBPHDI_EXTENT_TYPE_UNKNOWN	= 0,
	LIBPHDI_EXTENT_TYPE_COMPRESSED	= 1,
	LIBPHDI_EXTENT_TYPE_PLAIN	= 2
};

/* The (storage) image type definitions
 */
enum LIBPHDI_IMAGE_TYPE
{
	LIBPHDI_IMAGE_TYPE_UNKNOWN	= 0,
	LIBPHDI_IMAGE_TYPE_COMPRESSED	= 1,
	LIBPHDI_IMAGE_TYPE_PLAIN	= 2
};

#endif /* !defined( _LIBPHDI_DEFINITIONS_H ) */

