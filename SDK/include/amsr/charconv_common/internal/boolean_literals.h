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
 *        \brief  Provides character sequences for boolean variables.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BOOLEAN_LITERALS_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BOOLEAN_LITERALS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Provides character sequences for boolean variables.
 * \unit amsr::charconv::internal::BooleanLiterals
 * \vprivate Component Private
 */
class BooleanLiterals {
 public:
  /*! Size of the true literal. */
  static constexpr std::uint8_t const kTrueLiteralSize{4};

  /*!
   * \brief Get array of character sequence "true"
   * \return Pointers to begin and end of literal "true"
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  static amsr::core::StringView GetTrueLiteral() noexcept;

  /*! Size of the false literal. */
  static constexpr std::uint8_t const kFalseLiteralSize{5};

  /*!
   * \brief Get string view of character sequence "false"
   * \return String view of literal "false"
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  static amsr::core::StringView GetFalseLiteral() noexcept;
};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BOOLEAN_LITERALS_H_
