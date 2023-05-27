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
 * @file RDFValue.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief RDF/XSD object descriptions
 *
 * This class defines the implementation of RDF/XSD element types.
 */

#include "aff4.h"

#ifndef SRC_RDFVALUE_H_
#define SRC_RDFVALUE_H_

#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <cstring>
#include <locale>
#include <iomanip>
#include "AFF4Lexicon.h"

namespace aff4 {
	/**
	 * @brief Base RDF types
	 */
	namespace rdf {

		/**
		 * @brief XSD Types
		 */
		enum XSDType {
			/**
			 * Unknown/Not handled type.
			 */
			UNKNOWN,
			/**
			 * xsd:string
			 */
			String,
			/**
			 * xsd:integer
			 */
			Int,
			/**
			 * xsd:long
			 */
			Long,
			/**
			 * xsd:float
			 */
			Float,
			/**
			 * xsd:dateTime
			 */
			XSDDateTime,
			/**
			 * xsd:boolean
			 */
			Boolean,
			/**
			 * user defined type. (aff4:Lexicon + string).
			 * <p>
			 * Typically only found with hash values.
			 */
			Literal,
			/**
			 * Resource type.
			 */
			Resource

		};

		/**
		 * Determine the XSD data type.
		 * @param type The URI of the data type.
		 * @return The XSD Data type. (Note: AFF4 datas types return as Literal).
		 */
		LIBAFF4_API XSDType getType(const std::string& type);

		/**
		 * Determine the AFF4 data type.
		 * @param type The URI of the data type.
		 * @return The AFF4 Data type.
		 */
		LIBAFF4_API aff4::Lexicon getAFF4Type(const std::string& type);

		/**
		 * Convert the string to a timepoint.
		 * <p>
		 * Note: This function expects RFC3339 format. (Milliseconds are IGNORED).
		 * <p>
		 * FIXME: Implement msec support.
		 *
		 * @param value The string representation.
		 * @return The timepoint representation.
		 */
		LIBAFF4_API_LOCAL std::chrono::system_clock::time_point getTime(const std::string& value);

		/**
		 * @brief General interface for implemented RDF/XSD values.
		 *
		 * Note: Not all XSD datatypes have been implemented. Implemented types include:
		 * <ul>
		 * <li>string</li>
		 * <li>int</li>
		 * <li>long</li>
		 * <li>float</li>
		 * <li>boolean</li>
		 * <li>dateTime</li>
		 * <li>aff4:MD5</li>
		 * <li>aff4:SHA1</li>
		 * <li>aff4:SHA256</li>
		 * <li>aff4:SHA512</li>
		 * <li>aff4:Blake2b</li>
		 * </ul>
		 * For dateTime values, the millisecond portion of the timestamp is currently NOT supported.
		 */
		class RDFValue {

		private:
			XSDType xsdType;
			aff4::Lexicon rdfType;
			int32_t value_int;
			int64_t value_long;
			bool value_bool;
			float value_float;
			std::string value_literal;
			std::chrono::system_clock::time_point timestamp;

		public:

			/**
			 * Create a typed resource. (String value).
			 */
			explicit RDFValue(XSDType xsdType, aff4::Lexicon type, const std::string& literal) : xsdType(xsdType), rdfType(type),
			value_int(0), value_long(0), value_bool(false), value_float(0.0), value_literal(literal),
			timestamp(std::chrono::system_clock::time_point()) {}
			/**
			 * Create a resource property, type only
			 */
			explicit RDFValue(aff4::Lexicon type) : xsdType(Resource), rdfType(type),
			value_int(0), value_long(0), value_bool(false), value_float(0.0), value_literal(""),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create a literal type.
			 */
			explicit RDFValue(aff4::Lexicon type, const std::string& literal): xsdType(Literal), rdfType(type) ,
			value_int(0), value_long(0), value_bool(false), value_float(0.0), value_literal(literal),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create a string type.
			 */
			explicit RDFValue(const std::string& value): xsdType(String), rdfType(aff4::Lexicon::UNKNOWN) ,
			value_int(0), value_long(0), value_bool(false), value_float(0.0), value_literal(value),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create an Integer
			 */
			explicit RDFValue(int32_t value): xsdType(Int), rdfType(aff4::Lexicon::UNKNOWN) ,
			value_int(value), value_long(0), value_bool(false), value_float(0.0), value_literal(""),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create a long
			 */
			explicit RDFValue(int64_t value): xsdType(Long), rdfType(aff4::Lexicon::UNKNOWN) ,
			value_int(0), value_long(value), value_bool(false), value_float(0.0), value_literal(""),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create a float
			 */
			explicit RDFValue(float value): xsdType(Float), rdfType(aff4::Lexicon::UNKNOWN) ,
			value_int(0), value_long(0), value_bool(false), value_float(value), value_literal(""),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create a boolean type
			 */
			explicit RDFValue(bool value): xsdType(Boolean), rdfType(aff4::Lexicon::UNKNOWN) ,
			value_int(0), value_long(0), value_bool(value), value_float(0.0), value_literal(""),
			timestamp(std::chrono::system_clock::time_point()) {}

			/**
			 * Create a timestamp.
			 */
			explicit RDFValue(const std::chrono::system_clock::time_point& value): xsdType(XSDDateTime), rdfType(aff4::Lexicon::UNKNOWN) ,
			value_int(0), value_long(0), value_bool(false), value_float(0.0), value_literal(""),
			timestamp(value) {}

			virtual ~RDFValue() {}

			/**
			 * Copy constructor.
			 */
			RDFValue(const RDFValue& o) :
			xsdType(o.xsdType),
			rdfType(o.rdfType) ,
			value_int(o.value_int),
			value_long(o.value_long),
			value_bool(o.value_bool),
			value_float(o.value_float),
			value_literal(o.value_literal),
			timestamp(o.timestamp) {}

			/**
			 * = Operator overload.
			 */
			RDFValue& operator=(const RDFValue& o) {
				xsdType=o.xsdType;
				rdfType=o.rdfType;
				value_int=o.value_int;
				value_long=o.value_long;
				value_bool=o.value_bool;
				value_float=o.value_float;
				value_literal =o.value_literal;
				timestamp = o.timestamp;
				return *this;
			}

			/**
			 * Get the value type.
			 * @return the type.
			 */
			LIBAFF4_API inline XSDType getXSDType() const noexcept {
				return xsdType;
			}

			/**
			 * Get the type
			 * @return the value.
			 */
			LIBAFF4_API inline aff4::Lexicon getType() const noexcept {
				return rdfType;
			}
			/**
			 * Get the string value.
			 * @return the value.
			 */
			LIBAFF4_API inline std::string getValue() const noexcept {
				return value_literal;
			}
			/**
			 * Get the integer value.
			 * @return the value.
			 */
			LIBAFF4_API inline int32_t getInteger() const noexcept {
				return value_int;
			}
			/**
			 * Get the integer value.
			 * @return the value.
			 */
			LIBAFF4_API inline int64_t getLong() const noexcept {
				return value_long;
			}
			/**
			 * Get the boolean value
			 * @return the value.
			 */
			LIBAFF4_API inline bool getBoolean() const noexcept {
				return value_bool;
			}
			/**
			 * Get the XSD Datetime value.
			 * @return the value.
			 */
			LIBAFF4_API inline std::chrono::system_clock::time_point getXSDDateTime() const noexcept {
				return timestamp;
			}

			/**
			 * Get the value as a general string.
			 * <p>
			 * Note: For XSDDateTime, the returned format is RFC3339 (not including msec).
			 * @return the value represented as a string.
			 */
			LIBAFF4_API std::string toString() noexcept;

			/**
			 * Overload == operator
			 */
			LIBAFF4_API bool operator==(const RDFValue& rhs) const {
				return (this->xsdType == rhs.xsdType) &&
				(this->rdfType == rhs.rdfType) &&
				(this->timestamp == rhs.timestamp) &&
				(this->value_bool == rhs.value_bool) &&
				(this->value_float == rhs.value_float) &&
				(this->value_int == rhs.value_int) &&
				(this->value_long == rhs.value_long) &&
				(this->value_literal == rhs.value_literal);
			}

			/**
			 * Overload != operator
			 */
			LIBAFF4_API bool operator!=(const RDFValue& rhs) const {return !(*this == rhs);}

		};

	} /* namespace rdf */
} /* namespace aff4 */

#endif /* SRC_RDFVALUE_H_ */
