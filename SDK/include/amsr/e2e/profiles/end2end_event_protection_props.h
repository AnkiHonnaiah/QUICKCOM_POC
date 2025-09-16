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
 *        \brief  Declaration of the End2EndEventProtectionProps struct.
 *
 *      \details  Contains the declarations for the End2EndEventProtectionProps struct.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_END2END_EVENT_PROTECTION_PROPS_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_END2END_EVENT_PROTECTION_PROPS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/array.h"

#include "amsr/e2e/profiles/data_id_mode_enum.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Blueprint of necessary configuration parameters to define per event.
 * \vprivate Vector Internal API
 *
 */
struct End2EndEventProtectionProps {
  /*!
   * \brief This represents an unique numerical identifier for the referenced
   * event or field notifier that is included in the CRC calculation.
   * \details Used in: p_01, p_04, p_05, p_06, p_07, p_11, p_44, proprietary.
   */
  std::uint32_t const kDataId;

  /*!
   * \brief This represents 16 different values for each counter value.
   * \details Shall be unique system-wide.
   * \details Used in: p_02, p_22
   */
  amsr::core::Array<std::uint8_t, 16> const kDataIdList;

  /*!
   * \brief Offset of the E2E header in the Data[] array in bits.
   * \details Used in: p_04, p_05, p_06, p_07, p_22, p_44.
   */
  std::uint32_t const kBitOffset;

  /*!
   * \brief For variable size data: Minimum length of Data in bits.
   * \details Used in: p_04, p_06, p_07, p_44.
   */
  std::uint32_t const kMinDataLength;

  /*!
   * \brief For variable size data: Maximum length of Data in bits.
   * \details Used in: p_04, p_06, p_07, p_44.
   */
  std::uint32_t const kMaxDataLength;

  /*!
   * \brief For fixed size data: Length of Data in bits.
   * \details Used in: p_01, p_02, p_05, p_11, p_22
   */
  std::uint16_t const kDataLength;

  /*!
   * \brief This represents the offset of the counter in the Data[] array in bits.
   * \details Used in: p_01, p_11
   */
  std::uint16_t const kCounterOffset;

  /*!
   * \brief This represents the offset of the CRC in the Data[] array in bits.
   * \details Used in: p_01, p_11
   */
  std::uint16_t const kCRCOffset;

  /*!
   * \brief This represents the offset of the Data ID nibble in the Data[] array in bits.
   * \details Used in: p_01, p_11
   */
  std::uint16_t const kDataIDNibbleOffset;

  /*!
   * \brief This represents the inclusion mode that is used to include the implicit two-byte Data ID in the one-byte CRC
   * \details Used in: p_01, p_11
   */
  DataIdModeEnum const kDataIDMode;

  /*!
   * \brief The maximum allowed amount of consecutive failed counter checks.
   * \details Used in: p_01, p_02
   */
  std::uint8_t const kMaxNoNewOrRepeatedData;

  /*!
   * \brief Number of checks required for validating the consistency of the counter that shall be received with a valid
   * counter after the detection of an unexpected behavior of a received counter.
   * \details Used in: p_01, p_02
   */
  std::uint8_t const kSyncCounterInit;
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_END2END_EVENT_PROTECTION_PROPS_H_
