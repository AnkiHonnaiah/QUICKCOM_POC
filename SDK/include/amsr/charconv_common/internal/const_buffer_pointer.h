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
 *        \brief  Strong type for constant pointer to memory segment.
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_CONST_BUFFER_POINTER_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_CONST_BUFFER_POINTER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdlib>
#include <iterator>

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Strong type for constant pointer to memory segment.
 * \unit amsr::charconv::internal::ConstBufferPointer
 * \vprivate Component Private
 */
struct ConstBufferPointer {
  /*! Position in memory block. */
  char const* ptr{nullptr};
};

/*!
 * \brief Get value at location of constant buffer pointer.
 * \param[in] bp Buffer pointer.
 * \return Value at buffer location the pointer points to.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \vprivate Component Private
 * \steady UNKNOWN
 */
char operator*(ConstBufferPointer bp) noexcept;

/*!
 * \brief Advance constant buffer pointer.
 * \param[in, out] bp Buffer pointer.
 * \param[in] length Number of elements it should be advanced.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \vprivate Component Private
 * \steady UNKNOWN
 */
void StdAdvance(ConstBufferPointer& bp, std::ptrdiff_t const length) noexcept;

/*!
 * \brief Calculates the distance between two constant buffer pointers.
 * \param[in] bp1 First buffer pointer.
 * \param[in] bp2 Second buffer pointer.
 * \return Number of hops from first to second.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \vprivate Component Private
 * \steady UNKNOWN
 */
std::ptrdiff_t StdDistance(ConstBufferPointer bp1, ConstBufferPointer bp2) noexcept;

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_CONST_BUFFER_POINTER_H_
