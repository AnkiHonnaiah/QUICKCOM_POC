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
/*!        \file  envvar.h
 *        \brief  Get environment variables.
 *        \unit   osabstraction::Envvar
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENVVAR_ENVVAR_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENVVAR_ENVVAR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace envvar {

/*!
 * \brief           Searches the environment list to find the environment variable name and returns the corresponding
 *                  value.
 * \details         This function is a wrapper of POSIX getenv(), therefore the same constraints apply:
 *                  The implementation of getenv() is not required to be reentrant. The string pointed to
 *                  by the return value of getenv() may be statically allocated, and can be modified by a
 *                  subsequent call to getenv(), putenv(), setenv(), or unsetenv().
 *                  If you intend to store the returned value, use a string instead of the StringView as the
 *                  underlying pointer may get invalidated.
 *
 * \param[in]       env_name        Name of the environment variable.
 *
 * \return          String of the environment value.
 * \error           osabstraction::OsabErrc::kDoesNotExist   Environment variable does not exist.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-GetEnvironmentVariable
 *
 * \vprivate        Product private
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::Result<vac::container::CStringView> GetEnvironmentVariable(vac::container::CStringView env_name);

/*!
 * \brief           Sets the environment variable with the given name with the given value.
 * \details         This function is a wrapper of POSIX setenv().
 *
 * \param[in]       env_var_name    Name of the environment variable.
 * \param[in]       env_var_value   Environment variable value.
 *
 * \return          void
 * \error           osabstraction::OsabErrc::kUnexpected  Unexpected error that is mapped to no category.
 * \error           osabstraction::OsabErrc::kApiError  If the input env_var_name is null, empty or contains '='.
 * \error           osabstraction::OsabErrc::kResource  If envvar set fails due to insufficient memory.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-SetEnvironmentVariable
 *
 * \vprivate        Product private
 * \spec
 *   requires true;
 * \endspec
 */
::amsr::core::Result<void> SetEnvironmentVariable(vac::container::CStringView const& env_var_name,
                                                  vac::container::CStringView const& env_var_value);

}  // namespace envvar
}  // namespace amsr

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENVVAR_ENVVAR_H_
