/*
 * The internal type definitions
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

#if !defined( _LIBHMAC_INTERNAL_TYPES_H )
#define _LIBHMAC_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBHMAC for local use of libhmac
 * The definitions in <libhmac/types.h> are copied here
 * for local use of libhmac
 */
#if defined( HAVE_LOCAL_LIBHMAC )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libhmac_md5_context {}		libhmac_md5_context_t;
typedef struct libhmac_sha1_context {}		libhmac_sha1_context_t;
typedef struct libhmac_sha224_context {}	libhmac_sha224_context_t;
typedef struct libhmac_sha256_context {}	libhmac_sha256_context_t;
typedef struct libhmac_sha512_context {}	libhmac_sha512_context_t;

#else
typedef intptr_t libhmac_md5_context_t;
typedef intptr_t libhmac_sha1_context_t;
typedef intptr_t libhmac_sha224_context_t;
typedef intptr_t libhmac_sha256_context_t;
typedef intptr_t libhmac_sha512_context_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBHMAC ) */

#endif /* !defined( _LIBHMAC_INTERNAL_TYPES_H ) */

