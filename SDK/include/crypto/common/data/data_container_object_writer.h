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
/*!        \file  data_container_object_writer.h
 *        \brief  Writer class to convert structs to DataContainer.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_OBJECT_WRITER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_OBJECT_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/user_permissions.h"
#include "data_container.h"

namespace crypto {
namespace common {
namespace data {

/*!
 * \brief Writer class to convert structs to DataContainer.
 * \details This class contains multiple functions to convert API-defined objects into DataContainers.
 * \vprivate Component Private
 */
class DataContainerObjectWriter final {
 public:
  /*!
   * \brief Write KeySlotContentProps struct into the given DataContainer using the given key.
   * \param[in] key_slot_content_props Source struct.
   * \param[in] session_flag is_session property of content.
   * \return DataContainer containing the information from the KeySlotContentProps object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static DataContainer WriteObject(ara::crypto::keys::KeySlotContentProps const& key_slot_content_props,
                                   bool session_flag = false);

  /*!
   * \brief Write KeySlotPrototypeProps struct into the given DataContainer using the given key.
   * \param[in] key_slot_prototype_props Source struct.
   * \return DataContainer containing the information from the KeySlotPrototypeProps object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static DataContainer WriteObject(ara::crypto::keys::KeySlotPrototypeProps const& key_slot_prototype_props);

  /*!
   * \brief Write COUID struct into the given DataContainer using the given key.
   * \param[in] couid Crypto object UID to write.
   * \return DataContainer containing the information from the COUID object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static DataContainer WriteObject(ara::crypto::CryptoObjectUid const& couid);

  /*!
   * \brief Write UserPermissions struct into the given DataContainer using the given key.
   * \param[in] permissions Source struct.
   * \return DataContainer containing the information from the UserPermissions object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static DataContainer WriteObject(ara::crypto::keys::UserPermissions permissions);

  /*!
   * \brief Write Memory into the given DataContainer as HEX string using the given key.
   * \param[in] data Data to be written to container in hex representation.
   * \param[in] key Key to store the new object.
   * \return DataContainer containing \c data as a hex string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static DataContainer WriteMemRegion(ara::crypto::ReadOnlyMemRegion data, amsr::core::String const& key);

 private:
  // VECTOR NC AutosarC++17_10-A0.1.3: MD_CRYPTO_AutosarC++17_10-A0.1.3_not_called_static_function
  /*!
   * \brief Helper function to write a UUID to a data container.
   * \param[in] uuid The UUID to write.
   * \param[in] key The key to write the UUID for.
   * \param[out] data_container The data container to write to.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static inline void WriteUuid(ara::crypto::Uuid const& uuid, amsr::core::String const& key,
                               crypto::common::data::DataContainer& data_container) {
    data_container.AddString(key, uuid.ToString());
  }
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_OBJECT_WRITER_H_
