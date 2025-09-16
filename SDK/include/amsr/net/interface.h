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
 *         \unit  osabstraction::net
 *   \complexity  net unit is QM, no action required.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/net/ethernet_address.h"
#include "amsr/net/interface_address_entry.h"
#include "amsr/net/internal/os_constants.h"
#include "amsr/net/internal/specific_types.h"
#include "amsr/net/internal/types.h"
#include "amsr/net/ip/address.h"
#include "amsr/net/ip/address_v4.h"
#include "amsr/net/ip/address_v6.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/io/file_descriptor.h"

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "ara/core/vector.h"
#include "osabstraction/io/native_types.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace net {

/*!
 * \brief           Callback to communicate information about one address on the interface.
 *
 * \param[in]       entry One single address on the interface.
 *
 * \return          \c true if callback required to be executed for the next address on the interface,
 *                  \c false otherwise.
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
using GetAddressCallBack = vac::language::FunctionRef<bool(InterfaceAddressEntry const& entry)>;

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_NetInterface
/*!
 * \brief           Represents a network interface.
 */
class Interface final {
 public:
  /*!
   * \brief           Index that identifies the interface in the operating system.
   */
  using Index = net::internal::InterfaceIndex;

  /*!
   * \brief           Maximum number of characters an network interface name may be long.
   *
   * \details         Buffers to hold that name may be required to fit one character more (the \0 character).
   */
  static constexpr std::uint8_t const kMaxInterfaceName{net::internal::kMaxNetworkInterfaceName};

  /*!
   * \brief           Creates an Interface object from a local IP address.
   *
   * \param[in]       local_address             The address to create the interface from.
   * \param[in]       netstack_id               The network stack to use for interaction with the created interface.
   *                                            Defaults to default network stack.
   *
   * \return          The created interface.
   *
   * \error           OsabErrc::kUnexpected                 Some unexpected internal state was detected.
   *
   * \error           OsabErrc::kSystemEnvironmentError     No interface with that address assigned found.
   *
   * \error           OsabErrc::kInsufficientPrivileges     Insufficient privileges to extract addresses.
   *
   * \error           OsabErrc::kAddressError               Address assigned to multiple interfaces.
   *
   * \error           OsabErrc::kResource                   Not enough system resources to extract the addresses.
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
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Interface> MakeInterface(
      ip::Address const& local_address,
      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept;

  /*!
   * \brief           Makes an interface from a string that either holds its name or index.
   *
   * \details         The string is considered as the interface name if it contains at least one letter and as the
   *                  interface index if it only contains numbers.
   *
   * \param[in]       interface_identification  The string that holds the name or index of the interface.
   * \param[in]       netstack_id               The network stack to use for interaction with the created interface.
   *                                            Defaults to default network stack.
   *
   * \return          The created interface.
   *
   * \error           OsabErrc::kSystemEnvironmentError              No interface identified by that name/index found.
   *
   * \error           OsabErrc::kApiError                            The number represented by the input string is out
   *                                                                 of range for an interface index (std::uint32_t).
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
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Interface> MakeInterface(
      ::amsr::core::StringView interface_identification,
      net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept;

  /*!
   * \brief           Creates an Interface object from an interface index.
   *
   * \details         In case Linux obsolete IP-aliases mechanism is exploited
   *                  (https://www.kernel.org/doc/html/latest/networking/alias.html), the real device interface
   *                  name will used for the object creation.
   *
   * \param[in]       interface_index           The index of the interface.
   * \param[in]       netstack_id               The network stack to use for interaction with the created interface.
   *                                            Defaults to default network stack.
   *
   * \return          The created interface.
   *
   * \error           OsabErrc::kSystemEnvironmentError                 No interface indexed by that index found.
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
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector product internal API
   */
  static ::amsr::core::Result<Interface> MakeInterface(
      Index interface_index, net_utils::NetworkStackId const& netstack_id = net_utils::NetworkStackId{}) noexcept;

  /*!
   * \brief           Extracts the Ethernet address assigned to this interface.
   *
   * \return          The Ethernet address of this interface.
   *
   * \error           OsabErrc::kInsufficientPrivileges                 The process has insufficient privileges to
   *                                                                    extract the assigned address.
   *
   * \error           OsabErrc::kAddressError                           Interface does not have Ethernet address
   *                                                                    assigned.
   *
   * \error           OsabErrc::kUnexpected                             Some unexpected internal state was detected.
   *
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
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<EthernetAddress> ExtractEthAddress() const noexcept;

  /*!
   * \brief           Performs lookup of Ethernet, IPv4, IPv6 addresses assigned to this interface and returns them.
   *
   * \details         Different addresses of the same family will be reported as separate entries.
   *
   * \return          Vector of Ethernet, IPv4, IPv6 address entries assigned to this interface.
   *                  Empty if no supported addresses or no address at all are found for this interface.
   *
   * \error           OsabErrc::kInsufficientPrivileges                 The process has insufficient privileges to
   *                                                                    retrieve the assigned addresses.
   *
   * \error           OsabErrc::kUnexpected                             Some unexpected internal state was detected.
   *
   * \error           OsabErrc::kResource                               Not enough system resources to retrieve the
   *                                                                    assigned addresses.
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
   * \trace           DSGN-Osab-InterfaceTranslation, DSGN-Osab-NetworkDependentAddresses
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<ara::core::Vector<InterfaceAddressEntry>> GetAllAddresses() const noexcept;

  /*!
   * \brief           Performs lookup of Ethernet, IPv4, IPv6 addresses assigned to this interface and pass them to the
   *                  callback function.
   *
   * \details         Different addresses of the same family will be reported by separate callback calls.
   *
   * \param[in]       callback                Callback which is invoked for each address entry found. If the callback
   *                                          returns false, search is discontinued.
   *
   * \error           OsabErrc::kInsufficientPrivileges                 The process has insufficient privileges to
   *                                                                    retrieve the assigned addresses.
   *
   * \error           OsabErrc::kUnexpected                             Some unexpected internal state was detected.
   *
   * \error           OsabErrc::kResource                               Not enough system resources to retrieve the
   *                                                                    assigned addresses.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceTranslation, DSGN-Osab-NetworkDependentAddresses
   *
   * \vprivate        Vector product internal API
   */
  auto GetAllAddresses(amsr::net::GetAddressCallBack&& callback) const noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief           Interface status
   * \vprivate        Vector product internal API
   */
  enum class InterfaceStatus : std::uint8_t {
    /*!
     * \brief    The administrative and operational state of the interface is set.
     */
    kUp = 0x00,

    /*!
     * \brief    The administrative or operational state of the interface is NOT set.
     */
    kDown = 0x01,
  };

  /*!
   * \brief           Extracts the status of this interface
   *
   * \details         Checks if the administrative and operational state of the interface is set.
   *
   * \return          InterfaceStatus::kUp if the administrative and operational state of the network interface is set,
   *                  else InterfaceStatus::kDown.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Interface isn't available anymore.
   *
   * \context         ANY
   *
   * \pre             The interface name should not be longer than kMaxNetworkInterfaceName.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceStatus, DSGN-Osab-QNXInterfaceNotUp
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<InterfaceStatus> ExtractStatus() noexcept;

  /*!
   * \brief           Set the status of this network interface to bring it either up or down.
   *
   * \details         This API affects only the operational state of the interface. To subsequently check whether the
   *                  interface is actually ready to transfer data, use ExtractStatus() and check whether
   *                  the status is UP.
   *
   * \param[in]       interface_status  InterfaceStatus::kUp to set the operational state of the network interface to
   *                                    up. InterfaceStatus::kDown to set the operational state of the network interface
   *                                    to down. This routine opens a socket to extract the network interface status.
   *                                    The socket will be closed at the end of the routine call.
   *
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *                                                                    This error also occur if you try to use this
   *                                                                    function with PikeOS.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Interface isn't available anymore. Or it wasn't
   *                                                                    possible to set the flags to change the status
   *                                                                    of the network interface status.
   *
   * \context         ANY
   *
   * \pre              -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-InterfaceStatus
   *
   * \vprivate        Vector product internal API
   */
  ::amsr::core::Result<void> SetStatus(InterfaceStatus interface_status) noexcept;

  /*!
   * \brief           Returns the index that identifies the interface.
   *
   * \return          The index that identifies the interface.
   *
   * \context         ANY
   *
   * \pre             The interface name should not be longer than kMaxNetworkInterfaceName.
   *
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   *
   * \vprivate        Vector component internal API
   */
  Index GetIndex() const noexcept { return index_; }

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
   *
   * \vprivate        Vector product internal API
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
   *
   *
   * \vprivate        Vector product internal API
   */
  bool operator!=(Interface const& other) const noexcept;

  /*!
   * \brief           Default constructor.
   *
   * \details         The created Interface cannot be used by any functionality (like endpoint, socket option
   *                  creation...), because it has not been properly initialized. MakeInterface ensures proper
   *                  object initialization.
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
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  Interface() noexcept : Interface(Index{0}, {}, net_utils::NetworkStackId{}) {}

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
   */
  net_utils::NetworkStackId const& GetNetworkStackId() const noexcept { return network_stack_id_; }

 private:
  /*!
   * \brief           Creates an interface object from an index value and a name.
   *
   * \details         There is no further check if the index or the name are valid.
   *
   * \param[in]       index                     The index of the interface that shall be represented.
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
   *
   * \trace           DSGN-Osab-InterfaceTranslation
   *
   * \vprivate        Vector component internal API
   */
  explicit Interface(Index index, ::amsr::core::Array<char, kMaxInterfaceName + 1> const& name,
                     net_utils::NetworkStackId netstack_id) noexcept
      : index_{index}, name_{name}, network_stack_id_(std::move(netstack_id)) {}

  /*!
   * \brief           Opens an AF_INET socket and returns it.
   *
   * \return          The file descriptor to an open AF_INET socket.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources to open the socket.
   *
   * \context         ANY
   *
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   *\trace            DSGN-Osab-InterfaceStatus
   *
   * \vprivate        Vector component internal API
   */
  ::amsr::core::Result<osabstraction::io::FileDescriptor> OpenInternalSocket() const noexcept;

  /*!
   * \brief           Extracts the active flags of the Interface.
   *
   * \details         The function internally calls an Ioctl() with SIOCGIFFLAGS to get the ifreq struct and
   *                  returns the ifreq.ifr_flags
   *
   * \return          The ifr_flags of this Interface.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Interface isn't available anymore
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceStatus
   *
   * \vprivate        Vector component internal API
   */
  ::amsr::core::Result<amsr::net::internal::IfrIfFlags> ExtractIfFlags() noexcept;

  /*!
   * \brief           Set flags of the Interface.
   *
   * \details         The function internally calls an Ioctl() with SIOCSIFFLAGS and sets the ifreq struct.
   *
   * \param           ifreq_flags Flags that should be set.
   *
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  The process has insufficient privileges.
   *
   * \error           osabstraction::OsabErrc::kResource                Not enough system resources.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Interface isn't available anymore
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       TRUE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-InterfaceStatus
   *
   * \vprivate        Vector component internal API
   */
  ::amsr::core::Result<void> SetIfFlags(amsr::net::internal::IfrIfFlags ifreq_flags) noexcept;

  /*!
   * \brief           The index of this interface.
   */
  Index index_;

  /*!
   * \brief           The null terminated name of this interface.
   */
  ::amsr::core::Array<char, kMaxInterfaceName + 1> name_;

  /*!
   * \brief           ID of the network stack used to interact with this interface.
   */
  net_utils::NetworkStackId network_stack_id_;
};

}  // namespace net
}  // namespace amsr

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_NET_INTERFACE_H_
