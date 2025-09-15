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
/*!        \file
 *        \brief  Interface to operating system calls.
 *        \unit   osabstraction::endianness
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_ENDIANNESS_INTERNAL_OS_INTERFACE_ENDIANNESS_H_
#define LIB_UTILS_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_ENDIANNESS_INTERNAL_OS_INTERFACE_ENDIANNESS_H_

#include <endian.h>
#include <cstdint>

namespace amsr {
namespace endianness {
namespace internal {
namespace os_interface {

/*!
 * \brief Converts a 16 bit value from host to network byte order.
 *
 * \param[in]    host The value in host byte order.
 *
 * \return The value in network byte order.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate    Vector component internal API
 */
/*!
 * \internal
 * - Convert the 16 bit host ordered input value to network byte order and output the conversion result.
 * \endinternal
 */
inline std::uint16_t HostToNetworkUInt16(std::uint16_t host) noexcept {
  // VECTOR Disable AutosarC++17_10-A7.1.1: MD_OSA_A7.1.1_htobex_bextoh_family
  // VECTOR Disable AutosarC++17_10-A7.1.7: MD_OSA_A7.1.7_htobex_bextoh_family
  // VECTOR Disable AutosarC++17_10-M8.0.1: MD_OSA_M8.0.1_htobex_bextoh_family
  // VECTOR Disable AutosarC++17_10-M8.5.1: MD_OSA_M8.5.1_htobex_bextoh_family
  // VECTOR Disable VectorC++-V8.5.0: MD_OSA_V8.5.0_htobex_bextoh_family
  std::uint16_t const result{htobe16(host)};
  // VECTOR Enable VectorC++-V8.5.0: MD_OSA_V8.5.0_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-M8.5.1: MD_OSA_M8.5.1_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-M8.0.1: MD_OSA_M8.0.1_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-A7.1.7: MD_OSA_A7.1.7_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-A7.1.1: MD_OSA_A7.1.1_htobex_bextoh_family
  return result;
}

/*!
 * \brief Converts a 32 bit value from host to network byte order.
 *
 * \param[in]    host The value in host byte order.
 *
 * \return The value in network byte order.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate    Vector component internal API
 */
/*!
 * \internal
 * - Convert the 32 bit host ordered input value to network byte order and output the conversion result.
 * \endinternal
 */
inline std::uint32_t HostToNetworkUInt32(std::uint32_t host) noexcept { return htobe32(host); }

/*!
 * \brief Converts a 64 bit value from host to network byte order.
 *
 * \param[in]    host The value in host byte order.
 *
 * \return The value in network byte order.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate    Vector component internal API
 */
/*!
 * \internal
 * - Convert the 64 bit host ordered input value to network byte order and output the conversion result.
 * \endinternal
 */
inline std::uint64_t HostToNetworkUInt64(std::uint64_t host) noexcept { return htobe64(host); }

/*!
 * \brief Converts a 16 bit value from network to host byte order.
 *
 * \param[in]    network The value in network byte order.
 *
 * \return The value in host byte order.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate    Vector component internal API
 */
/*!
 * \internal
 * - Convert the 16 bit network ordered input value to host byte order and output the conversion result.
 * \endinternal
 */
inline std::uint16_t NetworkToHostUInt16(std::uint16_t network) noexcept {
  // VECTOR Disable AutosarC++17_10-A7.1.1: MD_OSA_A7.1.1_htobex_bextoh_family
  // VECTOR Disable AutosarC++17_10-A7.1.7: MD_OSA_A7.1.7_htobex_bextoh_family
  // VECTOR Disable AutosarC++17_10-M8.0.1: MD_OSA_M8.0.1_htobex_bextoh_family
  // VECTOR Disable AutosarC++17_10-M8.5.1: MD_OSA_M8.5.1_htobex_bextoh_family
  // VECTOR Disable VectorC++-V8.5.0: MD_OSA_V8.5.0_htobex_bextoh_family
  std::uint16_t const result{be16toh(network)};
  // VECTOR Enable VectorC++-V8.5.0: MD_OSA_V8.5.0_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-M8.5.1: MD_OSA_M8.5.1_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-M8.0.1: MD_OSA_M8.0.1_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-A7.1.7: MD_OSA_A7.1.7_htobex_bextoh_family
  // VECTOR Enable AutosarC++17_10-A7.1.1: MD_OSA_A7.1.1_htobex_bextoh_family
  return result;
}

/*!
 * \brief Converts a 32 bit value from network to host byte order.
 *
 * \param[in]    network The value in network byte order.
 *
 * \return The value in host byte order.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate    Vector component internal API
 */
/*!
 * \internal
 * - Convert the 32 bit network ordered input value to host byte order and output the conversion result.
 * \endinternal
 */
inline std::uint32_t NetworkToHostUInt32(std::uint32_t network) noexcept { return be32toh(network); }

/*!
 * \brief Converts a 64 bit value from network to host byte order.
 *
 * \param[in]    network The value in network byte order.
 *
 * \return The value in host byte order.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate    Vector component internal API
 */
/*!
 * \internal
 * - Convert the 64 bit network ordered input value to host byte order and output the conversion result.
 * \endinternal
 */
inline std::uint64_t NetworkToHostUInt64(std::uint64_t network) noexcept { return be64toh(network); }

}  // namespace os_interface
}  // namespace internal
}  // namespace endianness
}  // namespace amsr

#endif  // LIB_UTILS_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_ENDIANNESS_INTERNAL_OS_INTERFACE_ENDIANNESS_H_
