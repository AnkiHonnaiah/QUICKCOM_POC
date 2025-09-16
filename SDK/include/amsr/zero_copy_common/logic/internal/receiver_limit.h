/*!********************************************************************************************************************
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
 *        \brief Definition of the ReceiverLimit enum.
 *        \unit ZeroCopyCommon::ZeroCopyLogic::BorrowedManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_LIMIT_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_LIMIT_H_

#include <cstdint>

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Enumeration of reasons why a receiver is rate-limited, i.e. why a message is dropped for it.
 * \vprivate Product Private
 */
enum class ReceiverLimit : std::uint8_t { kNotLimited, kClassLimited };

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_LIMIT_H_
