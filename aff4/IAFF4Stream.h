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
 * @file IAFF4Stream.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief aff4:stream object description.
 *
 * This class defines the default API for aff4:stream objects.
 */

#include "aff4.h"

#ifndef SRC_IAFF4STREAM_H_
#define SRC_IAFF4STREAM_H_

namespace aff4 {

	/**
	 * @brief General interface for all aff4:Stream objects
	 */
	class IAFF4Stream: public IAFF4Resource {
	public:

		virtual ~IAFF4Stream() {}
		/**
		 * The size of the stream.
		 * <p>
		 * For when backed by aff4:DiscontiguousImage, this is the size of the address space as a whole.
		 *
		 * @return The size of the image in bytes.
		 */
		LIBAFF4_API virtual uint64_t size() = 0;

		/**
		 * Close the stream instance.
		 */
		LIBAFF4_API virtual void close() = 0;

		/**
		 * Read a number of bytes from the stream starting at offset
		 * @param buf A pointer to the buffer to read to.
		 * @param count The number of bytes to read
		 * @param offset The offset from the start of the stream.
		 * @return The number of bytes read. (0 indicates nothing read, or -1 indicates error).
		 */
		LIBAFF4_API virtual int64_t read(void *buf, uint64_t count, uint64_t offset) = 0;

	};

} /* namespace aff4 */

#endif /* SRC_IAFF4STREAM_H_ */
