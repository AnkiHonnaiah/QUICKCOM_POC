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
/*!        \file  key_slot_content.h
 *        \brief  Content of a single KeySlot.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_CONTENT_H_
#define LIB_LIBSERER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_CONTENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "amsr/core/variant.h"
#include "ara/core/optional.h"
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "ara/crypto/keys/user_permissions.h"
#include "crypto/common/data/data_container.h"
#include "crypto/common/trusted_container.h"
#include "crypto/common/util/logger.h"
#include "vac/container/string_literals.h"
#include "vac/testing/test_adapter.h"

namespace crypto {
namespace server {
namespace keys {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Comparison operator for KeySlotPrototypeProps.
 * \param[in] left KeySlotPrototypeProps.
 * \param[in] right KeySlotPrototypeProps.
 * \return True if both KeySlotPrototypeProps are equal.
 * \context ANY
 * \pre -
 * \threadsafe TRUE iff the input arguments are not accessed simultaneously in another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator==(ara::crypto::keys::KeySlotPrototypeProps const& left,
                ara::crypto::keys::KeySlotPrototypeProps const& right) noexcept -> bool;

/*!
 * \brief Comparison operator for KeySlotContentProps.
 * \param[in] left KeySlotContentProps.
 * \param[in] right KeySlotContentProps.
 * \return True if both KeySlotContentProps are equal.
 * \context ANY
 * \pre -
 * \threadsafe TRUE iff the provided parameters are not accessed simulataneously in another thread.
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator==(ara::crypto::keys::KeySlotContentProps const& left,
                ara::crypto::keys::KeySlotContentProps const& right) noexcept -> bool;

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Content of a single KeySlot
 * \vprivate Component Private
 */
class KeySlotContent final {
  /*! Type alias for a vector of user permissions */
  using PermissionVector = ara::core::Vector<ara::crypto::keys::UserPermissions>;

 public:
  // VECTOR NC AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
  /*!
   * \brief Default constructor
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  KeySlotContent() : KeySlotContent(ara::crypto::keys::KeySlotPrototypeProps{}){};

  /*!
   * \brief Constructor using KeySlotPrototypeProps
   * \param[in] prototype_props The prototype props for this key slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit KeySlotContent(ara::crypto::keys::KeySlotPrototypeProps const& prototype_props);

  /*!
   * \brief Default copy constructor
   * \param[in] other The object to copy from
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  KeySlotContent(KeySlotContent const& other) noexcept
      // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
      : acl_{}, slot_prototype_props_{}, content_{}, logger_{common::util::kCommonLoggingContext, ""_sv} {
    *this = other;
  }

  /*!
   * \brief Default move constructor
   * \param[in] other The object to move from
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  KeySlotContent(KeySlotContent&& other) noexcept = default;

  /*!
   * \brief Copy assignment constructor
   * \param[in] other The object to copy from
   * \return A reference to the assignee
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  KeySlotContent& operator=(KeySlotContent const& other) & noexcept;

  /*!
   * \brief Move assignment operator
   * \param[in] other The object to move from
   * \return A reference to the assignee
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  KeySlotContent& operator=(KeySlotContent&& other) & noexcept;

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~KeySlotContent() noexcept = default;

  /*!
   * \brief Returns the state of this KeySlots payload data
   * \return true if slot does not contain payload data, false otherwise
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsEmpty() const noexcept;

  /*!
   * \brief Deletes the payload of this key slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Clear();

  /*!
   * \brief Serialize the key slot content payload into JSON.
   * \return The serialized payload.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault if the key slot content payload can not be serialized into JSON.
   * \error ara::crypto::SecurityErrc::kEmptyContainer when \c IsEmpty() == \c true
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SerializePayload() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Creates a trusted container for the content of this keyslot
   * \param[in] as_owner flag marking the resulting TC as the Owner of the KeySlot
   * \return Unique pointer to a TrustedContainer that carries all required meta information and payload data
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault if the key slot content payload can not be serialized as JSON.
   * \error ara::crypto::SecurityErrc::kEmptyContainer when \c IsEmpty() == \c true
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<common::TrustedContainer::Uptr> CreateTrustedContainer(bool as_owner) const;

  /*!
   * \brief Updates and stores the KeySlot from the given trusted container, this means several meta information and
   * payload
   * \param[in] content_payload Key slot content payload.
   * \param[in] content_props KeySlot to be updated.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault when
   *        content_props ObjectType not allowed by KeySlotPrototypeProps or
   *        Exportability not allowed by KeySlotPrototypeProps or
   *        AlgorithmId not allowed by KeySlotPrototypeProps or
   *        Trying to emplace older version of same object in KeySlotContent or
   *        Slot has not enough capacity for payload.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> UpdateKeySlotContent(common::data::DataContainer const& content_payload,
                                               ara::crypto::keys::KeySlotContentProps const& content_props);

  /*!
   * \brief Updates and stores the KeySlotContentProps
   * \param[in] content_props Content props the key slot shall be updated with
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault when
   *        content_props ObjectType not allowed by KeySlotPrototypeProps or
   *        Exportability not allowed by KeySlotPrototypeProps or
   *        AlgorithmId not allowed by KeySlotPrototypeProps or
   *        Trying to emplace older version of same object in KeySlotContent or
   *        Slot has not enough capacity for payload.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> UpdateKeySlotContentProps(ara::crypto::keys::KeySlotContentProps const& content_props);

  /*!
   * \brief Returns the ContentProperties of this KeySlot
   * \return ContentProps of the KeySlot
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::keys::KeySlotContentProps> GetKeySlotContentProperties() const noexcept;

  /*!
   * \brief Sets the PrototypeProperties of this KeySlot
   * \param[in] prototype_props Prototype props for this key slot
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetKeySlotPrototypeProperties(ara::crypto::keys::KeySlotPrototypeProps const& prototype_props) noexcept;

  /*!
   * \brief Returns the PrototypeProperties of this KeySlot
   * \return PrototypeProps of this KeySlot
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::keys::KeySlotPrototypeProps GetKeySlotPrototypeProperties() const noexcept;

  /*!
   * \brief Getter for the Owner UID of this KeySlot
   * \return UID of the owner of this Keyslot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::CryptoProviderUid GetOwnerUid() const noexcept;

  /*!
   * \brief ACL of this KeySlot
   * \return Access control list for this KeySlot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Vector<ara::crypto::keys::UserPermissions> GetUserList() const;

  /*!
   * \brief Checks if the given object is contained in this KeySlot
   * \param[in] object_uid UID of the object to be found
   * \param[in] object_type Type of the object to be found
   * \return true if content object has same UID and Type, false otherwise
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool ContainsObject(ara::crypto::keys::KeyStorageProvider::ObjectUid const& object_uid,
                      ara::crypto::keys::KeyStorageProvider::ContentType object_type) const;

  /*!
   * \brief Returns the slot's payload data.
   * \return Payload data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kEmptyContainer when \c IsEmpty() == \c true
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<common::data::DataContainer const&> GetPayload() const noexcept;

  /*!
   * \brief Structure of the slot content.
   * \vprivate Component Private
   */
  struct Content {
    ara::crypto::keys::KeySlotContentProps slot_content_props; /*!< ContentProps for this Slot */
    common::data::DataContainer slot_payload;                  /*!< Payload data stored in the slot */
  };

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CRYPTO_AutosarC++17_10-A11.3.1_comparators
  /*! Friend declaration for equal to comparison operator */
  friend auto operator==(KeySlotContent const& lhs, KeySlotContent const& rhs) noexcept -> bool;

 private:
  PermissionVector acl_;                                          /*!< Access control list for this KeySlot */
  ara::crypto::keys::KeySlotPrototypeProps slot_prototype_props_; /*!< PrototypeProps for this Slot */
  ara::core::Optional<Content> content_;                          /*!< Content of the slot */
  crypto::common::util::Logger logger_;                           /*!< Logging instance */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__crypto__server__keys__KeySlotContent, MoveAssignment);
};

/*!
 * \brief Comparison operator.
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return True iff both objects are identical, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator==(KeySlotContent const& lhs, KeySlotContent const& rhs) noexcept -> bool;

/*!
 * \brief Comparison operator.
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return True iff both objects are not equal, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator!=(KeySlotContent const& lhs, KeySlotContent const& rhs) noexcept -> bool;

}  // namespace keys
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_CONTENT_H_
