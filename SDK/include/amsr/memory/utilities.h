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
 *        \brief   Utilities for memory abstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_UTILITIES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/memory/memory_types.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Returns the page size of the system.
 *
 * \return      Page size (in bytes) of the system.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 * \steady      TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate    Vector product internal API
 * \trace       DSGN-Osab-MemoryGetPageSize
 */
auto GetPageSize() noexcept -> SizeBytes;

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_UTILITIES_H_
