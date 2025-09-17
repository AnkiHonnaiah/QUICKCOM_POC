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
 *        \brief  StandardProfileCheckerImpl class for pimpl idiom.
 *
 *      \details  Contains the declarations for the StandardProfileCheckerImpl class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_STANDARD_PROFILE_CHECKER_IMPL_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_STANDARD_PROFILE_CHECKER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logger.h"

#include "amsr/core/string_view.h"
#include "amsr/e2e/profiles/internal/profile_frame_impl.h"
#include "amsr/e2e/profiles/profile_checker.h"
#include "amsr/e2e/profiles/profile_checker_impl_interface.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Pimpl idiom class for Profile Checker that exists because E2E_P*.h shall not be included in profile_checker.h.
 * \unit amsr::e2e::profiles::ProfileChecker
 * \vprivate Vector component internal class
 */
class ProfileChecker::StandardProfileCheckerImpl final : public ProfileChecker::ProfileCheckerImplInterface {
 public:
  /*!
   * \brief Checker impl constructor.
   * \param[in] profile_frame_p_impl        Pointer to ProfileFrameImpl instance
   * \context ANY
   * \pre props.kDataId <= std::numeric_limits<decltype(E2E_PXXConfigType::DataID)>::max()
   * \pre props.kBitOffset <= std::numeric_limits<decltype(E2E_PXXConfigType::Offset)>::max()
   * \pre props.kMinDataLength <= std::numeric_limits<decltype(E2E_PXXConfigType::MinDataLength)>::max()
   * \pre props.kMaxDataLength <= std::numeric_limits<decltype(E2E_PXXConfigType::MaxDataLength)>::max()
   * \pre max_delta_counter    <= std::numeric_limits<decltype(E2E_PXXConfigType::MaxDeltaCounter)>::max()
   * \pre props.kMinDataLength <= props.kMaxDataLength
   * \pre ((props.kBitOffset >> 3) + GetHeaderSize()) <= (props.kMinDataLength >> 3)
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  explicit StandardProfileCheckerImpl(internal::ProfileFrame::ProfileFrameImpl* profile_frame_p_impl) noexcept;

  /*!
   * \brief Checker impl destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  ~StandardProfileCheckerImpl() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  StandardProfileCheckerImpl() = delete;
  StandardProfileCheckerImpl(StandardProfileCheckerImpl const&) = delete;
  StandardProfileCheckerImpl(StandardProfileCheckerImpl&&) = delete;
  StandardProfileCheckerImpl& operator=(StandardProfileCheckerImpl const&) = delete;
  StandardProfileCheckerImpl& operator=(StandardProfileCheckerImpl&&) = delete;

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
   * \brief This is a type definition of a pointer to a PXXProtect method.
   */
  using ProfileSpecificCheckMethod =
      CheckStatusType (ProfileChecker::StandardProfileCheckerImpl::*)(internal::ProfileFrame::ConstBufferView const&);

  /*!
   * \brief This is a type definition of a pointer to a PXXMapToCheckStatus method.
   */
  using ProfileSpecificMapToCheckStatusMethod = amsr::e2e::state_machine::CheckStatus (*)(CheckStatusType);

  /*!
   * \brief Wrapper function for E2E_P01Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification or E2E_P01STATUS_WRONGCRC instead of
   * E_SAFETY_HARD_RUNTIMEERROR, when buffer is smaller than data length in bytes [SWS_E2EXf_00140].
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P01Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P01Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P04Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P04Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P04Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P05Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P05Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P05Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P06Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P06Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P06Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P07Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P07Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P07Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P22Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P22Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P22Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P44Check()
   * \param[in] buffer the buffer to check.
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P44Check().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatusType P44Check(internal::ProfileFrame::ConstBufferView const& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P01MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P01MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Wrapper function for E2E_P04MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P04MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Wrapper function for E2E_P05MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P05MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Wrapper function for E2E_P06MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P06MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Wrapper function for E2E_P07MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P07MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Wrapper function for E2E_P22MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P22MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Wrapper function for E2E_P44MapStatusToSM().
   * \param[in] check_status Profile-specific check status to convert
   * \return Generic State Machine check status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static amsr::e2e::state_machine::CheckStatus P44MapToCheckStatus(CheckStatusType check_status) noexcept;

  /*!
   * \brief Logs the check status result of an e2e check.
   * \param[in] function name of the calling function
   * \param[in] buffer the buffer to check
   * \param[in] result returned by the internal library
   * \param[in] data_id configured Data ID
   * \param[in] check_status Profile-specific check status to convert
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  void LogCheckResult(::amsr::core::StringView function, ProfileFrame::ConstBufferView const& buffer,
                      Std_ReturnType result, uint32 data_id, CheckStatusType check_status) noexcept;

  /*!
   * \brief Logger instance.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief Pointer to ProfileFrameImpl implementation instance.
   */
  internal::ProfileFrame::ProfileFrameImpl* profile_frame_p_impl_;

  /*!
   * \brief Current profile-1 specific check state.
   */
  E2E_P01CheckStateType p01_check_state_;

  /*!
   * \brief Current profile-4 specific check state.
   */
  E2E_P04CheckStateType p04_check_state_;

  /*!
   * \brief Current profile-5 specific check state.
   */
  E2E_P05CheckStateType p05_check_state_;

  /*!
   * \brief Current profile-6 specific check state.
   */
  E2E_P06CheckStateType p06_check_state_;

  /*!
   * \brief Current profile-7 specific check state.
   */
  E2E_P07CheckStateType p07_check_state_;

  /*!
   * \brief Current profile-22 specific check state.
   */
  E2E_P22CheckStateType p22_check_state_;

  /*!
   * \brief Current profile-44 specific check state.
   */
  E2E_P44CheckStateType p44_check_state_;

  /*!
   * \brief The selected PXXCheck() method in dependency of the profile.
   */
  ProfileSpecificCheckMethod profile_specific_check_method_;

  /*!
   * \brief The selected PXXMapToCheckStatus() method in dependency of the profile.
   */
  ProfileSpecificMapToCheckStatusMethod profile_specific_map_to_check_status_method_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileChecker);
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_STANDARD_PROFILE_CHECKER_IMPL_H_
