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
/**        \file  generic_proxy.h
 *        \brief  Generic proxy implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_PROXY_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_PROXY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/string_view.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/events/generic_proxy_event.h"
#include "amsr/socal/fields/generic_proxy_field.h"
#include "amsr/socal/generic_container.h"
#include "amsr/socal/handle_type.h"
#include "amsr/socal/internal/factory/backend_factory_registry.h"
#include "amsr/socal/internal/factory/backend_factory_registry_interface.h"
#include "amsr/socal/internal/factory/proxy_backend_factory_interface.h"
#include "amsr/socal/internal/generic_proxy_backend_interface.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/reboot_detection_state_machine.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_interface.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_internal_interface.h"
#include "amsr/socal/methods/generic_proxy_fire_and_forget_method.h"
#include "amsr/socal/methods/generic_proxy_method.h"
#include "amsr/socal/service_state.h"
#include "amsr/socal/skeleton_connection_state.h"
#include "ara/com/find_service_handle.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"
#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace socal {

/*!
 * \brief Generic proxy implementation.
 */
class AMSR_LG_BETA GenericProxy final {
  /*!
   * \brief Type alias to generic proxy event container.
   */
  using GenericEventContainer = GenericContainer<::amsr::socal::events::GenericProxyEvent>;

  /*!
   * \brief Type alias to generic proxy field container.
   */
  using GenericFieldContainer = GenericContainer<::amsr::socal::fields::GenericProxyField>;

  /*!
   * \brief Type alias to generic proxy method container.
   */
  using GenericMethodContainer = GenericContainer<::amsr::socal::methods::GenericProxyMethod>;

  /*!
   * \brief Type alias to generic proxy fire-and-forget method container.
   */
  using GenericFireAndForgetMethodContainer = GenericContainer<::amsr::socal::methods::GenericProxyFireAndForgetMethod>;

 public:
  /*!
   * \brief Type alias to the handle type.
   * \vpublic
   */
  using HandleType = ::amsr::socal::HandleType;

  /*!
   * \brief Type alias to the service handle container.
   * \vpublic
   */
  using ServiceHandleContainer = ::ara::com::ServiceHandleContainer<HandleType>;

  /*!
   * \brief The find service handle type used for stopping the asynchronous find service request.
   * \vpublic
   */
  using FindServiceHandle = ::ara::com::FindServiceHandle;

  /*!
   * \brief   The start find service callback type used for receiving the asynchronous find service updates.
   * \details API signature: void(ServiceHandleContainer, FindServiceHandle)
   * \vpublic
   */
  using StartFindServiceCallback = ::ara::com::FindServiceHandlerExt<HandleType>;

  /*!
   * \brief Type alias to the generic proxy event type.
   * \vpublic
   */
  using ProxyEvent = ::amsr::socal::events::GenericProxyEvent;

  /*!
   * \brief Type alias to the generic proxy field type.
   * \vpublic
   */
  using ProxyField = ::amsr::socal::fields::GenericProxyField;

  /*!
   * \brief Type alias to the generic proxy method type.
   * \vpublic
   */
  using ProxyMethod = ::amsr::socal::methods::GenericProxyMethod;

  /*!
   * \brief Type alias to the generic proxy fire-and-forget method type.
   * \vpublic
   */
  using ProxyFireAndForgetMethod = ::amsr::socal::methods::GenericProxyFireAndForgetMethod;

  /*!
   * \brief Type alias to element access to generic proxy event container.
   * \vpublic
   */
  using ProxyEventAccess = GenericEventContainer::ElementAccess;

  /*!
   * \brief Type alias to element access to generic proxy field container.
   * \vpublic
   */
  using ProxyFieldAccess = GenericFieldContainer::ElementAccess;

  /*!
   * \brief Type alias to element access to generic proxy method container.
   * \vpublic
   */
  using ProxyMethodAccess = GenericMethodContainer::ElementAccess;

  /*!
   * \brief Type alias to element access to generic proxy fire-and-forget method container.
   * \vpublic
   */
  using ProxyFireAndForgetMethodAccess = GenericFireAndForgetMethodContainer::ElementAccess;

  /*!
   * \brief Move constructs a GenericProxy.
   * \param[in] other  GenericProxy object which has to be moved.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   */
  GenericProxy(GenericProxy&& other) noexcept = default;

  /*!
   * \brief Destructs GenericProxy.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   */
  ~GenericProxy() noexcept;

  GenericProxy(GenericProxy const&) = delete;
  auto operator=(GenericProxy const&) -> GenericProxy& = delete;
  auto operator=(GenericProxy&&) -> GenericProxy& = delete;

  /*!
   * \brief Find the available services synchronously with InstanceIdentifier.
   * \param[in] instance_identifier  The instance identifier of the required service instance that needs to be searched.
   * \return Result containing the list of handles that can be used to create a generic proxy instance.
   * \pre         Socal is initialized.
   * \pre         The given instance identifier is valid.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto FindService(::ara::com::InstanceIdentifier const& instance_identifier) noexcept
      -> ::ara::core::Result<ServiceHandleContainer>;

  /*!
   * \brief Find the available services synchronously with InstanceSpecifier.
   * \param[in] instance_specifier  The instance specifier of the required service interface, for which a matching
   *                                service instance shall be searched.
   * \return Result containing the list of handles that can be used to create a generic proxy instance.
   * \pre         Socal is initialized.
   * \pre         The given instance specifier is valid.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto FindService(::ara::core::InstanceSpecifier const& instance_specifier) noexcept
      -> ::ara::core::Result<ServiceHandleContainer>;

  /*!
   * \brief   Start an asynchronous FindService notification using an instance identifier.
   * \details Note that execution of the provided handler is done within Socal's R-Port default thread-pool context.
   *          Therefore, it is strongly advised to reduce the blocking calls from the handler as much as possible to
   *          allow the R-Port default thread-pool to process other pending tasks. Non-compliance to this may have
   *          undesired consequences (e.g., delayed processing of communication requests).
   * \param[in] handler   The handler that shall be called upon any matching service is offered/stopped.
   *                      The given handler shall not be null and must be valid at least until the StopFindService is
   *                      called with the corresponding FindServiceHandle.
   *                      Any exception thrown by the callback will lead to a termination through "std::terminate()".
   * \param[in] instance  The instance identifier of the required service instance that needs to be searched.
   * \return Result containing the FindServiceHandle that can be used to stop the asynchronous notifications.
   * \pre         Socal is initialized.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \pre         The given instance identifier is valid.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto StartFindService(StartFindServiceCallback handler, ::ara::com::InstanceIdentifier instance) noexcept
      -> ::ara::core::Result<FindServiceHandle>;

  /*!
   * \brief   Start an asynchronous FindService notification using an instance specifier.
   * \details Note that execution of the provided handler is done within Socal's R-Port default thread-pool context.
   *          Therefore, it is strongly advised to reduce the blocking calls from the handler as much as possible to
   *          allow the R-Port default thread-pool to process other pending tasks. Non-compliance to this may have
   *          undesired consequences (e.g., delayed processing of communication requests).
   * \param[in] handler   The handler that shall be called upon any matching service is offered/stopped.
   *                      The given handler shall not be null and must be valid at least until the StopFindService is
   *                      called with the corresponding FindServiceHandle.
   *                      Any exception thrown by the callback will lead to a termination through "std::terminate()".
   * \param[in] instance  The instance specifier of the required service interface, for which a matching service
   *                      instance shall be searched.
   * \return Result containing the FindServiceHandle that can be used to stop the asynchronous notifications.
   * \pre         Socal is initialized.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \pre         The given instance specifier is valid.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto StartFindService(StartFindServiceCallback handler, ::ara::core::InstanceSpecifier instance) noexcept
      -> ::ara::core::Result<FindServiceHandle>;

  /*!
   * \brief   Stop receiving asynchronous FindService notifications.
   * \details Calling this API from an application thread (i.e., from outside the FindServiceHandler callback) may lead
   *          to a deadlock between the calling thread and the ara::com thread handling the call to FindServiceHandler,
   *          if any user mutex is locked in the calling thread, as well as in the FindServiceHandler callback.
   * \param[in] handle  The handle returned by the StartFindService API or within the StartFindService callback.
   * \pre         Socal is initialized.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \context     App
   * \threadsafe  FALSE for same FindServiceHandle instance, TRUE for different instances.
   * \reentrant   FALSE for same FindServiceHandle instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void StopFindService(FindServiceHandle handle) noexcept;

  /*!
   * \brief Creates a GenericProxy instance.
   * \param[in] handle  The handle returned by the FindService API.
   * \return Result containing the GenericProxy instance, or an error.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \error ComErrc::kNetworkBindingFailure   The network binding reports a failure.
   * \pre         Socal is initialized.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto Create(HandleType const& handle) noexcept -> ::ara::core::Result<GenericProxy>;

  /*!
   * \brief Polls the current connection state to the skeleton.
   * \return SkeletonConnectionState enum value representing the connection state to the skeleton.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto ReadSkeletonConnectionState() const noexcept -> ::amsr::socal::SkeletonConnectionState;

  /*!
   * \brief Poll the current state of the service backend.
   * \return A ServiceState indicating if the service backend is up, down or restarted.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto ReadServiceState() noexcept -> ::amsr::socal::ServiceState;

  /*!
   * \brief List of events corresponding to this generic proxy instance.
   * \vpublic
   */
  GenericEventContainer events;  // VECTOR SL VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private

  /*!
   * \brief List of fields corresponding to this generic proxy instance.
   * \vpublic
   */
  GenericFieldContainer fields;  // VECTOR SL VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private

  /*!
   * \brief List of methods corresponding to this generic proxy instance.
   * \vpublic
   */
  GenericMethodContainer methods;

  /*!
   * \brief List of fire-and-forget methods corresponding to this generic proxy instance.
   * \vpublic
   */
  GenericFireAndForgetMethodContainer fnf_methods;

  /*!
   * \brief Type alias for the generic proxy backend factory interface.
   */
  using BackendFactoryInterface = ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<
      ::amsr::socal::internal::GenericProxyBackendInterface>;

  /*!
   * \brief Type alias for the generic proxy backend factory registry interface.
   */
  using BackendFactoryRegistryInterface = ::amsr::socal::internal::factory::ProxyBackendFactoryRegistryInterface<
      ::amsr::socal::internal::GenericProxyBackendInterface>;

  /*!
   * \brief Type alias for the singleton access of generic proxy backend factory registry interface.
   */
  using BackendFactoryRegistrySingletonAccess = ::amsr::generic::SingletonAccess<BackendFactoryRegistryInterface>;

  /*!
   * \brief Type alias for the generic proxy service discovery interface.
   */
  using ServiceDiscoveryInterface = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface;

  /*!
   * \brief Type alias for the singleton access of generic proxy service discovery interface.
   */
  using ServiceDiscoverySingletonAccess = ::amsr::generic::SingletonAccess<ServiceDiscoveryInterface>;

  /*!
   * \brief Creates the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void CreateSingletons() noexcept;

  /*!
   * \brief Destroys the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static void DestroySingletons() noexcept;

  /*!
   * \brief Getter for ProxyBackendFactoryRegistry singleton member.
   * \return SingletonAccess to the ProxyBackendFactoryRegistryInterface.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto GetFactoryRegistry() noexcept -> BackendFactoryRegistrySingletonAccess;

  /*!
   * \brief Returns the service discovery singleton access to the ProxyServiceDiscoveryInterface.
   * \return SingletonAccess to the ProxyServiceDiscoveryInterface.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto GetServiceDiscovery() noexcept -> ServiceDiscoverySingletonAccess;

 private:
  /*!
   * \brief Type alias for the generic proxy backend factory registry.
   */
  using BackendFactoryRegistry = ::amsr::socal::internal::factory::ProxyBackendFactoryRegistry<
      ::amsr::socal::internal::GenericProxyBackendInterface>;

  /*!
   * \brief Type alias for the singleton of generic proxy backend factory registry.
   */
  using BackendFactoryRegistrySingleton = ::amsr::generic::Singleton<BackendFactoryRegistry>;

  /*!
   * \brief Type alias for the generic proxy service discovery.
   */
  using ServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscovery;

  /*!
   * \brief Type alias for the singleton of generic proxy service discovery.
   */
  using ServiceDiscoverySingleton = ::amsr::generic::Singleton<ServiceDiscovery>;

  /*!
   * \brief Type alias for a singleton access of ProxyServiceDiscoveryInternalInterface.
   */
  using InternalServiceDiscoverySingletonAccess = ::amsr::generic::SingletonAccess<
      ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInternalInterface>;

  /*!
   * \brief Type alias for the RequiredInstanceIdentifier.
   */
  using RequiredInstanceIdentifier = ::amsr::socal::internal::RequiredInstanceIdentifier;

  /*!
   * \brief Type alias for the ProvidedInstanceIdentifier.
   */
  using ProvidedInstanceIdentifier = ::amsr::socal::internal::ProvidedInstanceIdentifier;

  /*!
   * \brief Returns the service discovery singleton access to the ProxyServiceDiscoveryInternalInterface.
   * \return SingletonAccess to the ProxyServiceDiscoveryInternalInterface.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  static auto GetServiceDiscoveryInternal() noexcept -> InternalServiceDiscoverySingletonAccess;

  /*!
   * \brief Constructs a GenericProxy instance.
   * \param[in] required_instance_id  Required instance ID of the generic proxy instance.
   * \param[in] provided_instance_id  Provided instance ID of the generic proxy instance.
   * \param[in] proxy_backend         Proxy backend.
   * \param[in] generic_events        A map containing all the events of the generic proxy instance.
   * \param[in] generic_fields        A map containing all the fields of the generic proxy instance.
   * \param[in] generic_methods       A map containing all the methods of the generic proxy instance.
   * \param[in] generic_fnf_methods   A map containing all the fire-and-forget methods of the generic proxy instance.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  explicit GenericProxy(RequiredInstanceIdentifier const& required_instance_id,
                        ProvidedInstanceIdentifier const& provided_instance_id,
                        BackendFactoryRegistry::BackendInterfacePtr proxy_backend,
                        GenericEventContainer::ContainerType generic_events,
                        GenericFieldContainer::ContainerType generic_fields,
                        GenericMethodContainer::ContainerType generic_methods,
                        GenericFireAndForgetMethodContainer::ContainerType generic_fnf_methods) noexcept;

  /*!
   * \brief Resolve an instance specifier lookup table entry using the given instance identifier.
   * \param[in] instance_identifier  The InstanceIdentifier to be resolved.
   * \return The resolved instance specifier lookup table entry.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  static ::amsr::socal::internal::InstanceSpecifierLookupTableEntry ResolveInstanceSpecifierMapping(
      ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Resolve an instance specifier lookup table entry using the given instance specifier.
   * \param[in] instance_specifier  The InstanceSpecifier to be resolved.
   * \return List of instance specifier lookup table entries. The list can be empty in case no matching entry exists.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  static ::amsr::socal::internal::InstanceSpecifierLookupTableEntryContainer ResolveInstanceSpecifierMapping(
      ::amsr::core::InstanceSpecifier const& instance_specifier) noexcept;

  /*!
   * \brief Log the provided message and trigger an abort.
   * \param[in] log_message  The message to be logged.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  static void LogAndAbort(::amsr::core::StringView const& log_message) noexcept;

  /*!
   * \brief Singleton instance of generic proxy backend factory registry.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static BackendFactoryRegistrySingleton backend_factory_registry_;

  /*!
   * \brief Singleton instance of generic service discovery.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static ServiceDiscoverySingleton service_discovery_;

  /*!
   * \brief   Proxy backend.
   * \details The backend shall be destroyed after all the generic proxy events, methods, and fields are destroyed.
   */
  BackendFactoryRegistry::BackendInterfacePtr proxy_backend_;

  /*!
   * \brief Required instance identifier of this proxy.
   */
  ::amsr::socal::internal::RequiredInstanceIdentifier required_instance_id_;

  /*!
   * \brief Provided instance identifier of this proxy.
   */
  ::amsr::socal::internal::ProvidedInstanceIdentifier provided_instance_id_;

  /*!
   * \brief State machine for reboot detection.
   */
  std::unique_ptr<internal::RebootDetectionStateMachine> reboot_detection_{
      std::make_unique<internal::RebootDetectionStateMachine>()};
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_PROXY_H_
