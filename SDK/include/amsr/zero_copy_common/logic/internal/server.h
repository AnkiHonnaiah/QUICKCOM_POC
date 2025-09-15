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
 *        \brief  Definition of class Server.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_H_

#include <cstdint>
#include <memory>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "vac/container/static_vector.h"

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/logic/internal/borrowed_manager.h"
#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/receiver.h"
#include "amsr/zero_copy_common/logic/internal/receiver_connection_state.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_limit.h"
#include "amsr/zero_copy_common/logic/internal/receiver_manager.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_manager.h"
#include "amsr/zero_copy_common/logic/internal/slot_token.h"
#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Implementation of ServerInterface.
 * \details A server offers to register receivers, remove receivers, reclaim slots from receivers, acquire (reserve) a
 *          slot to write data to, access its content (writable) and send the slot to registered receivers.
 *          Note: The terms 'receiver' and 'client' are used interchangeably. The term 'receiver' is used where it's
 *                instructive to emphasize that a description refers to the server-side representation of a client.
 * \vprivate Component Private
 */
class Server final : public ServerInterface {
 public:
  /*!
   * \brief Construct a Server.
   * \details Intended to be used by the respective builder only.
   * \param receiver_manager ReceiverManager used by this instance.
   * \param borrowed_manager BorrowedManager used by this instance.
   * \param slot_manager SlotManager used by this instance.
   * \context ANY
   * \pre All managers must be configured with the same parameters where applicable. This cannot be checked.
   * \pre All managers must be in a consistent state with respect to each other. This cannot be checked.
   * \pre All slots must be reset before calling this method. This cannot be checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  /*!
   * \internal
   * - Save all managers.
   * - Reserve memory for the internally used buffer for slot handles (used to temporarily store slot handles in the
   *   RemoveReceiver member function) in order to support the steady mode when removing receivers.
   * \endinternal
   */
  Server(ReceiverManager receiver_manager, BorrowedManager borrowed_manager,
         SlotManager<WritableSlotDescriptor> slot_manager) noexcept;

  /*!
   * \brief Do not construct a Server by copying.
   * \vprivate Component Private
   */
  Server(Server const&) noexcept = delete;

  /*!
   * \brief Construct a Server by moving.
   * \vprivate Component Private
   */
  Server(Server&&) noexcept = default;

  /*!
   * \brief Do not copy another Server into this instance.
   * \vprivate Component Private
   */
  auto operator=(Server const&) noexcept -> Server& = delete;

  /*!
   * \brief Move another Server into this instance.
   * \vprivate Component Private
   */
  auto operator=(Server&&) & noexcept -> Server& = default;

  /*!
   * \brief Destruct the Server.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  // VCA_ZEROCOPYCOMMON_SPC_15_LOGIC_SERVER_VECTOR_DESTRUCTOR_FUNCTION_CONTRACT
  ~Server() noexcept final = default;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::CanRegisterReceiver
   */
  auto CanRegisterReceiver() const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::IsClassHandleValid
   */
  auto IsClassHandleValid(ClassHandle const& class_handle) const noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::RegisterReceiver
   */
  auto RegisterReceiver(ClassHandle const& receiver_class_handle,
                        std::unique_ptr<ReadingQueueEndInterface> free_queue_ptr,
                        std::unique_ptr<WritingQueueEndInterface> available_queue_ptr) noexcept -> ReceiverHandle final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::GetReceiverConnectionState
   */
  auto GetReceiverConnectionState(ReceiverHandle const& receiver_handle) const noexcept
      -> ::amsr::core::Optional<ReceiverConnectionState> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::RemoveReceiver
   */
  auto RemoveReceiver(ReceiverHandle const& receiver_handle) noexcept -> bool final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::AcquireSlot
   */
  auto AcquireSlot() noexcept -> ::amsr::core::Optional<SlotToken> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::UnacquireSlot
   */
  void UnacquireSlot(SlotToken token) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::AccessSlotContent
   */
  auto AccessSlotContent(SlotToken const& token) noexcept -> ::amsr::core::Span<std::uint8_t> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::SendSlot
   */
  auto SendSlot(SlotToken token, DroppedInformation& dropped_info) noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::ReclaimSlots
   */
  auto ReclaimSlots() noexcept -> ::amsr::core::Result<void> final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerInterface::SetCommunicationError
   */
  auto SetCommunicationError(ReceiverHandle const& receiver_handle) noexcept -> bool final;

 private:
  /*!
   * \brief Container used to store slot handles.
   */
  using SlotHandleBuffer = ::vac::container::StaticVector<SlotHandle>;

  /*!
   * \brief Type used for slot indices.
   */
  using SlotIndex = SlotHandle::SlotIndex;

  /*!
   * \brief Type used for optional slot indices.
   */
  using OptSlotIndex = ::amsr::core::Optional<SlotIndex>;

  /*!
   * \brief Type used for optional slot handles.
   */
  using OptSlotHandle = ::amsr::core::Optional<SlotHandle>;

  /*!
   * \brief Type used for optional slot tokens.
   */
  using OptSlotToken = ::amsr::core::Optional<SlotToken>;

  /*!
   * \brief Send a slot to a single receiver.
   * \details Pushes to the available queue of the receiver.
   * \param receiver Receiver to send the slot to.
   * \param index Index representing the slot to send.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server notices a protocol error
   *        during the communication with the client.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SendSlotToReceiver(Receiver const& receiver, SlotIndex index) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Reclaim slots that are no longer used for a single registered receiver.
   * \details Pops the free queue of the receiver until it is empty or a protocol error occurs.
   *          Updates the internally tracked state of slot usage.
   *          Optionally clears the GVF for each slot that is no longer used by any receiver.
   * \param receiver Receiver to reclaim slots for.
   *                 Must be valid for this instance.
   * \error ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the server notices a protocol error
   *        during the communication with the client.
   * \context ANY
   * \pre The given \p receiver must be registered to this instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ReclaimSlotsForReceiver(Receiver const& receiver) noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Check whether a slot is no longer used by anyone and clear the slot's GVF if that is the case.
   * \param slot_handle Handle representing the slot to optionally clear the GVF for.
   *                    Must be valid for this instance.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void ClearFlagIfUnused(SlotHandle const& slot_handle) noexcept;

  /*!
   * \brief Assert that the SlotToken is valid for this instance.
   * \param token SlotToken to be checked.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the AcquireSlot() method
   *      and not yet sent using the SendSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertSlotTokenValid(SlotToken const& token) const noexcept;

  /*!
   * \brief The ReceiverManager of this instance.
   */
  ReceiverManager receiver_manager_;

  /*!
   * \brief The BorrowedManager of this instance.
   */
  BorrowedManager borrowed_manager_;

  /*!
   * \brief The SlotManager of this instance.
   */
  SlotManager<WritableSlotDescriptor> slot_manager_;

  /*!
   * \brief The SlotHandleBuffer used to temporarily store slot handles during the RemoveReceiver member function.
   * \details The buffer is a data member instead of a local variable in order to support the steady mode
   *          (no allocations) during the removal of a receiver.
   *          The current implementation of the RemoveReceiver member function uses the buffer to temporarily store
   *          slot handles in order to implement clearing GVFs more efficiently, i.e., limiting the number of
   *          accesses to shared memory communication facilities.
   */
  SlotHandleBuffer remove_receiver_slots_to_check_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_H_
