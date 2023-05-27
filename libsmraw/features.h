/*
 * Features of libsmraw
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

#if !defined( _LIBSMRAW_FEATURES_H )
#define _LIBSMRAW_FEATURES_H

/* The libsmraw type support features
 */
#if defined( WINAPI ) || 0
#define LIBSMRAW_HAVE_WIDE_CHARACTER_TYPE	1
#endif

#if defined( WINAPI ) || 1
#define LIBSMRAW_HAVE_MULTI_THREAD_SUPPORT	1
#endif

#if defined( HAVE_LIBBFIO ) || ( !defined( WINAPI ) && 0 )
#define LIBSMRAW_HAVE_BFIO			1
#endif

#if !defined( LIBSMRAW_DEPRECATED )
#if defined( __GNUC__ ) && __GNUC__ >= 3
#define LIBSMRAW_DEPRECATED	__attribute__ ((__deprecated__))
#elif defined( _MSC_VER )
#define LIBSMRAW_DEPRECATED	__declspec(deprecated)
#else
#define LIBSMRAW_DEPRECATED
#endif
#endif

#endif /* !defined( _LIBSMRAW_FEATURES_H ) */

