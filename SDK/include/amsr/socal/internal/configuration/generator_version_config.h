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
/*!        \file  socal/internal/configuration/generator_version_config.h
 *        \brief  Generator version.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief   Generator version.
 * \details Class storing the generator version. Stored configuration parameters:
 *          - Name: The name of generator that created the JSON file.
 *          - CommitId: The commit ID of generator that created the JSON file.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
class GeneratorVersionConfig final {
 public:
  /*!
   * \brief Construct a new generator version.
   * \pre     -
   * \context Init
   * \spec requires true; \endspec
   * \steady FALSE
   */
  GeneratorVersionConfig() noexcept;

  GeneratorVersionConfig(GeneratorVersionConfig const&) noexcept = delete;
  GeneratorVersionConfig(GeneratorVersionConfig&& other) noexcept = delete;
  GeneratorVersionConfig& operator=(GeneratorVersionConfig const&) & = delete;
  GeneratorVersionConfig& operator=(GeneratorVersionConfig&&) & = delete;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ~GeneratorVersionConfig() noexcept = default;

  /*!
   * \brief Get the generator name.
   * \return name of the generator.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  std::string GetGeneratorName() const;

  /*!
   * \brief Set the name of the generator.
   * \param[in] generator_name  Name of the generator.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void SetGeneratorName(std::string const& generator_name);

  /*!
   * \brief Get the commit ID.
   * \return commit ID.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  std::string GetCommitId() const;

  /*!
   * \brief Set the name of the generator.
   * \param[in] commit_id  Commit ID of the generator.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void SetCommitId(std::string const& commit_id);

  /*!
   * \brief Default for string values.
   */
  static constexpr ::amsr::core::StringView kNoValueAvailable{"no_value_available"_sv};

 private:
  /*!
   * \brief Generator name.
   */
  std::string generator_name_;

  /*!
   * \brief Generator commit ID.
   */
  std::string generator_commit_id_;
};

}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_
