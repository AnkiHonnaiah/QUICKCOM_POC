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
/**        \file  signal_based_event_serializer.h
 *        \brief  Signal-based event serializer
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SignalBasedEventSerializer
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_EVENT_SERIALIZER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_EVENT_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding_core/internal/session_handler.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/e2e/ser_e2e_header.h"
#include "amsr/someip_binding_xf/internal/e2e/server_side_transformer.h"
#include "amsr/someip_binding_xf/internal/events/event_serializer_interface.h"
#include "amsr/someip_binding_xf/internal/ser_deser/serializer_interface.h"
#include "someip-protocol/internal/serialization/ser_pdu_header.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Template for Signal based Event Serializer.
 *
 * \tparam  SampleType Type of the serialized event sample.
 */
template <typename SampleType>
class SignalBasedEventSerializer final : public EventSerializerInterface<SampleType> {
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
   * \brief     Constructor of SignalBasedEventSerializer.
   *
   * \param[in] someip_event_identity     SOME/IP event identity.
   * \param[in] serializer                Reference to the the payload serializer.
   * \param[in] e2e_parametrization_data  Event E2E parametrization data. Nullopt if not E2E protected.
   * \param[in] pdu_header_extn           The pdu header extension. In case of an SomeIp serialized event it is
   *                                      empty.
   *
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  SignalBasedEventSerializer(
      ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
      SerializerInterface& serializer,
      amsr::core::Optional<
          ::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData> const&
          e2e_parametrization_data,
      amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionTx> const&
          pdu_header_extn)
      : EventSerializerInterface{},
        someip_event_identity_{someip_event_identity},
        is_e2e_protected_{e2e_parametrization_data.has_value()},
        serializer_{serializer},
        pdu_header_extn_{pdu_header_extn},
        kPduHeaderExtendedSize{pdu_header_extn.has_value()
                                   ? ::amsr::someip_binding::internal::configuration::EventConfig::kPduHeaderExtnLength
                                   : 0} {
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
  ~SignalBasedEventSerializer() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  SignalBasedEventSerializer(SignalBasedEventSerializer const&) = delete;
  SignalBasedEventSerializer& operator=(SignalBasedEventSerializer const&) = delete;
  SignalBasedEventSerializer(SignalBasedEventSerializer&&) = delete;
  SignalBasedEventSerializer& operator=(SignalBasedEventSerializer&&) = delete;

  /*!
   * \brief Returns the required buffer size to serialize the given datatype.
   *
   * \param[in]   data Reference to data object.
   *
   * \return      Calculated buffer size for serialization.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
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
   * \brief Serializes PDU event notification packet.
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
   * - In case pdu is not e2e protected:
   * - Build PDU header logical struct and serialize the PDU header into the allocated memory buffer.
   * - Serialize the event sample payload into the memory buffer.
   * - In case pdu is e2e protected:
   * - Build PDU header logical struct and serialize the PDU header into the allocated memory buffer.
   * - Instantiate an E2E header serializer to allocate the memory for the E2E header later filled with contents.
   * - Serialize the event sample payload into the memory buffer.
   * - Finalize E2E header serialization (header will be filled with data).
   * \endinternal
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const
  void SerializeEvent(BufferView& body_view, std::size_t const payload_size, SampleType const& data) noexcept final {
    // Fill Pdu header with data for the request
    ::amsr::someip_protocol::internal::PduMessageHeader const header{BuildPduHeader()};

    std::size_t const pdu_header_extn_size{GetHeaderExtensionSize()};

    // Size to be written in the length field (offset + Payload size (including E2E))
    std::size_t const pdu_payload_size{pdu_header_extn_size + payload_size};

    Writer writer{body_view};
    // writer: offset = 0
    // payload: [-, -, -, -]

    // Serialize Pdu header
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    ::amsr::someip_protocol::internal::serialization::SerializePduMessageHeader(writer, header, pdu_payload_size);

    // writer: offset = sizeof(header): 8
    // payload: [PDU Header, -, -, -]

    if (pdu_header_extn_.has_value()) {
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      writer.writeArray(pdu_header_extn_.value());
    }
    // writer: offset = 8 + pdu_header_extension (configurable)
    // payload: [PDU Header, -, -, -]

    if (is_e2e_protected_) {
      // For S2S the E2E header is part of the payload. I.e. the original writer should be passed to the Serialize()
      // function and the E2EHeaderSerializer will use a copy.
      Writer e2e_writer{writer};

      // writer: offset = 8 + payload_offset
      // e2e_writer: offset = 8 + payload_offset
      // payload: [PDU Header, -, -, -]

      // Serialize E2E header into the PDU payload
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_calculation_of_protected_offset
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_SOMEIPBINDING_AutosarC++17_10-A4.7.1_calculation_of_protected_offset
      ::amsr::someip_binding_xf::internal::e2e::E2EHeaderSerializer e2e_header_serializer{
          e2e_writer, e2e_transformer_.value(), body_view,
          static_cast<std::uint8_t>(::amsr::someip_protocol::internal::kPduHeaderSize + pdu_header_extn_size)};

      // writer: offset = 8 + payload_offset
      // e2e_writer: offset = 8 + payload_offset + E2E_Header_Size
      // payload: [PDU Header, -, -, -]

      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      serializer_.Serialize(writer, data);

      // writer: offset = 8 + payload_offset + PDU Size (including E2E_Header_Size)
      // e2e_writer: offset = 8 + payload_offset + E2E_Header_Size
      // payload: [PDU Header, -, -, PDU Payload]

      // Finally close the E2E header serializer which updates the attributes of the already allocated E2E header.
      e2e_header_serializer.Close();

      // writer: offset = 8 + payload_offset + PDU Size (including E2E_Header_Size)
      // e2e_writer: offset = 8 + payload_offset + E2E_Header_Size
      // payload: [PDU Header, -, E2E Header, PDU Payload]

    } else {
      // Serialize the event sample
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      serializer_.Serialize(writer, data);
    }
  }

  /*!
   * \brief Get the size of PDU header extension.
   *
   * \return Size of the pdu header extension in bytes.
   *
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetHeaderExtensionSize() const noexcept { return kPduHeaderExtendedSize; }

  /*!
   * \brief Calculate the size of the required packet header for PDU event.
   *
   * \return Size of the packet header in bytes.
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
    return ::amsr::someip_protocol::internal::kPduHeaderSize + GetHeaderExtensionSize();
  }

 private:
  /*!
   * \brief Build PDU header for the event sample to be transmitted.
   * \return The created PDU header struct.
   *
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Fill PDU header struct.
   * \endinternal
   */
  ::amsr::someip_protocol::internal::PduMessageHeader BuildPduHeader() const noexcept {
    // Fill Pdu header with data for the request
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    ::amsr::someip_protocol::internal::PduMessageHeader header;
    ::amsr::someip_protocol::internal::PduMessageHeaderUtility::SetServiceId(someip_event_identity_.GetServiceId(),
                                                                             header);
    ::amsr::someip_protocol::internal::PduMessageHeaderUtility::SetMethodId(someip_event_identity_.GetEventId(),
                                                                            header);
    header.length_ = 0U;  // To be calculated and passed as an argument when calling the SerializePduMessageHeader

    return header;
  }

  /*!
   * \brief SOME/IP ID of this event.
   */
  someip_binding_core::internal::SomeIpEventIdentity const someip_event_identity_;

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

  /*!
   * \brief The pdu header extension. In case of an SomeIp serialized event it is empty.
   */
  amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionTx> const
      pdu_header_extn_{amsr::core::nullopt};

  /*!
   * \brief Size by which the Pdu header is extended.
   */
  size_t const kPduHeaderExtendedSize;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_EVENT_SERIALIZER_H_
