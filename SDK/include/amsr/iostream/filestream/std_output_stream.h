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
 *        \brief  Implements an std output stream to console.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_OUTPUT_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_OUTPUT_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/iostream/filestream/std_stream.h"

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * \brief Implements a std output stream.
 * \unit amsr::stream::filestream::StdOutputStream
 * \vprivate Product Private
 */
class StdOutputStream : public StdStream {
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
  StdOutputStream() noexcept;

  /*!
   * \brief Constructs a stream to STDOUT using an external buffer.
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty (checked in FileStream::FileStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit StdOutputStream(amsr::core::Span<char> buffer) noexcept;

  // StdOutputStream is not copy constructible because base class OutputStream is not copy constructible.
  StdOutputStream(StdOutputStream const& output_file_stream) = delete;

  // StdOutputStream is not copy assignable because base class OutputStream is not copy assignable.
  StdOutputStream& operator=(StdOutputStream const& output_file_stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] output_file_stream Instance of StdOutputStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StdOutputStream(StdOutputStream&& output_file_stream) noexcept;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] output_file_stream Instance of StdOutputStream
   * \return Moved StdOutputStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StdOutputStream& operator=(StdOutputStream&& output_file_stream) & noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~StdOutputStream() noexcept override = default;  // VCA_IOSTREAM_DESTRUCTOR
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_OUTPUT_STREAM_H_
