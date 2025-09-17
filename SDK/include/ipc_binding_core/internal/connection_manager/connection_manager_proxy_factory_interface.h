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
 *        \brief  Interface to construct a connection manager proxy.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/access_control/access_control_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_manager_proxy_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_proxy_factory_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Defines an interface to create a generic connection manager proxy.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerProxy
 */
class ConnectionManagerProxyFactoryInterface {
 public:
  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual ~ConnectionManagerProxyFactoryInterface() noexcept = default;

  ConnectionManagerProxyFactoryInterface(ConnectionManagerProxyFactoryInterface const&) noexcept = delete;
  auto operator=(ConnectionManagerProxyFactoryInterface const&) noexcept
      -> ConnectionManagerProxyFactoryInterface& = delete;
  ConnectionManagerProxyFactoryInterface(ConnectionManagerProxyFactoryInterface&&) noexcept = delete;
  auto operator=(ConnectionManagerProxyFactoryInterface&&) noexcept -> ConnectionManagerProxyFactoryInterface& = delete;

  /*!
   * \brief Constructs a connection manager proxy and returns a unique pointer to its interface.
   *
   * \param[in,out] reactor                   The reactor interface. The object must be valid for Server's entire life
   *                                          time.
   * \param[in,out] trace_monitor             The trace monitor reference. The object must exist for the whole lifetime
   *                                          of this object.
   * \param[in]     access_control            Reference to an access control implementation. The object must be valid
   *                                          for the entire lifetime of this object.
   * \param[in]     connection_proxy_factory  Valid shared factory to create a ConnectionProxy.
   *
   * \return Valid unique pointer to a connection manager proxy.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor, trace::TraceMonitor const& trace_monitor,
                      access_control::AccessControlInterface const& access_control,
                      amsr::SharedPtr<ConnectionProxyFactoryInterface> connection_proxy_factory) noexcept
      -> amsr::UniquePtr<ConnectionManagerProxyInterface> = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ConnectionManagerProxyFactoryInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_PROXY_FACTORY_INTERFACE_H_
