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
/**        \file
 *        \brief  Application error method response transformation layer class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_APPLICATION_ERROR_RESPONSE_XF_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_APPLICATION_ERROR_RESPONSE_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "ara/com/com_error_domain.h"
#include "someip-protocol/internal/deserialization/deser_ap_application_error.h"
#include "someip-protocol/internal/deserialization/reader.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_transformation_layer/internal/methods/application_error_builder.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief   Method Response transformation layer class.
 * \details This class stores the response in serialized byte stream and also provides the deserialization logic for
 *          that response. Deserialization is triggered by Application thread.
 *
 * \tparam Output  The Output type for method response.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyMethod
 */
template <typename Output>
class ProxyApplicationErrorResponseXf final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] response            Response message.
   * \param[in] application_errors  Possible application errors.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  ProxyApplicationErrorResponseXf(::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage&& response,
                                  ApplicationErrorBuilder::ApplicationErrorMap const& application_errors)
      : logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyApplicationErrorResponseXf"}},
        response_{std::move(response)},
        // VCA_IPCB_STD_FUNCTION_CALLED
        application_errors_{application_errors} {};

  /*!
   * \brief Default move constructor.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ProxyApplicationErrorResponseXf(ProxyApplicationErrorResponseXf&&) noexcept = default;

  auto operator=(ProxyApplicationErrorResponseXf&&) noexcept -> ProxyApplicationErrorResponseXf& = delete;
  ProxyApplicationErrorResponseXf(ProxyApplicationErrorResponseXf const&) noexcept = delete;
  auto operator=(ProxyApplicationErrorResponseXf const&) noexcept -> ProxyApplicationErrorResponseXf& = delete;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     APP
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ProxyApplicationErrorResponseXf() noexcept = default;  // VCA_IPCB_STD_MAP_DESTRUCTOR

  /*!
   * \brief Deserialize the application error response message.
   *
   * \return Result containing an output value or an error.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Deserialize the response as per message type.
   * \endinternal
   */
  auto operator()() noexcept -> ::amsr::core::Result<Output, ::amsr::core::ErrorCode> { return GetOutput(); }

 private:
  /*!
   * \brief Deserialize Application Error response message.
   *
   * \return Result containing an error.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \internal
   * - Set an ErrorCode in result which indicates general network binding failure.
   * - Create a data view on the ApplicationError by omitting the message header.
   * - Deserialize the ApplicationError.
   * - Check if the deserialization is successful.
   *   - Set ApplicationError as error in the result.
   * - Return the result.
   * \endinternal
   */
  auto GetOutput() noexcept -> ::amsr::core::Result<Output, ::amsr::core::ErrorCode> {
    // PTP-B-IpcBinding-Xf-Proxy-Deserialize-ApApplicationError

    ::amsr::core::Result<Output, ::amsr::core::ErrorCode> result_deser{
        ::amsr::core::ErrorCode{::ara::com::ComErrc::kNetworkBindingFailure}};
    ::amsr::someip_protocol::internal::serialization::ApApplicationError app_error{};
    // Move the ownership of the packet to this scope for deserialization of the serialized byte stream.
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10_A18.5.8_false_positive
    ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared const packet{response_.GetPacket()};
    // Setup reader for application error payload deserialization
    ::amsr::ipc_binding_core::internal::ipc_protocol::MessageLength constexpr kHeaderLength{
        ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
        ::amsr::ipc_binding_core::internal::ipc_protocol::kApplicationErrorMessageHeaderLength};
    ::amsr::someip_protocol::internal::deserialization::Reader reader{
        {&((*packet)[kHeaderLength]), packet->size() - kHeaderLength}};
    // Deserialize application error payload.
    bool const deserialization_ok{
        ::amsr::someip_protocol::internal::deserialization::DeserializeApApplicationError(reader, app_error)};

    if (deserialization_ok) {
      ::amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct error_struct{
          ::amsr::core::get<::amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct>(app_error)};
      ApplicationErrorBuilder const error_code_builder{application_errors_};  // VCA_IPCB_UNDEFINED_FUNCTION

      ::amsr::core::ErrorDomain::StringType constexpr kDefaultApplicationErrorUserMessage{""};
      error_struct.user_message = kDefaultApplicationErrorUserMessage;

      ::amsr::core::ErrorCode error_code{
          error_code_builder.CreateErrorCode(error_struct)};  // VCA_IPCB_UNDEFINED_FUNCTION
      result_deser.EmplaceError(std::move(error_code));
    } else {
      ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessageHeader const header{
          response_.GetMessageHeader()};
      logger_.LogError(
          [&header](::ara::log::LogStream& s) {
            s << "Deserialization of ApApplicationError response message failed (";
            ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder::LogCompleteServiceInstanceMethodId(
                s, header.service_id_, header.major_version_, header.instance_id_, header.method_id_);
            s << ")";
          },
          ::amsr::ipc_binding_core::internal::logging::AraComLogger::LogLocation{static_cast<char const*>(__func__),
                                                                                 __LINE__});
    }

    // PTP-E-IpcBinding-Xf-Proxy-Deserialize-ApApplicationError
    return result_deser;
  }

  /*!
   * \brief Logger for printing debug and error messages.
   */
  ::amsr::ipc_binding_core::internal::logging::AraComLogger logger_;

  /*!
   * \brief Stores the response.
   */
  ::amsr::ipc_binding_core::internal::ipc_protocol::ApplicationErrorMessage response_;

  /*!
   * \brief   Map of application errors and their error domains.
   * \details ApplicationErrorMap is copied for each method reponse as the operator () could be triggered by the
   *          application user after the proxy has been destroyed.
   */
  ApplicationErrorBuilder::ApplicationErrorMap application_errors_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_APPLICATION_ERROR_RESPONSE_XF_H_
