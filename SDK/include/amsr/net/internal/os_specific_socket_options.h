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
 *        \brief
 *        \unit   osabstraction::net
 *
 *********************************************************************************************************************/
#ifndef LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_OS_SPECIFIC_SOCKET_OPTIONS_H_
#define LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_OS_SPECIFIC_SOCKET_OPTIONS_H_

#include <sys/socket.h>

#include <chrono>

#include "amsr/net/internal/types.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief VLAN priority option for sent packages.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 */
class SocketOptionVlanPriority final {
 public:
  /*!
   * \brief Constructs a priority option with the given priority value.
   *
   * \param[in] value Value to be set.
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
   * \vprivate        Vector product internal API
   */
  constexpr explicit SocketOptionVlanPriority(std::uint8_t value) noexcept : value_{value} {}

  /*!
   * \brief Constructs a priority option with initial value zero.
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
   * \vprivate        Vector product internal API
   */
  constexpr SocketOptionVlanPriority() noexcept = default;

  /*!
   * \brief Returns the socket option's level
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
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
  template <class Protocol>
  static constexpr auto GetLevel(Protocol const&) noexcept -> SockOptLevel {
    return SockOptLevel{SOL_SOCKET};
  }

  /*!
   * \brief Returns the socket option's name
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
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
  template <class Protocol>
  static constexpr auto GetName(Protocol const&) noexcept -> SockOptName {
    return SockOptName{kVLANPriority};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
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
  template <class Protocol>
  auto GetDataView(Protocol const&) const noexcept -> SockOptConstValueView {
    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return SockOptConstValueView{&value_, socklen_t{sizeof(int)}};
  }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details This function is intended to be used solely with getsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
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
  template <class Protocol>
  auto GetDataView(Protocol const&) noexcept -> SockOptMutableValueView {
    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return SockOptMutableValueView{&value_, socklen_t{sizeof(int)}};
  }

  /*!
   * \brief Returns the value of this option.
   *
   * \return Internal stored value.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   */
  constexpr std::uint8_t Value() const noexcept { return static_cast<std::uint8_t>(value_); }

  /*!
   * \brief Sets socket option value.
   *
   * \param[in] value Value to set.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   *
   */
  constexpr void SetValue(std::uint8_t value) noexcept {
    // VECTOR Next Line AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    value_ = static_cast<int>(value);
  }

 private:
  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  /*!
   * \brief VLAN priority.
   */
  int value_{0};
};

// forward declaration to avoid cycles in the include graph
template <std::int32_t Level, std::int32_t Name>
class SocketOptionTimeSignedInteger;

/*!
 * \brief Using an alias for TCP Keep Idle socket option.
 */
using SocketOptionTcpKeepIdle = SocketOptionTimeSignedInteger<IPPROTO_TCP, kTCPKeepIdle>;

/*!
 * \brief Using an alias for TCP Keep Interval socket option.
 */
using SocketOptionTcpKeepInterval = SocketOptionTimeSignedInteger<IPPROTO_TCP, kTCPKeepInterval>;

// forward declaration to avoid cycles in the include graph
template <std::int32_t Level, std::int32_t Name>
class SocketOptionSignedInteger;

/*!
 * \brief Using an alias for TCP Keep Count socket option.
 */
using SocketOptionTcpKeepCount = SocketOptionSignedInteger<IPPROTO_TCP, kTCPKeepCount>;

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_NET_INTERNAL_OS_SPECIFIC_SOCKET_OPTIONS_H_
