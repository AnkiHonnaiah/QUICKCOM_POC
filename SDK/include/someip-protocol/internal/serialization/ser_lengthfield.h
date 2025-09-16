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
/*!       \file ser_lengthfield.h
 *        \brief Defines the serialization function for length fields. Used by all types that may be
 *               preceded by a length field in the payload.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_LENGTHFIELD_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_LENGTHFIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <type_traits>
#include <utility>
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

namespace detail {

/*!
 * \brief Base template for length field serialization. SOME/IP supports using length fields before all types
 *         except primitives.
 * \tparam LengthConfig Length field config containing size of length field and endianness.
 */
template <typename LengthConfig>
class SerLengthImpl {
 public:
  /*!
   * \brief Reserves space for length field in the buffer and invokes a lambda function which may write to the buffer.
   *        After the callable returns, the size of the writer is written to the reserved space.
   * \tparam F Lambda function for specialized serialization.
   * \param[in,out] w Writer holding the bufferView where the serialized data shall be written to.
   * \param[in] f Function to be used to serialize the data.
   * \param[in] StaticLengthReduction A static term removed from the length. Only used when serializing variant
   *                                  in non-TLV context.
   * \pre The length of serialized data should be no more than the upper limit of the LengthField value.
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Create a writer with reserved space for the length field.
   * - Execute lambda function for specialized serialization.
   * - Calculate the size used by the specialized serialization.
   * - If size fits in the length field
   *   - Write the size value into the reserved space with the earlier created writer.
   * - Otherwise log fatal message and abort.
   * \endinternal
   */
  template <typename F>
  static void serialize(Writer& w, F&& f, std::uint8_t StaticLengthReduction) noexcept {
    /*!
     * \brief The type of the length field.
     */
    using lengthfield_t = typename UintWrite<LengthConfig::value, typename LengthConfig::endian>::type;
    Writer lengthWriter{w.consumeSubStream(LengthConfig::value)};
    std::size_t const sizeBefore{w.Size()};
    std::forward<F>(f)();
    std::size_t const sizeUsed{sizeBefore - w.Size() - StaticLengthReduction};
    if (sizeUsed <= std::numeric_limits<lengthfield_t>::max()) {
      lengthfield_t const sizeUsed_lengthfield{static_cast<lengthfield_t>(sizeUsed)};
      lengthWriter.writeUintOfSize<LengthConfig::value, typename LengthConfig::endian>(sizeUsed_lengthfield);
    } else {
      // clang-format off
      logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Length field overflow during serialization."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                          ara::core::StringView{AMSR_FILE_LINE});
      // clang-format off
    }
  }
};

/*!
 * \brief Serializes a value without a length field, as indicated by size 0.
 * \tparam Endianness Underlying endianness
 */
template <typename Endianness>
class SerLengthImpl<LengthSize<0, Endianness>> {
 public:
  /*!
   * \brief Serializes a value without a length field.
   * \tparam F Lambda function for specialized serialization.
   * \param[in] f Function to be used to serialize the data.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Execute lambda function for specialized serialization.
   * \endinternal
   */
  template <typename F>
  static void serialize(Writer const&, F&& f, std::uint8_t) noexcept {
    std::forward<F>(f)();
  }
};

}  // namespace detail

/*!
 * \brief Serializes a value using LengthConfig to determine whether to add a length field.
 * \tparam LengthConfig An instantiation of LengthSize with size and endian of the length field.
 * \tparam StaticLengthReduction A static term removed from the length. Only used for Variant.
 * \tparam F Lambda function for specialized serialization.
 * \param[in,out] w Writer holding the bufferView to serialize to.
 * \param[in] f Function to call for writing a value after the length field.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \trace SPEC-4981319
 */
template <typename LengthConfig, std::uint8_t StaticLengthReduction = 0, typename F>
void serializeWithLengthField(Writer& w, F&& f) noexcept {
  detail::SerLengthImpl<LengthConfig>::serialize(w, std::forward<F>(f), StaticLengthReduction);
}
}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_LENGTHFIELD_H_
