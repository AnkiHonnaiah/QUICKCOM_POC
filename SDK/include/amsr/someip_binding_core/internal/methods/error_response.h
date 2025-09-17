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
/**        \file  amsr/someip_binding_core/internal/methods/error_response.h
 *        \brief  Data type class for error response.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ErrorResponse
 *
 *      \details  This class holds the error response message, which can be used in send method
 *                respons. The class also contains helper functionality to create the data type from an incoming
 *                message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_ERROR_RESPONSE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_ERROR_RESPONSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/communication_error_domain.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   Data type class for error response message.
 * \details This class holds the error response message, which can be used in send method
 *          response. The class also contains helper functionality to create the data type from an incoming
 *          message.
 *
 */
class ErrorResponse final {
 public:
  /*!
   * \brief Named constructor of ErrorResponse
   * \details This function creates an ErrorResponse instance by using the incoming packet
   *
   * \param[in]   response Serialized SOME/IP Method Response [SOME/IP message]
   *
   * \return ErrorResponse object containing SOME/IP return code or an error if the message of a wrong type
   * \error  CommunicationErrc::error_not_ok If wrong message type or return code range not found
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
  static ::amsr::core::Result<ErrorResponse> Create(
      ::amsr::someipd_app_protocol::internal::SomeIpMessage const&& response);

  /*!
   * \brief constructor
   * \param[in]   service_id         The SOME/IP Service ID
   * \param[in]   interface_version  The SOME/IP InterfaceVersion
   * \param[in]   client_id          The SOME/IP Client ID
   * \param[in]   method_id          The SOME/IP Method ID
   * \param[in]   session_id         The SOME/IP Session ID
   * \param[in]   return_code        The SOME/IP return code
   * \pre         -
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
  ErrorResponse(::amsr::someip_protocol::internal::ServiceId const& service_id,
                ::amsr::someip_protocol::internal::InterfaceVersion const& interface_version,
                ::amsr::someip_protocol::internal::ClientId const& client_id,
                ::amsr::someip_protocol::internal::MethodId const& method_id,
                ::amsr::someip_protocol::internal::SessionId const& session_id,
                ::amsr::someip_protocol::internal::SomeIpReturnCode const& return_code) noexcept;

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
  ~ErrorResponse() noexcept = default;

  /*!
   * \brief Default move constructor
   * \details A move constructor is needed to allow this datatype to be returned
   *          from ErrorResponse::Create. Move operation is only used for this use
   *          case and therefore there is no moved-from object that could be in an invalid state
   * \context Reactor
   * \spec
   *   requires true;
   * \endspec
   */
  ErrorResponse(ErrorResponse&&) noexcept = default;

  ErrorResponse() = delete;
  ErrorResponse(ErrorResponse const&) = delete;
  ErrorResponse& operator=(ErrorResponse const&) & = delete;
  ErrorResponse& operator=(ErrorResponse&&) & = delete;

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
   * \brief Getter for SOME/IP return code
   *
   * \return SOME/IP return code
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::SomeIpReturnCode GetReturnCode() const noexcept;

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
   * \brief The SOME/IP return code
   */
  ::amsr::someip_protocol::internal::SomeIpReturnCode const return_code_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_ERROR_RESPONSE_H_
