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
/**        \file
 *        \brief  Defines an interface for construction of ConnectionProxy objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"
#include "ipc_binding_core/internal/connection_manager/proxy_router_connector_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_unicast_address.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Defines an interface to create ConnectionProxies and RouterConnectors.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ConnectionProxyFactoryInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

  /*!
   * \brief The type alias for a remote IPC unicast address.
   */
  using IpcUnicastAddress = ipc_protocol::IpcUnicastAddress;

  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ConnectionProxyFactoryInterface() noexcept = default;

  ConnectionProxyFactoryInterface(ConnectionProxyFactoryInterface const&) noexcept = delete;
  auto operator=(ConnectionProxyFactoryInterface const&) noexcept -> ConnectionProxyFactoryInterface& = delete;
  ConnectionProxyFactoryInterface(ConnectionProxyFactoryInterface&&) noexcept = delete;
  auto operator=(ConnectionProxyFactoryInterface&&) noexcept -> ConnectionProxyFactoryInterface& = delete;

  /*!
   * \brief Construct a ConnectionProxy and return an unique pointer to its interface.
   *
   * \param[in,out] reactor                   Used Reactor.
   * \param[in,out] connection_manager_proxy  The connection manager proxy. The object must be valid for created
   *                                          ConnectionProxy's entire life time.
   * \param[in,out] router_connector          The RouterConnector. The object must be valid for created
   *                                          ConnectionProxy's entire life time.
   * \param[in,out] connection                The connection. The object must be valid for created ConnectionProxy's
   *                                          entire life time.
   * \param[in]     address                   The remote IPC unicast address of an IPC server to which this connection
   *                                          is meant to be established.
   * \param[in]     trace_monitor             The trace monitor.
   * \param[in]     access_control            Reference to an access control implementation. The object must be valid
   *                                          for the entire lifetime of this object.
   * \param[in]     integrity_level           The expected IntegrityLevel for this connection.
   *
   * \return Shared pointer to the constructed ConnectionProxy instance.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto CreateConnectionProxy(ReactorInterface& reactor,
                                     ConnectionManagerProxyInterface& connection_manager_proxy,
                                     amsr::SharedPtr<ProxyRouterConnectorInterface> const& router_connector,
                                     amsr::UniquePtr<GenericConnection>&& connection, IpcUnicastAddress const& address,
                                     trace::TraceMonitor const& trace_monitor,
                                     access_control::AccessControlInterface const& access_control,
                                     amsr::ipc::IntegrityLevel const integrity_level) noexcept
      -> amsr::SharedPtr<ConnectionProxyInterface> = 0;

  /*!
   * \brief Construct a RouterConnector and return a shared pointer to its interface.
   *
   * \return Shared pointer to the constructed RouterConnector instance.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto CreateRouterConnector() const noexcept -> amsr::SharedPtr<ProxyRouterConnectorInterface> = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionProxyFactoryInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_PROXY_FACTORY_INTERFACE_H_
