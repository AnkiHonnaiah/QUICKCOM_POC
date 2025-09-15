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
/*!
 *      \file byteorder.h
 *      \brief Utilities to handle byte order (endianness).
 *      \details Support for
 *               - detection of platform endianness
 *               - byte swap operations
 *      \unit SomeIpProtocol::BasicSerdes::ByteOrder
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_BYTEORDER_H_
#define LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_BYTEORDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/span.h"

namespace amsr {
namespace someip_protocol {
namespace internal {

/**********************************************************************************************************************
 *  Byte order support
 *********************************************************************************************************************/

/*!
 * \brief Denotes the byte order to apply
 */
enum class ByteOrder : std::uint8_t {
  kMostSignificantByteFirst, /*! \brief < Big Endian */
  kMostSignificantByteLast,  /*! \brief < Little Endian */
  kMachineSpecific,          /*! \brief < Apply machine-endianness */
  kOpaque                    /*! \brief < Runtime dynamic encoding */
};

/*!
 * \brief Common base class for concrete endianness classes LittleEndian, BigEndian and MachineSpecificEndianness.
 */
class Endianness {
 public:
  Endianness& operator=(Endianness&) & = delete;
  Endianness& operator=(Endianness&&) & = delete;
  Endianness(Endianness const&) = delete;
  Endianness(Endianness&&) = delete;
  /*!
   * \brief   Set default destructor.
   * \pre     -
   * \context Any
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~Endianness() = default;
};

/*!
 * \brief Easy-to-read type for Little-Endian
 */
struct LittleEndian : public Endianness {
  /*!
   * \brief Define a constant expression for the conventional de/serializer.
   */
  static constexpr ByteOrder value{ByteOrder::kMostSignificantByteLast};
};

/*!
 * \brief Easy-to-read type for Big-Endian
 */
struct BigEndian : public Endianness {
  /*!
   * \brief Define a constant expression for the conventional de/serializer.
   */
  static constexpr ByteOrder value{ByteOrder::kMostSignificantByteFirst};
};

/*!
 * \brief Easy-to-read type for applying Machine-Endianness during serialization / deserialization.
 */
struct MachineSpecificEndianness : public Endianness {
  /*!
   * \brief Define a constant expression for the conventional de/serializer.
   */
  static constexpr ByteOrder value{ByteOrder::kMachineSpecific};
};

// If there is no availability of __BYTE_ORDER__ at compile time the serializer/deserializer compile time-optimizations
// based on endianess won't work. These are required on "TypeTraitIsOptimizableContainer"
// VECTOR NL AutosarC++17_10-A16.0.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A16.0.1_ConditionalCompilation
#ifndef __BYTE_ORDER__
static_assert(false, " Unsupported compiler. __BYTE_ORDER__ is required.");
#endif

/*!
 * \brief   Get the host platform byte order.
 * \return  Host platform byte order.
 *          Returns ByteOrder::kMostSignificantByteFirst in case the platform is using big-endian.
 *          Returns ByteOrder::kMostSignificantByteLast in case the platform is using little-endian.
 * \pre     -
 * \context Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR NL AutosarC++17_10-A0.1.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A0.1.3_function_not_called_false_positive
static inline constexpr ByteOrder GetHostByteOrder() noexcept {
  return (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__) ? ByteOrder::kMostSignificantByteFirst
                                                     : ByteOrder::kMostSignificantByteLast;
}

/**********************************************************************************************************************
 * byte-swap operations
 *********************************************************************************************************************/

// VECTOR Disable AutosarC++17_10-A4.7.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A4.7.1_CastMayTruncateValue

/*!
 * \brief   Swap uint16 value.
 * \param   res value to be swapped.
 * \return  Swapped value.
 * \pre     -
 * \context Any
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_CastMayTruncateValue_CheckPerformed
inline std::uint16_t ByteSwap16(std::uint16_t res) {
  return static_cast<std::uint16_t>((static_cast<std::uint16_t>(res >> 8) & 0x00FFU) | /*!< byte 1 to byte 0 */
                                    (static_cast<std::uint16_t>(res << 8) & 0xFF00U)); /*!< byte 0 to byte 1 */
}

/*!
 * \brief Swap uint32 value.
 * \param res value to be swapped.
 * \return Swapped value.
 * \pre     -
 * \context Any
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_CastMayTruncateValue_CheckPerformed
inline std::uint32_t ByteSwap32(std::uint32_t res) {
  return static_cast<std::uint32_t>((static_cast<std::uint32_t>(res >> 24) & 0x000000FFU) | /*!< byte 3 to byte 0 */
                                    (static_cast<std::uint32_t>(res >> 8) & 0x0000FF00U) |  /*!< byte 2 to byte 1 */
                                    (static_cast<std::uint32_t>(res << 8) & 0x00FF0000U) |  /*!< byte 1 to byte 2 */
                                    (static_cast<std::uint32_t>(res << 24) & 0xFF000000U)); /*!< byte 0 to byte 3 */
}

/*!
 * \brief Swap uint64 value.
 * \param res value to be swapped.
 * \return Swapped value.
 * \pre     -
 * \context Any
 * \spec
 *   requires true;
 * \endspec
 */
inline std::uint64_t ByteSwap64(std::uint64_t res) {
  return static_cast<std::uint64_t>(
      (static_cast<std::uint64_t>(res >> 56) & 0x00000000000000FFULL) | /*!< byte 7 to byte 0 */
      (static_cast<std::uint64_t>(res >> 40) & 0x000000000000FF00ULL) | /*!< byte 6 to byte 1 */
      (static_cast<std::uint64_t>(res >> 24) & 0x0000000000FF0000ULL) | /*!< byte 5 to byte 2 */
      (static_cast<std::uint64_t>(res >> 8) & 0x00000000FF000000ULL) |  /*!< byte 4 to byte 3 */
      (static_cast<std::uint64_t>(res << 8) & 0x000000FF00000000ULL) |  /*!< byte 3 to byte 4 */
      (static_cast<std::uint64_t>(res << 24) & 0x0000FF0000000000ULL) | /*!< byte 2 to byte 5 */
      (static_cast<std::uint64_t>(res << 40) & 0x00FF000000000000ULL) | /*!< byte 1 to byte 6 */
      (static_cast<std::uint64_t>(res << 56) & 0xFF00000000000000ULL)); /*!< byte 0 to byte 7 */
}

// VECTOR Enable AutosarC++17_10-A4.7.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A4.7.1_CastMayTruncateValue

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_BASIC_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_BYTEORDER_H_
