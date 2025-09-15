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
 *        \brief  Declaration of the ProfileProtector class.
 *
 *      \details  Contains the declarations for the ProfileProtector class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_PROTECTOR_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_PROTECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "vac/testing/test_adapter.h"

#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/internal/profile_frame.h"
#include "amsr/e2e/profiles/internal/profile_protector_interface.h"
#include "amsr/e2e/profiles/profile.h"

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Profile protector class.
 * \unit amsr::e2e::profiles::ProfileProtector
 * \vprivate Vector Internal API
 */
class ProfileProtector final : public internal::ProfileFrame, public internal::ProfileProtectorInterface {
 public:
  /*!
   * \brief Protector constructor.
   * \param[in] profile Profile to choose for the Profile Protector object to be created.
   * \param[in] props the E2E Event Protection Properties
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
  ProfileProtector(Profile profile, End2EndEventProtectionProps const& props) noexcept;

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
  ~ProfileProtector() noexcept final;

  // Explicitly delete all unused default/move/copy constructors and operators.
  ProfileProtector() = delete;
  ProfileProtector(ProfileProtector const&) = delete;
  ProfileProtector(ProfileProtector&&) = delete;
  ProfileProtector& operator=(ProfileProtector const&) = delete;
  ProfileProtector& operator=(ProfileProtector&&) = delete;

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
   * \spec
   *   requires true;
   * \endspec
   */
  ProtectReturnType Protect(ProfileFrame::BufferView& buffer) noexcept final;

 private:
  // Forward declaration because this header must not include any C header file
  class ProfileProtectorImplInterface;
  class StandardProfileProtectorImpl;
  class ProprietaryProfileProtectorImpl;

  /*!
   * \brief Pimpl Idiom used here because this header must not include any C header file.
   */
  std::unique_ptr<ProfileProtectorImplInterface> p_impl_;

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT_, ProfileProtector);
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_PROTECTOR_H_
