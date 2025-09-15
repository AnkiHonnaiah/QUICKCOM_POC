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
/*!        \file  ara/crypto/cryp/crypto_object.h
 *        \brief  A common interface for all cryptographic objects recognizable by the Crypto Provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_OBJECT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_OBJECT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/cryp/crypto_primitive_id.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief A common interface for all cryptographic objects recognizable by the Crypto Provider.
 * \details This interface (or any its derivative) represents a non-mutable (after completion) object
 *          loadable to a temporary transformation context.
 * \vpublic
 */
class CryptoObject : public CryptoPrimitiveId {
 protected:
  /*!
   *  \brief Check for equality this object type and needed one and return correspondent error code structured type.
   *  \param[in] needed_type  the needed type of expected \c CryptoObject
   *  \return complete error code structure initialized by \c SecurityErrc::kBadObjectType if actual type of
   *          this object is non-equal to the specified type
   *          or initialized by \c SecurityErrc::kNoError if they are equal
   *  \note This method informs the Crypto Stack that the exception is not related to a crypto object loading.
   *  \error SecurityErrorDomain::kBadObjectType if (needed_type != GetObjectType())
   *  \context ANY
   *  \pre -
   *  \threadsafe FALSE
   *  \reentrant FALSE
   *  \synchronous TRUE
   *  \vprivate Component Private
   */
  /*!
   * \internal
   * - Get the crypto object's type
   * - If the crypto object's type is the same as the given type
   *   - Return SecurityErrc::kNoError
   * - Else
   *   - Return SecurityErrc::kBadObjectType
   * \endinternal
   */
  ara::core::ErrorCode CheckObjectType(CryptoObjectType needed_type) const noexcept {
    CryptoObjectType const my_obj_type{GetObjectType()};
    ara::core::ErrorCode error_code{MakeErrorCode(SecurityErrc::kNoError, 0)};

    if (my_obj_type != needed_type) {
      static_assert(sizeof(ara::core::ErrorDomain::SupportDataType::CodeType) == sizeof(std::uint32_t),
                    "Cannot use std::uint32_t to represent SupportDataType.");

      // VECTOR NL AutosarC++17_10-M5.0.9: MS_CRYPTO_AutosarC++17_10-M5.0.9_only_bin_operation_on_signed_types
      ara::core::ErrorDomain::SupportDataType const data{static_cast<ara::core::ErrorDomain::SupportDataType::CodeType>(
          (static_cast<std::uint32_t>(my_obj_type) << static_cast<std::uint32_t>(16)) |
          static_cast<std::uint32_t>(needed_type))};
      error_code = MakeErrorCode(SecurityErrc::kBadObjectType, data);
    }

    return error_code;
  }

 public:
  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<CryptoObject, CustomDeleter>;

  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Unique smart pointer of the constant interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<CryptoObject const, CustomDeleter>;

  /*!
   * \brief Enumeration of all types of crypto objects.
   * \vpublic
   */
  using Type = CryptoObjectType;

  /*!
   * \brief Returns the type of this object.
   * \return One of object types except Type::UNKNOWN.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Type GetObjectType() const noexcept = 0;

  /*!
   * \brief Returns the "session" (or "temporary") attribute of the object.
   * \details A temporary object cannot be saved to a non-volatile trusted container!
   * \details A temporary object will be securely destroyed together with this interface instance!
   * \details A non-session object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is temporary (i.e. its life time is limited by the current session only).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsSession() const noexcept = 0;

  /*!
   * \brief Returns the "exportable" attribute of the object.
   * \details An exportable object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is exportable (i.e. if it can be exported
   *         outside the trusted environment of the Crypto Provider).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsExportable() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Returns the object's UID if it is assigned to the object.
   * \details An object that has no an assigned COUID cannot be (securely)
   *          serialized / exported or saved to a non-volatile container.
   * \details An object should not have a COUID if it is session and non-exportable
   *          simultaneously or if it is incomplete yet (last is applicable for domain parameters only).
   * \details A few related objects of different types can share a single COUID (e.g. private and public keys),
   *          but a combination of COUID and object type must be unique always!
   * \param[out] object_id Optional pointer to a buffer for the object's UID saving.
   * \return true if the object has assigned COUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool GetObjectId(CryptoObjectUid* object_id = nullptr) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Returns an identifier of an object from which depends the current one.
   * \details If (object_id != nullptr) but the current object has no
   *          dependence from other one then the object_id should be filled by zeros.
   * \details For signatures objects this method must return a reference to correspondent
   *          signature verification public key!
   * \details For keys objects this method should return a reference to domain parameters.
   * \details For domain parameters object this method (optionally) can return a reference
   *          to another domain parameters required for this one.
   * \param[out] object_id Optional pointer to a buffer for the target object UID.
   * \return Target object type for existing dependence or Type::UNKNOWN
   *         if the current object doesn't depend from any other one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Type HasDependence(CryptoObjectUid* object_id = nullptr) const noexcept = 0;

  /*!
   * \brief Returns a storage size of the object.
   * \return Size in bytes of the objects required for its storage.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t StorageSize() const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_autosar_api
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Saves itself to provided trusted container.
   * \details An object can be saved to a container only if the container is empty and has enough capacity.
   * \details The save operation is allowed to an empty container only, i.e. any updates of objects are prohibited!
   * \details Only a non-session/non-temporary and completed object (i.e. that have a COUID) can be saved!
   * \details Only a single instance of an object is allowed in the persistent storage! Any object is uniquely
   *          identified by the combination of its COUID and type.
   * \param[in] container A target trusted container.
   * \param[in] referenced A pointer to another trusted container that keeps referenced crypto object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kAccessViolation  if the target or the referenced non-volatile containers
   *         were opened "for usage", i.e. if (!container.HasOwnership() || !referenced->HasOwnership())
   * \error SecurityErrorDomain::kIncompatibleObject  if the object is "session", but the container
   * \error SecurityErrorDomain::kContentRestrictions  if the object doesn't satisfy the slot restrictions \c
   *         keys::KeySlotPrototypeProps)
   * \error SecurityErrorDomain::kInsufficientCapacity  if the capacity of the target container is not enough, i.e. if
   *         (container.Capacity() < this->StorageSize())
   * \error SecurityErrorDomain::kBadObjectReference
   *   - if (referenced != nullptr), but this crypto object doesn't support referencing to other objects;
   *   - if (referenced != nullptr), but an object in referenced container cannot be referenced;
   *   - if (referenced != nullptr), but this crypto object and referenced one have incompatible for
   *     referencing types and/or algorithms;
   *   - if (referenced != nullptr) and \c referenced is volatile, but \c container is non-volatile
   * \error SecurityErrorDomain::kEmptyContainer
   *   - if (referenced != nullptr), but the referenced trusted container is empty
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<void> Save(TrustedContainer& container,
                                       TrustedContainer* referenced = nullptr) const noexcept = 0;

  /*!
   * \brief Downcast and move unique smart pointer from the generic \c CryptoObject interface to concrete derived
   * object.
   * \tparam ConcreteObject target type (derived from \c CryptoObject) for downcasting
   * \param[in] object unique smart pointer to the constant generic \c CryptoObject interface
   * \return unique smart pointer to downcasted constant interface of specified derived type
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrorDomain::kBadObjectType  if an actual type of the \c object is not the specified \c
   * ConcreteObject
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Check if the given pointer is not nullptr
   * - Check if the object's type matchs the target concrete object type
   * - Down cast the pointer to the concrete type and return it
   * \endinternal
   */
  template <class ConcreteObject>
  static auto Downcast(CryptoObject::Uptrc&& object) noexcept -> ara::core::Result<typename ConcreteObject::Uptrc> {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using DstValType = typename ConcreteObject::Uptrc;
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using DstResType = ara::core::Result<DstValType>;

    ara::core::Result<typename ConcreteObject::Uptrc> result{nullptr};

    if (object != nullptr) {
      ara::core::ErrorCode error{object->CheckObjectType(ConcreteObject::sObjectType)};

      if (error != SecurityErrc::kNoError) {
        result = DstResType::FromError(error);
      }
    }

    if (result.HasValue()) {
      // VECTOR NC AutosarC++17_10-M5.2.3: MD_CRYPTO_AutosarC++17_10-M5.2.3_down_cast
      result.EmplaceValue(DstValType{static_cast<ConcreteObject const*>(object.release())});
    }

    return result;
  }
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_OBJECT_H_
