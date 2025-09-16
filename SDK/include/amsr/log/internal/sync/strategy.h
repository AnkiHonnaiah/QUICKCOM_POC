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
/**     \file
 *      \brief      Contains a simple enum that switches the synchronization primitve.
 *      \details
 *
 *      \unit       LogAndTrace::Common::SyncStrategy::Strategy
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_STRATEGY_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_STRATEGY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace log {
namespace internal {
namespace sync {
/*! \brief Thread synchronization specification */
enum class Strategy : std::uint8_t {
  kMutex = 1,
  kEmpty = 2,
  kDefault = kMutex,
};
}  // namespace sync
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_SYNC_STRATEGY_H_
