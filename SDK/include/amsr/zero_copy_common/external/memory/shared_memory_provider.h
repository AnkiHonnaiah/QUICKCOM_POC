/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief Indirection of SharedMemoryProvider.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_MEMORY_SHARED_MEMORY_PROVIDER_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_MEMORY_SHARED_MEMORY_PROVIDER_H_

#include "amsr/memory/shared_memory_provider.h"

namespace amsr {
namespace zero_copy_common {
namespace external {
namespace memory {

/*!
 * \brief Indirection to access SharedMemoryProvider.
 */
using SharedMemoryProvider = ::amsr::memory::SharedMemoryProvider;

}  // namespace memory
}  // namespace external
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_MEMORY_SHARED_MEMORY_PROVIDER_H_
