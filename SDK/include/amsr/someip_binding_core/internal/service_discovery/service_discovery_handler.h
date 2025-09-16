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
/*!        \file  amsr/someip_binding_core/internal/service_discovery/service_discovery_handler.h
 *        \brief  ara::com SOME/IP Binding Required Service Instance Handler
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ServiceDiscoveryHandler
 *
 *      \details  ServiceDiscoveryHandler serves as an anchor between the SOME/IP protocol via POSIX and the
 *                ara::com related high-level layers for service discovery communication.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_HANDLER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>

#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_discovery_handler_interface.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace service_discovery {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_sv
/*!
 * \brief The StringView operator.
 */
using ::vac::container::literals::operator""_sv;

/*!
 * \brief Realizes a concrete Binding for the proxy communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP for Service Discovery.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
class ServiceDiscoveryHandler final : public ServiceListenerInterface, public ServiceDiscoveryHandlerInterface {
 public:
  ServiceDiscoveryHandler(ServiceDiscoveryHandler const&) = delete;
  ServiceDiscoveryHandler(ServiceDiscoveryHandler&&) = delete;
  ServiceDiscoveryHandler& operator=(ServiceDiscoveryHandler const&) & = delete;
  ServiceDiscoveryHandler& operator=(ServiceDiscoveryHandler&&) & = delete;

  /*!
   * \brief Type alias for service listener pointer.
   */
  using ServiceListenerPtr = typename std::add_pointer<
      ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface>::type;

  /*!
   * \brief Registered service listener objects. This link is needed to communicate service updates.
   */
  using ServiceListenerMap =
      std::map<::amsr::someip_binding_core::internal::RequiredServiceInstanceId, ServiceListenerPtr>;

  /*!
   * \brief        Default constructor
   * \pre          -
   * \context      Init
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \vprivate
   * \synchronous  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceDiscoveryHandler() = default;

  /*!
   * \brief       Use default destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ServiceDiscoveryHandler() noexcept final = default;

 private:
  /*!
   * \brief       Start listening to service updates with this listener.
   * \param[in]   service_instance  The binding specific service interface identifier.
   * \param[in]   listener             Service listener that wants to listen to the service updates.
   *                                   Parameter must not be a nullptr.
   *
   * \pre         All Listeners shall be registered before StartServiceDiscovery is being called
   * \pre         Same instance id cannot be registered twice.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void StartListening(::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& service_instance,
                      ServiceListenerPtr listener) noexcept final;

  /*!
   * \brief       Stop listening to service updates.
   * \param[in]   service_instance  The binding specific service interface identifier.
   *
   * \pre         All Listeners shall stop listening after StopServiceDiscovery has been called
   * \pre         Listener must have been already registered
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void StopListening(
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& service_instance) noexcept final;

  /*!
   * \brief       Called on asynchronous notification about newly offered services.
   * \param[in]   offered_service The provided service instance ID of the offered service.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceUp(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final;

  /*!
   * \brief       Called upon a stopped service.
   * \param[in]   stopped_service The provided service instance ID of the stopped service.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceDown(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final;

  /*!
   * \brief Find an already registered listener with the same or overlapping required service instance id.
   * \param[in] required_service_instance The ID to compare against.
   * \return iterator pointing at the matching listener, or at service_listeners_.cend() if not found.
   *
   * \pre -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  typename ServiceListenerMap::const_iterator FindMatchingListener(
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& required_service_instance) const noexcept;

  /*!
   * \brief Find an already registered listener with the same or overlapping required service instance id.
   * \param[in] provided_service_instance The ID to compare against.
   * \return iterator pointing at the matching listener, or at service_listeners_.end() if not found.
   *
   * \pre -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  typename ServiceListenerMap::const_iterator FindMatchingListener(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& provided_service_instance) const noexcept;

  /*!
   * \brief Collection of service listeners.
   */
  ServiceListenerMap service_listeners_{};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, "ServiceDiscoveryHandler"_sv};
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_HANDLER_H_
