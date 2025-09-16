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
/**        \file  proxy_service_discovery_interface.h
 *        \brief  Interface for receiving service discovery updates from the Bindings.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/internal_instance_identifier.h"

namespace amsr {
namespace socal {
namespace internal {
namespace service_discovery {

/*!
 * \brief Interface for receiving service discovery updates from the Bindings.
 *
 * \unit Socal::ServiceDiscovery::ProxyServiceDiscovery
 */
class ProxyServiceDiscoveryInterface {
 public:
  /*!
   * \brief Constructs the ProxyServiceDiscoveryInterface.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProxyServiceDiscoveryInterface() noexcept = default;

  ProxyServiceDiscoveryInterface(ProxyServiceDiscoveryInterface const&) = delete;
  ProxyServiceDiscoveryInterface(ProxyServiceDiscoveryInterface&&) = delete;
  ProxyServiceDiscoveryInterface& operator=(ProxyServiceDiscoveryInterface const&) & = delete;
  ProxyServiceDiscoveryInterface& operator=(ProxyServiceDiscoveryInterface&&) & = delete;

  /*!
   * \brief Offers a service and informs all listeners about the available service instance.
   * \param[in] required_instance_id  ID of the required service instance.
   * \param[in] provided_instance_id  ID of the offered instance.
   * \pre         Service with the given provided instance id has not been already offered.
   * \context     Backend
   * \threadsafe  TRUE
   *              FALSE against StopOfferService for same provided instance identifier, and
   *              TRUE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual void OfferService(RequiredInstanceIdentifier const& required_instance_id,
                            ProvidedInstanceIdentifier const& provided_instance_id) noexcept = 0;

  /*!
   * \brief Stop the given service and inform all listeners about the stopped service.
   * \param[in] required_instance_id  Required Instance ID of the stopped service.
   * \param[in] provided_instance_id  Provided Instance ID of the stopped service.
   * \pre         The instance was offered before.
   * \context     Backend
   * \threadsafe  TRUE
   *              FALSE against OfferService for same provided instance identifier, and
   *              TRUE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   */
  virtual void StopOfferService(RequiredInstanceIdentifier const& required_instance_id,
                                ProvidedInstanceIdentifier const& provided_instance_id) noexcept = 0;

 protected:
  /*!
   * \brief Destroys the ProxyServiceDiscoveryInterface.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~ProxyServiceDiscoveryInterface() noexcept = default;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_INTERFACE_H_
