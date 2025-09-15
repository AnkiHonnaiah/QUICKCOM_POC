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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  key_slot_db_util.h
 *        \brief Utility class to read from the KeySlotDatabase and the KeySlot Files using the JSON format.
 *
 *        \details A collection of methods that can be used to easily access the KeySlotDatabase file.
 *        The file consists of Key Slot entries which contain some meta information about the KeySlot and a path to the
 *        specific KeySlot File. Key Slots can also be parsed and contain meta information about the key and the key
 *        material itself.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_KEYS_DB_KEY_SLOT_DB_UTIL_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_KEYS_DB_KEY_SLOT_DB_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <shared_mutex>

#include <memory>

#include "amsr/core/string.h"
#include "ara/core/vector.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "crypto/common/data/data_container.h"
#include "vac/testing/test_adapter.h"

namespace crypto {
namespace common {
namespace keys {
namespace db {

/*!
 * \brief Struct representing an entry of the KeySlot Database.
 * \vprivate Product Private
 */
struct SlotDbEntry {
  ara::crypto::keys::SlotNumber slot_number{}; /*!< Slot Number of the Key Slot */
  amsr::core::String file_path{};              /*!< path to the File containing the key data */
  ara::crypto::LogicalSlotUid slot_uuid{};     /*!< Uuid of the slot */
  ara::crypto::Uuid provider_uuid{};           /*!< Uuid of the provider this slot belongs to*/
  std::shared_ptr<std::shared_timed_mutex> lock{
      std::make_shared<std::shared_timed_mutex>()}; /*!< Mutex to manage access to the Slot File */
};

/*!
 * \brief Struct containing all KeySlot properties
 * \vprivate Product Private
 */
struct KeySlotProps {
  ara::crypto::keys::KeySlotPrototypeProps proto_props{}; /*!< Prototype properties of the slot */
  ara::crypto::keys::KeySlotContentProps content_props{}; /*!< Content properties of the Key inside the slot */
};

/*!
 * \brief Reads the content of an slot entry from the database.
 * \param[in] slot_entry Slot entry
 * \return DataContainer containing the content-content field
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kResourceFault if an underlying resource is not accessible
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat if file(s) could not be parsed
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto GetSlotContent(SlotDbEntry const& slot_entry) noexcept -> amsr::core::Result<data::DataContainer>;

/*!
 * \brief Reads the properties of an slot entry from the database.
 * \param[in] slot_entry Slot entry
 * \return KeySlotProps containing ContentProps and ProtoProps
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kResourceFault if the Slot Properties could not be read
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat if file(s) could not be parsed
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto GetSlotProperties(SlotDbEntry const& slot_entry) noexcept -> amsr::core::Result<KeySlotProps>;

/*!
 * \brief Reads the slot entries from the database.
 * \param[in] database_path Path to the JSON data base containing the slot entries
 * \return Vector of SlotDbEntry.
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kResourceFault if an underlying resource is not accessible
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat if file(s) could not be parsed
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto GetSlotEntriesFromDb(amsr::core::StringView database_path) noexcept
    -> amsr::core::Result<ara::core::Vector<SlotDbEntry>>;

/*!
 * \brief Writes the data to content field of this slot entries file.
 * \param[in] slot_entry that the data should be written to.
 * \param[in] content data that will be written to the content field.
 * \context ANY
 * \error ara::crypto::SecurityErrc::kResourceFault if an underlying resource is not accessible
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat if the file dose not contain a valid slot
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto WriteSlotContent(SlotDbEntry const& slot_entry, data::DataContainer const& content) noexcept
    -> amsr::core::Result<void>;

/*!
 * \brief Writes the data to content-meta field of this slot entries file.
 * \param[in] slot_entry that the data should be written to
 * \param[in] content_props data that will be written to the content field
 * \context ANY
 * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat If file content could not be parsed
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto WriteSlotContentProps(SlotDbEntry const& slot_entry, data::DataContainer const& content_props) noexcept
    -> amsr::core::Result<void>;

/*!
 * \brief Reads in a JSON file and parses it contents into a DataContainer
 * \param[in] file_path Path to the JSON file
 * \return DataContainer
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kResourceFault If an underlying resource is not accessible
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat If file content could not be parsed
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto ReadJson(amsr::core::StringView file_path) noexcept -> amsr::core::Result<crypto::common::data::DataContainer>;

/*!
 * \brief Creates a Slot entry object from a serial data container
 * \param[in] data DataContainer
 * \return SlotEntry
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kUnsupportedFormat if the data has invalid format or missing fields
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto DeserializeSlotDbEntry(crypto::common::data::DataContainer const& data) noexcept
    -> amsr::core::Result<SlotDbEntry>;

/*!
 * \brief Converts the path of an Slot entry to absolute an checks if the file exists.
 * \param[in] slot_entry Slot entry
 * \param[in] base_directory path to the base directory.
 * \return SlotEntry with absolute path to the slt file
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kResourceFault if an underlying resource is not accessible
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto ValidatePath(SlotDbEntry const& slot_entry, amsr::core::String const& base_directory) noexcept
    -> amsr::core::Result<SlotDbEntry>;

/*!
 * \brief Writes a given DataContainer to a JSON file.
 * \param[in] file_path path to the JSON file
 * \param[in] data DataContainer that will be written to the slot file.
 * \context ANY
 * \pre -
 * \error ara::crypto::SecurityErrc::kResourceFault if an underlying resource is not accessible
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Product Private
 */
auto WriteJson(amsr::core::StringView file_path, data::DataContainer const& data) noexcept -> amsr::core::Result<void>;

}  // namespace db
}  // namespace keys
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_KEYS_DB_KEY_SLOT_DB_UTIL_H_
