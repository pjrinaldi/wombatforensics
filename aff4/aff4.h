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
 * @file aff4.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief Default Header for AFF4 CPP
 */

#ifndef AFF4_H_
#define AFF4_H_

#include <string>
#include <map>
#include <vector>
#include <memory>

#ifdef _WIN32
#include "stdafx.h"
#endif

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

#include "RDFValue.h"
#include "AFF4Defaults.h"
#include "AFF4Lexicon.h"
#include "IAFF4Resource.h"
#include "IAFF4Resolver.h"
#include "IAFF4Container.h"
#include "IAFF4Image.h"
#include "IAFF4Stream.h"
#include "IAFF4Map.h"
#include "AFF4Containers.h"

namespace aff4 {
/**
 * Get the library version string
 * @return The library version string
 */
LIBAFF4_API std::string getVersionString() noexcept;

/**
 * Set the file handle for writing debug output to.
 * @param fileHandle The File handle to write debug output.
 */
LIBAFF4_API void setDebugOutput(FILE* fileHandle);


/**
 * Set the file handle for writing debug output to.
 * @return The File handle to write debug output.
 */
LIBAFF4_API FILE* getDebugOutput();

namespace stream {

/**
 * Get the size of the Cache (in bytes) each Image Stream Cache will utilise. (system default is 8 MiB).
 * <p>
 * Each opened aff4:ImageStream object will maintain a small cache of read blocks to improve IO performance.
 * This value is a global setting, and changes will only apply to new streams as they are opened.
 * @return The size of the cache each materialised Image Stream will consume.
 */
LIBAFF4_API uint64_t getImageStreamCacheSize();

/**
 * Set the size of the cache (in bytes) each image stream will utilise.
 * @param size The new size/limit for each cache. This value must be equal/greater than 1MiB, and must be a power 2 value.
 * @return The old size.
 */
LIBAFF4_API uint64_t setImageStreamCacheSize(uint64_t size);

}

} /* namespace aff4 */

#endif /* AFF4_H_ */
