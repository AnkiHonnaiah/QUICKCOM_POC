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
 *        \brief  Wraps POSIX macros to open file.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OPEN_MODE_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OPEN_MODE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <sys/stat.h>
#include <bitset>
#include <cstdint>

#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/core/result.h"

namespace amsr {
namespace stream {
namespace filestream {

/*! Type definition for POSIX file status.
 * \vprivate Product Private
 */
using OpenModeType = std::uint64_t;

/*! Number of bits necessary to represent an OpenModeType.
 * \vprivate Component Private
 */
static constexpr std::uint32_t kBitFieldSize{amsr::charconv::ConvertOrAbort<std::uint32_t>(sizeof(OpenModeType) * 8)};

/*! Number of open modes
 * \vprivate Component Private
 */
static constexpr std::int32_t kOpenModeCount{9};

/*!
 * \brief POSIX file status flags.
 * \details See https://www.gnu.org/software/libc/manual/html_node/File-Status-Flags.html for
 * explanation.
 * \unit amsr::stream::filestream::OpenMode
 * \vprivate Product Private
 */
class OpenMode {
 public:
  /*! Open mode to open a file read only.
   * \vprivate Product Private
   */
  static OpenModeType const kRead{1};

  /*! Open mode to open a file to write.
   * \vprivate Product Private
   */
  static OpenModeType const kWrite{2};

  /*! Open mode to open a file to read and write.
   * \vprivate Product Private
   */
  static OpenModeType const kReadWrite{4};

  /*! Open mode to open a file to append.
   * \vprivate Product Private
   */
  static OpenModeType const kAppend{8};

  /*! Open mode to truncate a file.
   * \vprivate Product Private
   */
  static OpenModeType const kTruncate{32};

  /*! Open mode to create a file if it doesn't already exist.
   * \vprivate Product Private
   */
  static OpenModeType const kCreate{64};

  /*! Open mode to avoid opening an existing file when used together with kCreate.
   * \vprivate Product Private
   */
  static OpenModeType const kExcl{128};

  /*! Open mode to enable asynchronous input mode.
   * \vprivate Product Private
   */
  static OpenModeType const kSync{256};

  /*! Open mode to close the new file descriptor upon execution of an exec family function.
   * \vprivate Product Private
   */
  static OpenModeType const kCloExec{512};

  /*!
   * \brief Constructs instance of class.
   * \param[in] mode An OpenModeType
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit OpenMode(OpenModeType mode) noexcept;

  /*!
   * \brief Copy constructs instance of class.
   * \param[in] om Instance of OpenMode
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OpenMode(OpenMode const& om) noexcept = default;  // VCA_IOSTREAM_NO_OVERLAP

  /*!
   * \brief Copy assigns instance of class.
   * \param[in] om Instance of OpenMode
   * \return Reference to self
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OpenMode& operator=(OpenMode const& om) & = default;  // VCA_IOSTREAM_NO_OVERLAP

  /*!
   * \brief Move constructs instance of class.
   * \param[in] om Instance of OpenMode
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OpenMode(OpenMode&& om) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] om Instance of OpenMode
   * \return Moved file handle
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OpenMode& operator=(OpenMode&& om) & noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual ~OpenMode() noexcept = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Checks if OpenModeType is part of OpenMode.
   * \param[in] type The OpenModeType to check.
   * \return \c true if OpenModeType is part of OpenMode, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasOpenModeType(OpenModeType type) const noexcept;

  /*!
   * \brief Checks if file can be written to.
   * \return \c true if file is writable, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasWritableMode() const noexcept;

  /*!
   * \brief Checks if file is not write-only.
   * \return \c true if file is not write-only, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasReadableMode() const noexcept;

  /*!
   * \brief Checks if file is appended on open.
   * \return \c true if file is appended, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasAppendMode() const noexcept;

  /*!
   * \brief Checks if file is truncated on open.
   * \return \c true if file is truncated, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasTruncMode() const noexcept;

  /*!
   * \brief Checks if file has create flag on open.
   * \return \c true if file is to be created, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasCreateMode() const noexcept;

  /*!
   * \brief Checks if file has exclusive flag on open.
   * \return \c true if file is opened exclusively, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool HasExclMode() const noexcept;

  /*!
   * \brief Checks if open mode given as argument is same.
   * \param[in] om OpenMode
   * \return \c true if open modes are equal, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsEqual(OpenMode om) const noexcept;

  /*!
   * \brief Converts OpenMode to Posix open flags.
   * \return Equivalent bitwise-inclusive OR combination of Posix open flags.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  std::uint64_t ToPosixFlags() const noexcept;

 private:
  /*!
   * \brief Adds a posix flag to combination of posix flags if condition is fulfilled.
   * \param[in] posix_flags Existing combination of Posix flags.
   * \param[in] condition Condition that has to be fulfilled for \c new_flag to be added.
   * \param[in] new_flag Posix flag to bitwise-inclusive OR to existing combination.
   * \return Updated bitwise-inclusive OR combination of Posix open flags.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  static std::uint64_t AddPosixFlagIf(std::uint64_t posix_flags, bool condition, std::int64_t new_flag) noexcept;

  /*! Bitfield representing at least one open mode, perhaps modified by one or more bitwise operators. */
  std::bitset<kBitFieldSize> bitfield_;
};

/*!
 * \brief Checks if two open modes are the same.
 * \param[in] om1 One OpenMode.
 * \param[in] om2 Another OpenMode.
 * \return \c true if open modes are equal, \c false otherwise.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::filestream::OpenMode
 * \vprivate Product Private
 * \steady UNKNOWN
 */
bool operator==(OpenMode om1, OpenMode om2) noexcept;

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OPEN_MODE_H_
