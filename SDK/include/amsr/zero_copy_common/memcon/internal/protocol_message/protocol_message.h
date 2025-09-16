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
/*!       \file
 *        \brief Abstract message for the side channel communication.
 *        \unit ZeroCopyCommon::MemCon::ProtocolMessage
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGE_H_

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message_types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace protocol_message {

/*!
 * \brief Defines the different unique messages.
 */
enum class ProtocolMessageId : std::uint8_t {
  kConnectionRequestSlotMemory,
  kConnectionRequestQueueMemory,
  kAckConnection,
  kAckQueueInitialization,
  kStartListening,
  kStopListening,
  kShutdown,
  kTermination
};

/*!
 * \brief An abstract protocol message for the side channel communication.
 * \details The abstract protocol message class provides services needed by all typed protocol messages. To provide
 *          these methods and being still trivially copyable the CRTP (The Curiously Recurring Template Pattern)
 *          pattern is used. So all derived classes must implement the payload, getter and setter for the data.
 * \tparam  T Type which derives from the ProtocolMessage class and implements the CRTP pattern.
 *          Must be trivially copyable and trivially destructible. Must be copy assignable and copy constructible.
 *          Therefore, no form of pointer should be used in T, as it might become invalid after performing an
 *          'inter-process mem-copy'. Note that this includes e.g. pointers to global constant memory.
 *          The trivially copyable property is needed to deserialize the message by casting and deserialize the
 *          message by using e.g. memcpy. Furthermore, the messages must be binary compatible for client and server.
 * \trace   DSGN-MemCon-SideChannelProtocol
 * \vprivate Component internal.
 */
template <typename T>
class ProtocolMessage {
 public:
  /*!
   * \brief Type used for protocol message id.
   */
  using Id = ProtocolMessageId;

  /*!
   * \brief Type used for ZeroCopyCommon error code.
   */
  using Errc = ::amsr::zero_copy_common::common::ZeroCopyCommonErrc;

  /*!
   * \brief Get the size of the typed (derived) message in bytes.
   * \return The size of the message in bytes.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static constexpr auto GetSize() noexcept -> size_t { return sizeof(T); }

  /*!
   * \brief Get the id of the message in bytes.
   * \return The unique id of the message.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetId() const noexcept -> Id { return id_; }

  /*!
   * \brief Serialize the message to a byte stream.
   * \details The message object is reinterpreted to a message view by
   *          simply casting the message object to a message view.
   *          No copy is performed. No memory will be allocated.
   * \return  A message view to the serialized message. The message view is
   *          only valid as long as the message instance exists.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto Serialize() const noexcept -> ConstMessageView {
    // VECTOR Next Construct AutosarC++17_10-A5.2.4: MD_ZEROCOPYCOMMON_A5.2.4_reinterpret_cast_message_view
    return ConstMessageConstView{reinterpret_cast<std::uint8_t const*>(GetDerived()), sizeof(T)};
  }

  /*!
   * \brief Deserialize the message from a byte stream.
   * \details The message object is deserialized by simply
   *          coping the byte stream to the message object.
   * \param[in] message_view A const message view to the message buffer.
   * \return Deserialized message.
   * \error ZeroCopyCommonErrc::kMessageCorrupted Received message is corrupted, size of
   *        message view and message are not equal.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto Deserialize(ConstMessageConstView message_view) noexcept -> ::amsr::core::Result<T> {
    ::amsr::core::Result<T> result{Errc::kDefaultError};

    if (IsBufferSizeCorrect(message_view)) {
      result.EmplaceError(Errc::kMessageCorrupted, "Size of buffer view does not fit. Received message is corrupted.");
    } else {
      T message{};
      std::memmove(&message, message_view.data(), sizeof(T));
      result.EmplaceValue(message);
    }
    return result;
  }

 protected:
  /*!
   * \brief Construct a message.
   * \param[in] id_p A unique message identifier.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   *
   * \internal
   * - Statically assert if the std::uint8_t is implemented same as unsigned char.
   * - Statically assert if Typed message is trivially copyable.
   * - Statically assert if Typed message is derived from the ProtocolMessage.
   * \endinternal
   */
  constexpr explicit ProtocolMessage(Id id) noexcept : id_{id} {
    // VECTOR NC AutosarC++17_10-A3.9.1: D_ZEROCOPYCOMMON_A3.9.1_basic_type_check
    static_assert(std::is_same<std::uint8_t, unsigned char>::value,
                  "It is required std::uint8_t to be implemented as unsigned char.");
    static_assert(std::is_trivially_copyable<T>::value, "Typed message must be trivially copyable.");
    static_assert(std::is_base_of<ProtocolMessage<T>, T>::value, "Typed message must be derived from ProtocolMessage.");
  }

  /*!
   * \brief Destroy a ProtocolMessage.
   * \vprivate Component Private
   */
  ~ProtocolMessage() noexcept = default;

  /*!
   * \brief Create a copy of a ProtocolMessage.
   * \vprivate Component Private
   */
  ProtocolMessage(ProtocolMessage const&) = default;

  /*!
   * \brief Move a ProtocolMessage into a new instance.
   * \vprivate Component Private
   */
  ProtocolMessage(ProtocolMessage&&) noexcept = default;

  /*!
   * \brief Copy a different ProtocolMessage into this instance.
   * \vprivate Component Private
   */
  auto operator=(ProtocolMessage const&) & noexcept -> ProtocolMessage& = default;

  /*!
   * \brief Move a different ProtocolMessage into this instance.
   * \vprivate Component Private
   */
  auto operator=(ProtocolMessage&&) & noexcept -> ProtocolMessage& = default;

 private:
  /*!
   * \brief Get derived this pointer (const).
   * \return Derived const this pointer.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto GetDerived() const noexcept -> T const* { return static_cast<T const*>(this); }

  /*!
   * \brief Check if buffer size is correct.
   * \details The buffer size and the message size must be equal.
   * \param[in] message_view A const message view to the message buffer.
   * \return True if size is equal, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto IsBufferSizeCorrect(ConstMessageConstView message_view) noexcept -> bool {
    return sizeof(T) != message_view.size();
  }

  /*!
   * \brief Unique message id.
   */
  Id id_{};
};

}  // namespace protocol_message
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_PROTOCOL_MESSAGE_H_
