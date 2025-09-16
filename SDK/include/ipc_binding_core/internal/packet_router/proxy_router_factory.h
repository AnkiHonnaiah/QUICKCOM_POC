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
 *        \brief  The ProxyRouterFactory is responsible for creating ProxyRouters.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/packet_router/proxy_router_factory_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Class for creating ProxyRouters.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterProxy
 */
class ProxyRouterFactory final : public ProxyRouterFactoryInterface {
 public:
  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterFactoryInterface::ProxyRouterFactoryInterface
   */
  ProxyRouterFactory() noexcept = default;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterFactoryInterface::~ProxyRouterFactoryInterface
   */
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~ProxyRouterFactory() noexcept final = default;

  ProxyRouterFactory(ProxyRouterFactory const&) noexcept = delete;
  ProxyRouterFactory(ProxyRouterFactory&&) noexcept = delete;
  auto operator=(ProxyRouterFactory const&) noexcept -> ProxyRouterFactory& = delete;
  auto operator=(ProxyRouterFactory&&) noexcept -> ProxyRouterFactory& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterFactoryInterface::Create
   */
  auto Create() const noexcept -> amsr::UniquePtr<ProxyRouterInterface> final;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_FACTORY_H_
