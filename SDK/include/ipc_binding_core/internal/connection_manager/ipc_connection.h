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
 *        \brief  Interface for a generic ipc connection.
 *        \unit IpcBinding::IpcBindingCore::IpcAdaptor
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_CONNECTION_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/ipc/connection.h"
#include "amsr/unique_ptr.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/unique_function.h"

#include "ipc_binding_core/internal/connection_manager/generic_connection.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace connection_manager {

/*!
 * \brief   Adapter around an amsr::ipc::Connection object.
 * \details Provides a generic interface for an IPC connection.
 */
class IpcConnection final : public GenericConnection {
 public:
  /*!
   * \brief Destroys this object. The Connection is closed by this destructor if it is open.
   *
   * \pre         No callback (ConnectCompletionCallback / MessageAvailableCallback / ReceiveCompletionCallback /
   *              SendCompletionCallback) is currently executing. If you are not sure that this is not the case,
   *              IsInUse() can be used after the Connection was closed to check if any callback is still executing.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcConnection() noexcept final;

  IpcConnection(IpcConnection&& other) noexcept = delete;
  auto operator=(IpcConnection&& other) noexcept -> IpcConnection& = delete;
  IpcConnection(IpcConnection const&) noexcept = delete;
  auto operator=(IpcConnection const&) noexcept -> IpcConnection& = delete;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::ReceiveAsync
   */
  auto ReceiveAsync(amsr::ipc::MessageAvailableCallback&& msg_available_callback,
                    amsr::ipc::ReceiveCompletionCallback&& receive_completion_callback) noexcept
      -> ara::core::Result<void> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::ConnectAsync
   */
  auto ConnectAsync(amsr::ipc::UnicastAddress remote_address, amsr::ipc::ConnectCompletionCallback&& callback) noexcept
      -> ara::core::Result<void> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::Send
   */
  auto Send(ara::core::Span<osabstraction::io::ConstIOBuffer> message,
            amsr::ipc::SendCompletionCallback&& callback) noexcept -> ara::core::Result<amsr::ipc::SendResult> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::CheckPeerIntegrityLevel
   */
  auto CheckPeerIntegrityLevel(amsr::ipc::IntegrityLevel min_expected_level) const noexcept
      -> ara::core::Result<bool> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::GetPeerIdentity
   */
  auto GetPeerIdentity() const noexcept -> ara::core::Result<amsr::ipc::Credentials> final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::Close
   */
  void Close() noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::CheckIsOpen
   */
  auto CheckIsOpen() const noexcept -> bool final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::connection_manager::GenericConnection::IsInUse
   */
  auto IsInUse() const noexcept -> bool final;

  /*!
   * \brief Emplaces a amsr::ipc::Connection object in an IpcConnection object and returns a unique pointer to it.
   *
   * \param[in] connection  Amsr::ipc::Connection object that contains the real Ipc implementation.
   *
   * \return Unique pointer to an IPC connection.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto Create(amsr::ipc::Connection&& connection) noexcept -> amsr::UniquePtr<IpcConnection>;

  /*!
   * \brief Emplaces a amsr::ipc::Connection object in an IpcConnection object and returns a unique pointer to it.
   *
   * \param[in] reactor  Reactor interface for asynchronous operation. The reactor has to exist for the whole lifetime
   *                     of this IpcConnection object.
   *
   * \return Unique pointer to an IPC connection.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept
      -> amsr::UniquePtr<IpcConnection>;

 private:
  /*!
   * \brief Takes ownership of an amsr::ipc::Connection object and constructs the IpcConnection.
   *
   * \param[in] connection  Amsr::ipc::Connection object that contains the real Ipc implementation.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit IpcConnection(amsr::ipc::Connection&& connection) noexcept;

  /*! Real ipc connection. */
  amsr::ipc::Connection connection_;
};

}  // namespace connection_manager
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_CONNECTION_MANAGER_IPC_CONNECTION_H_
