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
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_INTERFACE_H_

#include <cstdint>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "vac/container/static_vector.h"

#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/receiver_connection_state.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_token.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Interface of a server.
 * \details A server offers to register receivers, remove receivers, reclaim slots from receivers, acquire (reserve) a
 *          slot to write data to, access its content (writable) and send the slot to registered receivers.
 *          Note: The terms 'receiver' and 'client' are used interchangeably. The term 'receiver' is used where it's
 *                instructive to emphasize that a description refers to the server-side representation of a client.
 * \vprivate Component Private
 */
class ServerInterface {
 public:
  /*!
   * \brief Type to hold information on whether a slot was dropped for a receiver class.
   */
  using DroppedInformation = ::vac::container::StaticVector<ClassHandle>;

  /*!
   * \brief Destruct the ServerInterface.
   * \vprivate Component Private
   */
  virtual ~ServerInterface() noexcept = 0;

  /*!
   * \brief Check if an additional receiver can be registered.
   * \return True, if an additional receiver can be registered. False, otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-ConnectionEstablishmentServer
   * \vprivate Component Private
   */
  virtual auto CanRegisterReceiver() const noexcept -> bool = 0;

  /*!
   * \brief Check if the class handle is valid for this instance.
   * \param class_handle Handle representing a receiver class.
   * \return True, if the class handle is valid for this instance. False, otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-ConnectionEstablishmentServer
   * \vprivate Component Private
   */
  virtual auto IsClassHandleValid(ClassHandle const& class_handle) const noexcept -> bool = 0;

  /*!
   * \brief Register a new receiver.
   * \details A receiver consists of two queue ends and a receiver class for limiting slot usage.
   *          After registration, the server includes the receiver in the send and reclaim operations.
   * \param receiver_class_handle Handle representing the receiver class that the receiver belongs to.
   *                              Must be valid for this instance.
   * \param free_queue_ptr Readable free queue of the receiver.
   *                       Must not contain nullptr.
   *                       Must point to valid objects. This cannot be checked.
   *                       Must provide valid implementations of the respective base class. This cannot be checked.
   * \param available_queue_ptr Writable available queue of the receiver.
   *                            Must not contain nullptr.
   *                            Must point to valid objects. This cannot be checked.
   *                            Must provide valid implementations of the respective base class. This cannot be checked.
   * \return Handle representing the newly registered receiver.
   * \context ANY
   * \pre The maximum number of receivers must not be reached, i.e., CanRegisterReceiver() must return true.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-ConnectionEstablishmentServer
   * \vprivate Component Private
   */
  virtual auto RegisterReceiver(ClassHandle const& receiver_class_handle,
                                std::unique_ptr<ReadingQueueEndInterface> free_queue_ptr,
                                std::unique_ptr<WritingQueueEndInterface> available_queue_ptr) noexcept
      -> ReceiverHandle = 0;

  /*!
   * \brief Get the connection state of a receiver.
   * \param receiver_handle Handle representing the receiver to get the connection state for.
   * \return Optional containing the connection state for the receiver. Empty Optional, if the \p receiver_handle does
   *         not represent a registered receiver.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerConnectionState
   * \vprivate Component Private
   */
  virtual auto GetReceiverConnectionState(ReceiverHandle const& receiver_handle) const noexcept
      -> ::amsr::core::Optional<ReceiverConnectionState> = 0;

  /*!
   * \brief Remove a receiver.
   * \details After removal, the server no longer sends slots to the removed receiver.
   *          The server is allowed to re-use any slots that the receiver held before its removal.
   *          See the CAD for the specification of the remove receiver sequence regarding the usage of shared memory
   *          communication facilities.
   *          WARNING: This may lead to race conditions if the removed receiver accesses any slots after its removal.
   * \param receiver_handle Handle representing the receiver to remove.
   * \return True, if the receiver was removed. False, if the \p receiver_handle does not represent a registered
   *         receiver.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-ConnectionTerminationServer
   * \vprivate Component Private
   */
  virtual auto RemoveReceiver(ReceiverHandle const& receiver_handle) noexcept -> bool = 0;

  /*!
   * \brief Acquire an unused slot.
   * \details There exists only a single, unique SlotToken for each slot.
   *          An acquired slot may be used with the AccessSlotContent(), SendSlot() and UnacquireSlot() methods.
   *          Uses the internally tracked state of slot usage as a basis to find an unused slot. If an unused slot is
   *          found, it updates the internally tracked state of slot usage appropriately.
   *          Does not perform any communication with any client.
   * \return Optional containing a SlotToken representing the acquired slot if available, empty Optional otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerAcquireSlot
   * \vprivate Component Private
   */
  virtual auto AcquireSlot() noexcept -> ::amsr::core::Optional<SlotToken> = 0;

  /*!
   * \brief Release a previously acquired slot without sending it.
   * \details A previously acquired slot can be released to the server without sending it. Internally, this means that
   *          this slot is marked unused and can be re-acquired by calling AcquireSlot() again.
   *          Does not perform any communication with any client.
   * \param token SlotToken representing the slot to release.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the AcquireSlot() method
   *      and not yet sent using the SendSlot() method or released using the UnacquireSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerUnacquireSlot
   * \vprivate Component Private
   */
  virtual void UnacquireSlot(SlotToken token) noexcept = 0;

  /*!
   * \brief Access the content of an acquired slot.
   * \details Access is read-writable.
   * \param token SlotToken representing the slot to access the message data for.
   * \return Read-writable view of the message data of the slot represented by the \p token.
   *         The read-writable view is valid until the corresponding \p token is moved back to the server using
   *         SendSlot() or UnacquireSlot().
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the AcquireSlot() method
   *      and not yet sent using the SendSlot() method or released using the UnacquireSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerSendSlot
   * \vprivate Component Private
   */
  virtual auto AccessSlotContent(SlotToken const& token) noexcept -> ::amsr::core::Span<std::uint8_t> = 0;

  /*!
   * \brief Send a slot to all registered receivers, adhering to receiver class limits.
   * \details This operation requires to give-up ownership of the \p token.
   *          If the server has previously noticed a protocol error during the communication with a client, the server
   *          does not try to perform any operation that communicates with this client.
   *          When sending, the receiver class limits are adhered to, i.e., if the limit of a receiver class is reached,
   *          the slot is not sent ('dropped') to all receivers belonging to this receiver class.
   *          See the CAD for the specification of the send slot sequence regarding the usage of shared memory
   *          communication facilities.
   *          A sent slot may be reclaimed again.
   *          This send operation must synchronize with ClientInterface::ReceiveSlot() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, if a server sends a slot using this method, all clients (that the slot was sent to) belonging
   *          to the same Zero-Copy instance are able to receive the slot from the server.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   * \param[in] token SlotToken representing the slot to send.
   * \param[in,out] dropped_info Information on whether a slot was dropped for a receiver class.
   *                             Modified to reflect the receiver classes that the slot was not sent to.
   *                             Must have remaining capacity to add as many elements as the configured number of
   *                             receiver classes.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server has noticed/notices a
   *        protocol error during the communication with any client.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the AcquireSlot() method
   *      and not yet sent using the SendSlot() method or released using the UnacquireSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerSendSlot
   * \vprivate Component Private
   */
  virtual auto SendSlot(SlotToken token, DroppedInformation& dropped_info) noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Reclaim slots that are no longer used (e.g., released) by any registered receiver.
   * \details The server is allowed to re-use reclaimed slots.
   *          Updates the internally tracked state of slot usage.
   *          If the server has previously noticed a protocol error during the communication with a client, the server
   *          does not try to perform any operation that communicates with this client.
   *          See the CAD for the specification of the reclaim slots sequence regarding the usage of shared memory
   *          communication facilities.
   *          This reclaim operation must synchronize with ClientInterface::ReleaseSlot() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, if a client releases a slot, the server belonging to the same Zero-Copy instance is able to
   *          reclaim the slot from the clients (using this method) if the slot is no longer used by any client.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server has noticed/notices a
   *        protocol error during the communication with any client.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerReclaimSlots
   * \vprivate Component Private
   */
  virtual auto ReclaimSlots() noexcept -> ::amsr::core::Result<void> = 0;

  /*!
   * \brief Inform the server that the communication with one receiver is erroneous.
   * \details Afterwards, the server does not try to perform any operation that communicates with this receiver.
   * \param receiver_handle Handle representing the receiver to set the communication error.
   * \return True, if the communication was set successfully. False, if the \p receiver_handle does not represent a
   *         registered receiver.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferServerConnectionState
   * \vprivate Component Private
   */
  virtual auto SetCommunicationError(ReceiverHandle const& receiver_handle) noexcept -> bool = 0;

 protected:
  /*!
   * \brief Construct a ServerInterface.
   * \vprivate Component Private
   */
  ServerInterface() noexcept = default;

  /*!
   * \brief Construct a ServerInterface by copying.
   * \vprivate Component Private
   */
  ServerInterface(ServerInterface const&) noexcept = default;

  /*!
   * \brief Construct a ServerInterface by moving.
   * \vprivate Component Private
   */
  ServerInterface(ServerInterface&&) noexcept = default;

  /*!
   * \brief Copy another ServerInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerInterface const&) & noexcept -> ServerInterface& = default;

  /*!
   * \brief Move another ServerInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerInterface&&) & noexcept -> ServerInterface& = default;
};

inline ServerInterface::~ServerInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_INTERFACE_H_
