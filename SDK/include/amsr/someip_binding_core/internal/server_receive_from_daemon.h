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
/**        \file  amsr/someip_binding_core/internal/server_receive_from_daemon.h
 *        \brief  ara::com SOME/IP Binding Server Receive messages from daemon and forward it to the server manager
 *interface.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::ServerReceiveFromDaemon
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_RECEIVE_FROM_DAEMON_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_RECEIVE_FROM_DAEMON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/server_handler_interface.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip_daemon_client/internal/server_interface.h"
#include "someipd_app_protocol/internal/message_builder.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_sv
/*!
 * \brief The StringView operator.
 */
using ::vac::container::literals::operator""_sv;

/*!
 * \brief Class to receive method requests from DaemonClient and forward them to ServerHandlerInterface.
 */
class ServerReceiveFromDaemon final : public ::amsr::someip_daemon_client::internal::ServerInterface {
 public:
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ServerReceiveFromDaemon() noexcept final = default;

  ServerReceiveFromDaemon() = delete;
  ServerReceiveFromDaemon(ServerReceiveFromDaemon const&) = delete;
  ServerReceiveFromDaemon(ServerReceiveFromDaemon&&) = delete;
  ServerReceiveFromDaemon& operator=(ServerReceiveFromDaemon const&) & = delete;
  ServerReceiveFromDaemon& operator=(ServerReceiveFromDaemon&&) & = delete;

  /*!
   * \brief       Constructor of this binding initializes the attributes of this class.
   * \param[in]   server_transport_to_daemon A reference to the used SOME/IP transport layer
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \steady      FALSE
   * \vprivate
   * \synchronous TRUE
   */
  explicit ServerReceiveFromDaemon(ServerTransportToDaemonInterface& server_transport_to_daemon) noexcept;

  /*!
   * \brief                  Register Server Handler
   * \details                Register ServerHandler to forward method requests upon receiving it from DaemonClient.
   * \param[in] server_handler Pointer to ServerHandlerInterface
   * \pre                    -
   * \context                Init
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            FALSE
   * \steady                 FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void RegisterServerHandler(ServerHandlerInterface* server_handler) noexcept;

  /*!
   * \brief                  Unregister Server Handler
   * \details                Unregister ServerHandler to cleanup.
   *
   * \pre                    -
   * \context                Shutdown
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            FALSE
   * \steady                 FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void UnregisterServerHandler() noexcept;

 private:
  /*!
   * \brief       Send a SOME/IP error response packet.
   * \details     Used in case of:
   *              - Infrastructural checks failed (protocol version, return code, message type)
   *              - Deserialization failed
   *              - Serialization of method responses failed
   *              - Service / Method is not implemented.
   * \param[in]   return_code    The return code to write into the SOME/IP header.
   * \param[in]   instance_id    The SOME/IP instance ID to write into the SOME/IP header.
   * \param[in]   request_header The unmodified SOME/IP header.
   *                             This SOME/IP header will be used for assembling the error
   *                             response and is partially being reused.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \pre         Connection to the SOME/IP Daemon is established
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \trace       SPEC-4981454, SPEC-4981471, SPEC-4981472, SPEC-4981474, SPEC-4981475
   */
  void SendErrorResponse(::amsr::someip_protocol::internal::ReturnCode const return_code,
                         ::amsr::someip_protocol::internal::InstanceId const instance_id,
                         ::amsr::someip_protocol::internal::SomeIpMessageHeader const& request_header,
                         ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address);

  /*!
   * \brief                  Receive handler for SOME/IP requests.
   * \details                This API is called upon reception of a new method request.
   * \param[in] instance_id  The SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param[in] packet       Method Request SOME/IP header + Payload
   * \pre                    -
   * \context                Reactor
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \synchronous            TRUE
   * \steady                 TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Product Private
   */
  void HandleReceive(::amsr::someip_protocol::internal::InstanceId const instance_id,
                     ::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet) noexcept final;

  /*!
   * \brief Translate error codes from CommunicationErrc to SomeIpReturnCode.
   * \param[in] ret_code Return code to be translated.
   * \return translated error code.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::someip_protocol::internal::SomeIpReturnCode TranslateError(
      ::amsr::core::ErrorDomain::CodeType const ret_code) noexcept;

  /*!
   * \brief Reference to communication link to send and receive SOME/IP messages
   */
  ServerTransportToDaemonInterface& server_transport_to_daemon_;

  /*!
   * \brief Error message allocator.
   */
  FlexibleUniqueMemoryBufferAllocator error_message_memory_buffer_allocator_{};

  /*!
   * \brief Pointer to ServerHandlerInterface
   */
  ServerHandlerInterface* server_handler_{nullptr};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_RECEIVE_FROM_DAEMON_H_
