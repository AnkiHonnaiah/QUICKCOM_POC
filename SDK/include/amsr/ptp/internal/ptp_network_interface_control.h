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
/*!        \file  ptp_network_interface_control.h
 *        \brief  Controls hardware time stamping in network drivers.
 *
 *         \unit  amsr::ptp::PtpNetworkInterfaceControl
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_PTP_NETWORK_INTERFACE_CONTROL_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_PTP_NETWORK_INTERFACE_CONTROL_H_

#include "amsr/core/result.h"
#include "amsr/ptp/interface.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief Configures hardware time stamping in network drivers.
 *
 * \details A network driver may be selected by passing an interface associated to a network driver. On some operating
 *          systems this changes the systems wide network driver configuration. It is up to the application to restore
 *          the original network settings.
 */
class PtpNetworkInterfaceControl {
 public:
  /*!
   * \brief Specifies which Ethernet packets shall be hardware timestamped by the network adapter.
   */
  enum class HwTimestampingMode : std::uint32_t {
    /*!
     * \brief Hardware timestamp no Ethernet packets.
     */
    kNone,
    /*!
     * \brief Hardware timestamp only received Ethernet packets.
     */
    kRx,
    /*!
     * \brief Hardware timestamp only sent Ethernet packets.
     */
    kTx,
    /*!
     * \brief Hardware timestamp received and sent Ethernet packets.
     */
    kRxTx
  };

  /*!
   * \brief Change Ethernet packet hardware timestamping configuration for passed interface.
   *
   * \details Configures PTP hardware time stamping.
   *
   * \param[in] interface             Valid interface whose configuration shall be changed.
   * \param[in] hw_timestamping_mode  Select if and which ethernet packages shall be timestamped.
   *
   * \return Result of operation.
   *
   * \error osabstraction::OsabErrc::kUnexpected                Unexpected error that is mapped to no category.
   * \error osabstraction::OsabErrc::kInsufficientPrivileges    The process has insufficient privileges to change
   *                                                            interface configuration.
   * \error osabstraction::OsabErrc::kSystemEnvironmentError    The requested configuration is not supported by the
   *                                                            passed interface.
   * \error osabstraction::OsabErrc::kApiError                  OsAbstraction does not support all the requested
   *                                                            hardware timestamping modes on this operating system.
   * \error osabstraction::OsabErrc::kAddressError              Interface is not available.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static auto ChangeHwTimestamping(Interface const& interface, HwTimestampingMode hw_timestamping_mode) noexcept
      -> ::amsr::core::Result<void>;
};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_PTP_NETWORK_INTERFACE_CONTROL_H_
