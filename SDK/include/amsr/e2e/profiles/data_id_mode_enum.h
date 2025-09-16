/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Declaration of the DataIdModeEnum.
 *
 *      \details  Contains the declarations for the DataIdModeEnum. Used in profile 1.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_DATA_ID_MODE_ENUM_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_DATA_ID_MODE_ENUM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Enumeration representing the inclusion mode that is used to include the implicit two-byte Data ID in the
 * one-byte CRC
 */
enum class DataIdModeEnum : std::uint8_t {
  /*!
   * \brief Both two bytes (double ID configuration) are included in the CRC, first low byte and then high byte.
   */
  kBoth = 0u,
  /*!
   * \brief Depending on parity of the counter (alternating ID configuration) the high and the low byte is included
   */
  kAlt = 1u,
  /*!
   * \brief Only the low byte is included and high byte is never used.
   * \details This equals to the situation if the Data IDs are only 8 bits.
   */
  kLow = 2u,
  /*!
   * \brief The high nibble of high byte of DataID is not used. The low nibble of high byte of DataID is transmitted
   * explicitly and covered by CRC calculation. The low byte is not transmitted, but it is included in the CRC
   * computation as start value.
   */
  kNibble = 3u  // NOTE: Used as max value for assertion.
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_DATA_ID_MODE_ENUM_H_
