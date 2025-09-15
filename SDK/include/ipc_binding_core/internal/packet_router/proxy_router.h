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
 *        \brief  The ProxyRouter routes incoming and outgoing messages between transformation unit and connection proxy
 *                unit.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/shared_ptr.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_event_backend_interface.h"
#include "ipc_binding_core/internal/backend_interfaces/proxy_fire_and_forget_method_backend_interface.h"
#include "ipc_binding_core/internal/backend_interfaces/proxy_method_backend_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Class for routing between proxy and connection.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterProxy
 */
class ProxyRouter final : public ProxyRouterInterface {
 public:
  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ~ProxyRouter() noexcept final = default;  // VCA_IPCB_MOLE_1298

  ProxyRouter(ProxyRouter const&) noexcept = delete;
  ProxyRouter(ProxyRouter&&) noexcept = delete;
  auto operator=(ProxyRouter const&) noexcept -> ProxyRouter& = delete;
  auto operator=(ProxyRouter&&) noexcept -> ProxyRouter& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ServiceInstanceStateListenerInterface::OnServiceInstanceUp()
   */
  void OnServiceInstanceUp() noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ServiceInstanceStateListenerInterface::OnServiceInstanceDown()
   */
  void OnServiceInstanceDown() noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::OnConnected()
   */
  auto OnConnected(amsr::WeakPtr<ProxyRouterConnectorInterface> const& router_connector) noexcept -> void final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::OnDisconnected()
   */
  auto OnDisconnected() noexcept -> void final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::GetConnectionProxy()
   */
  auto GetConnectionProxy() const noexcept -> amsr::SharedPtr<connection_manager::ConnectionProxyInterface> final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::SetRouterConnector()
   */
  void SetRouterConnector(amsr::WeakPtr<ProxyRouterConnectorInterface> const& router_connector) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::AddEventXf()
   */
  void AddEventXf(ipc_protocol::EventId event_id, ProxyEventBackendInterface& event_xf) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::AddMethodXf()
   */
  void AddMethodXf(ipc_protocol::MethodId method_id, ProxyMethodBackendInterface& method_xf) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::AddFireAndForgetMethodXf()
   */
  void AddFireAndForgetMethodXf(ipc_protocol::MethodId method_id,
                                ProxyFireAndForgetMethodBackendInterface& method_xf) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::GetEventXf()
   */
  auto GetEventXf(ipc_protocol::EventId event_id) noexcept -> ProxyEventBackendInterface* final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::GetMethodXf()
   */
  auto GetMethodXf(ipc_protocol::MethodId method_id) noexcept -> ProxyMethodBackendInterface* final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::ReleaseEventXf()
   */
  void ReleaseEventXf(ipc_protocol::EventId event_id) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::ReleaseMethodXf()
   */
  void ReleaseMethodXf(ipc_protocol::MethodId method_id) noexcept final;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::ProxyRouterInterface::ReleaseFireAndForgetMethodXf()
   */
  void ReleaseFireAndForgetMethodXf(ipc_protocol::MethodId method_id) noexcept final;

  /*!
   * \brief Create a new proxy router.
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
  static auto Create() noexcept -> amsr::UniquePtr<ProxyRouter>;

 private:
  /*!
   * \brief Constructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyRouter() noexcept;

  /*!
   * \brief Type alias for the used logger.
   */
  using Logger = logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for storing EventXfInterfaces in a map.
   */
  using EventXfMap = ara::core::Map<ipc_protocol::EventId, ProxyEventBackendInterface*>;

  /*!
   * \brief Type alias for storing MethodXfInterfaces in a map.
   */
  using MethodXfMap = ara::core::Map<ipc_protocol::MethodId, ProxyMethodBackendInterface*>;

  /*!
   * \brief Type alias for storing FireAndForgetMethodXfInterfaces in a map.
   */
  using FireAndForgetMethodXfBackendMap =
      ara::core::Map<ipc_protocol::MethodId, ProxyFireAndForgetMethodBackendInterface*>;

  /*!
   * \brief Pointer to the currently stored RouterConnector.
   */
  amsr::WeakPtr<ProxyRouterConnectorInterface> router_connector_{};

  /*!
   * \brief Holds the currently registered EventXfInterfaces.
   */
  EventXfMap event_xf_table_{};

  /*!
   * \brief Holds the currently registered MethodXfInterfaces.
   */
  MethodXfMap method_xf_table_{};

  /*!
   * \brief Holds the currently registered FireAndForgetMethodXfInterfaces.
   */
  FireAndForgetMethodXfBackendMap fire_and_forget_method_xf_table_{};

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_PROXY_ROUTER_H_
