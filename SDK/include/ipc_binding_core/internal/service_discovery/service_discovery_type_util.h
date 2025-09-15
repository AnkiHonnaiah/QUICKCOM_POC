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
/**        \file
 *        \brief  Utility to convert types between IpcBinding and IpcServiceDiscovery component.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_TYPE_UTIL_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_TYPE_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {

/*!
 * \brief   Utility to convert types between IpcBinding and IpcServiceDiscovery component.
 * \details Support type conversions:
 *          - RequiredServiceInstanceId
 *          - ProvidedServiceInstanceId
 *          - IpcUnicastAddress
 *
 * \unit IpcBinding::IpcBindingCore::ServiceDiscoveryTypeUtil
 */
class ServiceDiscoveryTypeUtil final {
 public:
  ServiceDiscoveryTypeUtil() noexcept = delete;
  ServiceDiscoveryTypeUtil(ServiceDiscoveryTypeUtil const&) noexcept = delete;
  auto operator=(ServiceDiscoveryTypeUtil const&) noexcept -> ServiceDiscoveryTypeUtil& = delete;
  ServiceDiscoveryTypeUtil(ServiceDiscoveryTypeUtil&&) noexcept = delete;
  auto operator=(ServiceDiscoveryTypeUtil&&) noexcept -> ServiceDiscoveryTypeUtil& = delete;

  /*!
   * \brief   Destroy the ID translator.
   * \details Destructor for this utility class (static APIs only) provided to avoid potential undefined behavior in
   *          case of future extension (e.g. non-static members).
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ServiceDiscoveryTypeUtil() noexcept = default;

  // ---- Conversions of RequiredServiceInstanceId <-> RequiredServiceInstanceIdentifier -------------------------------

  /*!
   * \brief Convert an IpcBinding RequiredServiceInstanceId to IpcServiceDiscovery RequiredServiceInstanceIdentifier.
   *
   * \param[in] required_service_instance_id  The RequiredServiceInstanceId to be converted.
   *
   * \return The converted IpcServiceDiscovery RequiredServiceInstanceIdentifier.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto ToServiceDiscoveryId(RequiredServiceInstanceId const& required_service_instance_id) noexcept
      -> ::amsr::ipc_service_discovery::internal::RequiredServiceInstanceIdentifier;

  /*!
   * \brief Convert an IpcServiceDiscovery RequiredServiceInstanceIdentifier to IpcBinding RequiredServiceInstanceId.
   *
   * \param[in] required_service_instance_identifier  The RequiredServiceInstanceIdentifier to be converted.
   *
   * \return The converted IpcBinding RequiredServiceInstanceId.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto FromServiceDiscoveryId(::amsr::ipc_service_discovery::internal::RequiredServiceInstanceIdentifier const&
                                         required_service_instance_identifier) noexcept -> RequiredServiceInstanceId;

  // ---- Conversions of ProvidedServiceInstanceId <-> ProvidedServiceInstanceIdentifier -------------------------------

  /*!
   * \brief Convert an IpcBinding ProvidedServiceInstanceId to IpcServiceDiscovery ProvidedServiceInstanceIdentifier.
   *
   * \param[in] provided_service_instance_id  The ProvidedServiceInstanceId to be converted.
   *
   * \return The converted IpcServiceDiscovery ProvidedServiceInstanceIdentifier.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto ToServiceDiscoveryId(ProvidedServiceInstanceId const& provided_service_instance_id) noexcept
      -> ::amsr::ipc_service_discovery::internal::ProvidedServiceInstanceIdentifier;

  /*!
   * \brief Convert an IpcServiceDiscovery ProvidedServiceInstanceIdentifier to IpcBinding ProvidedServiceInstanceId.
   *
   * \param[in] provided_service_instance_identifier  The ProvidedServiceInstanceIdentifier to be converted.
   *
   * \return The converted IpcBinding ProvidedServiceInstanceId.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto FromServiceDiscoveryId(::amsr::ipc_service_discovery::internal::ProvidedServiceInstanceIdentifier const&
                                         provided_service_instance_identifier) noexcept -> ProvidedServiceInstanceId;

  // ---- Conversions of IpcUnicastAddress <-> UnicastAddress ----------------------------------------------------------

  /*!
   * \brief Convert an IpcBinding IpcUnicastAddress to IpcServiceDiscovery UnicastAddress.
   *
   * \param[in] unicast_address  The IpcUnicastAddress to be converted.
   *
   * \return The converted IpcServiceDiscovery UnicastAddress.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto ToServiceDiscoveryUnicastAddress(ipc_protocol::IpcUnicastAddress const& unicast_address) noexcept
      -> ::amsr::ipc_service_discovery::internal::UnicastAddress;

  /*!
   * \brief Convert an IpcServiceDiscovery UnicastAddress to IpcBinding IpcUnicastAddress.
   *
   * \param[in] unicast_address  The UnicastAddress to be converted.
   *
   * \return The converted IpcBinding IpcUnicastAddress.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto FromServiceDiscoveryUnicastAddress(
      ::amsr::ipc_service_discovery::internal::UnicastAddress const& unicast_address) noexcept
      -> ipc_protocol::IpcUnicastAddress;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_TYPE_UTIL_H_
