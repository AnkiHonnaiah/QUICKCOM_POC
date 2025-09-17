/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  persistency_load_callback_handler.h
 *        \brief  Callback handler interface for the persistency provider's load operation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_LOAD_CALLBACK_HANDLER_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_LOAD_CALLBACK_HANDLER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace x509provider {
namespace x509 {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Import string literal for StringView creation */
using vac::container::operator""_sv;

/*!
 * \brief Callback handler interface for the persistency provider's load operation.
 * \vprivate Component Private
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 */
class PersistencyLoadCallbackHandler {
 public:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  PersistencyLoadCallbackHandler() = default;

  /*!
   * \brief Default destructor.
   * \vprivate Component Private
   */
  virtual ~PersistencyLoadCallbackHandler() noexcept = default;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Callback handling a single item loaded from the persistency provider.
   *
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] data Data content of the item.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier Subgroup identifier.
   *
   * \return True to continue load operation, false to abort.
   * \vprivate Component Private
   */
  virtual bool HandleLoadedItem(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                                amsr::core::Span<std::uint8_t const> data, amsr::core::StringView item_type,
                                amsr::core::StringView subgroup_identifier = ""_sv) noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \vprivate Component Private
   */
  PersistencyLoadCallbackHandler(PersistencyLoadCallbackHandler const&) = default;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  PersistencyLoadCallbackHandler(PersistencyLoadCallbackHandler&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  PersistencyLoadCallbackHandler& operator=(PersistencyLoadCallbackHandler const&) & = default;

  /*!
   * \brief Default move assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  PersistencyLoadCallbackHandler& operator=(PersistencyLoadCallbackHandler&&) & noexcept = default;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_LOAD_CALLBACK_HANDLER_H_
