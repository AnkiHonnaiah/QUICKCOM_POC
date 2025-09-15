/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Definition of class StateMachineBase.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_MACHINE_BASE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_MACHINE_BASE_H_

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

#include "amsr/core/optional.h"

#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_base.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_connected.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_connecting.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_corrupted.h"
#include "amsr/zero_copy_common/memcon/internal/server/remote/states/state_disconnected.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {
namespace remote {
namespace states {

/*!
 * \brief       Get the maximum size of all states in byte.
 * \return      Maximum size of all states in byte.
 * \context     ANY
 * \pre         -
 *
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
static constexpr auto GetMaxStateSize() -> std::size_t {
  return std::max({sizeof(StateConnecting), sizeof(StateConnected), sizeof(StateDisconnected), sizeof(StateCorrupted)});
}

static_assert(GetMaxStateSize() > 0U, "The maximum state size must be greater than zero.");

/*!
 * \brief Base class of remote receiver state machines.
 * \vprivate Component Private
 */
class StateMachineBase {
 public:
  /*!
   * \brief Destruct the StateMachineBase.
   * \vprivate Component Private
   */
  virtual ~StateMachineBase() noexcept;

  /*!
   * \brief Do not construct a StateMachineBase by copying.
   * \vprivate Component Private
   */
  StateMachineBase(StateMachineBase const&) noexcept = delete;

  /*!
   * \brief Do not construct a StateMachineBase by moving.
   * \vprivate Component Private
   */
  StateMachineBase(StateMachineBase&&) noexcept = delete;

  /*!
   * \brief Do not copy another StateMachineBase into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateMachineBase const&) noexcept -> StateMachineBase& = delete;

  /*!
   * \brief Do not move another StateMachineBase into this instance.
   * \vprivate Component Private
   */
  auto operator=(StateMachineBase&&) noexcept -> StateMachineBase& = delete;

  /*!
   * \brief       Request a transition to another state.
   * \param       to_receiver_state Receiver state specifying the state to transition to.
   *                                Must not be ReceiverState::kConnecting. This is not checked.
   *                                Must not be the currently active state. This is not checked.
   * \param       opt_error_code    Optional containing an error code if a state transition to ReceiverState::kCorrupted
   *                                or ReceiverState::kDisconnected is requested.
   *                                Must only contain a value when \p to_receiver_state is ReceiverState::kCorrupted or
   *                                ReceiverState::kDisconnected. This is not checked.
   *                                Must be one of the following errors:
   *                                - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If
   *                                  the receiver closed the connection without adhering to the protocol.
   *                                - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                                  receiver crashed.
   *                                - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server
   *                                  notices a protocol error during the communication with the receiver.
   *                                This is not checked.
   * \context     ANY
   * \pre         There must not be a pending state transition request. This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  void TransitionToRequest(ReceiverState to_receiver_state,
                           ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code) noexcept;

 protected:
  /*!
   * \brief Construct a StateMachineBase.
   * \vprivate Component Private
   */
  StateMachineBase() noexcept = default;

  /*!
   * \brief       Emplace a state in the state storage.
   * \details     Constructs a state in-place in the state storage and updates the current state.
   * \tparam      T Type of the concrete state that is emplaced.
   *              The std::enable_if condition enforces that
   *              ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase is either the same as T
   *              or a public and unambiguous base of T, ignoring cv-qualifiers.
   *              Since ::amsr::zero_copy_common::memcon::internal::server::remote::states::StateBase has pure virtual
   *              functions, only the second option is possible.
   * \tparam      Args... Types of the arguments passed to the constructor of the concrete state that is emplaced.
   * \param       args Arguments passed to the constructor of the concrete state that is emplaced.
   * \context     ANY
   * \pre         The state storage must not contain an object whose lifetime is still ongoing.
   *              This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  // VECTOR NC VectorC++-V7.1.10: MD_ZEROCOPYCOMMON_V7.1.10_volatileInTemplateEnableIf
  template <class T, class... Args,
            typename = std::enable_if_t<(std::is_base_of<StateBase, T>::value) &&
                                        (std::is_convertible<T const volatile*, StateBase const volatile*>::value)>>
  void EmplaceState(Args&&... args) noexcept {
    // VECTOR NC VectorC++-V5.0.1: MD_ZEROCOPYCOMMON_V5.0.1_forwardingArguments
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_ZEROCOPYCOMMON_A5.0.1_forwardingArguments
    // VCA_ZEROCOPYCOMMON_SLC_20_STATE_MACHINE_BASE_EMPLACE_STATE_CLASS_TYPE_INVALID
    state_ptr_ = new (&state_storage_) T{std::forward<Args>(args)...};
  }

  /*!
   * \brief       Construct the specified state in the state storage.
   * \param       to_state Receiver state specifying the state to construct in the state storage.
   *                       Must not be ReceiverState::kConnecting.
   * \context     ANY
   * \pre         The state storage must not contain an object whose lifetime is still ongoing.
   *              This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  virtual void TransitionToConstructionHelper(ReceiverState to_state) noexcept = 0;

  /*!
   * \brief       Handle the last transition request.
   * \details     The transition request must have been requested using the TransitionToRequest() method.
   *              Does nothing, if no transition was requested.
   * \return      True, if a transition request was handled. False, otherwise.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto HandleTransitionToRequest() noexcept -> bool;

  /*!
   * \brief       Get the current state.
   * \return      The current state (non-const reference).
   * \context     ANY
   * \pre         Must only be called when there is a current state.
   *              This can be ensured by calling either TransitionToRequest() followed by HandleTransitionToRequest() or
   *              EmplaceState() at least once.
   *              This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetCurrentState() noexcept -> StateBase&;

  /*!
   * \brief       Get the current state.
   * \return      The current state (const reference).
   * \context     ANY
   * \pre         Must only be called when there is a current state.
   *              This can be ensured by calling either TransitionToRequest() followed by HandleTransitionToRequest() or
   *              EmplaceState() at least once.
   *              This is not checked.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetCurrentState() const noexcept -> StateBase const&;

  /*!
   * \brief       Get the current optional error code.
   * \return      The current optional error code.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  auto GetCurrentErrorCode() const noexcept -> ::amsr::core::Optional<::amsr::core::ErrorCode>;

 private:
  /*!
   * \brief   Type used to provide storage for the state.
   * \details The size of the storage is big enough to store every state (one at a time).
   *          The alignment of the storage is big enough to fulfill the strictest alignment requirement of the states.
   */
  using StateStorageType = std::aligned_storage_t<GetMaxStateSize()>;

  /*!
   * \brief Storage used to store the current state.
   */
  StateStorageType state_storage_{};

  /*!
   * \brief Pointer to the current state that is stored in the state storage.
   */
  StateBase* state_ptr_{nullptr};

  /*!
   * \brief   Optional containing the last unhandled transition to request.
   * \details A value is emplaced by the TransitionToRequest() method. The optional is reset by the
   *          HandleTransitionToRequest() method.
   */
  ::amsr::core::Optional<ReceiverState> transition_to_request_opt_{::amsr::core::nullopt};

  /*!
   * \brief Optional containing an error code if the last state transition was caused by an error.
   * \details A value is either set or reset by each call to the TransitionToRequest() method.
   */
  ::amsr::core::Optional<::amsr::core::ErrorCode> opt_transition_to_request_error_code_{::amsr::core::nullopt};
};

}  // namespace states
}  // namespace remote
}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_REMOTE_STATES_STATE_MACHINE_BASE_H_
