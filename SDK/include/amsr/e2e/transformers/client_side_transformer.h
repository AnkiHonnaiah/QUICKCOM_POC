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
 *        \brief  Convenience class to check protected data.
 *
 *      \details  Contains the declarations for the ClientSideTransformer class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_TRANSFORMERS_CLIENT_SIDE_TRANSFORMER_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_TRANSFORMERS_CLIENT_SIDE_TRANSFORMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/e2e/e2e_result.h"
#include "amsr/e2e/profiles/internal/profile_checker_interface.h"
#include "amsr/e2e/state_machine/internal/state_machine_interface.h"

namespace amsr {
namespace e2e {
namespace transformers {

/*!
 * \brief Convenience Class to check protected data that has been transformed by the E2E transformation process of a
 *        given byte stream.
 * \unit amsr::e2e::transformers::ClientSideTransformer
 * \vprivate Vector Internal API
 */
class ClientSideTransformer final {
 public:
  /*!
   * \brief Constructs a ClientSideTransformer for a given E2E-Profile.
   * \details On client-side the profile properties must be available for check functionality.
   * \param[in] checker Ref to profile checker to use
   * \param[in] e2e_state_machine Ref to E2E State Machine to use
   * \context ANY
   * \pre checker ref and its underlying object have to be valid until ~ClientSideTransformer() is called.
   * \pre e2e_state_machine ref and its underlying object have to be valid until ~ClientSideTransformer() is called.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ClientSideTransformer(amsr::e2e::profiles::internal::ProfileCheckerInterface& checker,
                        amsr::e2e::state_machine::internal::StateMachineInterface& e2e_state_machine) noexcept;

  /*!
   * \brief Default destructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~ClientSideTransformer() noexcept;

  // Explicitly delete all unused default/move/copy constructors and operators.
  ClientSideTransformer() = delete;
  ClientSideTransformer(ClientSideTransformer const&) = delete;
  ClientSideTransformer(ClientSideTransformer&&) = delete;
  ClientSideTransformer& operator=(ClientSideTransformer const&) = delete;
  ClientSideTransformer& operator=(ClientSideTransformer&&) = delete;

  /*!
   * \brief Executes an E2E check on a given stream, including the E2E header and the protected serialized payload.
   * \param[in] buffer Ref to the byte stream to do an E2E check for. This buffer includes the pre-allocated E2EHeader
   * for the given profile and the user-data.
   * \param[in] non_checked_offset The offset in bytes before the check.
   * \return The result of this E2E check.
   * \context ANY
   * \pre buffer ref and its underlying object have to be valid until function call completed.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::e2e::Result Check(profiles::internal::ProfileFrame::ConstBufferView const& buffer,
                          std::uint8_t const non_checked_offset = 0U) noexcept;

 private:
  /*!
   * \brief Profile checker
   */
  amsr::e2e::profiles::internal::ProfileCheckerInterface& checker_;

  /*!
   * \brief E2E state machine
   */
  amsr::e2e::state_machine::internal::StateMachineInterface& e2e_state_machine_;
};

}  // namespace transformers
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_TRANSFORMERS_CLIENT_SIDE_TRANSFORMER_H_
