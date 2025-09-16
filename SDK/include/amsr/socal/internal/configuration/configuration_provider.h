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
/*!        \file  socal/internal/configuration/configuration_provider.h
 *        \brief  JSON format specific configuration abstraction of the ara::com configuration.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_CONFIGURATION_PROVIDER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_CONFIGURATION_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/json/reader/json_data.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/configuration/parsing/socal_parser.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {

/*!
 * \brief Configuration provider.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
class ConfigurationProvider {
 public:
  /*!
   * \brief Constructor.
   * \param[in] json_data  JSON document to parse.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit ConfigurationProvider(::amsr::json::JsonData& json_data) noexcept;

  /*!
   * \brief Read configuration from component specific json file.
   * \return Result containing the configuration or an error.
   * \error ara::com::ComErrc::kJsonParsingFailed  If component specific JSON configuration parsing failed.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<Configuration> GetConfiguration() const noexcept;

 private:
  /*!
   * \brief Our logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ConfigurationProvider"_sv};

  /*!
   * \brief JSON document which is parsed by this class.
   */
  ::amsr::json::JsonData& json_data_;
};

}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_CONFIGURATION_PROVIDER_H_
