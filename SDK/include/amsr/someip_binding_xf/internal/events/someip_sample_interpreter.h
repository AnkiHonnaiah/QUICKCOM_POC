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
/**        \file  someip_sample_interpreter.h
 *        \brief  SOME/IP sample interpreter
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SomeIpSampleInterpreter
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_SAMPLE_INTERPRETER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_SAMPLE_INTERPRETER_H_

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
 * \brief   SOME/IP sample interpreter
 * \details A SampleInterpreter reads serialized event sample and provides the payload
 *
 *          For SOME/IP based events the packet layout has the following format:
            |   SOME/IP Header | Payload |
            SOME/IP header Size > [payload]
 */
class SomeIpSampleInterpreter final : public SampleInterpreterInterface {
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
   * \brief     Default constructor of SomeIpSampleInterpreter.
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  SomeIpSampleInterpreter() noexcept = default;

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
  ~SomeIpSampleInterpreter() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  SomeIpSampleInterpreter(SomeIpSampleInterpreter const&) = delete;
  SomeIpSampleInterpreter& operator=(SomeIpSampleInterpreter const&) = delete;
  SomeIpSampleInterpreter(SomeIpSampleInterpreter&&) = delete;
  SomeIpSampleInterpreter& operator=(SomeIpSampleInterpreter&&) = delete;

  /*!
   * \brief Extracts the payload out of a someIp sample, if there is no valid payload, the optional return value stays
   * empty
   * \param[in] event_buffer someIp sample \return An optional span for the payload
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
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_SAMPLE_INTERPRETER_H_
