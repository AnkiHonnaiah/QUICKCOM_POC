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
 *        \brief  Profile Frame Implementation class.
 *
 *      \details  Contains the declarations for the ProfileFrameImpl class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_FRAME_IMPL_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_FRAME_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
extern "C" {
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P01.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P04.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P05.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P06.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P07.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P22.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "E2E_P44.h"
}

// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "amsr/e2e/profiles/internal/profile_frame.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "amsr/e2e/profiles/proprietary/types.h"
// VECTOR Next Line AutosarC++17_10-M16.0.1: MD_E2E_M16.0.1
#include "amsr/e2e/profiles/end2end_event_protection_props.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace internal {

/*!
 * \brief Pimpl idiom class for Profile frame that exists because E2E_P*.h shall not be included in profile_frame.h.
 * \unit amsr::e2e::profiles::internal::ProfileFrame
 * \vprivate Vector component internal class
 */
class ProfileFrame::ProfileFrameImpl final {
 public:
  /*!
   * \brief Constructor for profile.
   * \param[in] profile Profile to choose for the Profile Checker object to be created.
   * \param[in] props the E2E Event Protection Properties
   * \param[in] max_delta_counter Maximum Delta Counter
   * \pre -
   * \context ANY
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  ProfileFrameImpl(Profile profile, End2EndEventProtectionProps const& props, std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Getter method for p01_config_ field.
   * \return Const reference to p01_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P01ConfigType const& GetP01Config() const noexcept;

  /*!
   * \brief Getter method for p04_config_ field.
   * \return Const reference to p04_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P04ConfigType const& GetP04Config() const noexcept;

  /*!
   * \brief Getter method for p05_config_ field.
   * \return Const reference to p05_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P05ConfigType const& GetP05Config() const noexcept;

  /*!
   * \brief Getter method for p06_config_ field.
   * \return Const reference to p06_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P06ConfigType const& GetP06Config() const noexcept;

  /*!
   * \brief Getter method for p07_config_ field.
   * \return Const reference to p07_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P07ConfigType const& GetP07Config() const noexcept;

  /*!
   * \brief Getter method for p22_config_ field.
   * \return Const reference to p22_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P22ConfigType const& GetP22Config() const noexcept;

  /*!
   * \brief Getter method for p44_config_ field.
   * \return Const reference to p44_config_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  E2E_P44ConfigType const& GetP44Config() const noexcept;

  /*!
   * \brief Getter method for profile_ field.
   * \return value of profile_.
   * \context ANY
   * \pre -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  Profile GetProfile() const noexcept;

  /*!
   * \brief Getter method for proprietary_profile_frame_ field.
   * \return value of proprietary_profile_frame_.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  ::amsr::e2e::profiles::proprietary::Frame const& GetProprietaryProfileFrame() const noexcept;

 private:
  /*!
   * \brief Profile 1-specific configuration.
   */
  E2E_P01ConfigType p01_config_;

  /*!
   * \brief Profile 4-specific configuration.
   */
  E2E_P04ConfigType p04_config_;

  /*!
   * \brief Profile 5-specific configuration.
   */
  E2E_P05ConfigType p05_config_;

  /*!
   * \brief Profile 6-specific configuration.
   */
  E2E_P06ConfigType p06_config_;

  /*!
   * \brief Profile 7-specific configuration.
   */
  E2E_P07ConfigType p07_config_;

  /*!
   * \brief Profile 22-specific configuration.
   */
  E2E_P22ConfigType p22_config_;

  /*!
   * \brief Profile 44-specific configuration.
   */
  E2E_P44ConfigType p44_config_;

  /*!
   * \brief Selected profile.
   */
  Profile profile_;

  /*!
   * \brief Proprietary profile frame.
   */
  ::amsr::e2e::profiles::proprietary::Frame proprietary_profile_frame_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileFrame);
};
}  // namespace internal
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_FRAME_IMPL_H_
