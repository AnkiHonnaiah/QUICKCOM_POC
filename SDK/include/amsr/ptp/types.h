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
 *        \brief  Collection of type safe types that are used in PTP.
 *
 *         \unit  amsr::ptp::Endpoint
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_TYPES_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_TYPES_H_

#include <cstdint>

namespace amsr {
namespace ptp {

/*!
 * \brief VLAN ID of an Ethernet frame.
 */
struct VlanId {
  /*!
   * \brief VLAN ID.
   */
  std::uint16_t value;
};

/*!
 * \brief Invalid VLAN ID.
 */
constexpr VlanId kInvalidVlanId{65535u};

/*!
 * \brief Compares two VlanId for equality.
 *
 * \param[in] lhs  First VlanId
 * \param[in] rhs  Second VlanId
 *
 * \return          true if the IDs are equal, false otherwise.
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
inline auto operator==(VlanId const& lhs, VlanId const& rhs) -> bool { return lhs.value == rhs.value; }

/*!
 * \brief Compares two VlanId for inequality.
 *
 * \param[in] lhs  First VlanId
 * \param[in] rhs  Second VlanId
 *
 * \return          true if the IDs are unequal, false otherwise.
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
inline auto operator!=(VlanId const& lhs, VlanId const& rhs) -> bool { return !(lhs == rhs); }

/*!
 * \brief Compares two vlan IDs for less-than order.
 *
 * \param[in] lhs  First VlanId
 * \param[in] rhs  Second VlanId
 *
 * \return          true if this ID < the other vlan, else false.
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
inline auto operator<(VlanId const& lhs, VlanId const& rhs) -> bool { return lhs.value < rhs.value; }

/*!
 * \brief Compares two vlan IDs for greater-than order.
 *
 * \param[in] lhs  First VlanId
 * \param[in] rhs  Second VlanId
 *
 * \return          true if this ID > the other vlan, else false.
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
inline auto operator>(VlanId const& lhs, VlanId const& rhs) -> bool { return lhs.value > rhs.value; }

/*!
 * \brief Checks if a VLAN ID is in the valid range.
 *
 * \param[in] vlan_id  VlanId to check
 *
 * \return          true if the ID is in the valid range, false otherwise.
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
inline auto IsVlanIdValid(VlanId vlan_id) -> bool {
  // We consider VLAN ID 0 as a valid VLAN ID here to allow sending of VLAN frames by TimeSync with the special VLAN ID
  // 0. A VLAN tag with VLAN ID 0 means that the frame does not belong to a VLAN just like a standard Ethernet frame
  // without VLAN tag but in contrast to a standard Ethernet frame without VLAN tag it does contain priority information
  // (PCP field of VLAN tag) which is used in Ethernet switches to prioritize traffic. To correctly receive VLAN frames
  // with VLAN ID 0, ESCAN00117110 needs to be fixed first.
  constexpr std::uint16_t kMaxValidVlanId{4094};
  return vlan_id.value <= kMaxValidVlanId;
}

/*!
 * \brief VLAN priority of an Ethernet frame.
 */
struct VlanPriority {
  /*!
   * \brief VLAN priority..
   */
  std::uint8_t value;
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Invalid VLAN priority.
 */
constexpr VlanPriority kInvalidVlanPriority{255u};

/*!
 * \brief Compares two VlanId for equality.
 *
 * \param[in] lhs  First VlanPriority
 * \param[in] rhs  Second VlanPriority
 *
 * \return          true if the priorities are equal, false otherwise.
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
inline auto operator==(VlanPriority const& lhs, VlanPriority const& rhs) -> bool { return lhs.value == rhs.value; }

/*!
 * \brief Compares two VlanPriority for inequality.
 *
 * \param[in] lhs  First VlanPriority
 * \param[in] rhs  Second VlanPriority
 *
 * \return          true if the priorities are unequal, false otherwise.
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
inline auto operator!=(VlanPriority const& lhs, VlanPriority const& rhs) -> bool { return !(lhs == rhs); }

/*!
 * \brief Compares two vlan priorities for less-than order.
 *
 * \param[in] lhs  First VlanPriority
 * \param[in] rhs  Second VlanPriority
 *
 * \return          true if this priority < the other vlan priority, else false.
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
inline auto operator<(VlanPriority const& lhs, VlanPriority const& rhs) -> bool { return lhs.value < rhs.value; }

/*!
 * \brief Compares two vlan priorities for greater-than order.
 *
 * \param[in] lhs  First VlanPriority
 * \param[in] rhs  Second VlanPriority
 *
 * \return          true if this priority > the other vlan priority, else false.
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
inline auto operator>(VlanPriority const& lhs, VlanPriority const& rhs) -> bool { return lhs.value > rhs.value; }

/*!
 * \brief Checks if a VLAN priority is in the valid range.
 *
 * \param[in] vlan_priority  VlanPriority to check
 *
 * \return          true if the priority is in the valid range, false otherwise.
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
inline auto IsVlanPriorityValid(VlanPriority vlan_priority) -> bool {
  constexpr std::uint16_t kMaxValidVlanPriority{7};
  return vlan_priority.value <= kMaxValidVlanPriority;
}

}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_PTP_TYPES_H_
