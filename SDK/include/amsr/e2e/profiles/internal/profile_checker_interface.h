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
 *        \brief  Interface for ProfileChecker.
 *
 *      \details  Contains the declarations for the ProfileCheckerInterface class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_CHECKER_INTERFACE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_CHECKER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/e2e/profiles/internal/profile_frame.h"
#include "amsr/e2e/state_machine/check_status.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace internal {

/*!
 * \brief Interface for Profile Checker used for testing purposes.
 * \vprivate Vector component internal base class
 */
class ProfileCheckerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProfileCheckerInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProfileCheckerInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  ProfileCheckerInterface(ProfileCheckerInterface const&) = delete;
  ProfileCheckerInterface(ProfileCheckerInterface&&) = delete;
  ProfileCheckerInterface& operator=(ProfileCheckerInterface const&) = delete;
  ProfileCheckerInterface& operator=(ProfileCheckerInterface&&) = delete;

  /*!
   * \brief Common check state type that holds a value of E2E_PXXCheckStatusType for XX = 01, 04, 06, 07, 22, 44.
   * Status of the received data in one cycle, protected with E2E Profile XX.
   *
   * \vprivate Vector Internal API
   */
  using CheckStatusType = std::uint8_t;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_E2E_M9.3.3
  /*!
   * \brief Check an incoming buffer on reception.
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification. Or E2E_P01STATUS_WRONGCRC instead of
   * E_SAFETY_HARD_RUNTIMEERROR, when buffer is smaller than data length in bytes for profile01 [SWS_E2EXf_00140].
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_PXXCheck().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector component internal pure virtual method
   */
  virtual CheckStatusType Check(ProfileFrame::ConstBufferView const& buffer) noexcept = 0;

  /*!
   * \brief Map profile-specific check status to the Generic State Machine Check status.
   * \details Wrapper function that calls E2E_PXXMapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector component internal pure virtual method
   */
  virtual amsr::e2e::state_machine::CheckStatus MapToCheckStatus(CheckStatusType check_status) const noexcept = 0;
};

}  // namespace internal
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_CHECKER_INTERFACE_H_
