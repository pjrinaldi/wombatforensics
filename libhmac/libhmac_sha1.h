/*
 * SHA1 functions
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

#if !defined( _LIBHMAC_SHA1_H )
#define _LIBHMAC_SHA1_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>
#endif

#include "libhmac_extern.h"
#include "libhmac_libcerror.h"
#include "libhmac_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH )
#define LIBHMAC_HAVE_SHA1_SUPPORT

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )
#define LIBHMAC_HAVE_SHA1_SUPPORT

#endif

#if !defined( LIBHMAC_HAVE_SHA1_SUPPORT )
#define LIBHMAC_SHA1_BLOCK_SIZE		64
#endif

typedef struct libhmac_internal_sha1_context libhmac_internal_sha1_context_t;

struct libhmac_internal_sha1_context
{
#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH )
	/* The SHA1 context
	 */
	SHA_CTX sha1_context;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H ) && defined( HAVE_EVP_SHA1 )
	/* The EVP message digest context
	 */
#if defined( HAVE_EVP_MD_CTX_INIT )
	EVP_MD_CTX internal_evp_md_context;
#endif

	EVP_MD_CTX *evp_md_context;

#else
	/* The number of bytes hashed
	 */
	uint64_t hash_count;

	/* The hash values
	 */
	uint32_t hash_values[ 5 ];

	/* The block offset
	 */
	size_t block_offset;

	/* The (data) block
	 */
	uint8_t block[ 128 ];

#endif /* defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H ) && defined( SHA_DIGEST_LENGTH ) */
};

#if !defined( LIBHMAC_HAVE_SHA1_SUPPORT )

ssize_t libhmac_sha1_transform(
         libhmac_internal_sha1_context_t *internal_context,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

#endif /* !defined( LIBHMAC_HAVE_SHA1_SUPPORT ) */

LIBHMAC_EXTERN \
int libhmac_sha1_initialize(
     libhmac_sha1_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha1_free(
     libhmac_sha1_context_t **context,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha1_update(
     libhmac_sha1_context_t *context,
     const uint8_t *buffer,
     size_t size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha1_finalize(
     libhmac_sha1_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha1_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     libcerror_error_t **error );

LIBHMAC_EXTERN \
int libhmac_sha1_calculate_hmac(
     const uint8_t *key,
     size_t key_size,
     const uint8_t *buffer,
     size_t size,
     uint8_t *hmac,
     size_t hmac_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBHMAC_SHA1_H ) */

