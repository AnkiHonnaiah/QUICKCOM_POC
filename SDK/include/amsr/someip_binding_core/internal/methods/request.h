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
/**        \file  amsr/someip_binding_core/internal/methods/request.h
 *        \brief  Data type class for method request.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::Request
 *
 *      \details  This class holds the request message, which can be used in handle method
 *                request. The class also contains helper functionality to create the data type from an incoming
 *                message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_REQUEST_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_REQUEST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/communication_error_domain.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/someip_message.h"
namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief RequesterAddress contains the IP address and port of the sender of the method request,This struct is empty
 *        now and will be implemented in WIZRD-1623.
 */
struct RequesterAddress {};

/*!
 * \brief   Data type class for method request.
 * \details This class holds the request message, which can be used in handle method
 *          request. The class also contains helper functionality to create the data type from an incoming
 *          message.
 *
 */
class Request final {
 public:
  /*!
   * \brief Named constructor of Request
   * \details This function creates an Request instance using the incoming packet
   *
   * \param[in]   request_message  SomeIp method request message.
   * \param[in]   instance_id      Instance Id.
   *
   * \return Request object containing the complete SOME/IP message or an error.
   *
   * \error  CommunicationErrc::wrong_message_type        If wrong message type is given.
   * \error  CommunicationErrc::wrong_protocol_version    If wrong protocol version is given.
   * \error  CommunicationErrc::wrong_return_code         If wrong return code is given.
   *
   * \pre         -
   * \context     APP
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Result<Request> Create(::amsr::someipd_app_protocol::internal::SomeIpMessage&& request_message,
                                              ::amsr::someip_protocol::internal::InstanceId const instance_id) noexcept;

  /*!
   * \brief constructor
   *
   * \param[in]   service_id         The SOME/IP Service ID
   * \param[in]   interface_version  The SOME/IP InterfaceVersion
   * \param[in]   client_id          The SOME/IP Client ID
   * \param[in]   method_id          The SOME/IP Method ID
   * \param[in]   session_id         The SOME/IP Session ID
   * \param[in]   instance_id        The SOME/IP Instance ID
   * \param[in]   message_header     The SOME/IP header
   * \param[in]   buffer             MemoryBuffer pointer
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires buffer != nullptr;
   * \endspec
   */
  Request(::amsr::someip_protocol::internal::ServiceId const& service_id,
          ::amsr::someip_protocol::internal::InterfaceVersion const& interface_version,
          ::amsr::someip_protocol::internal::ClientId const& client_id,
          ::amsr::someip_protocol::internal::MethodId const& method_id,
          ::amsr::someip_protocol::internal::SessionId const& session_id,
          ::amsr::someip_protocol::internal::InstanceId const instance_id,
          ::amsr::someip_protocol::internal::SomeIpMessageHeader const& message_header,
          MemoryBufferPtr buffer) noexcept;

  /*!
   * \brief Default destructor
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~Request() noexcept = default;

  /*!
   * \brief Default move constructor
   * \details A move constructor is needed to allow this datatype to be returned
   *          from Request::Create. Move operation is only used for this use
   *          case and therefore there is no moved-from object that could be in an invalid state.
   * \context Reactor
   * \spec
   *   requires true;
   * \endspec
   */
  Request(Request&&) noexcept = default;

  Request() = delete;
  Request(Request const&) = delete;
  Request& operator=(Request const&) & = delete;
  Request& operator=(Request&&) & = delete;

  /*!
   * \brief Getter for the Service ID.
   *
   * \return Service ID.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept;

  /*!
   * \brief Getter for the InterfaceVersion.
   *
   * \return InterfaceVersion.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::InterfaceVersion GetInterfaceVersion() const noexcept;

  /*!
   * \brief Getter for the Client ID.
   *
   * \return Client ID.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ClientId GetClientId() const noexcept;

  /*!
   * \brief Getter for the Method ID.
   *
   * \return Method ID.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MethodId GetMethodId() const noexcept;

  /*!
   * \brief Getter for the Session ID.
   *
   * \return Session ID.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::SessionId GetSessionId() const noexcept;

  /*!
   * \brief Getter for the Instance ID.
   *
   * \return Instance ID.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::InstanceId GetInstanceId() const noexcept;

  /*!
   * \brief Getter for the SomeIp header.
   *
   * \return The SomeIp header.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::SomeIpMessageHeader GetSomeIpHeader() const& noexcept;

  /*!
   * \brief Getter for the buffer containing SOME/IP request message.
   *
   * \return unique pointer containing the buffer.
   *
   * \pre Not allowed to call it twice.
   *
   * \steady TRUE
   */
  MemoryBufferPtr ReleaseBuffer() noexcept;

  /*!
   * \brief Getter for the requester address.
   *
   * \return Requester address.
   *
   * \steady TRUE
   */
  RequesterAddress GetRequesterAddress() const noexcept;

 private:
  /*!
   * \brief The Service ID.
   * Required to know which client shall triggered the request.
   */
  ::amsr::someip_protocol::internal::ServiceId const service_id_;

  /*!
   * \brief The InterfaceVersion.
   * Required to know which client shall triggered the request.
   */
  ::amsr::someip_protocol::internal::InterfaceVersion const interface_version_;

  /*!
   * \brief The Client ID.
   * Required to know which client shall triggered the request.
   */
  ::amsr::someip_protocol::internal::ClientId const client_id_;

  /*!
   * \brief The Method ID.
   * Required to know which client shall triggered the request.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief The Session ID.
   * Required to know which client shall triggered the request.
   */
  ::amsr::someip_protocol::internal::SessionId const session_id_;

  /*!
   * \brief The Instance ID.
   * Required to know which client shall triggered the request.
   */
  ::amsr::someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief The SomeIp message header.
   */
  ::amsr::someip_protocol::internal::SomeIpMessageHeader const message_header_;

  /*!
   * \brief A data buffer pointer containing a complete SOME/IP message.
   */
  MemoryBufferPtr buffer_;

  /*!
   * \brief RequesterAddress contaions the IP address and port of the sender of the method request.
   */
  RequesterAddress const requester_address_{};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_REQUEST_H_
