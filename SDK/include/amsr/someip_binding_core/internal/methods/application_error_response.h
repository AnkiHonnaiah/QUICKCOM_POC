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
/**        \file  amsr/someip_binding_core/internal/methods/application_error_response.h
 *        \brief  Data type class for application error method response.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ApplicationErrorResponse
 *
 *      \details  This class holds the application error method response message, which can be used in send method
 *                response. The class also contains helper functionality to create the data type from an incoming
 *                message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_APPLICATION_ERROR_RESPONSE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_APPLICATION_ERROR_RESPONSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_domain.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   Data type class for application error method response message.
 * \details This class holds the application error method response message, which can be used in reception or
 *          sending path. The class also contains helper functionality to create the data type from an incoming
 *          message.
 *
 */
class ApplicationErrorResponse final {
 public:
  /*!
   * \brief Named constructor of ApplicationErrorResponse
   * \details This function creates an ApplicationErrorResponse instance using the incoming packet
   *
   * \param[in]   response   Serialized SOME/IP Method Response [SOME/IP message]
   *
   * \return ApplicationErrorResponse object containing unique ErrorDomain identifier, domain-specific error code and
   *         support data or an error if the message of a wrong type
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
  static ::amsr::core::Result<ApplicationErrorResponse> Create(
      ::amsr::someipd_app_protocol::internal::SomeIpMessage const&& response) noexcept;

  /*!
   * \brief Constructor for adaptive stack.
   * \param[in]   service_id         The SOME/IP Service ID
   * \param[in]   interface_version  The SOME/IP InterfaceVersion
   * \param[in]   client_id          The SOME/IP Client ID
   * \param[in]   method_id          The SOME/IP Method ID
   * \param[in]   session_id         The SOME/IP Session ID
   * \param[in]   error_code         A domain-specific error code
   * \param[in]   error_domain_value A unique ErrorDomain identifier
   * \param[in]   support_data       Vendor- and context-specific supplementary data.
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
  ApplicationErrorResponse(::amsr::someip_protocol::internal::ServiceId const& service_id,
                           ::amsr::someip_protocol::internal::InterfaceVersion const& interface_version,
                           ::amsr::someip_protocol::internal::ClientId const& client_id,
                           ::amsr::someip_protocol::internal::MethodId const& method_id,
                           ::amsr::someip_protocol::internal::SessionId const& session_id,
                           ::amsr::core::ErrorDomain::CodeType const& error_code,
                           ::amsr::core::ErrorDomain::IdType const& error_domain_value,
                           ::amsr::core::ErrorDomain::SupportDataType::CodeType const& support_data) noexcept;

  /*!
   * \brief Constructor for classic stack.
   * \param[in]   service_id         The SOME/IP Service ID
   * \param[in]   interface_version  The SOME/IP InterfaceVersion
   * \param[in]   client_id          The SOME/IP Client ID
   * \param[in]   method_id          The SOME/IP Method ID
   * \param[in]   session_id         The SOME/IP Session ID
   * \param[in]   error_code         A domain-specific error code
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-4980104
   * \spec
   *   requires true;
   * \endspec
   */
  ApplicationErrorResponse(::amsr::someip_protocol::internal::ServiceId const& service_id,
                           ::amsr::someip_protocol::internal::InterfaceVersion const& interface_version,
                           ::amsr::someip_protocol::internal::ClientId const& client_id,
                           ::amsr::someip_protocol::internal::MethodId const& method_id,
                           ::amsr::someip_protocol::internal::SessionId const& session_id,
                           ::amsr::core::ErrorDomain::CodeType const& error_code) noexcept;

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
  ~ApplicationErrorResponse() noexcept = default;

  /*!
   * \brief Default move constructor
   * \details A move constructor is needed to allow this datatype to be returned
   *          from ApplicationErrorResponse::Create. Move operation is only used for this use
   *          case and therefore there is no moved-from object that could be in an invalid state
   * \context Reactor
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ApplicationErrorResponse(ApplicationErrorResponse&&) noexcept = default;

  ApplicationErrorResponse() = delete;
  ApplicationErrorResponse(ApplicationErrorResponse const&) = delete;
  ApplicationErrorResponse& operator=(ApplicationErrorResponse const&) & = delete;
  ApplicationErrorResponse& operator=(ApplicationErrorResponse&&) & = delete;

  /*!
   * \brief Following info might not be available for responses received from classic ECUs.
   */
  struct ApplicationErrorInfo {
    /*!
     * \brief The unique error domain identifier.
     */
    amsr::core::ErrorDomain::IdType error_domain_value_{};

    /*!
     * \brief Vendor- and context-specific supplementary data
     */
    amsr::core::ErrorDomain::SupportDataType::CodeType support_data_{};
  };

  /*!
   * \brief Getter for the Service Id.
   *
   * \return Service ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept;

  /*!
   * \brief Getter for the Interface version.
   *
   * \return Interface version.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::InterfaceVersion GetInterfaceVersion() const noexcept;

  /*!
   * \brief Getter for the Client Id.
   *
   * \return Client ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::ClientId GetClientId() const noexcept;

  /*!
   * \brief Getter for the Method Id.
   *
   * \return Method ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::MethodId GetMethodId() const noexcept;

  /*!
   * \brief Getter for the Session Id.
   *
   * \return Session ID.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  ::amsr::someip_protocol::internal::SessionId GetSessionId() const noexcept;

  /*!
   * \brief Getter for a domain-specific error code
   * \return Domain-specific error code
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::ErrorDomain::CodeType GetErrorCode() const noexcept;

  /*!
   * \brief Application error information contains both error domain id and support data
   * \return Application error information.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Optional<ApplicationErrorInfo> GetApplicationErrorInfo() const noexcept;

 private:
  /*!
   * \brief The service ID.
   * Required to know which client shall receive the response.
   */
  ::amsr::someip_protocol::internal::ServiceId const service_id_;

  /*!
   * \brief The interface version.
   * Required to know which client shall receive the response.
   */
  ::amsr::someip_protocol::internal::InterfaceVersion const interface_version_;

  /*!
   * \brief The client ID.
   * Required to know which client shall receive the response.
   */
  ::amsr::someip_protocol::internal::ClientId const client_id_;

  /*!
   * \brief The method ID.
   * Required to know to which method this response belongs.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief The session ID.
   * Required to know to which request this response belongs.
   */
  ::amsr::someip_protocol::internal::SessionId const session_id_;

  /*!
   * \brief A domain-specific error code
   */
  ::amsr::core::ErrorDomain::CodeType const error_code_;

  /*!
   * \brief The application error information contains both error domain id and support data..
   */
  ::amsr::core::Optional<ApplicationErrorInfo> const info_{};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_APPLICATION_ERROR_RESPONSE_H_
