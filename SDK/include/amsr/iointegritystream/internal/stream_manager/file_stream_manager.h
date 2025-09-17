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
/*!        \file  file_stream_manager.h
 *        \brief  Declaration of the FileStreamManager class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_MANAGER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/stream_manager/stream_resources.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream_manager {

/*!
 * \brief Integrity file stream manager.
 * \details Configure and manage streams.
 */
class FileStreamManager final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] validation_file_data Validation data containing checksums and file information.
   * \param[in] validation_file_name Path to the validation file.
   * \param[in] strategy Validation strategy behaviour.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit FileStreamManager(ValidationFileData validation_file_data, amsr::core::StringView validation_file_name,
                             IntegrityValidationStrategy strategy) noexcept;

  /*!
   * \brief Move constructor.
   * \param[in] other Other instance of FileStreamManager to move.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  FileStreamManager(FileStreamManager&& other) noexcept;

  /*!
   * \brief Destructor. Implicitly close the file stream.
   *
   * \trace DSGN-IoIntegrity-Reader, DSGN-IoIntegrity-Writer, DSGN-IoIntegrity-Reader-Writer
   */
  ~FileStreamManager() noexcept;

  // Delete move assignment.
  FileStreamManager& operator=(FileStreamManager&& other) & = delete;
  // Delete copy assignment/constructor.
  FileStreamManager(FileStreamManager const& other) = delete;
  FileStreamManager& operator=(FileStreamManager const& other) & = delete;

  /*!
   * \brief Open an InputStream object for read operations.
   * \param[in] file_name Name of file to open.
   * \param[in] cache_buffer Optional buffer to use for caching the file contents.
   * \return An InputStream object that can be used to read from the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<amsr::stream::InputStream&> OpenForRead(
      amsr::core::StringView file_name,
      amsr::core::Optional<amsr::core::Span<vac::language::byte>> cache_buffer = amsr::core::nullopt) noexcept;

  /*!
   * \brief Open an OutputIntegrityStream stream object for write operations.
   * \param[in] file_name Name of file to open.
   * \param[in] flags Combination of file status flags. kRead, kWrite bits will be unset and kReadWrite bit will be set.
   * \param[in] cache_buffer Optional buffer to use for caching the file contents.
   * \return An OutputStream object that can be used to write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<amsr::stream::OutputStream&> OpenForWrite(
      amsr::core::StringView file_name, amsr::stream::filestream::OpenModeType flags,
      amsr::core::Optional<amsr::core::Span<vac::language::byte>> cache_buffer = amsr::core::nullopt) noexcept;

  /*!
   * \brief Open an InputOutputFileStream object for read/write operations.
   * \param[in] file_name Name of file to open.
   * \param[in] flags Combination of file status flags. kRead, kWrite bits will be unset and kReadWrite bit will be set.
   * \return An InputOutputFileStream object that can be used to read/write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<amsr::stream::InputOutputStream&> OpenForReadWrite(
      amsr::core::StringView file_name, amsr::stream::filestream::OpenModeType flags) noexcept;

  /*!
   * \brief Close a file after reading or writing.
   * \details Shall update the validation file if any output or input/output streams have been opened.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::* Underlying stream operations Seek/Read/Write/Sync fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> Close() noexcept;

  /*!
   * \brief Sync all opened streams to disk and update the validation file.
   * \details No sync/update is done if only an input stream has been opened.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::* Underlying stream operations Seek/Read/Write/Sync fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> Sync() noexcept;

  /*!
   * \brief Get the file size of an opened file.
   * \param[in] file_name Name of the file.
   * \return The current size of the file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess File \c file_name is not opened.
   * \error amsr::stream::StreamErrc::kInvalidArgument File \c file_name does not exist in validation file.
   * \error amsr::stream::StreamErrc::* Underlying stream operation GetFileSize fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<std::uint64_t> GetFileSize(amsr::core::StringView file_name) const noexcept;

 private:
  /*!
   * \brief Precheck before opening a file.
   * \return Result of type void if \c file_name matches validation file and stream is in closed state.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> PreOpenCheck(amsr::core::StringView file_name) const noexcept;

  /*!
   * \brief Verify if a file name is inside the validation file.
   * \param[in] file_name The file name to check.
   * \return Result of type void if \c file_name matches validation file.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> CheckFileName(amsr::core::StringView file_name) const noexcept;

  /*!
   * \brief Validation data loaded from json format.
   */
  std::unique_ptr<ValidationFileData> validation_file_data_;

  /*!
   * \brief Name of the validation file.
   */
  amsr::core::String const validation_file_name_;

  /*!
   * \brief Variable holding info about integrity validation strategy.
   */
  IntegrityValidationStrategy const strategy_;

  /*!
   * \brief An integrity stream token.
   */
  IntegrityStreamToken token_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, OpenForRead__Success__Pre_SafeStorage);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, GetFileSizeFailsWithWrongName);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, OpenForRead__Success__Pre_NoSafeStorage);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, OpenForRead__Success__Post);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, OpenForWrite__Success);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, OpenForReadWrite__Success);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager_P, Close);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, IOFormat);
};

}  // namespace stream_manager
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_FILE_STREAM_MANAGER_H_
