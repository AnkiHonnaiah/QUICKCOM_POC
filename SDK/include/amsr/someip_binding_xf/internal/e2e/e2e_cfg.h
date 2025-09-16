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
/**        \file  someip_binding_xf/internal/e2e/e2e_cfg.h
 *        \brief  E2E marshalling utilities
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_E2E_CFG_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_E2E_CFG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/e2e_result.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace e2e {

/*!
 * \brief Bit offset of the E2E header inside a SOME/IP packet relative to the protected/checked parts of the E2E.
 * \details The E2E header is located after the SOME/IP header, but only the trailing 8 bytes of the SOME/IP header
 * (client ID, session ID, protocol version, ...) are involved in the E2E CRC calculation. The service ID, method ID and
 * length are not involved in the E2E CRC calculation.
 * before the SOME/IP payload.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOMEIPBINDING_AutosarC++17_10_M3.4.1_global
std::size_t constexpr kE2EHeaderSomeIpBitOffset{
    ((::amsr::someip_protocol::internal::kHeaderSize - ::amsr::someip_protocol::internal::kHeaderLength) * 8)};

/*!
 * \brief Bit offset of the E2E header inside a signal-based PDU packet relative to the protected/checked
 * parts of the E2E.
 * \details The E2E header is located directly after the PDU header and before the SOME/IP payload.
 * The service ID, method ID and length are not involved in the E2E CRC calculation.
 */
std::size_t constexpr kE2EHeaderPduBitOffset{0U};

}  // namespace e2e
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_E2E_CFG_H_
