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
/*!        \file
 *        \brief  Definition of IPC binding trace interface and parameters.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_TRACE_TRACE_IF_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_TRACE_TRACE_IF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstdint>

#include "ipc_binding_core/internal/ipc_protocol/ipc_packet.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace trace {

/*!
 * \brief Represents a IPC message.
 */
using PacketSpan = ipc_protocol::PacketSpan;

/*!
 * \brief The direction of the traced message.
 */
enum class MessageDirection : std::uint8_t {
  /*!
   * \brief Message was received.
   */
  kRx = 0U,
  /*!
   * \brief Message will be transmitted.
   */
  kTx
};

/*!
 * \brief Parameter container for a method call.
 */
struct IpcMethodCall {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The method id.
   */
  std::uint32_t method_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
  /*!
   * \brief The session id.
   */
  std::uint32_t session_id_{};
  /*!
   * \brief Read only span into the related IPC packet.
   */
  PacketSpan packet_{};
};

/*!
 * \brief Parameter container for a fire&forget method call.
 */
struct IpcMethodNoReturnCall {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The method id.
   */
  std::uint32_t method_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
  /*!
   * \brief The session id.
   */
  std::uint32_t session_id_{};
  /*!
   * \brief Read only span into the related IPC packet.
   */
  PacketSpan packet_{};
};

/*!
 * \brief Parameter container for a method response.
 */
struct IpcMethodResponse {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The method id.
   */
  std::uint32_t method_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
  /*!
   * \brief The session id.
   */
  std::uint32_t session_id_{};
  /*!
   * \brief Read only span into the related IPC packet.
   */
  PacketSpan packet_{};
};

/*!
 * \brief Parameter container for a method error response.
 */
struct IpcMethodErrorResponse {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The method id.
   */
  std::uint32_t method_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
  /*!
   * \brief The session id.
   */
  std::uint32_t session_id_{};
  /*!
   * \brief The return code.
   */
  std::uint32_t return_code_{};
  /*!
   * \brief Read only span into the related IPC packet.
   */
  PacketSpan packet_{};
};

/*!
 * \brief Parameter container for a notification.
 */
struct IpcNotification {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The event id.
   */
  std::uint32_t event_id_{};
  /*!
   * \brief The session id.
   */
  std::uint32_t session_id_{};
  /*!
   * \brief Read only span into the related IPC packet.
   */
  PacketSpan packet_{};
};

/*!
 * \brief Parameter container for an application error that cannot be convert to an ApApplicationError.
 */
struct IpcApplicationError {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The method id.
   */
  std::uint32_t method_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
  /*!
   * \brief The session id.
   */
  std::uint32_t session_id_{};
  /*!
   * \brief Read only span into the related IPC packet.
   */
  PacketSpan packet_{};
};

/*!
 * \brief Parameter container for an event subscription.
 */
struct IpcSubscribeEvent {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The event id.
   */
  std::uint32_t event_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
};

/*!
 * \brief Parameter container for an event unsubscription.
 */
struct IpcUnsubscribeEvent {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The event id.
   */
  std::uint32_t event_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
};

/*!
 * \brief Parameter container for an event SubscribeEventAck.
 */
struct IpcSubscribeEventAck {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The event id.
   */
  std::uint32_t event_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
};

/*!
 * \brief Parameter container for an event SubscribeEventNack.
 */
struct IpcSubscribeEventNack {
  /*!
   * \brief The direction of the operation.
   */
  MessageDirection direction_{};
  /*!
   * \brief The service id.
   */
  std::uint32_t service_id_{};
  /*!
   * \brief The instance id.
   */
  std::uint32_t instance_id_{};
  /*!
   * \brief The major version.
   */
  std::uint32_t major_version_{};
  /*!
   * \brief The event id.
   */
  std::uint32_t event_id_{};
  /*!
   * \brief The client id.
   */
  std::uint32_t client_id_{};
};

/*!
 * \brief Interface for IPC binding specific monitor messages.
 */
class TraceIf {
 public:
  /*!
   * \brief Constructor.
   *
   * \context App
   * \steady  FALSE
   */
  TraceIf() noexcept = default;

  /*!
   * \brief Destructor.
   *
   * \context     App
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   */
  virtual ~TraceIf() noexcept = default;

  TraceIf(TraceIf const&) noexcept = delete;
  TraceIf(TraceIf&&) noexcept = delete;
  auto operator=(TraceIf&&) noexcept -> TraceIf& = delete;
  auto operator=(TraceIf const&) noexcept -> TraceIf& = delete;

  /*!
   * \brief Trace method for a method call.
   *
   * \param[in] method_call  Reference to struct containing the method call packet IDs and a reference to the serialized
   *                         payload. Important: This reference as well as the reference to the serialized payload is
   *                         only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_TraceIf
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcMethodCall const& method_call) noexcept -> void = 0;

  /*!
   * \brief Trace method for a fire&forget method call.
   *
   * \param[in] method_no_return_call  Reference to struct containing the fire&forget method call packet IDs and a
   *                                   reference to the serialized payload. Important: This reference as well as the
   *                                   reference to the serialized payload is only valid in context of this callback
   *                                   API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_TraceIf
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcMethodNoReturnCall const& method_no_return_call) noexcept -> void = 0;

  /*!
   * \brief Trace method for a method response.
   *
   * \param[in] method_response  Reference to struct containing the method response packet IDs and a reference to the
   *                             serialized payload. Important: This reference as well as the reference to the
   *                             serialized payload is only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_TraceIf
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcMethodResponse const& method_response) noexcept -> void = 0;

  /*!
   * \brief   Trace method for a method error response.
   * \details Empty implementation provided by the interface due to backward-compatibility.
   *
   * \param[in] method_error_response  Reference to struct containing the method error response packet IDs and a
   *                                   reference to the serialized payload. Important: This reference as well as the
   *                                   reference to the serialized payload is only valid in context of this callback
   *                                   API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_TraceIf
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_no_effect
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcMethodErrorResponse const& method_error_response) noexcept -> void {
    static_cast<void>(method_error_response);
  };

  /*!
   * \brief Trace method for an event notification.
   *
   * \param[in] notification  Reference to struct containing the notification packet IDs and a reference to the
   *                          serialized payload. Important: This reference as well as the reference to the serialized
   *                          payload is only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_TraceIf
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcNotification const& notification) noexcept -> void = 0;

  /*!
   * \brief Trace method for an application error.
   *
   * \param[in] application_error  Reference to struct containing the ApplicationError packet IDs and a reference to the
   *                               serialized payload. Important: This reference as well as the reference to the
   *                               serialized payload is only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_TraceIf
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcApplicationError const& application_error) noexcept -> void = 0;

  /*!
   * \brief Trace method for an event subscription.
   *
   * \param[in] subscribe_event  Parameter container for an event subscription. Important: This reference is only valid
   *                             in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcSubscribeEvent const& subscribe_event) noexcept -> void = 0;

  /*!
   * \brief Trace method for an event unsubscription.
   *
   * \param[in] unsubscribe_event  Parameter container for for an event unsubscription. Important: This reference is
   *                               only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcUnsubscribeEvent const& unsubscribe_event) noexcept -> void = 0;

  /*!
   * \brief Trace method for an event subscription acknowledge.
   *
   * \param[in] subscribe_event_ack  Acknowledge Parameter container for an event subscription acknowledge. Important:
   *                                 This reference is only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcSubscribeEventAck const& subscribe_event_ack) noexcept -> void = 0;

  /*!
   * \brief Trace method for an event subscription not acknowledge.
   *
   * \param[in] subscribe_event_nack  Not acknowledge Parameter container for event subscription not acknowledge.
   *                                  Important: This reference is only valid in context of this callback API!.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10_M9.3.3_const_method_interface
  virtual auto Trace(IpcSubscribeEventNack const& subscribe_event_nack) noexcept -> void = 0;
};

}  // namespace trace
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_TRACE_TRACE_IF_H_
