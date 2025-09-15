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
/*!        \file  client_side_transformer.h
 *         \brief A class for wrapping the client side transformer implemented by amsr-vector-fs-e2e component
 *                to be used by the ara::com implementation and generators.
 *         \unit  SomeIpBinding::SomeIpBindingXf::ClientSideTransformer
 *
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_CLIENT_SIDE_TRANSFORMER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_CLIENT_SIDE_TRANSFORMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/span.h"
#include "amsr/e2e/profiles/e2e_profile_configuration.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/e2e/profiles/profile_checker.h"
#include "amsr/e2e/state_machine/state_machine.h"
#include "amsr/e2e/transformers/client_side_transformer.h"
#include "amsr/someip_binding_xf/internal/e2e/client_side_transformer_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace e2e {
/*!
 * \details The wrapper is also responsible for extracting the required E2E configurations needed by the E2E supplier
 *          library out of the ara::com E2E configurations.
 */
// VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
class ClientSideTransformer final : public ClientSideTransformerInterface {
 public:
  /*!
   * \brief       On client-side the profile properties must be available for check functionality.
   *
   * \param[in]   profile                      E2E profile.
   * \param[in]   event_protection_properties  E2E event protection props configuration.
   * \param[in]   profile_configuration        E2E profile configuration.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ClientSideTransformer(::amsr::e2e::profiles::Profile const profile,
                        ::amsr::e2e::profiles::End2EndEventProtectionProps const event_protection_properties,
                        ::amsr::e2e::profiles::E2EProfileConfiguration const profile_configuration) noexcept;
  /*!
   * \brief       Executes an E2E check on a given stream, including the E2E header and
   *              the protected serialized payload. This will only be called for checkers.
   *
   * \param[in]   input               Holds the byte stream to do an E2E check for. This buffer includes
   *                                  the pre-allocated E2EHeader for the given profile and the user-data.
   * \param[in]   non_checked_offset  The offset in bytes before the check.
   * \return      The result of this  E2E check.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace       SPEC-4980041
   * \trace       SPEC-4980051
   * \trace       SPEC-13650592
   * \trace       SPEC-4980056
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC VectorC++-V8.3.1: MD_SOMEIPBINDING_VectorC++-V8.3.1_default_parameter_in_virtual_method
  ::amsr::e2e::Result Check(amsr::core::Span<std::uint8_t const> const& input,
                            std::uint8_t const non_checked_offset = 0U) noexcept final;

  /*!
   * \brief       Notifies that an invalid sample was received.
   * \return      The result of this E2E check.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  ::amsr::e2e::Result NotifyInvalidSample() noexcept final;

  /*!
   * \brief       Get the header size to pre-allocate the correct buffer size.
   * \return      The header size from the specification.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::uint_fast8_t GetHeaderSize() const noexcept final;

 private:
  /*!
   * \brief E2E profile checker.
   */
  amsr::core::Optional<::amsr::e2e::profiles::ProfileChecker> profile_checker_;

  /*!
   * \brief E2E state machine.
   */
  amsr::core::Optional<::amsr::e2e::state_machine::StateMachine> state_machine_;

  /*!
   * \brief Client side transformer.
   */
  amsr::core::Optional<::amsr::e2e::transformers::ClientSideTransformer> client_side_transformer_;
};

}  // namespace e2e
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr
#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_CLIENT_SIDE_TRANSFORMER_H_
