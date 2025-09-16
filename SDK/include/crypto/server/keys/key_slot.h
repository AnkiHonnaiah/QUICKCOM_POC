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
/*!        \file  key_slot.h
 *        \brief  Class representing a key slot.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstdint>
#include <shared_mutex>

#include "amsr/core/string.h"
#include "ara/crypto/common/guid.h"
#include "ara/crypto/keys/elementary_types.h"

namespace crypto {
namespace server {
namespace keys {

/*!
 * \brief Internal slot representation.
 * \vprivate Component Private
 */
class KeySlot final : public std::enable_shared_from_this<KeySlot> {
 public:
  /*!
   * \brief Type alias for shared pointers to key slots.
   */
  using Sptr = std::shared_ptr<KeySlot>;

  /*!
   * \brief Slot number.
   * \vprivate Component Private
   */
  using SlotNumber = ara::crypto::keys::SlotNumber;

  /*!
   * \brief Construct a new KeySlot instance.
   * \param[in] slot_number The slot number
   * \param[in] slot_uuid The slot UUID
   * \param[in] provider_uuid The provider UUID
   * \param[in] storage_path The storage path
   * \vprivate Component Private
   */
  KeySlot(SlotNumber slot_number, ara::crypto::Uuid slot_uuid, ara::crypto::Uuid provider_uuid,
          amsr::core::StringView storage_path) noexcept;

  /*!
   * \brief Destroy this KeySlot instance.
   * \vprivate Component Private
   */
  ~KeySlot() noexcept = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  KeySlot(KeySlot const& /*other*/) noexcept = delete;

  /*!
   * \brief Move construct a new KeySlotHandle instance.
   * \vprivate Component Private
   */
  KeySlot(KeySlot&& /*other*/) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  auto operator=(KeySlot const& /*other*/) & noexcept -> KeySlot& = delete;

  /*!
   * \brief Move assignable a new KeySlotHandle instance.
   * \return reference to this.
   * \vprivate Component Private
   */
  auto operator=(KeySlot&& /*other*/) & noexcept -> KeySlot& = delete;

  /*!
   * \brief Get the slot number of this KeySlot.
   * \return Slot number (primary key).
   * \vprivate Component Private
   */
  auto GetSlotNumber() const noexcept -> SlotNumber { return number_; }

  /*!
   * \brief Get the slot UUID of this KeySlot.
   * \return Slot UUID.
   * \vprivate Component Private
   */
  auto GetSlotUuid() const noexcept -> ara::crypto::Uuid const& { return uuid_; }

  /*!
   * \brief Get the provider UUID of this KeySlot.
   * \return Slot provider UUID.
   * \vprivate Component Private
   */
  auto GetProviderUuid() const noexcept -> ara::crypto::Uuid const& { return provider_uuid_; }

  /*!
   * \brief Get the storage path of this KeySlot.
   * \return Identifier for the storage location.
   * \vprivate Component Private
   */
  auto GetStoragePath() const noexcept -> amsr::core::StringView { return storage_path_; }

 private:
  SlotNumber number_;               /*!< Slot number (primary key) */
  ara::crypto::Uuid uuid_;          /*!< Slot UUID */
  ara::crypto::Uuid provider_uuid_; /*!< Slot provider UUID */
  amsr::core::String storage_path_; /*!< Identifier for the storage location */
};

}  // namespace keys
}  // namespace server
}  // namespace crypto

#endif  //  LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_H_
