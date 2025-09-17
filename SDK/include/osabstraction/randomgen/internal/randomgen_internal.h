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
 *        \brief    Provides services to map occured errors from system calls to errors defined in the
 *                  osabstraction error domain.
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_RANDOMGEN_INTERNAL_RANDOMGEN_INTERNAL_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_RANDOMGEN_INTERNAL_RANDOMGEN_INTERNAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cerrno>

#include "amsr/core/result.h"

#include "amsr/core/error_code.h"
#include "osabstraction/internal/errors.h"
#include "osabstraction/osab_error_domain.h"

namespace osabstraction {
namespace randomgen {
namespace internal {

/*!
 * \brief Maps error of read call of random source to the osabstraction error domain.
 *
 * \param[in] number  Error number.
 *
 * \return The osabstraction error code.
 *
 * \retval OsabErrc::kBusy  No entropy left in random source.
 * \retval OsabErrc::kSystemEnvironmentError Failed to read from random source.
 *
 * \error           -
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector component internal API
 */
auto MapReadRandomSourceError(osabstraction::internal::OsErrorNumber number) noexcept -> ::amsr::core::ErrorCode;

}  // namespace internal
}  // namespace randomgen
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_RANDOMGEN_INTERNAL_RANDOMGEN_INTERNAL_H_
