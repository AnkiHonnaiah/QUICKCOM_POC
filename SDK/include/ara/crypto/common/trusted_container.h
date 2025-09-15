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
/*!        \file  ara/crypto/common/trusted_container.h
 *        \brief  Formal interface of a Trusted Container is used for saving and loading of security objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_TRUSTED_CONTAINER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_TRUSTED_CONTAINER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/custom_disposable.h"

namespace ara {
namespace crypto {

/*!
 * \brief Formal interface of a Trusted Container is used for saving and loading of security objects.
 * \details Actual saving and loading should be implemented by internal methods
 *          known to a trusted pair of Crypto Provider and Storage Provider.
 * \details Each object should be uniquely identified by its type and Crypto Object Unique Identifier (COUID).
 * \details This interface supposes that objects in the container are compressed
 *          i.e. have a minimal size optimized for saving or transferring.
 * \vpublic
 */
class TrustedContainer : public CustomDisposable {
 protected:
  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~TrustedContainer() noexcept override = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  TrustedContainer(TrustedContainer&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  TrustedContainer() = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  TrustedContainer(TrustedContainer const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  TrustedContainer& operator=(TrustedContainer&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment constructor
   * \vprivate Component Private
   */
  TrustedContainer& operator=(TrustedContainer const& /*other*/) = delete;

  /*!
   * \brief Unique smart pointer of the interface.
   * \vpublic
   */
  using Uptr = std::unique_ptr<TrustedContainer, CustomDeleter>;

  /*!
   * \brief Unique smart pointer of the constant interface.
   * \vpublic
   */
  using Uptrc = std::unique_ptr<TrustedContainer const, CustomDeleter>;

  /*!
   * \brief Content Type of the Trusted Container.
   * \vpublic
   */
  using ContentType = CryptoObjectType;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ContentType GetObjectId(CryptoObjectUid* object_uid = nullptr) const noexcept = 0;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
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
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ContentType GetDependenceId(CryptoObjectUid* object_uid = nullptr) const noexcept = 0;

  /*!
   * \brief Return capacity of the trusted container.
   * \return Capacity of the trusted container (in bytes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t Capacity() const noexcept = 0;

  /*!
   * \brief Return volatility of the trusted container.
   * \details A "session" object can be stored to a "volatile" container only!
   * \details A content of a "volatile" container will be destroyed together with the interface instance!
   * \return true if the container has a volatile nature (i.e. "temporary" or "in RAM") or false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsVolatile() const noexcept = 0;

  /*!
   * \brief Return the "session" (or "temporary") attribute of an object stored to the container.
   * \details A "session" object can be stored to a "volatile" container only!
   * \details If a "volatile" container keeps a non-session object then it can be saved permanently.
   * \return true if an object stored to the container has set the "session" attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsObjectSession() const noexcept = 0;

  /*!
   * \brief Return the "exportable" attribute of an object stored to the container.
   * \details The exportability of an object doesn't depend from the volatility of its container.
   * \return true if an object stored to the container has set the "exportable" attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool IsObjectExportable() const noexcept = 0;

  /*!
   * \brief Return size of an object stored to the trusted container.
   * \details If the container is empty then this method returns 0.
   * \return Size of an object stored to the trusted container (in bytes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t ObjectSize() const noexcept = 0;

  /*!
   * \brief Return content type restriction of this trusted container.
   * \details If a container has a type restriction different from ContentType::kNone
   *          then only objects of the mentioned type can be saved to this container.
   * \details Volatile containers don't have any content type restrictions.
   * \return An object type of allowed content (ContentType::kNone means without restriction).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ContentType TypeRestriction() const noexcept = 0;

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
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual AllowedUsageFlags AllowedUsage() const noexcept = 0;

  /*!
   * \brief Return current number of external references to a crypto object kept in the container.
   * \return References counter to an object stored in the container.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual std::size_t GetReferencesCounter() const noexcept = 0;

  /*!
   * \brief Check the ownership status of the current process on this trusted container.
   * \details A saving operation to the container can be done only if (IsReadOnly() == false)!
   * \return true if the container is owned by this process (always true for volatile containers), and
   *         false otherwise (the current process has only User rights on the container).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual bool HasOwnership() const noexcept = 0;

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
  virtual auto GetLogicalContainerUid() const noexcept -> Uuid = 0;
};

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_TRUSTED_CONTAINER_H_
