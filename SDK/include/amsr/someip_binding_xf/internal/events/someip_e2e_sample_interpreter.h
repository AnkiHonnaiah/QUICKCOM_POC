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
/**        \file  someip_e2e_sample_interpreter.h
 *        \brief  SOME/IP e2e sample interpreter
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SomeIpE2eSampleInterpreter
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_E2E_SAMPLE_INTERPRETER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_E2E_SAMPLE_INTERPRETER_H_

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
 * \brief   SOME/IP e2e sample interpreter
 * \details A SampleInterpreter reads serialized event sample and provides specific areas in the sample
 *
 *          For SOME/IP E2E based events the packet layout has the following format:
            ||       SOME/IP Header       |    E2E Header   | Payload ||
            16 byte SOME/IP Header size + e2e header size >

            Detailed layout for E2E protected area
            ||SOME/IP-Header non-protected parts | SOME/IP-Header protected parts | E2E Header | Payload |
            8 byte SOME/IP header length >
 */
class SomeIpE2eSampleInterpreter final : public E2eSampleInterpreterInterface {
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
   * \brief     Constructor of SomeIpE2eSampleInterpreter.
   * \param[in] e2e_profile e2e profile
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit SomeIpE2eSampleInterpreter(::amsr::e2e::profiles::Profile e2e_profile) noexcept;

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
  ~SomeIpE2eSampleInterpreter() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  SomeIpE2eSampleInterpreter(SomeIpE2eSampleInterpreter const&) = delete;
  SomeIpE2eSampleInterpreter& operator=(SomeIpE2eSampleInterpreter const&) = delete;
  SomeIpE2eSampleInterpreter(SomeIpE2eSampleInterpreter&&) = delete;
  SomeIpE2eSampleInterpreter& operator=(SomeIpE2eSampleInterpreter&&) = delete;

  /*!
   * \brief Extracts the payload out of a someIp e2e sample
   * \param[in] event_buffer someIp sample
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
  amsr::core::Optional<Span> GetPayloadSpan(BufferView const event_buffer) const noexcept final;

  /*!
   * \brief Extracts the e2e protected span out of a message sample
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
  amsr::core::Optional<Span> GetE2eProtectedSpan(BufferView const event_buffer) const noexcept final;

  /*!
   * \brief Checks if the e2e check is enabled
   * \param[in] event_buffer message sample
   * \return Always true.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<bool> IsE2eCheckEnabled(BufferView const event_buffer) const noexcept final;

 private:
  /*!
   * \brief The offset in bytes at which the payload starts.
   */
  std::size_t const payload_offset_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SOMEIP_E2E_SAMPLE_INTERPRETER_H_
