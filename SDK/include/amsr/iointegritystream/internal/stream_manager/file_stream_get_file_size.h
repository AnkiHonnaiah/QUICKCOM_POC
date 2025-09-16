/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 *                Informatik GmbH. Vector Informatik GmbH grants to you only those rights as
 *                set out in the license conditions. All other rights remain with Vector
 *                Informatik GmbH.
 * \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  file_stream_get_file_size.h
 *        \brief  Declaration of FileStreamGetFileSize
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_GET_FILE_SIZE_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_GET_FILE_SIZE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/stream_manager/stream_resources.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream_manager {

/*!
 * \brief Uses a stream token to get the file size of any stream type.
 */
class FileStreamGetFileSize final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] stream_token Reference to stream token.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit FileStreamGetFileSize(IntegrityStreamToken const& stream_token) noexcept;

  /*!
   * \brief Get the file size from the stream token.
   * \return The current file size.
   * \error amsr::stream::StreamErrc::* Underlying stream operation GetFileSize fail.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> GetFileSize() const noexcept;

  /*!
   * \brief Overloaded GetFileSize method for amsr::core::monostate.
   * \details This function should not be reached.
   * \return Returns an error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> operator()(amsr::core::monostate const&) const noexcept;

  /*!
   * \brief Overloaded GetFileSize method for InputStringStream.
   * \return The size of the stream's buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> operator()(InputStringStream const&) const noexcept;

  /*!
   * \brief Overloaded GetFileSize method for InputIntegrityStream.
   * \return The file size of the opened stream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> operator()(stream::InputIntegrityStream const&) const noexcept;

  /*!
   * \brief Overloaded GetFileSize method for OutputIntegrityStream.
   * \return The file size of the opened stream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> operator()(stream::OutputIntegrityStream const&) const noexcept;

  /*!
   * \brief Overloaded GetFileSize method for InputOutputFileStream.
   * \param[in] input_output_fs The file stream to get file size of.
   * \return The file size of the opened stream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> operator()(InputOutputFileStream const& input_output_fs) const noexcept;

 private:
  /*!
   * \brief Reference to stream token.
   */
  IntegrityStreamToken const& stream_token_;
};

}  // namespace stream_manager
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_GET_FILE_SIZE_H_
