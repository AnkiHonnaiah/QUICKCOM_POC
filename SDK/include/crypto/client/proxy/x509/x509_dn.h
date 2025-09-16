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
/*!        \file  proxy/x509/x509_dn.h
 *        \brief  Proxy implementation for ara::crypto::x509::X509DN.
 *
 *      \details  Client side implementation of the ara::crypto::x509::X509DN interface that
 *                transparently communicates with a server side implementation that performs the real cryptography.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_X509_DN_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_X509_DN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/core/string_view.h"
#include "ara/crypto/x509/x509_dn.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace x509 {

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client-side implementation for the proxy of ara::crypto::x509::X509DN.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class X509DN final : public BaseProxy, public ara::crypto::x509::X509DN {
 public:
  /*!
   * \brief Constructor
   * \param[in] transporter Transporter used for server/client communication
   * \param[in] message_builder Builder for message generation
   * \param[in] message_serializer Serializer for request serialization
   * \param[in] message_deserializer Deserializer for response deserialization
   * \vprivate Component Private
   */
  explicit X509DN(amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
                  amsr::sec::ipc::MessageSerializer& message_serializer,
                  amsr::sec::ipc::MessageDeserializer& message_deserializer)
      : BaseProxy{transporter, message_builder, message_serializer, message_deserializer},
        ara::crypto::x509::X509DN(),
        message_serializer_{message_serializer},
        message_deserializer_{message_deserializer} {}

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
  static std::unique_ptr<X509DN, ara::crypto::CustomDeleter> Create(
      amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
      amsr::sec::ipc::MessageSerializer& message_serializer, amsr::sec::ipc::MessageDeserializer& message_deserializer);

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~X509DN() noexcept final = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  X509DN(X509DN const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  X509DN(X509DN&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509DN& operator=(X509DN const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509DN& operator=(X509DN&& /*other*/) = delete;

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
  void Release() noexcept final;

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
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat The format is not supported.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer is too small to hold the encoded DN.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> ExportPublicly(
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept final;

  /*!
   * \brief Get the whole Distinguished Name (DN) as a single string.
   * \details Capacity of the output string must be enough for storing the output value!
   * \details If (dn == nullptr) then method only returns required buffer capacity.
   * \param[out] dn  the pointer to a string for storing whole DN value as a single string.
   * \return length of the whole DN string.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity String is too small to hold the DN.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetDnString(ara::core::String* dn) const noexcept final;

  /*!
   * \brief Set whole Distinguished Name (DN) from a single string.
   * \param[in] dn the single string containing the whole DN value in text format.
   * \details Method not supported.
   * \vprivate Component Private
   */
  ara::core::Result<void> SetDn(ara::core::StringView dn) noexcept final;

  /*!
   * \brief Get DN attribute by its ID
   *        (this method is applicable to all attributes except kOrgUnit and kDomainComponent).
   * \param[in] id  the identifier of required attribute.
   * \param[out] attribute  the pointer to a string for storing attribute value.
   * \return length of the attribute value (0 for empty attributes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Invalid attribute ID.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to derive the nid from the attribute ID.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Sting is too small to hole the attribute.
   * \error ara::crypto::SecurityErrc::kAboveBoundary Index value is greater than the current number of components in
   * the specified attribute.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to get the attribute.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetAttribute(AttributeId id, ara::core::String* attribute) const noexcept final;

  /*!
   * \brief Set DN attribute by its ID
   *        (this method is applicable to all attributes except kOrgUnit and kDomainComponent).
   * \param[in] id  the identifier of required attribute.
   * \param[in] attribute  the attribute value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument Invalid attribute ID.
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to derive the nid from the attribute ID.
   * \error ara::crypto::SecurityErrc::kAboveBoundary Index value is greater than the current number of components in
   * the specified attribute.
   * \error ara::crypto::SecurityErrc::kUnexpectedValue if the attribute string contains
   * incorrect characters or it has unsupported length.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetAttribute(AttributeId id, ara::core::StringView attribute) const noexcept final;

  /*!
   * \brief Return DN attribute by its ID and sequential index
   *        (this method is applicable to attributes kOrgUnit and kDomainComponent).
   * \param[in] id the identifier of required attribute.
   * \param[in] index the zero-based index of required component of the attribute.
   * \param[out] attribute the pointer to a string for storing attribute value.
   * \return length of the attribute value (0 for empty attributes).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If (id != kOrgUnit) && (id != kDomainComponent) && (index > 0).
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to derive the nid from the attribute ID.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Sting is too small to hole the attribute.
   * \error ara::crypto::SecurityErrc::kAboveBoundary Index value is greater than the current number of components in
   * the specified attribute.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to get the attribute.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<std::size_t> GetAttribute(AttributeId id, std::uint32_t index,
                                              ara::core::String* attribute) const noexcept final;

  /*!
   * \brief Set DN attribute by its ID and sequential index
   *        (this method is applicable to attributes kOrgUnit and kDomainComponent).
   * \param[in] id  the identifier of required attribute.
   * \param[in] index  the zero-based index of required component of the attribute.
   * \param[in] attribute  the attribute value.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If (id != kOrgUnit) && (id != kDomainComponent) && (index > 0).
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier Unable to derive the nid from the attribute ID.
   * \error ara::crypto::SecurityErrc::kAboveBoundary Index value is greater than the current number of components in
   * the specified attribute.
   * \error ara::crypto::SecurityErrc::kUnexpectedValue if the attribute string contains
   * incorrect characters or it has unsupported length.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<void> SetAttribute(AttributeId id, std::uint32_t index,
                                       ara::core::StringView attribute) const noexcept final;

 private:
  amsr::sec::ipc::MessageSerializer message_serializer_;     /*!< Message serializer for communication */
  amsr::sec::ipc::MessageDeserializer message_deserializer_; /*!< Message deserializer for communication */
};

}  // namespace x509
}  // namespace proxy
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_X509_DN_H_
