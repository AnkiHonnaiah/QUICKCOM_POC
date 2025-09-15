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
 *        \brief  Class to represent Ethernet network endpoints.
 *
 *         \unit  amsr::ptp::Endpoint
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_ETHERNET_ENDPOINT_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_ETHERNET_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/ptp/ethernet_address.h"
#include "amsr/ptp/interface.h"
#include "amsr/ptp/types.h"
#include "ara/core/abort.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ptp {
namespace internal {

/*!
 * \brief           Represents a PTP communication endpoint.
 *
 * \details         A communication endpoint uniquely identifies an addressable point in a network.
 *
 * \trace           DSGN-Osab-PtpEndpointCreation
 */
class Endpoint final {
 public:
  /*!
   * \brief           Creates an Endpoint from an Ethernet address and a local network interface.
   *
   * \param[in]       ether_address             Ethernet address the endpoint shall contain.
   * \param[in]       interface                 Interface the endpoint shall contain. The created endpoint uses the
   *                                            network stack this interface instance also uses.
   *
   * \return          The created endpoint.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static auto MakeEndpoint(EthernetAddress ether_address, Interface const& interface) noexcept -> Endpoint {
    return Endpoint{ether_address, interface, kInvalidVlanId};
  }

  /*!
   * \brief           Creates an Endpoint from an Ethernet address, local network interface and VLAN ID.
   *
   * \param[in]       ether_address             Ethernet address the endpoint shall contain.
   * \param[in]       interface                 Interface the endpoint shall contain. Must be physical network interface
   *                                            and not a VLAN network interface. The created endpoint uses the network
   *                                            stack this interface instance also uses.
   * \param[in]       vlan_id                   Valid ID of the VLAN that the endpoint shall be associated with.
   *                                            Must be in the range [1, 4094].
   *
   * \return          The created endpoint.
   * \error           osabstraction::OsabErrc::kAddressError   The specified VLAN ID is invalid.
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
  /*!
   * \internal
   * - Create and return the endpoint from the passed parameters or return an error if the VLAN ID is invalid.
   * \endinternal
   */
  // NOLINTNEXTLINE(modernize-pass-by-value)
  static auto MakeEndpoint(EthernetAddress ether_address, Interface const& interface, VlanId vlan_id) noexcept
      -> ::amsr::core::Result<Endpoint> {
    ::amsr::core::Result<Endpoint> result{MakeErrorCode(
        osabstraction::OsabErrc::kAddressError, static_cast<::amsr::core::ErrorDomain::SupportDataType>(vlan_id.value),
        "EthernetEndpoint::MakeEndpoint(): Passed invalid VLAN ID.")};
    if (IsVlanIdValid(vlan_id)) {
      result.EmplaceValue(Endpoint{ether_address, interface, vlan_id});
    }
    return result;
  }

  /*!
   * \brief           Creates an empty endpoint that uses the passed network stack.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Endpoint() noexcept : Endpoint(EthernetAddress{}, Interface{}, kInvalidVlanId) {}

  /*!
   * \brief           Outputs the contained Ethernet address.
   *
   * \return          The contained Ethernet address.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetAddress() const noexcept -> EthernetAddress const& { return ethernet_address_; }

  /*!
   * \brief           Outputs the contained interface.
   *
   * \return          The contained interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetInterface() const noexcept -> Interface const& { return interface_; }

  /*!
   * \brief           Outputs the contained VLAN ID.
   *
   * \return          The contained VLAN ID.
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
  auto GetVlanId() const noexcept -> VlanId { return vlan_id_; }

  /*!
   * \brief           Compares two endpoints for equality.
   *
   * \param[in]       other   The endpoint to compare with this endpoint.
   *
   * \return          True if the endpoints contain the same Ethernet address, interface and VLAN Id, else false.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  /*!
   * \internal
   * - Compare the interfaces, addresses and vlan IDs of the endpoints and output the result.
   * \endinternal
   */
  auto operator==(Endpoint const& other) const noexcept -> bool {
    bool const interfaces_equal{interface_ == other.interface_};
    bool const addresses_equal{ethernet_address_ == other.ethernet_address_};
    bool const vlans_equal{vlan_id_ == other.vlan_id_};
    return interfaces_equal && addresses_equal && vlans_equal;
  }

  /*!
   * \brief           Compares two endpoints for inequality.
   *
   * \param[in]       other   The endpoint to compare with this endpoint.
   *
   * \return          False if the endpoints contain the same Ethernet address, interface and VLAN ID, else true.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE if the input is thread-safe
   * \steady          TRUE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator!=(Endpoint const& other) const noexcept -> bool { return !(*this == other); }

 private:
  /*!
   * \brief           Creates an endpoint.
   *
   * \param[in]       ethernet_address      Ethernet address of this endpoint.
   * \param[in]       interface             Interface of this endpoint.
   * \param[in]       vlan_id               ID of the VLAN that the endpoint shall be associated with. kInvalidVlanId if
   *                                        shall not be associated with a VLAN.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  // NOLINTNEXTLINE(modernize-pass-by-value)
  explicit Endpoint(EthernetAddress const& ethernet_address, Interface const& interface, VlanId vlan_id) noexcept
      : ethernet_address_{ethernet_address}, interface_{interface}, vlan_id_{vlan_id} {}

  /*!
   * \brief   Ethernet address assigned to this endpoint.
   */
  EthernetAddress ethernet_address_;

  /*!
   * \brief   Interface assigned to this endpoint.
   */
  Interface interface_;

  /*!
   * \brief   ID of VLAN associated with this endpoint.
   * \details kInvalidVlanId if no VLAN is associated with this endpoint.
   */
  VlanId vlan_id_;
};

}  // namespace internal
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_INTERNAL_ETHERNET_ENDPOINT_H_
