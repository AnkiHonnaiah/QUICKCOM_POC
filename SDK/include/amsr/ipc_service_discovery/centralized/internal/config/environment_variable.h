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
/*! \file
 *  \brief Read and write environment variables.
 *  \unit IpcServiceDiscovery::Centralized::Config
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_ENVIRONMENT_VARIABLE_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_ENVIRONMENT_VARIABLE_H_

#include "amsr/charconv/to_number.h"
#include "amsr/envvar/envvar.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief A wrapper class for reading and setting environment variables.
 * \details The class provides two methods, one for getting and one for
 *          setting a value for a given environment variable.
 * \vprivate Component internal.
 * \trace DSGN-IpcServiceDiscovery-Centralized-Config-EnvironmentVariable
 */
class EnvironmentVariable {
 public:
  /*!
   * \brief Searches the environment list for a given environment variable name and returns the corresponding value.
   * \details This function is a wrapper of ::amsr::envvar::GetEnvironmentVariable, if the variable is not found the
   *          given default value is returned.
   * \param[in] name Name of the environment variable.
   * \param[in] default_value The default value for the variable.
   * \tparam T Type of the variable and its default value.
   * \return The environment variable value if it exists or the default value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  template <class T>
  static auto Get(vac::container::CStringView name, T default_value) noexcept -> T {
    return ::amsr::envvar::GetEnvironmentVariable(name)
        .AndThen([](vac::container::CStringView value_sv) { return amsr::charconv::ToNumber<T>{value_sv}.Convert(); })
        .Resolve([&default_value](::ara::core::ErrorCode) { return default_value; });
  }

  /*!
   * \brief Sets the environment variable with the given name with the given value.
   * \details This function is a wrapper of ::amsr::envvar::SetEnvironmentVariable.
   * \param[in] name Name of the environment variable.
   * \param[in] value Environment variable value.
   * \error osabstraction::OsabErrc::kUnexpected  Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kApiError  If the input env_var_name is null, empty or contains '='.
   * \error osabstraction::OsabErrc::kResource  If envvar set fails due to insufficient memory.
   * \context ANY
   * \pre Unused function. Do not use this function unless covered by component tests!
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  // clang-format off
  static auto Set(vac::container::CStringView const& name,
                  vac::container::CStringView const& value) noexcept -> ara::core::Result<void> /* COV_IpcSD_unused */ {
    // clang-format on
    return ::amsr::envvar::SetEnvironmentVariable(name, value);
  }
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_ENVIRONMENT_VARIABLE_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_unused
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is part of the public unit interface and supposed to be helpful for future extensions of
//            the component and tested by unit tests, no component test function coverage needs to be achieved for this
//            function. Usage of the function is strictly forbidden by precondition and compliance with this
//            precondition is checked by review.
// COV_JUSTIFICATION_END
