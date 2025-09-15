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
/**        \file  someip_binding_xf/internal/methods/positive_method_response_xf.h
 *        \brief  Positive Method response transformation layer
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyMethodXf
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_POSITIVE_METHOD_RESPONSE_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_POSITIVE_METHOD_RESPONSE_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/variant.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_xf/internal/methods/application_error_builder.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_deserializer_interface.h"
#include "ara/com/com_error_domain.h"
#include "someip-protocol/internal/deserialization/deser_ap_application_error.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someipd_app_protocol/internal/someip_message.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief   Positive Method Response Xf class.
 *
 * \details This the serialized response message and provides deserialization functionality for
 *          this message.
 * \remark  Deserialization is triggered by SOCAL.
 *
 * \tparam  Output    The Output type for method response.
 */
template <typename Output>
class PositiveMethodResponseXf final {
 public:
  /*!
   * \brief Type alias for Deserializer.
   */
  using DeserializerInterface = internal::ser_deser::MethodResponseDeserializerInterface<Output>;

  PositiveMethodResponseXf(PositiveMethodResponseXf const&) = delete;
  PositiveMethodResponseXf& operator=(PositiveMethodResponseXf const&) & = delete;
  /*!
   * \brief Default move assignment operator
   * \return Reference to the move-assigned PositiveMethodResponseXf.
   * \spec
   *   requires true;
   * \endspec
   */
  PositiveMethodResponseXf& operator=(PositiveMethodResponseXf&&) & noexcept = default;
  /*!
   * \brief Default move constructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  PositiveMethodResponseXf(PositiveMethodResponseXf&&) noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~PositiveMethodResponseXf() noexcept = default;

  /*!
   * \brief Constructor
   * \param[in] service_id             Service ID
   * \param[in] method_id              Method ID
   * \param[in] session_id             Session ID
   * \param[in] buffer                 buffer containing header + payload of the SOME/IP event message.
   * \param[in] response_deserializer  Shared pointer to a deserializer for the method response payload.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  PositiveMethodResponseXf(::amsr::someip_protocol::internal::ServiceId const& service_id,
                           ::amsr::someip_protocol::internal::MethodId const& method_id,
                           ::amsr::someip_protocol::internal::SessionId const& session_id,
                           ::amsr::someip_binding_core::internal::MemoryBufferPtr buffer,
                           std::shared_ptr<DeserializerInterface> const response_deserializer) noexcept
      : service_id_{service_id},
        method_id_{method_id},
        session_id_{session_id},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        buffer_{std::move(buffer)},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        response_deserializer_{response_deserializer} {}

  /*!
   * \brief   Deserialize the positive response.
   * \return  Result containing an output value or an error.
   * \pre     -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Product Private
   * \internal
   * - Deserialize positive response and return an output value or an error
   * \endinternal
   */
  ::amsr::core::Result<Output, amsr::core::ErrorCode> operator()() const noexcept {
    ::amsr::core::Result<Output, amsr::core::ErrorCode> result{ara::com::ComErrc::kNetworkBindingFailure};

    // Prepare Reader
    ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const packet_view{
        // VCA_SOMEIPBINDING_DATA_BUFFER_UNIQUE_PTR_VALIDITY
        buffer_->GetView(0U)};
    // Calling front on an empty container causes undefined behavior
    assert(!packet_view.empty());  // COV_MSR_INV_STATE_ASSERT
    // VECTOR Next Construct AutosarC++17_10-M5.2.8:MD_SOMEIPBINDING_AutosarC++17_10-M5.2.8_conv_from_voidp
    ::amsr::someip_protocol::internal::deserialization::BufferView const body_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        packet_view.data(), packet_view.size()};

    // Skip the header
    ::amsr::someip_protocol::internal::deserialization::BufferView const buffer_view{
        body_view.subspan(::amsr::someip_protocol::internal::kHeaderSize,
                          body_view.size() - ::amsr::someip_protocol::internal::kHeaderSize)};
    ::amsr::someip_protocol::internal::deserialization::Reader reader{buffer_view};

    // DeserializePositiveResponse
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Large_packets_allocated_on_stack
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    std::shared_ptr<Output> const output{std::make_shared<Output>()};
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    bool const deserialization_status{response_deserializer_->Deserialize(reader, *output)};
    if (deserialization_status) {
      // VCA_SOMEIPBINDING_TRIVIAL_VALID_REFERENCE
      result.EmplaceValue(*output);
    } else {
      logger_.LogError([](::ara::log::LogStream& s) { s << "Deserialization of response message failed"; },
                       static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

 private:
  /*!
   * \brief Service ID.
   */
  ::amsr::someip_protocol::internal::ServiceId const service_id_;

  /*!
   * \brief Method ID.
   */
  ::amsr::someip_protocol::internal::MethodId const method_id_;

  /*!
   * \brief Session ID.
   */
  ::amsr::someip_protocol::internal::SessionId const session_id_;

  /*!
   * \brief A buffer containing header + payload of the SOME/IP event message.
   */
  ::amsr::someip_binding_core::internal::MemoryBufferPtr buffer_;
  /*!
   * \brief       Generates a unique logger prefix.
   * \param[in]   service_id   SOME/IP service identifier.
   * \param[in]   method_id    SOME/IP method identifier.
   * \param[in]   session_id   SOME/IP session identifier.
   * \return      a string representing the logger prefix.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(::amsr::someip_protocol::internal::ServiceId const service_id,
                                            ::amsr::someip_protocol::internal::MethodId const method_id,
                                            ::amsr::someip_protocol::internal::SessionId const session_id) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{"PositiveMethodResponseXf (ServiceId: "} << service_id;
    prefix_builder << amsr::core::StringView{", MethodID: "} << method_id;
    prefix_builder << amsr::core::StringView{", SessionID: "} << session_id;
    prefix_builder << amsr::core::StringView{")"};
    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Logger Prefix.
   */
  amsr::core::String const logger_prefix_{GetLoggerPrefix(service_id_, method_id_, session_id_)};
  /*!
   * \brief Method logger for printing debug and error messages.
   * The concrete method will set the prefix for identification.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, logger_prefix_};

  /*!
   * \brief Deserializer shared pointer for the method response payload.
   * A shared pointer is used to allow calling GetResult on a request future after the corresponding proxy method xf
   * was deleted, since GetResult performs the method response payload deserialization lazily which requires the
   * deserializer to outlive the proxy method xf in case of a positive response.
   */
  std::shared_ptr<DeserializerInterface> response_deserializer_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_POSITIVE_METHOD_RESPONSE_XF_H_
