/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Proprietary profile checker implementation interface.
 *
 *      \details  Defines the interface for proprietary profile checker implementations.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_CHECKER_IMPL_INTERFACE_H_
#define LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_CHECKER_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/e2e/profiles/proprietary/types.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace proprietary {

/*!
 * \brief Class used to permit linking if there is no proprietary, OEM-specific library available.
 *        The OEM-specific library shall be used instead for linking if available.
 */
class ProprietaryProfileCheckerImplInterface {
 public:
  /*!
   * \brief Constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  ProprietaryProfileCheckerImplInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  virtual ~ProprietaryProfileCheckerImplInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  ProprietaryProfileCheckerImplInterface(ProprietaryProfileCheckerImplInterface const&) = delete;
  ProprietaryProfileCheckerImplInterface(ProprietaryProfileCheckerImplInterface&&) = delete;
  ProprietaryProfileCheckerImplInterface& operator=(ProprietaryProfileCheckerImplInterface const&) = delete;
  ProprietaryProfileCheckerImplInterface& operator=(ProprietaryProfileCheckerImplInterface&&) = delete;

  /*!
   * \brief Check a received payload buffer.
   * \param[in] buffer_view buffer view to the received payload
   * \return The check status according to the profile specification.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual CheckStatus Check(ConstBufferView const& buffer_view) noexcept = 0;
};

}  // namespace proprietary
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_CHECKER_IMPL_INTERFACE_H_
