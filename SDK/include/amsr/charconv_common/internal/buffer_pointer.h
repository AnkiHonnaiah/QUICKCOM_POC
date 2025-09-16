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
 *        \brief  Strong type for pointer to memory segment.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BUFFER_POINTER_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BUFFER_POINTER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Strong type for pointer to memory segment.
 * \unit amsr::charconv::internal::BufferPointer
 * \vprivate Component Private
 */
struct BufferPointer {
  /*! Position in memory block. */
  char* ptr{nullptr};
};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BUFFER_POINTER_H_
