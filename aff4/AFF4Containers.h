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
 * @file AFF4Containers.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief Factory methods for opening AFF4 containers.
 */

#ifndef SRC_AFF4CONTAINERS_H_
#define SRC_AFF4CONTAINERS_H_

#ifdef HAVE_CONFIG_H
#include "aff4config.h"
#endif
#include "aff4.h"

namespace aff4 {
namespace container {

/**
 * Determine if a AFF4 container based on the filename.
 *
 * @param filename The file
 * @return TRUE if the filename suggests a AFF4 container. (UTF-8)
 */
LIBAFF4_API bool isAFF4Container(std::string filename) noexcept;
/**
 * Get the resource ID string from the AFF4 container.
 * <p>
 * This implementation will use both the comment and contents of the 'container.description' file with the latter
 * overriding the first if both present.
 *
 * @param filename The file to open for the resource string. (UTF-8)
 * @return The found resource ID, or empty string if not found.
 */
LIBAFF4_API std::string getResourceID(const std::string& filename) noexcept;
/**
 * Open the given file as a AFF4 Container.
 * <p>
 * The container will NOT be supplied an external Resolver to assist in looking for elements outside of it's
 * own container.
 *
 * @param filename The file to open. (UTF-8)
 * @return A AFF4 container instance, or NULL if failed.
 */
LIBAFF4_API std::shared_ptr<IAFF4Container> openAFF4Container(const std::string& filename) noexcept;

/**
 * Open the given file as a AFF4 Container
 *
 * @param filename The file to open. (UTF-8)
 * @param resolver Set the container to utilise the given AFF4 object resolver to look for objects outside of it's
 *            own container.
 * @return A AFF4 container instance
 */
LIBAFF4_API std::shared_ptr<IAFF4Container> openAFF4Container(const std::string& filename, IAFF4Resolver* resolver) noexcept;

/**
 * Create a basic Lightweight Resolver.
 * @param path The base path. (UTF-8)
 * @param scanSubFolders TRUE to scan the folder and all sub folders for images.
 * @return A lightweight resolver, or NULL if unable to construct. (eg, bad path).
 */
LIBAFF4_API aff4::IAFF4Resolver* createResolver(std::string path, bool scanSubFolders = true) noexcept;

} /* namespace container */
} /* namespace aff4 */

#endif /* SRC_AFF4CONTAINERS_H_ */
