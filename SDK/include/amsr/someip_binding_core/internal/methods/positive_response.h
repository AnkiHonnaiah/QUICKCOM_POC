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
/**        \file  amsr/someip_binding_core/internal/methods/positive_response.h
 *        \brief  Data type class for positive response.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::PositiveResponse
 *
 *      \details  This class holds the positive response message, which can be used in send method
 *                respons. The class also contains helper functionality to create the data type from an incoming
 *                message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_POSITIVE_RESPONSE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_POSITIVE_RESPONSE_H_

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
 * \brief   Data type class for positive response message.
 * \details This class holds the positive response message, which can be used in send method
 *          response. The class also contains helper functionality to create the data type from an incoming
 *          message.
 *
 */
class PositiveResponse final {
 public:
  /*!
   * \brief Named constructor of PositiveResponse
   * \details This function creates an PositiveResponse instance using the incoming packet
   *
   * \param[in]   response  Serialized SOME/IP Method Response [SOME/IP message]
   *
   * \return PositiveResponse object containing the complete SOME/IP message or an error if the message of a wrong type
   * \error  CommunicationErrc::wrong_message_type  If wrong message type is given.
   * \error  CommunicationErrc::wrong_return_code   If wrong return code is given.
   * \pre         -
   * \context     APP
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Result<PositiveResponse> Create(
      ::amsr::someipd_app_protocol::internal::SomeIpMessage&& response) noexcept;

  /*!
   * \brief constructor
   * \param[in]   service_id         The SOME/IP Service ID
   * \param[in]   interface_version  The SOME/IP InterfaceVersion
   * \param[in]   client_id          The SOME/IP Client ID
   * \param[in]   method_id          The SOME/IP Method ID
   * \param[in]   session_id         The SOME/IP Session ID
   * \param[in]   buffer             MemoryBuffer pointer
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires buffer != nullptr;
   * \endspec
   */
  PositiveResponse(::amsr::someip_protocol::internal::ServiceId const& service_id,
                   ::amsr::someip_protocol::internal::InterfaceVersion const& interface_version,
                   ::amsr::someip_protocol::internal::ClientId const& client_id,
                   ::amsr::someip_protocol::internal::MethodId const& method_id,
                   ::amsr::someip_protocol::internal::SessionId const& session_id, MemoryBufferPtr buffer) noexcept;

  /*!
   * \brief Default destructor
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~PositiveResponse() noexcept = default;

  /*!
   * \brief Default move constructor
   * \details A move constructor is needed to allow this datatype to be returned
   *          from PositiveResponse::Create. Move operation is only used for this use
   *          case and therefore there is no moved-from object that could be in an invalid state.
   * \context Reactor
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  PositiveResponse(PositiveResponse&&) noexcept = default;

  PositiveResponse() = delete;
  PositiveResponse(PositiveResponse const&) = delete;
  PositiveResponse& operator=(PositiveResponse const&) & = delete;
  PositiveResponse& operator=(PositiveResponse&&) & = delete;

  /*!
   * \brief Getter for the Service ID.
   *
   * \return Service ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept;

  /*!
   * \brief Getter for the InterfaceVersion.
   *
   * \return InterfaceVersion.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::InterfaceVersion GetInterfaceVersion() const noexcept;

  /*!
   * \brief Getter for the Client ID.
   *
   * \return Client ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::ClientId GetClientId() const noexcept;

  /*!
   * \brief Getter for the Method ID.
   *
   * \return Method ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::MethodId GetMethodId() const noexcept;

  /*!
   * \brief Getter for the Session ID.
   *
   * \return Session ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::SessionId GetSessionId() const noexcept;

  /*!
   * \brief Getter for the buffer containing header + payload of the SOME/IP event message.
   *
   * \return unique pointer containing the buffer.
   *
   * \pre Not allowed to call it twice.
   * \steady           TRUE
   */
  MemoryBufferPtr ReleaseBuffer() noexcept;

 private:
  /*!
   * \brief The Service ID.
   * Required to know which client shall receive the response.
   */
  ::amsr::someip_protocol::internal::ServiceId const service_id_;

  /*!
   * \brief The InterfaceVersion.
   * Required to know which client shall receive the response.
   */
  ::amsr::someip_protocol::internal::InterfaceVersion const interface_version_;

  /*!
   * \brief The Client ID.
   * Required to know which client shall receive the response.
   */
  ::amsr::someip_protocol::internal::ClientId const client_id_;

  /*!
   * \brief The Method ID.
   * Required to know to which method this response belongs.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief The Session ID.
   * Required to know to which request this response belongs.
   */
  ::amsr::someip_protocol::internal::SessionId const session_id_;

  /*!
   * \brief A data buffer pointer containing a complete SOME/IP message with a header.
   */
  MemoryBufferPtr buffer_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_POSITIVE_RESPONSE_H_
