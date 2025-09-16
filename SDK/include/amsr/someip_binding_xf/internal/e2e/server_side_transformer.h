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
/*!        \file  server_side_transformer.h
 *         \brief A class for wrapping the server side transformers implemented by amsr-vector-fs-e2e component.
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::E2EHeaderSerializer
 *
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_SERVER_SIDE_TRANSFORMER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_SERVER_SIDE_TRANSFORMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/span.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/e2e/profiles/profile_protector.h"
#include "amsr/e2e/transformers/server_side_transformer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace e2e {

/*!
 * \details The wrapper is responsible for converting the input buffer and the result of protect method to the correct
 *          type.
 */
class ServerSideTransformer final {
 public:
  /*!
   * \brief       Constructor of ServerSideTransformer.
   *
   * \param[in]   profile                      The used E2E profile type.
   * \param[in]   event_protection_properties  E2E event protection props configuration.
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \trace SPEC-4980042
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal steady TRUE \endinternal
   */
  ServerSideTransformer(::amsr::e2e::profiles::Profile const profile,
                        ::amsr::e2e::profiles::End2EndEventProtectionProps const& event_protection_properties) noexcept;

  /*!
   * \brief          Protect a certain buffer based on the configured profile.
   *
   * \param[in, out] buffer            Holds the byte stream to Protect. This includes the pre-allocated
   *                                   E2EHeader for the given profile and the user-data.
   * \param[in]      protected_offset  Byte offset in the packet buffer to the payload which shall get protected by E2E.
   * \return         Based on the profile given a profile might give back additional information to its caller.
   * \pre            -
   * \context        App
   * \synchronous    TRUE
   * \trace          SPEC-4980041
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal steady TRUE \endinternal
   */
  ::amsr::e2e::profiles::ProfileProtector::ProtectReturnType Protect(amsr::core::Span<std::uint8_t>& buffer,
                                                                     std::size_t const protected_offset = 0U) noexcept;

  /*!
   * \brief        Get the header size to pre-allocate the correct buffer size.
   * \return       The header size from the specification.
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \synchronous  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::uint_fast8_t GetHeaderSize() const noexcept;

 private:
  /*!
   * \brief E2E profile protector
   */
  ::amsr::e2e::profiles::ProfileProtector profile_protector_;

  /*!
   * \brief Server side transformer
   */
  ::amsr::e2e::transformers::ServerSideTransformer transformer_;
};

}  // namespace e2e
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr
#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_SERVER_SIDE_TRANSFORMER_H_
