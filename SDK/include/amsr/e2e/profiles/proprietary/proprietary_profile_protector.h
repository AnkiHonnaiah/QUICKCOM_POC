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
 *        \brief  Proprietary profile protector.
 *
 *      \details  Contains the declarations for the ProprietaryProfileProtector class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_PROTECTOR_H_
#define LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_PROTECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/core/span.h"

#include "amsr/e2e/profiles/proprietary/proprietary_profile_protector_impl_interface.h"

namespace amsr {
namespace e2e {
namespace profiles {
namespace proprietary {

/*!
 * \brief Class used to permit linking if there is no proprietary, OEM-specific library available.
 *        The OEM-specific library shall be used instead for linking if available.
 */
class ProprietaryProfileProtector final {
 public:
  /*!
   * \brief Alias for amsr::core::Span<std::uint8_t>
   */
  using BufferView = amsr::core::Span<std::uint8_t>;

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
  explicit ProprietaryProfileProtector(Frame const& proprietary_profile_frame) noexcept;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~ProprietaryProfileProtector() noexcept = default;  // VCA_E2E_SLC15_UNIQUE_PTR

  // Explicitly delete all unused move/copy constructors and operators.
  ProprietaryProfileProtector(ProprietaryProfileProtector const&) = delete;
  ProprietaryProfileProtector(ProprietaryProfileProtector&&) = delete;
  ProprietaryProfileProtector& operator=(ProprietaryProfileProtector const&) = delete;
  ProprietaryProfileProtector& operator=(ProprietaryProfileProtector&&) = delete;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_E2E_M7.1.2
  /*!
   * \brief Protect routine for an E2E profile.
   * \param[in,out] buffer_view Holds the bytestream that includes the pre-allocated E2E header filled with dummy
   * values.
   * \return An empty result if successful.
   * \context ANY
   * \pre buffer.size() fits into the Length argument of E2E_PXXProtect().
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   */
  void Protect(BufferView& buffer_view) noexcept;

 private:
  /*!
   * \brief Pointer to proprietary protector implementation.
   */
  std::unique_ptr<ProprietaryProfileProtectorImplInterface> protector_pimpl_;
};

}  // namespace proprietary
}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBPROPRIETARY_INTERFACE_INCLUDE_AMSR_E2E_PROFILES_PROPRIETARY_PROPRIETARY_PROFILE_PROTECTOR_H_
