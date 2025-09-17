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
/*! \file
 *  \brief Abstract message for the SafeIpc communication.
 *  \unit IpcServiceDiscovery::Centralized::Message
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_BASE_MESSAGE_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_BASE_MESSAGE_H_

#include <cstring>
#include <type_traits>
#include "amsr/ipc_service_discovery/centralized/internal/ipc_service_discovery_centralized_error_domain.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/buffer_types.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/constants.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Defines the different unique messages.
 */
enum class MessageId : std::uint8_t { kInit, kOfferService, kListenService, kStopListenService, kStopOfferService };

/*!
 * \brief An abstract message for the SafeIpc communication.
 * \details The abstract message class provides services needed by all typed messages. To provide these
 *          methods and being still trivially copyable, the CRTP (The Curiously Recurring Template Pattern) pattern
 *          is used. So all derived classes must implement the payload and getter and setter for the data.
 *
 * \tparam T Type which derives from the message class and implements the CRTP pattern.
 *           Must be trivially copyable and trivially destructible. Must be copy-assignable and copy-constructible.
 *           Therefore, no form of pointer should be used in T, as it might become invalid after performing an
 *           'inter-process mem-copy'. Note that this includes e.g. pointers to global constant memory.
 *           The trivially copyable property is needed to deserialize the message by casting and deserialize the
 *           message by using e.g. memcpy. Furthermore, the messages must be binary compatible for client and server.
 * \vprivate Component internal.
 */
template <typename T>
class Message {
 public:
  /*!
   * \brief Position of the message Id inside the message buffer.
   */
  static size_t constexpr kIdPosition{0u};

  /*!
   * \brief Id to identify the message type.
   */
  using Id = MessageId;
  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  /*!
   * \brief Get the size of the typed (derived) message in bytes.
   * \return The size of the message in bytes.
   * \context ANY
   * \pre -
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
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetId() const noexcept -> Id { return id_; }

  /*!
   * \brief Serialize the message to a byte stream.
   * \details The message object is reinterpreted to a buffer view by
   *          simply casting the message object to a buffer view.
   *          No copy is performed. No memory will be allocated.
   * \return A buffer view to the serialized message. The buffer view is
   *         only valid as long as the message instance exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   * \trace DSGN-IpcServiceDiscovery-Centralized-MessageSerialization
   */
  auto Serialize() const noexcept -> ConstBufferView {
    // VECTOR NC AutosarC++17_10-A5.2.4: MD_IPCSERVICEDISCOVERY_A5.2.4_reinterpretCastShallNotBeUsed
    return ConstBufferView{reinterpret_cast<std::uint8_t const*>(GetDerived()), sizeof(T)};
  }

  /*!
   * \brief Deserialize the message from a byte stream.
   * \details The message object is deserialized by simply
   *          coping the byte stream to the message object.
   * \param[in] buffer_view A const buffer view to the message buffer.
   * \return Deserialized message.
   * \error IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted, size of
   *                                                              buffer view and message are not equal
   *                                                              or wrong MessageId.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   * \trace DSGN-IpcServiceDiscovery-Centralized-MessageSerialization
   */
  static auto Deserialize(ConstBufferConstView buffer_view) noexcept -> ::ara::core::Result<T> {
    return IsBufferSizeCorrect(buffer_view)
        .AndThen([&buffer_view]() {  // COV_IpcSD_fluentApiNoErrorReturn
          T message{};
          std::memmove(&message, buffer_view.data(), sizeof(T));
          return ::ara::core::Result<T>{message};
        })
        .AndThen([](T const& message) { return IsMessageValid(message); });
  }

 protected:
  /*!
   * \brief Construct a message.
   * \param[in] id_p A unique message identifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit Message(Id id) noexcept : id_{id} {
    // VECTOR NC AutosarC++17_10-A3.9.1: MD_IPCSERVICEDISCOVERY_A3.9.1_fixedWidthIntegerTypesShallBeUsed
    static_assert(std::is_same<std::uint8_t, unsigned char>::value,
                  "It is required std::uint8_t to be implemented as unsigned char");
    static_assert(std::is_trivially_copyable<T>::value, "Typed message must be trivially copyable");
  }

  ~Message() noexcept = default;
  Message(Message const&) = default;
  Message(Message&&) noexcept = default;
  auto operator=(Message const&) & noexcept -> Message& = default;
  auto operator=(Message&&) & noexcept -> Message& = default;

 private:
  /*!
   * \brief Get derived this pointer (const).
   * \return Derived const this pointer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  auto GetDerived() const noexcept -> T const* { return static_cast<T const*>(this); }

  /*!
   * \brief Check if buffer size is correct.
   * \details The buffer size and the message size must be equal.
   * \param[in] buffer_view A const buffer view to the message buffer.
   * \return Success if size is equal, error otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto IsBufferSizeCorrect(ConstBufferConstView buffer_view) noexcept -> ::ara::core::Result<void> {
    return (sizeof(T) == buffer_view.size())
               ? ::ara::core::Result<void>{}
               : ::ara::core::Result<void>::FromError(
                     CentralizedErrc::kMessageCorrupted,
                     "Size of buffer view is not equal to size of message type. Received message is corrupted.");
  }

  /*!
   * \brief Check if the deserialized message is valid.
   * \details Checks if each member of the Message is valid.
   * \param[in] message A const reference to the deserialized message.
   * \return Success if message is valid, error otherwise.
   * \error IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Invalid payload.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  static auto IsMessageValid(T const& message) noexcept -> ::ara::core::Result<T> {
    // VCA_IPCSERVICEDISCOVERY_SPC_15_AMSR_CORE_RESULT_FULFILLS_FUNCTION_CONTRACT
    // VCA_IPCSERVICEDISCOVERY_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
    return message.IsValid() ? ::ara::core::Result<T>{message}
                             : ::ara::core::Result<T>::FromError(CentralizedErrc::kMessageCorrupted,
                                                                 "Message is invalid. Received message is corrupted.");
  }

  /*! Unique message id. */
  Id id_{};
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_fluentApiNoErrorReturn
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the fluent API AndThen.
//            This is true, since this lambda can only return a result with a value but no error. Therefore, the error
//            case is untestable. It is verified by review that the error case cannot occur.
// COV_JUSTIFICATION_END

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_BASE_MESSAGE_H_
