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
/**        \file  proxy_service_discovery.h
 *        \brief  Proxy Service Discovery class.
 *
 *      \details  Class used for Discovery of offered Services on proxy side.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <mutex>

#include "amsr/core/string_view.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_interface.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_internal_interface.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_listener.h"
#include "ara/core/vector.h"

namespace amsr {
namespace socal {
namespace internal {
namespace service_discovery {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief   Class used for Discovery of offered Services on Proxy side.
 * \details Usage:
 *          - Single ServiceDiscovery per ServiceInterface Type shall be instantiated and set directly to the
 *            corresponding proxy class.
 *          - Initialization:
 *            - Each Binding shall register the factories for all the configured instance IDs during initialization.
 *          - Runtime:
 *            - Once Service is offered via the binding, OfferService shall be called.
 *            - Once Service is stopped via the binding, StopOfferService shall be called.
 *            - At anytime the application can create a proxy instance,
 *              - Proxy constructor will use the registered factory to create the backend.
 *            - At anytime on proxy side, GetOfferedServiceInstances() might be called.
 *            - At anytime on proxy side, Listen/StopListen() might be called (normally used for Start/StopFindService).
 *            - Reboot detection shall call AddRequester() upon construction, to monitor all Service updates.
 *            - Reboot detection shall call RemoveRequester() upon destruction to free resources.
 *          - Shutdown:
 *            - Reboot detection shall unregister as requester to SD.
 *            - SD instance shall be de-registered from Proxy.
 *            - SD instance shall be destroyed.
 *
 * \unit Socal::ServiceDiscovery::ProxyServiceDiscovery
 */
class ProxyServiceDiscovery final : public ProxyServiceDiscoveryInterface,
                                    public ProxyServiceDiscoveryInternalInterface {
 public:
  /*!
   * \brief Type alias for the container that stores the list of offered services mapped with required instance IDs.
   */
  using OfferedServiceInstanceMap = std::map<RequiredInstanceIdentifier, OfferedServiceInstanceContainer>;

  /*!
   * \brief Type alias for easy access to StringView.
   * \vprivate
   */
  using StringViewType = ::amsr::core::StringView;

  /*!
   * \brief Listeners Map type Key:Internal instance ID Value: List of listener pointers.
   * \tparam InstanceIdentifierType  Instance identifier type (i.e., Required or Provided instance identifier).
   */
  template <typename InstanceIdentifierType>
  using ListenersMap = std::map<InstanceIdentifierType, ::ara::core::Vector<SdListener*>>;

  /*!
   * \brief Constructs the ProxyServiceDiscovery.
   * \param[in] service_shortname  Name of the service.
   * \pre     -
   * \context Init
   * \spec requires true; \endspec
   * \steady FALSE
   */
  explicit ProxyServiceDiscovery(StringViewType const& service_shortname) noexcept;

  ProxyServiceDiscovery(ProxyServiceDiscovery const&) = delete;
  ProxyServiceDiscovery(ProxyServiceDiscovery&&) = delete;
  ProxyServiceDiscovery& operator=(ProxyServiceDiscovery const&) & = delete;
  ProxyServiceDiscovery& operator=(ProxyServiceDiscovery&&) & = delete;

  /*!
   * \brief Destroys the ProxyServiceDiscovery.
   * \pre     -
   * \context Shutdown
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ~ProxyServiceDiscovery() noexcept = default;  // VCA_SOCAL_CALLING_STL_APIS

  /*!
   * \copydoc ProxyServiceDiscoveryInternalInterface::AddRequester()
   */
  void AddRequester(RequiredInstanceIdentifier const& req_id, ProvidedInstanceIdentifier const& prov_id,
                    SdListener* listener) noexcept final;

  /*!
   * \copydoc ProxyServiceDiscoveryInternalInterface::RemoveRequester()
   */
  void RemoveRequester(ProvidedInstanceIdentifier const& prov_id, SdListener* listener) noexcept final;

  /*!
   * \copydoc ProxyServiceDiscoveryInternalInterface::Listen()
   */
  void Listen(RequiredInstanceIdentifier const& req_instance_id, SdListener* listener) noexcept final;

  /*!
   * \copydoc ProxyServiceDiscoveryInternalInterface::StopListen()
   */
  void StopListen(RequiredInstanceIdentifier const& req_id, SdListener* listener) noexcept final;

  /*!
   * \copydoc ProxyServiceDiscoveryInternalInterface::GetOfferedServiceInstances()
   */
  OfferedServiceInstanceContainer GetOfferedServiceInstances(
      RequiredInstanceIdentifier const& required_instance_id) const noexcept final;

  /*!
   * \copydoc ProxyServiceDiscoveryInterface::OfferService()
   */
  void OfferService(RequiredInstanceIdentifier const& required_instance_id,
                    ProvidedInstanceIdentifier const& provided_instance_id) noexcept final;

  /*!
   * \copydoc ProxyServiceDiscoveryInterface::StopOfferService()
   */
  void StopOfferService(RequiredInstanceIdentifier const& required_instance_id,
                        ProvidedInstanceIdentifier const& provided_instance_id) noexcept final;

 private:
  /*!
   * \brief De-register a listener for a specific service.
   * \tparam    InstanceIdentifierType  Instance identifier type (i.e., Required or Provided instance identifier).
   * \param[in] id                      Instance Identifier of the monitored service.
   * \param[in] listener                Pointer of the callback handler.
   * \param[in] map                     The map that needs to be updated.
   * \pre         Listen() has been called for id and listener.
   * \context     Callback | App.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires listener != nullptr; \endspec
   * \steady FALSE
   */
  template <typename InstanceIdentifierType>
  void RemoveListener(InstanceIdentifierType const& id, SdListener* listener,
                      ListenersMap<InstanceIdentifierType>& map) const noexcept;

  /*!
   * \brief   This map stores for each required service instance the offered provided instance ids.
   * \details The required instance id is necessary to support the FindService ALL usecase, where multiple provided
   *          instance Ids can be offered for the instance id "ALL". The following functions add/remove the entries:
   *          - OfferService
   *          - StopOfferService The following functions are reading the entries:
   *          - GetOfferedServiceInstances
   */
  OfferedServiceInstanceMap offered_services_instances_map_{};

  /*!
   * \brief Map for each Instance ID to a given Service Discovery Entry. The following functions add/remove the map
   *        entries:
   *        - AddRequester
   *        - RemoveRequester The following functions are reading the map entries:
   *        - OfferService
   *        - StopOfferService
   */
  ListenersMap<ProvidedInstanceIdentifier> requesters_map_{};

  /*!
   * \brief Map for each Instance ID to a given Service Discovery Entry. The following functions add/remove the map
   *        entries:
   *        - Listen
   *        - StopListen The following functions are reading the map entries:
   *        - OfferService
   *        - StopOfferService
   */
  ListenersMap<RequiredInstanceIdentifier> listeners_map_{};

  /*!
   * \brief Mutex to guard listeners map, requesters map and offered instances list against parallel read/write.
   */
  mutable std::mutex service_discovery_mutex_{};

  /*!
   * \brief Our logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ProxyServiceDiscovery"_sv};

  /*!
   * \brief Name of the service.
   */
  StringViewType const service_name_;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_H_

// clang-format off
/*!
 * \exclusivearea amsr::socal::internal::service_discovery::ProxyServiceDiscovery::service_discovery_mutex_
 * Used for protecting against parallel read/write access to the map of listeners and requesters and
 * used for protecting the thread offering/stopping the service or listening to it against the thread
 * finding the service or doing the update.
 *
 * \protects amsr::socal::internal::service_discovery::ProxyServiceDiscovery::listeners_map_
 *           amsr::socal::internal::service_discovery::ProxyServiceDiscovery::requesters_map_
 *           amsr::socal::internal::service_discovery::ProxyServiceDiscovery::offered_services_instances_map_
 *           are the protected resource of an ProxyServiceDiscovery object.
 *
 * \usedin amsr::socal::internal::service_discovery::ProxyServiceDiscovery::OfferService
 * \usedin amsr::socal::internal::service_discovery::ProxyServiceDiscovery::StopOfferService
 * \usedin amsr::socal::internal::service_discovery::ProxyServiceDiscovery::RemoveListener
 * \usedin amsr::socal::internal::service_discovery::ProxyServiceDiscovery::GetOfferedServiceInstances
 * \usedin amsr::socal::internal::service_discovery::ProxyServiceDiscovery::AddRequester
 * \usedin amsr::socal::internal::service_discovery::ProxyServiceDiscovery::Listen
 *
 * \exclude User threads from accessing a variable that is already in access.
 * \length Medium Modifying / Reading the map and doing callbacks on other module.
 * \endexclusivearea
 */
// clang-format on
