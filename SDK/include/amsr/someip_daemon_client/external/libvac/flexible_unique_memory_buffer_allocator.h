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
/*!        \file    flexible_unique_memory_buffer_allocator.h
 *        \brief    Indirection to access FlexibleUniqueMemoryBufferAllocator functions.
 *      \details    Indirections facilitate mocking of external libraries / generated source files in test suite.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_SOMEIP_DAEMON_CLIENT_EXTERNAL_LIBVAC_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_SOMEIP_DAEMON_CLIENT_EXTERNAL_LIBVAC_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"

namespace amsr {
namespace someip_daemon_client {
namespace external {
namespace libvac {

/*!
 * \brief Indirection to access FlexibleUniqueMemoryBufferAllocator.
 */
template <typename Alloc>
using FlexibleUniqueMemoryBufferAllocator =
    vac::memory::allocator::flexible::FlexibleUniqueMemoryBufferAllocator<Alloc>;

}  // namespace libvac
}  // namespace external
}  // namespace someip_daemon_client
}  // namespace amsr
#endif  // LIB_EXTERNAL_INCLUDE_AMSR_SOMEIP_DAEMON_CLIENT_EXTERNAL_LIBVAC_FLEXIBLE_UNIQUE_MEMORY_BUFFER_ALLOCATOR_H_
