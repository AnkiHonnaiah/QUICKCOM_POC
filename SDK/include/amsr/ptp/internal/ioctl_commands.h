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
 *        \brief  Stores command objects that can be passed to the ioctl call.
 *
 *         \unit  amsr::ptp::OsInterface
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_IOCTL_COMMANDS_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_IOCTL_COMMANDS_H_

#include <linux/ethtool.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <net/if.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "amsr/core/abort.h"
#include "amsr/core/string_view.h"
#include "amsr/ptp/internal/os_specific_constants.h"
#include "amsr/ptp/internal/types.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief Configure the hardware time stamping for a network device.
 *
 * Sets the hardware time stamping mode in the device driver. The device driver is identified by the passed interface.
 * This option changes the device driver setting for the whole system. The original setting will not be restored if the
 * socket is closed. Therefore the class provides a function to get or set the raw operating system configuration.
 *
 * The process needs admin rights to change the configuration.
 *
 * The internal Linux data structure is passed to the kernel. The flags are hints to the kernel. The kernel may decide
 * to use a more permissive configuration than the requested one. The hardware driver updates the data structure
 * according to the supported time stamping configuration.
 */
class IOControlSetHwTimeStampingConfig final {
 public:
  /*!
   * \brief Constructs a time stamping configuration for a specific interface.
   *
   * \details Initializes the req data structure and copies the interface name to the char buffer.
   *
   * \param[in] interface_name Name of the interface. The view must point to a valid interface name. The length must
   *                           not exceed kMaxNetworkInterfaceName length.
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
   * - Check that interface name size does not exceed the limits.
   * - Prepare interface request structure. The interface request's ifr_data member points to the timestamp
   *   configuration.
   * - Copy interface name into interface request structure.
   */
  explicit IOControlSetHwTimeStampingConfig(::amsr::core::StringView interface_name) {
    if (interface_name.size() > kMaxNetworkInterfaceName) {
      ::amsr::core::Abort("Invalid interface name. The name cannot be stored in an ifreq structure.");
    }
    // VECTOR Next Line AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastLinuxHardwareTimeStamp
    this->interface_request_.ifr_data = reinterpret_cast<decltype(this->interface_request_.ifr_data)>(&(this->config_));
    static_cast<void>(
        std::copy(interface_name.cbegin(), interface_name.cend(), std::begin(this->interface_request_.ifr_name)));
  }

  /*!
   * \brief Deleted copy constructor
   */
  IOControlSetHwTimeStampingConfig(IOControlSetHwTimeStampingConfig const&) = delete;

  /*!
   * \brief Deleted assignment operator.
   */
  IOControlSetHwTimeStampingConfig& operator=(IOControlSetHwTimeStampingConfig const&) = delete;

  /*!
   * \brief Deleted move constructor
   */
  IOControlSetHwTimeStampingConfig(IOControlSetHwTimeStampingConfig&&) = delete;

  /*!
   * \brief Deleted move assignment operator
   */
  IOControlSetHwTimeStampingConfig& operator=(IOControlSetHwTimeStampingConfig&&) = delete;

  /*!
   * \brief Destroys the object.
   */
  ~IOControlSetHwTimeStampingConfig() noexcept = default;

  /*!
   * \brief Enable hardware time stamping for incoming PTP Ethernet packages.
   *
   * \details The function chooses the enable PTP time stamping for Ethernet packages. However, the driver may choose to
   *          use a more permissive configuration.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void EnablePtpRxHwTimestamping() noexcept { this->config_.rx_filter = HWTSTAMP_FILTER_PTP_V2_L2_EVENT; }

  /*!
   * \brief Enable hardware time stamping for outgoing Ethernet packages.
   *
   * \details The function chooses the enable time stamping for all outgoing Ethernet packages. However, the driver may
   *          choose to use a more permissive configuration.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \vprivate        Vector component internal API
   */
  void EnableTxHwTimestamping() noexcept { this->config_.tx_type = HWTSTAMP_TX_ON; }

  /*!
   * \brief Returns the ioctl command's name.
   *
   * \return The ioctl command's name.
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
  static constexpr auto GetName() noexcept -> IoctlCmdName { return IoctlCmdName{SIOCSHWTSTAMP}; }

  /*!
   * \brief Returns an mutable view to the internal data.
   *
   * \details This function is intended to be used solely with ioctl() calls.
   *
   * \return          Mutable view of the internal data.
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
  auto GetDataView() noexcept -> IoctlMutableValueView { return IoctlMutableValueView{&(this->interface_request_)}; }

 private:
  /*!
   * \brief The interface request structure.
   *
   * \details The interface request structure defines the interface that shall be configured.
   */
  struct ifreq interface_request_ {};
  /*!
   * \brief Linux specific type to store the hardware time stamping configuration.
   *
   * \details The default constructed option disables all time stamping.
   */
  struct hwtstamp_config config_ {
    0,                       /*!< No flags are used. */
        HWTSTAMP_TX_OFF,     /*!< No timestamping for outgoing packages. */
        HWTSTAMP_FILTER_NONE /*!< No timestamping for incoming packages. */
  };
};

/*!
 * \brief   Get timestamping capabilities of interface.
 *
 * \details Uses the SIOCETHTOOL ioctl with the ETHTOOL_GET_TS_INFO value to read out the timestamping capabilities of
 *          the interface.
 */
class IOControlInterfaceTimestampingInfo final {
 public:
  /*!
   * \brief Constructs a time stamping info ioctl for a specific interface.
   *
   * \details Initializes the req data structure and copies the interface name to the char buffer.
   *
   * \param[in] interface_name Name of the interface. The view must point to a valid interface name. The length must
   *                           not exceed kMaxNetworkInterfaceName length.
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
   * - Check that interface name size does not exceed the limits.
   * - Prepare interface request structure. The interface request's ifr_data member points to the timestamp
   *   configuration.
   * - Copy interface name into interface request structure.
   */
  explicit IOControlInterfaceTimestampingInfo(::amsr::core::StringView interface_name) {
    if (interface_name.size() > kMaxNetworkInterfaceName) {
      ::amsr::core::Abort("Invalid interface name. The name cannot be stored in an ifreq structure.");
    }
    // VECTOR Next Construct AutosarC++17_10-A5.2.4: MD_OSA_A5.2.4_ReinterpretCastSocketDataStructure
    this->interface_request_.ifr_data =
        reinterpret_cast<decltype(this->interface_request_.ifr_data)>(&(this->timestamping_info_));
    static_cast<void>(
        std::copy(interface_name.cbegin(), interface_name.cend(), std::begin(this->interface_request_.ifr_name)));
  }

  /*!
   * \brief Deleted copy constructor
   */
  IOControlInterfaceTimestampingInfo(IOControlInterfaceTimestampingInfo const&) = delete;

  /*!
   * \brief Deleted assignment operator.
   */
  IOControlInterfaceTimestampingInfo& operator=(IOControlInterfaceTimestampingInfo const&) = delete;

  /*!
   * \brief Deleted move constructor
   */
  IOControlInterfaceTimestampingInfo(IOControlInterfaceTimestampingInfo&&) = delete;

  /*!
   * \brief Deleted move assignment operator
   */
  IOControlInterfaceTimestampingInfo& operator=(IOControlInterfaceTimestampingInfo&&) = delete;

  /*!
   * \brief Destroys the object.
   */
  ~IOControlInterfaceTimestampingInfo() noexcept = default;

  /*!
   * \brief Check if the interface supports hardware receive timestamping.
   *
   * \details The ioctl has to be performed before this function can return the correct result.
   *          timestamping_info.rx_filters is not checked because this already happens in
   *          PtpNetworkInterfaceControl::ChangeHwTimestamping().
   *
   * \return          true if hardware receive timestamping is supported, false otherwise.
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
  bool IsRxHwTimestampingSupported() const noexcept {
    return ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_RAW_HARDWARE)) !=
            0) &&
           ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_RX_HARDWARE)) != 0);
  }

  /*!
   * \brief Check if the interface supports software receive timestamping.
   *
   * \details The ioctl has to be performed before this function can return the correct result.
   *          timestamping_info.rx_filters is not checked because this already happens in
   *          PtpNetworkInterfaceControl::ChangeHwTimestamping().
   *
   * \return          true if software receive timestamping is supported, false otherwise.
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
  bool IsRxSwTimestampingSupported() const noexcept {
    return ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_SOFTWARE)) != 0) &&
           ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_RX_SOFTWARE)) != 0);
  }

  /*!
   * \brief Check if the interface supports hardware send timestamping.
   *
   * \details The ioctl has to be performed before this function can return the correct result.
   *          timestamping_info.tx_types is not checked because this already happens in
   *          PtpNetworkInterfaceControl::ChangeHwTimestamping().
   *
   * \return          true if software receive timestamping is supported, false otherwise.
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
  bool IsTxHwTimestampingSupported() const noexcept {
    return ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_RAW_HARDWARE)) !=
            0) &&
           ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_TX_HARDWARE)) != 0);
  }

  /*!
   * \brief Check if the interface supports software send timestamping.
   *
   * \details The ioctl has to be performed before this function can return the correct result.
   *          timestamping_info.tx_types is not checked because this already happens in
   *          PtpNetworkInterfaceControl::ChangeHwTimestamping().
   *
   * \return          true if software send timestamping is supported, false otherwise.
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
  bool IsTxSwTimestampingSupported() const noexcept {
    return ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_SOFTWARE)) != 0) &&
           ((this->timestamping_info_.so_timestamping & static_cast<std::uint16_t>(SOF_TIMESTAMPING_TX_SOFTWARE)) != 0);
  }

  /*!
   * \brief Returns the ioctl command's name.
   *
   * \return          The ioctl command's name.
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
  static constexpr auto GetName() noexcept -> IoctlCmdName { return IoctlCmdName{SIOCETHTOOL}; }

  /*!
   * \brief Returns an mutable view to the internal data.
   *
   * \details This function is intended to be used solely with ioctl() calls.
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
  auto GetDataView() noexcept -> IoctlMutableValueView { return IoctlMutableValueView{&(this->interface_request_)}; }

 private:
  /*!
   * \brief The interface request structure.
   *
   * \details The interface request structure defines the interface from which information shall be extracted.
   */
  struct ifreq interface_request_ {};
  /*!
   * \brief Linux specific type to store the timestamping information of an interface.
   *
   * \details Initialized with the correct ethtool command to extract timestamping information of the interface.
   */
  struct ethtool_ts_info timestamping_info_ {
    ETHTOOL_GET_TS_INFO, /*!< Get time stamping. */
        0, 0, 0, {0, 0, 0}, 0, {
      0, 0, 0
    }
  };
};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_INTERNAL_IOCTL_COMMANDS_H_
