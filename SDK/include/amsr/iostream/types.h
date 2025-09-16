/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Defines aliases for character types and seek direction.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_TYPES_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_TYPES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <cstdint>
#include <cstdio>  // VECTOR SL AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api

#include "amsr/core/span.h"

namespace amsr {
namespace stream {

static_assert(sizeof(std::size_t) == 8, "Contact Vector. All casts to std::size_t have to be checked.");

/*!
 * \brief Specification of seek direction.
 * \vprivate Component Private
 */
enum class SeekDirection : std::int8_t {
  // VECTOR NL AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api
  kBeg = SEEK_SET, /*!< beginning of the file */
  // VECTOR NL AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api
  kCur = SEEK_CUR,                         /*!< current position in the file */
  kEnd = SEEK_END /*!< end of the file */  // VECTOR SL AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api
};

/*!
 * \brief Strong type for position in memory block
 * \vprivate Component Private
 */
struct BufferIterator {
  /*! Position in memory block. */
  amsr::core::Span<char>::iterator pos{};
};

/*!
 * \brief Strong type for position in memory block.
 * \vprivate Component Private
 */
struct ConstBufferIterator {
  /*! Position in memory block. */
  amsr::core::Span<char const>::iterator pos{};
};

/*!
 * \brief Strong type for flag if file is open.
 * \vprivate Component Private
 */
struct FsOpen {
  /*! Whether file is open. */
  bool as_bool{false};
};

/*!
 * \brief Strong type for flag if file stream is readable.
 * \vprivate Component Private
 */
struct FsReadable {
  /*! Whether file stream is readable. */
  bool as_bool{false};
};

/*!
 * \brief Strong type for flag if file stream is writable.
 * \vprivate Component Private
 */
struct FsWritable {
  /*! Whether file stream is writable. */
  bool as_bool{false};
};

/*!
 * \brief Strong type for flag if file stream is seekable.
 * \vprivate Component Private
 */
struct FsSeekable {
  /*! Whether file stream is seekable. */
  bool as_bool{false};
};

/*!
 * \brief Strong type for flag if file stream needs synchronization.
 * \vprivate Component Private
 */
struct FsNeedsSync {
  /*! Whether file stream needs synchronization. */
  bool as_bool{false};
};

/*!
 * \brief Strong type for flag if file stream was opened with OpenMode::kAppend.
 * \vprivate Component Private
 */
struct FsAppend {
  /*! Whether file stream was opened with OpenMode::kAppend. */
  bool as_bool{false};
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_TYPES_H_
