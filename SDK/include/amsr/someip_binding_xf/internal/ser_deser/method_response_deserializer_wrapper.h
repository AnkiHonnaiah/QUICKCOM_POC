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
/*!        \file  someip_binding_xf/internal/ser_deser/method_response_deserializer_wrapper.h
 *        \brief  Definition of method response deserializer wrapper.
 *
 *      \details  -
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::MethodResponseDeserializerWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_DESERIALIZER_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_DESERIALIZER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_deserializer_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Method response deserializer Wrapper for SOME/IP protocol.
 *
 * \tparam DataType Data type to deserialize.
 * \tparam PayloadDeserializerType Payload deserializer type.
 */
template <typename DataType, typename PayloadDeserializerType>
class MethodResponseDeserializerWrapper final : public MethodResponseDeserializerInterface<DataType> {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  MethodResponseDeserializerWrapper() = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~MethodResponseDeserializerWrapper() noexcept final = default;

  MethodResponseDeserializerWrapper(MethodResponseDeserializerWrapper const&) = delete;
  MethodResponseDeserializerWrapper(MethodResponseDeserializerWrapper&&) = delete;
  MethodResponseDeserializerWrapper& operator=(MethodResponseDeserializerWrapper const&) & = delete;
  MethodResponseDeserializerWrapper& operator=(MethodResponseDeserializerWrapper&&) & = delete;

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
   * \spec
   *   requires true;
   * \endspec
   * \internal steady TRUE \endinternal
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

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_DESERIALIZER_WRAPPER_H_
