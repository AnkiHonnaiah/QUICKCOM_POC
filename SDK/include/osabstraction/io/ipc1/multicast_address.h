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
/*!        \file  ipc1/multicast_address.h
 *        \brief  Defines a basic IPC multicast address.
 *         \unit  osabstraction::ipc1
 *   \complexity  ipc1 unit is QM, no action required.
 *
 *********************************************************************************************************************/

#ifndef LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_ADDRESS_H_
#define LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace osabstraction {
namespace io {
namespace ipc1 {

/*!
 * \brief Definition of Basic IPC multicast address, domain part.
 */
using Domain = std::uint32_t;

/*!
 * \brief Definition Basic IPC multicast address, group part.
 */
using Group = std::uint32_t;

/*!
 * \brief Basic IPC multicast address.
 */
struct MulticastAddress {
  // VECTOR Next Construct VectorC++-V11.0.2: MD_OSA_V11.0.2_struct
  /*!
   * \brief Domain of the multicast address.
   */
  Domain domain;

  // VECTOR Next Construct VectorC++-V11.0.2: MD_OSA_V11.0.2_struct
  /*!
   * \brief Group of the multicast address.
   */
  Group group;
};

}  // namespace ipc1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_NET_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_IPC1_MULTICAST_ADDRESS_H_
