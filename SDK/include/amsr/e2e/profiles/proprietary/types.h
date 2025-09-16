/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Type definitions for the proprietary protection interface.
 *********************************************************************************************************************/
#ifndef LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_TYPES_H_
#define LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/span.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace proprietary {

/*!
 * \brief Enumeration representing the results of the check of a single sample.
 * \vprivate Vector Internal API
 */
enum class CheckStatus : std::uint8_t {
  /*!
   * \brief the checks of the sample in this cycle were successful (including counter check
   */
  Ok = 0x00u,
  /*!
   * \brief sample has a repeated counter.
   */
  Repeated = 0x01u,
  /*!
   * \brief The checks of the sample in this cycle were successful, with the exception of counter jump, which changed
   * more than the allowed delta.
   */
  WrongSequence = 0x02u,
  /*!
   * \brief Error not related to counters occurred (e.g. wrong crc, wrong length, wrong Data ID).
   */
  Error = 0x03u,
  /*!
   * \brief No value has been received yet (e.g. during initialization). This is used as the initialization value for
   * the buffer.
   */
  NotAvailable = 0x04u,
  /*!
   * \brief No new data is available (assuming a sample has already been received since the initialization).
   */
  NoNewData = 0x05u,
  /*!
   * \brief No new data has been received after detection of an unexpected behavior of counter.
   */
  Sync = 0x06u,
  /*!
   * \brief No new data has been received according to communication medium, the CRC is correct, but this is the first
   * Data since the receiver's initialization or re-initialization, so the Counter cannot be verified yet
   */
  Initial = 0x07u
};

/*!
 * \brief Alias for amsr::core::Span<std::uint8_t>
 * \vprivate Vector product internal API
 */
using BufferView = ::amsr::core::Span<std::uint8_t>;

/*!
 * \brief Alias for amsr::core::Span<std::uint8_t const>
 * \vprivate Vector product internal API
 */
using ConstBufferView = ::amsr::core::Span<std::uint8_t const>;

/*!
 * \brief Structure that contains the Data ID that has the service_id and event_id encoded inside.
 * \vprivate Vector product internal API
 */
struct Frame {
  /*!
   * \brief Data ID of the End2EndEventProtectionProps struct.
   *        This is one valid index of the kProprietaryProfileCheckerConfiguration array.
   */
  std::uint32_t data_id;
};

}  // namespace proprietary
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_TYPES_H_
