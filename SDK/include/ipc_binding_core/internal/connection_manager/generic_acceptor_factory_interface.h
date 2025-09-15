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
 *        \brief  Defines interface to create a generic acceptor.
 *        \unit IpcBinding::IpcBindingCore::IpcAdaptor
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_ACCEPTOR_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_ACCEPTOR_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/ipc/unicast_address.h"
#include "ara/core/result.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"
#include "vac/language/unique_function.h"

#include "ipc_binding_core/internal/connection_manager/generic_acceptor.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Defines an interface to create a generic Acceptor.
 */
class GenericAcceptorFactoryInterface {
 public:
  /*!
   * \brief Default destroys the interface.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~GenericAcceptorFactoryInterface() noexcept = default;

  GenericAcceptorFactoryInterface(GenericAcceptorFactoryInterface const&) noexcept = delete;
  auto operator=(GenericAcceptorFactoryInterface const&) noexcept -> GenericAcceptorFactoryInterface& = delete;
  GenericAcceptorFactoryInterface(GenericAcceptorFactoryInterface&&) noexcept = delete;
  auto operator=(GenericAcceptorFactoryInterface&&) noexcept -> GenericAcceptorFactoryInterface& = delete;

  /*!
   * \brief Constructs a generic acceptor and returns a unique pointer to it.
   *
   * \param[in] reactor  Reactor interface for asynchronous operation. The reactor has to exist for the whole lifetime
   *                     of this Acceptor object. An Acceptor uses up to 4 Reactor callback slots.
   * \param[in] address  Local address of the acceptor.
   *
   * \return Valid unique pointer to a  GenericAcceptor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor,
                      amsr::ipc::UnicastAddress const& address) noexcept -> amsr::UniquePtr<GenericAcceptor> = 0;

 protected:
  /*!
   * \brief Default construct an interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericAcceptorFactoryInterface() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_ACCEPTOR_FACTORY_INTERFACE_H_
