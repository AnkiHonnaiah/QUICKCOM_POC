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
/*!        \file  someip_binding_xf/internal/ser_deser/someip_event_serializer_wrapper.h
 *        \brief  Definition of serializer wrapper.
 *
 *      \details  -
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::SomeIpEventSerializerWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_SOMEIP_EVENT_SERIALIZER_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_SOMEIP_EVENT_SERIALIZER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/error_domain.h"
#include "amsr/someip_binding_xf/internal/ser_deser/serializer_interface.h"
#include "someip-protocol/internal/serialization/infinity_size_t.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Someip event Serializer Wrapper.
 *
 * \tparam DataType Data type to serialize.
 * \tparam PayloadDeserializerType Payload serializer type.
 */
template <typename DataType, typename PayloadSerializer>
class SomeIpEventSerializerWrapper final : public SerializerInterface<DataType> {
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
  SomeIpEventSerializerWrapper() = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SomeIpEventSerializerWrapper() noexcept final = default;

  SomeIpEventSerializerWrapper(SomeIpEventSerializerWrapper const&) = delete;
  SomeIpEventSerializerWrapper(SomeIpEventSerializerWrapper&&) = delete;
  SomeIpEventSerializerWrapper& operator=(SomeIpEventSerializerWrapper const&) & = delete;
  SomeIpEventSerializerWrapper& operator=(SomeIpEventSerializerWrapper&&) & = delete;

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
    return PayloadSerializer::GetRequiredBufferSize(data);
  }

  /*!
   * \brief Returns the maximum buffer size to serialize the given datatype.
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
  ::amsr::core::Result<std::size_t> GetMaximumBufferSize() const noexcept final {
    ::amsr::core::Result<std::size_t> max_buffer_size_result{
        ::amsr::someip_binding_core::internal::SomeIpBindingErrc::error_not_ok};

    ::amsr::someip_protocol::internal::serialization::InfSizeT const max_buffer_size{
        PayloadSerializer::GetMaximumBufferSize()};
    if (!max_buffer_size.IsInfinity()) {
      max_buffer_size_result.EmplaceValue(max_buffer_size.Value());
    }
    return max_buffer_size_result;
  }

  /*!
   * \brief Serialize the given data.
   *
   * \param[in,out] writer Reference to the byte stream writer.
   * \param[in]       data Reference to data object, whose value will be serialized into the writer.
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
    PayloadSerializer::Serialize(writer, data);
  }
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_SOMEIP_EVENT_SERIALIZER_WRAPPER_H_
