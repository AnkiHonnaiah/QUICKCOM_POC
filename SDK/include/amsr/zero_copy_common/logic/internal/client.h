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
 *        \brief  Definition of class Client.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/logic/internal/client_interface.h"
#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/sender_connection_state.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_manager.h"
#include "amsr/zero_copy_common/logic/internal/slot_token.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Implementation of ClientInterface.
 * \details A client offers to receive a slot, access its content and release the slot.
 * \vprivate Component Private
 */
class Client final : public ClientInterface {
 public:
  /*!
   * \brief Construct a Client.
   * \details Intended to be used by the respective builder only.
   * \param slot_manager SlotManager used by this instance.
   * \param free_queue_ptr Writable free queue used by this instance.
   *                       Must not contain nullptr.
   *                       Must point to valid objects. This cannot be checked.
   *                       Must provide valid implementations of the respective base class. This cannot be checked.
   * \param available_queue_ptr Readable available queue used by this instance.
   *                            Must not contain nullptr.
   *                            Must point to valid objects. This cannot be checked.
   *                            Must provide valid implementations of the respective base class. This cannot be checked.
   * \context ANY
   * \pre The queue implementations and the slot implementations must properly synchronize with each other.
   *      This cannot be checked. See the CAD for more details.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-ConnectionEstablishmentClient
   * \vprivate Component Private
   */
  Client(SlotManager<ReadableSlotDescriptor>&& slot_manager, std::unique_ptr<WritingQueueEndInterface> free_queue_ptr,
         std::unique_ptr<ReadingQueueEndInterface> available_queue_ptr) noexcept;

  /*!
   * \brief Do not construct a Client by copying.
   * \vprivate Component Private
   */
  Client(Client const&) noexcept = delete;

  /*!
   * \brief Construct a Client by moving.
   * \vprivate Component Private
   */
  Client(Client&&) noexcept = default;

  /*!
   * \brief Do not copy another Client into this instance.
   * \vprivate Component Private
   */
  auto operator=(Client const&) noexcept -> Client& = delete;

  /*!
   * \brief Move another Client into this instance.
   * \vprivate Component Private
   */
  auto operator=(Client&&) & noexcept -> Client& = default;

  /*!
   * \brief Destruct the Client.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-ConnectionTerminationClient
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SPC_15_DESTRUCTOR_UNIQUE_PTR_MEMBER_PASSED_IN_CONSTRUCTOR
  // VCA_ZEROCOPYCOMMON_SPC_15_DESTRUCTOR_UNIQUE_PTR_MEMBER_PASSED_IN_CONSTRUCTOR
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~Client() noexcept final = default;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientInterface::ReceiveSlot
   */
  auto ReceiveSlot() noexcept -> amsr::core::Result<amsr::core::Optional<SlotToken>> final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientInterface::AccessSlotContent
   */
  auto AccessSlotContent(SlotToken const& token) const noexcept -> ::amsr::core::Span<std::uint8_t const> final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientInterface::ReleaseSlot
   */
  auto ReleaseSlot(SlotToken token) noexcept -> amsr::core::Result<void> final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientInterface::SetCommunicationError
   */
  void SetCommunicationError() noexcept final;

 private:
  /*!
   * \brief Type used for slot indices.
   */
  using SlotIndex = SlotHandle::SlotIndex;

  /*!
   * \brief Type used for optional slot indices.
   */
  using OptSlotIndex = amsr::core::Optional<SlotIndex>;

  /*!
   * \brief Type used for optional slot handles.
   */
  using OptSlotHandle = amsr::core::Optional<SlotHandle>;

  /*!
   * \brief Type used for optional slot tokens.
   */
  using OptSlotToken = amsr::core::Optional<SlotToken>;

  /*!
   * \brief Get the SlotToken for a peeked SlotIndex.
   * \details Pops the available queue if the respective slot is visible.
   * \param index_peek Peeked SlotIndex.
   * \return Optional containing the SlotToken representing the peeked SlotIndex if the respective slot is visible,
   *         empty Optional otherwise.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSlotTokenForPeekedSlotIndexIfVisible(SlotHandle::SlotIndex index_peek) noexcept
      -> amsr::core::Result<amsr::core::Optional<SlotToken>>;

  /*!
   * \brief Get the current visibility state of a slot represented by a SlotHandle.
   * \param handle SlotHandle representing the slot to get the current visibility state for.
   *               Must be valid for this instance.
   * \return True, if the slot is visible. False otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsSlotVisible(SlotHandle const& handle) noexcept -> bool;

  /*!
   * \brief Check whether the Result of a pop operation is equal to the previously peeked SlotIndex.
   * \param result_pop Result of a pop operation.
   * \param index_peek Previously peeked SlotIndex.
   * \return True, if the Result of the pop operation is equal to the previously peeked SlotIndex. False otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsQueuePopValid(amsr::core::Result<amsr::core::Optional<SlotHandle::SlotIndex>> const& result_pop,
                              SlotHandle::SlotIndex index_peek) noexcept -> bool;

  /*!
   * \brief Get the SlotToken for a SlotHandle.
   * \param handle SlotHandle to get the SlotToken for.
   *               Must be valid for this instance.
   * \return Optional containing the SlotToken for the SlotHandle. Never an empty Optional.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client notices a protocol error
   *        during the communication with the server.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSlotTokenForSlotHandle(SlotHandle const& handle) noexcept
      -> amsr::core::Result<amsr::core::Optional<SlotToken>>;

  /*!
   * \brief Assert that the SlotToken is valid for this instance.
   * \param token SlotToken to be checked.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertSlotTokenValid(SlotToken const& token) const noexcept;

  /*!
   * \brief The SenderConnectionState that the Client has detected.
   */
  SenderConnectionState sender_connection_state_{SenderConnectionState::kOk};

  /*!
   * \brief The SlotManager of this instance.
   */
  SlotManager<ReadableSlotDescriptor> slot_manager_;

  /*!
   * \brief The free queue of this instance.
   */
  std::unique_ptr<WritingQueueEndInterface> free_queue_ptr_;

  /*!
   * \brief The available queue of this instance.
   */
  std::unique_ptr<ReadingQueueEndInterface> available_queue_ptr_;

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, Construction);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPeekError);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPeekEmpty);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPeekInvalidIndex);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPeekInvisible);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPopError);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPopEmpty);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotPopDifferentIndex);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlot);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotDuplicate);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, ReceiveSlotNoDuplicateAfterReleaseSlot);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, AccessSlotContentAbortTokenInvalid);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client, AccessSlotContent);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, ReleaseSlotAbortTokenInvalid);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, ReleaseSlotPushError);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, ReleaseSlotPushFull);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, ReleaseSlot);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, ReceiveSlotCorruptedStateBehavior);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, AccessSlotContentCorruptedStateBehavior);

  /*! Friend test declaration */
  FRIEND_TEST(UT__Client__Client__DeathTest, ReleaseSlotCorruptedStateBehavior);
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_H_
