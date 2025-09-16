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
 *        \brief  Method response transformation layer class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_RESPONSE_XF_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_RESPONSE_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
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
 * \tparam ResponseDeserializer  Deserializer type for the Output. Either void (if the method has no output parameters),
 *                               or a generated deserializer. Also used to inject a mocked deserializer for testing.
 * \tparam Output                The Output type for method response.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyMethod
 */
template <typename ResponseDeserializer, typename Output>
class ProxyMethodResponseXf final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] response  Response message.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  explicit ProxyMethodResponseXf(::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage&& response)
      : logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyMethodResponseXf"}},
        response_{std::move(response)} {};

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
  ProxyMethodResponseXf(ProxyMethodResponseXf&&) noexcept = default;

  auto operator=(ProxyMethodResponseXf const&) noexcept -> ProxyMethodResponseXf& = delete;
  ProxyMethodResponseXf(ProxyMethodResponseXf const&) noexcept = delete;
  auto operator=(ProxyMethodResponseXf&&) noexcept -> ProxyMethodResponseXf& = delete;

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
  ~ProxyMethodResponseXf() noexcept = default;

  /*!
   * \brief Deserialize the positive response message.
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
   * \brief   Deserialize positive response.
   * \details This method is available if both ResponseMessage and ResponseDeserializer are configured, otherwise it is
   *          removed.
   *
   * \tparam T  Configured ResponseDeserializer.
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
   * \internal
   * - Set an ErrorCode in result which indicates general network binding failure.
   * - Create a data view on the payload by omitting the message header.
   * - Deserialize the payload.
   * - Check if the deserialization is successful.
   *   - Set the deserialized payload as value in the result.
   * - Return the result.
   * \endinternal
   */
  template <class T = ResponseDeserializer>
  auto GetOutput() noexcept ->
      typename std::enable_if<!std::is_void<T>::value, ::amsr::core::Result<Output, ::amsr::core::ErrorCode>>::type {
    // PTP-B-IpcBinding-Xf-Proxy-Deserialize-MethodResponse

    Output output{};
    ::amsr::core::Result<Output, ::amsr::core::ErrorCode> result_deser{
        ::amsr::core::ErrorCode{::ara::com::ComErrc::kNetworkBindingFailure}};

    ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared const packet{response_.GetPacket()};

    // Setup reader for payload deserialization
    ::amsr::ipc_binding_core::internal::ipc_protocol::MessageLength constexpr kHeaderLengthVal{
        ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
        ::amsr::ipc_binding_core::internal::ipc_protocol::kResponseMessageHeaderLength};
    ::amsr::someip_protocol::internal::deserialization::Reader reader{
        {&((*packet)[kHeaderLengthVal]), packet->size() - kHeaderLengthVal}};

    // Deserialize method output arguments.
    // VCA_IPCB_DESERIALIZE_SAFE_FUNCTION
    bool const deserialization_ok{ResponseDeserializer::Deserialize(reader, output)};

    if (deserialization_ok) {
      result_deser.EmplaceValue(std::move(output));
    } else {
      ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessageHeader const header{
          response_.GetMessageHeader()};
      logger_.LogError(
          [&header](::ara::log::LogStream& s) {
            s << "Deserialization of response message failed (";
            ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder::LogCompleteServiceInstanceMethodId(
                s, header.service_id_, header.major_version_, header.instance_id_, header.method_id_);
            s << ")";
          },
          ::amsr::ipc_binding_core::internal::logging::AraComLogger::LogLocation{static_cast<char const*>(__func__),
                                                                                 __LINE__});
    }

    // PTP-E-IpcBinding-Xf-Proxy-Deserialize-MethodResponse
    return result_deser;
  }  // VCA_IPCB_STD_UNIQUE_POINTER_DESTRUCTOR

  /*!
   * \brief   Handles positive response message for no output.
   * \details This method is available if ResponseMessage is configured and ResponseDeserializer is not configured,
   *          otherwise it is removed.
   *
   * \tparam T  Configured ResponseDeserializer.
   *
   * \return Result containing an output value.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \internal
   * - Set an empty output in result.
   * - Return the result.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10-M9.3.3_static_method
  template <class T = ResponseDeserializer>
  auto GetOutput() noexcept ->
      typename std::enable_if<(std::is_void<T>::value), ::amsr::core::Result<Output, ::amsr::core::ErrorCode>>::type {
    Output const output{};
    ::amsr::core::Result<Output, ::amsr::core::ErrorCode> const result{output};
    return result;
  }

  /*!
   * \brief Logger for printing debug and error messages.
   */
  ::amsr::ipc_binding_core::internal::logging::AraComLogger logger_;

  /*!
   * \brief Stores the response.
   */
  ::amsr::ipc_binding_core::internal::ipc_protocol::ResponseMessage response_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_RESPONSE_XF_H_
