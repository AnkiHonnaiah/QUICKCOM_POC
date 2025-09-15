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
 *        \brief  Collection of type safe types that are used in in operating system interface.
 *
 *      \details  Most operating system arguments are not type safe. The provided types encapsulate these types.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_TYPES_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_TYPES_H_

#include "amsr/ptp/internal/os_specific_constants.h"

#include <chrono>
#include <cstdint>
#include "amsr/core/array.h"
#include "amsr/ptp/internal/specific_types.h"
#include "amsr/ptp/internal/system_header/ifaddrs.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief           Number of bytes an ethernet address consists of.
 */
constexpr std::uint8_t const kEthernetAddressLength{ETHER_ADDR_LEN};

/*!
 * \brief           Maximal length of a string that contains an Ethernet address.
 *
 * \details         The address is represented by hexadecimal characters (which means two characters per byte) and one
 *                  separation character is between each two bytes of the address (which means one character less than
 *                  there are bytes in the address), there is no terminating \0 character.
 */
constexpr std::uint8_t const kMaxEtherAddressString{(2 * kEthernetAddressLength) + (kEthernetAddressLength - 1)};

/*!
 * \brief           The length of an Ethernet header in bytes.
 */
constexpr std::size_t kEtherHeaderLength{ETHER_HDR_LEN};

/*!
 * \brief           Maximal length of the payload for one Ethernet frame in bytes (Maximum Transmission Unit).
 */
constexpr std::size_t kMaxEtherPayload{ETHERMTU};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           Offset (in bytes) the EtherType field has from the beginning of the Ethernet header.
 */
constexpr std::size_t kEthHeaderEtherTypeOffset{kEthernetAddressLength * 2ul};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_network_constants
/*!
 * \brief           TPID constant for IEEE 802.1q VLAN Ethernet frames (in host byte order).
 */
constexpr std::uint16_t k8021qVlanTpid{0x8100u};

/*!
 * \brief           The length of a VLAN tag in bytes.
 */
constexpr std::size_t kVlanTagLength{4};

/*!
 * \brief           The minimum length of a Precision Time Protocol (PTP) header in bytes.
 * \details         The minimum length of a PTP header is 34 bytes, which includes NO options.
 */
constexpr std::size_t kPtpHeaderLengthMin{34};

/*!
 * \brief           The maximum length of a Precision Time Protocol (PTP) header in bytes.
 * \details         The maximum length of a PTP header is 60 bytes, when all optional fields are present. The actual
 *                  length of a PTP header in a packet may be less than 60 bytes.
 */
constexpr std::size_t kPtpHeaderLengthMax{60};

/***********************************************************************************************************************
 * socket() arguments
 **********************************************************************************************************************/

/*!
 * \brief Maps to a socket family.
 *
 * \details
 * Check the POSIX socket() call for details.
 */
struct SocketFamily {
  /*! \brief Socket family */
  std::int32_t value;
};

/*!
 * \brief Maps to a socket type.
 *
 * \details
 * Check the POSIX socket() call for details.
 */
struct SocketType {
  /*! \brief Socket type  */
  std::int32_t value;
};

/*!
 * \brief Maps to a socket protocol.
 *
 * \details
 * Check the POSIX socket() call for details.
 */
struct SocketProtocol {
  /*! \brief Socket protocol */
  std::int32_t value;
};

/***********************************************************************************************************************
 * Ethernet address.
 **********************************************************************************************************************/

/*!
 * \brief Ethernet address
 */
struct EthNative {
  /*! \brief Bytes that make up an Ethernet address. */
  ::amsr::core::Array<std::uint8_t, internal::kEthernetAddressLength> data{};
};

/***********************************************************************************************************************
 * Network interface address abstraction.
 **********************************************************************************************************************/

/*!
 * \brief View to a list of network interface addresses.
 *
 * \details
 * Check the POSIX getifaddrs() call for details.
 */
struct InterfaceAddrView {
  /*!
   * \brief Pointer to the linked list of interface address structures.
   */
  struct ifaddrs* pointer;
};

/***********************************************************************************************************************
 * ioctl() arguments
 **********************************************************************************************************************/

/*!
 * \brief IOctl command name.
 *
 * \details
 * Check POSIX ioctl() call for details.
 */
struct IoctlCmdName {
  /*!
   * The ioctl command name.
   */
  IoctlRequestType value;
};

/*!
 * \brief Stores a mutable view to the parameter value.
 */
struct IoctlMutableValueView {
  /*!
   * \brief Base pointer to option value.
   */
  void* data;
};

/***********************************************************************************************************************
 * Blocking mode arguments
 **********************************************************************************************************************/
/*!
 * \brief Stores a socket's blocking mode.
 */
struct SocketBlockingMode {
  /*!
   * \brief True if blocking shall be enabled, otherwise false.
   */
  bool value;
};

/***********************************************************************************************************************
 * Types for timestamping
 **********************************************************************************************************************/
/*!
 * \brief Defines how to generate timestamps
 */
enum class TimestampMode : std::uint32_t {
  /*!
   * \brief Generate the time stamps by software.
   *
   * \details
   * Support by the kernel is needed, support by the device driver of the network adapter may be needed.
   */
  kSoftware = 1,

  /*!
   * \brief Use hardware support to generate time stamping.
   *
   * \details
   * The network adapter and its device driver must support this option.
   */
  kHardware = 2,
};

/*!
 * \brief Stores information about a received send timestamp.
 */
struct SendTimestamp {
  /*!
   * \brief Send time in nanoseconds.
   */
  std::chrono::nanoseconds timestamp_ns;
};

/*!
 * \brief Stores information about a received receive timestamp.
 */
struct ReceiveTimestamp {
  /*!
   * \brief Receive time in nanoseconds.
   */
  std::chrono::nanoseconds timestamp_ns;
};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_TYPES_H_
