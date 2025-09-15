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
 *        \brief  Common types for memory aware container abstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace memory {
namespace container {

/*!
 * \brief   Type for the shared, memory mapped queue index.
 */
struct QueueIndex {
  /*!
   * \brief   Value of the queue index.
   */
  std::atomic<std::uint32_t> value;
};

/*!
 * \brief   Type to count the number of queue slots.
 */
struct QueueSlots {
  /*!
   * \brief   Amount of the queue slots.
   */
  std::size_t amount;
};

}  // namespace container
}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_CONTAINER_TYPES_H_
