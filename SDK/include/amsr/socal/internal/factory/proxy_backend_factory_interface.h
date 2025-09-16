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
/**        \file  proxy_backend_factory_interface.h
 *        \brief  Factory interface to create the proxy backends from the Bindings.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_PROXY_BACKEND_FACTORY_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_PROXY_BACKEND_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/socal/internal/internal_instance_identifier.h"

namespace amsr {
namespace socal {
namespace internal {
namespace factory {

/*!
 * \brief Factory interface to create the proxy backends from the Bindings.
 * \tparam ProxyBackendInterfaceType  The proxy backend interface to be created.
 */
template <typename ProxyBackendInterfaceType>
class ProxyBackendFactoryInterface {
 public:
  /*!
   * \brief Type alias for the proxy backend interface pointer.
   */
  using BackendInterfacePtr = std::unique_ptr<ProxyBackendInterfaceType>;

  /*!
   * \brief Constructs the proxy backend factory.
   * \pre         -
   * \context     BindingInit
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   */
  ProxyBackendFactoryInterface() noexcept = default;

  ProxyBackendFactoryInterface(ProxyBackendFactoryInterface const&) = delete;
  ProxyBackendFactoryInterface(ProxyBackendFactoryInterface&&) = delete;
  ProxyBackendFactoryInterface& operator=(ProxyBackendFactoryInterface const&) & = delete;
  ProxyBackendFactoryInterface& operator=(ProxyBackendFactoryInterface&&) & = delete;

  /*!
   * \brief Destroys the proxy backend factory.
   * \pre         -
   * \context     BindingShutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProxyBackendFactoryInterface() noexcept = default;

  /*!
   * \brief   Creates the proxy backend.
   * \details For each proxy instance a distinct backend instance shall be created to ensure thread-safety.
   * \param[in] provided_instance_id  The provided instance ID for which the backend shall be created.
   * \return Result containing a pointer to the created backend or an error. The returned pointer must not be null.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \error ComErrc::kNetworkBindingFailure   The network binding reports a failure.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::core::Result<BackendInterfacePtr> CreateBackend(
      ProvidedInstanceIdentifier const& provided_instance_id) noexcept = 0;
};

}  // namespace factory
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_PROXY_BACKEND_FACTORY_INTERFACE_H_
