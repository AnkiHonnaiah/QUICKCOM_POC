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
 *        \brief  Implements an std stream (output or error) to console.
 *      \details  The class should not be used via StdOutputStream and StdErrorStream by other components.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <memory>
#include <string>

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/iostream/filestream/output_file_based_stream.h"
#include "amsr/iostream/filestream/std_io_file_descriptor.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace filestream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Implements a std stream (StdOut or StdErr) to console.
 * \unit amsr::stream::filestream::StdStream
 * \vprivate Component Private
 */
class StdStream : public OutputFileBasedStream {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] fd File descriptor of std stream (StdOut or StdErr).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit StdStream(StdIoFileDescriptor fd) noexcept;

  /*!
   * \brief Constructs instance of class.
   * \param[in] file_stream_ptr File stream.
   * \param[in] fd File descriptor of std stream (StdOut or StdErr).
   * \context ANY
   * \pre Aborts if \c file_stream_ptr is a nullptr (handled in OutputFileBasedStream::OutputFileBasedStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  StdStream(std::unique_ptr<FileStreamInterface> file_stream_ptr, StdIoFileDescriptor fd) noexcept;

  /*!
   * \brief Constructs a stream to STDERR or STDOUT depending on file descriptor using an external buffer.
   * \param[in] fd File descriptor of std stream (StdOut or StdErr).
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty (checked in FileStream::FileStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit StdStream(StdIoFileDescriptor fd, amsr::core::Span<char> buffer) noexcept;

  // StdStream is not copy constructible because base class OutputStream is not copy constructible.
  StdStream(StdStream const& output_file_stream) = delete;

  // StdStream is not copy assignable because base class OutputStream is not copy assignable.
  StdStream& operator=(StdStream const& output_file_stream) & = delete;

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] output_file_stream Instance of StdStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  StdStream(StdStream&& output_file_stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] output_file_stream Instance of StdStream
   * \return Moved StdStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  StdStream& operator=(StdStream&& output_file_stream) & noexcept = default;

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  ~StdStream() noexcept override = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Opens a file stream to STDOUT / STDERR.
   * \return A valid result if operation succeeds, otherwise error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument File descriptor limit reached caused by \c errno EINVAL, or invalid
   * return value caused by \c errno EOVERFLOW.
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is \c errno EMFILE.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Open() const noexcept;

 private:
  /*! file descriptor */
  StdIoFileDescriptor fd_;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__StdStream, MoveConstructor);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__StdStream, MoveAssignment);
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_STREAM_H_
