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
 *        \brief  Provided service instance configuration class
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_PROVIDED_SERVICE_INSTANCE_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_PROVIDED_SERVICE_INSTANCE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/integrity_level.h"
#include "ara/core/vector.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief Class to hold the data for a provided service instance.
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class ProvidedServiceInstanceConfig final {
 public:
  /*! Type name alias for a vector of Instance Specifier. */
  using InstanceSpecifierContainer = ara::core::Vector<InstanceSpecifier>;

  /*! Type name alias of Instance identifier. */
  using InstanceIdentifier = ipc_protocol::InstanceId;

  /*! Type name alias of BasicIpc Domain. */
  using BasicIpcDomain = ipc_protocol::BasicIpcDomain;

  /*! Type name alias of BasicIpc Port. */
  using BasicIpcPort = ipc_protocol::BasicIpcPort;

  /*! Type name alias of integrity level. */
  using IntegrityLevel = amsr::ipc::IntegrityLevel;

  /*!
   * \brief Construct an empty provided service instance configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ProvidedServiceInstanceConfig() noexcept = default;

  /*!
   * \brief Destroy provided service instance configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITHOUT_ARGUMENT
  ~ProvidedServiceInstanceConfig() noexcept = default;

  /*!
   * \brief Copy-Construct a provided service instance configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VCA_IPCB_STD_VECTOR_COPY_CONSTRUCTOR, VCA_IPCB_DEFAULT_COPY_CONSTRUCTOR_REFERENCE_ARGUMENT
  ProvidedServiceInstanceConfig(ProvidedServiceInstanceConfig const&) = default;

  auto operator=(ProvidedServiceInstanceConfig const&) noexcept -> ProvidedServiceInstanceConfig& = delete;
  ProvidedServiceInstanceConfig(ProvidedServiceInstanceConfig&&) noexcept = delete;
  auto operator=(ProvidedServiceInstanceConfig&&) noexcept -> ProvidedServiceInstanceConfig& = delete;

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
   * \brief Gets the IPC domain.
   *
   * \return The IPC domain.
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
  auto GetIpcDomain() const noexcept -> BasicIpcDomain;

  /*!
   * \brief Gets the IPC port.
   *
   * \return The IPC port.
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
  auto GetIpcPort() const noexcept -> BasicIpcPort;

  /*!
   * \brief Gets the expected client integrity level.
   *
   * \return The expected client integrity level.
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
  auto GetExpectedClientIntegrityLevel() const noexcept -> IntegrityLevel;

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
   * \brief Sets the IPC domain.
   *
   * \param[in] ipc_domain  The IPC domain.
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
  void SetIpcDomain(BasicIpcDomain const& ipc_domain) noexcept;

  /*!
   * \brief Sets the IPC port.
   *
   * \param[in] ipc_port  The IPC port.
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
  void SetIpcPort(BasicIpcPort const& ipc_port) noexcept;

  /*!
   * \brief Sets the expected client integrity level.
   *
   * \param[in] expected_integrity_level  The expected client integrity level.
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
  void SetExpectedClientIntegrityLevel(IntegrityLevel const& expected_integrity_level) noexcept;

 private:
  /*! The instance identifier. */
  InstanceIdentifier instance_identifier_{};

  /*! The instance specifier. */
  InstanceSpecifierContainer instance_specifiers_{};

  /*! The ipc domain. */
  BasicIpcDomain domain_{};

  /*! The ipc port. */
  BasicIpcPort port_{};

  /*! The expected client integrity level. */
  IntegrityLevel expected_client_integrity_level_{};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_PROVIDED_SERVICE_INSTANCE_CONFIG_H_
