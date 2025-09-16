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
/*!        \file  file_system_persistency_provider.h
 *        \brief  File system based persistency provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_FILE_SYSTEM_PERSISTENCY_PROVIDER_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_FILE_SYSTEM_PERSISTENCY_PROVIDER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "crypto/x509provider/x509/persistency_provider.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief File system based persistency provider.
 * \vprivate Component Private
 * \trace DSGN-Crypto-X509Provider_X509_Certificate_Management
 */
class FileSystemPersistencyProvider : public PersistencyProvider {
 public:
  /*!
   * \brief Identifier to path map type.
   * \vprivate Component Private
   */
  using IdentifierToPathMap = amsr::core::Map<amsr::core::StringView, amsr::core::StringView>;

  /*!
   * \brief Creates a new file system based persistency provider.
   * \param[in] path_map (optional) Point to a map containing group identifier to file system path mappings.
   * \vprivate Component Private
   */
  explicit FileSystemPersistencyProvider(std::unique_ptr<IdentifierToPathMap> path_map = nullptr);

  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  ~FileSystemPersistencyProvider() noexcept override = default;

  /*!
   * \brief Deleted copy constructor.
   * \vprivate Component Private
   */
  FileSystemPersistencyProvider(FileSystemPersistencyProvider const&) = delete;

  /*!
   * \brief Deleted move constructor.
   * \vprivate Component Private
   */
  FileSystemPersistencyProvider(FileSystemPersistencyProvider&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return -
   * \vprivate Component Private
   */
  FileSystemPersistencyProvider& operator=(FileSystemPersistencyProvider const&) & = delete;

  /*!
   * \brief Deleted move assignment.
   * \return -
   * \vprivate Component Private
   */
  FileSystemPersistencyProvider& operator=(FileSystemPersistencyProvider&&) & noexcept = delete;

  /*!
   * \brief Initiates loading of all items found in the given group.
   * \details Loads available items until all have been processed or the operation has been aborted by the callback.
   * \param[in] group_identifier Group identifier of the group to be loaded.
   * \param[in] callback_handler Reference to the load callback handler.
   * \details The thread safety and the reentrancy depend on the provided callback.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE If the callback is reentrant
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void LoadGroup(amsr::core::StringView group_identifier,
                 PersistencyLoadCallbackHandler& callback_handler) noexcept override;

  /*!
   * \brief Checks whether an item has been persisted.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if a persisted item exists, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsPersisted(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                   amsr::core::StringView item_type, amsr::core::StringView subgroup_identifier) noexcept override;

  /*!
   * \brief Moves an item from one group to another.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] from_group Group identifier of the group the item belongs to.
   * \param[in] to_group Group identifier of the group the item shall be moved to.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if the item was moved successfully, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool MoveToGroup(amsr::core::StringView item_identifier, amsr::core::StringView from_group,
                   amsr::core::StringView to_group, amsr::core::StringView item_type,
                   amsr::core::StringView subgroup_identifier) noexcept override;

  /*!
   * \brief Persists an item.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] data Data to be stored for the item.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if a persisted item exists, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool PersistItem(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                   amsr::core::Span<std::uint8_t const> data, amsr::core::StringView item_type,
                   amsr::core::StringView subgroup_identifier) noexcept override;

  /*!
   * \brief Removes an item.
   * \param[in] item_identifier Item identifier of the item itself.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] item_type Type of the item.
   * \param[in] subgroup_identifier (optional) The subgroup identifier.
   * \return True if the item was removed, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool RemoveItem(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                  amsr::core::StringView item_type, amsr::core::StringView subgroup_identifier) noexcept override;

  /*!
   * \brief Removes a subgroup.
   * \param[in] group_identifier Group identifier of the group the item belongs to.
   * \param[in] subgroup_identifier The subgroup identifier.
   * \return True if the item was removed, otherwise false.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool RemoveSubgroup(amsr::core::StringView group_identifier,
                      amsr::core::StringView subgroup_identifier) noexcept override;

  /*!
   * \brief Determine whether the given identifier string is valid.
   * \param[in] identifier Identifier to validate
   * \return True if \c identifier is valid an can be used, false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsValidIdentifier(amsr::core::StringView identifier) const noexcept -> bool override;

  /*!
   * \brief Gets the group identifier to path map.
   * \return The path map.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  IdentifierToPathMap const& GetIdentifierToPathMap() const noexcept;

  /*!
   * \brief Sets the storage root folder.
   * \param[in] root Path to set root folder.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetRoot(amsr::core::StringView root) noexcept;

  /*!
   * \brief Retrieves the storage root folder.
   * \return The storage root folder.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::StringView GetRoot() const noexcept;

  /*!
   * \brief Generates the path to the folder for a given group.
   * \param[in] group_identifier Identifier of the group.
   * \return The path to the folder.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If group name is unknown or the associated directory path is
   * invalid.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If joining the paths is not possible.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<amsr::core::String> GeneratePath(amsr::core::StringView group_identifier) const;

  /*!
   * \brief Generates the path to the folder for a given subgroup.
   * \param[in] group_identifier Identifier of the group.
   * \param[in] subgroup_identifier Identifier of the subgroup.
   * \return The path to the folder.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If group name is unknown or the associated directory path is
   * invalid.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If joining the paths is not possible.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<amsr::core::String> GeneratePath(amsr::core::StringView group_identifier,
                                                      amsr::core::StringView subgroup_identifier) const;

  /*!
   * \brief Generates the file name for a given item.
   * \param[in] item_identifier Identifier of the item.
   * \param[in] item_type Type of the item.
   * \return File name to store the data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE The provided arguments must not be accessed in another thread at the same time.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::String GenerateFileName(amsr::core::StringView item_identifier, amsr::core::StringView item_type);

  /*!
   * \brief Check for a valid POSIX file name.
   * \details Only A-Za-z0-9.-_ are allowed
   * \param[in] file_name String to check
   * \return True if label is valid, false otherwise
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto IsValidPosixFileName(amsr::core::StringView file_name) noexcept -> bool;

 private:
  crypto::common::util::Logger logger_;           /*!< Logging instance */
  std::unique_ptr<IdentifierToPathMap> path_map_; /*!< Optionally injected group identifier to path map */
  amsr::core::String root_path_;                  /*!< Storage root folder */
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_FILE_SYSTEM_PERSISTENCY_PROVIDER_H_
