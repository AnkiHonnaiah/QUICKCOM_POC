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
 *        \brief Interface of the pending request map.
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/core/promise.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   Provides functionality to store method requests into a map and restore requests on asynchronous response.
 * \details The map that stores the pending requests is protected from concurrent access, so that multiple requests can
 *          be triggered in parallel or a request and response at the same time.
 *
 * \tparam Output  Type of the response value that will be wrapped in a promise.
 *
 * \unit IpcBinding::IpcBindingCore::ProxyMethod
 */
template <typename Output>
class PendingRequestMapInterface {
 public:
  /*!
   * \brief Alias for SessionId.
   */
  using SessionId = ::amsr::ipc_binding_core::internal::ipc_protocol::SessionId;

  /*!
   * \brief Type-alias for the stored promise.
   */
  using Promise = ::amsr::core::Promise<Output>;

  /*!
   * \brief Optional for map entry.
   */
  using OptionalMapEntry = ::amsr::core::Optional<std::pair<SessionId::type, Promise>>;

  /*!
   * \brief Constructs a pending request map.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  PendingRequestMapInterface() noexcept = default;

  PendingRequestMapInterface(PendingRequestMapInterface const&) noexcept = delete;
  PendingRequestMapInterface(PendingRequestMapInterface&&) noexcept = delete;
  auto operator=(PendingRequestMapInterface const&) -> PendingRequestMapInterface& = delete;
  auto operator=(PendingRequestMapInterface&&) -> PendingRequestMapInterface& = delete;

  /*!
   * \brief Destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~PendingRequestMapInterface() noexcept = default;

  /*!
   * \brief   Create a promise, store it in the pending request map and return a future associated with the promises.
   * \details If there is already a request entry with the identical key existing, the returned optional is empty.
   *
   * \param[in] session_id  Session ID used as map key.
   *
   * \return An optional future for the stored promise.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3 : MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto StoreRequest(SessionId session_id) noexcept -> ::amsr::core::Optional<::amsr::core::Future<Output>> = 0;

  /*!
   * \brief Fetch a request by the given request key (Session ID) and remove the request entry from the pending request
   *        map.
   *
   * \param[in] session_id  Session ID to identify the pending request promise.
   *
   * \return An Optional containing the promise corresponding to the request key, if it was found.
   *
   * \pre         A request is already moved to the map.
   * \context     App | Reactor.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3 : MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto MoveOutRequest(SessionId session_id) noexcept -> ::amsr::core::Optional<Promise> = 0;

  /*!
   * \brief Fetch the next request and remove the request entry from the wrapped map.
   *
   * \return An optional entry from the pending request map.
   *
   * \pre         A request is already moved to the map.
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3 : MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto MoveOutNextRequest() noexcept -> OptionalMapEntry = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_INTERFACE_H_
