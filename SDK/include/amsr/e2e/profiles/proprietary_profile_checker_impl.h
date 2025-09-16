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
 *        \brief  ProprietaryProfileCheckerImpl class for pimpl idiom.
 *
 *      \details  Contains the declarations for the ProprietaryProfileCheckerImpl class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROFILE_CHECKER_IMPL_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROFILE_CHECKER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/profiles/internal/profile_frame_impl.h"
#include "amsr/e2e/profiles/profile_checker.h"
#include "amsr/e2e/profiles/profile_checker_impl_interface.h"
#include "amsr/e2e/profiles/proprietary/proprietary_profile_checker.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Pimpl idiom class for Profile Checker that exists because E2E_P*.h shall not be included in profile_checker.h.
 * \unit amsr::e2e::profiles::ProfileChecker
 * \vprivate Vector component internal class
 */
class ProfileChecker::ProprietaryProfileCheckerImpl final : public ProfileChecker::ProfileCheckerImplInterface {
 public:
  /*!
   * \brief Checker impl constructor.
   * \param[in] profile_frame_p_impl        Pointer to ProfileFrameImpl instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit ProprietaryProfileCheckerImpl(
      internal::ProfileFrame::ProfileFrameImpl const* const profile_frame_p_impl) noexcept;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~ProprietaryProfileCheckerImpl() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  ProprietaryProfileCheckerImpl() = delete;
  ProprietaryProfileCheckerImpl(ProprietaryProfileCheckerImpl const&) = delete;
  ProprietaryProfileCheckerImpl(ProprietaryProfileCheckerImpl&&) = delete;
  ProprietaryProfileCheckerImpl& operator=(ProprietaryProfileCheckerImpl const&) = delete;
  ProprietaryProfileCheckerImpl& operator=(ProprietaryProfileCheckerImpl&&) = delete;

  /*!
   * \brief Check an incoming buffer on reception.
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_PXXCheck().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Component private method
   */
  CheckStatusType Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept final;

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
  amsr::e2e::state_machine::CheckStatus MapToCheckStatus(CheckStatusType check_status) const noexcept final;

 private:
  /*!
   * \brief Proprietary Profile checker instance.
   */
  proprietary::ProprietaryProfileChecker proprietary_profile_checker_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileChecker);
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROFILE_CHECKER_IMPL_H_
