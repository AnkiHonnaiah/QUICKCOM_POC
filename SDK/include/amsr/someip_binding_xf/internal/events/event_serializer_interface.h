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
/**        \file  event_serializer_interface.h
 *        \brief  Event serializer interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_EVENT_SERIALIZER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_EVENT_SERIALIZER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief   Template for Event Serializer interface.
 *
 * \tparam  SampleType Type of the serialized event sample.
 */
template <typename SampleType>
class EventSerializerInterface {
 public:
  /*!
   * \brief Type-alias for the SOME/IP protocol BufferView.
   */
  using BufferView = ::amsr::someip_protocol::internal::serialization::BufferView;

  /*!
   * \brief Constructor.
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  EventSerializerInterface() noexcept = default;

  /*!
   * \brief Virtual destructor
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~EventSerializerInterface() noexcept = default;

  EventSerializerInterface(EventSerializerInterface const&) = delete;
  EventSerializerInterface(EventSerializerInterface&&) = delete;
  EventSerializerInterface& operator=(EventSerializerInterface const&) & = delete;
  EventSerializerInterface& operator=(EventSerializerInterface&&) & = delete;

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
   * \spec
   *   requires true;
   * \endspec
   */
  virtual std::size_t GetRequiredBufferSize(SampleType const& data) const noexcept = 0;

  /*!
   * \brief Returns the maximum buffer size to serialize the given datatype.
   *
   * \return      Maximum buffer size for serialization.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<std::size_t> GetMaximumBufferSize() const noexcept = 0;

  /*!
   * \brief Serializes event notification packet.
   *
   * \param[in,out] body_view Underlying buffer view of the writer.
   * \param[in] payload_size Size of the event sample payload.
   * \param[in] data Event sample value.
   *
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void SerializeEvent(BufferView& body_view, std::size_t const payload_size,
                              SampleType const& data) noexcept = 0;

  /*!
   * \brief Calculate the size of the required packet header for event.
   *
   * \return Size of the packet header in bytes.
   *
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual std::size_t CalculateHeaderSize() const noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr
#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_EVENT_SERIALIZER_INTERFACE_H_
