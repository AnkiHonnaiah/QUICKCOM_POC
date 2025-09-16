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
/*!        \file  method_config.h
 *        \brief  Method configuration class
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_METHOD_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_METHOD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Method deployment configuration.
 * \details Configuration class storing the configuration parameters of a Method deployment.
 *          Stored configuration parameters:
 *          - MethodId
 *          - shortname
 *          - isFireAndForget
 */
class MethodConfig final {
 public:
  /*! Name alias for method identifier type. */
  using MethodId = ::amsr::someip_protocol::internal::MethodId;
  /*! Name alias for Shortname type. */
  using Shortname = ::amsr::someip_binding::internal::configuration::Shortname;
  /*! Name alias for IsFireAndForget type. */
  using IsFireAndForget = bool;

  /*!
   * \brief Set the id of the Method.
   * \details The id defines the id of the Method.
   * \param[in] id The Method id.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMethodId(MethodId const id) noexcept;

  /*!
   * \brief Get the id of the Method.
   * \return The id of the Method.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MethodId GetMethodId() const noexcept;

  /*!
   * \brief  Gets the shortname path.
   * \return The shortname path.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Shortname const& GetShortname() const noexcept;

  /*!
   * \brief Sets the shortname.
   * \param shortname The shortname.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetShortname(Shortname const& shortname) noexcept;

  /*!
   * \brief  Gets the isFireAndForget flag.
   * \return The isFireAndForget flag.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  IsFireAndForget GetIsFireAndForget() const noexcept;

  /*!
   * \brief Sets isFireAndForget flag.
   * \param is_fire_and_forget The isFireAndForget flag.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetIsFireAndForget(IsFireAndForget const is_fire_and_forget) noexcept;

 private:
  MethodId method_id_{};                 /*!< The Method identifier. */
  MethodConfig::Shortname shortname_{};  /*!< The Short name  */
  IsFireAndForget is_fire_and_forget_{}; /*!< The IsFireAndForget flag  */
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_METHOD_CONFIG_H_
