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
 *        \brief  Interface class for all output streams.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_OUTPUT_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_OUTPUT_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/type_helper.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/io_format.h"
#include "amsr/iostream/types.h"
#include "vac/container/c_string_view.h"
#include "vac/language/byte.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Interface class for all output streams to abstract from the underlying type of 'stream'.
 * \unit amsr::stream::OutputStream
 * \vprivate Product Private
 */
class OutputStream : public IOFormat {
 public:
  /*!
   * \brief Constructs instance of class.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputStream() noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~OutputStream() noexcept override = default;

  // OutputStream is not copy constructible because a stream is not copy constructible.
  OutputStream(OutputStream const& stream) = delete;

  // OutputStream is not copy assignable.
  OutputStream& operator=(OutputStream const&) = delete;

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of OutputStream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputStream(OutputStream&& stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] stream Instance of OutputStream.
   * \return Moved InputOutputFileStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputStream& operator=(OutputStream&& stream) & noexcept = default;

 public:
  /*!
   * \brief Get the current position.
   * \return The current position, measured in bytes from the beginning of the file/string.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition Position in stream cannot be determined.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Tell() const noexcept = 0;

  /*!
   * \brief Changes the position of the underlying stream.
   * \param[in] offset Offset to change the position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file/string.
   * - kEnd: offset is a count of characters from the end of the file/string.
   * - kCur: offset is a count of characters from the current file/string position.
   * \return The updated position in the file/string, measured in bytes from the beginning of the file/string.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not seekable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits (can be caused by \c errno EOVERFLOW) or
   * whence argument invalid (can be caused by \c errno EINVAL), or file size exceeds file size limit caused
   * by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept = 0;

  /*!
   * \brief Write a character to the stream.
   * \param[in] data A character.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(char data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type bool.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(bool const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int8_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int8_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int8_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int16_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int16_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int16_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int32_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int32_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int32_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int64_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int64_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::int64_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint8_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint8_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint8_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint16_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint16_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint16_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint32_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint32_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint32_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint64_t.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint64_t const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(std::uint64_t const& data, Base base) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type float.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(float const& data) noexcept = 0;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type double.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(double const& data) noexcept = 0;

  /*!
   * \brief Write a basic string view to the stream.
   * \param[in] data A basic string view.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::StringView data) noexcept = 0;

  /*!
   * \brief Write a basic C string view to the stream.
   * \param[in] data A basic C string view.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(vac::container::CStringView data) noexcept = 0;

  /*!
   * \brief Write a span of characters to the stream.
   * \param[in] data The span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG, or input and
   * output buffer overlap.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept = 0;

  /*!
   * \brief Write a span of bytes to the stream.
   * \param[in] data The span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG, or input and
   * output buffer overlap.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::Span<vac::language::byte const> data) noexcept = 0;

  /*!
   * \brief Write a number of characters to the file stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] max_count The maximum number of characters to be written.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Write(
      std::uint64_t max_count, vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept = 0;

  /*!
   * \brief Write data to the stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Write(
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept = 0;

  /*!
   * \brief Writes a byte array in hexadecimal or binary base to the stream.
   * \param[in] data A span for a the byte array.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC; or string stream buffer is too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type or base is kDecimal.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data, Base base) noexcept = 0;

  /*!
   * \brief Inserts character into the underlying stream (e.g. a file or a string).
   * \param[in] data The character to be inserted.
   * \return Result of type void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<void> Put(char data) noexcept = 0;

  /*!
   * \brief Synchronizes written data.
   * \return Result with value void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS, ENOSPC or ENOMEM.
   * \error StreamErrc::kInvalidArgument File descriptor does not allow this operation caused by \c errno
   * EINVAL, or file position is invalid caused by \c errno EOVERFLOW, or file size
   * exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<void> Sync() noexcept = 0;

  /*!
   * \brief Flushes internal write buffer to file.
   * \return Result with value void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<void> Flush() noexcept = 0;

  /*!
   * \brief Get the number of remaining characters in buffer.
   * \details For file streams the buffer is flushed when the last element is written.
   * \return The number of characters left.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Span<char>::index_type GetRemainingBufferSize() const noexcept = 0;

  /*!
   * \brief Return open status of associated file.
   * \details For string streams always true is returned.
   * \return True if file is open, false otherwise. For string streams always true is returned.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual bool IsOpen() const noexcept = 0;

 protected:
  /*!
   * \brief Do not use Write(char*).
   * \details Without this overload, a function with a char* parameter will call the overload Write(uint8_t). To prevent
   * this unintended behavior, this overload is defined as a protected member function to cause a compile error.
   * \return Dummy value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(char const* /*data*/) noexcept = 0;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__GoodStream_SuccessfulFunction);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__GoodStream_FailingFunction);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__BadStream_SuccessfulFunction);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__BadStream_FailingFunction);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, OperatorStar);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, OperatorArrow);
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_OUTPUT_STREAM_H_
