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
 * @file IAFF4Map.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief aff4:Map object description.
 *
 * This class defines the default API for aff4:Map objects.
 */

#include "aff4.h"

#ifndef SRC_IAFF4MAP_H_
#define SRC_IAFF4MAP_H_

#include "IAFF4Stream.h"

namespace aff4 {

	/**
	 * @brief General interface for all aff4:Map objects
	 */
	class IAFF4Map: public IAFF4Resource {
	public:

		LIBAFF4_API virtual ~IAFF4Map() {
		}
		/**
		 * The size of the Image.
		 * <p>
		 * For aff4:DiscontiguousImage, this is the size of the address space as a whole.
		 *
		 * @return The size of the image in bytes.
		 */
		LIBAFF4_API virtual uint64_t size() const = 0;

		/**
		 * Get the AFF4 Map Stream instance of this image.
		 *
		 * @return The AFF4 Map Stream instance of this image.
		 */
		LIBAFF4_API virtual std::shared_ptr<IAFF4Stream> getStream() = 0;

		/**
		 * Get the IAFF4Stream instance that will be used to override the default Unknown Stream.
		 *
		 * @return The stream used to override the Unknown Stream or nullptr if none set.
		 */
		LIBAFF4_API virtual std::shared_ptr<IAFF4Stream> getUnknownStreamOverride() = 0;

		/**
		 * Set the IAFF4Stream instance that will be used to override the default Unknown Stream.
		 * <p>
		 * Note: This MUST be called before {@link getStream()} for this setting to come into effect.
		 *
		 * @param stream The stream used to override the Unknown Stream or nullptr to reset to system default
		 */
		LIBAFF4_API virtual void setUnknownStreamOverride(std::shared_ptr<IAFF4Stream>& stream) = 0;

		/**
		 * Get the IAFF4Stream instance that will be used to fill in gaps in Sparse images.
		 *
		 * @return The stream used to override the Unknown Stream or nullptr if none set.
		 */
		LIBAFF4_API virtual std::shared_ptr<IAFF4Stream> getMapGapStreamOverride() = 0;

		/**
		 * Set the IAFF4Stream instance that will be used to fill in gaps in Sparse images
		 * <p>
		 * Note: This MUST be called before {@link getStream()} for this setting to come into effect.
		 *
		 * @param stream The stream used to fill in gaps in Sparse images or nullptr to reset to system default
		 */
		LIBAFF4_API virtual void setMapGapStreamOverride(std::shared_ptr<IAFF4Stream>& stream) = 0;

	};

} /* namespace aff4 */

#endif /* SRC_IAFF4MAP_H_ */
