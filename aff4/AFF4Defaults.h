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
 * @file AFF4Defaults.h
 * @author Schatz Forensic, Ptd Ltd.
 * @version 1.0
 * @date 12-Sep-2017
 * @copyright Copyright Schatz Forensic, Ptd Ltd. 2017. All Rights Reserved. This project is released under the LGPL 3.0+.
 *
 * @brief AFF4 default parameters
 *
 * This file contains standard definitions for AFF4 handling.
 */

#ifndef SRC_AFF4DEFAULTS_H_
#define SRC_AFF4DEFAULTS_H_

/**
 * The base URI for AFF4 Standard v1.0.
 */
#define AFF4_BASE_URI  "http://aff4.org/Schema#"

/**
 * Typical Prefix for aff4 URN. (eg "aff4://").
 */
#define AFF4_URN_PREFIX  "aff4://"

/**
 * RDF Prefix type for JENA.
 */
#define AFF4_RDF_PREFIX  "http://www.w3.org/1999/02/22-rdf-syntax-ns#"

/**
 * XSD Data type prefix
 */
#define AFF4_XSD_PREFIX  "http://www.w3.org/2001/XMLSchema#"

/**
 * Black Bag Technologies base URI for custom properties
 */
#define BBT_BASE_URI     "https://blackbagtech.com/aff4/Schema#"
/**
 * The default chunk size
 */
#define AFF4_DEFAULT_CHUNK_SIZE  32768
/**
 * The default number of chunks for each segment/bevvy instance.
 */
#define AFF4_DEFAULT_CHUNKS_PER_SEGMENT  2048
/**
 * The default number of bytes per sector.
 */
#define AFF4_BYTES_PER_SECTOR_DEFAULT  512

/**
 * The number of entries to cache in the bevvy index cache. (each bevvy index is approx 25KiB).
 */
#define AFF4_IMAGE_STREAM_BEVVY_INDEX_CACHE_SIZE 64

/**
 * The default size of the image stream cache. (bytes)
 */
#define AFF4_IMAGE_STREAM_CHUNK_CACHE_SIZE (8 * 1024 * 1204)

/**
 * The minimum size of the image stream cache. (bytes)
 */
#define AFF4_MINIMUM_IMAGE_STREAM_CHUNK_CACHE_SIZE (1024 * 1204)

/**
 * The default filename extension for AFF4 files.
 */
#define AFF4_DEFAULT_AFF4_EXTENSION  ".aff4"
/**
 * Filename of the TURTLE file being stored.
 */
#define AFF4_INFORMATIONTURTLE  "information.turtle"
/**
 * Filename of the container description file used to hold the Volume AFF4 resource ID.
 */
#define AFF4_FILEDESCRIPTOR  "container.description"
/**
 * Version file.
 */
#define AFF4_VERSIONDESCRIPTIONFILE  "version.txt"

/**
 * Field definition for version.txt
 */
#define AFF4_VERSION_MAJOR "major="
/**
 * Field definition for version.txt
 */
#define AFF4_VERSION_MINOR "minor="
/**
 * Field definition for version.txt
 */
#define AFF4_VERSION_TOOL "tool="

#endif /* SRC_AFF4DEFAULTS_H_ */
