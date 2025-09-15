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
 *        \brief  Internal utilities for memory abstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_ALIGNMENT_UTILITIES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_ALIGNMENT_UTILITIES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "amsr/memory/memory_types.h"

namespace amsr {
namespace memory {
namespace internal {

/*!
 * \brief       Calculates alignment of address.
 *
 * \param[in]   address  Address for which to calculate the alignment. Must not be nullptr.
 *
 * \return      Largest power-of-2-alignment of the address.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 */
auto CalculateAlignment(void const* address) noexcept -> Alignment;

/*!
 * \brief       Asserts that the passed alignment is a power of 2.
 *
 * \param[in]   alignment  Alignment value to verify. Execution is aborted if this value is not a power of 2.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  TRUE
 *
 * \spec
 *   requires true;
 * \endspec
 */
void AssertAlignmentValid(Alignment alignment) noexcept;

}  // namespace internal
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_INTERNAL_ALIGNMENT_UTILITIES_H_
