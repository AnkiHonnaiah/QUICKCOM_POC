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
/*!
 *        \file  memory_buffer_ptr.h
 *        \brief Memory buffer pointer
 *        \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_PTR_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/unique_ptr.h"
#include "vac/memory/allocator/memory_buffer_interface.h"

namespace vac {
namespace memory {
namespace allocator {

/*!
 * \brief Alias for unique pointer to a MemoryBuffer.
 */
using MemoryBufferPtr = amsr::UniquePtr<MemoryBuffer>;

}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_PTR_H_
