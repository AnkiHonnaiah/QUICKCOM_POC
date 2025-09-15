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
/*!       \file   ioctl_commands_common.h
 *        \brief  Stores and handles command objects that can be passed to the ioctl call.
 *        \unit   amsr::net
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_IOCTL_COMMANDS_COMMON_H_
#define LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_IOCTL_COMMANDS_COMMON_H_

#include <net/if.h>
#include <sys/ioctl.h>
#include <algorithm>
#include <cstdint>
#include "amsr/core/abort.h"
#include "amsr/core/string_view.h"
#include "amsr/net/internal/specific_types.h"

namespace amsr {
namespace net {
namespace internal {

/*!
 * \brief Implements IO control command that accepts an ifreq structure.
 *
 * \tparam  Name  Type of the IO control request representing the ioctl command's name.
 */
template <IoctlRequestType Name>
class IOControlInterfaceCommand final {
 public:
  /*!
   * \brief Constructs a command for a specific interface.
   *
   * \details
   * Initializes the req data structure and copies the interface name to the char buffer.
   *
   * \param[in] interface_name Name of the interface. The view must point to a valid interface name. The length must
   *                           not exceed kMaxNetworkInterfaceName length.
   * \context         ANY
   * \pre             The interface name should not be longer than kMaxNetworkInterfaceName.
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector component internal API
   */
  /*!
   * \internal
   * - Check that interface name size does not exceed the limits.
   * - Copy interface name into interface request structure.
   * \endinternal
   */
  explicit IOControlInterfaceCommand(::amsr::core::StringView interface_name) {
    if (interface_name.size() > kMaxNetworkInterfaceName) {
      ::amsr::core::Abort("Invalid interface name. The name cannot be stored in an ifreq structure.");
    }
    static_cast<void>(
        std::copy(interface_name.cbegin(), interface_name.cend(), std::begin(this->interface_request_.ifr_name)));
  }

  /*!
   * \brief Deleted copy constructor
   */
  IOControlInterfaceCommand(IOControlInterfaceCommand const&) = delete;

  /*!
   * \brief Deleted assignment operator.
   */
  IOControlInterfaceCommand& operator=(IOControlInterfaceCommand const&) = delete;

  /*!
   * \brief Deleted move constructor
   */
  IOControlInterfaceCommand(IOControlInterfaceCommand&&) = delete;

  /*!
   * \brief Deleted move assignment operator
   */
  IOControlInterfaceCommand& operator=(IOControlInterfaceCommand&&) = delete;

  /*!
   * \brief Destroys the object.
   */
  ~IOControlInterfaceCommand() noexcept = default;

  /*!
   * \brief Returns the ioctl command's name.
   * \return The ioctl command's name.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector component internal API
   */
  static constexpr auto GetName() noexcept -> IoctlCmdName { return IoctlCmdName{Name}; }

  /*!
   * \brief Returns a mutable view to the internal data.
   * \details This function is intended to be used solely with ioctl() calls.
   * \return          Mutable view to the internal data.
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector component internal API
   */
  auto GetDataView() noexcept -> IoctlMutableValueView { return IoctlMutableValueView{&(this->interface_request_)}; }

  /*!
   * \brief           Set the flags the stored Flags
   * \param[in]       flags flag that should be set
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector component internal API
   */
  void SetFlags(IfrIfFlags flags) { this->interface_request_.ifr_flags = flags; }

  /*!
   * \brief           Returns the stored flags as a copy
   * \return          returns the stored  ifreq.ifr_flags as a copy
   *
   * \context         ANY
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector component internal API
   */
  IfrIfFlags GetFlags() const noexcept { return this->interface_request_.ifr_flags; }

 private:
  /*!
   * \brief The interface request structure.
   *
   * \details
   * The interface request structure defines the interface that shall be configured.
   */
  ifreq interface_request_{};
};
}  // namespace internal
}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_POSIX_INCLUDE_AMSR_NET_INTERNAL_IOCTL_COMMANDS_COMMON_H_
