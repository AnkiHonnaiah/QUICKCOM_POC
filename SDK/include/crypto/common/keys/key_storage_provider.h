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
/*!        \file  libcommon/include/crypto/common/keys/key_storage_provider.h
 *        \brief  Internal interface for crypto providers to interact with the KeyStorageProvider.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_KEYS_KEY_STORAGE_PROVIDER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_KEYS_KEY_STORAGE_PROVIDER_H_

#include "amsr/core/result.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "vac/language/unique_function.h"

namespace crypto {
namespace common {
namespace keys {

/*!
 * \brief           Extended KeyStorageProvider interface for crypto provider implementations interacting with the
 *                  KeyStorageProvider.
 * \vpublic
 */
class KeyStorageProvider : public ara::crypto::keys::KeyStorageProvider {
 public:
  /*!
   * \brief         Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<KeyStorageProvider>;

  /*!
   * \brief         Callback function called when a key slot has been cleared via the KeyStorageProvider.
   * \details       The callback function is invoked before the key slot is actually updated in the KeyStorageProvider.
   *                The update will only be performed after this method has returned \c true.
   * \param[in]     slot_number The slot number of the slot that has been cleared.
   * \param[in]     content_props The metadata of the current key slot content.
   * \param[in]     content_payload The current key slot content payload, can be empty.
   * \return        \c true if the slot was successfully cleared by the crypto provider, \c false otherwise.
   * \context       ReactorThread
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vpublic
   */
  using OnClearCallback = vac::language::UniqueFunction<bool(
      ara::crypto::keys::SlotNumber slot_number, ara::crypto::keys::KeySlotContentProps const& content_props,
      ara::crypto::ReadOnlyMemRegion content_payload)>;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief         Updates the ContentProps and (optional) payload of a key slot.
   * \details       Payload is expected to be JSON encoded.
   * \param[in]     slot_number The slot number of the slot to update.
   * \param[in]     content_props The metadata to update the slot with.
   * \param[in]     content_payload The payload to update the slot with, can be empty.
   * \context       CryptoProvider
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kUnreservedResource
   *                If the slot at \c slot_num does not exist.
   * \error         ara::crypto::SecurityErrc::kUnsupportedFormat
   *                If the \c content_payload can not be processed JSON encoded.
   * \error         ara::crypto::SecurityErrc::kContentRestrictions
   *                If the content does not meet the slot restrictions.
   * \error         ara::crypto::SecurityErrc::kRuntimeFault
   *                If the slot cannot be updated.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vpublic
   */
  virtual auto UpdateKeySlot(ara::crypto::keys::SlotNumber slot_number,
                             ara::crypto::keys::KeySlotContentProps content_props,
                             ara::crypto::ReadOnlyMemRegion content_payload) noexcept -> amsr::core::Result<void> = 0;

  /*!
   * \brief         Returns all key slot numbers for a crypto provider.
   * \param[in]     crypto_provider The CryptoProvider for which to return all key slot numbers.
   * \return        All key slot numbers belonging to the crypto provider \c crypto_provider with their respective
   *                key slot UUIDs. The returned collection has no particular order.
   * \context       CryptoProvider
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kRuntimeFault
   *                It the request could not be performed.
   * \error         ara::crypto::SecurityErrc::kUnknownIdentifier
   *                If \c crypto_provider is unknown to this key storage provider.
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vpublic
   */
  virtual auto GetAllKeySlotNumbers(ara::crypto::cryp::CryptoProvider const& crypto_provider) const noexcept
      -> amsr::core::Result<ara::core::Vector<std::pair<ara::crypto::keys::SlotNumber, ara::crypto::Uuid>>> = 0;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief         Register an OnClearCallback function to be notified.
   * \details       Only one callback can be registered per crypto provider (i.e. provider UUID).
   * \param[in]     crypto_provider The CryptoProvider for which to register the callback
   * \param[in]     callback_function Callback function to register
   * \return        -
   * \context       CryptoProvider
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vpublic
   */
  virtual auto RegisterOnClearCallback(ara::crypto::cryp::CryptoProvider const& crypto_provider,
                                       OnClearCallback&& callback_function) noexcept -> void = 0;
};

}  // namespace keys
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_KEYS_KEY_STORAGE_PROVIDER_H_
