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
 *        \brief  Declaration of interface ClientInterface.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_INTERFACE_H_

#include <cstdint>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"

#include "amsr/zero_copy_common/logic/internal/slot_token.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Interface of a client.
 * \details A client offers to receive a slot, access its content and release the slot.
 * \vprivate Component Private
 */
class ClientInterface {
 public:
  /*!
   * \brief Destruct the ClientInterface.
   * \vprivate Component Private
   */
  virtual ~ClientInterface() noexcept = 0;

  /*!
   * \brief Receive a slot that was sent from the server.
   * \details A received slot may be used with the AccessSlotContent and ReleaseSlot methods.
   *          If the client has previously noticed a protocol error during the communication with the server, the client
   *          does not try to perform any operation that communicates with the server.
   *          See the CAD for the specification of the receive sequence regarding the usage of shared memory
   *          communication facilities.
   *          This receive operation must synchronize with ServerInterface::SendSlot() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, after the server has sent a slot by calling ServerInterface::SendSlot(), all clients belonging
   *          to the same Zero-Copy instance must be able to receive the sent slot.
   *          Note that either all clients that are not rate-limited are able to receive a slot or none is.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   * \return Optional containing a SlotToken representing the received slot if available, empty Optional otherwise.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client has noticed/notices a
   *        protocol error during the communication with the server. Duplicate reception of a slot is treated as a
   *        protocol error.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferClientReceiveSlot
   * \vprivate Component Private
   */
  virtual auto ReceiveSlot() noexcept -> amsr::core::Result<amsr::core::Optional<SlotToken>> = 0;

  /*!
   * \brief Access the content of a received slot.
   * \details Access is read-only.
   * \param token SlotToken representing the slot to access the message data for.
   * \return Read-only view of the message data of the slot represented by the \p token.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferClientReceiveSlot
   * \vprivate Component Private
   */
  virtual auto AccessSlotContent(SlotToken const& token) const noexcept -> ::amsr::core::Span<std::uint8_t const> = 0;

  /*!
   * \brief Release a previously received slot to the server.
   * \details A released slot may be received again.
   *          This operation requires to give-up ownership of the \p token.
   *          If the client has previously noticed a protocol error during the communication with the server, the client
   *          does not try to perform any operation that communicates with the server.
   *          See the CAD for the specification of the release sequence regarding the usage of shared memory
   *          communication facilities.
   *          This release operation must synchronize with ServerInterface::ReclaimSlots() operations referring to the
   *          same Zero-Copy instance (in either this or another process).
   *          In detail, if a client releases a slot using this method, the server belonging to the same Zero-Copy
   *          instance is able to reclaim the slot from the client.
   *          All communication operations between a server and its clients must be non-blocking and appear atomic.
   * \param token SlotToken representing the slot to release.
   * \error amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the client has noticed/notices a
   *        protocol error during the communication with the server.
   * \context ANY
   * \pre The given \p token must be valid. I.e. the \p token must previously be acquired using the ReceiveSlot() method
   *      and not yet returned using the ReleaseSlot() method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferClientReleaseSlot
   * \vprivate Component Private
   */
  virtual auto ReleaseSlot(SlotToken token) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief Inform the client that the communication with the server is erroneous.
   * \details Afterwards, the client does not try to perform any operation that communicates with the server.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace DSGN-ZeroCopyCommon-MessageTransferClientConnectionState
   * \vprivate Component Private
   */
  virtual void SetCommunicationError() noexcept = 0;

 protected:
  /*!
   * \brief Construct a ClientInterface.
   * \vprivate Component Private
   */
  ClientInterface() noexcept = default;

  /*!
   * \brief Construct a ClientInterface by copying.
   * \vprivate Component Private
   */
  ClientInterface(ClientInterface const&) noexcept = default;

  /*!
   * \brief Construct a ClientInterface by moving.
   * \vprivate Component Private
   */
  ClientInterface(ClientInterface&&) noexcept = default;

  /*!
   * \brief Copy another ClientInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientInterface const&) & noexcept -> ClientInterface& = default;

  /*!
   * \brief Move another ClientInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientInterface&&) & noexcept -> ClientInterface& = default;
};

inline ClientInterface::~ClientInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_INTERFACE_H_
