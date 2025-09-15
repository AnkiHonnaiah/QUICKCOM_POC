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
/**        \file  legacy_signal_based_e2e_sample_interpreter.h
 *        \brief  Legacy signal based e2e sample interpreter
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::LegacySignalBasedE2eSampleInterpreter
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_LEGACY_SIGNAL_BASED_E2E_SAMPLE_INTERPRETER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_LEGACY_SIGNAL_BASED_E2E_SAMPLE_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstdint>
#include "amsr/core/optional.h"
#include "amsr/core/span.h"
#include "amsr/someip_binding_xf/internal/events/e2e_sample_interpreter_interface.h"
namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Legacy signal based e2e sample interpreter
 * \details A SampleInterpreter reads serialized event sample and provides specific areas from
 *          the sample. The legacy interpreter is an interpreter which handles signal based e2e protected events
 *          similarly to e2e protected SOME/IP events. Usage of an update bit and definition of partially protected PDUs
 *          are not supported. This Interpreter is used in specific configurations where the e2e protection for a signal
 *          based event is configured in the same manner as for regular SOME/IP events.
 *
 *          For S2S events it is calculated using the Pdu header size constant
 *          and a configurable payload offset which is used by OEMs to extend
 *          the PDU header with proprietary information :
 *          |   PDU Header   |  kPduHeaderExtnLength  | Payload with E2e Header  |
 *          PDU header Size + kPduHeaderExtnLength      > [e2e header] ... [payload]
 *          For S2S events the protected range equals in this case the payload span.
 *          To get that range, the method GetE2eProtectedSpan is used.
 *          We pass a reader which starts at the payload to that method:
 *          |   PDU Header   |  kPduHeaderExtnLength  | Payload with E2e Header      |
 *          PDU header Size + kPduHeaderExtnLength      >
 */
class LegacySignalBasedE2eSampleInterpreter final : public E2eSampleInterpreterInterface {
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
   * \brief     Constructor of LegacySignalBasedE2eSampleInterpreter.
   * \param[in] has_pdu_header_extension Flag for the usage of the pdu header extension.
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit LegacySignalBasedE2eSampleInterpreter(bool has_pdu_header_extension) noexcept;

  /*!
   * \brief       Default destructor.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   */
  ~LegacySignalBasedE2eSampleInterpreter() noexcept final =
      default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  LegacySignalBasedE2eSampleInterpreter(LegacySignalBasedE2eSampleInterpreter const&) = delete;
  LegacySignalBasedE2eSampleInterpreter& operator=(LegacySignalBasedE2eSampleInterpreter const&) = delete;
  LegacySignalBasedE2eSampleInterpreter(LegacySignalBasedE2eSampleInterpreter&&) = delete;
  LegacySignalBasedE2eSampleInterpreter& operator=(LegacySignalBasedE2eSampleInterpreter&&) = delete;

  /*!
   * \brief Extracts the payload from a signal based e2e sample
   * \param[in] event_buffer signal based sample
   * \return An optional span for the payload
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
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
   */
  amsr::core::Optional<Span> GetE2eProtectedSpan(BufferView event_buffer) const noexcept final;

  /*!
   * \brief Checks if the e2e check is enabled
   * \param[in] event_buffer message sample
   * \return An optional boolean that indicates if the e2e check is enabled.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  amsr::core::Optional<bool> IsE2eCheckEnabled(BufferView event_buffer) const noexcept final;

 private:
  /*!
   * \brief Offset to get the start of the payload.
   */
  std::size_t const payload_offset_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_LEGACY_SIGNAL_BASED_E2E_SAMPLE_INTERPRETER_H_
