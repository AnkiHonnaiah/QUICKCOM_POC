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
 *        \brief  Maps clock related error codes.
 *        \unit   osabstraction::time_CommonClocks_Linux
 *        \unit   osabstraction::time_CommonClocks_PikeOS
 *        \unit   osabstraction::time_CommonClocks_QNX
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_INTERNAL_CLOCK_ERRORS_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_INTERNAL_CLOCK_ERRORS_H_

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"

namespace osabstraction {
namespace time {
namespace internal {

/*!
 * \brief Maps error number to the osabstraction error
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
::amsr::core::ErrorCode MapClockGetTimeError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace internal
}  // namespace time
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_INTERNAL_CLOCK_ERRORS_H_
