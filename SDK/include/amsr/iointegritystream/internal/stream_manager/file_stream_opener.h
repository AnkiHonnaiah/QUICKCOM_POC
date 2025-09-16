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
/*!        \file  file_stream_opener.h
 *        \brief  Declaration of the FileStreamOpener class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_OPENER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_OPENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/stream_manager/stream_resources.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream_manager {

/*!
 * \brief File stream opener.
 */
class FileStreamOpener final {
 public:
  /*!
   * \brief Alias.
   */
  using OpenMode = amsr::stream::filestream::OpenMode;

  /*!
   * \brief Alias.
   */
  using OpenModeType = amsr::stream::filestream::OpenModeType;

  /*!
   * \brief Constructor.
   * \param[in,out] integrity_data Reference to validation data.
   * \param[in] stream_token Reference to the stream token.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit FileStreamOpener(IntegrityFileData& integrity_data, IntegrityStreamToken& stream_token) noexcept;

  /*!
   * \brief Open method.
   * \param[in] filename File to be opened.
   * \param[in] flags Opening flags.
   * \return Void result on success.
   * \context ANY
   * \pre If KExcl flag is used, KCreate must also be used.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> Open(amsr::core::StringView filename, OpenModeType flags) const noexcept;

  /*!
   * \brief Overloaded open method for amsr::core::monostate.
   * \context ANY
   * \return Void result on success.
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(amsr::core::monostate const&, amsr::core::StringView,
                                      OpenModeType) const noexcept;

  /*!
   * \brief Overloaded open method for InputStringStream.
   * \param[in] input_ss Reference to the InputStringStream stored in variant.
   * \param[in] file_name File to open.
   * \return Void result on success.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(InputStringStream& input_ss, amsr::core::StringView file_name,
                                      OpenModeType) const noexcept;

  /*!
   * \brief Overloaded open method for InputIntegrityStream.
   * \param[in] file_name File to open.
   * \return Void result on success.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(stream::InputIntegrityStream const&, amsr::core::StringView file_name,
                                      OpenModeType) const noexcept;

  /*!
   * \brief Overloaded open method for OutputIntegrityStream.
   * \param[in] file_name File to open.
   * \param[in] flags Flags used to open file.
   * \return Void result on success.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(stream::OutputIntegrityStream const&, amsr::core::StringView file_name,
                                      OpenModeType flags) const noexcept;

  /*!
   * \brief Overloaded open method for InputOutputFileStream.
   * \detail Used both for OpenForRead with pre-validation, no safe storage, and OpenForReadWrite which
   *         currently always uses pre-validation. Use flags parameter to see which one.
   * \param[in] input_output_fs InputOutputFileStream to be opened.
   * \param[in] file_name File to open.
   * \param[in] flags Flags used to open file.
   * \return Void result on success.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> operator()(InputOutputFileStream& input_output_fs, amsr::core::StringView file_name,
                                      OpenModeType flags) const noexcept;

 private:
  /*! Friend test declaration. */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamOpener, PreValidateFile_SeekFailsAfterIntegrityReadCurrentBlock);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamOpener, PreValidateFile_SeekFailsOnResetFilePosition);

  /*!
   * \brief Reference to validation data.
   */
  IntegrityFileData& integrity_data_;

  /*!
   * \brief Reference to stream token.
   */
  IntegrityStreamToken& stream_token_;
};

}  // namespace stream_manager
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr
#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_OPENER_H_
