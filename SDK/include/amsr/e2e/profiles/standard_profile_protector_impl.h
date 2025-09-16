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
 *        \brief  StandardProfileProtectorImpl class for pimpl idiom.
 *
 *      \details  Contains the declarations for the StandardProfileProtectorImpl class.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_STANDARD_PROFILE_PROTECTOR_IMPL_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_STANDARD_PROFILE_PROTECTOR_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/log/logging.h"

#include "amsr/core/string_view.h"
#include "amsr/e2e/profiles/internal/profile_frame_impl.h"
#include "amsr/e2e/profiles/profile_protector.h"
#include "amsr/e2e/profiles/profile_protector_impl_interface.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Pimpl idiom class for Profile Protector that exists because E2E_P*.h shall not be included in
 *        profile_protector.h.
 * \unit amsr::e2e::profiles::ProfileProtector
 * \vprivate Vector component internal class
 */
class ProfileProtector::StandardProfileProtectorImpl final : public ProfileProtector::ProfileProtectorImplInterface {
 public:
  /*!
   * \brief Protector impl constructor.
   * \param[in] profile_frame_p_impl Pointer to ProfileFrameImpl instance
   * \context ANY
   * \pre props.kDataId <= std::numeric_limits<decltype(E2E_PXXConfigType::DataID)>::max()
   * \pre props.kBitOffset <= std::numeric_limits<decltype(E2E_PXXConfigType::Offset)>::max()
   * \pre props.kDataLength <= std::numeric_limits<decltype(E2E_PXXConfigType::DataLength)>::max()
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
  explicit StandardProfileProtectorImpl(internal::ProfileFrame::ProfileFrameImpl* profile_frame_p_impl) noexcept;

  /*!
   * \brief Protector impl destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  ~StandardProfileProtectorImpl() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  StandardProfileProtectorImpl() = delete;
  StandardProfileProtectorImpl(StandardProfileProtectorImpl const&) = delete;
  StandardProfileProtectorImpl(StandardProfileProtectorImpl&&) = delete;
  StandardProfileProtectorImpl& operator=(StandardProfileProtectorImpl const&) = delete;
  StandardProfileProtectorImpl& operator=(StandardProfileProtectorImpl&&) = delete;

  /*!
   * \brief Protect routine for an E2E profile.
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return An empty result if successful.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_PXXProtect().
   * \threadsafe FALSE
   * \error E2eErrc::kWrongInput E2E_PXXProtect() detected a wrong input for buffer.
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   */
  ProtectReturnType Protect(ProfileFrame::BufferView& buffer) noexcept final;

 private:
  /*!
   * \brief This is a type definition of a pointer to a PXXProtect method.
   */
  using ProfileSpecificProtectMethod =
      Std_ReturnType (StandardProfileProtectorImpl::*)(internal::ProfileFrame::BufferView&);

  /*!
   * \brief Wrapper function for E2E_P01Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P01Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P01Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P04Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P04Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P04Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P05Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P05Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P05Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P06Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P06Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P06Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P07Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P07Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P07Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P22Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P22Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P22Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Wrapper function for E2E_P44Protect().
   * \param[in,out] buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values.
   * \return E2E_E_OK on success, E2E_E_INPUTERR_NULL or E2E_E_INPUTERR_WRONG on error.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_P44Protect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  Std_ReturnType P44Protect(internal::ProfileFrame::BufferView& buffer) noexcept;

  /*!
   * \brief Logs the given parameters related to data to be protected.
   * \param[in] function name of the calling function
   * \param[in] buffer the buffer to check
   * \param[in] data_id of the current profile config
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  void LogDataToProtect(::amsr::core::StringView function, ProfileFrame::ConstBufferView const& buffer,
                        uint32_t data_id) noexcept;

  /*!
   * \brief Logs the given parameters related to data protection result.
   * \param[in] function name of the calling function
   * \param[in] buffer the buffer to protect
   * \param[in] data_id of the current profile config
   * \param[in] counter new counter returned by the internal library
   * \param[in] ret_val protection result from internal library
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  void LogProtectResult(::amsr::core::StringView function, ProfileFrame::BufferView& buffer, uint32_t data_id,
                        uint32_t counter, Std_ReturnType ret_val) noexcept;

  /*!
   * \brief Logger instance.
   */
  ::ara::log::Logger& logger_;

  /*!
   * \brief Pointer to ProfileFrameImpl implementation instance.
   */
  internal::ProfileFrame::ProfileFrameImpl* profile_frame_p_impl_;

  /*!
   * \brief Current profile 01-specific protect state.
   */
  E2E_P01ProtectStateType p01_protect_state_;

  /*!
   * \brief Current profile 04-specific protect state.
   */
  E2E_P04ProtectStateType p04_protect_state_;

  /*!
   * \brief Current profile 05-specific protect state.
   */
  E2E_P05ProtectStateType p05_protect_state_;

  /*!
   * \brief Current profile 06-specific protect state.
   */
  E2E_P06ProtectStateType p06_protect_state_;

  /*!
   * \brief Current profile 07-specific protect state.
   */
  E2E_P07ProtectStateType p07_protect_state_;

  /*!
   * \brief Current profile 22-specific protect state.
   */
  E2E_P22ProtectStateType p22_protect_state_;

  /*!
   * \brief Current profile 44-specific protect state.
   */
  E2E_P44ProtectStateType p44_protect_state_;

  /*!
   * \brief The selected PXXProtect() method in dependency of the profile.
   */
  ProfileSpecificProtectMethod profile_specific_protect_method_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileProtector);
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_STANDARD_PROFILE_PROTECTOR_IMPL_H_
