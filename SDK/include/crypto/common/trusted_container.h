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
/*!        \file  libcommon/include/crypto/common/trusted_container.h
 *        \brief  Interface of the TrustedContainer used by the KeyStorageProvider and CryptoProvider implementations.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TRUSTED_CONTAINER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TRUSTED_CONTAINER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "crypto/common/data/data_container.h"
#include "crypto/common/util/observable.h"

namespace crypto {
namespace common {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
/*!
 * \brief AMSR CryptoProvider TrustedContainer implementation
 * \vprivate Product Private
 */
class TrustedContainer : public ara::crypto::TrustedContainer, public crypto::common::util::Observable {
 public:
  /*!
   * \brief UniquePtr for this TrustedContainer
   * \vprivate Product Private
   */
  using Uptr = std::unique_ptr<crypto::common::TrustedContainer, ara::crypto::CustomDeleter>;

  /*!
   * \brief Handle type to identify the content of this TrustedContainer
   * \vprivate Product Private
   */
  using ContainerHandle = ara::crypto::Uuid;

  /*!
   * \brief Get the KeySlotContent properties of the KeySlot associated with this trusted container
   * \return KeySlotContentProps of this trusted container
   * \error ara::crypto::SecurityErrc::kEmptyContainer If the Trusted container dose not contain any content props.
   * \vprivate Product Private
   */
  virtual amsr::core::Result<ara::crypto::keys::KeySlotContentProps> GetKeySlotContentProps() const noexcept = 0;

  /*!
   * \brief Update the ContentProps data of this trusted container.
   * \param[in] content_props Meta data about the payload.
   * \vprivate Product Private
   */
  virtual void UpdateContentProps(ara::crypto::keys::KeySlotContentProps const& content_props) noexcept = 0;

  /*!
   * \brief Update the ContentProps data of this trusted container.
   * \param[in] key Key to store.
   * \vprivate Product Private
   */
  virtual void UpdateContentProps(ara::crypto::cryp::Key const& key) noexcept = 0;

  /*!
   * \brief Indicates if the payload of this trusted container is set.
   * \return True if the payload of this trusted container is set.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual bool IsEmpty() const noexcept = 0;

  /*!
   * \brief Capacity of a single keyslot in bytes
   * \vprivate Product Private
   */
  constexpr static std::uint32_t kKeySlotCapacityBytes{20000};

  /*!
   * \brief Getter for the custom payload in this TrustedContainer
   * \return A view of the custom payload. May be empty.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto GetContentPayload() const noexcept -> ara::crypto::ReadOnlyMemRegion = 0;

  /*!
   * \brief Setter for the custom payload in this TrustedContainer
   * \param[in] blob The custom payload to set
   * \error ara::crypto::SecurityErrc::kInsufficientResource If the containers capacity is smaller than \c blob and
   * non-adaptive.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto UpdateContentPayload(ara::crypto::ReadOnlyMemRegion const& blob) noexcept
      -> amsr::core::Result<void> = 0;
};

}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TRUSTED_CONTAINER_H_
