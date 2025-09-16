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
/**        \file  signal_based_e2e_sample_interpreter.h
 *        \brief  Signal based e2e sample interpreter
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SignalBasedE2eSampleInterpreter
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_E2E_SAMPLE_INTERPRETER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_E2E_SAMPLE_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstdint>
#include "amsr/core/optional.h"
#include "amsr/core/span.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/someip_binding_xf/internal/events/e2e_sample_interpreter_interface.h"
#include "someip-protocol/internal/serialization/writer.h"
namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Signal based e2e sample interpreter
 * \details A SampleInterpreter reads serialized event sample and provides specific areas in the sample.
 *
 *          For S2S events it is calculated using the Pdu header size constant
            and a configurable payload offset which is used by OEMs to extend
            the PDU header with proprietary information :
            |   PDU Header   |      ...       | Payload with E2e Header |
            PDU header Size + payload offset > [e2e header] ... [payload]
            For S2S events the protected range can be a sub range of the complete payload.
            To get that range, the method GetE2eProtectedSpan is used.
            We pass a reader which starts at the payload to that method:
            |   PDU Header   |      ...       | Payload with E2e Header      |
            PDU header Size + payload offset >
            We receive an e2e protected range from that method which is passed to the
            e2e transformer for the e2e check. That range can be within the payload and
            does not need to start at the beginning of the payload and does not need to
            end with the end of the payload.
            |   PDU Header   |      ...       | Payload with E2e Header      |
                                                < e2e range >
 *
 *
 */
class SignalBasedE2eSampleInterpreter final : public E2eSampleInterpreterInterface {
 public:
  /*!
   * \brief events::Span is amsr::core::Span
   */

  using Span = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type-alias for the SOME/IP protocol BufferView.
   */
  using BufferView = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type-alias for update bit position
   */
  using E2eUpdateBitPosition = std::size_t;

  /*!
   * \brief Type-alias for the E2E data length.
   */
  using E2eProtectedLength = std::size_t;

  /*!
   * \brief Type-alias for the E2E header offset.
   */
  using E2eProtectedOffset = std::size_t;

  /*!
   * \brief     Constructor of SignalBasedE2eSampleInterpreter.
   * \param[in] has_pdu_header_extension Flag for the usage of the pdu header extension.
   * \param[in] e2e_update_bit_pos Bit offset related to the start of the payload, for the e2e update bit position.
   * \param[in] e2e_protected_offset Byte offset related to the start of the payload, for the e2e protected area.
   * \param[in] e2e_protected_length Length of the e2e protected area in bits.
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  SignalBasedE2eSampleInterpreter(bool has_pdu_header_extension,
                                  amsr::core::Optional<E2eUpdateBitPosition> e2e_update_bit_pos,
                                  E2eProtectedOffset e2e_protected_offset,
                                  E2eProtectedLength e2e_protected_length) noexcept;

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
  ~SignalBasedE2eSampleInterpreter() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  SignalBasedE2eSampleInterpreter(SignalBasedE2eSampleInterpreter const&) = delete;
  SignalBasedE2eSampleInterpreter& operator=(SignalBasedE2eSampleInterpreter const&) = delete;
  SignalBasedE2eSampleInterpreter(SignalBasedE2eSampleInterpreter&&) = delete;
  SignalBasedE2eSampleInterpreter& operator=(SignalBasedE2eSampleInterpreter&&) = delete;

  /*!
   * \brief Extracts the payload from a signal based e2e sample
   * \param[in] event_buffer signal based sample
   * \return An optional span for the payload
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<Span> GetPayloadSpan(BufferView event_buffer) const noexcept final;

  /*!
   * \brief Extracts the e2e protected span from a signal based e2e sample
   * \param[in] event_buffer message sample
   * \return An optional span for the e2e protected area
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<Span> GetE2eProtectedSpan(BufferView event_buffer) const noexcept final;

  /*!
   * \brief Checks if the e2e check is enabled
   * \param[in] event_buffer message sample
   * \return A boolean that indicates if the e2e check is enabled.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<bool> IsE2eCheckEnabled(BufferView event_buffer) const noexcept final;

 private:
  /*!
   * \brief Determines if an E2E protected PDU has an updated E2E protected area.
   *
   * \param[in] pdu A read-only view for a PDU. The PDU shall not be empty.
   * \param[in] update_bit_offset An offset in bits for the UpdateBit. The offset shall be within the PDU.
   * \return The state of the UpdateBit.
   *
   * \threadsafe FALSE
   * \context APP
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Optional<bool> IsProtected(BufferView const& pdu, std::size_t const update_bit_offset) noexcept;

  /*!
   * \brief Complete header offset without e2e area.
   */
  size_t const header_offset_;

  /*!
   * \brief Update bit position
   */
  amsr::core::Optional<E2eUpdateBitPosition> const e2e_update_bit_position_;

  /*!
   * \brief E2E header offset
   */
  E2eProtectedOffset const e2e_protected_offset_;

  /*!
   * \brief E2E data length
   */
  E2eProtectedLength const e2e_protected_length_;

  /*!
   * \brief bits-bytes conversion factor.
   */
  static constexpr size_t kByteSizeFactor{8};
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_E2E_SAMPLE_INTERPRETER_H_
