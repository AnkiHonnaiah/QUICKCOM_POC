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
 *        \brief  Proprietary profile protector implementation interface.
 *
 *      \details  Defines the interface for proprietary profile protector implementations.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_PROTECTOR_IMPL_INTERFACE_H_
#define LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_PROTECTOR_IMPL_INTERFACE_H_

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
class ProprietaryProfileProtectorImplInterface {
 public:
  /*!
   * \brief Constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  ProprietaryProfileProtectorImplInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  virtual ~ProprietaryProfileProtectorImplInterface() noexcept = default;

  // Explicitly delete all unused move/copy constructors and operators.
  ProprietaryProfileProtectorImplInterface(ProprietaryProfileProtectorImplInterface const&) = delete;
  ProprietaryProfileProtectorImplInterface(ProprietaryProfileProtectorImplInterface&&) = delete;
  ProprietaryProfileProtectorImplInterface& operator=(ProprietaryProfileProtectorImplInterface const&) = delete;
  ProprietaryProfileProtectorImplInterface& operator=(ProprietaryProfileProtectorImplInterface&&) = delete;

  /*!
   * \brief Protect routine for an E2E profile.
   * \param[in,out] buffer_view Holds the bytestream that includes the pre-allocated E2E header filled with dummy
   * values.
   * \return An empty result if successful.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_PXXProtect().
   * \threadsafe FALSE
   * \error E2eErrc::kWrongInput E2E_PXXProtect() detected a wrong input for buffer.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Vector Internal API
   */
  virtual void Protect(BufferView const& buffer_view) noexcept = 0;
};

}  // namespace proprietary
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_PROTECTOR_IMPL_INTERFACE_H_
