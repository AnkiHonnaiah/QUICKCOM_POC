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
/*!        \file  someip_binding_xf/internal/ser_deser/method_request_deserializer_wrapper.h
 *        \brief  Definition of deserializer wrapper.
 *
 *      \details  -
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::MethodRequestDeserializerWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_DESERIALIZER_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_DESERIALIZER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_deserializer_interface.h"
#include "someip-protocol/internal/deserialization/reader.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Deserializer Wrapper.
 *
 * \tparam PayloadDeserializerType Payload deserializer type.
 * \tparam InputArgs A variadic list of data types that correspond to the types of the elements of InputType.
 */
template <typename PayloadDeserializerType, typename... InputArgs>
class MethodRequestDeserializerWrapper final : public MethodRequestDeserializerInterface<InputArgs...> {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  MethodRequestDeserializerWrapper() = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~MethodRequestDeserializerWrapper() noexcept final = default;

  MethodRequestDeserializerWrapper(MethodRequestDeserializerWrapper const&) = delete;
  MethodRequestDeserializerWrapper(MethodRequestDeserializerWrapper&&) = delete;
  MethodRequestDeserializerWrapper& operator=(MethodRequestDeserializerWrapper const&) & = delete;
  MethodRequestDeserializerWrapper& operator=(MethodRequestDeserializerWrapper&&) & = delete;

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
  bool Deserialize(::amsr::someip_protocol::internal::deserialization::Reader& reader,
                   std::tuple<InputArgs...>& data) const noexcept final {
    return PayloadDeserializerType::Deserialize(reader, data);
  }
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_DESERIALIZER_WRAPPER_H_
