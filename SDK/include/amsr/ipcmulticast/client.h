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
/*!        \file
 *        \brief  Definition of the IPC multicast client.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPCMULTICAST_IPV6_INCLUDE_AMSR_IPCMULTICAST_CLIENT_H_
#define LIB_IPCMULTICAST_IPV6_INCLUDE_AMSR_IPCMULTICAST_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipcmulticast/ipv6_impl/ipv6_client.h"

namespace amsr {
namespace ipcmulticast {

/*! Alias selecting the daemon implementation.  */
using Client = amsr::ipcmulticast::ipv6_impl::Ipv6Client;

}  // namespace ipcmulticast
}  // namespace amsr

#endif  // LIB_IPCMULTICAST_IPV6_INCLUDE_AMSR_IPCMULTICAST_CLIENT_H_
