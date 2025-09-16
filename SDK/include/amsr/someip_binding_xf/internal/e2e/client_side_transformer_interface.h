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
/*!        \file  client_side_transformer_interface.h
 *        \brief
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_CLIENT_SIDE_TRANSFORMER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_CLIENT_SIDE_TRANSFORMER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/core/span.h"
#include "amsr/e2e/e2e_result.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace e2e {

/*!
 * \brief Interface defining client side transformer API.
 */
class ClientSideTransformerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context Init
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ClientSideTransformerInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady  FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClientSideTransformerInterface() noexcept = default;

  ClientSideTransformerInterface(ClientSideTransformerInterface const&) = delete;
  ClientSideTransformerInterface(ClientSideTransformerInterface&&) = delete;
  ClientSideTransformerInterface& operator=(ClientSideTransformerInterface const&) & = delete;
  ClientSideTransformerInterface& operator=(ClientSideTransformerInterface&&) & = delete;

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
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC VectorC++-V8.3.1: MD_SOMEIPBINDING_VectorC++-V8.3.1_default_parameter_in_virtual_method
  virtual ::amsr::e2e::Result Check(amsr::core::Span<std::uint8_t const> const& input,
                                    std::uint8_t const non_checked_offset = 0U) noexcept = 0;

  /*!
   * \brief       Notifies that an invalid sample was received.
   * \return      The result of this E2E check.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual ::amsr::e2e::Result NotifyInvalidSample() noexcept = 0;

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
  virtual std::uint_fast8_t GetHeaderSize() const noexcept = 0;
};

}  // namespace e2e
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_CLIENT_SIDE_TRANSFORMER_INTERFACE_H_
