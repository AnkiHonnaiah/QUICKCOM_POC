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
/**        \file  someip_binding_xf/internal/methods/proxy_method_xf.h
 *        \brief  Method manager to handle common logic for method requests/responses on proxy side within SOME/IP
 *                binding.
 * \unit SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyMethodXf
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_METHOD_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_METHOD_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/methods/application_error_response.h"
#include "amsr/someip_binding_core/internal/methods/error_response.h"
#include "amsr/someip_binding_core/internal/methods/pending_request_map.h"
#include "amsr/someip_binding_core/internal/methods/positive_response.h"
#include "amsr/someip_binding_core/internal/methods/proxy_method_response_handler_interface.h"
#include "amsr/someip_binding_core/internal/session_handler.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/methods/application_error_builder.h"
#include "amsr/someip_binding_xf/internal/methods/positive_method_response_xf.h"
#include "amsr/someip_binding_xf/internal/methods/proxy_method_xf_interface.h"
#include "amsr/someip_binding_xf/internal/methods/types.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_serializer_interface.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_deserializer_interface.h"
#include "ara/core/promise.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief Configuration class for SOME/IP proxy method manager.
 * \unit SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyMethodXf
 *
 * \details This class combines all required definitions for ProxyMethodXf class. It is passed as a template
 *          parameter to ProxyMethodXf.
 *
 * \tparam OutputType The Output type for PendingRequestMap.
 * \tparam ArgsT The Variadic template parameter.
 */
template <typename OutputType, typename... ArgsT>
class ProxyMethodXf final : public ProxyMethodXfInterface<OutputType, ArgsT...>,
                            public ::amsr::someip_binding_core::internal::methods::ProxyMethodResponseHandlerInterface {
 public:
  /*!
   * \brief Type alias for the concrete memory buffer type.
   */
  using BufferPtrType = ::amsr::someip_binding_core::internal::MemoryBufferPtr;

  /*!
   * \brief The output struct type containing the method return type
   */
  using Output = OutputType;

  /*!
   * \brief Deserializer for the method response
   */
  using DeserializerInterface = internal::ser_deser::MethodResponseDeserializerInterface<Output>;

  /*!
   * \brief Promise type of the response value.
   */
  using ResponseValuePromise = ::ara::core::Promise<Output>;

  /*!
   * \brief Optional of the promise type.
   */
  using PromiseOptional = ::amsr::core::Optional<ResponseValuePromise>;

  /*!
   * \brief Future type for the response value.
   */
  using ResponseValueFuture = ::amsr::core::Future<Output>;

  /*!
   * \brief Optional of the ResponseValueFuture type.
   */
  using FutureOptional = ::amsr::core::Optional<ResponseValueFuture>;

  /*!
   * \brief Type alias for the service ID.
   */
  using ServiceId = ::amsr::someip_protocol::internal::ServiceId;

  /*!
   * \brief Type alias for the instance ID.
   */
  using InstanceId = ::amsr::someip_protocol::internal::InstanceId;

  /*!
   * \brief Type alias for the major version.
   */
  using MajorVersion = ::amsr::someip_protocol::internal::MajorVersion;

  /*!
   * \brief Type alias for the client ID.
   */
  using ClientId = ::amsr::someip_protocol::internal::ClientId;

  /*!
   * \brief Type alias for the method ID.
   */
  using MethodId = ::amsr::someip_protocol::internal::MethodId;

  /*!
   * \brief An alias to the map of errordomain.
   */
  using ApplicationErrorMap =
      std::map<amsr::core::ErrorDomain::IdType, ::amsr::someip_binding_xf::internal::methods::ApplicationErrorDomain>;

  /*!
   * \brief Type alias for MethodRequestSerializerInterface.
   */
  using SerializerInterface = internal::ser_deser::MethodRequestSerializerInterface<ArgsT...>;

  /*!
   * \brief Type alias for pending request map.
   */
  using PendingRequestMapType = amsr::someip_binding_core::internal::methods::PendingRequestMap<Output>;

  /*!
   * \brief Type alias for shared pointer to PendingRequestMap.
   */
  using PendingRequestMapSharedPtr = std::shared_ptr<PendingRequestMapType>;

  /*!
   * \brief Constructor. Sets a reference to the ProxyBinding, for sending out the serialized packets.
   *
   * \param[in]  local_client           The LocalClient.
   * \param[in]  service_id             SOME/IP service ID used by this binding.
   * \param[in]  instance_id            SOME/IP instance ID used by this binding.
   * \param[in]  major_version          SOME/IP major version used by this binding.
   * \param[in]  client_id              SOME/IP related client ID used by this concrete proxy instance.
   * \param[in]  method_id              SOME/IP method identifier for this method.
   * \param[in]  method_name            SOME/IP method name.
   * \param[in]  application_error_map  Application Error Map to decode application errors responses.
   * \param[in]  request_serializer     Method request serializer.
   * \param[in]  response_deserializer  Response deserializer for the payload in case of a positive response.
   * \param[in]  allocator              Allocator for memory used to serialize/send method request.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \trace SPEC-10144387
   */
  ProxyMethodXf(::amsr::someip_binding_core::internal::LocalClient& local_client, ServiceId const service_id,
                InstanceId const instance_id, MajorVersion const major_version, ClientId const client_id,
                MethodId const method_id, ::amsr::core::StringView const method_name,
                ApplicationErrorMap const& application_error_map,
                std::unique_ptr<SerializerInterface> request_serializer,
                std::shared_ptr<DeserializerInterface> response_deserializer,
                std::shared_ptr<amsr::someip_binding_core::internal::MemoryBufferAllocator> allocator) noexcept
      : ProxyMethodXfInterface<OutputType, ArgsT...>{},
        ProxyMethodResponseHandlerInterface{},
        local_client_{local_client},
        service_id_{service_id},
        instance_id_{instance_id},
        major_version_{major_version},
        client_id_{client_id},
        method_id_{method_id},
        method_name_{method_name},
        // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
        pending_request_map_{std::make_shared<PendingRequestMapType>()},
        // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
        application_error_map_{std::make_shared<ApplicationErrorMap const>(application_error_map)},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        request_serializer_{std::move(request_serializer)},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        response_deserializer_{response_deserializer},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        tx_buffer_allocator_{allocator} {
    // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
    local_client_.RegisterMethodResponseHandler(method_id_, this);

    if (response_deserializer_ == nullptr) {
      // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Response deserializer is invalid.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Destructor.
   *        Removes the callback for response handler link from Binding Client to this MethodXf.
   * \spec
   *   requires true;
   * \endspec
   *
   * \steady FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ProxyMethodXf() noexcept final { local_client_.UnregisterMethodResponseHandler(method_id_); }

  ProxyMethodXf& operator=(ProxyMethodXf&&) & = delete;
  ProxyMethodXf(ProxyMethodXf const&) = delete;
  ProxyMethodXf& operator=(ProxyMethodXf const&) & = delete;
  ProxyMethodXf(ProxyMethodXf&&) = delete;

  /*!
   * \brief Handle received method response and forward the result to the response handler
   *
   * \param[in] method_response Variant of the method response
   *
   * \pre              A response handler has been registered
   * \context          Reactor
   * \threadsafe       FALSE
   * \reentrant        FALSE
   * \synchronous      FALSE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   *
   * \internal
   * - Extract the request from the pending request map
   * - If the request is found
   *   - Check response type
   *     - If it is the PositiveResponse
   *       - Create a PositiveMethodResponseXf object
   *       - Set the response executable into the promise
   *     - Otherwise, SetError for both ApplicationErrorResponse and ErrorResponse
   * - Otherwise, log warning
   * \endinternal
   *
   * \trace SPEC-10144398
   */
  void HandleMethodResponse(
      ::amsr::someip_binding_core::internal::methods::ProxyMethodResponseHandlerInterface::MethodResponseType&&
          method_response) noexcept final {
    ::amsr::someip_protocol::internal::SessionId const session_id{
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        ::amsr::core::visit([](auto const& response) { return response.GetSessionId(); }, method_response)};
    // Find the promise in the list of pending requests
    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    PromiseOptional request{pending_request_map_->MoveOutRequest(session_id)};
    if (request.has_value()) {  // Promise found
      logger_.LogDebug(
          [&session_id, this](ara::log::LogStream& s) {
            s << "SOME/IP method response (ServiceID = 0x";
            s << ara::log::HexFormat(service_id_);
            s << ", InstanceID = 0x";
            s << ara::log::HexFormat(instance_id_);
            s << ", MethodID = 0x";
            s << ara::log::HexFormat(method_id_);
            s << ", SessionID = 0x";
            s << ara::log::HexFormat(session_id);
            s << ")";
          },
          static_cast<char const*>(__func__), __LINE__);

      ::amsr::someip_binding_core::internal::methods::PositiveResponse* const positive_response{
          method_response.get_if<::amsr::someip_binding_core::internal::methods::PositiveResponse>()};
      if (positive_response != nullptr) {
        ::amsr::someip_binding_core::internal::MemoryBufferPtr buffer{positive_response->ReleaseBuffer()};
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        PositiveMethodResponseXf<Output> response{service_id_, method_id_, session_id, std::move(buffer),
                                                  response_deserializer_};

        request.value().SetExecutable(std::move(response));
      } else {
        ::amsr::someip_binding_core::internal::methods::ApplicationErrorResponse const* const
            application_error_response{
                method_response.get_if<::amsr::someip_binding_core::internal::methods::ApplicationErrorResponse>()};

        if (application_error_response != nullptr) {
          ::amsr::core::Optional<::amsr::core::ErrorCode> const application_error{
              // VCA_SOMEIPBINDING_PASSING_REFERENCE
              ApplicationErrorBuilder::CreateErrorCode(*application_error_map_, *application_error_response,
                                                       kDefaultApplicationErrorUserMessage)};
          if (application_error.has_value()) {
            request.value().SetError(application_error.value());
          } else {
            // No match or multiple matchs for the given application error are found.
            request.value().SetError(ara::com::ComErrc::kNetworkBindingFailure);
          }
        } else {
          ::amsr::someip_binding_core::internal::methods::ErrorResponse const* const error_response{
              method_response.get_if<::amsr::someip_binding_core::internal::methods::ErrorResponse>()};

          ::amsr::core::ErrorCode const error_code{
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              CreateErrorCode(error_response->GetServiceId(), error_response->GetReturnCode())};

          request.value().SetError(error_code);
        }
      }
    } else {  // Promise not found
      logger_.LogWarn(
          [&session_id](::ara::log::LogStream& s) {
            s << "No request found for the received response with session ID: (0x" << ::ara::log::HexFormat(session_id)
              << "). Possible reason: Method has been cancelled. Response dropped.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Reply to all pending requests with error responses
   * \param[in]        stopped_service The stopped service instance.
   * \context          Reactor
   * \threadsafe       FALSE
   * \reentrant        FALSE
   * \synchronous      FALSE
   * \steady           FALSE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \spec
   *   requires true;
   * \endspec
   */
  void OnServiceInstanceDown(
      someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final {
    static_cast<void>(stopped_service);
    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    pending_request_map_->RejectAll();
  }

  /*!
   * \brief Get the current count of pending requests from the request map.
   * \details this is only used for testing
   * \return pending request count.
   * \context     Testing
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetPendingRequestCount() const noexcept final { return pending_request_map_->GetPendingRequestCount(); }

 private:
  /*!
   * \brief Serialization writer.
   */
  using Writer = ::amsr::someip_protocol::internal::serialization::Writer;

  /*!
   * \brief Type alias for logger.
   */
  using Logger = ::amsr::someip_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for session handler.
   */
  using Session = ::amsr::someip_binding_core::internal::SessionHandler;

  /*!
   * \brief Serialization of a method.
   * \details In case of memory allocation failure log and exit.
   *
   * \param[in] args Value of method argument.
   * \return amsr::core::Future for return value of the method.
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Product Private
   *
   * \internal
   * - Get the session ID for this method request.
   * - Construct the SOME/IP header using session ID and other relevant information.
   * - Allocate memory and serialize the packet contents.
   * - Pass the serialized request to "ProxyRequestHandler" for further transmission.
   * \endinternal
   *
   * \trace SPEC-4981292, SPEC-4980087, SPEC-4980122
   * \trace SPEC-4981447, SPEC-4981448
   * \trace SPEC-8053501, SPEC-10144311, SPEC-10144312, SPEC-10144368, SPEC-10144369, SPEC-10144370
   * \trace SPEC-10144387, SPEC-10144394, SPEC-10144371, SPEC-10144392
   */
  ResponseValueFuture HandleRequest(ArgsT const&... args) noexcept final {
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    // Method Request format:
    // <SOME/IP Header>
    //   <not covered by length field>: 8 bytes
    // ||             ServiceID               |             MethodID                ||
    // ||                               LengthField                                 ||
    //   </not covered by length field>
    //   <covered by length field>: 8 bytes
    // ||             ClientId                |             SessionId               ||
    // || ProtocolVersion  | InterfaceVersion | MessageType      | ReturnCode       ||
    //   </covered by length field>
    // </SOME/IP Header>
    // <Payload>: ? bytes (depends on the args)
    // ||                               Payload                                     ||
    // </Payload>

    // Get the exact required buffer size to serialize the given request arguments.
    // VCA_SOMEIPBINDING_PACKET_SERIALIZER_LIFECYCLE
    std::size_t const payload_size{request_serializer_->GetRequiredBufferSize((args)...)};

    // Allocate enough memory for complete packet serialization (including header)
    std::size_t const alloc_size{::amsr::someip_protocol::internal::kHeaderSize + payload_size};
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::core::Result<BufferPtrType> packet{tx_buffer_allocator_->Allocate(alloc_size)};
    if (!packet.HasValue()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Failed to allocate memory buffer.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Serialize payload
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const payload_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        packet.Value()->GetView(kPayloadOffset)};
    SerializeRequestArgs(payload_view, (args)...);

    // Transport the Request
    ResponseValueFuture result{TransportRequest(std::move(packet.Value()))};

    return result;
  }

  /*!
   * \brief Generate SOME/IP header for the next request.
   * \param[in] payload_size  size of the payload to calculate the length field.
   * \return The generated header. All header parameters for this method would be the same for all requests except for:
   *         Session ID:  increments for each request, and
   *         length field: depends on the given payload.
   *
   * \pre              Payload length must not exceed the maximum allowed length (32 bits field excluding 8 bytes
   *                   for the first part of the SOME/IP Header)
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   */
  ::amsr::someip_protocol::internal::SomeIpMessageHeader GenerateRequestHeader(
      std::size_t const payload_size) noexcept {
    // Validate length field
    if (payload_size > (kMaximumSomeIpPayloadLength)) {
      logger_.LogFatalAndAbort(
          [&payload_size, this](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Violation: Length field overflow during serialization.";
            s << " Payload length ";
            s << payload_size;
            s << ". Maximum Payload length ";
            s << kMaximumSomeIpPayloadLength;
            s << ". Please select another datatype for this method that can be serialized using SOME/IP Protocol. "
                 "Service ID: ";
            s << service_id_;
            s << " Major Version: ";
            s << major_version_;
            s << " Method ID: ";
            s << method_id_;
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // Build SOME/IP header
    ::amsr::someip_protocol::internal::SomeIpMessageHeader header{};
    header.service_id_ = service_id_;
    header.method_id_ = method_id_;
    header.client_id_ = client_id_;
    header.message_type_ = ::amsr::someip_protocol::internal::SomeIpMessageType::kRequest;
    header.protocol_version_ = ::amsr::someip_protocol::internal::kProtocolVersion;
    header.interface_version_ = major_version_;
    header.return_code_ = ::amsr::someip_protocol::internal::SomeIpReturnCode::kOk;
    // payload + 8 bytes of SOME/IP header are covered by this length field
    header.length_ = static_cast<::amsr::someip_protocol::internal::LengthField>(
        payload_size + ::amsr::someip_protocol::internal::kHeaderLength);

    // Compute next session ID for header
    {
      // Ensure thread-safe increment of session id
      std::lock_guard<std::mutex> const guard{session_id_lock_};
      ::amsr::someip_protocol::internal::SessionId const session_id{session_};
      // Update the header
      header.session_id_ = session_id;
      // Increment the session id
      ++session_;
    }

    return header;
  }

  /*!
   * \brief Serialize the given SOME/IP header into the given view.
   * \param[in] header_view  view where the header shall be serialized.
   * \param[in] header       SOME/IP header to be serialized.
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   */
  static void SerializeHeader(amsr::core::Span<std::uint8_t> header_view,
                              ::amsr::someip_protocol::internal::SomeIpMessageHeader const& header) noexcept {
    Writer writer{header_view};

    // Serialize SOME/IP header and payload
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    ::amsr::someip_protocol::internal::serialization::SerializeSomeIpMessageHeaderObject(writer, header);
  }

  /*!
   * \brief Serialize the given request arguments into the given view.
   * \param[in] payload_view  view where the arguments shall be serialized.
   * \param[in] args          The arguments to be serialized.
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   */
  void SerializeRequestArgs(amsr::core::Span<std::uint8_t> payload_view, ArgsT const&... args) const noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    Writer writer{payload_view};
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    request_serializer_->Serialize(writer, (args)...);
  }

  /*!
   * \brief Send the method request to the server.
   * \param[in] packet  The packet to be sent (containing the memory for the serialized header + the serialized
   *                    payload).
   *  \return amsr::core::Future for return value of the method.
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   */
  ResponseValueFuture TransportRequest(BufferPtrType packet) noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    ResponseValueFuture result{};

    // Build & Serialize SOME/IP header
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const payload_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        packet->GetView(kPayloadOffset)};
    std::size_t const payload_size{payload_view.size()};
    ::amsr::someip_protocol::internal::SomeIpMessageHeader const someip_header{GenerateRequestHeader(payload_size)};
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const header_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        packet->GetView(kSomeIpHeaderOffset)};
    SerializeHeader(header_view, someip_header);

    // Transmit request
    {  // Ensure thread-safe request transmission
      std::lock_guard<std::mutex> const guard{transmission_lock_};
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      FutureOptional added_request{pending_request_map_->StoreRequestInMap(someip_header.session_id_)};
      if (added_request.has_value()) {
        logger_.LogDebug(
            [&someip_header, this](ara::log::LogStream& s) {
              s << "SOME/IP method request (ServiceID = 0x";
              s << ara::log::HexFormat(someip_header.service_id_);
              s << ", InstanceID = 0x";
              s << ara::log::HexFormat(instance_id_);
              s << ", MethodID = 0x";
              s << ara::log::HexFormat(someip_header.method_id_);
              s << ", SequenceID = 0x";
              s << ara::log::HexFormat(someip_header.session_id_);
              s << ")";
            },
            static_cast<char const*>(__func__), __LINE__);

        result = std::move(added_request.value());
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE_POINTER_PARAMETER
        amsr::core::Result<void> const request_sent{local_client_.SendMethodRequest(std::move(packet))};
        if (!request_sent.HasValue()) {
          logger_.LogError([](ara::log::LogStream& s) { s << "Transmission error: Method request was not sent"; },
                           static_cast<char const*>(__func__), __LINE__);
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          PromiseOptional request{pending_request_map_->MoveOutRequest(someip_header.session_id_)};

          // The request was added to the map in this context, so it must be found
          assert(request.has_value());  // COV_MSR_INV_STATE_ASSERT

          request.value().SetError(
              {ara::com::ComErrc::kNetworkBindingFailure, "Transmission error: Method request was not sent."});
        }
      } else {
        // Error handling: Session ID already found.

        // VECTOR NL AutosarC++17_10-A7.1.1: MD_SOMEIPBINDING_AutosarC++17_10-A7.1.1_Immutable_shall_be_qualified_const
        ResponseValuePromise promise{};
        promise.SetError(
            {ara::com::ComErrc::kNetworkBindingFailure, "Session id already in use: Method request was not sent."});

        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "Failed to emplace promise object for a method request in pending requests map. Request will be "
                   "dropped and is not sent.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result = promise.get_future();
      }
    }
    return result;
  }

  /*!
   * \brief Return error code based on the return code retrieved from the negative response.
   *
   * \param[in] service_id  Service ID.
   * \param[in] return_code Return code value
   *
   * \return  Error code
   * \pre     -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Return error code based on the return code retrieved from the negative response.
   * \endinternal
   */
  static ::amsr::core::ErrorCode CreateErrorCode(
      ::amsr::someip_protocol::internal::ServiceId const& service_id,
      ::amsr::someip_protocol::internal::SomeIpReturnCode const& return_code) noexcept {
    ::amsr::core::ErrorCode error_code{ara::com::ComErrc::kNetworkBindingFailure};
    // Handle specific negative responses
    switch (return_code) {
      case (::amsr::someip_protocol::internal::SomeIpReturnCode::kUnknownService): {
        error_code = ::amsr::core::ErrorCode{ara::com::ComErrc::kServiceNotAvailable,
                                             static_cast<std::int32_t>(service_id), "Requested service is not known."};
        break;
      }
      case (::amsr::someip_protocol::internal::SomeIpReturnCode::kNotReady): {
        error_code =
            ::amsr::core::ErrorCode{ara::com::ComErrc::kServiceNotAvailable, static_cast<std::int32_t>(service_id),

                                    "Requested service is not ready."};

        break;
      }
      case (::amsr::someip_protocol::internal::SomeIpReturnCode::kMalformedMessage): {
        error_code = ::amsr::core::ErrorCode{
            ara::com::ComErrc::kNetworkBindingFailure,
            "Malformed SOME/IP message: Service replied with return code 'E_MALFORMED_MESSAGE'"};

        break;
      }
      case (::amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk): {
        error_code = ::amsr::core::ErrorCode{ara::com::ComErrc::kNetworkBindingFailure,
                                             "Unspecified protocol error: Service replied with return code 'E_NOT_OK'"};

        break;
      }
      case (::amsr::someip_protocol::internal::SomeIpReturnCode::kNotReachable): {
        error_code = ::amsr::core::ErrorCode{
            ara::com::ComErrc::kNetworkBindingFailure,
            "Unreachable peer or IAM access denied: Service replied with return code 'E_NOT_REACHABLE'"};
        break;
      }
      default:
        // Nothing to do
        break;
    }

    return error_code;
  }

  /*!
   * \brief payload offset within the method request
   */
  static std::size_t const kPayloadOffset{::amsr::someip_protocol::internal::kHeaderSize};

  /*!
   * \brief maximum allowed payload length (including E2E) of a SOME/IP message.
   */
  static std::size_t const kMaximumSomeIpPayloadLength{
      std::numeric_limits<::amsr::someip_protocol::internal::LengthField>::max() -
      ::amsr::someip_protocol::internal::kMinimumPayloadLength};

  /*!
   * \brief SOME/IP header offset within the method request
   */
  static std::size_t const kSomeIpHeaderOffset{0};

  /*!
   * \brief The LocalClient.
   */
  ::amsr::someip_binding_core::internal::LocalClient& local_client_;

  /*!
   * \brief SOME/IP service ID used by this binding.
   */
  ServiceId const service_id_;

  /*!
   * \brief SOME/IP instance ID used by this binding.
   */
  InstanceId const instance_id_;

  /*!
   * \brief SOME/IP major version used by this binding.
   */
  MajorVersion const major_version_;

  /*!
   * \brief SOME/IP related client ID used by this concrete proxy instance.
   */
  ClientId const client_id_;

  /*!
   * \brief SOME/IP method identifier for this method.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief SOME/IP method name for this method.
   */
  ::amsr::core::StringView const method_name_;

  /*!
   * \brief Pending request map object to store method requests.
   */
  PendingRequestMapSharedPtr pending_request_map_;

  /*!
   * \brief Provides functionality to create unique IDs for each request (session)
   */
  Session session_{};

  /*!
   * \brief Method logger for printing debug and error messages.
   * The concrete method will set the prefix for identification.
   */
  Logger logger_{::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
                 ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, method_name_};

  /*!
   * \brief Mutex to protect parallel access to the session id generator
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  std::mutex session_id_lock_{};

  /*!
   * \brief Mutex to protect parallel requests to transmit a method request
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  std::mutex transmission_lock_{};

  /*!
   * \brief Application error map shared pointer for mapping application error responses to error codes.
   * A shared pointer is used to allow calling GetResult on a request future after the corresponding proxy method xf was
   * deleted, since GetResult performs the application error response deserialization lazily which requires the map to
   * outlive the proxy method xf in case of an application error response.
   */
  std::shared_ptr<ApplicationErrorBuilder::ApplicationErrorMap const> application_error_map_;

  /*!
   * \brief Request serializer.
   */
  std::unique_ptr<SerializerInterface> request_serializer_;

  /*!
   * \brief Deserializer shared pointer for the method response payload.
   * A shared pointer is used to allow calling GetResult on a request future after the corresponding proxy method xf was
   * deleted, since GetResult performs the method response payload deserialization lazily which requires the
   * deserializer to outlive the proxy method xf in case of a positive response.
   */
  std::shared_ptr<DeserializerInterface> response_deserializer_;

  /*!
   * \brief Memory buffer allocator.
   */
  std::shared_ptr<amsr::someip_binding_core::internal::MemoryBufferAllocator> tx_buffer_allocator_{};

  /*!
   * \brief Default user message text for error codes created for ApApplicationErrors.
   * \details The error code object stores only a pointer to the user message. Therefore the error code creator must
   * ensure that the string lifetime exceeds the lifetime of the error code.
   * This lifetime management is currently not supported. Therefore a constant empty string is used for all user
   * messages.
   */
  static constexpr ::amsr::core::ErrorDomain::StringType kDefaultApplicationErrorUserMessage{""};
};

/*!
 * \brief Initialization of static Default Application error user message.
 */
template <typename OutputType, typename... ArgsT>
constexpr ::amsr::core::ErrorDomain::StringType
    ProxyMethodXf<OutputType, ArgsT...>::kDefaultApplicationErrorUserMessage;

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_METHOD_XF_H_

/*!
 * \exclusivearea ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf::session_id_lock_
 *                Used for protecting simultaneous calls to HandleMethodRequest
 *
 * \protects ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf::session_
 *           is the protected resource of an ProxyMethodXf object.
 *
 * \usedin Generated code method request (read, write)
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length SHORT counter increment/reset.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf::transmission_lock_
 *                Used for protecting simultaneous calls to HandleMethodRequest
 *
 * \protects ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf::proxy_binding_,
 *           ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf::pending_request_map_,
 *           are the protected resources of an ProxyMethodXf object.
 *
 * \usedin Generated code method request (read, write)
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length LONG IPC communication.
 * \endexclusivearea
 */
