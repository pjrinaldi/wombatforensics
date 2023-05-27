/*
 * The internal definitions
 *
 * Copyright (C) 2011-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( LIBHMAC_INTERNAL_DEFINITIONS_H )
#define LIBHMAC_INTERNAL_DEFINITIONS_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBHMAC for local use of libhmac
 */
#if !defined( HAVE_LOCAL_LIBHMAC )
#include <libhmac/definitions.h>

/* The definitions in <libhmac/definitions.h> are copied here
 * for local use of libhmac
 */
#else
#define LIBHMAC_VERSION			20230205

/* The libhmac version string
 */
#define LIBHMAC_VERSION_STRING		"20230205"

/* The digest hash sizes
 */
#define LIBHMAC_MD5_HASH_SIZE		16
#define LIBHMAC_SHA1_HASH_SIZE		20
#define LIBHMAC_SHA224_HASH_SIZE	28
#define LIBHMAC_SHA256_HASH_SIZE	32
#define LIBHMAC_SHA512_HASH_SIZE	64

#endif /* !defined( HAVE_LOCAL_LIBHMAC ) */

#define LIBHMAC_UNFOLLED_LOOPS

#endif /* !defined( LIBHMAC_INTERNAL_DEFINITIONS_H ) */

