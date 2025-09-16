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
 *        \brief  Provides byte order conversion functions.
 *        \unit   osabstraction::endianness
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENDIANNESS_ENDIANNESS_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENDIANNESS_ENDIANNESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <functional>
#include "amsr/endianness/internal/os_interface_endianness.h"

namespace amsr {
namespace endianness {

/*!
 * \brief Converts a 16 bit value from host to network byte order.
 *
 * \param[in]    host The value in host byte order.
 *
 * \tparam       Out  Type of the return value in network byte order.
 * \tparam       In   Type of the input parameter in host byte order, restricted to unsigned 16 bit integer.
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
 * \trace           DSGN-Osab-Endiannes-ByteOrderConversion
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Out, typename In,
          std::enable_if_t<std::is_same<typename std::decay<In>::type, std::uint16_t>::value, std::int32_t> = 0>
inline auto HostToNetwork(In host) noexcept -> Out {
  static_assert(std::is_same<typename std::decay<In>::type, Out>::value,
                "HostToNetwork will not perform any type conversions");
  return amsr::endianness::internal::os_interface::HostToNetworkUInt16(host);
}

/*!
 * \brief Converts a 32 bit value from host to network byte order.
 *
 * \param[in]    host The value in host byte order.
 *
 * \tparam       Out  Type of the return value in network byte order.
 * \tparam       In   Type of the input parameter in host byte order, restricted to unsigned 32 bit integer.
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
 * \trace           DSGN-Osab-Endiannes-ByteOrderConversion
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Out, typename In,
          std::enable_if_t<std::is_same<typename std::decay<In>::type, std::uint32_t>::value, std::int32_t> = 0>
inline auto HostToNetwork(In host) noexcept -> Out {
  static_assert(std::is_same<typename std::decay<In>::type, Out>::value,
                "HostToNetwork will not perform any type conversions");
  return amsr::endianness::internal::os_interface::HostToNetworkUInt32(host);
}

/*!
 * \brief Converts a 64 bit value from host to network byte order.
 *
 * \param[in]    host The value in host byte order.
 *
 * \tparam       Out  Type of the return value in network byte order.
 * \tparam       In   Type of the input parameter in host byte order, restricted to unsigned 64 bit integer.
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
 *
 * \trace           DSGN-Osab-Endiannes-ByteOrderConversion
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Out, typename In,
          std::enable_if_t<std::is_same<typename std::decay<In>::type, std::uint64_t>::value, std::int32_t> = 0>
inline auto HostToNetwork(In host) noexcept -> Out {
  static_assert(std::is_same<typename std::decay<In>::type, Out>::value,
                "HostToNetwork will not perform any type conversions");
  return amsr::endianness::internal::os_interface::HostToNetworkUInt64(host);
}

/*!
 * \brief Converts a 16 bit value from network to host byte order.
 *
 * \param[in]    network The value in network byte order.
 *
 * \tparam       Out  Type of the return value in host byte order.
 * \tparam       In   Type of the input parameter in network byte order, restricted to unsigned 16 bit integer.
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
 * \trace           DSGN-Osab-Endiannes-ByteOrderConversion
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Out, typename In,
          std::enable_if_t<std::is_same<typename std::decay<In>::type, std::uint16_t>::value, std::int32_t> = 0>
inline auto NetworkToHost(In network) noexcept -> Out {
  static_assert(std::is_same<typename std::decay<In>::type, Out>::value,
                "NetworkToHost will not perform any type conversions");
  return amsr::endianness::internal::os_interface::NetworkToHostUInt16(network);
}

/*!
 * \brief Converts a 32 bit value from network to host byte order.
 *
 * \param[in]    network The value in network byte order.
 *
 * \tparam       Out  Type of the return value in host byte order.
 * \tparam       In   Type of the input parameter in network byte order, restricted to unsigned 32 bit integer.
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
 * \trace           DSGN-Osab-Endiannes-ByteOrderConversion
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Out, typename In,
          std::enable_if_t<std::is_same<typename std::decay<In>::type, std::uint32_t>::value, std::int32_t> = 0>
inline auto NetworkToHost(In network) noexcept -> Out {
  static_assert(std::is_same<typename std::decay<In>::type, Out>::value,
                "NetworkToHost will not perform any type conversions");
  return amsr::endianness::internal::os_interface::NetworkToHostUInt32(network);
}

/*!
 * \brief Converts a 64 bit value from network to host byte order.
 *
 * \param[in]    network The value in network byte order.
 *
 * \tparam       Out  Type of the return value in host byte order.
 * \tparam       In  Type of the input parameter in network byte order, restricted to unsigned 64 bit integer.
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
 * \trace           DSGN-Osab-Endiannes-ByteOrderConversion
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Out, typename In,
          std::enable_if_t<std::is_same<typename std::decay<In>::type, std::uint64_t>::value, std::int32_t> = 0>
inline auto NetworkToHost(In network) noexcept -> Out {
  static_assert(std::is_same<typename std::decay<In>::type, Out>::value,
                "NetworkToHost will not perform any type conversions");
  return amsr::endianness::internal::os_interface::NetworkToHostUInt64(network);
}

}  // namespace endianness
}  // namespace amsr

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_ENDIANNESS_ENDIANNESS_H_
