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
 * @file IAFF4Container.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief AFF4 Container object description.
 *
 *  This class defines the default API for AFF4 Container objects.
 */

#ifndef SRC_IAFF4CONTAINER_H_
#define SRC_IAFF4CONTAINER_H_

#include "aff4.h"
#include "IAFF4Image.h"
#include "IAFF4Resolver.h"
#include "IAFF4Resource.h"

namespace aff4 {

/**
 * @brief General interface for all aff4 containers.
 *
 * To open a container, see {@link aff4::openAFF4Container()}.
 */
class IAFF4Container: public IAFF4Resource, public IAFF4Resolver {
public:

	LIBAFF4_API virtual ~IAFF4Container(){}
	/**
	 * Get a vector to all AFF4 Images within this container
	 *
	 * @return An iterator to all AFF4 Images within this container.
	 */
	LIBAFF4_API virtual std::vector<std::shared_ptr<IAFF4Image>> getImages() = 0;

	/**
	 * Set an external resolver that this container can use to query and acquire AFF4 objects not in this container.
	 *
	 * @param newResolver The new resolver to set
	 */
	LIBAFF4_API virtual void setResolver(IAFF4Resolver* newResolver)  = 0;

	/**
	 * Get the external resolver for this container.
	 *
	 * @return The external resolver. (may be NULL if none set).
	 */
	LIBAFF4_API virtual IAFF4Resolver* getResolver()  = 0;

	/**
	 * Close the AFF4 Container instance.
	 */
	LIBAFF4_API virtual void close() = 0;

};

} /* namespace aff4 */

#endif /* SRC_IAFF4CONTAINER_H_ */
