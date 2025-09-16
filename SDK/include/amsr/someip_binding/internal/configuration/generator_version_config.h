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
/*!        \file  someip_binding/internal/configuration/generator_version_config.h
 *        \brief  Generator version.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Generator version.
 * \details Class storing the generator version.
 *          Stored configuration parameters:
 *          - Name: The name of generator that created the JSON file.
 *          - CommitId: The commit ID of generator that created the JSON file.
 */
class GeneratorVersionConfig final {
 public:
  /*!
   * \brief   Construct a new generator version.
   *
   * \pre     -
   * \context Init
   * \spec
   *   requires true;
   * \endspec
   */
  GeneratorVersionConfig() noexcept;
  /*!
   * \brief Default copy constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  GeneratorVersionConfig(GeneratorVersionConfig const&) noexcept = default;
  /*!
   * \brief Default move constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  GeneratorVersionConfig(GeneratorVersionConfig&&) noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~GeneratorVersionConfig() = default;

  GeneratorVersionConfig& operator=(GeneratorVersionConfig const&) & = delete;
  GeneratorVersionConfig& operator=(GeneratorVersionConfig&&) & = delete;

  /*!
   * \brief     Get the generator name.
   * \return    name of the generator.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::StringView GetGeneratorName() const;

  /*!
   * \brief     Set the name of the generator.
   * \param[in] generator_name name of the generator.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetGeneratorName(amsr::core::StringView const generator_name) noexcept;

  /*!
   * \brief     Get the commit ID.
   * \return    commit ID.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::StringView GetCommitId() const;

  /*!
   * \brief     Set the name of the generator.
   * \param[in] commit_id commit ID of the generator.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetCommitId(amsr::core::StringView const commit_id) noexcept;

  /*!
   * \brief Default for string values.
   */
  static constexpr amsr::core::StringView kNoValueAvailable{"no_value_available"_sv};

 private:
  /*!
   * \brief Generator name.
   */
  amsr::core::String generator_name_;

  /*!
   * \brief Generator commit ID.
   */
  amsr::core::String generator_commit_id_;
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_GENERATOR_VERSION_CONFIG_H_
