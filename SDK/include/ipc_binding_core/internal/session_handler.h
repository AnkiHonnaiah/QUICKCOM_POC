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
 *        \brief  Session Id handling (incrementing) for method request, events and fields.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SESSION_HANDLER_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SESSION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief   Session ID handling (incrementing) for method request, events and fields.
 * \details Wrapper prevents from incorrect usage of the session identifier. For instance it prevents from decrementing
 *          (--session) or using a signed SessionType.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class SessionHandler final {
 public:
  /*!
   * \brief Type alias for session ID.
   */
  using SessionId = ipc_protocol::SessionId;

  /*!
   * \brief The initial session ID.
   */
  static constexpr SessionId kInitialSessionId{1};

  /*!
   * \brief Construct a SessionHandler with default initial session ID.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  SessionHandler() noexcept;

  /*!
   * \brief Construct a SessionHandler with a custom initial session ID.
   *
   * \param[in] initial_session_id  Optional initial value of the session ID.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit SessionHandler(SessionId initial_session_id) noexcept;

  /*!
   * \brief Destroy the SessionHandler.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~SessionHandler() noexcept = default;

  SessionHandler(SessionHandler const&) noexcept = delete;
  auto operator=(SessionHandler const&) noexcept -> SessionHandler& = delete;
  SessionHandler(SessionHandler&&) noexcept = delete;
  auto operator=(SessionHandler&&) noexcept -> SessionHandler& = delete;

  /*!
   * \brief Return the current session ID value.
   *
   * \return Current session Id.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto GetSessionId() const noexcept -> SessionId;

  /*!
   * \brief   Handles the increment and wrap-around logic.
   * \details The user is not required to check for the 0-value, it will skipped internally.
   *
   * \return The updated session handler.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto operator++() noexcept -> SessionHandler&;

  /*!
   * \brief Reset the current session ID to the initial value configured via ctor parameter.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  void Reset() noexcept;

 private:
  /*!
   * \brief Type-alias for underlying integer type of SessionId.
   */
  using SessionIdRaw = SessionId::type;

  /*!
   * \brief The initial session ID.
   */
  SessionId const initial_session_id_;

  /*!
   * \brief Session ID.
   */
  std::atomic<SessionIdRaw> current_session_id_;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SESSION_HANDLER_H_
