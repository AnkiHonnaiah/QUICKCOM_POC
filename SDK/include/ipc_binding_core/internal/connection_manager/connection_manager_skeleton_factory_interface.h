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
 *        \brief  Interface to construct a connection manager skeleton.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/shared_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "ipc_binding_core/internal/connection_manager/connection_manager_skeleton_interface.h"
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_factory_interface.h"
#include "ipc_binding_core/internal/connection_manager/generic_acceptor_factory_interface.h"
#include "ipc_binding_core/internal/trace/trace_monitor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Defines an interface to create a generic connection manager skeleton.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionManagerSkeleton
 */
class ConnectionManagerSkeletonFactoryInterface {
 public:
  /*!
   * \brief Type-alias for ReactorInterface.
   */
  using ReactorInterface = osabstraction::io::reactor1::Reactor1Interface;

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
  virtual ~ConnectionManagerSkeletonFactoryInterface() noexcept = default;

  ConnectionManagerSkeletonFactoryInterface(ConnectionManagerSkeletonFactoryInterface const&) noexcept = delete;
  auto operator=(ConnectionManagerSkeletonFactoryInterface const&) noexcept
      -> ConnectionManagerSkeletonFactoryInterface& = delete;
  ConnectionManagerSkeletonFactoryInterface(ConnectionManagerSkeletonFactoryInterface&&) noexcept = delete;
  auto operator=(ConnectionManagerSkeletonFactoryInterface&&) noexcept
      -> ConnectionManagerSkeletonFactoryInterface& = delete;

  /*!
   * \brief Constructs a connection manager skeleton and returns a unique pointer to its interface.
   *
   * \param[in,out] reactor                      The reactor interface. The object must be valid for this object's
   *                                             entire life time.
   * \param[in,out] skeleton_router              The skeleton router that shall be used by ConnectionSkeleton. The
   *                                             object must be valid for this object's entire life time.
   * \param[in,out] trace_monitor                The trace monitor.
   * \param[in]     access_control               Reference to an access control implementation. The object must be valid
   *                                             for the entire lifetime of this object.
   * \param[in,out] connection_skeleton_factory  Valid factory to create a ConnectionSkeleton. It will be shared with
   *                                             each created server instance.
   * \param[in,out] acceptor_factory             Valid factory to create an ipc acceptor. It will be shared with each
   *                                             created server instance.
   *
   * \return Valid unique pointer to a connection manager skeleton interface.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Create(ReactorInterface& reactor, SkeletonRouterInterface& skeleton_router,
                      trace::TraceMonitor const& trace_monitor,
                      access_control::AccessControlInterface const& access_control,
                      amsr::SharedPtr<ConnectionSkeletonFactoryInterface> connection_skeleton_factory,
                      amsr::SharedPtr<GenericAcceptorFactoryInterface> acceptor_factory) noexcept
      -> amsr::UniquePtr<ConnectionManagerSkeletonInterface> = 0;

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
  ConnectionManagerSkeletonFactoryInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_CONNECTION_MANAGER_SKELETON_FACTORY_INTERFACE_H_
