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
 * @file IAFF4Resolver.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief aff4:Resolver object description.
 *
 * This class defines the default API for aff4:Resolver objects.
 * <p>
 * aff4:Resolver Object
 * <p>
 * Note: As a bare minimum a resolver must at least support resolution of volume resource IDs (that is return an
 * IAFF4Container). Support for streams (IAFF4Resource) and other artifacts such as time stamp metadata, query
 * information (RDFObjects) is optional.
 * <p>
 * It is recommended that for classes that utilise this interface to explicitly document what resources they resolve.
 */

#ifndef SRC_IAFF4RESOLVER_H_
#define SRC_IAFF4RESOLVER_H_

#include <memory>
#include <string>

#include "aff4.h"
#include "IAFF4Resource.h"

namespace aff4 {

/**
 * @brief General interface for all aff4:Resolvers.
 * <p>
 * Note: As a bare minimum a resolver must at least support resolution of volume resource IDs (that is return an
 * IAFF4Container). Support for streams (IAFF4Resource) and other artifacts such as time stamp metadata, query
 * information (RDFObjects) is optional.
 * <p>
 * It is recommended that for classes that utilise this interface to explicitly document what resources they resolve.
 */
class IAFF4Resolver {
public:

	LIBAFF4_API virtual ~IAFF4Resolver() {
	}

	/**
	 * Open the given resource.
	 * <p>
	 * This will always create a new instance of the resource.
	 *
	 * @param resource The resource to acquire
	 * @return The object if found, or NULL if not available.
	 */
	LIBAFF4_API virtual std::shared_ptr<aff4::IAFF4Resource> open(const std::string& resource) = 0;
	/**
	 * Does this resolver know of the given resource
	 *
	 * @param resource The resource to enquire about
	 * @return TRUE if the resource knows how to acquire and initialise this resource.
	 */
	LIBAFF4_API virtual bool hasResource(const std::string& resource) = 0;

};

} /* namespace aff4 */

#endif /* SRC_IAFF4RESOLVER_H_ */
