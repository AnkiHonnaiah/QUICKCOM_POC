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
/*!       \file         integrity_engine.h
 *        \brief        Declaration of the IntegrityEngine class.
 *        \unit         amsr::iointegritystream::internal::stream::IntegrityEngine
 *
 *        \complexity   The complexity is increased by many inter module calls which are necessary for the correct
 *                      functionality of this unit and does not introduce any risk.
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INTEGRITY_ENGINE_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INTEGRITY_ENGINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/common_types.h"
#include "amsr/iointegritystream/internal/stream/cache_manager.h"
#include "amsr/iointegritystream/internal/utility/crc_code.h"
#include "amsr/iostream/input_output_stream.h"
#include "amsr/iostream/stream_error_domain.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream {

/*!
 * \brief Integrity engine that provides read/write with integrity check on top of a raw stream.
 */
class IntegrityEngine final {  // VCA_IOIS_VALID_PARAMETERS

 public:
  /*!
   * \brief Constructor.
   * \param[in] raw_stream Reference to a raw stream for reading and/or writing.
   * \param[in] integrity_data Reference to the integrity file data.
   * \param[in] cache_buffer_ Buffer that will be used for caching.
   * \context ANY
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  explicit IntegrityEngine(amsr::stream::InputOutputStream& raw_stream, IntegrityFileData& integrity_data,
                           amsr::core::Span<vac::language::byte> cache_buffer) noexcept;

  /*!
   * \brief Enable append mode for writing. Only relevant for writing operations.
   * \return -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto EnableAppendMode() noexcept -> void;

  /*!
   * \brief Read file into buffer with integrity verification.
   * \detail Used for pre-validation for files with safe storage. Does not use the cache manager.
   * \param[in] input_stream The file stream to read from.
   * \param[in] integrity_data The validation data.
   * \param[out] buffer The memory to read into.
   * \pre \c input_stream is opened and positioned at beginning of file, buffer is the same size as the file.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::kIoError Underlying stream did not return expected number of bytes to fill buffer.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static auto IntegrityPreValidateSafe(amsr::stream::InputStream& input_stream, IntegrityFileData& integrity_data,
                                       amsr::core::Span<char> buffer) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Read file and validate all blocks, without storing or returning any file data
   * \detail Used for pre-validation without safe storage. Does not use the cache manager. The stream position
   *         is reset to the beginning of the file before returning.
   * \param[in] input_stream The file stream to read from.
   * \param[in] integrity_data The validation data.
   * \pre \c input_stream_ is opened an positioned at beginning of file.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static auto IntegrityPreValidateNoSafe(amsr::stream::InputStream& input_stream,
                                         IntegrityFileData const& integrity_data) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Read file into buffer with integrity verification.
   * \tparam T Data element type to use.
   * \param[out] buffer The memory to read into.
   * \return Span of read data or error.
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  template <typename T>
  auto IntegrityRead(amsr::core::Span<T> buffer) noexcept -> amsr::core::Result<amsr::core::Span<T>>;

  /*!
   * \brief Reads up to \c count bytes from the stream without additional copy.
   * \details Returns a validated view on the internal buffer. This view may be smaller than the requested amount of
   * data.
   * \param[in] count Maximum number of bytes to be read.
   * \return A span of at most size \c count (empty span if EOF is reached).
   * \pre \c raw_stream_ is opened.
   * \pre The internal buffer size of \c raw_stream_ equal to the configured block length.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto IntegrityReadCount(std::uint64_t count) noexcept -> amsr::core::Result<amsr::core::Span<char const>>;

  /*!
   * \brief Write data into file with 'kAppend' awareness.
   * \tparam T Data element type to use.
   * \param[in] data The data to be written.
   * \return Length of written data.
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read/Write fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  template <typename T>
  auto IntegrityWrite(amsr::core::Span<T> data) noexcept -> amsr::core::Result<std::int64_t>;

  /*!
   * \brief Flushes all dirty pages and calls Sync() on the raw file stream
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read/Write fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto Sync() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Flushes all dirty pages and calls Flush() on the raw file stream
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read/Write fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto Flush() noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc CacheManager::GetRemainingBufferSize
   */
  auto GetRemainingBufferSize() const noexcept -> amsr::core::Span<char>::index_type;

 private:
  /*!
   * \brief Retrieves up to \c count bytes of the cached data with integrity verification.
   * \details Returns a validated view on the cache buffer. This view may be smaller than the requested amount of
   * data.
   * \param[in] count Maximum number of bytes to be read.
   * \return A span of at most size \c count (empty span if EOF is reached).
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto IntegrityReadCurrentBlock(std::uint64_t count) noexcept
      -> amsr::core::Result<amsr::core::Span<vac::language::byte>>;

  /*!
   * \brief Write data into file with integrity verification.
   * \tparam T Data element type to use.
   * \param[in] data The data to be written.
   * \return Length of written data.
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read/Write fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  template <typename T>
  auto Write(amsr::core::Span<T> data) noexcept -> amsr::core::Result<std::int64_t>;

  /*!
   * \brief Reads file data into just loaded cache page and do crc check
   * \param[in] block_id The block to read.
   * \param[in] block_info Info for the block/page just loaded.
   * \return The amount of data read, possibly smaller than page size if at EOF.
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ReadBlockIntoCache(std::size_t block_id, CacheManager::BlockInfo const& block_info) noexcept
      -> amsr::core::Result<std::size_t>;

  /*!
   * \brief Writes a dirty block to file and recalculates its CRC.
   * \param[in] block_id The block to flush.
   * \param[in] span The memory span of the page.
   * \param[in] used size Used size of the memeoty span, possibly smaller than page size.
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto FlushBlock(std::size_t block_id, amsr::core::Span<vac::language::byte> span, std::size_t used_size) noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Flushes an block just evicted from the cache
   * \param[in] block_info Info for the evictd block being flushed.
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto FlushEvictedBlock(CacheManager::BlockInfo const& block_info) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Flushes all dirty blocks in the cache
   * \pre \c raw_stream_ is opened.
   * \context ANY
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto FlushAllBlocks() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Helper function for IntegrityPreValidateNoSafe(). Reads and validates the next block of the stream.
   * \details Reads chunks of the block into supplied buffer and accumulates the crc for each chunk. When
   *          full block is read or EOF reached, compares the crc with the validataion data.
   * \param[in] input_stream The stream to read from.
   * \param[in] buffer Temprorary buffer for reading chunks, possibly smaller than block size.
   * \param[in] integrity_data The validation data.
   * \param[in] block_nr Current block number.
   * \return An error, or a bool value indicating if EOF was reached.
   * \pre block_nr is less than number of block checksums in integrity data.
   * \context ANY
   * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of protected file is invalid.
   * \error amsr::stream::StreamErrc::* Underlying stream operations Tell/Seek/Read fail.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static auto ReadAndValidateNextBlock(amsr::stream::InputStream& input_stream, amsr::core::Span<char> buffer,
                                       IntegrityFileData const& integrity_data, std::size_t block_nr) noexcept
      -> amsr::core::Result<bool>;

  /*!
   * \brief Reference to file stream to use for I/O operations.
   */
  amsr::stream::InputOutputStream& raw_stream_;
  /*!
   * \brief Reference to the integrity file data.
   */
  IntegrityFileData& integrity_data_;

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_IoIntegrityStream_A12.1.3_NSDMI
  /*!
   * \brief boolean flag represents whether write is in append mode or not, default to false unless explicit enabled.
   */
  bool append_mode_;

  /*!
   * \brief Cache manager.
   */
  CacheManager cache_manager_;

  /*!
   * \brief Last written byte position
   * \detail Used when writing in append mode. The logical file end to seek to is
   * the largest of this value and current physical file size.
   */
  uint64_t last_written_position_{0u};

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityRead__CachedBlockSeveralparts);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__StreamSeekFail__CacheHit);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__IntegrityNotOk);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCount__UnprotectedBlock);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__NoChecksum);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__StreamReadFail);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__StreamSeekFail__CacheMiss);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__EvictedFlushFail);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  template <class T>
  FRIEND_TEST(UT__IntegrityEngine_Read_T, IntegrityReadCurrentBlock__StreamTellFail);

  /*!
   * \brief           Friend test
   */
  FRIEND_TEST(UT__IntegrityProvider, ReadWhile__SeekBackError);

  /*!
   * \brief           Friend test
   */
  FRIEND_TEST(MUT__Stream__InputIntegrityStream, Find__Corrupt);
};

}  // namespace stream
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INTEGRITY_ENGINE_H_
