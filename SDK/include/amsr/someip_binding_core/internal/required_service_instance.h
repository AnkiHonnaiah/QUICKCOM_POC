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
/*!        \file  amsr/someip_binding_core/internal/required_service_instance.h
 *        \brief  ara::com SOME/IP Binding Required Service Instance memory wrapper
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::RequiredServiceInstance
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_binding_core/internal/required_service_instance_impl.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
/*!
 * \brief Manages memory ownership of RequiredServiceInstanceImpl to allow the object to be movable
 */
class RequiredServiceInstance final {
 public:
  /*!
   * \brief Type alias for shared_ptr<RequiredServiceInstanceImpl>
   */
  using RequiredServiceInstanceImplSharedPtr = std::shared_ptr<RequiredServiceInstanceImpl>;

  /*!
   * \brief       Constructor.
   * \param[in]   required_service_instance_impl The required service instance implementation
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit RequiredServiceInstance(RequiredServiceInstanceImplSharedPtr required_service_instance_impl) noexcept;

  /*!
   * \brief       Default Destructor.
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
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  ~RequiredServiceInstance() noexcept = default;

  /*!
   * \brief       Default move constructor.
   * \details     This will invalidate the owned resource. Any call to an instance of a
   *              moved-from object of this type will trigger an abort.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  RequiredServiceInstance(RequiredServiceInstance&&) noexcept = default;

  /*!
   * \brief       Default move assignment operator.
   * \details     This will invalidate the owned resource. Any call to an instance of a
   *              moved-from object of this type will trigger an abort.
   * \return      The required service instance.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  RequiredServiceInstance& operator=(RequiredServiceInstance&&) & noexcept = default;

  RequiredServiceInstance(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance const&) & = delete;

  /*!
   * \brief        Registers a listener
   * \param[in]    listener The listener to register
   * \pre          -
   *
   * \context App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void RegisterListener(std::weak_ptr<service_discovery::ServiceListenerInterface> listener) noexcept;

  /*!
   * \brief        Unregisters a listener
   * \param[in]    listener The listener to unregister
   * \pre          -
   *
   * \context App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void UnregisterListener(std::weak_ptr<service_discovery::ServiceListenerInterface> listener) noexcept;

  /*!
   * \brief        Creates a binding client for a specific service id
   * \param[in]    service_id The service id
   *
   * \return       amsr::core::Result with the created client or an error.
   *
   * \error       ClientError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ClientError::kSystemConfigurationNotAvailable if this required service instance is not configured.
   * \error       ClientError::kSystemPrivilegesNoAccess if access is denied for this service.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre          -
   *
   * \context App  (multi threaded)
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<LocalClient> CreateClient(ProvidedServiceInstanceId const& service_id) const noexcept;

 private:
  /*!
   * \brief The required service instance impl.
   *        Multiple RequiredServiceInstances can share the same impl.
   *        Example: SomeIpBindingXf and SomeIpBindingLite search for the same service at the same time.
   */
  RequiredServiceInstanceImplSharedPtr required_service_instance_impl_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_H_
