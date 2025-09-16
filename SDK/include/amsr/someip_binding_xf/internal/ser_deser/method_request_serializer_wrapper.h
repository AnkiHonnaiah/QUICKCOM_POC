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
/*!        \file  someip_binding_xf/internal/ser_deser/method_request_serializer_wrapper.h
 *        \brief  Definition of serializer wrapper.
 *
 *      \details  -
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::MethodRequestSerializerWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_SERIALIZER_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_SERIALIZER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/someip_binding_xf/internal/ser_deser/method_request_serializer_interface.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Serializer Wrapper.
 *
 * \tparam PayloadDeserializerType Payload serializer type.
 * \tparam ArgsT Variadic template parameter.
 */
template <typename PayloadSerializer, typename... ArgsT>
class MethodRequestSerializerWrapper final : public MethodRequestSerializerInterface<ArgsT...> {
 public:
  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  MethodRequestSerializerWrapper() = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~MethodRequestSerializerWrapper() noexcept final = default;

  MethodRequestSerializerWrapper(MethodRequestSerializerWrapper const&) = delete;
  MethodRequestSerializerWrapper(MethodRequestSerializerWrapper&&) = delete;
  MethodRequestSerializerWrapper& operator=(MethodRequestSerializerWrapper const&) & = delete;
  MethodRequestSerializerWrapper& operator=(MethodRequestSerializerWrapper&&) & = delete;

  /*!
   * \brief Returns the required buffer size to serialize the given datatype.
   *
   * \param[in]   args Value of method argument.
   *
   * \return      Calculated buffer size for serialization.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetRequiredBufferSize(ArgsT const&... args) const noexcept final {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return PayloadSerializer::GetRequiredBufferSize(args...);
  }

  /*!
   * \brief Serialize the given data.
   *
   * \param[in,out] writer Reference to the byte stream writer.
   * \param[in]     args   Value of method argument.
   *
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void Serialize(::amsr::someip_protocol::internal::serialization::Writer& writer,
                 ArgsT const&... args) const noexcept final {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    PayloadSerializer::Serialize(writer, args...);
  }
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_SERIALIZER_WRAPPER_H_
