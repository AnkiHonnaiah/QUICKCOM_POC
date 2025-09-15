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
 *        \brief  Class to represent network interfaces.
 *
 *         \unit  amsr::ptp::Endpoint
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERFACE_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/net_utils/network_stack_id.h"
#include "amsr/ptp/ethernet_address.h"
#include "amsr/ptp/internal/os_specific_constants.h"
#include "amsr/ptp/internal/types.h"

namespace amsr {
namespace ptp {

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_NetInterface
/*!
 * \brief           Represents a network interface.
 *
 * \trace           DSGN-Osab-PtpInterfaces
 */
class Interface final {
 public:
  /*!
   * \brief           Maximum number of characters an network interface name may be long.
   *
   * \details         Buffers to hold that name may be required to fit one character more (the \0 character).
   */
  static constexpr std::uint8_t const kMaxInterfaceName{internal::kMaxNetworkInterfaceName};

  /*!
   * \brief           Makes an interface from a string that holds its name.
   *
   * \param[in]       interface_name            The string that holds the name of the interface.
   * \param[in]       netstack_id               The network stack to use for interaction with the created interface.
   *                                            Defaults to default network stack.
   *
   * \return          The created interface.
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
  static Interface MakeInterface(::amsr::core::StringView interface_name,
                                 net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept;

  /*!
   * \brief           Extracts the Ethernet address assigned to this interface.
   *
   * \return          The Ethernet address of this interface.
   *
   * \error           OsabErrc::kInsufficientPrivileges                 The process has insufficient privileges to
   *                                                                    extract the assigned address.
   * \error           OsabErrc::kAddressError                           The interface is not available.
   * \error           OsabErrc::kAddressError                           Interface does not have Ethernet address
   *                                                                    assigned.
   * \error           OsabErrc::kUnexpected                             Some unexpected internal state was detected.
   * \error           OsabErrc::kResource                               Not enough system resources to extract the
   *                                                                    address.
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
  ::amsr::core::Result<EthernetAddress> ExtractEthAddress() const noexcept;

  /*!
   * \brief           Outputs the name of the interface.
   *
   * \param[out]      buffer                    Buffer to write the \0-terminated name string into.
   *
   * \return          Number of characters the name consists of (terminating \0 character not counted).
   *
   * \context         ANY
   *
   * \pre             buffer must be able to hold at least kMaxInterfaceName + 1 characters.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::core::Span<char>::size_type GetName(::amsr::core::Span<char> buffer) const noexcept;

  /*!
   * \brief           Outputs the name of the interface.
   *
   * \details         Allocates memory to hold the name of the interface.
   *
   * \return          The name of the interface, empty string if the interface is not available.
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
  ::amsr::core::String GetName() const noexcept;

  /*!
   * \brief           Compares this interface to another one for equality.
   *
   * \param[in]       other                     The interface to compare with.
   *
   * \return          True if the instances identify the same network interface managed by the same network stack, else
   *                  false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(Interface const& other) const noexcept;

  /*!
   * \brief           Compares this interface to another one for inequality.
   *
   * \param[in]       other                     The interface to compare with.
   *
   * \return          True if the instances identify different network interfaces managed by the same network stack or
   *                  interfaces managed by different network stacks, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator!=(Interface const& other) const noexcept { return !(*this == other); }

  /*!
   * \brief           Default constructor.
   *
   * \details         The created Interface cannot be used by any functionality (like endpoint creation...), because it
   *                  has not been properly initialized. MakeInterface ensures proper object initialization.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Interface() noexcept : Interface({}, net_utils::NetworkStackId{}) {}  // VCA_OSA_CALL_CTOR

  /*!
   * \brief           Outputs the ID of the network stack used to interact with this interface.
   *
   * \return          The ID of the network stack. Reference is only valid as long as this object exists.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  net_utils::NetworkStackId const& GetNetworkStackId() const noexcept { return network_stack_id_; }

 private:
  /*!
   * \brief           Creates an interface object from an index value and a name.
   *
   * \details         There is no further check if the index or the name are valid.
   *
   * \param[in]       name                      The null terminated name of the interface that shall be represented.
   * \param[in]       netstack_id               ID of the network stack used to interact with this interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate        Vector component internal API
   */
  explicit Interface(::amsr::core::Array<char, kMaxInterfaceName + 1> const& name,
                     net_utils::NetworkStackId netstack_id) noexcept
      : name_{name}, network_stack_id_(std::move(netstack_id)) {}

  /*!
   * \brief           The null terminated name of this interface.
   */
  ::amsr::core::Array<char, kMaxInterfaceName + 1> name_;

  /*!
   * \brief           ID of the network stack used to interact with this interface.
   */
  net_utils::NetworkStackId network_stack_id_;
};

}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERFACE_H_
