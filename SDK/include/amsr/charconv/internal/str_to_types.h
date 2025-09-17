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
 *        \brief  Defines types for strtoll and strtoull
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_STR_TO_TYPES_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_STR_TO_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace charconv {
namespace internal {

static constexpr std::uint8_t kNbDigitsOfLimitMinSignedLLDecimalNumber{19};
static constexpr std::uint8_t kNbDigitsOfLimitMaxUnsignedLLDecimalNumber{20};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_INTERNAL_STR_TO_TYPES_H_
