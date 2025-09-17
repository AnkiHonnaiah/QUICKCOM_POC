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
/**        \file  amsr/someip_binding_core/internal/methods/request_no_return.h
 *        \brief  Data type class for method request no return.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::RequestNoReturn
 *
 *      \details  This class holds the request message, which can be used in handle method
 *                request no return. The class also contains helper functionality to create the data type from an
 *                incoming message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_REQUEST_NO_RETURN_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_REQUEST_NO_RETURN_H_

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
 * \brief   Data type class for method request no return.
 * \details This class holds the request message, which can be used in handle method
 *          request no return. The class also contains helper functionality to create the data type from an incoming
 *          message.
 *
 */
class RequestNoReturn final {
 public:
  /*!
   * \brief Named constructor of RequestNoReturn
   * \details This function creates a RequestNoReturn instance using the incoming packet
   *
   * \param[in]   request_message  SomeIp method request message.
   * \param[in]   instance_id      Instance Id.
   *
   * \return RequestNoReturn object containing the complete SOME/IP message or an error.
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
  static ::amsr::core::Result<RequestNoReturn> Create(
      ::amsr::someipd_app_protocol::internal::SomeIpMessage&& request_message,
      ::amsr::someip_protocol::internal::InstanceId const instance_id) noexcept;

  /*!
   * \brief constructor
   *
   * \param[in]   service_id         The SOME/IP Service ID
   * \param[in]   interface_version  The SOME/IP InterfaceVersion
   * \param[in]   method_id          The SOME/IP Method ID
   * \param[in]   instance_id        The SOME/IP Instance ID
   * \param[in]   request_header     SOME/IP header
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
  RequestNoReturn(::amsr::someip_protocol::internal::ServiceId const& service_id,
                  ::amsr::someip_protocol::internal::InterfaceVersion const& interface_version,
                  ::amsr::someip_protocol::internal::MethodId const& method_id,
                  ::amsr::someip_protocol::internal::InstanceId const instance_id,
                  ::amsr::someip_protocol::internal::SomeIpMessageHeader const& request_header,
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
  ~RequestNoReturn() noexcept = default;

  /*!
   * \brief Default move constructor
   * \details A move constructor is needed to allow this datatype to be returned
   *          from RequestNoReturn::Create. Move operation is only used for this use
   *          case and therefore there is no moved-from object that could be in an invalid state.
   * \context Reactor
   * \spec
   *   requires true;
   * \endspec
   */
  RequestNoReturn(RequestNoReturn&&) noexcept = default;

  RequestNoReturn() = delete;
  RequestNoReturn(RequestNoReturn const&) = delete;
  RequestNoReturn& operator=(RequestNoReturn const&) & = delete;
  RequestNoReturn& operator=(RequestNoReturn&&) & = delete;

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
   * \brief Getter for the Method ID.
   *
   * \return Method ID.
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MethodId GetMethodId() const noexcept;

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
   */
  MemoryBufferPtr ReleaseBuffer() noexcept;

 private:
  /*!
   * \brief The Service ID.
   */
  ::amsr::someip_protocol::internal::ServiceId const service_id_;

  /*!
   * \brief The InterfaceVersion.
   */
  ::amsr::someip_protocol::internal::InterfaceVersion const interface_version_;

  /*!
   * \brief The Method ID.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief The Instance ID.
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
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_REQUEST_NO_RETURN_H_
