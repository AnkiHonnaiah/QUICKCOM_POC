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
 *        \brief  ProfileProtectorImplInterface class for pimpl idiom.
 *
 *      \details  Contains the declarations for the ProfileProtectorImplInterface class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_PROTECTOR_IMPL_INTERFACE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_PROTECTOR_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/profiles/internal/profile_frame_impl.h"
#include "amsr/e2e/profiles/profile_protector.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Pimpl idiom interface class for Profile Protector that exists because E2E_P*.h shall not be included in
 *        profile_protector.h.
 * \vprivate Vector component internal class
 */
class ProfileProtector::ProfileProtectorImplInterface {
 public:
  /*!
   * \brief Constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProfileProtectorImplInterface() noexcept = default;

  /*!
   * \brief Default Destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProfileProtectorImplInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  ProfileProtectorImplInterface(ProfileProtectorImplInterface const&) = delete;
  ProfileProtectorImplInterface(ProfileProtectorImplInterface&&) = delete;
  ProfileProtectorImplInterface& operator=(ProfileProtectorImplInterface const&) = delete;
  ProfileProtectorImplInterface& operator=(ProfileProtectorImplInterface&&) = delete;

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
   * \vprivate Component private method
   */
  virtual ProtectReturnType Protect(ProfileFrame::BufferView& buffer) noexcept = 0;
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_PROTECTOR_IMPL_INTERFACE_H_
