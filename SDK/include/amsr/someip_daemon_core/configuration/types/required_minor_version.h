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
/**        \file  required_minor_version.h
 *        \brief  Definition of a type-safe required minor version and its version driven find behavior
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_MINOR_VERSION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_MINOR_VERSION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "ara/core/abort.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief The class represents a required minor version from the client point of view
 */
class RequiredMinorVersion final {
 public:
  /*!
   * \brief Constructor sets the reference to the port.
   * \param[in] port Reference to the 'port'.
   * \pre port is not zero.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  RequiredMinorVersion(someip_protocol::internal::MinorVersion const minor_version,
                       ConfigurationTypesAndDefs::VersionDrivenFindBehavior const version_driven_find_behavior) noexcept
      : minor_version_{minor_version}, version_driven_find_behavior_{version_driven_find_behavior} {
    if ((minor_version == ConfigurationTypesAndDefs::kMinorVersionAny) &&
        (version_driven_find_behavior == ConfigurationTypesAndDefs::VersionDrivenFindBehavior::kMinimumMinorVersion)) {
      ara::core::Abort(
          "Forbidden construction of RequiredMinorVersion with kMinorVersionAny and kMinimumMinorVersion version "
          "driven find behavior");
    }
  }

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredMinorVersion() noexcept = default;

  /*!
   * \brief Default copy constructor.
   * \steady FALSE
   */
  RequiredMinorVersion(RequiredMinorVersion const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   * \steady FALSE
   */
  RequiredMinorVersion(RequiredMinorVersion&&) noexcept = default;

  RequiredMinorVersion() = delete;
  RequiredMinorVersion& operator=(RequiredMinorVersion const& other) noexcept = delete;
  RequiredMinorVersion& operator=(RequiredMinorVersion&& other) noexcept = delete;

  /*!
   * \brief Return the minor version.
   * \return The minor version.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  someip_protocol::internal::MinorVersion GetMinorVersion() const noexcept { return minor_version_; }

  /*!
   * \brief Return the version driven find behavior.
   * \return The version driven find behavior.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ConfigurationTypesAndDefs::VersionDrivenFindBehavior GetVersionDrivenFindBehavior() const noexcept {
    return version_driven_find_behavior_;
  }

 private:
  /*!
   * \brief The required minor version.
   */
  someip_protocol::internal::MinorVersion const minor_version_;

  /*!
   * \brief The version driven find behavior.
   */
  ConfigurationTypesAndDefs::VersionDrivenFindBehavior const version_driven_find_behavior_;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_MINOR_VERSION_H_
