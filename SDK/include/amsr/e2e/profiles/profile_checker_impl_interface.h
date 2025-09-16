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
 *        \brief  ProfileCheckerImplInterface class for pimpl idiom.
 *
 *      \details  Contains the declarations for the ProfileCheckerImplInterface class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_CHECKER_IMPL_INTERFACE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_CHECKER_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/profiles/internal/profile_frame_impl.h"
#include "amsr/e2e/profiles/profile_checker.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Pimpl idiom interface class for Profile Checker that exists because E2E_P*.h shall not be included in
 *        profile_checker.h.
 * \vprivate Vector component internal class
 */
class ProfileChecker::ProfileCheckerImplInterface {
 public:
  /*!
   * \brief Constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  ProfileCheckerImplInterface() noexcept = default;

  /*!
   * \brief Default Destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProfileCheckerImplInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  ProfileCheckerImplInterface(ProfileCheckerImplInterface const&) = delete;
  ProfileCheckerImplInterface(ProfileCheckerImplInterface&&) = delete;
  ProfileCheckerImplInterface& operator=(ProfileCheckerImplInterface const&) = delete;
  ProfileCheckerImplInterface& operator=(ProfileCheckerImplInterface&&) = delete;

  /*!
   * \brief Check an incoming buffer on reception.
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_PXXCheck().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \vprivate Component private method
   */
  virtual CheckStatusType Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept = 0;

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
   * \vprivate Component private method
   */
  virtual amsr::e2e::state_machine::CheckStatus MapToCheckStatus(CheckStatusType check_status) const noexcept = 0;
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_CHECKER_IMPL_INTERFACE_H_
