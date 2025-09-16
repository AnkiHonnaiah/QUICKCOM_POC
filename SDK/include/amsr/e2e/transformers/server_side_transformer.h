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
 *        \brief  Convenience class to protect data by the Transformer.
 *
 *      \details  Contains the declarations for the ServerSideTransformer class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_TRANSFORMERS_SERVER_SIDE_TRANSFORMER_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_TRANSFORMERS_SERVER_SIDE_TRANSFORMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/internal/profile_protector_interface.h"
#include "amsr/e2e/profiles/profile.h"

namespace amsr {
namespace e2e {
namespace transformers {

/*!
 * \brief Convenience Class for performing the E2E transformation process of a given byte stream by protecting the data
 *        inside the stream.
 * \unit amsr::e2e::transformers::ServerSideTransformer
 * \vprivate Vector Internal API
 */
class ServerSideTransformer final {
 public:
  /*!
   * \brief Constructs a ServerSideTransformer for a given E2E-Profile.
   * \details On server-side the profile properties are not available.
   * \param[in] protector Reference to protector.
   * \context ANY
   * \pre protector ref and its underlying object have to be valid until ~ServerSideTransformer() is called.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ServerSideTransformer(profiles::internal::ProfileProtectorInterface& protector) noexcept;

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
  ~ServerSideTransformer() noexcept;

  // Explicitly delete all unused default/move/copy constructors and operators.
  ServerSideTransformer() = delete;
  ServerSideTransformer(ServerSideTransformer const&) = delete;
  ServerSideTransformer(ServerSideTransformer&&) = delete;
  ServerSideTransformer& operator=(ServerSideTransformer const&) = delete;
  ServerSideTransformer& operator=(ServerSideTransformer&&) = delete;

  // VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_E2E_M7.1.2
  /*!
   * \brief Protect a certain buffer based on the configured profile.
   * \param[in,out] buffer Holds the byte stream to verify. This includes the pre-allocated E2EHeader for the given
   *               profile and the user-data.
   * \param[in] protected_offset Byte offset in the packet buffer to the payload which shall
   *                             get protected by E2E.
   * \return An empty result if successful.
   * \context ANY
   * \pre buffer ref and its underlying object have to be valid until function call completed.
   * \threadsafe FALSE
   * \error E2eErrc::kWrongInput wrong input provided.
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   * \spec
   *   requires true;
   * \endspec
   */
  profiles::internal::ProfileProtectorInterface::ProtectReturnType Protect(
      profiles::internal::ProfileFrame::BufferView& buffer, std::size_t const protected_offset = 0U) noexcept;

 private:
  /*!
   * \brief Protector instance.
   */
  profiles::internal::ProfileProtectorInterface& protector_;
};

}  // namespace transformers
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_TRANSFORMERS_SERVER_SIDE_TRANSFORMER_H_
