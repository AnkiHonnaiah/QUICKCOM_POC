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
/*!        \file  socket_option_ether.h
 *        \brief  Socket option for the ether package layer.
 *
 *      \details  Some of these option are for internal purpose only.
 *
 *         \unit  amsr::ptp::PtpSocket
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_SOCKET_OPTION_ETHER_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_SOCKET_OPTION_ETHER_H_

#include <linux/filter.h>
#include <linux/if_packet.h>
#include <linux/ioctl.h>
#include <linux/net_tstamp.h>
#include <sys/socket.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include "amsr/ptp/ethernet_address.h"
#include "amsr/ptp/internal/types.h"
#include "amsr/ptp/types.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief Socket option to receive physical layer multicast packages
 *
 * \details
 * The option is for internal purpose. It enables/disables the reception of multicast packages for the whole
 * interface.
 */
class SocketOptionJoinEtherMulticastGroup {
 public:
  /*!
   * \brief Constructs a socket option that can either be enabled or disabled.
   *
   * \param[in] address   Multicast address that has to be joined.
   * \param[in] interface Interface that hast to be used.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   *  - Stores the ethernet address and the network interface index.
   *  - Request to forward multicast packets.
   * \endinternal
   */
  SocketOptionJoinEtherMulticastGroup(EthernetAddress const& address, InterfaceIndex interface) noexcept {
    static_assert(sizeof(packet_mreq::mr_address) >= kEthernetAddressLength, "Cannot store link layer address");
    EthNative const eth_address{address.GetNative()};
    static_cast<void>(
        std::copy(std::cbegin(eth_address.data), std::cend(eth_address.data), std::begin(packet_request_.mr_address)));
    packet_request_.mr_alen = kEthernetAddressLength;
    packet_request_.mr_type = PACKET_MR_MULTICAST;
    // Expect a valid interface index.
    packet_request_.mr_ifindex = static_cast<decltype(packet_mreq::mr_ifindex)>(interface.value);
  }

  /*!
   * \brief Default constructor.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  constexpr SocketOptionJoinEtherMulticastGroup() noexcept = default;

  /*!
   * \brief Returns the socket option's level
   *
   * \return Socket option's level
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetLevel() noexcept -> SockOptLevel { return SockOptLevel{SOL_PACKET}; }

  /*!
   * \brief Returns the socket option's name
   *
   * \return Socket option's name
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetName() noexcept -> SockOptName { return SockOptName{PACKET_ADD_MEMBERSHIP}; }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \return          Immutable view to the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetConstDataView() const noexcept -> SockOptConstValueView {
    return SockOptConstValueView{&packet_request_, socklen_t{sizeof(packet_request_)}};
  }

 private:
  /*!
   * \brief Linux specific multicast request structure.
   */
  packet_mreq packet_request_{};
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief Configures various different time stamping modes.
 *
 * \details
 * This option can be used to enable hardware time stamping.
 */
class SocketOptionTimeStamping {
 public:
  /*!
   * \brief Enables software time stamping for incoming messages.
   *
   * \details
   * Sets the flags to enable the software time stamping for incoming messages.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void EnableRxSoftwareTimeStamping() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    value_ |=
        static_cast<std::int32_t>(SOF_TIMESTAMPING_SOFTWARE) | static_cast<std::int32_t>(SOF_TIMESTAMPING_RX_SOFTWARE);
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    // VECTOR Next Line AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    value_ &= static_cast<std::int32_t>(~SOF_TIMESTAMPING_RX_HARDWARE);
  }

  /*!
   * \brief Enables hardware time stamping for incoming messages.
   *
   * \details
   * Sets the flags to enable the hardware time stamping for incoming messages.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void EnableRxHardwareTimeStamping() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    value_ |= static_cast<std::int32_t>(SOF_TIMESTAMPING_RAW_HARDWARE) |
              static_cast<std::int32_t>(SOF_TIMESTAMPING_RX_HARDWARE);
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    // VECTOR Next Line AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    value_ &= static_cast<std::int32_t>(~SOF_TIMESTAMPING_RX_SOFTWARE);
  }

  /*!
   * \brief Enables software time stamping for outgoing messages.
   *
   * \details
   * Sets the flags to enable the software time stamping for outgoing messages.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void EnableTxSoftwareTimeStamping() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    value_ |=
        static_cast<std::int32_t>(SOF_TIMESTAMPING_SOFTWARE) | static_cast<std::int32_t>(SOF_TIMESTAMPING_TX_SOFTWARE) |
        static_cast<std::int32_t>(SOF_TIMESTAMPING_OPT_ID) | static_cast<std::int32_t>(SOF_TIMESTAMPING_OPT_TSONLY);
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    // VECTOR Next Line AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    value_ &= static_cast<std::int32_t>(~SOF_TIMESTAMPING_TX_HARDWARE);
  }

  /*!
   * \brief Enables hardware time stamping for outgoing messages.
   *
   * \details
   * Sets the flags to enable the hardware time stamping for outgoing messages.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void EnableTxHardwareTimeStamping() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    value_ |= static_cast<std::int32_t>(SOF_TIMESTAMPING_RAW_HARDWARE) |
              static_cast<std::int32_t>(SOF_TIMESTAMPING_TX_HARDWARE) |
              static_cast<std::int32_t>(SOF_TIMESTAMPING_OPT_ID) |
              static_cast<std::int32_t>(SOF_TIMESTAMPING_OPT_TSONLY);
    // VECTOR Next Construct AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_SysCallEnumAsFlags
    // VECTOR Next Line AutosarC++17_10-A4.5.1: MD_OSA_A4.5.1_SysCallEnumAsFlags
    value_ &= static_cast<std::int32_t>(~SOF_TIMESTAMPING_TX_SOFTWARE);
  }

  /*!
   * \brief Returns the socket option's level
   *
   * \return Socket option's level
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetLevel() noexcept -> SockOptLevel { return SockOptLevel{SOL_SOCKET}; }

  /*!
   * \brief Returns the socket option's name
   *
   * \return Socket option's name
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetName() noexcept -> SockOptName { return SockOptName{SO_TIMESTAMPING}; }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \return          Immutable view to the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetConstDataView() const noexcept -> SockOptConstValueView {
    return SockOptConstValueView{&value_, socklen_t{sizeof(std::int32_t)}};
  }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \return          Mutable view to the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetMutableDataView() noexcept -> SockOptMutableValueView {
    return SockOptMutableValueView{&value_, socklen_t{sizeof(std::int32_t)}};
  }

 private:
  /*!
   * \brief Socket option value.
   */
  std::int32_t value_{};
};

/*!
 * \brief Configures a Berkley packet filter to receive PTP frames.
 *
 */
class SocketOptionBpfEthFrame {
 private:
  /*! Number of BPF commands in the standard Ethernet frame filter. */
  static constexpr std::uint16_t kNumFilterCommandsStandardFilter{14u};
  /*! Number of BPF commands in the VLAN Ethernet frame filter. */
  static constexpr std::uint16_t kNumFilterCommandsVlanFilter{17u};

  /*! Type that defines the BPF program. */
  using BPFProgram = std::array<sock_filter, std::max(kNumFilterCommandsStandardFilter, kNumFilterCommandsVlanFilter)>;

 public:
  /*!
   * \brief Construct a BPF filter to receive standard ethernet frames.
   *
   * \param[in] address        The frames destination address.
   * \param[in] ether_type     The ether type in machine byte order.
   * \param[in] local_address  The interface's local address. All packets with this source address are dropped.
   * \context             ANY
   * \pre                 -
   * \reentrant           FALSE
   * \synchronous         TRUE
   * \threadsafe          FALSE
   */
  explicit SocketOptionBpfEthFrame(EthernetAddress const& address, std::uint16_t const ether_type,
                                   EthernetAddress const& local_address) noexcept {
    BpfAddress const dest_address{SplitAddress(address)};
    BpfAddress const src_address{SplitAddress(local_address)};

    bpf_filter_ = BPFProgram{
        {// Load a word starting from frame byte number 2 (Destination Address + 2)
         BPF_STMT(BPF_LD + BPF_W + BPF_ABS, 2),
         // Compare destination address [2:5] with low_32_bit_address.
         // If true continue to the next instruction otherwise jump to last filter instruction.
         BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, dest_address.lower_addr_bits, 0, 11),
         // Load a half word starting from frame byte number 0 (Destination Address)
         BPF_STMT(BPF_LD + BPF_H + BPF_ABS, 0),
         // Compare destination address [0:1] with high_16_bit_address.
         // If true continue to the next instruction otherwise jump to last filter instruction.
         BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, dest_address.higher_addr_bits, 0, 9),
         // Load a half word starting from frame byte number 12 (Ether Type)
         BPF_STMT(BPF_LD + BPF_H + BPF_ABS, 12),
         // Compare Ether Type with the given protocol.
         // If true continue to the next instruction otherwise jump to last filter instruction.
         BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, ether_type, 0, 7),
         // Load a byte at the special location which indicates if a VLAN tag is present.
         // VECTOR NL AutosarC++17_10-M5.19.1: MD_OSA_M5.19.1_OverflowDuringEvaluationOfConstantCastToUnsigned
         BPF_STMT(BPF_LD + BPF_B + BPF_ABS, static_cast<std::uint32_t>(SKF_AD_OFF + SKF_AD_VLAN_TAG_PRESENT)),
         // Compare VLAN tag is present indication with false constant (0).
         // If true continue to the next instruction otherwise jump to last filter instruction.
         BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, 0, 0, 5),
         // Load a word starting from frame byte number 8 (Source Address + 2)
         BPF_STMT(BPF_LD + BPF_W + BPF_ABS, 8),
         // Compare source address [2:5] with low_32_bit_address.
         // If false jump to success otherwise continue with the next instruction.
         BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, src_address.lower_addr_bits, 0, 2),
         // Load a half word starting from frame byte number 6 (Source Address)
         BPF_STMT(BPF_LD + BPF_H + BPF_ABS, 6),
         // Compare source address [0:1] with high_16_bit_address.
         // If false continue jump to success. Otherwise jump to failure.
         BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, src_address.higher_addr_bits, 1, 0),
         // Return success. Use 2^16 value which is the maximum length field for most network protocols.
         BPF_STMT(BPF_RET + BPF_K, 0x0000FFFF),
         // Return 0. Packet is discarded.
         BPF_STMT(BPF_RET + BPF_K, 0),
         // Default initialize rest of BPFProgram array to satisfy Bauhaus, this array element is unused.
         {0, 0, 0, 0},
         // Default initialize rest of BPFProgram array to satisfy Bauhaus, this array element is unused.
         {0, 0, 0, 0},
         // Default initialize rest of BPFProgram array to satisfy Bauhaus, this array element is unused.
         {0, 0, 0, 0}}};

    bpf_.len = kNumFilterCommandsStandardFilter;
  }

  /*!
   * \brief Construct a BPF filter to receive VLAN ethernet frames.
   *
   * \param[in] address        The frames destination address.
   * \param[in] ether_type     The ether type in machine byte order.
   * \param[in] local_address  The interface's local address. All packets with this source address are dropped.
   * \param[in] vlan_id        ID of the VLAN to receive from.
   * \context             ANY
   * \pre                 -
   * \reentrant           FALSE
   * \synchronous         TRUE
   * \threadsafe          FALSE
   */
  explicit SocketOptionBpfEthFrame(EthernetAddress const& address, std::uint16_t const ether_type,
                                   EthernetAddress const& local_address, VlanId vlan_id) noexcept {
    BpfAddress const dest_address{SplitAddress(address)};
    BpfAddress const src_address{SplitAddress(local_address)};

    bpf_filter_ = BPFProgram{{
        // Load a word starting from frame byte number 2 (Destination Address + 2)
        BPF_STMT(BPF_LD + BPF_W + BPF_ABS, 2),
        // Compare destination address [2:5] with low_32_bit_address.
        // If true continue to the next instruction otherwise jump to last filter instruction.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, dest_address.lower_addr_bits, 0, 14),
        // Load a half word starting from frame byte number 0 (Destination Address)
        BPF_STMT(BPF_LD + BPF_H + BPF_ABS, 0),
        // Compare destination address [0:1] with high_16_bit_address.
        // If true continue to the next instruction otherwise jump to last filter instruction.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, dest_address.higher_addr_bits, 0, 12),
        // Load a half word starting from frame byte number 12 (EtherType)
        BPF_STMT(BPF_LD + BPF_H + BPF_ABS, 12),
        // Compare EtherType with the given protocol.
        // If true continue to the next instruction otherwise jump to last filter instruction.
        // This will also reject the packet if it is a double tagged 802.1ad VLAN packet because a 802.1ad VLAN packet
        // will have the TPID of the inner VLAN tag (0x88A8) at offset 12 which is never equal to ether_type.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, ether_type, 0, 10),
        // Load a byte at the special location which indicates if a VLAN tag is present.
        // VECTOR NL AutosarC++17_10-M5.19.1: MD_OSA_M5.19.1_OverflowDuringEvaluationOfConstantCastToUnsigned
        BPF_STMT(BPF_LD + BPF_B + BPF_ABS, static_cast<std::uint32_t>(SKF_AD_OFF + SKF_AD_VLAN_TAG_PRESENT)),
        // Compare VLAN tag is present indication with true constant (1).
        // If true continue to the next instruction otherwise jump to last filter instruction.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, 1, 0, 8),
        // Load a half word at the special location which stores the VLAN tag.
        // VECTOR NL AutosarC++17_10-M5.19.1: MD_OSA_M5.19.1_OverflowDuringEvaluationOfConstantCastToUnsigned
        BPF_STMT(BPF_LD + BPF_H + BPF_ABS, static_cast<std::uint32_t>(SKF_AD_OFF + SKF_AD_VLAN_TAG)),
        // Mask the VLAN ID field of the VLAN tag.
        BPF_STMT(BPF_ALU + BPF_AND + BPF_K, 0xFFF),
        // Compare VLAN ID with given VLAN ID.
        // If true continue to the next instruction otherwise jump to last filter instruction.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, vlan_id.value, 0, 5),
        // Load a word starting from frame byte number 8 (Source Address + 2)
        BPF_STMT(BPF_LD + BPF_W + BPF_ABS, 8),
        // Compare source address [2:5] with low_32_bit_address.
        // If false jump to success otherwise continue with the next instruction.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, src_address.lower_addr_bits, 0, 2),
        // Load a half word starting from frame byte number 6 (Source Address)
        BPF_STMT(BPF_LD + BPF_H + BPF_ABS, 6),
        // Compare source address [0:1] with high_16_bit_address.
        // If false continue jump to success. Otherwise jump to failure.
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, src_address.higher_addr_bits, 1, 0),
        // Return success. Use 2^16 value which is the maximum length field for most network protocols.
        BPF_STMT(BPF_RET + BPF_K, 0x0000FFFF),
        // Return 0. Packet is discarded.
        BPF_STMT(BPF_RET + BPF_K, 0),
    }};

    bpf_.len = kNumFilterCommandsVlanFilter;
  }

  /*!
   * \brief Returns the socket option's level
   *
   * \return Socket option's level
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetLevel() noexcept -> SockOptLevel { return SockOptLevel{SOL_SOCKET}; }

  /*!
   * \brief Returns the socket option's name
   *
   * \return Socket option's name
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetName() noexcept -> SockOptName { return SockOptName{SO_ATTACH_FILTER}; }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \return          Immutable view to the internal data.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  auto GetConstDataView() const noexcept -> SockOptConstValueView {
    return SockOptConstValueView{&bpf_, socklen_t{sizeof(bpf_)}};
  }

 private:
  /*!
   * \brief An ethernet address split into the lower 32 bit and the higher 16 bit.
   */
  struct BpfAddress {
    /*! Lower 32 bit */
    std::uint32_t lower_addr_bits{};

    /*! Higher 16 bits. */
    std::uint32_t higher_addr_bits{};
  };

  /*!
   * \brief Splits an ethernet address into a lower 32 bit and higher 16 bit.
   *
   * \param[in] address Ethernet address.
   * \return Address split into lower and higher bytes.
   * \context             ANY
   * \pre                 -
   * \reentrant           FALSE
   * \synchronous         TRUE
   * \threadsafe          FALSE
   */
  static auto SplitAddress(EthernetAddress const& address) noexcept -> BpfAddress {
    BpfAddress split_address{};
    EthernetAddress::Native const native_address{address.GetNative()};
    split_address.higher_addr_bits = static_cast<uint32_t>((static_cast<uint32_t>(native_address.data[0]) << 8U) |
                                                           static_cast<uint32_t>(native_address.data[1]));
    split_address.lower_addr_bits = static_cast<uint32_t>((static_cast<uint32_t>(native_address.data[2]) << 24U) |
                                                          (static_cast<uint32_t>(native_address.data[3]) << 16U) |
                                                          (static_cast<uint32_t>(native_address.data[4]) << 8U) |
                                                          static_cast<uint32_t>(native_address.data[5]));
    return split_address;
  }

  /*!
   * \brief Socket option value.
   */
  BPFProgram bpf_filter_{};
  /*!
   *  \brief    Socket option data structure that defines the BPF program.
   */
  sock_fprog bpf_{0, bpf_filter_.data()};
};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_SOCKET_OPTION_ETHER_H_
