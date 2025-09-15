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
 *        \brief  Proprietary profile checker.
 *
 *      \details  Contains the declarations for the ProprietaryProfileChecker class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_CHECKER_H_
#define LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_CHECKER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/e2e/profiles/proprietary/proprietary_profile_checker_impl_interface.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace proprietary {

/*!
 * \brief Class used to permit linking if there is no proprietary, OEM-specific library available.
 *        The OEM-specific library shall be used instead for linking if available.
 */
class ProprietaryProfileChecker final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] proprietary_profile_frame Profile frame to use
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  explicit ProprietaryProfileChecker(Frame const& proprietary_profile_frame) noexcept;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~ProprietaryProfileChecker() noexcept = default;  // VCA_E2E_SLC15_UNIQUE_PTR

  // Explicitly delete all unused move/copy constructors and operators.
  ProprietaryProfileChecker(ProprietaryProfileChecker const&) = delete;
  ProprietaryProfileChecker(ProprietaryProfileChecker&&) = delete;
  ProprietaryProfileChecker& operator=(ProprietaryProfileChecker const&) = delete;
  ProprietaryProfileChecker& operator=(ProprietaryProfileChecker&&) = delete;

  /*!
   * \brief Check a received payload buffer.
   * \param[in] buffer_view buffer view to the received payload
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  CheckStatus Check(ConstBufferView const& buffer_view) noexcept;

 private:
  /*!
   * \brief Pointer to proprietary checker implementation.
   */
  std::unique_ptr<ProprietaryProfileCheckerImplInterface> checker_pimpl_;
};

}  // namespace proprietary
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_CHECKER_H_
