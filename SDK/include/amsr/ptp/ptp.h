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
 *        \brief  Refers the PTP protocol.
 *         \unit  amsr::ptp::Endpoint
 *
 *   \complexity  The PTP library is not meant to be used yet. Therefore no action is required.
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_PTP_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_PTP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ptp/internal/basic_ether_socket.h"
#include "amsr/ptp/internal/ptp_network_interface_control.h"

namespace amsr {
namespace ptp {

/*!
 * \brief           Class that refers the PTP protocol.
 */
class Ptp {
 public:
  /*!
   * \brief           The endpoint used when using PTP.
   */
  using Endpoint = amsr::ptp::internal::Endpoint;

  /*!
   * \brief           The socket used when using PTP.
   */
  using Socket = amsr::ptp::internal::BasicEtherSocket;

  /*!
   * \brief           Interface to configure hardware time stamping for an interface.
   */
  using InterfaceControl = amsr::ptp::internal::PtpNetworkInterfaceControl;

  /*!
   * \brief          PTP EtherType value in Ethernet frame.
   *
   * \details        This value has been assigned by IEEE Registration Authority to identify the Precision Time Protocol
   *                 over Ethernet, and can be used to perform protocol specific actions on the payload.
   *                 This is the representation in machine byte order.
   */
  static constexpr std::uint16_t const kEtherTypePtp{0x88F7U};
};

}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_PTP_H_
