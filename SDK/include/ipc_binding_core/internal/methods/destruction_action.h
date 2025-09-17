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
 *        \brief Provides functionality for cleaning up resources on future destruction.
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_DESTRUCTION_ACTION_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_DESTRUCTION_ACTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/methods/pending_request_map_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Provides functionality to clean up the remaining promise and route while destructing a future.
 *
 * \unit IpcBinding::IpcBindingCore::ProxyMethod
 */
template <typename Output>
class DestructionAction final {
  /*!
   * \brief Type-alias for SessionId.
   */
  using SessionId = ipc_protocol::SessionId;

 public:
  /*!
   * \brief   Constructs a destruction action.
   * \details During construction a weak_ptr for the pending request map is created. This is used to avoid any invalid
   *          access to the pending request map.
   *
   * \param[in] pending_request_map_ptr  A shared_ptr to a pending request map shared pointer.
   * \param[in] session_id               The session ID identifying the pending request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  DestructionAction(amsr::SharedPtr<PendingRequestMapInterface<Output>> const& pending_request_map_ptr,
                    SessionId const session_id)
      : pending_request_map_ptr_{pending_request_map_ptr}, session_id_{session_id} {}

  DestructionAction(DestructionAction const&) = delete;
  DestructionAction(DestructionAction const&&) = delete;
  auto operator=(DestructionAction const&) -> DestructionAction& = delete;
  auto operator=(DestructionAction&&) -> DestructionAction& = delete;

  /*!
   * \brief Destructor.
   *
   * \steady FALSE
   */
  // VCA_IPCB_STD_WEAK_POINTER_DESTRUCTOR
  ~DestructionAction() = default;

  /*!
   * \brief   Function call operator.
   * \details Remove key from pending_request_map if there is a map. Delete the response route.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Creates a shared_ptr from pending request map's weak pointer.
   * - If pointer is not null move out the request.
   * - Delete response route.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  void operator()() noexcept {
    // On destruction of the future this lambda is called and will clean up remnants.
    // If the pending request map is not expired, then we remove the request.
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10_A18.5.8_false_positive
    amsr::SharedPtr<PendingRequestMapInterface<Output>> const pending_request_map{pending_request_map_ptr_.Lock()};
    if (pending_request_map) {
      // The request is cleaned so we do not need to know if it exist before or not, so that the return value can be
      // safely discarded.
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_IPCBINDING_AutosarC++17_10_M0.3.1_NULLPointerCheckDoneInVCA
      // VCA_IPCB_DESTRUCTION_ACTION_HEADER_IS_VALID, VCA_IPCB_DESTRUCTION_ACTION_PERSISTENCY_WEAK_PTR_OBJECT
      static_cast<void>(pending_request_map->MoveOutRequest(session_id_));
    }
  }

 private:
  /*!
   * Weak pointer to the pending request map.
   */
  amsr::WeakPtr<PendingRequestMapInterface<Output>> pending_request_map_ptr_;

  /*!
   * The session ID.
   */
  SessionId const session_id_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_DESTRUCTION_ACTION_H_
