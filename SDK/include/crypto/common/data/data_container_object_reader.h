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
/*!        \file  data_container_object_reader.h
 *        \brief  Reader class to convert DataContainer to structs.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_OBJECT_READER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_OBJECT_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdlib>
#include <tuple>
#include <vector>

#include "amsr/core/string.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/user_permissions.h"
#include "crypto/common/data/data_container.h"

namespace crypto {
namespace common {
namespace data {

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Reader class to convert DataContainer to structs.
 * \details This class provides multiple functions to convert DataContainers (which are created by parsing JSON files)
 * into API-defined data types.
 * \vprivate Component Private
 */
class DataContainerObjectReader final {
 public:
  /*!
   * \brief Read KeySlotContentProps struct from the given container.
   * \param[in] data_container DataContainer that contains the object to be read.
   * \return Object read from configuration.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if at least one required field is missing.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<ara::crypto::keys::KeySlotContentProps> ReadKeySlotContentProps(
      DataContainer const& data_container) noexcept;
  /*!
   * \brief Read KeySlotPrototypeProps struct from the given container.
   * \param[in] data_container DataContainer that contains the object to be read.
   * \return Object read from configuration.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if at least one required field is missing.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<ara::crypto::keys::KeySlotPrototypeProps> ReadKeySlotPrototypeProps(
      DataContainer const& data_container) noexcept;
  /*!
   * \brief Read a nested UUID located in a field named "uuid" in data_container under key.
   * \param[in] data_container DataContainer that contains the object to be read.
   * \param[in] key key of the object containing the uuid field.
   * \return UUID read from field.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if key is not in data_container or object corresponding to key
   * has no "uuid" field.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<ara::crypto::Uuid> ReadNestedUuid(crypto::common::data::DataContainer const& data_container,
                                                              amsr::core::String const& key) noexcept;
  /*!
   * \brief Read COUID struct from the given container.
   * \param[in] data_container DataContainer that contains the object to be read.
   * \param[in] key name of the COUID field in data_container (usually "couid").
   * \return Object read from configuration.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if at least one required field is missing.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<ara::crypto::CryptoObjectUid> ReadCouid(DataContainer const& data_container,
                                                                    amsr::core::String const& key) noexcept;
  /*!
   * \brief Reads the hex content of the data container and write it into the provided memory.
   * \details The return value will be a subspan of the provided memory filled with the read data.
   * \param[in] data_container Configuration that contains the data to be read as hex string.
   * \param[in] key key that contains the hex string to read.
   * \param[in] memory Memory to read the data into.
   * \return Deserialized data in a subspan of the provided memory.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if key is not in data_container.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<ara::crypto::WritableMemRegion> ReadMemRegion(
      DataContainer const& data_container, amsr::core::String const& key,
      ara::crypto::WritableMemRegion memory) noexcept;

  /*!
   * \brief Method will read the hex content of the data container and write it into the provided memory.
   * \details The return value will be a subspan of the provided memory filled with the read data.
   * \param[in] data_container Configuration that contains the data to be read as hex string.
   * \param[in] key key that contains the hex string to read.
   * \param[in] memory Memory to read the data into.
   * \param[in] expected_size Expected size of the data to be read.
   * \return Deserialized data in a subspan of the provided memory.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if key is not in data_container.
   * \error ara::crypto::SecurityErrc::kInvalidArgument if actual size read from memory does not match the expected
   * size.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<ara::crypto::WritableMemRegion> ReadMemRegion(DataContainer const& data_container,
                                                                          amsr::core::String const& key,
                                                                          ara::crypto::WritableMemRegion memory,
                                                                          std::size_t expected_size) noexcept;
  /*!
   * \brief Reads version control information from \c data_container.GetObject(key).
   * \details There is currently no use for the CryptoObjectType, since the AUTOSAR API does not specify a field for
   * that in the KeySlotPrototypeProps. But since the COUID (mVersionTrack) is useless without the corresponding type,
   * we included a field for the CryptoObjectType in our schema. \param[in] data_container DataContainer which contains
   * version control information at \c key. \param[in] key Key of version control information in \c data_container.
   * \details If no DataContainer exists for \c key this function returns a tuple containing default values.
   * \return Tuple comprised of a COUID (KeySlotPrototypeProps.mVersionTrack), CryptoObjectType and a VersionControlType
   * (KeySlotPrototypeProps.mVersionControl)
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier If version control container exists but COUID is missing.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<
      std::tuple<ara::crypto::CryptoObjectUid, ara::crypto::CryptoObjectType, ara::crypto::keys::VersionControlType>>
  ReadVersionControl(DataContainer const& data_container, amsr::core::String const& key) noexcept;

  /*!
   * \brief Reads content restriction information from \c data_container.GetObject(key).
   * \param[in] data_container DataContainer which contains contente restriction information at \c key.
   * \param[in] key Key of content restriction information in \c data_container.
   * \details If no DataContainer exists for \c key this function returns a tuple containing default values.
   * \return Tuple comprised of a CryptoObjectType (KeySlotPrototypeProps.mObjectType), an algorithm id
   * (KeySlotPrototypeProps.mAlgId) and a bool (KeySlotPrototypeProps.mExportibility).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier If content restriction container exists but it doesn't contain
   * any valid values for aforementioned properties.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<std::tuple<ara::crypto::CryptoObjectType, ara::crypto::CryptoAlgId, bool>>
  ReadContentRestriction(DataContainer const& data_container, amsr::core::String const& key) noexcept;

 private:
  /*!
   * \brief Deserializes a string to the correct CryptoObjectType
   * \param[in] co_name Name of the crypto object in plaintext
   * \return CryptoObjectType
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if invalid string was passed.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<ara::crypto::CryptoObjectType> DeserializeCryptoObjectType(
      amsr::core::String const& co_name) noexcept;

  /*!
   * \brief Deserializes multiple strings to the correct AllowedUsage enum
   * \param[in] allowed_usages Vector of plain text AllowedUsages
   * \return AllowedUsageFlags
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if invalid string was passed.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<ara::crypto::AllowedUsageFlags> DeserializeAllowedUsage(
      ara::core::Vector<std::reference_wrapper<amsr::core::String const>> const& allowed_usages) noexcept;

  /*!
   * \brief Deserializes plain text AlgId to correct enum
   * \param[in] serial_alg_id Plain text Algorithm ID
   * \return AlgId enum value
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if invalid string was passed.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<ara::crypto::CryptoAlgId> DeserializeAlgId(
      amsr::core::String const& serial_alg_id) noexcept;

  /*!
   * \brief Read the algorithm identifier.
   * \param[in] data_container The data container to be read from
   * \return AlgId integer value
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if invalid string was passed.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static auto ReadAlgId(DataContainer const& data_container) noexcept -> amsr::core::Result<ara::crypto::CryptoAlgId>;

  /*!
   * \brief Read the object type.
   * \param[in] data_container The data container to be read from
   * \return CryptoObjectType enum value
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if invalid string was passed.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static auto ReadObjectType(DataContainer const& data_container) noexcept
      -> amsr::core::Result<ara::crypto::CryptoObjectType>;

  /*!
   * \brief Read the allowed usage flags.
   * \param[in] data_container The data container to be read from
   * \return AllowedUsageFlags enum value
   * \context ANY
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument if invalid string was passed.
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static auto ReadAllowedUsage(DataContainer const& data_container) noexcept
      -> amsr::core::Result<ara::crypto::AllowedUsageFlags>;

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__DataContainer_ObjectReader, DeserializeAllowedUsage);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__DataContainer_ObjectReader, DeserializeObjectType);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__DataContainer_ObjectReader, DeserializeAlgId);
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_OBJECT_READER_H_
