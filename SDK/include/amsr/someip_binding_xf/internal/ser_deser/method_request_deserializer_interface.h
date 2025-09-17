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
/*!        \file  someip_binding_xf/internal/ser_deser/method_request_deserializer_interface.h
 *        \brief  Interface defining the deserializer API.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_DESERIALIZER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_DESERIALIZER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include "someip-protocol/internal/deserialization/reader.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace ser_deser {

/*!
 * \brief Interface defining deserializer API.
 *
 * \tparam InputArgs A variadic list of data types that correspond to the types of the elements of InputType.
 */
template <typename... InputArgs>
class MethodRequestDeserializerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  MethodRequestDeserializerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~MethodRequestDeserializerInterface() noexcept = default;

  MethodRequestDeserializerInterface(MethodRequestDeserializerInterface const&) = delete;
  MethodRequestDeserializerInterface(MethodRequestDeserializerInterface&&) = delete;
  MethodRequestDeserializerInterface& operator=(MethodRequestDeserializerInterface const&) & = delete;
  MethodRequestDeserializerInterface& operator=(MethodRequestDeserializerInterface&&) & = delete;

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
   */
  virtual bool Deserialize(::amsr::someip_protocol::internal::deserialization::Reader& reader,
                           std::tuple<InputArgs...>& data) const noexcept = 0;
};

}  // namespace ser_deser
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SER_DESER_METHOD_REQUEST_DESERIALIZER_INTERFACE_H_
