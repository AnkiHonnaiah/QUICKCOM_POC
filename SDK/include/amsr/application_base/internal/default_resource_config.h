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
/**     \file       default_resource_config.h
 *      \brief      Utility for configuring the default resource.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_DEFAULT_RESOURCE_CONFIG_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_DEFAULT_RESOURCE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include "envvar_parser.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace application_base {
namespace internal {
/*!
 * \brief Configuration of a BSW MEX Default Resource.
 */
struct DefaultResourceConfig {
  /*!
   * \brief The configured size.
   */
  std::size_t size;

  /*!
   * \brief Flag for expanding.
   */
  bool expanding;
};

/*!
 * \brief Parse the default resource configuration from the environment variables.
 */
inline auto ParseDefaultResourceConfig() noexcept -> Result<DefaultResourceConfig> {
  // VECTOR NL AutosarC++17_10-M7.3.6: MD_APP_BASE_AutosarC++17_10-M7.3.6_using_literal
  using vac::container::literals::operator""_sv;

  return EnvvarParser::ParsePositiveNumber("AMSR_DEFAULT_RESOURCE_SIZE"_sv)
      .AndThen([](EnvvarParser::PositiveNumber size) {
        return EnvvarParser::ParseBool("AMSR_DEFAULT_RESOURCE_EXPANDING"_sv).Map([&size](bool expanding) {
          return DefaultResourceConfig{static_cast<std::size_t>(size), expanding};
        });
      });
}

}  // namespace internal
}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_DEFAULT_RESOURCE_CONFIG_H_