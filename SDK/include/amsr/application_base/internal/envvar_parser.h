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
/**     \file       envvar_parser.h
 *      \brief      Functionality for parsing environment variables.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_ENVVAR_PARSER_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_ENVVAR_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/application_base/initialization_error_domain.h"
#include "amsr/application_base/internal/utils.h"
#include "amsr/charconv/to_number_unsigned.h"
#include "amsr/envvar/envvar.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace application_base {
namespace internal {
/*!
 * \brief Parse environment variables.
 */
class EnvvarParser final {
 public:
  /*!
   * \brief The type to parse as a positive integer.
   */
  using PositiveNumber = std::size_t;

  /*!
   * \brief Parse boolean environment variable.
   */
  static auto ParseBool(vac::container::CStringView envvar_name) noexcept -> Result<bool> {
    return envvar::GetEnvironmentVariable(envvar_name).Map(ParseBoolValue);
  }

  /*!
   * \brief Parse positive number environment variable.
   */
  static auto ParsePositiveNumber(vac::container::CStringView envvar_name) noexcept -> Result<PositiveNumber> {
    return envvar::GetEnvironmentVariable(envvar_name).AndThen(ParseUnsignedValue);
  }

 private:
  /*!
   * \brief Parse boolean value.
   */
  static auto ParseBoolValue(vac::container::CStringView envvar_value) -> bool {
    using vac::container::literals::operator""_sv;

    return envvar_value == "1"_sv || envvar_value == "true"_sv;
  }

  /*!
   * \brief Parse unsigned integer value.
   */
  static auto ParseUnsignedValue(vac::container::CStringView envvar_value) -> Result<PositiveNumber> {
    charconv::ToNumber<PositiveNumber> to_number{envvar_value};
    return to_number.Convert();
  }
};
}  // namespace internal
}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_ENVVAR_PARSER_H_