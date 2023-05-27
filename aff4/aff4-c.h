/*-
 This file is part of AFF4 CPP.

 AFF4 CPP is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AFF4 CPP is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with AFF4 CPP.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file aff4-c.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief AFF4 C API.
 */

#ifndef AFF4_C_H_
#define AFF4_C_H_

#include <stdint.h>

#ifndef LIBAFF4_API
#if defined _WIN32 || defined __CYGWIN__
#ifdef LIBAFF4_EXPORTS
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
#define LIBAFF4_API __declspec(dllexport)
#endif
#define LIBAFF4_API_LOCAL
#else
#ifdef __GNUC__
#define LIBAFF4_API __attribute__ ((dllimport))
#else
#define LIBAFF4_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define LIBAFF4_API_LOCAL
#else
// GCC
#if __GNUC__ >= 4
#define LIBAFF4_API __attribute__ ((visibility ("default")))
#define LIBAFF4_API_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define LIBAFF4_API
#define LIBAFF4_API_LOCAL
#endif
#endif
#endif

/*
 * This is the C interface into libaff4.
 *
 * Note: This API is NOT MT-SAFE.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the AFF4 version
 */
LIBAFF4_API const char* AFF4_version();

/**
 * Initialise libaff4.
 */
LIBAFF4_API void AFF4_init();

/**
 * Open the given filename, and access the first aff4:Image in the container.
 * @param filename The filename to open. (UTF-8)
 * @return Object handle, or -1 on error. See errno.
 * (ENOENT = No Such File, or the file is not a valid AFF4 file).
 */
LIBAFF4_API int AFF4_open(const char* filename);

/**
 * Get the size of the AFF4 Object that was opened.
 * @return The size of the object, or -1 on error. See errno.
 * (EBADF = Bad handle).
 */
LIBAFF4_API int64_t AFF4_object_size(int handle);

/**
 * Get the block size of the AFF4 Object that was opened.
 * @return The block size of the object, or -1 on error. See errno.
 * (EBADF = Bad handle).
 * (ENODATA = Value not set).
 */
LIBAFF4_API int64_t AFF4_object_blocksize(int handle);

/**
 * Read a block from the given handle.
 * @param handle The Object handle.
 * @param offset the offset into the stream
 * @param buffer Pointer to a buffer of length.
 * @param length The length of the buffer to fill.
 * @return The number of bytes placed into the buffer. or -1 on error. See errno
 */
LIBAFF4_API int AFF4_read(int handle, uint64_t offset, void* buffer, int length);

/**
 * Close the given handle.
 * @param handle The Object handle to close.
 * @return 0 for success, or -1 on error. See errno
 */
LIBAFF4_API int AFF4_close(int handle);

#ifdef __cplusplus
}
#endif

#endif /* AFF4_C_H_ */
