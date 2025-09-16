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
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief   Generator version.
 * \details Class storing the generator version. Stored configuration parameters:
 *          - Name: The name of generator that created the JSON file.
 *          - CommitId: The commit ID of generator that created the JSON file.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class GeneratorVersionConfig final {
 public:
  /*!
   * \brief Construct an empty generator version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GeneratorVersionConfig() noexcept = default;

  /*!
   * \brief Destroy generator version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~GeneratorVersionConfig() noexcept = default;

  /*!
   * \brief Copy-Construct a generator version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GeneratorVersionConfig(GeneratorVersionConfig const&) noexcept = default;

  /*!
   * \brief Copy-Assign a generator version configuration.
   *
   * \return Reference to the copy-assigned version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(GeneratorVersionConfig const&) & noexcept -> GeneratorVersionConfig& = default;

  GeneratorVersionConfig(GeneratorVersionConfig&&) noexcept = delete;
  auto operator=(GeneratorVersionConfig&&) noexcept -> GeneratorVersionConfig& = delete;

  /*!
   * \brief Get the generator name.
   *
   * \return Name of the generator.
   *
   * \pre       Config object was set by runtime configuration loader.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetName() const noexcept -> amsr::core::StringView;

  /*!
   * \brief Set the name of the generator.
   *
   * \param[in] generator_name  Name of the generator.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  void SetName(amsr::core::StringView const generator_name) noexcept;

  /*!
   * \brief Get the commit ID.
   *
   * \return Commit ID.
   *
   * \pre       Config object was set by runtime configuration loader.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetCommitId() const noexcept -> amsr::core::StringView;

  /*!
   * \brief Set the name of the generator.
   *
   * \param[in] commit_id  Commit ID of the generator.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  void SetCommitId(amsr::core::StringView const commit_id) noexcept;

 private:
  /*!
   * \brief Default for string values.
   */
  static constexpr ara::core::StringView kNoValueAvailable{"no_value_available"_sv};

  /*!
   * \brief Generator name.
   *
   * \steady FALSE
   */
  ::amsr::core::String generator_name_{kNoValueAvailable};

  /*!
   * \brief Generator commit ID.
   *
   * \steady FALSE
   */
  ::amsr::core::String generator_commit_id_{kNoValueAvailable};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_
