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
 *        \brief  Definition of interface ServerInterface.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_INTERFACE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_INTERFACE_H_

#include <cstdint>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "vac/container/static_vector.h"
#include "vac/language/unique_function.h"

#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_token.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_id.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_state.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"

#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

/*!
 * \brief Interface of a server.
 * \vprivate Product Private
 */
// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_ServerInterface
class ServerInterface {
 public:
  /*!
   * \brief Type used for a class handle.
   */
  using ClassHandle = ::amsr::zero_copy_common::memcon::internal::ClassHandle;

  /*!
   * \brief Type used for a slot token.
   */
  using SlotToken = ::amsr::zero_copy_common::logic::internal::SlotToken;

  /*!
   * \brief Type used for a side channel interface.
   */
  using SideChannelInterface = ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface;

  /*!
   * \brief Type used for a receiver id.
   */
  using ReceiverId = ::amsr::zero_copy_common::memcon::internal::server::ReceiverId;

  /*!
   * \brief Type used for a receiver state.
   */
  using ReceiverState = ::amsr::zero_copy_common::memcon::internal::server::ReceiverState;

  /*!
   * \brief Type used for a server state.
   */
  using ServerState = ::amsr::zero_copy_common::memcon::internal::server::ServerState;

  /*!
   * \brief       Type used for the callback that is called when a state transition happened in any receiver.
   * \details     The callback is called each time a state transition happened asynchronously in any receiver.
   *              I.e., the state transition is triggered by the peer, not by the user of the server.
   *              Therefore, this callback is only called in the reactor context.
   *              Note: The receiver with \p receiver_id is in use when this callback is executing which means that
   *                    RemoveReceiver() cannot be called directly from this callback.
   * \param       receiver_id    The id of the receiver which triggered a state transition.
   * \param       receiver_state The new receiver state.
   * \param       opt_error_code Optional containing an error code that identifies the cause of the error.
   *                             Only contains a value when receiver_state is set to ReceiverState::kCorrupted.
   *                             Possible errors are:
   *                              - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the
   *                                receiver closed the connection without adhering to the protocol.
   *                              - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the
   *                                receiver crashed.
   *                              - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server
   *                                notices a protocol error during the communication with the receiver.
   * \context     REACTOR
   * \pre -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  using OnReceiverStateTransitionCallback =
      ::vac::language::UniqueFunction<void(ReceiverId receiver_id, ReceiverState receiver_state,
                                           ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code)>;

  /*!
   * \brief Type to hold information on whether a slot was dropped for a receiver class.
   */
  using DroppedInformation = ::vac::container::StaticVector<ClassHandle>;

  /*!
   * \brief       Destruct the ServerInterface.
   * \details     The preconditions ensure that this server is only destructed when the end state is reached and when
   *              it's ensured that no asynchronous reception of messages is currently ongoing.
   *              As for any other object, the user of this class is responsible for general lifetime concerns
   *              (including captures of this server in other asynchronous tasks).
   * \context     ANY
   * \pre         The server must be shut down, i.e. GetServerState() must return ServerState::kDisconnected.
   * \pre         The server must not be in use, i.e. IsInUse() must return false.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual ~ServerInterface() noexcept = 0;

  /*!
   * \brief       Get the state of this server.
   * \details     Note that the state may change at any time. This means that between a call to this function and a call
                  to another function, the state may have already changed again.
   * \return      State of this server.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto GetServerState() const noexcept -> ServerState = 0;

  /*!
   * \brief       Check if an additional receiver can be added to this instance.
   * \return      True, if an additional receiver can be added. False, otherwise.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto CanAddReceiver() const noexcept -> ::amsr::core::Result<bool> = 0;

  /*!
   * \brief       Add a new receiver to this instance.
   * \details     The new receiver instance is created with the receiver class it belongs to and a receiver id is
   *              reserved. There isn't any communication started yet with the actual receiver. The communication
   *              starts with a call to ConnectReceiver() where the handshake protocol is started. For details, see the
   *              CDD of ConnectReceiver().
   *              If a receiver was added it can only be removed when it is in state ReceiverState::kDisconnected. The
   *              state ReceiverState::kDisconnected can be reached either with a shutdown by the remote receiver, a
   *              call to TerminateReceiver() or a call to Shutdown() for this the server.
   * \param       receiver_class_handle Handle representing the receiver class that the receiver belongs to.
   *                                    Must be valid for this instance, if the server is not already in state
   *                                    ServerState::kDisconnected.
   * \param       side_channel_ptr      Side channel to communicate with the receiver the \p receiver_id belongs to.
   *                                    Must not be a nullptr.
   *                                    The side channel must be valid for the whole lifetime of this receiver instance,
   *                                    i.e. until RemoveReceiver() was successfully called for this \p receiver_id or
   *                                    the server was destructed. This cannot be checked.
   * \return      ReceiverId as unique representation to the newly registered receiver.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         The maximum number of receivers must not be reached, i.e., CanAddReceiver() must return true, if the
   *              server is not already in state ServerState::kDisconnected.
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-ConnectionEstablishmentServer
   * \vprivate    Product Private
   */
  virtual auto AddReceiver(ClassHandle receiver_class_handle,
                           std::unique_ptr<SideChannelInterface> side_channel_ptr) noexcept
      -> ::amsr::core::Result<ReceiverId> = 0;

  /*!
   * \brief       Connect a receiver by triggering the connection handshake.
   * \details     The connection handshake will be triggered and asynchronous reception of messages is started which
   *              means that asynchronous state transitions can happen after a call to this API.
   *              After a successful connection handshake, the server includes the receiver in the send and reclaim
   *              operations. All asynchronous state changes are propagated with the callback provided to the server
   *              builder by using the configured OnReceiverStateTransitionCallback.
   * \param       receiver_id      Id representing the receiver.
   *                               Must be valid for this instance, i.e. the corresponding receiver must be added once
   *                               and not yet removed.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedReceiverState If the receiver is not
   *              in ReceiverState::kConnecting or if the receiver is in ReceiverState::kConnecting and this method has
   *              already been called before.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the communication peer with
   *              the provided \p receiver_id crashed.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the first handshake message
   *              to the receiver with the provided \p receiver_id could not be sent for any other reason.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-ConnectionEstablishmentServer
   * \vprivate    Product Private
   */
  virtual auto ConnectReceiver(ReceiverId receiver_id) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Get the connection state of a receiver.
   * \details     Note that the state may change at any time. This means that between a call to this function and a call
   *              to another function, the state may have already changed again.
   *              If the connection state of a receiver is ReceiverState::kCorrupted once, it can only change its state
   *              by a call to TerminateReceiver() or Shutdown(). If the current state of a receiver is
   *              ReceiverState::kDisconnected once, it won't change its state anymore.
   * \param       receiver_id Id representing the receiver to get the connection state for.
   *                          Must be valid for this instance, i.e. the corresponding receiver must be added once
   *                          and not yet removed.
   * \return      Connection state for the receiver with an optional error code if the current state is
   *              ReceiverState::kCorrupted or ReceiverState::kDisconnected. Possible errors are:
   *               - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the receiver closed
   *                 the connection without adhering to the protocol.
   *               - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the receiver crashed.
   *               - ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server notices a
   *                 protocol error during the communication with the receiver.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto GetReceiverState(ReceiverId const& receiver_id) const noexcept
      -> std::pair<ReceiverState, ::amsr::core::Optional<::amsr::core::ErrorCode>> = 0;

  /*!
   * \brief       Terminate the communication to a receiver.
   * \details     After the termination, the server no longer sends slots to the terminated receiver and its state is
   *              changed to ReceiverState::kDisconnected. Communication errors during the forceful termination of a
   *              receiver are ignored as this is already a corruption handling mechanism.
   *              The server is allowed to re-use any slots that the receiver held before its disconnect.
   *              WARNING: This may lead to race conditions if the disconnected receiver accesses any slots after its
   *                       removal.
   * \param       receiver_id Id representing the receiver.
   *                          Must be valid for this instance, i.e. the corresponding receiver must be connected once
   *                          and not yet removed.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedReceiverState If the receiver is
   *              in state ReceiverState::kDisconnected.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-ConnectionServerRemoveClient
   * \vprivate    Product Private
   */
  virtual auto TerminateReceiver(ReceiverId receiver_id) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Check if a receiver is currently in use.
   * \details     Note that this function always returns true if the receiver is not in state
   *              ReceiverState::kDisconnected. The state ReceiverState::kDisconnected can be reached either with a
   *              shutdown by the remote receiver, a call to TerminateReceiver() or a call to Shutdown() for this the
   *              server.
   *              Additionally, asynchronous reception of messages may currently be ongoing even after the receiver is
   *              in state ReceiverState::kDisconnected.
   *              When this function returns false once for a receiver, it will always return false for all future calls
   *              regarding the same receiver.
   * \param       receiver_id Id representing the receiver to get the usage information for.
   *                          Must be valid for this instance, i.e. the corresponding receiver must be connected once
   *                          and not yet removed.
   * \return      True if asynchronous operation for the receiver is ongoing, false otherwise.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto IsReceiverInUse(ReceiverId const& receiver_id) const noexcept -> ::amsr::core::Result<bool> = 0;

  /*!
   * \brief       Remove a receiver from this server instance.
   * \details     After a call to this function, the receiver with \p receiver_id is removed and all its related
   *              resources are freed. The \p receiver_id will be invalid for any further call to this instance.
   * \param       receiver_id Id representing the receiver to remove.
   *                          Must be valid for this instance, i.e. the corresponding receiver must be connected once
   *                          and not yet removed.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         Receiver must be in state ReceiverState::kDisconnected, if the server is not already in
   *              ServerState::kDisconnected.
   * \pre         IsReceiverInUse() must return false for \p receiver_id, if the server is not already in
   *              ServerState::kDisconnected.
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-ConnectionServerRemoveClient
   * \vprivate    Product Private
   */
  virtual auto RemoveReceiver(ReceiverId receiver_id) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Shut this server down.
   * \details     Afterwards, this server is in state ServerState::kDisconnected and all receivers perform a state
   *              transition to ReceiverState::kDisconnected, but they're not removed.
   *              All receivers are allowed to continue receiving all slots which were sent before shutdown.
   *              Asynchronous operations may still be ongoing and this can be checked with IsInUse().
   *              See ~ServerInterface() why such a check may be necessary.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kReceiverError If the server has noticed/notices
   *              a corruption during the communication with at least one receiver. The corrupted receiver(s) can be
   *              identified using the GetReceiverState() API. However, all corrupted receivers are also in state
   *              ReceiverState::kDisconnected, but with an optional error code stored.
   * \context     ANY
   * \pre         All acquired slot tokens must be returned to this instance by using UnacquireSlot() or SendSlot().
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-ConnectionServerShutdown
   * \vprivate    Product Private
   */
  virtual auto Shutdown() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Check if any asynchronous operation on this server instance is currently ongoing.
   * \details     Note that this function always returns true if Shutdown() was not called yet. Additionally,
   *              asynchronous reception of messages for any receiver may currently be ongoing even after a call to
   *              Shutdown().
   *              When this function returns false once, it will always return false for all future calls.
   * \return      True if any asynchronous operation on this server is ongoing, false otherwise.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto IsInUse() const noexcept -> bool = 0;

  /*!
   * \brief       Acquire an unused slot.
   * \details     There exists only a single, unique SlotToken for each slot.
   *              An acquired slot may be used with the AccessSlotContent(), SendSlot() and UnacquireSlot() methods.
   *              Uses the internally tracked state of slot usage as a basis to find an unused slot. If an unused slot
   *              is found, it updates the internally tracked state of slot usage appropriately.
   *              Does not perform any communication with any receiver.
   * \return      Optional containing a SlotToken representing the acquired slot if available, empty Optional otherwise.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-MessageTransfer
   * \vprivate    Product Private
   */
  virtual auto AcquireSlot() noexcept -> ::amsr::core::Result<::amsr::core::Optional<SlotToken>> = 0;

  /*!
   * \brief       Release a previously acquired slot without sending it.
   * \details     A previously acquired slot can be released to the server without sending it. Internally, this means
   *              that this slot is marked unused and can be re-acquired by calling AcquireSlot() again.
   *              Does not perform any communication with any receiver.
   * \param       token SlotToken representing the slot to release.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         The given \p token must be valid. I.e. the \p token must previously be acquired using the
   *              AcquireSlot() method and not yet sent using the SendSlot() method or released using the
   *              UnacquireSlot() method, if the server is not already in state ServerState::kDisconnected.
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-MessageTransfer
   * \vprivate    Product Private
   */
  virtual auto UnacquireSlot(SlotToken token) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Access the content of an acquired slot.
   * \details     Access is read-writable.
   * \param       token SlotToken representing the slot to access the message data for.
   * \return      Read-writable view of the message data of the slot represented by the \p token.
   *              The read-writable view is valid until the corresponding \p token is moved back to the server using
   *              SendSlot() or UnacquireSlot().
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         The given \p token must be valid. I.e. the \p token must previously be acquired using the
   *              AcquireSlot() method and not yet sent using the SendSlot() method or released using the
   *              UnacquireSlot() method, if the server is not already in state ServerState::kDisconnected.
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-MessageTransfer
   * \vprivate    Product Private
   */
  virtual auto AccessSlotContent(SlotToken const& token) noexcept
      -> ::amsr::core::Result<::amsr::core::Span<std::uint8_t>> = 0;

  /*!
   * \brief       Send a slot to all registered receivers, adhering to receiver class limits.
   * \details     This operation requires to give-up ownership of the \p token.
   *              If the server has previously noticed a protocol error during the communication with a receiver, the
   *              server does not try to perform any operation that communicates with this receiver.
   *              When sending, the receiver class limits are adhered to, i.e., if the limit of a receiver class is
   *              reached, the slot is not sent ('dropped') to all receivers belonging to this receiver class.
   *              A sent slot may be reclaimed again.
   *              This send operation must synchronize with ClientInterface::ReceiveSlot() operations referring to the
   *              same Zero-Copy instance (in either this or another process).
   *              In detail, if a server sends a slot using this method, all receivers (that the slot was sent to)
   *              belonging to the same Zero-Copy instance are able to receive the slot from the server.
   *              All communication operations between a server and its receivers must be non-blocking and appear
   *              atomic.
   *              All listening receivers which received the message are notified over the side channel. Notifications
   *              can be dropped silently due to any limitations of the side channel which don't violate the protocol.
   * \param[in]     token        SlotToken representing the slot to send.
   * \param[in,out] dropped_info Information on whether a slot was dropped for a receiver class.
   *                             Modified to reflect the receiver classes that the slot was not sent to.
   *                             Must have remaining capacity to add as many elements as the configured number of
   *                             receiver classes.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kReceiverError If the server has noticed/notices
   *              a corruption during the communication with at least one receiver. The corrupted receiver(s) can be
   *              identified using the GetReceiverState() API.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   * \pre         The given \p token must be valid. I.e. the \p token must previously be acquired using the
   *              AcquireSlot() method and not yet sent using the SendSlot() method or released using the
   *              UnacquireSlot() method, if the server is not already in state ServerState::kDisconnected.
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-MessageTransfer,
   *              DSGN-MemCon-MessageNotification
   * \vprivate    Product Private
   */
  virtual auto SendSlot(SlotToken token, DroppedInformation& dropped_info) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief       Reclaim slots that are no longer used (e.g., released) by any registered receiver.
   * \details     The server is allowed to re-use reclaimed slots.
   *              Updates the internally tracked state of slot usage.
   *              If the server has previously noticed a protocol error during the communication with a receiver, the
   *              server does not try to perform any operation that communicates with this receiver.
   *              This reclaim operation must synchronize with ClientInterface::ReleaseSlot() operations referring to
   *              the same Zero-Copy instance (in either this or another process).
   *              In detail, if a receiver releases a slot, the server belonging to the same Zero-Copy instance is able
   *              to reclaim the slot from the receivers (using this method) if the slot is no longer used by any
   *              receiver.
   *              All communication operations between a server and its receivers must be non-blocking and appear
   *              atomic.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kReceiverError If the server has noticed/notices
   *              a corruption during the communication with at least one receiver. The corrupted receiver(s) can be
   *              identified using the GetReceiverState() API.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kUnexpectedState If the server is disconnected
   *              and this call cannot be fulfilled anymore.
   * \context     ANY
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \trace       DSGN-MemCon-MessageTransfer
   * \vprivate    Product Private
   */
  virtual auto ReclaimSlots() noexcept -> ::amsr::core::Result<void> = 0;

 protected:
  /*!
   * \brief Construct a ServerInterface.
   * \vprivate Product Private
   */
  ServerInterface() noexcept = default;

  /*!
   * \brief Construct a ServerInterface by copying.
   * \vprivate Product Private
   */
  ServerInterface(ServerInterface const&) noexcept = default;

  /*!
   * \brief Construct a ServerInterface by moving.
   * \vprivate Product Private
   */
  ServerInterface(ServerInterface&&) noexcept = default;

  /*!
   * \brief Copy another ServerInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ServerInterface const&) & noexcept -> ServerInterface& = default;

  /*!
   * \brief Move another ServerInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ServerInterface&&) & noexcept -> ServerInterface& = default;
};

inline ServerInterface::~ServerInterface() noexcept = default;

}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_INTERFACE_H_
