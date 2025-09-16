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
 *        \brief  Required service instance configuration class
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_REQUIRED_SERVICE_INSTANCE_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_REQUIRED_SERVICE_INSTANCE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/integrity_level.h"
#include "ara/core/vector.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief Class to hold the data for a required service instance.
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class RequiredServiceInstanceConfig final {
 public:
  /*! Type name alias for a vector of Instance Specifier. */
  using InstanceSpecifierContainer = ara::core::Vector<InstanceSpecifier>;
  /*! Type name alias of Instance identifier. */
  using InstanceIdentifier = ipc_protocol::InstanceId;
  /*! Type name alias of Instance identifier. */
  using MinorVersion = ipc_protocol::MinorVersion;
  /*! Type name alias of integrity level. */
  using IntegrityLevel = amsr::ipc::IntegrityLevel;

  /*!
   * \brief Construct an empty required service instance configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RequiredServiceInstanceConfig() noexcept = default;

  /*!
   * \brief Destroy required service instance configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RequiredServiceInstanceConfig() noexcept = default;  // VCA_IPCB_STD_FUNCTION_DESTRUCTOR

  /*!
   * \brief Copy-Construct a required service instance configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_STD_VECTOR_COPY_CONSTRUCTOR, VCA_IPCB_DEFAULT_COPY_CONSTRUCTOR_REFERENCE_ARGUMENT
  RequiredServiceInstanceConfig(RequiredServiceInstanceConfig const&) = default;

  auto operator=(RequiredServiceInstanceConfig const&) noexcept -> RequiredServiceInstanceConfig& = delete;
  RequiredServiceInstanceConfig(RequiredServiceInstanceConfig&&) noexcept = delete;
  auto operator=(RequiredServiceInstanceConfig&&) noexcept -> RequiredServiceInstanceConfig& = delete;

  /*!
   * \brief Gets the instance identifier.
   *
   * \return The instance identifier.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetInstanceIdentifier() const noexcept -> InstanceIdentifier;

  /*!
   * \brief Gets the instance specifiers.
   *
   * \return The instance specifier container.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetInstanceSpecifiers() const noexcept -> InstanceSpecifierContainer const&;

  /*!
   * \brief Gets the minor version.
   *
   * \return The minor version.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMinorVersion() const noexcept -> MinorVersion;

  /*!
   * \brief Gets the expected server integrity level.
   *
   * \return The expected server integrity level.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetExpectedServerIntegrityLevel() const noexcept -> IntegrityLevel;

  /*!
   * \brief Sets the instance identifier.
   *
   * \param[in] instance_id  The instance identifier.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetInstanceIdentifier(InstanceIdentifier const& instance_id) noexcept;

  /*!
   * \brief Adds an instance specifier.
   *
   * \param[in] instance_specifier  The instance specifier string.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void AddInstanceSpecifier(InstanceSpecifier&& instance_specifier) noexcept;

  /*!
   * \brief Sets the minor version.
   *
   * \param[in] minor_version  The minor version.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetMinorVersion(MinorVersion const& minor_version) noexcept;

  /*!
   * \brief Sets the expected server integrity level.
   *
   * \param[in] expected_integrity_level  The expected server integrity level.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetExpectedServerIntegrityLevel(IntegrityLevel const& expected_integrity_level) noexcept;

 private:
  /*! The instance identifier. */
  InstanceIdentifier instance_identifier_{};
  /*! The instance specifier. */
  InstanceSpecifierContainer instance_specifiers_{};
  /*! The minor version. */
  MinorVersion minor_version_{};
  /*! The expected server integrity level. */
  IntegrityLevel expected_server_integrity_level_{};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_REQUIRED_SERVICE_INSTANCE_CONFIG_H_
