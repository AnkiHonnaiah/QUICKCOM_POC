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
/**        \file  someip_event_serializer.h
 *        \brief  SOME/IP Event serializer
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SomeIpEventSerializer
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_EVENT_SERIALIZER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_EVENT_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding_core/internal/session_handler.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/e2e/ser_e2e_header.h"
#include "amsr/someip_binding_xf/internal/e2e/server_side_transformer.h"
#include "amsr/someip_binding_xf/internal/events/event_serializer_interface.h"
#include "amsr/someip_binding_xf/internal/ser_deser/serializer_interface.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Template for SOME/IP Event Serializer.
 *
 * \tparam  SampleType Type of the serialized event sample.
 */
template <typename SampleType>
class SomeIpEventSerializer final : public EventSerializerInterface<SampleType> {
 public:
  /*!
   * \brief Type alias for SerializerInterface.
   */
  using SerializerInterface = ::amsr::someip_binding_xf::internal::ser_deser::SerializerInterface<SampleType>;

  /*!
   * \brief Type-alias for the SOME/IP protocol writer.
   */
  using Writer = ::amsr::someip_protocol::internal::serialization::Writer;

  /*!
   * \brief Type-alias for the SOME/IP protocol BufferView.
   */
  using BufferView = ::amsr::someip_protocol::internal::serialization::BufferView;

  /*!
   * \brief Type of the E2E transformer.
   */
  using E2eTransformerType = ::amsr::someip_binding_xf::internal::e2e::ServerSideTransformer;

  /*!
   * \brief Type alias for EventSerializerInterface.
   */
  using EventSerializerInterface = ::amsr::someip_binding_xf::internal::events::EventSerializerInterface<SampleType>;

  /*!
   * \brief     Constructor of SomeIpEventSerializer.
   *
   * \param[in] someip_event_identity     SOME/IP event identity.
   * \param[in] serializer                Reference to the the payload serializer.
   * \param[in] e2e_parametrization_data  Event E2E parametrization data. Nullopt if not E2E protected.
   * \param[in] session_handling_disabled Session handling flag
   *
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  SomeIpEventSerializer(
      ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
      SerializerInterface& serializer,
      amsr::core::Optional<
          ::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData> const&
          e2e_parametrization_data,
      bool const session_handling_disabled)
      : EventSerializerInterface{},
        someip_event_identity_{someip_event_identity},
        session_handling_disabled_{session_handling_disabled},
        is_e2e_protected_{e2e_parametrization_data.has_value()},
        serializer_{serializer} {
    if (is_e2e_protected_) {
      e2e_transformer_.emplace(e2e_parametrization_data.value().e2e_profile,
                               e2e_parametrization_data.value().e2e_event_protection_props);
      e2e_header_size_ = e2e_transformer_.value().GetHeaderSize();
    }
  }

  /*!
   * \brief       Default destructor.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~SomeIpEventSerializer() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  SomeIpEventSerializer(SomeIpEventSerializer const&) = delete;
  SomeIpEventSerializer& operator=(SomeIpEventSerializer const&) = delete;
  SomeIpEventSerializer(SomeIpEventSerializer&&) = delete;
  SomeIpEventSerializer& operator=(SomeIpEventSerializer&&) = delete;

  /*!
   * \brief Returns the required buffer size to serialize the given datatype.
   *
   * \param[in]   data Reference to data object.
   *
   * \return      Calculated buffer size for serialization.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetRequiredBufferSize(SampleType const& data) const noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return serializer_.GetRequiredBufferSize(data);
  }

  /*!
   * \brief       Returns the maximum buffer size to serialize the given datatype.
   *
   * \return      The maximum buffer size needed for serializing this datatype if it has finite length or an error code
   *              if it cannot be determined.
   * \error       SomeIpBindingErrc::error_not_ok if the current datatype is dynamic without a predefined maximum size.
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Result<std::size_t> GetMaximumBufferSize() const noexcept final {
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return serializer_.GetMaximumBufferSize();
  }

  /*!
   * \brief Serializes SOME/IP event notification packet.
   *
   * \param[in,out] body_view buffer to serialize into.
   * \param[in] payload_size Size of the event sample payload.
   * \param[in] data Event sample value.
   *
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-10144339
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - In case event is not e2e protected:
   * - Build SOME/IP header logical struct and serialize the SOME/IP header into the allocated memory buffer.
   * - Serialize the event sample payload into the memory buffer.
   *  - In case event is e2e protected:
   * - Build SOME/IP header logical struct and serialize the SOME/IP header into the allocated memory buffer.
   * - Instantiate an E2E header serializer to allocate the memory for the E2E header later filled with contents.
   * - Serialize the event sample payload into the memory buffer.
   * - Finalize E2E header serialization (header will be filled with data).
   * \endinternal
   */
  void SerializeEvent(BufferView& body_view, std::size_t const payload_size, SampleType const& data) noexcept final {
    // Size to be written in the length field (E2E header size + Payload size)
    std::size_t const someip_payload_size{e2e_header_size_ + payload_size};
    // Fill SOME/IP header with data for the request
    ::amsr::someip_protocol::internal::SomeIpMessageHeader const header{BuildSomeIpHeader(someip_payload_size)};

    Writer writer{body_view};
    // writer: offset = 0
    // payload: [-, -, -]

    // Serialize SOME/IP header
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    ::amsr::someip_protocol::internal::serialization::SerializeSomeIpMessageHeaderObject(writer, header);

    // writer: offset = sizeof(header): 16
    // payload: [SOME/IP Header, -, -]

    if (is_e2e_protected_) {
      // Serialize E2E header
      ::amsr::someip_binding_xf::internal::e2e::E2EHeaderSerializer e2e_header_serializer{
          writer, e2e_transformer_.value(), body_view,
          static_cast<std::uint8_t>(::amsr::someip_protocol::internal::kHeaderLength)};

      // writer: offset = sizeof(header): 16 + E2E_Header_Size
      // payload: [SOME/IP Header, -, -]

      // Serialize the event sample
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      serializer_.Serialize(writer, data);

      // writer: offset = sizeof(header): 16 + E2E_Header_Size + Payload
      // payload: [SOME/IP Header, -, Payload]

      // Finally close the E2E header serializer which updates the attributes of the already allocated E2E header.
      e2e_header_serializer.Close();

      // writer: offset = sizeof(header): 16 + E2E_Header_Size + Payload
      // payload: [SOME/IP Header, E2E Header, Payload]
    } else {
      // Serialize the event sample
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      serializer_.Serialize(writer, data);
    }
  }

  /*!
   * \brief Calculate the size of the required packet header for SOME/IP event.
   *
   * \return Size of the packet header in bytes (SOME/IP Header size [+ E2E header size]).
   *
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t CalculateHeaderSize() const noexcept final {
    std::size_t header_size{};

    std::size_t const e2e_header_size{is_e2e_protected_ ? e2e_transformer_.value().GetHeaderSize()
                                                        : static_cast<std::size_t>(0U)};
    header_size = ::amsr::someip_protocol::internal::kHeaderSize + e2e_header_size;

    return header_size;
  }

 private:
  /*!
   * \brief Build SOME/IP header for the event sample to be transmitted.
   * \param[in] payload_size Size of the event sample payload.
   * \return The created SOME/IP header struct.
   *
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Get current session ID
   * - Increment session ID for next event sample transmission.
   * - Fill SOME/IP header struct.
   * \endinternal
   * \trace SPEC-10144311, SPEC-10144312, SPEC-10144337, SPEC-10144338, SPEC-10144336
   */
  ::amsr::someip_protocol::internal::SomeIpMessageHeader BuildSomeIpHeader(std::size_t const payload_size) noexcept {
    ::amsr::someip_protocol::internal::SessionId session_id{0};
    if (!session_handling_disabled_) {
      // Get session ID and increment for next Send()
      ::amsr::someip_protocol::internal::SessionId const generated_session_id{session_handler_};
      session_id = generated_session_id;
      ++session_handler_;
    }
    // Fill SOME/IP header with data for the request
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    ::amsr::someip_protocol::internal::SomeIpMessageHeader header;
    header.service_id_ = someip_event_identity_.GetServiceId();
    header.method_id_ = someip_event_identity_.GetEventId();
    header.client_id_ = 0U;
    header.session_id_ = session_id;
    header.message_type_ = ::amsr::someip_protocol::internal::SomeIpMessageType::kNotification;
    header.protocol_version_ = ::amsr::someip_protocol::internal::kProtocolVersion;
    header.interface_version_ = someip_event_identity_.GetMajorVersion();
    header.return_code_ = ::amsr::someip_protocol::internal::SomeIpReturnCode::kOk;
    header.length_ = static_cast<someip_protocol::internal::LengthField>(
        ::amsr::someip_protocol::internal::kMinimumPayloadLength + payload_size);

    return header;
  }

  /*!
   * \brief SOME/IP ID of this event.
   */
  someip_binding_core::internal::SomeIpEventIdentity const someip_event_identity_;

  /*!
   * \brief Session handling flag, indicates whether session handling is disabled or not.
   */
  bool const session_handling_disabled_;

  /*!
   * \brief SE2E protected flag, indicates whether E2E protection is disabled or not.
   */
  bool const is_e2e_protected_;

  /*!
   * \brief E2E header size (0 if E2E is not used)
   */
  std::size_t e2e_header_size_{};

  /*!
   * \brief Payload serializer.
   */
  SerializerInterface& serializer_;

  /*!
   * \brief E2E server-side transformer.
   */
  amsr::core::Optional<E2eTransformerType> e2e_transformer_{amsr::core::nullopt};

  /*!
   * \brief Session handler.
   */
  ::amsr::someip_binding_core::internal::SessionHandler session_handler_{};
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_EVENT_SERIALIZER_H_
