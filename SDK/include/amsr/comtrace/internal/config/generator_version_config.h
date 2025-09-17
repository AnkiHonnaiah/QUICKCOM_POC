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
 *        \brief  Generator version.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_GENERATOR_VERSION_CONFIG_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_GENERATOR_VERSION_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/generator_version_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_ComTrace_M7.3.6_StringView_literal
/*!
 * \brief Use string literal for creation of StringViews.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief   Generator version.
 * \details Class storing the generator version. Stored configuration parameters:
 *          - Name: The name of generator that created the JSON file.
 *          - CommitId: The commit ID of generator that created the JSON file.
 *
 * \unit ComTrace::TraceConfig
 */
class GeneratorVersionConfig final : public GeneratorVersionConfigInterface {
 public:
  /*!
   * \brief Construct an empty generator version configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GeneratorVersionConfig() noexcept = default;

  /*!
   * \brief Copy-Construct a generator version configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GeneratorVersionConfig(GeneratorVersionConfig const&) noexcept = default;

  auto operator=(GeneratorVersionConfig const&) & noexcept -> GeneratorVersionConfig& = delete;
  GeneratorVersionConfig(GeneratorVersionConfig&&) noexcept = delete;
  auto operator=(GeneratorVersionConfig&&) noexcept -> GeneratorVersionConfig& = delete;

  /*!
   * \brief Destroy generator version configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~GeneratorVersionConfig() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \copydoc amsr::comtrace::internal::config::GeneratorVersionConfigInterface::GetName()
   *
   * \spec requires true; \endspec
   */
  auto GetName() const noexcept -> StringView final;

  /*!
   * \brief Set the name of the generator.
   *
   * \param[in] generator_name  Name of the generator.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetName(StringView generator_name) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::GeneratorVersionConfigInterface::GetCommitId()
   *
   * \spec requires true; \endspec
   */
  auto GetCommitId() const noexcept -> StringView final;

  /*!
   * \brief Set the name of the generator.
   *
   * \param[in] commit_id  Commit ID of the generator.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetCommitId(StringView commit_id) noexcept -> void;

 private:
  /*!
   * \brief Default for string values.
   */
  static constexpr StringView kNoValueAvailable{"no_value_available"_sv};

  /*!
   * \brief Generator name.
   */
  ::amsr::core::String generator_name_{kNoValueAvailable};

  /*!
   * \brief Generator commit ID.
   */
  ::amsr::core::String generator_commit_id_{kNoValueAvailable};
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_GENERATOR_VERSION_CONFIG_H_
