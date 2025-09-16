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
/*!        \file  socket_option.h
 *        \brief  Contains templates for common socket options
 *        \unit   osabstraction::net
 *
 *      \details  Most socket options are either boolean socket options or integer socket options. This file provides
 *                templates for the most common socket options.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_OPTION_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_OPTION_H_

#include <net/if.h>
#include <algorithm>  // Required for std::max().
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/net/interface.h"
#include "amsr/net/internal/os_specific_socket_options.h"
#include "amsr/net/internal/specific_types.h"
#include "amsr/net/internal/types.h"
#include "amsr/net/ip/address.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Supported address families.
 */
enum class SocketOptionAddressFamily : std::uint8_t {
  kUndefined, /*!< Allow undefined for usage in GetOption. */
  kIpv4,      /*!< Ipv4. */
  kIpv6       /*!< Ipv6. */
};

// VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_OSA_M0.1.8_VoidFunctionHasSideEffectAbort
/*!
 * \brief         Aborts if protocol family is not AF_INET or AF_INET6.
 *
 * \tparam        Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
 *                dependent option evaluation. Check IP related options for details.
 *
 * \param[in]     protocol  Protocol instance that indicates which IP family is used.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
template <class Protocol>
constexpr void CheckProtocol(Protocol const& protocol) noexcept {
  if (!((protocol.GetFamily().value == AF_INET) || (protocol.GetFamily().value == AF_INET6))) {
    ::amsr::core::Abort("Wrong protocol. Protocol must be either IPv4 or IPv6.");
  }
}

/*!
 * \brief Boolean socket option
 *
 * \tparam Level The level parameter in the getsockopt() call.
 * \tparam Name  The name parameter in the getsockopt() call.
 *
 * \details Boolean options enable or disable an option.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t Level, std::int32_t Name>
class SocketOptionBoolean final {
 public:
  /*!
   * \brief Constructs a socket option that can either be enabled or disabled.
   *
   * \param[in] enable True if option shall be enabled or False if option shall be disabled.
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
  constexpr explicit SocketOptionBoolean(bool enable) noexcept : value_{enable ? 1 : 0} {}

  /*!
   * \brief Constructs a boolean socket object with initial value false.
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
  constexpr SocketOptionBoolean() noexcept = default;

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
    return SockOptLevel{Level};
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
   * \vprivate        Vector component internal API
   */
  template <class Protocol>
  static constexpr auto GetName(Protocol const&) noexcept -> SockOptName {
    return SockOptName{Name};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \return A view to the internal data.
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
   *
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
   * \return A view to the internal data.
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
   *
   */
  template <class Protocol>
  auto GetDataView(Protocol const&) noexcept -> SockOptMutableValueView {
    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return SockOptMutableValueView{&value_, socklen_t{sizeof(int)}};
  }

  /*!
   * \brief Returns true if enabled, otherwise false.
   *
   * \details Outputs if the setting done by this socket option is enabled or disabled by this instance of the class.
   *
   * \return True if option is enabled, otherwise false.
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
  constexpr bool Value() const noexcept { return value_ != 0; }

  /*!
   * \brief Sets socket option value.
   *
   * \details Sets if the setting done by this socket option is enabled or disabled by this instance of the class.
   *
   * \param[in] enable True to enable option, otherwise false.
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
  constexpr void SetValue(bool enable) noexcept {
    if (enable) {
      value_ = 1;
    } else {
      value_ = 0;
    }
  }

 private:
  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  /*!
   * \brief
   * Stores the boolean value.
   *
   * \details
   * The setsockopt()/getsockopt() expect the datatype int. The constructor and all setter enforce that this value is
   * either 0 or 1.
   *
   */
  int value_{0};
};

/*!
 * \brief Boolean socket option that supports IPv4 and IPv6
 *
 * \tparam IPv4Level The level parameter in the getsockopt() call for IPv4.
 * \tparam IPv4Name  The name parameter in the getsockopt() call for IPv4.
 * \tparam IPv6Level The level parameter in the getsockopt() call for IPv6.
 * \tparam IPv6Name  The name parameter in the getsockopt() call for IPv6.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t IPv4Level, std::int32_t IPv4Name, std::int32_t IPv6Level, std::int32_t IPv6Name>
class SocketOptionIPBoolean final {
 public:
  /*!
   * \brief Constructs a socket option that can either be enabled or disabled.
   *
   * \param[in] enable True if option shall be enabled or False if option shall be disabled.
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
  constexpr explicit SocketOptionIPBoolean(bool enable) noexcept : value_{enable ? 1 : 0} {}

  /*!
   * \brief Constructs a boolean socket object with initial value false.
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
  constexpr SocketOptionIPBoolean() noexcept = default;

  /*!
   * \brief Returns the socket option's level
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's level
   *
   * \context         ANY
   *
   * \pre             Protocol must either be IPv4 or IPv6.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  template <class Protocol>
  static constexpr auto GetLevel(Protocol const& protocol) noexcept -> SockOptLevel {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptLevel{IPv4Level} : SockOptLevel{IPv6Level};
  }

  /*!
   * \brief Returns the socket option's name
   *
   * \tparam Protocol   The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                    dependent option evaluation. Check IP related options for details.
   *
   * \param[in]   protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's name.
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
  static constexpr auto GetName(Protocol const& protocol) noexcept -> SockOptName {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptName{IPv4Name} : SockOptName{IPv6Name};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return A view to the internal data.
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
   *
   */
  template <class Protocol>
  auto GetDataView(Protocol const& protocol) const noexcept -> SockOptConstValueView {
    CheckProtocol(protocol);
    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return SockOptConstValueView{&value_, socklen_t{sizeof(int)}};
  }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details This function is intended to be used solely with getsockopt() calls.
   *
   * \tparam Protocol   The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                    dependent option evaluation. Check IP related options for details.
   *
   * \param[in]   protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          A view to the internal data.
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
   *
   */
  template <class Protocol>
  auto GetDataView(Protocol const& protocol) noexcept -> SockOptMutableValueView {
    CheckProtocol(protocol);
    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return SockOptMutableValueView{&value_, socklen_t{sizeof(int)}};
  }

  /*!
   * \brief Returns true if enabled, otherwise false.
   *
   * \details Outputs if the setting done by this socket option is enabled or disabled by this instance of the class.
   *
   * \return True if option is enabled, otherwise false.
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
  constexpr bool Value() const noexcept { return value_ != 0; }

  /*!
   * \brief Sets socket option value.
   *
   *\details Sets if the setting done by this socket option is enabled or disabled by this instance of the class.
   *
   * \param[in] enable True to enable option, otherwise false.
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
  constexpr void SetValue(bool enable) noexcept {
    if (enable) {
      value_ = 1;
    } else {
      value_ = 0;
    }
  }

 private:
  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  /*!
   * \brief
   * Stores the boolean value.
   *
   * \details
   * The setsockopt()/getsockopt() expect the datatype int. The constructor and all setter enforce that this value is
   * either 0 or 1.
   *
   */
  int value_{0};
};

/*!
 * \brief Signed integer socket option
 *
 * \tparam Level The level parameter in the getsockopt() call.
 * \tparam Name  The name parameter in the getsockopt() call.
 *
 * \details
 *  Template for signed integer backed socket options. At this point we expect a integer to be a 32 bit type.
 *
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t Level, std::int32_t Name>
class SocketOptionSignedInteger final {
 public:
  /*!
   * \brief Constructs a signed integer socket option.
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
  constexpr explicit SocketOptionSignedInteger(std::int32_t value) noexcept : value_{value} {}

  /*!
   * \brief Constructs a signed integer socket option with initial value zero.
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
   */
  constexpr SocketOptionSignedInteger() noexcept = default;

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
    return SockOptLevel{Level};
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
    return SockOptName{Name};
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
   * \brief Returns the value of this socket option.
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
  constexpr std::int32_t Value() const noexcept { return value_; }

  /*!
   * \brief Sets socket option value.
   *
   * \param[in] value value to be set.
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
   */
  constexpr void SetValue(std::int32_t value) noexcept { value_ = value; }

 private:
  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  /*!
   * \brief
   * Stores the 32 integer value as native int type.
   *
   */
  int value_{0};
};

/*!
 * \brief Signed integer socket option with std::chrono::seconds as an external type.
 *
 * \tparam Level The level parameter in the getsockopt() call.
 * \tparam Name  The name parameter in the getsockopt() call.
 *
 * \details
 *  Template for signed integer socket options that handle time values. At this point we expect a integer to be a 32 bit
 *  type.
 *
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t Level, std::int32_t Name>
class SocketOptionTimeSignedInteger final {
 public:
  /*!
   * \brief Constructs a signed integer socket option.
   *
   * \details A value greater than the maximum number of std::int32_t will be set to the maximum number of std::int32_t.
   *
   * \param[in] value Value to be set.
   *
   * \context         ANY
   *
   * \pre             The provided time in seconds is zero or positive.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *
   * \vprivate        Vector product internal API
   */
  explicit SocketOptionTimeSignedInteger(std::chrono::seconds value) noexcept { SetValue(value); }

  /*!
   * \brief Constructs a signed integer socket option with initial value zero.
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
   */
  constexpr SocketOptionTimeSignedInteger() noexcept = default;

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
    return SockOptLevel{Level};
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
    return SockOptName{Name};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in] protocol the protocol as mentioned in the template parameter.
   *
   * \return          A view to the internal data.
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
  auto GetDataView(Protocol const& protocol) const noexcept -> SockOptConstValueView {
    return delegate_.GetDataView(protocol);
  }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details This function is intended to be used solely with getsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in] protocol The protocol as mentioned in the template parameter.
   *
   * \return A view to the internal data.
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
  auto GetDataView(Protocol const& protocol) noexcept -> SockOptMutableValueView {
    return delegate_.GetDataView(protocol);
  }

  /*!
   * \brief Returns the value of this socket option.
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
  constexpr std::chrono::seconds Value() const noexcept { return std::chrono::seconds{delegate_.Value()}; }

  /*!
   * \brief Sets socket option value.
   *
   * \details A value greater than the maximum number of std::int32_t will be set to the maximum number of std::int32_t.
   *
   * \param[in] value value to be set.
   *
   * \context         ANY
   *
   * \pre             The provided time in seconds is zero or positive.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  void SetValue(std::chrono::seconds value) noexcept {
    std::int32_t converted_value{std::numeric_limits<std::int32_t>::max()};
    if (value.count() < 0) {
      ::amsr::core::Abort("Cannot use a negative value for this socket option.");
    } else if (value.count() > std::numeric_limits<std::int32_t>::max()) {
      // keep maximum value
    } else {
      converted_value = static_cast<std::int32_t>(value.count());
    }

    delegate_.SetValue(converted_value);
  }

 private:
  /*!
   * \brief Delegate object.
   */
  SocketOptionSignedInteger<Name, Level> delegate_{0};
};

/*!
 * \brief Socket option to represent time durations with microsecond granularity.
 *
 * \tparam Level The level parameter in the getsockopt() call.
 * \tparam Name  The name parameter in the getsockopt() call.
 *
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 * \trace DSGN-Osab-BasicIP_BlockingModeTimeout
 */
template <std::int32_t Level, std::int32_t Name>
class SocketOptionTimeMicrosecond final {
 public:
  /*!
   * \brief Constructs a time duration socket option.
   *
   * \details A value greater than representable by the internal representation (struct timeval) is mapped to the
   *          largest value representable by the internal representation.
   *
   * \param[in] duration Duration that shall be represented.
   *
   * \context         ANY
   *
   * \pre             The provided time duration is zero or positive.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *
   * \vprivate        Vector product internal API
   */
  constexpr explicit SocketOptionTimeMicrosecond(std::chrono::microseconds duration) noexcept
      : duration_{Pack(duration)} {}

  /*!
   * \brief Constructs a time duration socket option which represents a duration of 0.
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
   */
  constexpr SocketOptionTimeMicrosecond() noexcept = default;

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
    return SockOptLevel{Level};
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
    return SockOptName{Name};
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
    return SockOptConstValueView{&duration_, socklen_t{sizeof(struct timeval)}};
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
    return SockOptMutableValueView{&duration_, socklen_t{sizeof(struct timeval)}};
  }

  /*!
   * \brief Returns the value of this socket option.
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
  constexpr std::chrono::microseconds Value() const noexcept { return Unpack(duration_); }

  /*!
   * \brief Sets socket option value.
   *
   * \details A value greater than representable by the internal representation (struct timeval) is mapped to the
   *          largest value representable by the internal representation.
   *
   * \param[in] value value to be set.
   *
   * \context         ANY
   *
   * \pre             The provided time duration is zero or positive.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector product internal API
   */
  constexpr void SetValue(std::chrono::microseconds value) noexcept { duration_ = Pack(value); }

 private:
  /*!
   * \brief Transforms the external representation of the time duration to the internal representation.
   *
   * \details A value greater than representable by the internal representation is mapped to the largest value
   *          representable by the internal representation.
   *
   * \param[in]       duration  Time duration in external representation.
   *
   * \return          Internal representation of the time duration.
   *
   * \context         ANY
   *
   * \pre             The provided time duration is zero or positive.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  static constexpr struct timeval Pack(std::chrono::microseconds duration) noexcept {
    constexpr decltype(timeval::tv_usec) kOneUsToS{999999};  // Number of microseconds in a second decremented by 1.
    static_assert(std::numeric_limits<decltype(timeval::tv_usec)>::max() >= kOneUsToS,
                  "Type of timeval::tv_usec can not handle up to 999999 microseconds");

    struct timeval internal_representation {
      std::numeric_limits<decltype(timeval::tv_sec)>::max(), kOneUsToS
    };

    std::chrono::seconds const whole_seconds{std::chrono::duration_cast<std::chrono::seconds>(duration)};
    std::chrono::microseconds const remaining_microseconds{
        duration - std::chrono::duration_cast<std::chrono::microseconds>(whole_seconds)};

    if (duration.count() < 0) {
      ::amsr::core::Abort("Cannot use a negative value for this socket option.");
    } else if (static_cast<std::size_t>(whole_seconds.count()) >
               std::size_t(std::numeric_limits<decltype(timeval::tv_sec)>::max())) {
      // keep maximum value
    } else {
      internal_representation.tv_sec = static_cast<decltype(timeval::tv_sec)>(whole_seconds.count());
      if (remaining_microseconds.count() < std::numeric_limits<decltype(timeval::tv_usec)>::max()) {
        internal_representation.tv_usec = static_cast<decltype(timeval::tv_usec)>(remaining_microseconds.count());
      } else {
        // This is only reached for following situations:
        //   1. remaining_microseconds.count() < 0
        //   2. remaining_microseconds.count() > 999999
        // Both conditions are mathematically impossible:
        //   1. From remaining_microseconds = duration - whole_seconds and whole_seconds <= duration follows that
        //      remaining_microseconds is never negative.
        //   2. Not possible because everything larger than 999999 would be one more complete second that should have
        //      resulted in whole_seconds being larger for remaining_microseconds = duration - whole_seconds.
        ::amsr::core::Abort("Mathematically impossible situation encountered, possibly a bug in std::chrono.");
      }
    }

    return internal_representation;
  }

  /*!
   * \brief Transforms the internal representation of the time duration to the external representation.
   *
   * \param[in]       duration  Time duration in internal representation.
   *
   * \return          External representation of the time duration.
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
  static constexpr std::chrono::microseconds Unpack(struct timeval duration) noexcept {
    std::chrono::seconds const seconds_part{duration.tv_sec};
    std::chrono::microseconds const microseconds_part{duration.tv_usec};

    return std::chrono::duration_cast<std::chrono::microseconds>(seconds_part) + microseconds_part;
  }

  /*!
   * \brief Representation of the time duration for the operating system.
   */
  struct timeval duration_ {
    0, 0
  };
};

/*!
 * \brief Signed integer socket option that supports IPv4 and IPv6
 *
 * \tparam IPv4Level The level parameter in the getsockopt() call for IPv4.
 * \tparam IPv4Name  The name parameter in the getsockopt() call for IPv4.
 * \tparam IPv6Level The level parameter in the getsockopt() call for IPv6.
 * \tparam IPv6Name  The name parameter in the getsockopt() call for IPv6.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t IPv4Level, std::int32_t IPv4Name, std::int32_t IPv6Level, std::int32_t IPv6Name>
class SocketOptionIPSignedInteger final {
 public:
  /*!
   * \brief Constructs a signed integer socket option.
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
  constexpr explicit SocketOptionIPSignedInteger(std::int32_t value) noexcept : value_{value} {}

  /*!
   * \brief Constructs a signed integer socket option with initial value zero.
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
   */
  constexpr SocketOptionIPSignedInteger() noexcept = default;

  /*!
   * \brief Returns the socket option's level
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's level
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
  static constexpr auto GetLevel(Protocol const& protocol) noexcept -> SockOptLevel {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptLevel{IPv4Level} : SockOptLevel{IPv6Level};
  }

  /*!
   * \brief Returns the socket option's name
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's name
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
  static constexpr auto GetName(Protocol const& protocol) noexcept -> SockOptName {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptName{IPv4Name} : SockOptName{IPv6Name};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          A view to the internal data.
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
  auto GetDataView(Protocol const& protocol) const noexcept -> SockOptConstValueView {
    CheckProtocol(protocol);
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
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          A view to the internal data.
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
  auto GetDataView(Protocol const& protocol) noexcept -> SockOptMutableValueView {
    CheckProtocol(protocol);
    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return SockOptMutableValueView{&value_, socklen_t{sizeof(int)}};
  }

  /*!
   * \brief Returns the value of this socket option.
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
  constexpr std::int32_t Value() const noexcept { return value_; }

  /*!
   * \brief Sets socket option value.
   *
   * \param[in] value value to be set.
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
   */
  constexpr void SetValue(std::int32_t value) noexcept { value_ = value; }

 private:
  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  /*!
   * \brief
   * Stores the 32 integer value as native int type.
   *
   */
  int value_{0};
};

/*!
 * \brief Multicast group option that supports IPv4 and IPv6
 *
 * \details This option cannot be used in GetOption().
 *
 * \tparam IPv4Level The level parameter in the getsockopt() call for IPv4.
 * \tparam IPv4Name  The name parameter in the getsockopt() call for IPv4.
 * \tparam IPv6Level The level parameter in the getsockopt() call for IPv6.
 * \tparam IPv6Name  The name parameter in the getsockopt() call for IPv6.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t IPv4Level, std::int32_t IPv4Name, std::int32_t IPv6Level, std::int32_t IPv6Name>
class SocketOptionIPMulticastGroup final {
 public:
  /*!
   * \brief         Creates a multicast group option.
   * \details       Optimized performance for address parameter of type IPv6.
   *
   * \param[in]     address Address of the multicast group.
   * \param[in]     interface Interface used to access the multicast group.
   *
   * \return        The multicast group option.
   *
   * \error         OsabErrc::kAddressError Address is not a multicast address or the network interface is not available
   *                                        or the interface cannot be used to access the multicast group.
   *
   * \error         OsabErrc::kUnexpected   Some unexpected internal state was detected.
   *
   * \error         OsabErrc::kResource     Address is an IPv4 address and there are not enough system resources to
   *                                        extract the IPv4 address of interface.
   *
   * \error         OsabErrc::kInsufficientPrivileges Address is an IPv4 address and the process has insufficient
   *                                                  privileges to extract the assigned IPv4 address of interface.
   *
   * \error         OsabErrc::kBusy         The operation should be retried later.
   *
   * \error         OsabErrc::kSystemEnvironmentError Network interface index could not be stored.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector product internal API
   */
  /*!
   * \internal
   * - If address is not a multicast address:
   *   - Return an address error.
   * - If address is an IPv4 address:
   *   - Extract all IP addresses from the network interface and return the first IPv4 address found.
   *   - If the extraction fails return the appropriate error.
   *   - Otherwise:
   *     - Create a multicast group option with the IPv4 address and the IPv4 interface address.
   *     - Return the multicast group option.
   * - Otherwise if address is an IPv6 address:
   *   - Create a multicast group option with the IPv6 address and the index of interface.
   * \endinternal
   */
  static auto MakeSocketOptionMulticastGroup(amsr::net::ip::Address const& address,
                                             amsr::net::Interface const& interface) noexcept
      -> ::amsr::core::Result<SocketOptionIPMulticastGroup> {
    ::amsr::core::Result<SocketOptionIPMulticastGroup> result{
        MakeErrorCode(osabstraction::OsabErrc::kAddressError, "Multicast group is not a multicast address.")};

    if (address.IsMulticast()) {
      if (address.IsV4()) {
        ::amsr::core::Result<amsr::net::ip::AddressV4> result_first_ipv4_address{
            ::amsr::core::Result<amsr::net::ip::AddressV4>::FromError(osabstraction::OsabErrc::kAddressError)};
        result = interface.GetAllAddresses(
            [&result_first_ipv4_address](amsr::net::InterfaceAddressEntry const& entry) -> bool {
              // VECTOR Next Line AutosarC++17_10-M0.1.2: MD_OSA_M0.1.2_dead_branch_false_positive
              if ((entry.HasIPAddressAndNetmask()) && (entry.GetIPAddress().IsV4())) {
                // The IP address is used to identify the network interface. Therefor any of
                // the extracted IPv4 can be used because they identify the same interface.
                result_first_ipv4_address.EmplaceValue(entry.GetIPAddress().GetV4Unsafe());
              }
              return !result_first_ipv4_address.HasValue();
            })
            .AndThen([&result_first_ipv4_address]() { return result_first_ipv4_address; })
            .Map([&address](amsr::net::ip::AddressV4 interface_v4_address) -> SocketOptionIPMulticastGroup {
                // Create socket option from the extracted IPv4 address.
                return SocketOptionIPMulticastGroup{address.GetV4Unsafe(), interface_v4_address};
            });
      } else if (std::numeric_limits<Ipv6MreqInterfaceIndexType>::max() < interface.GetIndex().value) {
        result = ::amsr::core::Result<SocketOptionIPMulticastGroup>{
            MakeErrorCode(osabstraction::OsabErrc::kSystemEnvironmentError,
                          "Interface index could not be stored for multicast group socket option.")};
      } else {
        result = ::amsr::core::Result<SocketOptionIPMulticastGroup>{
            SocketOptionIPMulticastGroup{address.GetV6Unsafe(), interface}};
      }
    }

    return result;
  }

  /*!
   * \brief         Creates a multicast group option.
   * \details       Optimized performance for both address parameters of type IPv4.
   *
   * \param[in]     address             Address of the multicast group.
   * \param[in]     interface_address   Address of local interface.
   * \param[in]     netstack_id         ID of the network stack that manages the local interface. Defaults to the
   *                                    default network stack.
   *
   * \return        The multicast group option.
   *
   * \error         OsabErrc::kAddressError Address is not a multicast address or the network interface is not available
   *                                        or the network address is assigned to multiple interfaces or the interface
   *                                        cannot be used to access the multicast group.
   *
   * \error         OsabErrc::kUnexpected   Some unexpected internal state was detected.
   *
   * \error         OsabErrc::kResource     Address is an IPv4 address and there are not enough system resources to
   *                                        extract the IPv4 address of interface or address is an IPv6 address and
   *                                        there are not enough system resources to extract the address of the network
   *                                        interface.
   *
   * \error         OsabErrc::kInsufficientPrivileges Address is an IPv4 address and the process has insufficient
   *                                                  privileges to extract the assigned IPv4 address of interface or
   *                                                  address is an IPv6 address and the process has insufficient
   *                                                  privileges to extract the address of the network interface.
   *
   * \error         OsabErrc::kBusy         The operation should be retried later.
   *
   * \error         OsabErrc::kSystemEnvironmentError No network interface with that address assigned found or network
   *                                                  interface index could not be stored.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector product internal API
   */
  /*!
   * \internal
   * - If address is not a multicast address:
   *   - Return an address error.
   * - If both addresses are IPv4 addresses:
   *     - Create a multicast group option with the IPv4 address and the IPv4 interface address.
   *     - Return the multicast group option.
   * - Else:
   *     - Create interface.
   *     - Call MakeSocketOptionMulticastGroup for IPv6.
   * \endinternal
   */
  static auto MakeSocketOptionMulticastGroup(
      amsr::net::ip::Address const& address, amsr::net::ip::Address const& interface_address,
      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<SocketOptionIPMulticastGroup> {
    ::amsr::core::Result<SocketOptionIPMulticastGroup> result{
        MakeErrorCode(osabstraction::OsabErrc::kAddressError, "Multicast group is not a multicast address.")};
    if (address.IsMulticast()) {
      if (address.IsV4() && interface_address.IsV4()) {
        result = ::amsr::core::Result<SocketOptionIPMulticastGroup>{
            SocketOptionIPMulticastGroup{address.GetV4Unsafe(), interface_address.GetV4Unsafe()}};
      } else {
        result =
            amsr::net::Interface::MakeInterface(interface_address, netstack_id)
                .AndThen([&address](amsr::net::Interface interface) {
                  return SocketOptionIPMulticastGroup<IPv4Level, IPv4Name, IPv6Level,
                                                      IPv6Name>::MakeSocketOptionMulticastGroup(address, interface);
                });
      }
    }
    return result;
  }

  /*!
   * \brief Returns the socket option's level
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's level
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
  static constexpr auto GetLevel(Protocol const& protocol) noexcept -> SockOptLevel {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptLevel{IPv4Level} : SockOptLevel{IPv6Level};
  }

  /*!
   * \brief Returns the socket option's name
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's name
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
  static constexpr auto GetName(Protocol const& protocol) noexcept -> SockOptName {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptName{IPv4Name} : SockOptName{IPv6Name};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return A view to the internal data.
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
  /*!
   * \internal
   * - If the specified socket protocol differs from the one used for constructing this option:
   *   - Abort program execution.
   * - Otherwise:
   *   - Return an immutable view to the internal data.
   * \endinternal
   */
  template <class Protocol>
  auto GetDataView(Protocol const& protocol) const noexcept -> SockOptConstValueView {
    CheckProtocol(protocol);

    bool const protocol_family_is_af_inet{protocol.GetFamily().value == AF_INET};

    if (protocol_family_is_af_inet) {
      if (!is_v4_) {
        ::amsr::core::Abort(
            "SocketOptionIPMulticastGroup::GetDataView(): Wrong protocol. This option was constructed for IPv6.");
      }
    } else if (is_v4_) {
      ::amsr::core::Abort(
          "SocketOptionIPMulticastGroup::GetDataView(): Wrong protocol. This option was constructed for IPv4.");
    } else {
      // correct protocol was chosen
    }

    return protocol_family_is_af_inet ? SockOptConstValueView{&v4_value_, socklen_t{sizeof(ip_mreq)}}
                                      : SockOptConstValueView{&v6_value_, socklen_t{sizeof(ipv6_mreq)}};
  }

 private:
  /*!
   * \brief Type of interface index in IPv6 socket address.
   */
  using Ipv6MreqInterfaceIndexType = decltype(ipv6_mreq::ipv6mr_interface);

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_SocketOptionIPDelegateConstructor
  /*!
   * \brief         Constructs a IPv4 multicast group option.
   *
   * \param[in]     address IPv4 address of the multicast group.
   * \param[in]     interface_address IPv4 address of the interface used to access the multicast group.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector component internal API
   */
  SocketOptionIPMulticastGroup(amsr::net::ip::AddressV4 const& address,
                               amsr::net::ip::AddressV4 const& interface_address) noexcept
      : is_v4_{true}, v4_value_{address.GetNative(), interface_address.GetNative()} {}

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_SocketOptionIPDelegateConstructor
  /*!
   * \brief         Constructs a IPv6 multicast group option.
   *
   * \param[in]     address IPv6 address of the multicast group.
   * \param[in]     interface Interface used to access the multicast group. Must fit into Ipv6MreqInterfaceIndexType.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector component internal API
   */
  SocketOptionIPMulticastGroup(amsr::net::ip::AddressV6 const& address, amsr::net::Interface const& interface) noexcept
      : is_v4_{false},
        v6_value_{address.GetNative(), static_cast<Ipv6MreqInterfaceIndexType>(interface.GetIndex().value)} {}

  /*!
   * \brief True if this option was constructed for IPv4. False if this option was constructed for IPv6.
   */
  bool is_v4_;

  /*!
   * \brief IPv4 multicast request.
   */
  ip_mreq v4_value_{};

  /*!
   * \brief IPv6 multicast request.
   */
  ipv6_mreq v6_value_{};
};

/*!
 * \brief Network interface option that supports IPv4 and IPv6
 *
 * \tparam Level The level parameter in the getsockopt() call.
 * \tparam Name  The name parameter in the getsockopt() call.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t Level, std::int32_t Name>
class SocketOptionNetworkInterface final {
 public:
  /*!
   * \brief         Constructs a network interface option with invalid data.
   *
   * \details       This function is intended to be used solely with getsockopt() calls.
   *
   * \param[in]     netstack_id   Network stack that manages the interface. Defaults to default network stack.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector component internal API
   */
  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_DelegateConstructor
  explicit SocketOptionNetworkInterface(net_utils::NetworkStackId netstack_id = net_utils::NetworkStackId{}) noexcept
      : network_stack_id_(std::move(netstack_id)) {
    interface_request_.ifr_name[0] = '\0';
  }
  /*!
   * \brief         Constructs a network interface option.
   *
   * \param[in]     interface Index of the interface.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector product internal API
   */
  explicit SocketOptionNetworkInterface(amsr::net::Interface const& interface) noexcept
      : network_stack_id_{interface.GetNetworkStackId()} {
    static_assert(sizeof(interface_request_.ifr_name) > amsr::net::Interface::kMaxInterfaceName,
                  "Class invariant violated, interface name string can not be stored in OS data structure.");
    static_cast<void>(interface.GetName(::amsr::core::Span<char>{interface_request_.ifr_name}));
  }

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
    return SockOptLevel{Level};
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
   * \vprivate        Vector component internal API
   */
  template <class Protocol>
  static constexpr auto GetName(Protocol const&) noexcept -> SockOptName {
    return SockOptName{Name};
  }

  /*!
   * \brief Returns the value of this socket option.
   *
   * \return Interface created from the internal stored data.
   *
   * \error           OsabErrc::kApiError  The option was default constructed and has an invalid value.
   *
   * \error           OsabErrc::kSystemEnvironmentError Interface creation failed, interface name not found.
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
  /*!
   * \internal
   * - If an interface name is contained
   *   - make an interface from it and return the result of that.
   * - Otherwise return an API error.
   * \endinternal
   */
  ::amsr::core::Result<amsr::net::Interface> Value() const noexcept {
    ::amsr::core::Result<amsr::net::Interface> result{
        ::amsr::core::Result<amsr::net::Interface>::FromError(osabstraction::OsabErrc::kApiError)};

    if (interface_request_.ifr_name[0] != '\0') {
      result = amsr::net::Interface::MakeInterface(::amsr::core::StringView{&(interface_request_.ifr_name[0])},
                                                   network_stack_id_);
    }

    return result;
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \return          A view to the internal data.
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
  /*!
   * \internal
   * - Return an immutable view to the internal data.
   * \endinternal
   */
  template <class Protocol>
  auto GetDataView(Protocol const&) const noexcept -> SockOptConstValueView {
    return SockOptConstValueView{&interface_request_, socklen_t{sizeof(struct ifreq)}};
  }

  /*!
   * \brief Returns an mutable view to the internal data.
   *
   * \details This function is intended to be used solely with getsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \return A view to the internal data.
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
  /*!
   * \internal
   * - Return a mutable view to the internal data.
   * \endinternal
   */
  template <class Protocol>
  auto GetDataView(Protocol const&) noexcept -> SockOptMutableValueView {
    return SockOptMutableValueView{&interface_request_, socklen_t{sizeof(struct ifreq)}};
  }

 private:
  /*!
   * \brief The interface request structure.
   *
   * \details
   * The interface request structure defines the interface that shall be configured.
   */
  ifreq interface_request_{};

  /*!
   * \brief Network stack that manages the interface.
   */
  net_utils::NetworkStackId network_stack_id_{};
};

/*!
 * \brief Network interface option that supports IPv4 and IPv6
 *
 * \tparam IPv4Level The level parameter in the getsockopt() call for IPv4.
 * \tparam IPv4Name  The name parameter in the getsockopt() call for IPv4.
 * \tparam IPv6Level The level parameter in the getsockopt() call for IPv6.
 * \tparam IPv6Name  The name parameter in the getsockopt() call for IPv6.
 *
 * \trace DSGN-Osab-BasicIP_SetOption
 *
 */
template <std::int32_t IPv4Level, std::int32_t IPv4Name, std::int32_t IPv6Level, std::int32_t IPv6Name>
class SocketOptionIPNetworkInterface final {
 public:
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_SocketOptionIPDelegateConstructor
  /*!
   * \brief         Constructs a network interface option with invalid data.
   *
   * \details       This function is intended to be used solely with getsockopt() calls.
   *
   * \param[in]     netstack_id   Network stack that manages the interface.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector component internal API
   */
  explicit SocketOptionIPNetworkInterface(net_utils::NetworkStackId netstack_id) noexcept
      : v4_value_{0x0}, v6_value_{0}, network_stack_id_{std::move(netstack_id)} {}

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_SocketOptionIPDelegateConstructor
  /*!
   * \brief         Constructs a IPv4 network interface option.
   *
   * \param[in]     interface_address IPv4 address of the interface.
   * \param[in]     netstack_id       ID of the network stack that manages the interface.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector product internal API
   */
  explicit SocketOptionIPNetworkInterface(amsr::net::ip::AddressV4 const& interface_address,
                                          net_utils::NetworkStackId netstack_id = net_utils::NetworkStackId{}) noexcept
      : address_family_{SocketOptionAddressFamily::kIpv4},
        v4_value_{interface_address.GetNative()},
        v6_value_{0},
        network_stack_id_{std::move(netstack_id)} {}

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_OSA_A12.1.5_SocketOptionIPDelegateConstructor
  /*!
   * \brief         Constructs a IPv6 network interface option.
   *
   * \param[in]     interface_index Index of the interface.
   * \param[in]     netstack_id     ID of the network stack that manages the interface.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector product internal API
   */
  explicit SocketOptionIPNetworkInterface(amsr::net::Interface::Index const& interface_index,
                                          net_utils::NetworkStackId netstack_id = net_utils::NetworkStackId{}) noexcept
      : address_family_{SocketOptionAddressFamily::kIpv6},
        v4_value_{0x0},
        v6_value_{interface_index.value},
        network_stack_id_{std::move(netstack_id)} {}

  /*!
   * \brief         Creates an network interface option.
   *
   * \tparam        Protocol The socket's protocol. It has to fulfill the protocol requirements.
   *                Required for protocol dependent option evaluation. Check IP related options for details.
   *
   * \param[in]     interface   The network interface.
   * \param[in]     protocol    Protocol instance that indicates which IP family is used.
   *
   * \return        The network interface option.
   *
   * \error         OsabErrc::kAddressError IPv4 is used and the interface does not have an IPv4 address assigned.
   *
   * \error         OsabErrc::kUnexpected   Some unexpected internal state was detected.
   *
   * \error         OsabErrc::kResource     IPv4 is used and there are not enough system resources to
   *                                        extract the IPv4 address of interface.
   *
   * \error         OsabErrc::kInsufficientPrivileges  IPv4 is used and the process has insufficient
   *                                                   privileges to extract the assigned IPv4 address of interface.
   *
   * \context       ANY
   *
   * \pre           -
   *
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \threadsafe    FALSE
   *
   * \vprivate      Vector product internal API
   */
  /*!
   * \internal
   * - If the protocol indicates that IPv4 should be used to create this option:
   *   - Extract all IP addresses from the network interface and return the first IPv4 address found.
   *   - If this fails return the appropiate error.
   *   - Otherwise create an IPv4 network interface option and return it.
   * - Otherwise create an IPv6 network interface option and return it.
   * \endinternal
   */
  template <class Protocol>
  static auto MakeSocketOptionIPNetworkInterface(amsr::net::Interface const& interface,
                                                 Protocol const& protocol) noexcept
      -> ::amsr::core::Result<SocketOptionIPNetworkInterface> {
    ::amsr::core::Result<SocketOptionIPNetworkInterface> result_sock_opt{
        ::amsr::core::Result<SocketOptionIPNetworkInterface>::FromError(osabstraction::OsabErrc::kAddressError)};

    bool const protocol_family_is_af_inet{protocol.GetFamily().value == AF_INET};

    if (protocol_family_is_af_inet) {
      ::amsr::core::Result<amsr::net::ip::AddressV4> result_first_ipv4_address{
          ::amsr::core::Result<amsr::net::ip::AddressV4>::FromError(osabstraction::OsabErrc::kAddressError)};
      result_sock_opt = interface.GetAllAddresses(
          [&result_first_ipv4_address](amsr::net::InterfaceAddressEntry const& entry) -> bool {
            // VECTOR Next Line AutosarC++17_10-M0.1.2: MD_OSA_M0.1.2_dead_branch_false_positive
            if ((entry.HasIPAddressAndNetmask()) && (entry.GetIPAddress().IsV4())) {
              // The IP address is used to identify the network interface. Therefor any of
              // the extracted IPv4 can be used because they identify the same interface.
              result_first_ipv4_address.EmplaceValue(entry.GetIPAddress().GetV4Unsafe());
            }
            return !result_first_ipv4_address.HasValue();
          })
          .AndThen([&result_first_ipv4_address](){ return result_first_ipv4_address; })
          .Map([&interface](amsr::net::ip::AddressV4 const& interface_v4_address) -> SocketOptionIPNetworkInterface {
              // Create socket option from the extracted IPv4 address.
              return SocketOptionIPNetworkInterface{interface_v4_address, interface.GetNetworkStackId()};
          });
    } else {
      result_sock_opt.EmplaceValue(SocketOptionIPNetworkInterface{interface.GetIndex(), interface.GetNetworkStackId()});
    }

    return result_sock_opt;
  }

  /*!
   * \brief Returns the socket option's level
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's level
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
  static constexpr auto GetLevel(Protocol const& protocol) noexcept -> SockOptLevel {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptLevel{IPv4Level} : SockOptLevel{IPv6Level};
  }

  /*!
   * \brief Returns the socket option's name
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          Socket option's name
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
  static constexpr auto GetName(Protocol const& protocol) noexcept -> SockOptName {
    CheckProtocol(protocol);
    return (protocol.GetFamily().value == AF_INET) ? SockOptName{IPv4Name} : SockOptName{IPv6Name};
  }

  /*!
   * \brief Returns the value of this socket option.
   *
   * \return Interface created from the internal stored data.
   *
   * \error           OsabErrc::kApiError  The option was default constructed and has an invalid value.
   *
   * \error           OsabErrc::kSystemEnvironmentError Interface creation failed.
   *
   * \error           OsabErrc::kUnexpected  Some unexpected internal state was detected.
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
  /*!
   * \internal
   * - If the address family used to create this option is IPv4 or IPv6:
   *   - Create an interface from the internal data.
   *   - If the interface creation fails return the appropiate error.
   *   - Otherwise return the interface.
   * - Otherwise return an API error.
   * \endinternal
   */
  ::amsr::core::Result<amsr::net::Interface> Value() const noexcept {
    ::amsr::core::Result<amsr::net::Interface> result{
        ::amsr::core::Result<amsr::net::Interface>::FromError(osabstraction::OsabErrc::kApiError)};

    if (address_family_ == SocketOptionAddressFamily::kIpv4) {
      result = amsr::net::Interface::MakeInterface(amsr::net::ip::Address{amsr::net::ip::AddressV4{v4_value_}},
                                                   network_stack_id_);
    } else if (address_family_ == SocketOptionAddressFamily::kIpv6) {
      result = amsr::net::Interface::MakeInterface(amsr::net::Interface::Index{v6_value_}, network_stack_id_);
    } else {
      // return an API error if the address family is undefined
    }

    return result;
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return          A view to the internal data.
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
  /*!
   * \internal
   * - If the address family used for constructing this option is undefined or differs from the one used for
   *   constructing this option:
   *   - Abort program execution.
   * - Return an immutable view to the internal data.
   * \endinternal
   */
  template <class Protocol>
  auto GetDataView(Protocol const& protocol) const noexcept -> SockOptConstValueView {
    CheckProtocol(protocol);

    if (address_family_ == SocketOptionAddressFamily::kUndefined) {
      ::amsr::core::Abort(
          "SocketOptionIPNetworkInterface::GetDataView(): This option was default constructed and does not contain "
          "valid data.");
    }

    bool const protocol_family_is_af_inet{protocol.GetFamily().value == AF_INET};
    if (protocol_family_is_af_inet) {
      if (address_family_ == SocketOptionAddressFamily::kIpv6) {
        ::amsr::core::Abort(
            "SocketOptionIPNetworkInterface::GetDataView(): Wrong protocol. This option was constructed for IPv4.");
      } else {
        // intentionally left empty
      }
    } else if (address_family_ == SocketOptionAddressFamily::kIpv4) {
      ::amsr::core::Abort(
          "SocketOptionIPNetworkInterface::GetDataView(): Wrong protocol. This option was constructed for IPv6.");
    } else {
      // intentionally left empty
    }

    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return protocol_family_is_af_inet ? SockOptConstValueView{&v4_value_, socklen_t{sizeof(in_addr)}}
                                      : SockOptConstValueView{&v6_value_, socklen_t{sizeof(unsigned int)}};
  }

  /*!
   * \brief Returns an mutable view to the internal data.
   *
   * \details This function is intended to be used solely with getsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \param[in]       protocol  Protocol instance that indicates which IP family is used.
   *
   * \return A view to the internal data.
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
  /*!
   * \internal
   * - If the address family used for constructing this option is undefined:
   *   - Set this options address family to the one indicated by protocol.
   * - Otherwise if the address family indicated by protocol differs from the one used for constructing this option:
   *   - Abort program execution.
   * - Return a mutable view to the internal data.
   * \endinternal
   */
  template <class Protocol>
  auto GetDataView(Protocol const& protocol) noexcept -> SockOptMutableValueView {
    CheckProtocol(protocol);

    bool const protocol_family_is_af_inet{protocol.GetFamily().value == AF_INET};

    if (protocol_family_is_af_inet) {
      if (address_family_ == SocketOptionAddressFamily::kUndefined) {
        address_family_ = SocketOptionAddressFamily::kIpv4;
      } else if (address_family_ == SocketOptionAddressFamily::kIpv6) {
        ::amsr::core::Abort(
            "SocketOptionIPNetworkInterface::GetDataView(): Wrong protocol. This option was constructed for IPv4.");
      } else {
        // intentionally left empty
      }
    } else if (address_family_ == SocketOptionAddressFamily::kUndefined) {
      address_family_ = SocketOptionAddressFamily::kIpv6;
    } else if (address_family_ == SocketOptionAddressFamily::kIpv4) {
      ::amsr::core::Abort(
          "SocketOptionIPNetworkInterface::GetDataView(): Wrong protocol. This option was constructed for IPv6.");
    } else {
      // intentionally left empty
    }

    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
    return protocol_family_is_af_inet ? SockOptMutableValueView{&v4_value_, socklen_t{sizeof(in_addr)}}
                                      : SockOptMutableValueView{&v6_value_, socklen_t{sizeof(unsigned int)}};
  }

 private:
  /*!
   * \brief Address family used to create this option.
   */
  SocketOptionAddressFamily address_family_{SocketOptionAddressFamily::kUndefined};

  /*!
   * \brief Interface address used in IPv4.
   */
  in_addr v4_value_;

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  /*!
   * \brief Interface index used in IPv6.
   */
  unsigned int v6_value_;

  /*!
   * \brief Network stack that manages the interface.
   */
  net_utils::NetworkStackId network_stack_id_;
};

/*!
 * \brief The socket option that sets lingering timeout to zero.
 *
 * \details
 * Enables socket lingering and sets it to zero or disables socket lingering. Note that the default is disabled. Disable
 * refers to no lingering configured. In that case close() will never block and the operating system will try to send
 * the data in the background.
 *
 * \trace DSGN-Osab-BasicIP_OptionLingerZero
 *
 */
class SocketOptionLingerZero final {
 public:
  /*!
   * \brief Constructs a linger socket option that can either disable lingering or enable it with timeout zero.
   *
   * \param[in] enable True if lingering is enabled with timeout zero. False if lingering shall be disabled.
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
  constexpr explicit SocketOptionLingerZero(bool enable) noexcept { this->SetValue(enable); }

  /*!
   * \brief Constructs a linger socket option with lingering disabled.
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
  constexpr SocketOptionLingerZero() noexcept = default;

  /*!
   * \brief Returns the socket option's level
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \return Always SOL_SOCKET.
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
   * \return Always SO_LINGER
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
  static constexpr auto GetName(Protocol const&) noexcept -> SockOptName {
    return SockOptName{SO_LINGER};
  }

  /*!
   * \brief Returns an immutable view to the internal data.
   *
   * \details This function is intended to be used solely with setsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \return An immutable view to the internal data.
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
   *
   */
  template <class Protocol>
  auto GetDataView(Protocol const&) const noexcept -> SockOptConstValueView {
    return SockOptConstValueView{&linger_, socklen_t{sizeof(struct linger)}};
  }

  /*!
   * \brief Returns a mutable view to the internal data.
   *
   * \details This function is intended to be used solely with getsockopt() calls.
   *
   * \tparam Protocol The socket's protocol. It has to fulfill the protocol requirements. Required for protocol
   *                  dependent option evaluation. Check IP related options for details.
   *
   * \return A view to the internal data.
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
   *
   */
  template <class Protocol>
  auto GetDataView(Protocol const&) noexcept -> SockOptMutableValueView {
    return SockOptMutableValueView{&linger_, socklen_t{sizeof(struct linger)}};
  }

  /*!
   * \brief Returns true if lingering is enabled. Otherwise false.
   *
   * \return True if lingering is enabled. Otherwise false.
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
   */
  constexpr bool Value() const noexcept { return linger_.l_onoff != 0; }

  /*!
   * \brief Sets socket option value.
   *
   * \param[in] enable True if lingering is enabled with timeout zero. False if lingering shall be disabled.
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
  constexpr void SetValue(bool enable) noexcept {
    if (enable) {
      linger_.l_onoff = 1;
    } else {
      linger_.l_onoff = 0;
    }
  }

 private:
  /*!
   * \brief
   * Stores linger value.
   *
   * \details
   * The initial state is lingering disabled and timeout 0 seconds.
   *
   */
  ::linger linger_{0, 0};
};

}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERNAL_SOCKET_OPTION_H_
