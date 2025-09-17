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
/*!     \file         integrity_file_stream.h
 *      \brief        IntegrityFileStream class manages read and write access to a file. Only for product internal use.
 *      \unit         amsr::iointegritystream::IntegrityFileStream
 *
 *      \complexity   The complexity is increased by many inter module calls which are necessary for the correct
 *                      functionality of this unit and does not introduce any risk.
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTEGRITY_FILE_STREAM_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTEGRITY_FILE_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/core/vector.h"
#include "amsr/iointegritystream/internal/common_types.h"
#include "amsr/iointegritystream/internal/stream_manager/file_stream_manager.h"
#include "amsr/iointegritystream/strategy_types.h"
#include "amsr/iostream/filestream/file_permission.h"
#include "amsr/iostream/input_output_stream.h"
#include "amsr/iostream/input_stream.h"
#include "amsr/iostream/output_stream.h"
#include "ara/core/vector.h"

namespace amsr {
namespace iointegritystream {

/*!
 * \brief Manages read and write access to files through a validation file.
 * \vprivate Vector product internal API.
 */
class IntegrityFileStream final {
 public:
  /*!
   * \brief Create an IntegrityFileStream object.
   * \param[in] validation_file_name Path to the validation file.
   * \param[in] target Unique identifier used to open validation file.
   * \param[in] strategy Validation strategy behaviour.
   * \return An IntegrityFileStream object.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of validation file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
   * \error amsr::json::JsonErrc::kUserValidationFailed Unknown/unexpected value has been encountered.
   * \error amsr::json::JsonErrc::kInvalidJson Parsing has failed due to invalid JSON data.
   * \error amsr::json::JsonErrc::StreamFailure If validation file not including checksum is larger than 4MiB.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Reader, DSGN-IoIntegrity-Writer, DSGN-IoIntegrity-Reader-Writer,
   * CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-1
   */
  static amsr::core::Result<IntegrityFileStream> Create(amsr::core::StringView validation_file_name,
                                                        amsr::core::StringView target,
                                                        IntegrityValidationStrategy strategy) noexcept;

  /*!
   * \brief Create a new validation file with an integrity protected file.
   * \details If the integrity protected file already exists, calculate and write checksums to validation file.
   * \param[in] validation_file_name The absolute/relative path name of the validation file to create.
   * \param[in] unique_id Unique identifier used to open validation file.
   * \param[in] file_name Integrity protected filename relative to the validation file.
   * \param[in] block_length Length of data to calculate each data block checksum
   * \param[in] algorithm The algorithm to use during crc calculations.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidArgument Either \c validation_file_name or \c file_name is empty, or \c
   * block_length is 0.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read/Write fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Create-Validation-File, CREQ-IoIntegrityStream-F-ValidationOptions-Crc32AndCrc64,
   * CREQ-IoIntegrityStream-F-ValidationOptions-Crc32AndCrc64MaxBlockLengthOverride
   */
  static amsr::core::Result<void> CreateValidationFile(amsr::core::StringView validation_file_name,
                                                       amsr::core::StringView unique_id,
                                                       amsr::core::StringView file_name, std::size_t block_length,
                                                       CRCAlgorithm algorithm) noexcept;

  /*!
   * \brief Create a new validation file with an integrity protected file with specified permission.
   * \details If the integrity protected file already exists, calculate and write checksums to validation file.
   * \param[in] validation_file_name The absolute/relative path name of the validation file to create.
   * \param[in] unique_id Unique identifier used to open validation file.
   * \param[in] file_name The relative (to validation file) path name for the new integrity protected file.
   * \param[in] block_length Length of data to calculate each data block checksum
   * \param[in] algorithm The algorithm to use during crc calculations.
   * \param[in] permission The permission mode when we create validation file.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidArgument Either \c validation_file_name or \c file_name is empty, or \c
   * block_length is 0.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read/Write fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Create-Validation-File
   */
  static amsr::core::Result<void> CreateValidationFile(amsr::core::StringView validation_file_name,
                                                       amsr::core::StringView unique_id,
                                                       amsr::core::StringView file_name, std::size_t block_length,
                                                       CRCAlgorithm algorithm,
                                                       amsr::stream::filestream::FilePermission permission) noexcept;

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
   * \vprivate Vector product internal API.
   *
   *
   * \spec
   * requires true;
   * \endspec
   */
  amsr::core::Result<std::uint64_t> GetFileSize(amsr::core::StringView file_name) const noexcept;

  /*!
   * \brief Get CRC value stored in the associated validation file of the given protected file.
   * \param[in] file_name The name of the protected file.
   * \param[in] validation_file The name of the associated validation file.
   * \return CRC values for the file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the given \c validation_file.
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of validation file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
   * \error amsr::json::JsonErrc::StreamFailure If validation file not including checksum is larger than 4MiB.
   * \error amsr::json::JsonErrc::kUserValidationFailed Unknown/unexpected value has been encountered.
   * \error amsr::json::JsonErrc::kInvalidJson Parsing has failed due to invalid JSON data.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  static amsr::core::Result<ara::core::Vector<std::uint64_t>> GetCrc(amsr::core::StringView file_name,
                                                                     amsr::core::StringView validation_file) noexcept;

  /*!
   * \brief Default move constructor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  IntegrityFileStream(IntegrityFileStream&&) noexcept = default;  // VCA_IOIS_UNDEFINED_FUNCTION

  IntegrityFileStream& operator=(IntegrityFileStream&& integrity_file_stream) & = delete;
  IntegrityFileStream(IntegrityFileStream const& integrity_file_stream) = delete;
  IntegrityFileStream& operator=(IntegrityFileStream const& integrity_file_stream) & = delete;

  /*!
   * \brief Open a file for reading.
   * \param[in] file_name File to open.
   * \return An InputFileStream object that can be used to read the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Reader
   * \trace DSGN-IoIntegrity-Pre-Validation-File
   * \trace DSGN-IoIntegrity-Post-Validation-File
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   */
  amsr::core::Result<amsr::stream::InputStream&> OpenForRead(amsr::core::StringView file_name) noexcept;

  /*!
   * \brief Open a file for reading.
   * \param[in] file_name File to open.
   * \param[in] cache_buffer Buffer to use for caching.
   * \return An InputFileStream object that can be used to read the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::kInefficientResources The cache size is not sufficient.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Reader
   * \trace DSGN-IoIntegrity-Pre-Validation-File
   * \trace DSGN-IoIntegrity-Post-Validation-File
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   * \trace CREQ-IoIntegrityStream-F-CacheManager-OptionalUserBuffer
   */
  amsr::core::Result<amsr::stream::InputStream&> OpenForRead(
      amsr::core::StringView file_name, amsr::core::Span<vac::language::byte> cache_buffer) noexcept;

  /*!
   * \brief Open a file for writing.
   * \details The returned stream calculates CRC during write operations.
   * \param[in] file_name File to open.
   * \return An OutputIntegrityFileStream object that can be used to write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Writer
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   */
  amsr::core::Result<amsr::stream::OutputStream&> OpenForWrite(amsr::core::StringView file_name) noexcept;

  /*!
   * \brief Open a file for writing.
   * \details The returned stream calculates CRC during write operations.
   * \param[in] file_name File to open.
   * \param[in] cache_buffer Buffer to use for caching.
   * \return An OutputIntegrityFileStream object that can be used to write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::kInefficientResources The cache size is not sufficient.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Writer
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   * \trace CREQ-IoIntegrityStream-F-CacheManager-OptionalUserBuffer
   */
  amsr::core::Result<amsr::stream::OutputStream&> OpenForWrite(
      amsr::core::StringView file_name, amsr::core::Span<vac::language::byte> cache_buffer) noexcept;

  /*!
   * \brief Open a file for writing.
   * \details The returned stream calculates CRC during write operations.
   * \param[in] file_name File to open.
   * \param[in] flags Combination of file status flags.
   * \return An OutputIntegrityFileStream object that can be used to write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Writer
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   */
  amsr::core::Result<amsr::stream::OutputStream&> OpenForWrite(amsr::core::StringView file_name,
                                                               amsr::stream::filestream::OpenModeType flags) noexcept;

  /*!
   * \brief Open a file for writing.
   * \details The returned stream calculates CRC during write operations.
   * \param[in] file_name File to open.
   * \param[in] flags Combination of file status flags.
   * \param[in] cache_buffer Buffer to use for caching.
   * \return An OutputIntegrityFileStream object that can be used to write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::kInefficientResources The cache size is not sufficient.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open fails.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Writer
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   * \trace CREQ-IoIntegrityStream-F-CacheManager-OptionalUserBuffer
   */
  amsr::core::Result<amsr::stream::OutputStream&> OpenForWrite(
      amsr::core::StringView file_name, amsr::stream::filestream::OpenModeType flags,
      amsr::core::Span<vac::language::byte> cache_buffer) noexcept;

  /*!
   * \brief Open a file for read/writing.
   * \param[in] file_name File to open.
   * \return An InputOutputStream object that can be used to read/write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Reader-Writer
   * \trace CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   */
  amsr::core::Result<amsr::stream::InputOutputStream&> OpenForReadWrite(amsr::core::StringView file_name) noexcept;

  /*!
   * \brief Open a file for read/writing.
   * \details kRead and kWrite are filtered to always result in kReadWrite.
   * \param[in] file_name File to open.
   * \param[in] flags Combination of file status flags.
   * \return An InputOutputStream object that can be used to read/write to the opened file.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::kInvalidAccess Stream already opened or integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kInvalidArgument \c file_name is not protected by the current integrity stream.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open/Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Reader-Writer, CREQ-IoIntegrityStream-F-Masquerading-CorrectPath-2
   */
  amsr::core::Result<amsr::stream::InputOutputStream&> OpenForReadWrite(
      amsr::core::StringView file_name, amsr::stream::filestream::OpenModeType flags) noexcept;
  /*!
   * \brief Default destructor.
   * \details Associated file will be closed when IntegrityFileStream instance is destructed.
   * \vprivate Vector product internal API
   *
   * \spec
   * requires true;
   * \endspec
   */
  ~IntegrityFileStream() noexcept = default;  // VCA_IOIS_UNDEFINED_FUNCTION

  /*!
   * \brief Close a file after reading or writing.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \trace DSGN-IoIntegrity-Reader, DSGN-IoIntegrity-Writer, DSGN-IoIntegrity-Reader-Writer
   */
  amsr::core::Result<void> Close() noexcept;

  /*!
   * \brief Sync all opened streams to disk and update the validation file.
   * \return Result of type void.
   * \error amsr::stream::StreamErrc::* Underlying stream operation Seek/Read/Write/Sync fail.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector product internal API.
   *
   * \spec
   * requires true;
   * \endspec
   */
  amsr::core::Result<void> Sync() noexcept;

 private:
  /*!
   * \brief Explicit constructor that creates an IntegrityFileStream object.
   * \param[in] validation_file_name The validation file to use.
   * \param[in] validation_file_data Validation data struct to use.
   * \param[in] strategy Strategy to use.
   * \context amsr::iointegritystream::IntegrityFileStream::Create
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit IntegrityFileStream(amsr::core::StringView validation_file_name,
                               internal::ValidationFileData validation_file_data,
                               IntegrityValidationStrategy strategy) noexcept;

  /*!
   * \brief Creates a new integrity protected file with specified permission.
   * \details If the file exists, calculate the checksums of the pre-existing content and insert into the
   * 'validation_data' parameter.
   * \param[in,out] integrity_file_data
   * \param[in] permission The permission mode when we create validation file.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \error amsr::stream::StreamErrc::* Underlying stream operation Open/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static amsr::core::Result<void> CreateIntegrityFile(internal::IntegrityFileData& integrity_file_data,
                                                      amsr::stream::filestream::FilePermission permission) noexcept;

  /*!
   * \brief Manages the different types of streams based on the user validation strategy.
   * \details Handles the open/sync/close operations for this class.
   * \vprivate Vector component internal use.
   */
  internal::stream_manager::FileStreamManager file_stream_manager_;
};

}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTEGRITY_FILE_STREAM_H_
