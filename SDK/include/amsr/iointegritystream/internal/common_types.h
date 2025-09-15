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
/*!        \file  common_types.h
 *        \brief  Common types used by integrity stream
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_COMMON_TYPES_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_COMMON_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/string.h"
#include "amsr/iointegritystream/strategy_types.h"
#include "ara/core/vector.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
/*!
 * \brief Type for holding data for an integrity protected file
 */
struct IntegrityFileData {  // VCA_IOIS_VALID_PARAMETERS, VCA_IOIS_VALID_OBJECT
  /*! \brief Filename relative to validation file. */
  amsr::core::String file_name_relative{};
  /*!
   * \brief Full path to integrity protected file.
   * \details Not parsed to validation file.
   */
  amsr::core::String file_name_full{};
  /*! \brief Parsed data variable */
  ara::core::Vector<std::uint64_t> checksum{};
  /*! \brief Parsed data variable */
  std::size_t block_length{0};
  /*! \brief Parsed data variable */
  CRCAlgorithm algorithm{CRCAlgorithm::CRC32};
};

/*!
 * \brief Type for holding data about a validation file
 */
struct ValidationFileData {  // VCA_IOIS_VALID_PARAMETERS, VCA_IOIS_VALID_OBJECT
  /*! \brief Parsed data variable for format version */
  amsr::core::String format_version;
  /*! \brief Parsed data variable for file target */
  amsr::core::String target;
  /*! \brief Parsed data variable for file data */
  IntegrityFileData file_info;
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_IoIntegrityStream_M3.4.1_typeCanBeDeclaredLocally
/*!
 * \brief Maximum amount of data (in bytes) that a CRC32 value can protect.
 * \details There is no guarantee of integrity if the protected data size exceeds 4KiB.
 */
static constexpr std::uint64_t kMaxBlockLengthCrc32{4 * 1024};

/*!
 * \brief Maximum file size (in bytes) that a CRC64 value can protect.
 * \details There is no guarantee of integrity if the protected data size exceeds 4MiB.
 */
static constexpr std::uint64_t kMaxBlockLengthCrc64{4 * 1024 * 1024};

}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_COMMON_TYPES_H_
