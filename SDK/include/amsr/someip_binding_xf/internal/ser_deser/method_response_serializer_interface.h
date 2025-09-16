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
/*!        \file  someip_binding_xf/internal/ser_deser/method_response_serializer_interface.h
 *        \brief  Interface defining the serializer API.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_SERIALIZER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_SERIALIZER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Interface defining serializer API.
 *
 * \tparam DataType Data type to serialize.
 */
template <typename DataType>
class MethodResponseSerializerInterface {
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
  MethodResponseSerializerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  virtual ~MethodResponseSerializerInterface() noexcept = default;

  MethodResponseSerializerInterface(MethodResponseSerializerInterface const&) = delete;
  MethodResponseSerializerInterface(MethodResponseSerializerInterface&&) = delete;
  MethodResponseSerializerInterface& operator=(MethodResponseSerializerInterface const&) & = delete;
  MethodResponseSerializerInterface& operator=(MethodResponseSerializerInterface&&) & = delete;

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
  virtual std::size_t GetRequiredBufferSize(DataType const& data) const noexcept = 0;

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
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPBINDING_AutosarC++17_10-M7.1.2_parameter_const_ref
  virtual void Serialize(::amsr::someip_protocol::internal::serialization::Writer& writer,
                         DataType const& data) const noexcept = 0;
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_RESPONSE_SERIALIZER_INTERFACE_H_
