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
 * @file IAFF4Image.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief aff4:Image object description.
 *
 * This class defines the default API for aff4:Image objects.
 */

#include "aff4.h"

#ifndef SRC_IAFF4IMAGE_H_
#define SRC_IAFF4IMAGE_H_

#include "IAFF4Map.h"

namespace aff4 {

/**
 * @brief General interface for all aff4:Image objects
 */
class IAFF4Image: public IAFF4Resource {
public:

	LIBAFF4_API virtual ~IAFF4Image() {
	}

	/**
	 * Get the IAFF4Map instance of this image.
	 *
	 * @return The IAFF4Map instance of this image.
	 */
	LIBAFF4_API virtual std::shared_ptr<IAFF4Map> getMap() = 0;

};

} /* namespace aff4 */

#endif /* SRC_IAFF4IMAGE_H_ */
