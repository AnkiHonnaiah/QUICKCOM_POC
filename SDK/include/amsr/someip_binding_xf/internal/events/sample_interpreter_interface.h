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
/*!        \file  sample_interpreter_interface.h
 *        \brief  Interface for interpreting samples within proxy event xf
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_INTERPRETER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_INTERPRETER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/optional.h"
#include "amsr/core/span.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Interface for interpreting samples within proxy event xf
 */
class SampleInterpreterInterface {
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
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  SampleInterpreterInterface() = default;

  /*!
   * \brief Virtual destructor
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~SampleInterpreterInterface() noexcept = default;

  SampleInterpreterInterface(SampleInterpreterInterface const&) = delete;
  SampleInterpreterInterface(SampleInterpreterInterface&&) = delete;
  SampleInterpreterInterface& operator=(SampleInterpreterInterface const&) & = delete;
  SampleInterpreterInterface& operator=(SampleInterpreterInterface&&) & = delete;

  /*!
   * \brief Extracts the payload out of a message sample, if there is no valid payload, the optional return value stays
   * empty
   * \param[in] event_buffer message sample
   * \return An optional span for the payload
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Optional<Span> GetPayloadSpan(BufferView event_buffer) const noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_INTERPRETER_INTERFACE_H_
