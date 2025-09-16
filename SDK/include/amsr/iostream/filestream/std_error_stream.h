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
 *        \brief  Implements an std error stream to console.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_ERROR_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_ERROR_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/iostream/filestream/std_stream.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * \brief Implements a std error stream.
 * \unit amsr::stream::filestream::StdErrorStream
 * \vprivate Product Private
 */
class StdErrorStream : public StdStream {
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
  StdErrorStream() noexcept;

  /*!
   * \brief Constructs a stream to STDERR using an external buffer.
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty (checked in FileStream::FileStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit StdErrorStream(amsr::core::Span<char> buffer) noexcept;

  // StdErrorStream is not copy constructible because base class OutputStream is not copy constructible.
  StdErrorStream(StdErrorStream const& output_file_stream) = delete;

  // StdErrorStream is not copy assignable because base class OutputStream is not copy assignable.
  StdErrorStream& operator=(StdErrorStream const& output_file_stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] output_file_stream Instance of StdErrorStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StdErrorStream(StdErrorStream&& output_file_stream) noexcept;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] output_file_stream Instance of StdErrorStream
   * \return Moved StdErrorStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StdErrorStream& operator=(StdErrorStream&& output_file_stream) & noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~StdErrorStream() noexcept override = default;  // VCA_IOSTREAM_DESTRUCTOR
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_ERROR_STREAM_H_
