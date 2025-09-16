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
 *        \brief  Provides a generic acceptor interface.
 *        \unit IpcBinding::IpcBindingCore::IpcAdaptor
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_ACCEPTOR_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_ACCEPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/ipc/unicast_address.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "osabstraction/osab_error_domain.h"
#include "vac/language/unique_function.h"

#include "ipc_binding_core/internal/connection_manager/generic_connection.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief Generic acceptor.
 */
class GenericAcceptor {
 public:
  /*!
   * \brief   The result of the accept operation.
   * \details On success it contains the accepted connection. Possible errors: osabstraction::OsabErrc::kResource (Linux
   *          only) Callback slots of the reactor exhausted. osabstraction::OsabErrc::kDisconnected  (Linux only)
   *          Acceptor object is not listening to connect requests anymore until the Acceptor is closed and
   *          reinitialized.
   */
  using AcceptCompletionResult = amsr::core::Result<amsr::UniquePtr<GenericConnection>>;

  /*!
   * \brief   Notifies about a completed asynchronous accept operation.
   * \details All Acceptor APIs are allowed to be called from an AcceptCompletionCallback.
   *
   * \param[in] result  The result of the accept operation. On success it contains the accepted connection. Possible
   *                    errors:
   *
   * \error osabstraction::OsabErrc::kResource      (Linux only) Callback slots of the reactor exhausted.
   * \error osabstraction::OsabErrc::kDisconnected  (Linux only) Acceptor object is not listening to connect requests
   *                                                anymore until the Acceptor is closed and reinitialized.
   *
   * \pre         -
   * \context     Reactor1::HandleEvents()
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  using AcceptCompletionCallback = vac::language::UniqueFunction<void(AcceptCompletionResult)>;

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
  virtual ~GenericAcceptor() noexcept = default;

  GenericAcceptor(GenericAcceptor const&) noexcept = delete;
  auto operator=(GenericAcceptor const&) noexcept -> GenericAcceptor& = delete;
  GenericAcceptor(GenericAcceptor&&) noexcept = delete;
  auto operator=(GenericAcceptor&&) noexcept -> GenericAcceptor& = delete;

  /*!
   * \brief   Initializes Acceptor object.
   * \details Starts accepting connection requests from clients in the background and enqueues them in a connection
   *          backlog. Call AcceptAsync() to get a callback (AcceptCompletionCallback) when an established connection is
   *          available in the connection backlog. The established connection is then returned in the
   *          AcceptCompletionCallback and removed from the connection backlog.
   *
   * \error osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kAlreadyConnected        Tried to initialize already initialized acceptor.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges  Not allowed to create server.
   * \error osabstraction::OsabErrc::kAddressNotAvailable     The address is not available or is currently in use.
   * \error osabstraction::OsabErrc::kResource                Not enough system resources to create server.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError  (Linux only) The file system in which the server shall be
   *                                                          created does not support a required operation or a file
   *                                                          system I/O error occurred.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto Init() noexcept -> ara::core::Result<void> = 0;

  /*!
   * \brief   Accept the next connection asynchronously.
   * \details Acceptor's Reactor will be used by to-be-constructed Connection object.
   *
   * \param[in] callback  Callback to notify about the completion of the accept operation. The Acceptor object takes
   *                      ownership of the passed callback.The ownership is held until the callback has been executed or
   *                      until the Acceptor object is closed.
   *
   * \error osabstraction::OsabErrc::kUninitialized  Tried to AcceptAsync for uninitialized acceptor.
   * \error osabstraction::OsabErrc::kBusy           AcceptAsync operating already ongoing.
   * \error osabstraction::OsabErrc::kDisconnected   (Linux only) Acceptor object is not listening to connect requests
   *                                                 anymore until the Acceptor is closed and reinitialized.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   */
  virtual auto AcceptAsync(AcceptCompletionCallback&& callback) noexcept -> ara::core::Result<void> = 0;

  /*!
   * \brief   Closes the Acceptor.
   * \details Stops all ongoing asynchronous operations, releases the system resources and puts the Acceptor object back
   *          to uninitialized state. No callback (AcceptCompletionCallback) will be called anymore but a callback might
   *          currently be ongoing. IsInUse() can be used after this call to check if a provided callback
   *          (AcceptCompletionCallback) is still executing. See ~Acceptor() why such a check may be necessary.
   *
   * \pre         -
   * \context     Not timer_manager::HandleTimerExpiry() if a timer manager is used.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto Close() noexcept -> void = 0;

  /*!
   * \brief   Checks if the Acceptor object still uses a provided callback (AcceptCompletionCallback) after it was
   *          closed.
   * \details This function should be called after the Acceptor was closed if you want to check if a provided callback
   *          is still executing. Note that this function always returns true if the Acceptor is still initialized (the
   *          Acceptor was once initialized but Close() was not called yet). See ~Acceptor() why such a check may be
   *          necessary.
   *
   * \return True if the Acceptor is initialized or any provided callback is currently executing, false otherwise.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto IsInUse() const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Default constructs the interface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericAcceptor() noexcept = default;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_GENERIC_ACCEPTOR_H_
