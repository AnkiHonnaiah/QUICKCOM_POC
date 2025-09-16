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
/*!       \file         helper.h
 *        \brief        Helper functions.
 *        \unit         Helper
 *
 *        \complexity  The complexity is increased by many inter module calls which are necessary for the correct
 *                     functionality of this unit and does not introduce any risk.
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_UTILITY_HELPER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_UTILITY_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/core/span.h"
#include "amsr/filesystem/path.h"
#include "amsr/iointegritystream/internal/json_validation/validation_parser.h"
#include "amsr/iointegritystream/strategy_types.h"
#include "amsr/iostream/filestream/input_file_stream.h"
#include "amsr/iostream/output_stream.h"
#include "ara/core/vector.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace utility {

/*!
 * \brief Calculate the checksum on a given data and insert into vector.
 * \details The new checksums are put on top of the already existing ones in the given vector.
 * \param[in] data The data to calculate the checksum on.
 * \param[out] crc_vector Vector of checksums to insert calculated ones.
 * \param[in] block_length Length of data to calculate each data block checksum
 * \param[in] alg The algorithm to use during crc calculations.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 */
void CalculateCrcIntoVector(amsr::core::Span<char const> data, ara::core::Vector<std::uint64_t>& crc_vector,
                            std::size_t block_length, CRCAlgorithm alg) noexcept;

/*!
 * \brief Reads data from file into buffer.
 * \param[in] filename File to read.
 * \param[out] read_buffer Buffer to use for reading.
 * \return A span pointing to the read data.
 * \context ANY
 * \pre -
 * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
 * \threadsafe FALSE
 * \reentrant FALSE
 */
amsr::core::Result<amsr::core::Span<char>> ReadFileIntoBuffer(amsr::core::StringView filename,
                                                              ara::core::Vector<char>& read_buffer) noexcept;

/*!
 * \brief Reads and verifies the integrity of a validation file.
 * \param[in] validation_file Path to validation file.
 * \param[out] read_buffer Buffer to use for reading.
 * \return A span pointing to the read data.
 * \context ANY
 * \pre -
 * \error amsr::stream::StreamErrc::kInvalidAccess Integrity of validation file is invalid.
 * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \trace DSGN-IoIntegrity-Verify-Validation-File
 */
amsr::core::Result<amsr::core::Span<char>> ReadAndVerifyValidationFile(ara::core::Vector<char>& read_buffer,
                                                                       amsr::core::StringView validation_file) noexcept;

/*!
 * \brief Calculate the checksum of an integrity protected file.
 * \param[in,out] integrity_data Integrity file data.
 * \return Result of type void.
 * \context ANY
 * \pre -
 * \error amsr::stream::StreamErrc::* Underlying stream operations Open/Seek/Read fail.
 * \threadsafe FALSE
 * \reentrant FALSE
 */
amsr::core::Result<void> CalculateIntegrityFileChecksum(IntegrityFileData& integrity_data) noexcept;

/*!
 * \brief Get relative paths between two paths.
 * \param[in] base Starting point of the relative path.
 * \param[in] path The path we want to find from base.
 * \return A result object containing the relative path or error.
 * \context ANY
 * \pre -
 * \error amsr::stream::StreamErrc::kInvalidArgument One of the entered paths are empty.
 * \threadsafe FALSE
 * \reentrant FALSE
 */
amsr::core::Result<filesystem::Path> GetRelativePath(amsr::core::StringView base, amsr::core::StringView path) noexcept;

}  // namespace utility
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_UTILITY_HELPER_H_
