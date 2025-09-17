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
/*!        \file  e2e_sample_interpreter_interface.h
 *        \brief  Interface for interpreting e2e samples within proxy event xf
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_E2E_SAMPLE_INTERPRETER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_E2E_SAMPLE_INTERPRETER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/span.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Interface for interpreting e2e samples within proxy event xf
 */
class E2eSampleInterpreterInterface {
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
   * \brief Default constructor
   * \spec
   *   requires true;
   * \endspec
   */
  E2eSampleInterpreterInterface() = default;

  /*!
   * \brief Virtual destructor
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~E2eSampleInterpreterInterface() noexcept = default;

  E2eSampleInterpreterInterface(E2eSampleInterpreterInterface const&) = delete;
  E2eSampleInterpreterInterface(E2eSampleInterpreterInterface&&) = delete;
  E2eSampleInterpreterInterface& operator=(E2eSampleInterpreterInterface const&) & = delete;
  E2eSampleInterpreterInterface& operator=(E2eSampleInterpreterInterface&&) & = delete;

  /*!
   * \brief Extracts the payload out of a message sample
   * \param[in] event_buffer message sample
   * \return An optional span for the payload
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Optional<Span> GetPayloadSpan(BufferView event_buffer) const noexcept = 0;

  /*!
   * \brief Extracts the e2e protected span out of a message sample
   * \param[in] event_buffer message sample
   * \return An optional span for the e2e protected area
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Optional<Span> GetE2eProtectedSpan(BufferView event_buffer) const noexcept = 0;

  /*!
   * \brief Checks if the e2e check is enabled
   * \param[in] event_buffer message sample
   * \return An optional boolean. True if E2E check is enabled, false otherwise.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Optional<bool> IsE2eCheckEnabled(BufferView event_buffer) const noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_E2E_SAMPLE_INTERPRETER_INTERFACE_H_
