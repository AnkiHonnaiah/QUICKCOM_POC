
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
/**        \file  skeleton_connection_state.h
 *        \brief  Defines an enum class to describe the state of the connection between proxy and skeleton.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_SKELETON_CONNECTION_STATE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_SKELETON_CONNECTION_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace socal {

/*!
 * \brief Connection status between proxy and skeleton.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_SkeletonConnectionState
enum class SkeletonConnectionState : std::uint8_t {
  kConnected = 0U,    // A connection has been established between proxy and skeleton.
  kDisconnected = 1U  // No connection between proxy and skeleton exists currently.
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_SKELETON_CONNECTION_STATE_H_
