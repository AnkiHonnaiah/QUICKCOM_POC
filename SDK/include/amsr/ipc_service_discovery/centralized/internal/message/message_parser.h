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
 *  \brief Parse buffer view and convert it to a typed message.
 *  \unit IpcServiceDiscovery::Centralized::Message
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_MESSAGE_PARSER_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_MESSAGE_PARSER_H_

#include "amsr/ipc_service_discovery/centralized/internal/message/messages.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "ara/core/result.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Parse a byte stream containing the message and convert it to the typed message.
 * \details The parser converts a byte stream, received over a SafeIpc connection, to a typed message.
 *          For every message, an abstract method must be implemented to handle the received message.
 *          The received buffer view is analyzed, verified and finally copied to the typed message.
 *          The typed message is forwarded to the user with the help of the abstract methods.
 *          So the user should inherit privately (because it is not an "is a" relation) from the
 *          message parser and implement these methods.
 * \tparam A list of types to forward to the user whenever a message has been parsed.
 * \vprivate Component internal.
 */
template <typename... Args>
class MessageParserCustomArgs {
 public:
  /*!
   * \brief Error code type for the centralized backend.
   */
  using CentralizedErrc = IpcServiceDiscoveryCentralizedErrc;

  MessageParserCustomArgs(MessageParserCustomArgs const&) = delete;
  MessageParserCustomArgs(MessageParserCustomArgs&&) noexcept = delete;
  auto operator=(MessageParserCustomArgs const&) noexcept -> MessageParserCustomArgs& = delete;
  auto operator=(MessageParserCustomArgs&&) noexcept -> MessageParserCustomArgs& = delete;

 protected:
  MessageParserCustomArgs() noexcept = default;
  ~MessageParserCustomArgs() noexcept = default;

  /*!
   * \brief Parse the given buffer view and convert it to a typed message.
   * \details The given buffer will be parsed and converted to a typed message. The abstract method
   *           for this typed message will be called and the message forwarded to the user.
   * \param[in] buffer_view A const buffer view to the message buffer.
   * \param[in] args A parameter pack of values to forward to the user whenever the message has been parsed.
   * \error IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted.
   *                                                              Message id is out of bounce.
   * \context ANY
   * \pre The buffer must contain a valid message.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   * \trace DSGN-IpcServiceDiscovery-Centralized-MessageParse
   */
  auto Parse(ConstBufferConstView buffer_view, Args... args) noexcept -> ::ara::core::Result<void> {
    ::ara::core::Result<void> result{CentralizedErrc::kDefaultError};
    BufferElementType const message_id_raw{buffer_view[Message<InitMessage>::kIdPosition]};

    if (message_id_raw == static_cast<BufferElementType>(MessageId::kOfferService)) {
      // VCA_IPCSERVICEDISCOVERY_SLC_10_SLC_22_CALLING_OWN_VIRTUAL_FUNC
      OnOfferServiceMessage(OfferServiceMessage::Deserialize(buffer_view), std::forward<Args>(args)...);
      result.EmplaceValue();
    } else if (message_id_raw == static_cast<BufferElementType>(MessageId::kStopOfferService)) {
      // VCA_IPCSERVICEDISCOVERY_SLC_10_SLC_22_CALLING_OWN_VIRTUAL_FUNC
      OnStopOfferServiceMessage(StopOfferServiceMessage::Deserialize(buffer_view), std::forward<Args>(args)...);
      result.EmplaceValue();
    } else if (message_id_raw == static_cast<BufferElementType>(MessageId::kListenService)) {
      // VCA_IPCSERVICEDISCOVERY_SLC_10_SLC_22_CALLING_OWN_VIRTUAL_FUNC
      OnListenServiceMessage(ListenServiceMessage::Deserialize(buffer_view), std::forward<Args>(args)...);
      result.EmplaceValue();
    } else if (message_id_raw == static_cast<BufferElementType>(MessageId::kStopListenService)) {
      // VCA_IPCSERVICEDISCOVERY_SLC_10_SLC_22_CALLING_OWN_VIRTUAL_FUNC
      OnStopListenServiceMessage(StopListenServiceMessage::Deserialize(buffer_view), std::forward<Args>(args)...);
      result.EmplaceValue();
    } else if (message_id_raw == static_cast<BufferElementType>(MessageId::kInit)) {
      // VCA_IPCSERVICEDISCOVERY_SLC_10_SLC_22_CALLING_OWN_VIRTUAL_FUNC
      OnInitMessage(InitMessage::Deserialize(buffer_view), std::forward<Args>(args)...);
      result.EmplaceValue();
    } else {
      result.EmplaceError(CentralizedErrc::kMessageCorrupted, "Message is not known, message id out of bounds.");
    }

    return result;
  }

 private:
  /*!
   * \brief Called if received message is an init message.
   * \param[in] init_message A reference to the result of the deserialization operation. On success,
   *                         it contains the deserialized init message. The reference is only valid
   *                         during the call of the function.
   *            Error: IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted, size of
   *                                                                         buffer view and message are not equal.
   * \param[in] args A parameter pack of values to forward to the user whenever the message has been parsed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  virtual void OnInitMessage(::ara::core::Result<InitMessage> const& init_message_result, Args... args) noexcept = 0;

  /*!
   * \brief Called if received message is an offer service message.
   * \param[in] offer_service_message A reference to the result of the deserialization operation. On success,
   *                                  it contains the deserialized offer service message. The reference is
   *                                  only valid during the call of the function.
   *            Error: IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted, size of
   *                                                                         buffer view and message are not equal.
   * \param[in] args A parameter pack of values to forward to the user whenever the message has been parsed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  virtual void OnOfferServiceMessage(::ara::core::Result<OfferServiceMessage> const& offer_service_message_result,
                                     Args... args) noexcept = 0;

  /*!
   * \brief Called if received message is a stop offer service message.
   * \param[in] stop_offer_service_message A reference to the result of the deserialization operation. On success,
   *                                       it contains the deserialized stop offer service message. The reference
   *                                       is only valid during the call of the function.
   *            Error: IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted, size of
   *                                                                         buffer view and message are not equal.
   * \param[in] args A parameter pack of values to forward to the user whenever the message has been parsed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  virtual void OnStopOfferServiceMessage(
      ::ara::core::Result<StopOfferServiceMessage> const& stop_offer_service_message_result, Args... args) noexcept = 0;

  /*!
   * \brief Called if received message is a listen service message.
   * \param[in] listen_service_message A reference to the result of the deserialization operation. On success,
   *                                   it contains the deserialized listen service message. The reference
   *                                   is only valid during the call of the function.
   *            Error: IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted, size of
   *                                                                         buffer view and message are not equal.
   * \param[in] args A parameter pack of values to forward to the user whenever the message has been parsed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  virtual void OnListenServiceMessage(::ara::core::Result<ListenServiceMessage> const& listen_service_message_result,
                                      Args... args) noexcept = 0;

  /*!
   * \brief Called if received message is a stop listen service message.
   * \param[in] stop_listen_service_message A reference to the result of the deserialization operation. On success,
   *                                        it contains the deserialized stop listen service message. The reference
   *                                        is only valid during the call of the function.
   *            Error: IpcServiceDiscoveryCentralizedErrc::kMessageCorrupted Received message is corrupted, size of
   *                                                                         buffer view and message are not equal.
   * \param[in] args A parameter pack of values to forward to the user whenever the message has been parsed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  virtual void OnStopListenServiceMessage(
      ::ara::core::Result<StopListenServiceMessage> const& stop_listen_service_message_result,
      Args... args) noexcept = 0;
};

/*!
 * \brief Default implementation of MessageParserCustomArgs without custom arguments.
 */
using MessageParser = MessageParserCustomArgs<>;

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_MESSAGE_PARSER_H_
