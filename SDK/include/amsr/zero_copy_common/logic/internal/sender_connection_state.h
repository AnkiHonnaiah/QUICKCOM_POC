/*!********************************************************************************************************************
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
 *        \brief  Definition of enum SenderConnectionState.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SENDER_CONNECTION_STATE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SENDER_CONNECTION_STATE_H_

#include <cstdint>

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief States of the connection with a sender, as observed from client-side.
 * \vprivate Component Private
 */
enum class SenderConnectionState : std::uint8_t { kOk = 1, kCorrupted = 2 };

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SENDER_CONNECTION_STATE_H_
