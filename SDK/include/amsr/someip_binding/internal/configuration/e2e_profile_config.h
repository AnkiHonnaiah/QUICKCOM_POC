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
/*!        \file  e2e_profile_config.h
 *        \brief  Configuration for the E2EProfile.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_E2E_PROFILE_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_E2E_PROFILE_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/e2e/profiles/e2e_profile_configuration.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

// VECTOR Next Construct Metric-OO.WMC.One: MD_SOMEIPBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief E2EProfile configuration.
 * \details Configuration class storing the configuration parameters for the E2EProfile.
 *          Stored configuration parameters:
 *          - Shortname
 *          - ProfileName
 *          - MaxDeltaCounter
 *          - MinOkStateInit
 *          - MaxErrorStateInit
 *          - MinOkStateValid
 *          - MaxErrorStateValid
 *          - MinOkStateInvalid
 *          - MaxErrorStateInvalid
 *          - WindowSizeValid
 *          - WindowSizeInit
 *          - WindowSizeInvalid
 *          - ClearFromValidToInvalid
 *          - TransiToInvalidExtended
 */
class E2EProfileConfig final {
 public:
  /*! Type-alias for reference of E2EConfig.  */
  using E2EProfileConfigRef = std::reference_wrapper<E2EProfileConfig const>;

  /*! Type-alias for the E2E profile shortname. */
  using E2EShortname = ::amsr::someip_binding::internal::configuration::Shortname;
  /*! Type-alias for the E2E profile shortname. */
  using E2EProfile = amsr::e2e::profiles::Profile;

  /*! Type-alias for Max Delta Counter */
  using MaxDeltaCounter = std::uint32_t;
  /*! Type-alias for Min Ok State Init */
  using MinOkStateInit = std::uint8_t;
  /*! Type-alias for Max Error State Init */
  using MaxErrorStateInit = std::uint8_t;
  /*! Type-alias for Min Ok State Valid */
  using MinOkStateValid = std::uint8_t;
  /*! Type-alias for Max Error State Valid */
  using MaxErrorStateValid = std::uint8_t;
  /*! Type-alias for Min Ok State Invalid */
  using MinOkStateInvalid = std::uint8_t;
  /*! Type-alias for Max Error State Invalid */
  using MaxErrorStateInvalid = std::uint8_t;
  /*! Type-alias for Window Size Valid */
  using WindowSizeValid = std::uint8_t;
  /*! Type-alias for Window Size Init */
  using WindowSizeInit = std::uint8_t;
  /*! Type-alias for Window Size Invalid */
  using WindowSizeInvalid = std::uint8_t;
  /*! Type-alias for clear from valid to invalid */
  using ClearToInvalid = bool;
  /*! Type-alias for transit to invalid extended */
  using TransitToInvalidExtended = bool;

  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  E2EProfileConfig() noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  ~E2EProfileConfig() noexcept = default;

  /*!
   * \brief Default copy constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
  E2EProfileConfig(E2EProfileConfig const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  E2EProfileConfig(E2EProfileConfig&&) noexcept = default;

  E2EProfileConfig& operator=(E2EProfileConfig const&) & = delete;

  /*!
   * \brief Default move assignment.
   * \return Reference to the move-constructed E2EProfileConfig.
   * \spec
   *   requires true;
   * \endspec
   */
  E2EProfileConfig& operator=(E2EProfileConfig&&) & noexcept = default;

  /*!
   * \brief Set shortname
   * \param[in] e2e_shortname The shortname
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetE2EShortname(E2EShortname const& e2e_shortname) noexcept;

  /*!
   * \brief Get the shortname of e2e profile
   * \return The shortname of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  E2EShortname GetE2EShortname() const noexcept;

  /*!
   * \brief Set profile
   * \param[in] e2e_profile the profile
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetE2EProfile(E2EProfile const& e2e_profile) noexcept;

  /*!
   * \brief Get the profilename of e2e profile
   * \return The profilename of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  E2EProfile GetE2EProfile() const noexcept;

  /*!
   * \brief Set max delta counter of e2e profile
   * \param[in] value the max delta counter value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMaxDeltaCounter(MaxDeltaCounter value) noexcept;

  /*!
   * \brief Get the max delta counter of e2e profile
   * \return The max delta counter of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MaxDeltaCounter GetMaxDeltaCounter() const noexcept;

  /*!
   * \brief Set min ok state init of e2e profile
   * \param[in] value the min ok state init value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMinOkStateInit(MinOkStateInit value) noexcept;

  /*!
   * \brief Get the min ok state init of e2e profile
   * \return The min ok state init of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MinOkStateInit GetMinOkStateInit() const noexcept;

  /*!
   * \brief Set max error state init of e2e profile
   * \param[in] value the max error state init value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMaxErrorStateInit(MaxErrorStateInit value) noexcept;

  /*!
   * \brief Get the max error state init of e2e profile
   * \return The max error state init of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MaxErrorStateInit GetMaxErrorStateInit() const noexcept;

  /*!
   * \brief Set min ok state valid of e2e profile
   * \param[in] value the min ok state valid value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMinOkStateValid(MinOkStateValid value) noexcept;

  /*!
   * \brief Get the min ok state valid of e2e profile
   * \return The min ok state valid of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MinOkStateValid GetMinOkStateValid() const noexcept;

  /*!
   * \brief Set max error state valid of e2e profile
   * \param[in] value the max error state valid value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMaxErrorStateValid(MaxErrorStateValid value) noexcept;

  /*!
   * \brief Get the max error state valid of e2e profile
   * \return The max error state valid of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MaxErrorStateValid GetMaxErrorStateValid() const noexcept;

  /*!
   * \brief Set min ok state invalid of e2e profile
   * \param[in] value the min ok state invalid value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMinOkStateInvalid(MinOkStateInvalid value) noexcept;

  /*!
   * \brief Get the min ok state invalid of e2e profile
   * \return The min ok state invalid of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MinOkStateInvalid GetMinOkStateInvalid() const noexcept;

  /*!
   * \brief Get the max error state invalid of e2e profile
   * \return The max error state invalid of e2e profile
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MaxErrorStateInvalid GetMaxErrorStateInvalid() const noexcept;

  /*!
   * \brief Set max error state invalid of e2e profile
   * \param[in] value the max error state invalid value
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetMaxErrorStateInvalid(MaxErrorStateInvalid value) noexcept;

  /*!
   * \brief Get the window size of e2e profile for the valid state
   * \return The window size of e2e profile for the valid state
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  WindowSizeValid GetWindowSizeValid() const noexcept;

  /*!
   * \brief Set window size of e2e profile for the valid state
   * \param[in] value the window size value for the valid state
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetWindowSizeValid(WindowSizeValid value) noexcept;

  /*!
   * \brief Get the window size of e2e profile for the init state
   * \return The window size of e2e profile for the init state
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  WindowSizeInit GetWindowSizeInit() const noexcept;

  /*!
   * \brief Set window size of e2e profile for the init state
   * \param[in] value the window size value for the init state
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetWindowSizeInit(WindowSizeInit value) noexcept;

  /*!
   * \brief Get the window size of e2e profile for the invalid state
   * \return The window size of e2e profile for the invalid state
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  WindowSizeInvalid GetWindowSizeInvalid() const noexcept;

  /*!
   * \brief Set window size of e2e profile for the invalid state
   * \param[in] value the window size value for the invalid state
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetWindowSizeInvalid(WindowSizeInvalid value) noexcept;

  /*!
   * \brief Get clear from valid to invalid.
   * \return The clear from valid to invalid
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ClearToInvalid GetClearToInvalid() const noexcept;

  /*!
   * \brief Set clear from valid to invalid.
   * \param[in] value the clear from valid to invalid
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetClearToInvalid(ClearToInvalid value) noexcept;

  /*!
   * \brief Get transit to invalid extended.
   * \return The transit to invalid extended
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  TransitToInvalidExtended GetTransitToInvalidExtended() const noexcept;

  /*!
   * \brief Set transit to invalid extended.
   * \param[in] value the transit to invalid extended
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetTransitToInvalidExtended(TransitToInvalidExtended value) noexcept;

  /*!
   * \brief Get the E2E profile configuration
   * \return The E2E profile configuration
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::e2e::profiles::E2EProfileConfiguration GetE2EProfileConfiguration() const noexcept;

 private:
  E2EShortname e2e_shortname_{};                                /*!< The e2e shortname */
  E2EProfile e2e_profile_{};                                    /*!< The e2e profile */
  MaxDeltaCounter max_delta_counter_{0};                        /*!< The max delta counter */
  MinOkStateInit min_ok_state_init_{0};                         /*!< The min ok state init */
  MaxErrorStateInit max_error_state_init_{0};                   /*!< The max error state init */
  MinOkStateValid min_ok_state_valid_{0};                       /*!< The min ok state valid */
  MaxErrorStateValid max_error_state_valid_{0};                 /*!< The error state valid */
  MinOkStateInvalid min_ok_state_invalid_{0};                   /*!< The min ok state invalid */
  MaxErrorStateInvalid max_error_state_invalid_{0};             /*!< The max error state invalid */
  WindowSizeValid window_size_valid_{0};                        /*!< The window size state valid */
  WindowSizeInit window_size_init_{0};                          /*!< The window size state init */
  WindowSizeInvalid window_size_invalid_{0};                    /*!< The window size state invalid */
  ClearToInvalid clear_to_invalid_{false};                      /*!< The clear from valid to invalid */
  TransitToInvalidExtended transit_to_invalid_extended_{false}; /*!< The transit to invalid extended */
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_E2E_PROFILE_CONFIG_H_
