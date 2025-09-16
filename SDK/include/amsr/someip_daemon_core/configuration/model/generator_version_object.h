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
/*!     \file     generator_version_object.h
 *      \brief    Configuration object for 'generatorVersion'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_GENERATOR_VERSION_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_GENERATOR_VERSION_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'generator version' object which is filled by a
 * parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class GeneratorVersionConfigObject {
 public:
  /*!
   * \brief Name element.
   */
  using NameElement = CfgElement<ara::core::String>;

  /*!
   * \brief Commit Id element.
   */
  using CommitIdElement = CfgElement<ara::core::String>;

  /*!
   * \brief Constructor sets the references to the element of the structure to write to.
   * \param[in] generator_version Reference to the generator version object to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit GeneratorVersionConfigObject(ConfigurationTypesAndDefs::GeneratorVersion& generator_version) noexcept
      : name_(generator_version.name), commit_id_(generator_version.commit_id) {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  virtual ~GeneratorVersionConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  GeneratorVersionConfigObject(GeneratorVersionConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  GeneratorVersionConfigObject(GeneratorVersionConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(GeneratorVersionConfigObject const&) & -> GeneratorVersionConfigObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(GeneratorVersionConfigObject&&) & -> GeneratorVersionConfigObject& = delete;

  /*!
   * \brief Set the Name when the parser finds the Name.
   * \param[in] name Name.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetName(ara::core::String const& name) noexcept { name_.SetElement(name); }

  /*!
   * \brief Getter for the Name element.
   * \return Reference to the Name of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  NameElement const& GetName() const noexcept { return name_; }

  /*!
   * \brief Set the Commit Id when the parser finds the Commit Id.
   * \param[in] commit_id The Commit Id.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetCommitId(ara::core::String const& commit_id) noexcept { commit_id_.SetElement(commit_id); }

  /*!
   * \brief Getter for the Commit Id element.
   * \return Reference to the Commit Id of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CommitIdElement const& GetCommitId() const noexcept { return commit_id_; }

 private:
  /*!
   * \brief The Address element.
   */
  NameElement name_;

  /*!
   * \brief The Mtu element.
   */
  CommitIdElement commit_id_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_GENERATOR_VERSION_OBJECT_H_
