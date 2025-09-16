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
 *        \brief  Maps the errors for the interface services.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_INTERFACE_SERVICES_ERRORS_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_INTERFACE_SERVICES_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief Maps error number to the osabstraction error
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval osabstraction::OsabErrc::kResource                 Not enough system resources to acquire the list of
 *                                                            addresses.
 * \retval osabstraction::OsabErrc::kInsufficientPrivileges   The process has insufficient privileges to acquire the
 *                                                            network interface names.
 * \retval osabstraction::OsabErrc::kUnexpected               Unexpected error that is mapped to no
 *                                                            category.
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \threadsafe      FALSE
 *
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::ErrorCode MapGetInterfaceAddressesError(osabstraction::internal::OsErrorNumber number) noexcept;

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_INTERFACE_SERVICES_ERRORS_H_
