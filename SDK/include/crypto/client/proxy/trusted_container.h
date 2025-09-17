// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  proxy/trusted_container.h
 *        \brief  Proxy implementation for ara::crypto::TrustedContainer.
 *
 *      \details  Client side implementation of the ara::crypto::TrustedContainer interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_TRUSTED_CONTAINER_IMPL_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_TRUSTED_CONTAINER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/crypto/common/trusted_container.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client side proxy implementation for ara::crypto::TrustedContainer
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class TrustedContainer : public BaseProxy, public ara::crypto::TrustedContainer {
 public:
  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptr = std::unique_ptr<TrustedContainer, ara::crypto::CustomDeleter>;

  /*!
   * \brief Inherit base class constructor.
   * \vprivate Component Private
   */
  using BaseProxy::BaseProxy;

  /*!
   * \brief Create function for instance generation without access to new/delete operator (which are protected)
   * \param[in] transporter Transporter used for server/client communication
   * \param[in] message_builder Builder for message generation
   * \param[in] message_serializer Serializer for request serialization
   * \param[in] message_deserializer Deserializer for response deserialization
   * \return Unique pointer to the newly created object
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static std::unique_ptr<TrustedContainer, ara::crypto::CustomDeleter> Create(
      amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
      amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<TrustedContainer, ara::crypto::CustomDeleter>(
        // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
        new TrustedContainer(transporter, message_builder, message_serializer, message_deserializer));
  }

  // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
  /*!
   * \brief Call the destructor and release the memory.
   * \details May be used only if the object's memory was allocated using the new operator.
              The object must not be used once the method is called.
   * \context ANY
   * \pre The object's memory is allocated with the new operator.
          The memory is not released.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Release() noexcept override { delete this; }

  /*!
   * \brief Returns COUID and type of an object stored to this trusted container.
   * \param[out] object_uid An optional pointer to buffer for getting COUID of an object stored in the container.
   * \return Type of the content stored in the container.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ContentType GetObjectId(ara::crypto::CryptoObjectUid* object_uid) const noexcept override;

  /*!
   * \brief Returns COUID and type of an object from which current object (in the container) depends.
   * \param[out] object_uid An optional pointer to buffer for getting COUID an object from which current object depends.
   * \return Type of an object from which current one depends or ContentType::kNone if it has no any dependencies.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
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
   * \vprivate Component Private
   */
  std::size_t Capacity() const noexcept override;

  /*!
   * \brief Return volatility of the trusted container.
   * \details A "session" object can be stored to a "volatile" container only!
   * \details A content of a "volatile" container will be destroyed together with the interface instance!
   * \return true if the container has a volatile nature (i.e. "temporary" or "in RAM") or false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsVolatile() const noexcept override;

  /*!
   * \brief Return the "session" (or "temporary") attribute of an object stored to the container.
   * \details A "session" object can be stored to a "volatile" container only!
   * \details If a "volatile" container keeps a non-session object then it can be saved permanently.
   * \return true if an object stored to the container has set the "session" attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsObjectSession() const noexcept override;

  /*!
   * \brief Return the "exportable" attribute of an object stored to the container.
   * \details The exportability of an object doesn't depend from the volatility of its container.
   * \return true if an object stored to the container has set the "exportable" attribute.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsObjectExportable() const noexcept override;

  /*!
   * \brief Return size of an object stored to the trusted container.
   * \details If the container is empty then this method returns 0.
   * \return Size of an object stored to the trusted container (in bytes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
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
   * \vprivate Component Private
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
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::AllowedUsageFlags AllowedUsage() const noexcept override;

  /*!
   * \brief Return current number of external references to a crypto object kept in the container.
   * \return References counter to an object stored in the container.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetReferencesCounter() const noexcept override;

  /*!
   * \brief Check the ownership status of the current process on this trusted container.
   * \details A saving operation to the container can be done only if (IsReadOnly() == false)!
   * \return true if the container is owned by this process (always true for volatile containers), and
   *         false otherwise (the current process has only User rights on the container).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
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
   * \vprivate Component Private
   */
  auto GetLogicalContainerUid() const noexcept -> ara::crypto::Uuid override;
};
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_TRUSTED_CONTAINER_IMPL_H_
