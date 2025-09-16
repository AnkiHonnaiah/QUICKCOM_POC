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
 *        \brief  Provides a generic acceptor interface
 *        \unit IpcBinding::IpcBindingCore::IpcAdaptor
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ACCEPTOR_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ACCEPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/ipc/acceptor.h"
#include "amsr/ipc/unicast_address.h"
#include "ara/core/result.h"
#include "osabstraction/osab_error_domain.h"
#include "vac/language/unique_function.h"

#include "ipc_binding_core/internal/connection_manager/generic_acceptor.h"
#include "ipc_binding_core/internal/connection_manager/generic_connection.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Adaptor around an amsr::ipc::Acceptor.
 * \details Stores an amsr::ipc::Connection object and delegates all calls to it. The amsr::ipc::Acceptor constructs a
 *          amsr::ipc::Connection. The IpcAcceptor needs to wrap this connection into a GenericConnection derived
 *          object.
 */
class IpcAcceptor : public GenericAcceptor {
 public:
  /*!
   * \brief Destroys the Acceptor object. The Acceptor is closed by this destructor if it is open.
   *
   * \pre         No callback (AcceptCompletionCallback) is currently executing. If you are not sure that this is not
   *              the case, IsInUse() can be used after the Acceptor was closed to check if a callback is still
   *              executing.
   * \context     Not timer_manager::HandleTimerExpiry() if a timer manager is used.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcAcceptor() noexcept override;

  IpcAcceptor(IpcAcceptor const& other) noexcept = delete;
  auto operator=(IpcAcceptor const& other) noexcept -> IpcAcceptor& = delete;
  IpcAcceptor(IpcAcceptor&& other) noexcept = delete;
  auto operator=(IpcAcceptor&& other) noexcept -> IpcAcceptor& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericAcceptor::Init
   */
  auto Init() noexcept -> ara::core::Result<void> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericAcceptor::AcceptAsync
   */
  auto AcceptAsync(AcceptCompletionCallback&& callback) noexcept -> ara::core::Result<void> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericAcceptor::Close
   */
  void Close() noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericAcceptor::IsInUse
   */
  auto IsInUse() const noexcept -> bool final;

  /*!
   * \brief Constructs a amsr::ipc::Acceptor and returns a unique pointer to it.
   *
   * \param[in] reactor  Reactor interface for asynchronous operation. The reactor has to exist for the whole lifetime
   *                     of this Acceptor object. An Acceptor uses up to 4 Reactor callback slots.
   * \param[in] address  Local address of the acceptor.
   *
   * \return Unique pointer to an IpcAcceptor adapter.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor,
                     amsr::ipc::UnicastAddress const& address) noexcept -> amsr::UniquePtr<IpcAcceptor>;

 private:
  /*!
   * \brief Constructs a IpcAcceptor and returns a unique pointer to it.
   *
   * \param[in] reactor  Reactor interface for asynchronous operation. The reactor has to exist for the whole lifetime
   *                     of this Acceptor object. An Acceptor uses up to 4 Reactor callback slots.
   * \param[in] address  Local address of the acceptor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  IpcAcceptor(osabstraction::io::reactor1::Reactor1Interface& reactor,
              amsr::ipc::UnicastAddress const& address) noexcept;

  /*!
   * \brief   Accepts an amsr::ipc::Acceptor AcceptAsync result and forwards it to the user of GenericAcceptor.
   * \details Rewraps the passed result into a type suitable for calling GenericAcceptor's AcceptCompletionCallback.
   *          This function must only be called from an amsr::ipc::Acceptor::AcceptCompletionCallback.
   *
   * \param[in] result  Result containing an established amsr::ipc::Connection.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnRealAcceptorCallback(amsr::core::Result<amsr::ipc::Connection>&& result) noexcept;

  /*!
   * \brief IPC connection acceptor.
   */
  amsr::ipc::Acceptor acceptor_;

  /*!
   * \brief   Generic acceptor's AcceptCompletionCallback.
   * \details The IpcAcceptor stores the callback passed during an AcceptAsync call. This callback must be valid during
   *          an ongoing IpcAcceptor::AcceptAsync call.
   */
  AcceptCompletionCallback callback_;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_ACCEPTOR_H_
