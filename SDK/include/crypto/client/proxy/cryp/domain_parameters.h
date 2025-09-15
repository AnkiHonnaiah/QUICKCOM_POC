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
/*!        \file  proxy/cryp/domain_parameters.h
 *        \brief  Proxy implementation for ara::crypto::cryp::DomainParameters.
 *
 *      \details  Client side implementation of the ara::crypto::cryp::DomainParameters interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_DOMAIN_PARAMETERS_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_DOMAIN_PARAMETERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace cryp {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
// VECTOR NC AutosarC++17_10-M10.2.1: MD_CRYPTO_AutosarC++17_10-M10.2.1_unique_names
// VECTOR NC Metric-OO.WMC.One: MD_CRYPTO_Metric-OO.WMC.One
/*!
 * \brief Client side proxy implementation for ara::crypto::cryp::DomainParameters
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class DomainParameters : public crypto::client::BaseProxy, public ara::crypto::cryp::DomainParameters {
 public:
  /*!
   * \brief Unique constant smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Uptrc = std::unique_ptr<DomainParameters const, ara::crypto::CustomDeleter>;

  /*!
   * \brief Unique smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Sptr = std::shared_ptr<DomainParameters>;

  /*!
   * \brief Unique const smart pointer of the proxy.
   * \vprivate Component Private
   */
  using Sptrc = std::shared_ptr<DomainParameters const>;

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] provider_reference    Reference to the creating crypto provider
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  DomainParameters(ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
                   amsr::sec::ipc::MessageBuilder& message_builder,
                   amsr::sec::ipc::MessageSerializer& message_serializer,
                   amsr::sec::ipc::MessageDeserializer& message_deserializer);

  /*!
   * \brief Create function for instance generation without access to new/delete operator (which are protected)
   * \param[in] provider_reference Reference to the creating crypto provider
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
  static std::unique_ptr<DomainParameters, ara::crypto::CustomDeleter> Create(
      ara::crypto::cryp::CryptoProvider& provider_reference, amsr::sec::ipc::Transporter& transporter,
      amsr::sec::ipc::MessageBuilder& message_builder, amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) noexcept {
    // VECTOR NC AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<DomainParameters, ara::crypto::CustomDeleter>(new DomainParameters(
        provider_reference, transporter, message_builder, message_serializer, message_deserializer));
  }

  /*!
   * \brief Get the type of this object.
   * \return One of object types except Type::UNKNOWN.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Type GetObjectType() const noexcept override;

  /*!
   * \brief Returns the "session" (or "temporary") attribute of the object.
   * \details A temporary object cannot be saved to a non-volatile trusted container!
   * \details A temporary object will be securely destroyed together with this interface instance!
   * \details A non-session object must have an assigned COUID (see GetObjectId()).
   * \return true if the object is temporary (i.e. its life time is limited by the current session only).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsSession() const noexcept override;

  /*!
   * \brief Returns the "exportable" attribute of the object.
   * \return true if the object is exportable (i.e. if it can be exported
   *         outside the trusted environment of the Crypto Provider).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsExportable() const noexcept override;

  /*!
   * \brief Returns the object's UID if it is assigned to the object.
   * \param[out] object_id pointer to a buffer for the object's UID saving.
   * \return true if the object has assigned COUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool GetObjectId(ara::crypto::CryptoObjectUid* object_id) const noexcept override;

  /*!
   * \brief Returns an identifier of an object from which depends the current one.
   * \param[out] object_id pointer to a buffer for the target object UID.
   * \return Target object type for existing dependence or Type::kNone
   *         if the current object doesn't depend from any other one.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Type HasDependence(ara::crypto::CryptoObjectUid* object_id) const noexcept override;

  /*!
   * \brief Returns a storage size of the object.
   * \return Size in bytes of the objects required for its storage.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t StorageSize() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  /*!
   * \brief Save to provided trusted container.
   * \param[in] container A target trusted container.
   * \param[in] referenced A pointer to another trusted container that keeps referenced crypto object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> Save(ara::crypto::TrustedContainer& container,
                               ara::crypto::TrustedContainer* referenced = nullptr) const noexcept override;

  /*!
   * \brief Get secrecy attribute of these parameters set.
   * \return true if the set of these parameters is secret.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsSecret() const noexcept override;

  /*!
   * \brief Returns number of supported parameters.
   * \return Number of supported parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetParametersCount() const noexcept override;

  /*!
   * \brief Returns maximal length between all names of this domain parameters.
   * \return Maximal length between all parameters' names.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::size_t GetMaxParameterNameLength() const noexcept override;

  /*!
   * \brief Returns the parameter name by its index.
   * \param[in] index Index of a parameter, name of which is requested.
   *                  It should be less than GetParametersCount().
   * \param[out] name A pointer to a string where the parameter name should be saved.
   * \return Actual length of the parameter name.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity if (name != nullptr), but
   *                the \c name->capacity() is not enough to store the parameter name.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetParameterName(std::size_t index, ara::core::String* name) const noexcept override;

  /*!
   * \brief Gets expected size of specific parameter of the Crypto Primitive.
   * \param[in] index Index of a parameter that should be set. It should be less than GetParametersCount().
   * \return Expected (presize or maximal) size of the parameter defined by the index.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier  if the \c index argument is incorrect.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExpectedParameterSize(std::size_t index) const noexcept override;

  /*!
   *
   * \brief Sets a value to specific parameter of the Crypto Primitive.
   * \param[in] index Index of a parameter, which should be set. It should be less than GetParametersCount().
   * \param[in] value New value of the parameter that should be set.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetParameter(std::size_t index, ara::crypto::ReadOnlyMemRegion value) noexcept override;

  /*!
   * \brief Checks completeness and consistency of this parameters set.
   * \return true if this set of domain parameters is completed and false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool IsCompleted() const noexcept override;

  /*!
   * \brief Checks completeness and consistency of this parameters set.
   * \param[in] allowed_usage The allowed usages for the domain parameters.
   * \return true If this set of domain parameters is completed and "false" otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool Complete(Usage allowed_usage) noexcept override;

  /*!
   * \brief Returns a unique "well-known" name of the parameters set.
   * \return Unique name of the Domain Parameters set or the empty string if a unique name is not assigned.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::StringView GetUniqueName() const noexcept override;

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
  void Release() noexcept override;

  /*!
   * \brief Get a unified name of the primitive.
   * \return The unified name of the crypto primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::StringView const GetPrimitiveName() const noexcept override;

  /*!
   * \brief Get the primitive ID.
   * \return The binary Crypto Primitive ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  AlgId GetPrimitiveId() const noexcept override;

  /*!
   * \brief Get the category of the crypto primitive.
   * \return The category of the crypto primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Category GetCategory() const noexcept override;

  /*!
   * \brief Get the crypto provider of the hash context.
   * \return A reference to the crypto provider of the domain parameters.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::CryptoProvider& MyProvider() const noexcept override;

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is interested).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Method not supported.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportPublicly(
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id =
          ara::crypto::Serializable::kFormatDefault) const noexcept override;

  /*!
   * \brief Get allowed usages of this object.
   * \return A combination of bit-flags that specifies allowed applications of the object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::crypto::cryp::RestrictedUseObject::Usage GetAllowedUsage() const noexcept override;

 private:
  ara::crypto::cryp::CryptoProvider& provider_reference_; /*!< Reference to the creating provider */
  mutable ara::core::String my_name_;                     /*!< Memory for the name of this primitive */
  mutable ara::core::String my_unique_name_;              /*!< Memory for the unique name */
};

}  // namespace cryp
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_CRYP_DOMAIN_PARAMETERS_H_
