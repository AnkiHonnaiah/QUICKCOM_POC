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
/*!        \file  json_key_database.h
 *        \brief  Database to handle JSON encoding and saving of keys.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_CRYPTO_SERVER_KEYS_JSON_KEY_DATABASE_H_
#define LIB_LIBSERVER_CRYPTO_SERVER_KEYS_JSON_KEY_DATABASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/stat.h>
#include <shared_mutex>

#include <atomic>
#include <limits>
#include <map>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "amsr/core/map.h"
#include "amsr/iostream/filestream/output_file_stream.h"
#include "ara/core/future.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/entry_point.h"
#include "crypto/server/keys/key_slot.h"
#include "crypto/server/keys/key_slot_content.h"
#include "crypto/server/keys/key_slot_handle.h"
#include "vac/container/static_list.h"
#include "vac/container/static_map.h"
#include "vac/testing/test_adapter.h"

namespace crypto {
namespace server {
namespace keys {

// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Model for JSON key storage access and maintenance.
 * \vprivate Component Private
 */
class JsonKeyDatabase final {
 public:
  /*!
   * \brief Slot number.
   * \vprivate Component Private
   */
  using SlotNumber = KeySlot::SlotNumber;

  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  JsonKeyDatabase() noexcept;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~JsonKeyDatabase() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   * \vprivate Component Private
   */
  JsonKeyDatabase(JsonKeyDatabase const&) noexcept = delete;

  /*!
   * \brief Deleted move constructor.
   * \vprivate Component Private
   */
  JsonKeyDatabase(JsonKeyDatabase&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return -
   */
  auto operator=(JsonKeyDatabase const&) & noexcept -> JsonKeyDatabase& = delete;

  /*!
   * \brief Deleted move assignment.
   * \return -
   */
  auto operator=(JsonKeyDatabase&&) & noexcept -> JsonKeyDatabase& = delete;

  /*!
   * \brief Create database in the specified file.
   * \param[in] db_path File to load the database from.
   * \return
   * \context ANY
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault If a generic runtime fault occurred
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If database is already attached to a source
   * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Create(amsr::core::String const& db_path) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Loads database from the specified file.
   * \param[in] db_path File to load the database from.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is already attached to a resource
   * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Open(amsr::core::String const& db_path) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Get a handle to the slot with a given slot number.
   * \param[in] slot_number Number of the slot.
   * \return A handle to the slot.
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnreservedResource No key slot found for given slot number.
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is not open.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSlot(SlotNumber slot_number) noexcept -> amsr::core::Result<KeySlotHandle>;

  /*!
   * \brief Read the meta data of a given slot.
   * \param[in] slot_handle Handle to the slot.
   * \return The slot meta data.
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is not open
   * \error ara::crypto::SecurityErrc::kUnreservedResource If key handle is invalid
   * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible or invalid
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto ReadSlot(KeySlotHandle const& slot_handle) noexcept -> amsr::core::Result<KeySlotContent>;

  /*!
   * \brief Write the meta data of a given slot.
   * \param[in] slot_handle Handle to the slot.
   * \param[in] slot_content Meta data to be saved in the slot.
   * \return
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is not open
   * \error ara::crypto::SecurityErrc::kUnreservedResource If key handle is invalid
   * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible or invalid
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto UpdateSlot(KeySlotHandle const& slot_handle, KeySlotContent const& slot_content) noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Updates the database configuration file.
   * \return
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible or invalid
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Update() const noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Closes the database.
   * \return
   * \context ANY
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is already attached to a resource.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Close() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Deletes the database.
   * \return
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the database is not open
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Delete() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Assures that neither slots are added to nor deleted from the database.
   * \return Scoped lock.
   * \context ANY
   * \threadsafe TRUE.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Freeze() const noexcept -> std::shared_lock<std::shared_timed_mutex>;

  /*!
   * \brief Gets the count of slots in the database.
   * \return Slot count.
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the Database is not yet attached to a source via Open()
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CountSlots() const noexcept -> amsr::core::Result<std::size_t>;

  /*!
   * \brief Creates a vector of currently available slots in the database.
   * \return Vector of slot handles.
   * \context ANY
   * \threadsafe TRUE.
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If the Database is not yet attached to a source via Open() v
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto LoadSlotHandles() const noexcept -> amsr::core::Result<vac::container::StaticVector<KeySlotHandle>>;

 private:
  /*!
   * \brief Writes the given data container to the path.
   * \param[in] path Destination file path
   * \param[in] content Serialized key slot content
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kResourceFault If any file access operation fails
   * \error ara::crypto::SecurityErrc::kRuntimeFault If serializing the data container to JSON string fails
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto WriteDataContainer(amsr::core::StringView path, common::data::DataContainer const& content) noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Read JSON file contents into data container.
   * \param[in] file_path Path to the file to be read
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kResourceFault If any file I/O operations fail or the file does not contain valid
   *                                                  JSON content
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto ReadFile(amsr::core::StringView file_path) noexcept -> amsr::core::Result<common::data::DataContainer>;

  /*!
   * \brief Deletes the slot synchronously.
   * \param[in] slot_handle Handle to the slot to be deleted.
   * \param[in] exclusive Indicates that this call is to be treated as exclusive, i.e. requires locking & config update.
   * \return
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidUsageOrder If database is not open
   * \error ara::crypto::SecurityErrc::kUnreservedResource If slot handle is invalid
   * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible or invalid
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto DeleteSlotInternal(KeySlotHandle const& slot_handle, bool exclusive) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Inserts a slot representation in the database.
   * \remark This method does not write to the file system. It only creates the runtime representation of the given
   * slots.
   * \param[in] slot Slot to be added.
   * \return Handle to the successfully registered slot.
   * \context ANY
   * \pre \c slot != nullptr
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kBusyResource If the slot number for the slot is already in use
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto RegisterSlot(KeySlot::Sptr const& slot) noexcept -> amsr::core::Result<KeySlotHandle>;

  /*!
   * \brief Serializes the overview of all available slots.
   * \param[in] acquire_locks Indicates if locks should be acquired internally.
   * \return Serial.
   * \context ANY
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SerializeConfig(bool acquire_locks = true) const noexcept -> common::data::DataContainer;

  /*!
   * \brief Writes the config file containing an overview of all available slots.
   * \param[in] data DataContainer object.
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kResourceFault If any file access operation fails
   * \error ara::crypto::SecurityErrc::kRuntimeFault If JSON string conversion fails
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto WriteConfigFile(common::data::DataContainer const& data) const noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Serializes the given COUID.
   * \param[in] couid COUID to be serialized.
   * \return Serial.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeCouid(ara::crypto::CryptoObjectUid couid) noexcept -> common::data::DataContainer;

  /*!
   * \brief Serializes the given slot prototype properties.
   * \param[in] props Slot prototype properties to be serialized.
   * \return Serial.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Dependency pointing to NULL slot UUID.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeProtoProps(ara::crypto::keys::KeySlotPrototypeProps const& props) noexcept
      -> amsr::core::Result<common::data::DataContainer>;

  /*!
   * \brief Serializes the given slot content properties.
   * \param[in] props Slot content properties to be serialized.
   * \return Serial.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeContentProps(ara::crypto::keys::KeySlotContentProps const& props) noexcept
      -> common::data::DataContainer;

  /*!
   * \brief Serializes the given slot.
   * \param[in] content_container Slot content to be serialized.
   * \return Serial.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Dependency pointing to NULL slot UUID.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat If the content could not be serialized.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeSlot(KeySlotContent const& content_container) noexcept
      -> amsr::core::Result<common::data::DataContainer>;

  /*!
   * \brief Deserializes the given COUID serial.
   * \param[in] serial Serial.
   * \return The COUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Cannot parse UUID: invalid format.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeCouid(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::CryptoObjectUid>;

  /*!
   * \brief Deserializes the given slot prototype properties serial.
   * \param[in] serial Serial.
   * \return The slot prototype properties.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeProtoProps(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::keys::KeySlotPrototypeProps>;

  /*!
   * \brief Deserializes the slot uid from serial.
   * \param[in] serial Serial.
   * \return The crypto slot uid.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeProtoPropsSetSlotUid(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::Uuid>;

  /*!
   * \brief Deserializes the provider uid from serial.
   * \param[in] serial Serial.
   * \return The crypto provider uid.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeProtoPropsSetProviderUid(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::Uuid>;

  /*!
   * \brief Deserializes the owner uid from serial.
   * \param[in] serial Serial.
   * \return The owner uid.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeProtoPropsOwnerUid(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::Uuid>;

  /*!
   * \brief Deserializes the slot capacity from serial.
   * \param[in] serial Serial.
   * \return The slot capacity.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeProtoPropsSlotCapacity(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<std::uint64_t>;

  /*!
   * \brief Deserializes the given slot content properties serial.
   * \param[in] serial Serial.
   * \return The slot content properties.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentProps(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::keys::KeySlotContentProps>;

  /*!
   * \brief Deserializes the given slot serial.
   * \param[in] serial Serial.
   * \return The slot.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to set the slot payload.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeSlot(common::data::DataContainer const& serial) noexcept -> amsr::core::Result<KeySlotContent>;

  /*!
   * \brief Deserializes a key slot.
   * \param[in] key_slot_content Key Slot content.
   * \param[in] data_container_content Data container content.
   * \return The slot content properties.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to set the slot payload.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeSlotMetaResult(KeySlotContent const& key_slot_content,
                                        common::data::DataContainer const& data_container_content) noexcept
      -> ara::core::Result<KeySlotContent>;
  /*!
   * \brief Convert a human-readable string representation of an algorithm to its ID.
   * \param[in] alg_name Algorithm string representation.
   * \return Algorithm ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c alg_name is not a valid algorithm name.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeAlgId(amsr::core::String const& alg_name) noexcept
      -> amsr::core::Result<ara::crypto::cryp::CryptoPrimitiveId::AlgId>;

  /*!
   * \brief Convert an algorithm ID to its human-readable string representation.
   * \param[in] alg_id Algorithm ID.
   * \return Algorithm string representation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c alg_name is not a valid algorithm id.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeAlgId(ara::crypto::cryp::CryptoPrimitiveId::AlgId alg_id) noexcept
      -> amsr::core::Result<amsr::core::String>;

  /*!
   * \brief Convert a human-readable string representation of a crypto object type to its ID.
   * \param[in] co_name Crypto object type string representation.
   * \return Crypto object type ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c co_name is not a name of a CryptoObjectType.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeCryptoObjectType(amsr::core::String const& co_name) noexcept
      -> amsr::core::Result<ara::crypto::CryptoObjectType>;

  /*!
   * \brief Convert a crypto object type ID to its human-readable string representation.
   * \param[in] co_type Crypto object type ID.
   * \return Crypto object type string representation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c co_type is not a valid CryptoObjectType.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeCryptoObjectType(ara::crypto::CryptoObjectType co_type) noexcept
      -> amsr::core::Result<amsr::core::String>;

  /*!
   * \brief Convert an allowed usage flag set to its human-readable string representations.
   * \param[in] allowed_usage Allowed usage flag set.
   * \return Vector of allowed usage string representations.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c allowed_usage contains only invalid flags.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto SerializeAllowedUsage(ara::crypto::AllowedUsageFlags allowed_usage) noexcept
      -> amsr::core::Result<ara::core::Vector<amsr::core::String>>;

  /*!
   * \brief Convert a collection of human-readable string representations of an allowed usage to the corresponding flag
   * set.
   * \param[in] allowed_usage Allowed usage string representation.
   * \return Allowed usage flag set.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c allowed_usage at least one unknown flag name.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeAllowedUsage(
      ara::core::Vector<std::reference_wrapper<amsr::core::String const>> const& allowed_usage) noexcept
      -> amsr::core::Result<ara::crypto::AllowedUsageFlags>;

  /*!
   * \brief Private method used in DeserializeContentProps
   * \param[in] serial Serial.
   * \param[in] props Serial.
   * \return The slot content properties.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeBitContent(common::data::DataContainer const& serial,
                                    ara::crypto::keys::KeySlotContentProps const& props) noexcept
      -> amsr::core::Result<ara::crypto::keys::KeySlotContentProps>;

  /*!
   * \brief Private method used in DeserializeContentProps
   * \param[in] serial Serial.
   * \param[in] props Serial.
   * \return The slot content properties.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Failed to fetch the data container.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContent(common::data::DataContainer const& serial,
                                 ara::crypto::keys::KeySlotContentProps const& props) noexcept
      -> amsr::core::Result<ara::crypto::keys::KeySlotContentProps>;

  /*!
   * \brief Tuple containing the content restriction information of a slot (object type, AlgId, and Exportability).
   * \vprivate Component Private
   */
  using ContentRestriction = std::tuple<ara::crypto::CryptoObjectType, ara::crypto::CryptoAlgId, bool>;

  /*!
   * \brief Tuple containing the dependency information of a slot (object type, SlotUid).
   * \vprivate Component Private
   */
  using SlotDependency = std::pair<ara::crypto::CryptoObjectType, ara::crypto::Uuid>;

  /*!
   * \brief Tuple containing the version control information of a slot (version control type, COUID).
   * \vprivate Component Private
   */
  using VersionControl = std::pair<ara::crypto::keys::VersionControlType, ara::crypto::CryptoObjectUid>;

  /*!
   * \brief Deserializes the content restriction given by \c serial
   * \param[in] serial DataContainer containing serialized ContentRestriction
   * \return The content restrictions.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentRestriction(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ContentRestriction>;

  /*!
   * \brief Deserialize content restriction Crypto Algorithm ID given by \c serial
   * \param[in] serial DataContainer containing serialized ContentRestriction
   * \return Deserialized Alg ID.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentRestrictionAlgIdAsString(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::CryptoAlgId>;

  /*!
   * \brief Deserialize content restriction Crypto Algorithm ID given by \c serial
   * \param[in] serial DataContainer containing serialized ContentRestriction
   * \return Deserialized Alg ID
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Encountered unknown argument.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentRestrictionAlgIdAsInteger(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::CryptoAlgId>;
  /*!
   * \brief Deserialize content restriction Crypto ObjectType given by serial.
   * \param[in] serial DataContainer containing serialized ContentRestriction
   * \return Deserialized CryptoObjectType
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid arguments.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentRestrictionCryptoObjectFromString(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::CryptoObjectType>;

  /*!
   * \brief Deserialize content restriction Crypto ObjectType given by serial.
   * \param[in] serial DataContainer containing serialized ContentRestriction
   * \return Deserialized CryptoObjectType.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Encountered unknown arguments.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentRestrictionCryptoObjectFromInteger(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<ara::crypto::CryptoObjectType>;

  /*!
   * \brief Deserialize content restriction exportability given by serial.
   * \param[in] serial DataContainer containing serialized ContentRestriction
   * \return Deserialized exportability.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Encountered unknown arguments.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeContentRestrictionExportability(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<bool>;

  /*!
   * \brief Deserializes the dependency type and slot id given by \c serial.
   * \param[in] serial DataContainer containing serialized slot dependency
   * \return The slot dependency information.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeSlotDependency(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<SlotDependency>;

  /*!
   * \brief Deserializes the dependency type and slot id given by \c serial.
   * \param[in] serial DataContainer containing serialized slot dependency
   * \return The slot dependency information.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Encountered invalid argument.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto DeserializeVersionControl(common::data::DataContainer const& serial) noexcept
      -> amsr::core::Result<VersionControl>;

  mutable std::shared_timed_mutex mutex_; /*!< Mutex for synchronizing member access */
  amsr::core::String config_file_path_;   /*!< Physical location of the database config file */
  amsr::core::String config_file_dir_;    /*!< Directory containing the database config file */
  crypto::common::util::Logger logger_;   /*!< Logging instance */
  std::atomic<bool> is_open_;             /*!< Indicator of the open state */

  using SlotMap = amsr::core::Map<KeySlotHandle, KeySlot::Sptr>; /*!< Type alias for the key slot map */
  SlotMap slot_map_;                                             /*!< In memory slot representation */

  /*!
   * Friend test declaration for member access
   */
  FRIEND_TEST(JsonKeyDatabase, MemberAccess);

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, WriteDataContainer);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, ReadFile);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, CreateSlot__UpdateFailed);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeAllowedUsage);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, CreateSlot);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeleteSlot);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, ReuseFreedSlot);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, CreateMultipleSlots);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeleteDatabase);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NoCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__Dependency);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NoDependencyCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidDependencyCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NoAlgId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidDependencyCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidAlgId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NumericAlgId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NoBitSize);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NoAllowedUsage);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidAllowedUsage);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NoType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__NumericType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeContentProps__InvalidType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoOwner);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoOwnerUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidOwnerUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoProvider);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoProviderUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidProviderUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoVersionControl);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoVersionControlPrevContent);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoVersionControlPrevContentCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidVersionControlPrevContentCouid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoVersionControlType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoContentRestriction);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__EmptyContentRestriction);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoContentRestrictionAlgId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NumericContentRestrictionAlgId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidContentRestrictionAlgId);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoContentRestrictionType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NumericContentRestrictionType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidContentRestrictionType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoDependency);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoDependencyUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__InvalidDependencyUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeProtoProps__NoCapacity);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__NoPath);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__AbsPath);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__NonExistingFile);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__NoSlotNumber);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__NoUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__InvalidUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__NoProvider);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__NoProviderUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__InvalidProviderUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__InsufficientCapacity);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Open__InvalidUuid);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, SerializeCryptoObjectType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeCryptoObjectType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeSlot_Empty);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, DeserializeSlot_EmptyContent);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__JsonKeyDatabase, Freeze);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__keys__CompositeKeyStorageProvider, Connect__MissingSlot);
};

}  // namespace keys
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_CRYPTO_SERVER_KEYS_JSON_KEY_DATABASE_H_
