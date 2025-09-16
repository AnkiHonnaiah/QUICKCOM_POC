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
 *      \details  Contains the declarations for the ProfileCheckerImpl class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROFILE_PROTECTOR_IMPL_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROFILE_PROTECTOR_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/profiles/internal/profile_frame_impl.h"
#include "amsr/e2e/profiles/profile_protector.h"
#include "amsr/e2e/profiles/profile_protector_impl_interface.h"
#include "amsr/e2e/profiles/proprietary/proprietary_profile_protector.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Pimpl idiom class for Profile Protector that exists because E2E_P*.h shall not be included in
 *        profile_protector.h.
 * \unit amsr::e2e::profiles::ProfileProtector
 * \vprivate Vector component internal class
 */
class ProfileProtector::ProprietaryProfileProtectorImpl final : public ProfileProtector::ProfileProtectorImplInterface {
 public:
  /*!
   * \brief Protector impl constructor.
   * \param[in] profile_frame_p_impl        Pointer to ProfileFrameImpl instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit ProprietaryProfileProtectorImpl(
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
  ~ProprietaryProfileProtectorImpl() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  ProprietaryProfileProtectorImpl() = delete;
  ProprietaryProfileProtectorImpl(ProprietaryProfileProtectorImpl const&) = delete;
  ProprietaryProfileProtectorImpl(ProprietaryProfileProtectorImpl&&) = delete;
  ProprietaryProfileProtectorImpl& operator=(ProprietaryProfileProtectorImpl const&) = delete;
  ProprietaryProfileProtectorImpl& operator=(ProprietaryProfileProtectorImpl&&) = delete;

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
   * \brief Proprietary Profile protector instance.
   */
  proprietary::ProprietaryProfileProtector proprietary_profile_protector_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileProtector);
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROFILE_PROTECTOR_IMPL_H_
