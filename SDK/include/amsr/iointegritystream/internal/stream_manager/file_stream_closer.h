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
/*!        \file  file_stream_closer.h
 *        \brief  Declaration of the FileStreamCloser class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_CLOSER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_CLOSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/stream_manager/stream_resources.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream_manager {

/*!
 * \brief File stream closer.
 */
class FileStreamCloser final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] stream_token Reference to stream token.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit FileStreamCloser(IntegrityStreamToken& stream_token) noexcept;

  /*!
   * \brief Close method.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> Close() noexcept;

  /*!
   * \brief Overloaded close method for amsr::core::monostate.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(amsr::core::monostate const&) const noexcept;

  /*!
   * \brief Overloaded close method for InputStringStream.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(InputStringStream const&) const noexcept;

  /*!
   * \brief Overloaded close method for InputIntegrityStream.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(stream::InputIntegrityStream const&) const noexcept;

  /*!
   * \brief Overloaded close method for OutputIntegrityStream.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(stream::OutputIntegrityStream const&) const noexcept;

  /*!
   * \brief Overloaded close method for InputOutputFileStream.
   * \param[in] input_output_fs The file stream to be closed.
   * \return Void result on sucess.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(InputOutputFileStream const& input_output_fs) const noexcept;

 private:
  /*!
   * \brief Reference to stream token.
   */
  IntegrityStreamToken& stream_token_;
};

}  // namespace stream_manager
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_CLOSER_H_
