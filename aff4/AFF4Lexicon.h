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
 * @file AFF4Lexicon.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief Definition of basic set of the lexicon for AFF4
 *
 * This file defines a subset of the lexicon of AFF4 data types, and properties.
 */

#ifndef SRC_AFF4LEXICON_H_
#define SRC_AFF4LEXICON_H_

#include <string>

#include "AFF4Defaults.h"

namespace aff4 {

/**
 * @brief Available lexicon
 */
enum Lexicon {
	/**
	 * Unknown Lexicon.
	 */
	UNKNOWN = -1,
	/*
	 * BASE TYPE
	 */
	/**
	 * Base RDF resource type
	 */
	AFF4_TYPE = 1,
	/*
	 * TOOL TYPES
	 */
	/**
	 * The AFF4 tool that generated the container
	 */
	AFF4_TOOL,
	/**
	 * The Minor version of the tool that generator the container.
	 */
	AFF4_MINOR_VERSION,
	/**
	 * The Major version of the tool that generator the container.
	 */
	AFF4_MAJOR_VERSION,
	/*
	 * CONTAINER INFORMATION
	 */
	/**
	 * The Creation Time of the AFF4 container.
	 */
	AFF4_CREATION_TIME,
	/**
	 * The filename for any segment URI.
	 */
	AFF4_FILENAME,
	/**
	 * Stored property
	 */
	AFF4_STORED,
	/**
	 * Contains property
	 */
	AFF4_CONTAINS,
	/**
	 * Target property
	 */
	AFF4_TARGET,
	/**
	 * Zip Volume resource type (safe cast to IAFF4Container).
	 */
	AFF4_ZIP_TYPE,
	/**
	 * AFF4 Primary Object Types. (safe cast to IAFF4Image).
	 * The aff4:Image may contain device information, and MUST contain the size and reference to the map (via
	 * aff4:dataStream
	 */
	AFF4_IMAGE_TYPE,
	/**
	 * Disk Image Type
	 */
	AFF4_DISK_IMAGE_TYPE,
	/**
	 * DIsk Volume Image Type
	 */
	AFF4_VOLUME_IMAGE_TYPE,
	/**
	 * Physical Memory Image.
	 */
	AFF4_MEMORY_IMAGE_TYPE,
	/**
	 * The AFF4 Object has a aff4:ContigiousImage type
	 */
	AFF4_CONTIGUOUS_IMAGE_TYPE,
	/**
	 * The AFF4 Object has a aff4:DiscontigiousImage type
	 */
	AFF4_DISCONTIGUOUS_IMAGE_TYPE,
	/**
	 * AFF4 Map Stream (safe cast to IAFF4Map).
	 * The aff4:Map stream will contain the hash digests, and dataStream references to the lower streams.
	 */
	AFF4_MAP_TYPE,
	/**
	 * The AFF4 Stream to use when filling in gaps for aff4:DiscontiguousImage
	 */
	AFF4_MAP_GAP_STREAM,

	/**
	 * AFF4 Data Stream Type (safe cast to IAFF4Stream).
	 */
	AFF4_IMAGESTREAM_TYPE,
	/**
	 * The Map or ImageStream which corresponds to the bytestream of the Image
	 */
	AFF4_DATASTREAM,
	/**
	 * Child ImageStream’s that are used for block storage
	 */
	AFF4_DEPENDENT_STREAM,
	/**
	 * The size of the object (aff4:Map and aff4:ImageStream).
	 */
	AFF4_SIZE,
	/**
	 * The chunk size being utilise in aff4:ImageStream.
	 */
	AFF4_STREAM_CHUNK_SIZE,
	/**
	 * The internal version of the aff4:ImageStream.
	 */
	AFF4_STREAM_VERSION,
	/**
	 * The number of chunks per segment being utilise in aff4:ImageStream.
	 */
	AFF4_STREAM_CHUNKS_PER_SEGMENT,

	/*
	 * Compression Types
	 */

	/**
	 * The compression method being utilise in aff4:ImageStream.
	 */
	AFF4_IMAGE_COMPRESSION,
	/**
	 * Zlib.compress Compression (defunct)
	 */
	AFF4_IMAGE_COMPRESSION_ZLIB,
	/**
	 * Deflate Compression
	 */
	AFF4_IMAGE_COMPRESSION_DEFLATE,
	/**
	 * Snappy Compression
	 */
	AFF4_IMAGE_COMPRESSION_SNAPPY,
	/**
	 * Snappy Compression (alternate URI)
	 */
	AFF4_IMAGE_COMPRESSION_SNAPPY2,
	/**
	 * LZ4 Compression
	 */
	AFF4_IMAGE_COMPRESSION_LZ4,
	/**
	 * No Compression.
	 */
	AFF4_IMAGE_COMPRESSION_STORED,

	/*
	 * Image Stream types.
	 */
	/**
	 * Defined ImageStream that contains Zeros.
	 */
	AFF4_IMAGESTREAM_ZERO,
	/**
	 * Defined ImageStream contains 0xff (or -1).
	 */
	AFF4_IMAGESTREAM_FF,
	/**
	 * Defined ImageStream for Unknown Data.
	 */
	AFF4_IMAGESTREAM_UNKNOWN,
	/**
	 * Defined ImageStream for UnreadableData.
	 */
	AFF4_IMAGESTREAM_UNREADABLE,
	/**
	 * Defined ImageStream for SymbolicStream.
	 */
	AFF4_IMAGESTREAM_SYMBOLIC_PREFIX,

	/*
	 * Digest types. (Base types only).
	 */

	/**
	 * Hash property.
	 */
	AFF4_HASH,
	/**
	 * aff4:MD5
	 */
	AFF4_DIGEST_MD5,
	/**
	 * aff4:SHA1
	 */
	AFF4_DIGEST_SHA1,
	/**
	 * aff4:SHA256
	 */
	AFF4_DIGEST_SHA256,
	/**
	 * aff4:SHA512
	 */
	AFF4_DIGEST_SHA512,
	/**
	 * aff4:BLAKE2B
	 */
	AFF4_DIGEST_BLAKE2B,
	/**
	 * aff4:BLAKE2B
	 */
	AFF4_DIGEST_BLAKE2B_,

	/*
	 * Map information
	 */
	/**
	 * Hash of index segment
	 */
	AFF4_MAP_IDX_HASH,
	/**
	 * Hash of point segment
	 */
	AFF4_MAP_POINT_HASH,
	/**
	 * Hash of mapPath segment
	 */
	AFF4_MAP_PATH_HASH,
	/**
	 * Hash of full map
	 */
	AFF4_MAP_HASH,

	/**
	 * aff4:blockMapHash
	 */
	AFF4_DIGEST_BLOCK_MAP_HASH_SHA512,

	/**
	 * Acquisition details
	 */
	AFF4_ACQUISITION_COMPLETION_STATE,
	/**
	 * Acquisition type.
	 */
	AFF4_ACQUISITION_TYPE,

	/*
	 * Device Information.
	 */

	/**
	 * Optional field to define the page size for physical memory acquisitions
	 */
	AFF4_PAGESIZE,
	/**
	 * Optional field to define the page table offset for physical memory acquisitions
	 */
	AFF4_MEMORY_PAGETABLE_ENTRY_OFFSET,
	/**
	 * Optional field to define the installed memory size for physical memory acquisitions
	 */
	AFF4_MEMORY_INSTALLED_SIZE,
	/**
	 * Optional field to define the addressable memory size for physical memory acquisitions
	 */
	AFF4_MEMORY_ADDRESSIBLE_SIZE,
	/**
	 * Optional field to define the block/sector size for disk related acquisitions
	 */
	AFF4_BLOCKSIZE,
	/**
	 * Optional field to define the device name (as per Operating System) for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_NAME,
	/**
	 * Optional field to define the device firmware for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_FIRMWARE,
	/**
	 * Optional field to define the device interface type for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_INTERFACE,
	/**
	 * Optional field to define the device make for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_MAKE,
	/**
	 * Optional field to define the device model for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_MODEL,
	/**
	 * Optional field to define the device serial number for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_SERIAL,
	/**
	 * Optional field to define the sector count for disk related acquisitions
	 */
	AFF4_DISK_DEVICE_SECTOR_COUNT,

	/**
	 * Optional RDF type to define case details.
	 */
	AFF4_CASE_DETAILS,
	/**
	 * Optional RDF type to define case description.
	 */
	AFF4_CASE_DESCRIPTION,
	/**
	 * Optional RDF type to define case name.
	 */
	AFF4_CASE_NAME,
	/**
	 * Optional RDF type to define case examiner who performed the acquisition.
	 */
	AFF4_CASE_EXAMINER,

	/*
	 * BlackBag Technologies Custom Properties
	 */

	/**
	 * APFS Container type
	 */
	BBT_APFS_CONTAINER_TYPE,
	/**
	* APFS Container type (T2)
	*/
	BBT_APFS_CONTAINER_TYPE_T2,
	/**
	* APFS Container type (Fusion)
	*/
	BBT_APFS_CONTAINER_TYPE_FUSION,
	/**
	* APFS Container type (Standard disk)
	*/
	BBT_APFS_CONTAINER_TYPE_STANDARD,
	/**
	* Volume/Disk contains extents.
	*/
	BBT_CONTAINS_EXTENTS,
	/**
	* Volume/Disk contains unallocated regions (sparse).
	*/
	BBT_CONTAINS_UNALLOCATED,


};

namespace lexicon {
/**
 * Get the string for the applicable lexicon value.
 * @param lexicon The lexicon.
 * @return The full lexicon value.
 */
LIBAFF4_API std::string getLexiconString(aff4::Lexicon lexicon) noexcept;

/**
 * Get the enum for the applicable lexicon string value
 * @param lexicon The lexicon.
 * @return The enum lexicon.
 */
LIBAFF4_API aff4::Lexicon getLexicon(const std::string& lexicon) noexcept;
} /* namespace lexicon */

} /* namespace aff4 */
#endif /* SRC_AFF4LEXICON_H_ */
