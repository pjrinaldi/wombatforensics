/*
 * The error code definitions for libsmraw
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

#if !defined( _LIBSMRAW_ERROR_H )
#define _LIBSMRAW_ERROR_H

#include "types.h"

/* External error type definition hides internal structure
 */
typedef intptr_t libsmraw_error_t;

/* The error domains
 */
enum LIBSMRAW_ERROR_DOMAINS
{
	LIBSMRAW_ERROR_DOMAIN_ARGUMENTS			= (int) 'a',
	LIBSMRAW_ERROR_DOMAIN_CONVERSION		= (int) 'c',
	LIBSMRAW_ERROR_DOMAIN_COMPRESSION		= (int) 'C',
	LIBSMRAW_ERROR_DOMAIN_IO			= (int) 'I',
	LIBSMRAW_ERROR_DOMAIN_INPUT			= (int) 'i',
	LIBSMRAW_ERROR_DOMAIN_MEMORY			= (int) 'm',
	LIBSMRAW_ERROR_DOMAIN_OUTPUT			= (int) 'o',
	LIBSMRAW_ERROR_DOMAIN_RUNTIME			= (int) 'r'
};

/* The argument error codes
 * to signify errors regarding arguments passed to a function
 */
enum LIBSMRAW_ARGUMENT_ERROR
{
	LIBSMRAW_ARGUMENT_ERROR_GENERIC			= 0,

	/* The argument contains an invalid value
	 */
	LIBSMRAW_ARGUMENT_ERROR_INVALID_VALUE		= 1,

	/* The argument contains a value less than zero
	 */
	LIBSMRAW_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO	= 2,

	/* The argument contains a value zero or less
	 */
	LIBSMRAW_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS	= 3,

	/* The argument contains a value that exceeds the maximum
	 * for the specific type
	 */
	LIBSMRAW_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM	= 4,

	/* The argument contains a value that is too small
	 */
	LIBSMRAW_ARGUMENT_ERROR_VALUE_TOO_SMALL		= 5,

	/* The argument contains a value that is too large
	 */
	LIBSMRAW_ARGUMENT_ERROR_VALUE_TOO_LARGE		= 6,

	/* The argument contains a value that is out of bounds
	 */
	LIBSMRAW_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS	= 7,

	/* The argument contains a value that is not supported
	 */
	LIBSMRAW_ARGUMENT_ERROR_UNSUPPORTED_VALUE	= 8,

	/* The argument contains a value that conficts with another argument
	 */
	LIBSMRAW_ARGUMENT_ERROR_CONFLICTING_VALUE	= 9
};

/* The conversion error codes
 * to signify errors regarding conversions
 */
enum LIBSMRAW_CONVERSION_ERROR
{
	LIBSMRAW_CONVERSION_ERROR_GENERIC		= 0,

	/* The conversion failed on the input
	 */
	LIBSMRAW_CONVERSION_ERROR_INPUT_FAILED		= 1,

	/* The conversion failed on the output
	 */
	LIBSMRAW_CONVERSION_ERROR_OUTPUT_FAILED		= 2
};

/* The compression error codes
 * to signify errors regarding compression
 */
enum LIBSMRAW_COMPRESSION_ERROR
{
	LIBSMRAW_COMPRESSION_ERROR_GENERIC		= 0,

	/* The compression failed
	 */
	LIBSMRAW_COMPRESSION_ERROR_COMPRESS_FAILED	= 1,

	/* The decompression failed
	 */
	LIBSMRAW_COMPRESSION_ERROR_DECOMPRESS_FAILED	= 2
};

/* The input/output error codes
 * to signify errors regarding input/output
 */
enum LIBSMRAW_IO_ERROR
{
	LIBSMRAW_IO_ERROR_GENERIC			= 0,

	/* The open failed
	 */
	LIBSMRAW_IO_ERROR_OPEN_FAILED			= 1,

	/* The close failed
	 */
	LIBSMRAW_IO_ERROR_CLOSE_FAILED			= 2,

	/* The seek failed
	 */
	LIBSMRAW_IO_ERROR_SEEK_FAILED			= 3,

	/* The read failed
	 */
	LIBSMRAW_IO_ERROR_READ_FAILED			= 4,

	/* The write failed
	 */
	LIBSMRAW_IO_ERROR_WRITE_FAILED			= 5,

	/* Access denied
	 */
	LIBSMRAW_IO_ERROR_ACCESS_DENIED			= 6,

	/* The resource is invalid i.e. a missing file
	 */
	LIBSMRAW_IO_ERROR_INVALID_RESOURCE		= 7,

	/* The ioctl failed
	 */
	LIBSMRAW_IO_ERROR_IOCTL_FAILED			= 8,

	/* The unlink failed
	 */
	LIBSMRAW_IO_ERROR_UNLINK_FAILED			= 9
};

/* The input error codes
 * to signify errors regarding handing input data
 */
enum LIBSMRAW_INPUT_ERROR
{
	LIBSMRAW_INPUT_ERROR_GENERIC			= 0,

	/* The input contains invalid data
	 */
	LIBSMRAW_INPUT_ERROR_INVALID_DATA		= 1,

	/* The input contains an unsupported signature
	 */
	LIBSMRAW_INPUT_ERROR_SIGNATURE_MISMATCH		= 2,

	/* A checksum in the input did not match
	 */
	LIBSMRAW_INPUT_ERROR_CHECKSUM_MISMATCH		= 3,

	/* A value in the input did not match a previously
	 * read value or calculated value
	 */
	LIBSMRAW_INPUT_ERROR_VALUE_MISMATCH		= 4
};

/* The memory error codes
 * to signify errors regarding memory
 */
enum LIBSMRAW_MEMORY_ERROR
{
	LIBSMRAW_MEMORY_ERROR_GENERIC			= 0,

	/* There is insufficient memory available
	 */
	LIBSMRAW_MEMORY_ERROR_INSUFFICIENT		= 1,

	/* The memory failed to be copied
	 */
	LIBSMRAW_MEMORY_ERROR_COPY_FAILED		= 2,

	/* The memory failed to be set
	 */
	LIBSMRAW_MEMORY_ERROR_SET_FAILED		= 3
};

/* The output error codes
 */
enum LIBSMRAW_OUTPUT_ERROR
{
	LIBSMRAW_OUTPUT_ERROR_GENERIC			= 0,

	/* There is insuficient space to write the output
	 */
	LIBSMRAW_OUTPUT_ERROR_INSUFFICIENT_SPACE	= 1
};

/* The runtime error codes
 * to signify errors regarding runtime processing
 */
enum LIBSMRAW_RUNTIME_ERROR
{
	LIBSMRAW_RUNTIME_ERROR_GENERIC			= 0,

	/* The value is missing
	 */
	LIBSMRAW_RUNTIME_ERROR_VALUE_MISSING		= 1,

	/* The value was already set
	 */
	LIBSMRAW_RUNTIME_ERROR_VALUE_ALREADY_SET	= 2,

	/* The creation and/or initialization of an internal structure failed
	 */
	LIBSMRAW_RUNTIME_ERROR_INITIALIZE_FAILED	= 3,

	/* The resize of an internal structure failed
	 */
	LIBSMRAW_RUNTIME_ERROR_RESIZE_FAILED		= 4,

	/* The free and/or finalization of an internal structure failed
	 */
	LIBSMRAW_RUNTIME_ERROR_FINALIZE_FAILED		= 5,

	/* The value could not be determined
	 */
	LIBSMRAW_RUNTIME_ERROR_GET_FAILED		= 6,

	/* The value could not be set
	 */
	LIBSMRAW_RUNTIME_ERROR_SET_FAILED		= 7,

	/* The value could not be appended/prepended
	 */
	LIBSMRAW_RUNTIME_ERROR_APPEND_FAILED		= 8,

	/* The value could not be copied
	 */
	LIBSMRAW_RUNTIME_ERROR_COPY_FAILED		= 9,

	/* The value could not be removed
	 */
	LIBSMRAW_RUNTIME_ERROR_REMOVE_FAILED		= 10,

	/* The value could not be printed
	 */
	LIBSMRAW_RUNTIME_ERROR_PRINT_FAILED		= 11,

	/* The value was out of bounds
	 */
	LIBSMRAW_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS	= 12,

	/* The value exceeds the maximum for its specific type
	 */
	LIBSMRAW_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM	= 13,

	/* The value is unsupported
	 */
	LIBSMRAW_RUNTIME_ERROR_UNSUPPORTED_VALUE	= 14,

	/* An abort was requested
	 */
	LIBSMRAW_RUNTIME_ERROR_ABORT_REQUESTED		= 15
};

#endif /* !defined( _LIBSMRAW_ERROR_H ) */

