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
/**        \file  someip_binding_xf/internal/methods/proxy_fire_and_forget_method_xf.h
 *        \brief  Method manager to handle common logic for method requests/responses on proxy side within SOME/IP
 *                binding.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/session_handler.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/methods/proxy_fire_and_forget_method_xf_interface.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_serializer_interface.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief Method manager to handle SOME/IP-binding method request/response on proxy side.
 * \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyFireAndForgetMethodXf
 *
 * \details This is the base class for the generated Proxy<ClientServerOperation.shortName> classes
 *          and maintains the logger and pending request map for specific methods.
 *
 * \tparam ArgsT The Variadic template parameter.
 */
template <typename... ArgsT>
class ProxyFireAndForgetMethodXf final : public ProxyFireAndForgetMethodXfInterface<ArgsT...> {
 public:
  /*!
   * \brief Type alias for the concrete memory buffer type.
   */
  using BufferPtrType = ::amsr::someip_binding_core::internal::MemoryBufferPtr;

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
   * \brief Type alias for MethodRequestSerializerInterface.
   */
  using SerializerInterface = internal::ser_deser::MethodRequestSerializerInterface<ArgsT...>;

  /*!
   * \brief Constructor sets a reference to the ProxyBinding, for sending out the serialized packets.
   *
   * \param[in]  local_client        The LocalClient.
   * \param[in]  service_id          SOME/IP service ID used by this binding.
   * \param[in]  instance_id         SOME/IP instance ID used by this binding.
   * \param[in]  major_version       SOME/IP major version used by this binding.
   * \param[in]  client_id           SOME/IP related client ID used by this concrete proxy instance.
   * \param[in]  method_id           SOME/IP method identifier for this method.
   * \param[in]  method_name         SOME/IP method name.
   * \param[in]  request_serializer  Method request serializer.
   * \param[in]  allocator           Allocator for memory used to serialize/send method request.
   *
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  ProxyFireAndForgetMethodXf(
      ::amsr::someip_binding_core::internal::LocalClient& local_client, ServiceId const service_id,
      InstanceId const instance_id, MajorVersion const major_version, ClientId const client_id,
      MethodId const method_id, ::amsr::core::StringView const method_name,
      std::unique_ptr<SerializerInterface> request_serializer,
      std::shared_ptr<amsr::someip_binding_core::internal::MemoryBufferAllocator> allocator) noexcept
      : ProxyFireAndForgetMethodXfInterface<ArgsT...>{},
        local_client_{local_client},
        service_id_{service_id},
        instance_id_{instance_id},
        major_version_{major_version},
        client_id_{client_id},
        method_id_{method_id},
        method_name_{method_name},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        request_serializer_{std::move(request_serializer)},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        tx_buffer_allocator_{allocator} {}

  /*!
   * \brief Default Destructor.
   * \spec
   *   requires true;
   * \endspec
   *
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ProxyFireAndForgetMethodXf() final = default;

  ProxyFireAndForgetMethodXf& operator=(ProxyFireAndForgetMethodXf&&) = delete;
  ProxyFireAndForgetMethodXf(ProxyFireAndForgetMethodXf const&) = delete;
  ProxyFireAndForgetMethodXf& operator=(ProxyFireAndForgetMethodXf const&) = delete;
  ProxyFireAndForgetMethodXf(ProxyFireAndForgetMethodXf&&) = delete;

 private:
  /*!
   * \brief Serialization writer.
   */
  using Writer = ::amsr::someip_protocol::internal::serialization::Writer;

  /*!
   * \brief Serialization of a method
   * \details In case of memory allocation failure log and exit.
   *
   * \param[in] args Value of method argument.
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
   * - Get the session id for this method request.
   * - Construct the SOME/IP header using session id and other relevant information.
   * - Allocate memory and serialize the packet contents.
   * - Pass the serialized request along with the serialization status to ProxyRequestHandler for
   *   further transmission or error handling.
   * \endinternal
   *
   * \trace SPEC-4981292, SPEC-4980087, SPEC-4980122, SPEC-5951541, SPEC-4981304, SPEC-4981453
   * \trace SPEC-10144311, SPEC-10144312, SPEC-10144368, SPEC-10144394, SPEC-10144409
   * \trace SPEC-10144410, SPEC-10144411, SPEC-10144392
   */
  void HandleRequest(ArgsT const&... args) noexcept final {
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

    // Transmit request
    ::amsr::core::Result<void> const result{TransportRequest(std::move(packet.Value()))};
    if (!result.HasValue()) {
      logger_.LogError(
          [](ara::log::LogStream& s) { s << "Transmission error: Fire&Forget method request was not sent"; },
          static_cast<char const*>(__func__), __LINE__);
    }
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
    header.message_type_ = ::amsr::someip_protocol::internal::SomeIpMessageType::kRequestNoReturn;
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
   * \brief  Generates SOME/IP header for the method request and sends it to the server.
   * \param[in] packet  The packet to be sent (containing the memory for the serialized header + the serialized
   *                    payload).
   * \return void if the message was sent successfully, and error otherwise.
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   */
  ::amsr::core::Result<void> TransportRequest(BufferPtrType&& packet) noexcept {
    ::amsr::core::Result<void> result{};

    // Build & Serialize SOME/IP header
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const payload_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        packet->GetView(kPayloadOffset)};
    std::size_t const payload_size{payload_view.size()};

    ::amsr::someip_protocol::internal::SomeIpMessageHeader const someip_header{GenerateRequestHeader(payload_size)};
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

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const header_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        packet->GetView(kSomeIpHeaderOffset)};
    SerializeHeader(header_view, someip_header);

    // Transmit request
    {  // Ensure thread-safe request transmission
      std::lock_guard<std::mutex> const guard{transmission_lock_};
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE_POINTER_PARAMETER
      result = local_client_.SendMethodRequest(std::move(packet));
    }
    return result;
  }

 protected:
  /*!
   * \brief Type alias for logger.
   */
  using Logger = ::amsr::someip_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for session handler.
   */
  using Session = ::amsr::someip_binding_core::internal::SessionHandler;

 private:
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
   * \brief SOME/IP related client ID used by this concrete proxy instance
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
   * \brief request Serializer.
   */
  std::unique_ptr<SerializerInterface> request_serializer_;

  /*!
   * \brief Memory buffer allocator.
   */
  std::shared_ptr<amsr::someip_binding_core::internal::MemoryBufferAllocator> tx_buffer_allocator_;

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
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_XF_H_

/*!
 * \exclusivearea
 * ::amsr::someip_binding_xf::internal::methods::ProxyFireAndForgetMethodXf::session_id_lock_ Used for
 * protecting simultaneous calls to HandleMethodRequest
 *
 * \protects ::amsr::someip_binding_xf::internal::methods::ProxyFireAndForgetMethodXf::session_
 *           is the protected resource of an ProxyFireAndForgetMethodXf object.
 *
 * \usedin Generated code method request (read, write)
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length SHORT counter increment/reset.
 * \endexclusivearea
 */

/*!
 * \exclusivearea
 * ::amsr::someip_binding_xf::internal::methods::ProxyFireAndForgetMethodXf::transmission_lock_ Used
 * for protecting simultaneous calls to HandleMethodRequest
 *
 * \protects ::amsr::someip_binding_xf::internal::methods::ProxyFireAndForgetMethodXf::proxy_binding_
 *           are the protected resources of an ProxyFireAndForgetMethodXf object.
 *
 * \usedin Generated code method request (read, write)
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length LONG IPC communication.
 * \endexclusivearea
 */
