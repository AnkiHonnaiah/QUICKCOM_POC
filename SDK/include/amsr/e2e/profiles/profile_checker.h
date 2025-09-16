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
 *        \brief  Declaration of the ProfileChecker class.
 *
 *      \details  Contains the definitions for the ProfileChecker class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_CHECKER_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_CHECKER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "vac/testing/test_adapter.h"

#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/internal/profile_checker_interface.h"
#include "amsr/e2e/profiles/internal/profile_frame.h"
#include "amsr/e2e/state_machine/check_status.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Profile checker class.
 * \unit amsr::e2e::profiles::ProfileChecker
 * \vprivate Vector Internal API
 */
class ProfileChecker final : public internal::ProfileFrame, public internal::ProfileCheckerInterface {
 public:
  /*!
   * \brief Checker constructor.
   * \param[in] profile Profile to choose for the Profile Checker object to be created.
   * \param[in] props the E2E Event Protection Properties
   *
   *
   * The following constraints apply (proprietary profile has no constraints)
   * \code
   *
   * | Applicable Profiles        | Constraint                                                                        |
   * |----------------------------|-----------------------------------------------------------------------------------|
   * | All                        | props.kDataId <= std::numeric_limits<decltype(E2E_PXXConfigType::DataID)>::max()  |
   * | 1 [props.kDataIDMode == 3] | props.kDataId <= 4095                                                             |
   * | 1 [props.kDataIDMode != 3] | props.kDataId <= 65535                                                            |
   * | 4,5,6,7,22,44              | (props.kBitOffset % 8) == 0                                                       |
   * | 4,6,7,44                   | ((props.kBitOffset >> 3) + GetHeaderSize(Profile)) <= (props.kMinDataLength >> 3) |
   * | 5,22                       | ((props.kBitOffset >> 3) + GetHeaderSize(Profile)) <= (props.kDataLength >> 3)    |
   * | 4,6                        | props.kMinDataLength <= props.kMaxDataLength <= 32768                             |
   * | 7                          | props.kMinDataLength <= props.kMaxDataLength <= 33554432                          |
   * | 44                         | props.kMinDataLength <= props.kMaxDataLength <= 524280                            |
   * | 1,5,22                     | (props.kDataLength % 8) == 0                                                      |
   * | 1,22                       | props.kDataLength <= 256                                                          |
   * | 5                          | props.kDataLength <= 32768                                                        |
   * | 1                          | (props.kCounterOffset % 4) == 0                                                   |
   * | 1                          | props.kCounterOffset <= (props.kDataLength - 4)                                   |
   * | 1                          | (props.kCRCOffset % 8) == 0                                                       |
   * | 1                          | props.kCRCOffset <= (props.kDataLength - 8)                                       |
   * | 1 [props.kDataIDMode == 3] | (props.kDataIDNibbleOffset % 4) == 0                                              |
   * | 1 [props.kDataIDMode == 3] | props.kDataIDNibbleOffset <= (props.kDataLength - 4)                              |
   *
   *\endcode
   *
   * \param[in] max_delta_counter Maximum Delta Counter
   *
   * The following constraints apply (proprietary profile has no constraints):
   *
   * \code
   *
   * | Applicable Prof | Constraint                                                                                   |
   * |-----------------|----------------------------------------------------------------------------------------------|
   * | All             | max_delta_counter <=std::numeric_limits<decltype(E2E_PXXConfigType::MaxDeltaCounter)>::max() |
   * | 1               | max_delta_counter <= 14                                                                      |
   * | 22              | max_delta_counter <= 15                                                                      |
   *
   * \endcode
   *
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ProfileChecker(Profile profile, End2EndEventProtectionProps const& props, std::uint32_t max_delta_counter) noexcept;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~ProfileChecker() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  ProfileChecker() = delete;
  ProfileChecker(ProfileChecker const&) = delete;
  ProfileChecker(ProfileChecker&&) = delete;
  ProfileChecker& operator=(ProfileChecker const&) = delete;
  ProfileChecker& operator=(ProfileChecker&&) = delete;

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
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  CheckStatusType Check(ProfileFrame::ConstBufferView const& buffer) noexcept final;

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
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::e2e::state_machine::CheckStatus MapToCheckStatus(CheckStatusType check_status) const noexcept final;

 private:
  // Forward declarations because this header must not include any C header file
  class ProfileCheckerImplInterface;
  class StandardProfileCheckerImpl;
  class ProprietaryProfileCheckerImpl;

  /*!
   * \brief Pimpl Idiom used here because this header must not include any C header file.
   */
  std::unique_ptr<ProfileCheckerImplInterface> p_impl_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileChecker);
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_CHECKER_H_
