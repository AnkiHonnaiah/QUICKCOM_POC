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
/**        \file deser_lengthfield.h
 *        \brief Defines the deserialization function for length fields. Used by all types that may be
 *               preceded by a length field in the payload.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_LENGTHFIELD_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_LENGTHFIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

namespace detail {

/*!
 * \brief Deserializes a value with length field.
 * \tparam LengthConfig Length field config containing size of length field and endianness.
 */
template <typename LengthConfig>
class DeserLengthImpl {
 public:
  /*!
   * \brief Reads the length field according to the given configuration and forwards the reader to the provided
   *        deserialization function.
   * \tparam F Lambda function for specialized deserialization.
   * \param[in,out] r Reader holding the bufferView on the serialized data.
   * \param[in] f Function to be used to deserialize the data.
   * \param[in] static_length_field_increase Offset in bytes to be added to the static size. Required to handle the
   *                                         variant type selector field which is not considered in the dynamic
   *                                         length field value.
   * \return true if deserialization was successful, false otherwise.
   * \pre It has been verfied, at least the number of bytes are present
   *      the length field size is configured to.
   * \context Reactor|App
   * \reentrant TRUE for different reader objects.
   *
   * \internal
   * - Read length field from byte stream
   * - If length field is valid
   *   - Increase read length field in case a type selector field is present.
   *   - Call provided deserialization function with a sub reader holding a view on serialized byte
   *     stream with size of the length field.
   * \endinternal
   */
  template <typename F>
  static Result Deserialize(Reader& r, F&& f, std::uint8_t const static_length_field_increase) noexcept {
    typename UintRead<LengthConfig::value, typename LengthConfig::endian>::type length{};
    r.ReadUintOfSize<LengthConfig::value, typename LengthConfig::endian>(length);

    SizeType const increased_length{static_cast<SizeType>(length) + static_length_field_increase};
    Result result{r.VerifySize(increased_length)};
    if (result) {
      Reader sub_reader{r.ConsumeSubStream(increased_length)};
      result = std::forward<F>(f)(sub_reader);
    }
    return result;
  }
};

/*!
 * \brief Deserializes a value without length field.
 * \tparam Endian the endianness.
 */
template <typename Endian>
class DeserLengthImpl<LengthSize<0, Endian>> {
 public:
  /*!
   * \brief Forwards the reader to the provided deserialization function.
   * \tparam F Lambda function for specialized deserialization.
   * \param[in,out] r Reader holding the bufferView on the serialized data.
   * \param[in] f Function to be used to deserialize the data.
   * \return true if deserialization was successful, false otherwise.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE for different reader objects.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call provided deserialization function.
   * \endinternal
   */
  template <typename F>
  static Result Deserialize(Reader& r, F&& f, std::uint8_t) noexcept {
    return std::forward<F>(f)(r);
  }
};

}  // namespace detail

/*!
 * \brief Deserializes a value using LengthConfig to determine whether to use a length field.
 * \tparam LengthConfig Length field configuration for the data type.
 * \tparam StaticLengthFieldIncrease Offset in bytes to be added to the static size.Required to handle the
 *                                   variant type selector field which is not considered in the dynamic length
 *                                   field value.
 * \tparam F Lambda function for specialized deserialization.
   \param[in,out] r Reader holding the bufferView on the serialized data.
 * \param[in] f Function to be used to deserialize the data.
 * \return true if deserialization was successful, false otherwise.
 * \pre It has been verfied, at least the number of bytes are present the length field size is configured to.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 */
template <typename LengthConfig, std::uint8_t StaticLengthFieldIncrease = 0U, typename F>
Result DeserializeWithLengthField(Reader& r, F&& f) noexcept {
  return detail::DeserLengthImpl<LengthConfig>::Deserialize(r, std::forward<F>(f), StaticLengthFieldIncrease);
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_LENGTHFIELD_H_
