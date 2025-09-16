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
/**     \file       mpsc_fwd.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_MPSC_FWD_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_MPSC_FWD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/internal/sync/strategy.h"

namespace amsr {
namespace log {
namespace internal {
namespace mpsc {

/*! \brief A Producer of a "Multiple Producer, Single Consumer" queue */
template <typename, sync::Strategy>
class Producer;

/*! \brief A Consumer of a "Multiple Producer, Single Consumer" queue */
template <typename, sync::Strategy>
class Consumer;

}  // namespace mpsc
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MPSC_MPSC_FWD_H_
