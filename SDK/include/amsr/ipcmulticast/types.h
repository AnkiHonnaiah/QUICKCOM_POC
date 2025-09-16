/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  Multicast package memory types.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPCMULTICAST_COMMON_INCLUDE_AMSR_IPCMULTICAST_TYPES_H_
#define LIB_IPCMULTICAST_COMMON_INCLUDE_AMSR_IPCMULTICAST_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/ipc/unicast_address.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace ipcmulticast {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCMULTICAST_AutosarC++17_10-M7.3.6_sv
/*! String literals. */
using vac::container::literals::operator""_sv;

/*! Logger context id for IPC multicast. */
static constexpr amsr::core::StringView kIpcMulticastLoggerContextId{"vImc"_sv};

/*! PacketBuffer. */
using PacketBuffer = ara::core::Vector<std::uint8_t>;

/*! PacketView */
using PacketView = amsr::core::Span<std::uint8_t const>;

/*! Maximum packet size in bytes.
 *
 * Leave 1024 bytes for protocol overhead in different implementations.
 */
static constexpr std::size_t kMaximumPacketSize{static_cast<std::size_t>(63) * 1024};

/*! Definition of Basic IPC multicast address, domain part. */
using Domain = std::uint32_t;

/*! Definition Basic IPC multicast address, group part. */
using Group = std::uint32_t;

/*! Domain reserved for the IpcMulticast component */
constexpr amsr::ipc::Domain kIpcMulticastDomain{76};

/*! Definition of the Multicast Daemon Adress */
constexpr amsr::ipc::UnicastAddress kIpcMulticastDaemonAddress{kIpcMulticastDomain, amsr::ipc::Port{132}};

/*!
 * \brief       Basic IPC multicast address.
 *
 * \details     Needs to be compatible to the osabstraction::ipc1::MulticastAddress type. However, it is not
 *              possible to link against osab::net here because osab::net cannot be used in an ASIL context.
 */
struct MulticastAddress {
  /*! Domain of the multicast address. */
  Domain domain;

  /*!  Group of the multicast address. */
  Group group;
};

/*!
 * \brief       Interface of the callback function that handles the message.
 *
 * \details     The multicast daemon client owns the memory, it is only guaranteed for the call of this callback
 *              function. If the message is used longer it must copied by the user.
 *
 * \param[in]   packet_view   The view to the packet data.
 *
 * \context     reactor
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 *
 * \vprivate    Vector product internal API
 */
using MessageHandleCallback = vac::language::UniqueFunction<void(PacketView const& packet_view)>;

}  // namespace ipcmulticast
}  // namespace amsr

#endif  // LIB_IPCMULTICAST_COMMON_INCLUDE_AMSR_IPCMULTICAST_TYPES_H_
