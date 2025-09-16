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
/*!        \file  someip_binding_xf/internal/ser_deser/s2s_event_deserializer_wrapper.h
 *        \brief  Definition of s2s event deserializer wrapper.
 *
 *      \details  -
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::S2sEventDeserializerWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_S2S_EVENT_DESERIALIZER_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_S2S_EVENT_DESERIALIZER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_xf/internal/ser_deser/event_deserializer_interface.h"
#include "someip-protocol/internal/deserialization/reader.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Deserializer Wrapper for S2S protocol.
 *
 * \tparam DataType Data type to deserialize.
 * \tparam PayloadDeserializerType Payload deserializer type.
 */
template <typename DataType, typename PayloadDeserializerType>
class S2sEventDeserializerWrapper final : public EventDeserializerInterface<DataType> {
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
  S2sEventDeserializerWrapper() = default;

  /*!
   * \brief Default destructor.
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~S2sEventDeserializerWrapper() noexcept final = default;

  S2sEventDeserializerWrapper(S2sEventDeserializerWrapper const&) = delete;
  S2sEventDeserializerWrapper(S2sEventDeserializerWrapper&&) = delete;
  S2sEventDeserializerWrapper& operator=(S2sEventDeserializerWrapper const&) & = delete;
  S2sEventDeserializerWrapper& operator=(S2sEventDeserializerWrapper&&) & = delete;

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
   *
   */
  bool Deserialize(::amsr::someip_protocol::internal::deserialization::Reader& reader,
                   DataType& data) const noexcept final {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return PayloadDeserializerType::Deserialize(reader, data);
  }
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_S2S_EVENT_DESERIALIZER_WRAPPER_H_
