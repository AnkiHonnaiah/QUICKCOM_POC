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
/*!        \file  file_stream_syncer.h
 *        \brief  Declaration of the FileStreamSyncer class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_SYNCER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_SYNCER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/stream_manager/stream_resources.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream_manager {

/*!
 * \brief File stream syncer.
 */
class FileStreamSyncer final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] validation_file_name Validation file name.
   * \param[in] validation_file_data Reference to validation file data.
   * \param[in] is_read_only True if the stream was opened with OpenForRead
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit FileStreamSyncer(amsr::core::StringView validation_file_name, ValidationFileData& validation_file_data,
                            bool is_read_only) noexcept;

  /*!
   * \brief Overloaded sync method for amsr::core::monostate.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(amsr::core::monostate const&) const noexcept;

  /*!
   * \brief Overloaded sync method for InputStringStream.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(InputStringStream const&) const noexcept;

  /*!
   * \brief Overloaded sync method for InputIntegrityStream.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(stream::InputIntegrityStream const&) const noexcept;

  /*!
   * \brief Overloaded sync method for OutputIntegrityStream.
   * \param[in] output_integrity_stream The stream to be synced.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(stream::OutputIntegrityStream& output_integrity_stream) noexcept;

  /*!
   * \brief Overloaded sync method for InputOutputFileStream.
   * \param[in] input_output_fs The stream to be synced.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(InputOutputFileStream& input_output_fs) const noexcept;

 private:
  /*!
   * \brief Validation file name.
   */
  amsr::core::StringView validation_file_name_;

  /*!
   * \brief Reference to validation file data.
   */
  ValidationFileData& validation_file_data_;
  /*!
   * \brief True if the stream was opened with OpenForRead
   */
  bool is_read_only_;
};

}  // namespace stream_manager
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_SYNCER_H_
