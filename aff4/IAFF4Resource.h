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
 * @file IAFF4Resource.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief Methods common for all AFF4 objects.
 *
 * This class defines the default API for all AFF4 objects.
 */

#include "aff4.h"

#ifndef SRC_IAFF4RESOURCE_H_
#define SRC_IAFF4RESOURCE_H_

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "AFF4Lexicon.h"
#include "RDFValue.h"

namespace aff4 {

	/**
	 * @brief General interface for all AFF4 Objects.
	 * <p>
	 * All AFF4 Objects shall implement this interface to allow for object type enumeration and discovery.
	 */
	class IAFF4Resource {
	public:

		LIBAFF4_API virtual ~IAFF4Resource() {}
		/**
		 * Get the resource URN for this AFF4 Object
		 *
		 * @return The resource URN for this AFF4 Object.
		 */
		LIBAFF4_API virtual std::string getResourceID() const = 0;

		/**
		 * Get the base core type of this AFF4 Resource. (can be used to safe cast to another object type).
		 * <p>
		 * Safe casting types:
		 * <ul>
		 * <li>aff4::IAFF4Container = aff4::Lexicon::AFF4_ZIP_TYPE</li>
		 * <li>aff4::IAFF4Image = aff4::Lexicon::AFF4_IMAGE_TYPE</li>
		 * <li>aff4::IAFF4Map = aff4::Lexicon::AFF4_MAP_TYPE</li>
		 * <li>aff4::IAFF4Stream = aff4::Lexicon::AFF4_IMAGESTREAM_TYPE</li>
		 * </ul>
		 */
		LIBAFF4_API virtual aff4::Lexicon getBaseType() = 0;
		/**
		 * Get a basic collection of properties for this object.
		 *
		 * @return A basic collection of properties for this object.
		 */
		LIBAFF4_API virtual std::map<aff4::Lexicon, std::vector<aff4::rdf::RDFValue>> getProperties() = 0;
		/**
		 * Get the collection of objects for the given property
		 *
		 * @param resource The resource to acquired
		 * @return The collection of objects for the given key.
		 */
		LIBAFF4_API virtual std::vector<aff4::rdf::RDFValue> getProperty(aff4::Lexicon resource) = 0;

	};

} /* namespace aff4 */

#endif /* SRC_IAFF4RESOURCE_H_ */
