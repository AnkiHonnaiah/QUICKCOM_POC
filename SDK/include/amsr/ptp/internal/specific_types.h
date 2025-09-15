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
 *        \brief  Collection of OS specify type safe types that are used in in operating system interface.
 *
 *      \details  Most operating system arguments are not type safe. The provided types encapsulate these types.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_SPECIFIC_TYPES_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_SPECIFIC_TYPES_H_

#include <ifaddrs.h>  // Required for ifaddrs (which is required in the file that includes this file).
#include <net/if.h>
#include <cstdint>

namespace amsr {
namespace ptp {
namespace internal {

/***********************************************************************************************************************
 * Socket address abstraction.
 **********************************************************************************************************************/
/*!
 * \brief View to a mutable socket address.
 */
struct SocketMutableAddrView {
  /*!
   * \brief Pointer to the socket address.
   */
  struct sockaddr* pointer;

  /*!
   * \brief Size of the socket address in bytes.
   */
  socklen_t size;
};

/*!
 * \brief View to a const socket address.
 */
struct SocketConstAddrView {
  /*!
   * \brief Pointer to the socket address.
   */
  struct sockaddr const* pointer;

  /*!
   * \brief Size of the socket address in bytes.
   */
  socklen_t const size;
};

/***********************************************************************************************************************
 * Network interface index abstraction.
 **********************************************************************************************************************/

/*!
 * \brief Maps to a network interface index.
 */
struct InterfaceIndex {
  /*! \brief Network interface index */
  std::uint32_t value;
};

/***********************************************************************************************************************
 * setsockopt()/getsockopt() arguments
 **********************************************************************************************************************/

/*!
 * \brief The socket option level.
 *
 * \details
 * Check POSIX getsockopt() call for details
 */
struct SockOptLevel {
  /*!
   * \brief Level
   */
  std::int32_t value;
};

/*!
 * \brief Socket option name
 *
 * \details
 * Check POSIX getsockopt() call for details
 */
struct SockOptName {
  /*!
   * The socket option name.
   */
  std::int32_t value;
};

/*!
 * \brief Stores a mutable view to the value
 *
 * \details
 * Combines the optval and the optlen parameter into one view.
 */
struct SockOptMutableValueView {
  /*!
   * \brief Base pointer to option value.
   */
  void* data;

  /*!
   * \brief Size of option value.
   */
  socklen_t size;
};

/*!
 * \brief Stores a const view to the value
 *
 * \details
 * Combines the optval and the optlen parameter into one view.
 */
struct SockOptConstValueView {
  /*!
   * \brief Base pointer to option value.
   */
  void const* data;

  /*!
   * \brief Size of option value.
   */
  socklen_t const size;
};

/***********************************************************************************************************************
 * Generic ancillary data packages.
 * Used together with sendmsg() / recvmsg()
 **********************************************************************************************************************/

/*!
 * \brief Stores the protocol specific type.
 */
struct ControlMessageType {
  /*!
   * \brief Protocol specific type.
   */
  std::int32_t value;
};

/*!
 * \brief Buffer view for ancillary data.
 *
 * \details
 * The data and size specify the buffer to store the requested data.
 */
struct AncillaryDataView {
  /*!
   * \brief Base pointer to buffer to store the ancillary data.
   */
  void* data;

  /*!
   * \brief Buffer size of ancillary data.
   */
  std::size_t size;
};

/*!
 * \brief Bundles an ancillary data request into one structure.
 *
 * \details
 * It is possible to pass multiple ancillary data request to the operating system layer. This structure bundles
 * a request's data into one object.
 */
struct AncillaryDataRequest {
  /*!
   * \brief protocol level.
   *
   * \details
   * POSIX defines that the cmsg_level in the request has to be a legal level for getsockopt() / setsockopt().
   */
  SockOptLevel level;

  /*!
   * \brief The protocol specific type.
   */
  ControlMessageType type;

  /*!
   * \brief The buffer to store the data.
   */
  AncillaryDataView data_view;

  /*!
   * \brief Stores the actually used buffer size.
   *
   * This variable stores the used buffer space. The operating system interface will update the value
   * to the actually used space. The request is only used if the level and the type matches the received ancillary
   * data request.
   */
  std::size_t* used_bytes;
};

/*!
 * \brief A set of lags that are passed to the recv() and recvfrom() call.
 *
 */
struct ReceiveFlags {
  /*!
   * \brief Receive flags.
   */
  std::int32_t flags{};
};

/*!
 * \brief The ioctl request type.
 *
 * Shall only be used together with the ioctl() call.
 */
using IoctlRequestType = unsigned long;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief Type of the ireq.ifru_flags.
 *
 */
using IfrIfFlags = decltype(ifreq::ifr_flags);

/*!
 * \brief Token to identify a send time stamp.
 */
struct SendTimestampToken {
  /*!
   * \brief The ID that identifies the send time stamp.
   */
  std::uint32_t id;
};

/*!
 * \brief Compares two SendTimestampToken for equality.
 *
 * \param[in] lhs  First SendTimestampToken
 * \param[in] rhs  Second SendTimestampToken
 *
 * \return          true if the tokens are equal, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline bool operator==(SendTimestampToken const& lhs, SendTimestampToken const& rhs) { return lhs.id == rhs.id; }

/*!
 * \brief Compares two SendTimestampToken for inequality.
 *
 * \param[in] lhs  First SendTimestampToken
 * \param[in] rhs  Second SendTimestampToken
 *
 * \return          true if the tokens are unequal, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \vprivate        Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline bool operator!=(SendTimestampToken const& lhs, SendTimestampToken const& rhs) { return !(lhs == rhs); }

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_SPECIFIC_TYPES_H_
