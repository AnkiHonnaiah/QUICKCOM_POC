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
/*!        \file  common/trusted_container_impl.h
 *        \brief  AMSR CryptoProvider TrustedContainer implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TRUSTED_CONTAINER_IMPL_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TRUSTED_CONTAINER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/optional.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/key.h"
#include "crypto/common/trusted_container.h"
#include "crypto/common/util/observable.h"

namespace crypto {
namespace common {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_false_positive
// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief AMSR CryptoProvider TrustedContainer implementation
 * \trace CREQ-AraCrypto-Common_CryptographicMaterialContainer
 * \vpublic
 */
class TrustedContainerImpl : public crypto::common::TrustedContainer {
 public:
  /*!
   * \brief UniquePtr for this TrustedContainer
   * \vpublic
   */
  using Uptr = std::unique_ptr<crypto::common::TrustedContainerImpl, ara::crypto::CustomDeleter>;

  /*!
   * \brief UniquePtr for this const TrustedContainer
   * \vpublic
   */
  using Uptrc = std::unique_ptr<crypto::common::TrustedContainerImpl const, ara::crypto::CustomDeleter>;
  /*!
   * \brief Deleted copy constructor
   * \vprivate Product Private
   */
  TrustedContainerImpl(TrustedContainerImpl const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Product Private
   */
  TrustedContainerImpl(TrustedContainerImpl&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Product Private
   */
  TrustedContainerImpl& operator=(TrustedContainerImpl const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \vprivate Product Private
   */
  TrustedContainerImpl& operator=(TrustedContainerImpl&& /*other*/) noexcept = delete;

  /*!
   * \brief Destructor
   * \vprivate Product Private
   */
  ~TrustedContainerImpl() noexcept override;

  /*!
   * \brief Creation method for a TrustedContainer without content.
   * \param[in] capacity Maximum capacity of the newly created TrustedContainer (0 means adaptive capacity).
   * \param[in] content_props Meta data about the payload
   * \param[in] content_payload The payload itself
   * \param[in] uuid The Uuid of this TrustedContainer
   * \param[in] is_volatile marks the TrustedContainer as temporary
   * \param[in] is_owner marks the TrustedContainer as the owner of the underlying key slot
   * \return Unique pointer to newly created, empty trusted container
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static Uptr Create(std::size_t capacity, ara::crypto::keys::KeySlotContentProps const& content_props,
                     ara::crypto::ReadOnlyMemRegion content_payload, ara::crypto::Uuid uuid, bool is_volatile = false,
                     bool is_owner = false) noexcept {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_call_allocate_leaking_memory
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_call_allocate_leaking_memory
    return Uptr(new TrustedContainerImpl(capacity, content_props, content_payload, uuid, is_volatile, is_owner));
  }

  /*!
   * \brief Creation method for a TrustedContainer without content.
   * \param[in] capacity Maximum capacity of the newly created TrustedContainer (0 means adaptive capacity).
   * \param[in] content_props Meta data about the payload
   * \param[in] uuid The Uuid of this TrustedContainer
   * \param[in] is_volatile marks the TrustedContainer as temporary
   * \param[in] is_owner marks the TrustedContainer as the owner of the underlying key slot
   * \return Unique pointer to newly created, empty trusted container
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static Uptr Create(std::size_t capacity, ara::crypto::keys::KeySlotContentProps const& content_props,
                     ara::crypto::Uuid uuid, bool is_volatile = false, bool is_owner = false) noexcept {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_call_allocate_leaking_memory
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_call_allocate_leaking_memory
    return Uptr(new TrustedContainerImpl(capacity, content_props, uuid, is_volatile, is_owner));
  }

  // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
  /*!
   * \brief Creator function for generation of new unique_ptr for this trusted Container.
   * \param[in] capacity Maximum capacity of the newly created TrustedContainer (0 means adaptive capacity).
   * \param[in] uuid The Uuid of this TrustedContainer
   * \param[in] is_volatile marks the TrustedContainer as temporary
   * \param[in] is_owner marks the TrustedContainer as the owner of the underlying key slot
   * \return unique_ptr of the new container
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static Uptr Create(std::size_t capacity, ara::crypto::Uuid uuid, bool is_volatile = false,
                     bool is_owner = false) noexcept {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_call_allocate_leaking_memory
    return Uptr(new TrustedContainerImpl(capacity, uuid, is_volatile, is_owner));
  }

  /*!
   * \brief Release allocated memory and other resources.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void Release() noexcept override { delete this; };

  /*!
   * \brief Returns COUID and type of an object stored to this trusted container.
   * \details If the container is empty then this method returns ContentType::kNone.
   * \details If (object_uid != nullptr), but the container is empty then the object_uid will be filled by all zeros.
   * \details Unambiguous identification of a crypto object requires both components: CryptoObjectUid and ContentType.
   * \details A caller code may omit the optional argument if only the content type is interested.
   * \param[out] object_uid An optional pointer to buffer for getting COUID of an object stored in the container.
   * \return Type of the content stored in the container.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  ContentType GetObjectId(ara::crypto::CryptoObjectUid* object_uid) const noexcept override;

  /*!
   * \brief Returns COUID and type of an object from which current object (in the container) depends.
   * \details If the object doesn't depend from (or refer to) another
   *          object then the object_id will be filled by all zeros.
   * \details Unambiguous identification of a crypto object requires both components: CryptoObjectUid and ContentType.
   * \details A caller code may omit the optional argument if only the dependency type is interested.
   * \param[out] object_uid An optional pointer to buffer for getting COUID an object from which current object depends.
   * \return Type of an object from which current one depends or ContentType::kNone if it has no any dependencies.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  ContentType GetDependenceId(ara::crypto::CryptoObjectUid* object_uid) const noexcept override;

  /*!
   * \brief Return capacity of the trusted container.
   * \return Capacity of the trusted container (in bytes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  std::size_t Capacity() const noexcept override;

  /*!
   * \brief Return volatility of the trusted container.
   * \details A "session" object can be stored to a "volatile" container only!
   * \details A content of a "volatile" container will be destroyed together with the interface instance!
   * \return true if the container has a volatile nature (i.e. "temporary" or "in RAM") or false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  bool IsVolatile() const noexcept override;

  /*!
   * \brief Return the "session" (or "temporary") attribute of an object stored to the container.
   * \details A "session" object can be stored to a "volatile" container only!
   * \details If a "volatile" container keeps a non-session object then it can be saved permanently.
   * \return true if an object stored to the container has set the "session" attribute.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  bool IsObjectSession() const noexcept override;

  /*!
   * \brief Return the "exportable" attribute of an object stored to the container.
   * \details The exportability of an object doesn't depend from the volatility of its container.
   * \return true if an object stored to the container has set the "exportable" attribute.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  bool IsObjectExportable() const noexcept override;

  /*!
   * \brief Return size of an object stored to the trusted container.
   * \details If the container is empty then this method returns 0.
   * \return Size of an object stored to the trusted container (in bytes).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  std::size_t ObjectSize() const noexcept override;

  /*!
   * \brief Return content type restriction of this trusted container.
   * \details If a container has a type restriction different from ContentType::kNone
   *          then only objects of the mentioned type can be saved to this container.
   * \details Volatile containers don't have any content type restrictions.
   * \return An object type of allowed content (ContentType::kNone means without restriction).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  ContentType TypeRestriction() const noexcept override;

  /*!
   * \brief Returns actual allowed key/seed usage flags defined by the key slot prototype
   *        for this Actor (application) and current content of the container.
   * \details Volatile containers don't have any prototyped restrictions, but can have
   *          restrictions defined at run-time for a current instance of object.
   * \details A value returned by this method is bitwise AND of the common usage flags defined at run-time
   *          and the usage flags defined by the UserPermissions prototype for current Actor (application).
   * \details This method is especially useful for empty permanent prototyped containers!
   * \return Allowed key/seed usage flags.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  ara::crypto::AllowedUsageFlags AllowedUsage() const noexcept override;

  /*!
   * \brief Return current number of external references to a crypto object kept in the container.
   * \return References counter to an object stored in the container.
   * \details Function not yet implemented.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  std::size_t GetReferencesCounter() const noexcept override;

  /*!
   * \brief Check the ownership status of the current process on this trusted container.
   * \details A saving operation to the container can be done only if (IsReadOnly() == false)!
   * \return true if the container is owned by this process (always true for volatile containers), and
   *         false otherwise (the current process has only User rights on the container).
   * \details Function not yet implemented.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  bool HasOwnership() const noexcept override;

  /*!
   * \brief Get the logical identifier for this container.
   * \details For non-volatile slots (i.e. \c IsVolatile() is \c false) this is equal to the \c mLogicalSlotUid property
   * of the corresponding KeySlotPrototypeProps.
   * \return A UUID uniquely identifying this container.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto GetLogicalContainerUid() const noexcept -> ara::crypto::Uuid override;

  /*!
   * \brief Get the key slot content properties of this trusted container
   * \return KeySlotContentProps of this trusted container
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Result<ara::crypto::keys::KeySlotContentProps> GetKeySlotContentProps() const noexcept override;

  /*!
   * \brief Marks the trusted container volatility.
   * \param[in] is_volatile Volatility.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void SetVolatile(bool is_volatile) noexcept;

  /*!
   * \copydoc TrustedContainer::IsEmpty
   * \vprivate Product Private
   */
  bool IsEmpty() const noexcept override;

  /*!
   * \brief Setter for the private member variable to be used by the deserialization function
   * \param[in] session_flag new value
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void SetSessionFlag(bool session_flag) noexcept;

  /*!
   * \brief Setter for the private member variable to be used by the deserialization function
   * \param[in] content_props Meta data about the payload.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void UpdateContentProps(ara::crypto::keys::KeySlotContentProps const& content_props) noexcept override;

  /*!
   * \brief Setter for the private member variable to be used by the deserialization function
   * \param[in] key Key to store.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void UpdateContentProps(ara::crypto::cryp::Key const& key) noexcept override;

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
  auto GetContentPayload() const noexcept -> ara::crypto::ReadOnlyMemRegion override;

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
  auto UpdateContentPayload(ara::crypto::ReadOnlyMemRegion const& blob) noexcept -> amsr::core::Result<void> override;

 protected:
  /*!
   * \brief Constructor for TrustedContainer with content, with uuid
   * \param[in] capacity Capacity of the TrustedContainer (0 means adaptive capacity).
   * \param[in] content_props KeySlot Content properties of this container
   * \param[in] content_payload KeySlot Content payload of this container
   * \param[in] uuid Uuid to identify the container with a slot
   * \param[in] is_volatile marks the TrustedContainer as temporary
   * \param[in] is_owner marks the TrustedContainer as the owner of the underlying key slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  TrustedContainerImpl(std::size_t capacity, amsr::core::Optional<ara::crypto::keys::KeySlotContentProps> content_props,
                       ara::crypto::ReadOnlyMemRegion content_payload, ara::crypto::Uuid uuid, bool is_volatile,
                       bool is_owner) noexcept;

  /*!
   * \brief Constructor for TrustedContainer with content, with uuid
   * \param[in] capacity Capacity of the TrustedContainer (0 means adaptive capacity).
   * \param[in] content_props KeySlot Content properties of this container
   * \param[in] uuid Uuid to identify the container with a slot
   * \param[in] is_volatile marks the TrustedContainer as temporary
   * \param[in] is_owner marks the TrustedContainer as the owner of the underlying key slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  TrustedContainerImpl(std::size_t capacity, amsr::core::Optional<ara::crypto::keys::KeySlotContentProps> content_props,
                       ara::crypto::Uuid uuid, bool is_volatile, bool is_owner) noexcept;

  /*!
   * \brief Constructor for TrustedContainer without content, with uuid
   * \param[in] capacity The capacity of the trusted container to construct (0 means adaptive capacity).
   * \param[in] uuid Uuid to identify the container with a slot
   * \param[in] is_volatile marks the TrustedContainer as temporary
   * \param[in] is_owner marks the TrustedContainer as the owner of the underlying key slot
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  explicit TrustedContainerImpl(std::size_t capacity, ara::crypto::Uuid uuid, bool is_volatile, bool is_owner) noexcept;

 private:
  amsr::core::Optional<ara::crypto::keys::KeySlotContentProps> content_props_; /*!< Slot content metadata. */
  ara::core::Vector<ara::crypto::Byte> content_payload_;                       /*!< Additional Slot content payload. */

  bool session_flag_{false};  /*!< Indicates if the TC is a session object. */
  bool volatile_flag_{false}; /*!< Indicates if the TC is volatile. */
  std::size_t capacity_;      /*!< Capacity of the TC. */
  bool capacity_adaptive_;    /*!< Capacity of the TC is adaptive. */
  ara::crypto::Uuid uuid_;    /*!< Uuid identifying the KeySlot */
  bool has_ownership_;        /*!< Indicates if the TC was created by Opening as Slot as Owner */
};

}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_TRUSTED_CONTAINER_IMPL_H_
