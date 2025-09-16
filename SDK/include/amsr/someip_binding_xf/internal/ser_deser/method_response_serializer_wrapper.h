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
/*!        \file  method_response_serializer_wrapper.h
 *        \brief
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::MethodResponseSerializerWrapper
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_SERIALIZER_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_SERIALIZER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/error_domain.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_serializer_interface.h"
#include "someip-protocol/internal/serialization/infinity_size_t.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Serializer Wrapper which has a logger and calls serialize with this logger.
 *
 * \tparam DataType Data type to serialize.
 * \tparam PayloadDeserializerType Payload serializer type.
 */
template <typename DataType, typename PayloadSerializer>
class MethodResponseSerializerWrapper final : public MethodResponseSerializerInterface<DataType> {
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
  MethodResponseSerializerWrapper() = default;

  MethodResponseSerializerWrapper(MethodResponseSerializerWrapper const&) = delete;
  MethodResponseSerializerWrapper(MethodResponseSerializerWrapper&&) = delete;
  MethodResponseSerializerWrapper& operator=(MethodResponseSerializerWrapper const&) & = delete;
  MethodResponseSerializerWrapper& operator=(MethodResponseSerializerWrapper&&) & = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~MethodResponseSerializerWrapper() noexcept final = default;

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
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetRequiredBufferSize(DataType const& data) const noexcept final {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    return PayloadSerializer::GetRequiredBufferSize(data);
  }

  /*!
   * \brief Serialize the given data.
   *
   * \param[in,out] writer Reference to the byte stream writer.
   * \param[in]     data Reference to data object, whose value will be serialized into the writer.
   *
   * \pre           -
   * \context       App
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void Serialize(::amsr::someip_protocol::internal::serialization::Writer& writer,
                 DataType const& data) const noexcept final {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    PayloadSerializer::Serialize(writer, data);
  }
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_SERIALIZER_WRAPPER_H_
