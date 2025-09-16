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
/**        \file  amsr/someip_binding_core/internal/client_manager_interface.h
 *        \brief  Client Manager Interface
 *
 *      \details  Interface of ClientManager
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding_core/internal/proxy_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Realizes a concrete Binding for the proxy communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
class ClientManagerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal steady FALSE \endinternal
   */
  ClientManagerInterface() = default;

  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal steady FALSE \endinternal
   */
  virtual ~ClientManagerInterface() = default;

  ClientManagerInterface(ClientManagerInterface const&) = delete;
  ClientManagerInterface(ClientManagerInterface&&) = delete;
  ClientManagerInterface& operator=(ClientManagerInterface const&) & = delete;
  ClientManagerInterface& operator=(ClientManagerInterface&&) & = delete;

  /*!
   * \brief       Registers one proxy binding on construction of a ServiceProxySomeIpBinding.
   * \details     Called from user thread(s)
   * \param[in]   proxy_identity Uniquely identify a proxy binding using service id, instance id, client id.
   * \param[in]   proxy_handler  A weak pointer to the constructed element to store in the map proxy_bindings_. On
   *                             method response a look-up through the registered proxy bindings occurs, to delegate
   *                             the response.
   *                             Proxy binding shall not be already registered.
   *                             Proxy binding shall not be nullptr.
   * \pre         No client with the same identity has been registered.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \internal steady FALSE \endinternal
   */
  virtual void RegisterProxyBinding(
      ProxyBindingIdentity const proxy_identity,
      std::weak_ptr<::amsr::someip_binding_core::internal::ProxyHandlerInterface> proxy_handler) noexcept = 0;

  /*!
   * \brief       De-register one proxy binding from the map (e.g. a proxy service instance gets destructed).
   * \details     Called from user thread(s)
   * \param[in]   proxy_identity The key to search for and the associated value to erase from the map.
   * \pre         Proxy binding has already been registered.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \internal steady FALSE \endinternal
   */
  virtual void DeRegisterProxyBinding(ProxyBindingIdentity const proxy_identity) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CLIENT_MANAGER_INTERFACE_H_
