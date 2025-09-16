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
 *        \brief  Interface for ProfileProtector.
 *
 *      \details  Contains the declarations for the ProfileProtectorInterface class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_PROTECTOR_INTERFACE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_PROTECTOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/result.h"

#include "amsr/e2e/profiles/internal/profile_frame.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace internal {

/*!
 * \brief Interface for Profile Protector used for testing purposes.
 * \vprivate Vector component internal base class
 */
class ProfileProtectorInterface {
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
  ProfileProtectorInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~ProfileProtectorInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  ProfileProtectorInterface(ProfileProtectorInterface const&) = delete;
  ProfileProtectorInterface(ProfileProtectorInterface&&) = delete;
  ProfileProtectorInterface& operator=(ProfileProtectorInterface const&) = delete;
  ProfileProtectorInterface& operator=(ProfileProtectorInterface&&) = delete;

  /*!
   * \brief Alias for the return type of the Protect() method.
   * \vprivate Vector Internal API
   */
  using ProtectReturnType = amsr::core::Result<void, amsr::core::ErrorCode>;

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
   */
  virtual ProtectReturnType Protect(ProfileFrame::BufferView& buffer) noexcept = 0;
};

}  // namespace internal
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_INTERNAL_PROFILE_PROTECTOR_INTERFACE_H_
