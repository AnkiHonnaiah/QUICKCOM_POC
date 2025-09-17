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
/*!        \file  strategy_types.h
 *        \brief  Types used as the strategy for integrity validation.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_STRATEGY_TYPES_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_STRATEGY_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace iointegritystream {
/*!
 * \brief CRC algorithm type.
 * \vprivate Vector product internal API.
 */
enum class CRCAlgorithm : std::uint32_t {
  CRC32, /*!< \brief CRC32 algorithm */
  CRC64  /*!< \brief CRC64 algorithm */
};

/*!
 * \brief Validation type.
 * \details Decides whether to perform pre- or post-validation when performing read operations.
 * \vprivate Vector product internal API.
 */
enum class ValidationType : std::uint32_t {
  PRE, /*!< \brief Pre-validation */
  POST /*!< \brief Post validation */
};

/*!
 * \brief Validation strategy for IntegrityFileStream.
 * \details Decides the strategy for integrity validation.
 * \vprivate Vector product internal API.
 */
struct IntegrityValidationStrategy {
  /*! \brief Validation type */
  ValidationType validation_type_{ValidationType::PRE};
  /*! \brief Boolean for pre-validation strategy method i.e safe storage or not */
  bool safe_storage_{true};
  /*! \brief Boolean for block_length validation */
  bool block_length_max_validation_{true};
};

}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_STRATEGY_TYPES_H_
