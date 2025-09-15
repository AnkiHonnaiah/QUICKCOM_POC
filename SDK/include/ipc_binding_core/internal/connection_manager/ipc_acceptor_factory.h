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
 *        \brief  Creates a generic acceptor interface
 *        \unit IpcBinding::IpcBindingCore::IpcAdaptor
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ACCEPTOR_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ACCEPTOR_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/ipc/unicast_address.h"

#include "ipc_binding_core/internal/connection_manager/generic_acceptor.h"
#include "ipc_binding_core/internal/connection_manager/generic_acceptor_factory_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Factory to construct an IpcAcceptor object.
 */
class IpcAcceptorFactory : public GenericAcceptorFactoryInterface {
 public:
  /*!
   * \brief Default constructs an IpcAcceptorFactory.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  IpcAcceptorFactory() noexcept = default;

  /*!
   * \brief Destroys the Acceptor factory object.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcAcceptorFactory() noexcept override = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  IpcAcceptorFactory(IpcAcceptorFactory const&) noexcept = delete;
  auto operator=(IpcAcceptorFactory const&) noexcept -> IpcAcceptorFactory& = delete;
  IpcAcceptorFactory(IpcAcceptorFactory&&) noexcept = delete;
  auto operator=(IpcAcceptorFactory&&) noexcept -> IpcAcceptorFactory& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericAcceptorFactoryInterface::Create
   */
  auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor,
              amsr::ipc::UnicastAddress const& address) noexcept -> amsr::UniquePtr<GenericAcceptor> final;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr
#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ACCEPTOR_FACTORY_H_
