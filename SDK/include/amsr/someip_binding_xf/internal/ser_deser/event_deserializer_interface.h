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
/*!        \file  someip_binding_xf/internal/ser_deser/event_deserializer_interface.h
 *        \brief  Interface defining the deserializer API for events like someip or S2S events.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_EVENT_DESERIALIZER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_EVENT_DESERIALIZER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/deserialization/reader.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Interface defining deserializer API for events.
 *
 * \tparam DataType Data type to deserialize.
 */
template <typename DataType>
class EventDeserializerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  EventDeserializerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~EventDeserializerInterface() noexcept = default;

  EventDeserializerInterface(EventDeserializerInterface const&) = delete;
  EventDeserializerInterface(EventDeserializerInterface&&) = delete;
  EventDeserializerInterface& operator=(EventDeserializerInterface const&) & = delete;
  EventDeserializerInterface& operator=(EventDeserializerInterface&&) & = delete;

  /*!
   * \brief Deserialize the given data type.
   *
   * \param[in,out] reader Reference to the byte stream reader.
   * \param[out]    data Reference to data object in which the deserialized value will be written.
   * \return        True if the deserialization is successful, false otherwise.
   *
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   * \reentrant     TRUE for different reader objects.
   * \synchronous   TRUE
   * \steady        TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual bool Deserialize(::amsr::someip_protocol::internal::deserialization::Reader& reader,
                           DataType& data) const noexcept = 0;
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_EVENT_DESERIALIZER_INTERFACE_H_
