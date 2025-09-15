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
/**        \file  signal_based_sample_interpreter.h
 *        \brief  signal-based sample interpreter
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SignalBasedSampleInterpreter
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_SAMPLE_INTERPRETER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_SAMPLE_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/span.h"
#include "amsr/someip_binding_xf/internal/events/sample_interpreter_interface.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Signal based sample interpreter
 * \details A SampleInterpreter reads serialized event sample and provides the payload
 *
            For signal based events there can be an additional pdu header extension
            in the sample, that needs to be considered for the extraction of the payload.
            The packet layout has the following format:
            |   PDU Header   |      ...       | Payload |
            PDU header Size + pdu header extension > [payload]
 */
class SignalBasedSampleInterpreter final : public SampleInterpreterInterface {
 public:
  /*!
   * \brief events::Span is ara::core::Span
   */
  using Span = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief Type-alias for the signal based protocol BufferView.
   */
  using BufferView = amsr::core::Span<std::uint8_t const>;

  /*!
   * \brief     constructor of SignalBasedSampleInterpreter.
   * \param[in] has_pdu_header_extension flag to enable or disable pdu header extension
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SignalBasedSampleInterpreter(bool has_pdu_header_extension) noexcept;

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
  ~SignalBasedSampleInterpreter() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  SignalBasedSampleInterpreter(SignalBasedSampleInterpreter const&) = delete;
  SignalBasedSampleInterpreter& operator=(SignalBasedSampleInterpreter const&) = delete;
  SignalBasedSampleInterpreter(SignalBasedSampleInterpreter&&) = delete;
  SignalBasedSampleInterpreter& operator=(SignalBasedSampleInterpreter&&) = delete;

  /*!
   * \brief Extracts the payload out of a signal based sample,
   * if there is no valid payload, the optional return value stays empty
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

 private:
  /*!
   * \brief Size by which the Pdu header is extended.
   */
  size_t const pdu_header_extended_size_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SIGNAL_BASED_SAMPLE_INTERPRETER_H_
