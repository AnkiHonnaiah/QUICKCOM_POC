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
/**        \file  amsr/someip_binding_core/internal/service_listener_registry_interface.h
 *        \brief  ara::com service status update handler interface
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_LISTENER_REGISTRY_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_LISTENER_REGISTRY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class to allow a listener to service instance status updates to register and unregister itself
          for receiving service instance status updates.
 */
class ServiceListenerRegistryInterface {
 public:
  /*!
   * \brief Default default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceListenerRegistryInterface() noexcept = default;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServiceListenerRegistryInterface() noexcept = default;

  ServiceListenerRegistryInterface(ServiceListenerRegistryInterface const&) = delete;
  ServiceListenerRegistryInterface(ServiceListenerRegistryInterface&&) = delete;
  ServiceListenerRegistryInterface& operator=(ServiceListenerRegistryInterface const&) & = delete;
  ServiceListenerRegistryInterface& operator=(ServiceListenerRegistryInterface&&) & = delete;

  /*!
   * \brief        Registers a listener
   *
   * \param[in]    listener The listener to register
   *
   * \pre          -
   *
   * \context       App  (multi threaded)
   *
   * \threadsafe    true
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void RegisterListener(std::weak_ptr<service_discovery::ServiceListenerInterface> listener) noexcept = 0;

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
  virtual void UnregisterListener(std::weak_ptr<service_discovery::ServiceListenerInterface> listener) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVICE_LISTENER_REGISTRY_INTERFACE_H_
