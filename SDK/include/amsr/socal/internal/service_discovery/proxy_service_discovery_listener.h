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
/**        \file  proxy_service_discovery_listener.h
 *        \brief  Listener for ServiceDiscovery update on the proxy side.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_LISTENER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace socal {
namespace internal {
namespace service_discovery {

/*!
 * \brief   Listener for service update state. Used by ProxyServiceDiscovery class.
 * \details All instances that need live update for the Service available state shall implement this interface and
 *          register itself within ProxyServiceDiscovery. ProxyServiceDiscovery will trigger the callbacks once an
 *          update is available.
 *
 * \unit Socal::ServiceDiscovery::ProxyServiceDiscovery
 */
class ProxyServiceDiscoveryListener {
 public:
  /*!
   * \brief Default constructs the listener.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ProxyServiceDiscoveryListener() noexcept = default;

  ProxyServiceDiscoveryListener& operator=(ProxyServiceDiscoveryListener const&) & noexcept = delete;
  ProxyServiceDiscoveryListener& operator=(ProxyServiceDiscoveryListener&&) & noexcept = delete;

  /*!
   * \brief Callback that shall be triggered as soon as a service instance is being offered.
   * \pre         The Listener shall register itself into a ServiceDiscovery instance.
   * \context     Backend | App.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual void OnServiceOffered() noexcept = 0;

  /*!
   * \brief Callback that shall be triggered as soon as a service instance is being stopped.
   * \pre         The Listener shall register itself into a ServiceDiscovery instance.
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual void OnServiceStopped() noexcept = 0;

 protected:
  /*!
   * \brief Destroys the listener.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~ProxyServiceDiscoveryListener() noexcept = default;

  /*!
   * \brief Copy constructs a listener.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ProxyServiceDiscoveryListener(ProxyServiceDiscoveryListener const&) noexcept = default;

  /*!
   * \brief Move constructs a listener from another one.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ProxyServiceDiscoveryListener(ProxyServiceDiscoveryListener&&) noexcept = default;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_LISTENER_H_
