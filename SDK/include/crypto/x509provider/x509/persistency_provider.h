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
/*!        \file  persistency_provider.h
 *        \brief  Abstract persistency provider interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_PROVIDER_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_PROVIDER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/span.h"
#include "crypto/x509provider/x509/persistency_load_callback_handler.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief Persistency provider interface.
 * \vprivate Component Private
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 */
class PersistencyProvider {
 public:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  PersistencyProvider() = default;

  /*!
   * \brief Default destructor.
   * \vprivate Component Private
   */
  virtual ~PersistencyProvider() noexcept = default;

  /*!
   * \brief Initiates loading of all items found in the given group.
   * \details Loads available items until all have been processed or the operation has been aborted by the callback.
   * \param[in] group_identifier Group identifier of the group to be loaded.
   * \param[in] callback_handler Reference to the load callback handler.
   * \vprivate Component Private
   */
  virtual void LoadGroup(amsr::core::StringView group_identifier,
                         PersistencyLoadCallbackHandler& callback_handler) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Checks whether an item has been persisted.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if a persisted item exists, otherwise false.
   * \vprivate Component Private
   */
  virtual bool IsPersisted(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                           amsr::core::StringView item_type,
                           amsr::core::StringView subgroup_identifier = ""_sv) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Moves an item from one group to another.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] from_group Group identifier of the group the item belongs to.
   * \param[in] to_group Group identifier of the group the item shall be moved to.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if the item was moved successfully, otherwise false.
   * \vprivate Component Private
   */
  virtual bool MoveToGroup(amsr::core::StringView item_identifier, amsr::core::StringView from_group,
                           amsr::core::StringView to_group, amsr::core::StringView item_type,
                           amsr::core::StringView subgroup_identifier = ""_sv) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Persists an item.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] data Data to be stored for the item.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if a persisted item exists, otherwise false.
   * \vprivate Component Private
   */
  virtual bool PersistItem(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                           amsr::core::Span<std::uint8_t const> data, amsr::core::StringView item_type,
                           amsr::core::StringView subgroup_identifier = ""_sv) noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Removes an item.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if the item was removed, otherwise false.
   * \vprivate Component Private
   */
  virtual bool RemoveItem(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                          amsr::core::StringView item_type,
                          amsr::core::StringView subgroup_identifier = ""_sv) noexcept = 0;

  /*!
   * \brief Removes a subgroup.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] subgroup_identifier The subgroup identifier.
   * \return True if the item was removed, otherwise false.
   * \vprivate Component Private
   */
  virtual bool RemoveSubgroup(amsr::core::StringView group_identifier,
                              amsr::core::StringView subgroup_identifier) noexcept = 0;

  /*!
   * \brief Determine whether the given identifier string is valid.
   * \param[in] identifier Identifier to validate
   * \return Ture if \c identifier is valid and can be used, false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto IsValidIdentifier(amsr::core::StringView identifier) const noexcept -> bool = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \vprivate Component Private
   */
  PersistencyProvider(PersistencyProvider const&) = default;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  PersistencyProvider(PersistencyProvider&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  PersistencyProvider& operator=(PersistencyProvider const&) & = default;

  /*!
   * \brief Default move assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  PersistencyProvider& operator=(PersistencyProvider&&) & noexcept = default;
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_PROVIDER_H_
