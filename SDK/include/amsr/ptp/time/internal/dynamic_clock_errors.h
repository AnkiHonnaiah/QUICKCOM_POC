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
/*!        \file
 *        \brief  Maps errors of the dynamic clock interface.
 *
 *         \unit  amsr::ptp::DynamicClock
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_TIME_INTERNAL_DYNAMIC_CLOCK_ERRORS_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_TIME_INTERNAL_DYNAMIC_CLOCK_ERRORS_H_

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/time/internal/clock_errors.h"

namespace amsr {
namespace ptp {
namespace time {
namespace internal {

/*!
 * \brief Maps dynamic clock open error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapDynamicClockOpenError(osabstraction::internal::OsErrorNumber number) noexcept;

/*!
 * \brief Maps dynamic clock get time error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 */
::amsr::core::ErrorCode MapDynamicClockGetTimeError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace internal
}  // namespace time
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_TIME_INTERNAL_DYNAMIC_CLOCK_ERRORS_H_
