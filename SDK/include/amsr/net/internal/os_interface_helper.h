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
 *        \brief  Internal OS interface helper functions.
 *        \unit   osabstraction::net
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_HELPER_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/socket.h>

namespace amsr {
namespace net {
namespace internal {
namespace os_interface {
/*!
 * \brief           Gets the first header for ancillary data object
 *
 * \param[in]       message_header    Valid pointer message header for recvmsg()/sendmsg() calls.
 *
 * \return          Valid pointer to header for ancillary data object or nullptr.
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
inline struct cmsghdr* GetFirstCMSGHeader(msghdr const* const message_header) noexcept {
  // VECTOR Disable AutosarC++17_10-A4.10.1: MD_OSA_A4.10.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M4.10.2: MD_OSA_M4.10.2_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.2: MD_OSA_M5.0.2_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_os_structure_msghdr_cmsghdr
  struct cmsghdr* const cmsg_header{CMSG_FIRSTHDR(message_header)};
  // VECTOR Enable AutosarC++17_10-A4.10.1: MD_OSA_A4.10.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M4.10.2: MD_OSA_M4.10.2_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.2: MD_OSA_M5.0.2_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_os_structure_msghdr_cmsghdr
  return cmsg_header;
}

// VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_os_structure_msghdr_cmsghdr
/*!
 * \brief           Gets the next header for ancillary data objects
 *
 * \param[in]       message_header    Valid pointer to message header for recvmsg()/sendmsg() calls.
 * \param[in]       cur_cmsg_header   Valid pointer to header for ancillary data object.
 *
 * \return          Valid pointer to header for ancillary data object or nullptr.
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
inline cmsghdr* GetNextCMSGHeader(msghdr* message_header, cmsghdr* const cur_cmsg_header) noexcept {
  // Not all operating systems need the message header for the CMSG_NXTHDR call.
  static_cast<void>(message_header);
  // VECTOR Disable AutosarC++17_10-A4.10.1: MD_OSA_A4.10.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M4.10.2: MD_OSA_M4.10.2_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-A5.0.1: MD_OSA_A5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable VectorC++-V5.0.1: MD_OSA_V5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.2: MD_OSA_M5.0.2_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.3: MD_OSA_M5.0.3_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.4: MD_OSA_M5.0.4_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.15: MD_OSA_M5.0.15_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.20: MD_OSA_M5.0.20_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-A7.1.1: MD_OSA_A7.1.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_os_structure_msghdr_cmsghdr
  struct cmsghdr* const cmsg_header{
      CMSG_NXTHDR(message_header, cur_cmsg_header)};  // NOLINT(readability/nolint), NOLINT(vector-object-not-changed)
  // VECTOR Enable AutosarC++17_10-A4.10.1: MD_OSA_A4.10.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M4.10.2: MD_OSA_M4.10.2_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-A5.0.1: MD_OSA_A5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable VectorC++-V5.0.1: MD_OSA_V5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.2: MD_OSA_M5.0.2_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.3: MD_OSA_M5.0.3_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.4: MD_OSA_M5.0.4_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.15: MD_OSA_M5.0.15_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.20: MD_OSA_M5.0.20_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-A7.1.1: MD_OSA_A7.1.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M7.1.2: MD_OSA_M7.1.2_os_structure_msghdr_cmsghdr
  return cmsg_header;
}

/*!
 * \brief           Gets the ancillary data for a passed header.
 *
 * \param[in]       cmsg_header       Valid pointer to header for ancillary data object.
 *
 * \return          Pointer to an ancillary data object.
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
//  NOLINT(readability/nolint),  NOLINTNEXTLINE(vector-param-can-be-ptr-or-ref-to-const)
inline void* GetCMSGData(cmsghdr* const cmsg_header) noexcept {
  // VECTOR Disable AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-A5.0.1: MD_OSA_A5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable VectorC++-V5.0.1: MD_OSA_V5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.3: MD_OSA_M5.0.2_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.4: MD_OSA_M5.0.4_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.15: MD_OSA_M5.0.15_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.20: MD_OSA_M5.0.20_os_structure_msghdr_cmsghdr
  // VECTOR Disable AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_os_structure_msghdr_cmsghdr
  void* const cmsg_data{CMSG_DATA(cmsg_header)};
  // VECTOR Enable AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-A5.0.1: MD_OSA_A5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable VectorC++-V5.0.1: MD_OSA_V5.0.1_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.3: MD_OSA_M5.0.2_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.4: MD_OSA_M5.0.4_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.15: MD_OSA_M5.0.15_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.20: MD_OSA_M5.0.20_os_structure_msghdr_cmsghdr
  // VECTOR Enable AutosarC++17_10-M5.0.21: MD_OSA_M5.0.21_os_structure_msghdr_cmsghdr
  return cmsg_data;
}
}  // namespace os_interface
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_OS_INTERFACE_HELPER_H_
