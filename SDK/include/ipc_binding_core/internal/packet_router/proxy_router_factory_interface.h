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
/*!        \file
 *        \brief  The ProxyRouterFactoryInterface is responsible for creating ProxyRouters.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for creating ProxyRouters.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterProxy
 */
class ProxyRouterFactoryInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyRouterFactoryInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~ProxyRouterFactoryInterface() noexcept = default;

  ProxyRouterFactoryInterface(ProxyRouterFactoryInterface const&) noexcept = delete;
  ProxyRouterFactoryInterface(ProxyRouterFactoryInterface&&) noexcept = delete;
  auto operator=(ProxyRouterFactoryInterface const&) noexcept -> ProxyRouterFactoryInterface& = delete;
  auto operator=(ProxyRouterFactoryInterface&&) noexcept -> ProxyRouterFactoryInterface& = delete;

  /*!
   * \brief Constructs a ProxyRouter and returns a shared pointer to its interface.
   *
   * \return Valid shared pointer to a ProxyRouter.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Create() const noexcept -> amsr::UniquePtr<ProxyRouterInterface> = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_FACTORY_INTERFACE_H_
