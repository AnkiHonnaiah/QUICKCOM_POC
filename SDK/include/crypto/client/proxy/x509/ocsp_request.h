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
/*!        \file    proxy/x509/ocsp_request.h
 *         \brief  Proxy implementation for ara::crypto::x509::OcspRequest.
 *
 *         \details  Client side implementation of the ara::crypto::x509::OcspRequest interface that
 *                   transparently communicates with a server side implementation that performs the real cryptography.
 *********************************************************************************************************************/
#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_OCSP_REQUEST_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_OCSP_REQUEST_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/core/string_view.h"
#include "ara/crypto/x509/ocsp_request.h"
#include "crypto/client/base_proxy.h"

namespace crypto {
namespace client {
namespace proxy {
namespace x509 {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::operator""_sv;

// VECTOR NC AutosarC++17_10-A10.1.1: MD_CRYPTO_AutosarC++17_10-A10.1.1_multiple_inheritance_for_proxies
/*!
 * \brief Client-side implementation for the proxy of ara::crypto::x509::OcspRequest.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class OcspRequest final : public BaseProxy, public ara::crypto::x509::OcspRequest {
 public:
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
  static std::unique_ptr<OcspRequest, ara::crypto::CustomDeleter> Create(
      amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
      amsr::sec::ipc::MessageSerializer& message_serializer,
      amsr::sec::ipc::MessageDeserializer& message_deserializer) {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_raii
    return std::unique_ptr<OcspRequest, ara::crypto::CustomDeleter>(
        // VECTOR NL AutosarC++17_10-A18.5.2: MD_CRYPTO_AutosarC++17_10-A18.5.2
        new OcspRequest(transporter, message_builder, message_serializer, message_deserializer));
  };

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~OcspRequest() noexcept final = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  OcspRequest(OcspRequest const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  OcspRequest(OcspRequest&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  OcspRequest& operator=(OcspRequest const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  OcspRequest& operator=(OcspRequest&& /*other*/) = delete;

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
  void Release() noexcept final { delete this; };

  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *                     size of the output buffer is interested) - Parameter unused.
   * \param[in] format_id  The Crypto Provider specific identifier of the output format - Paramter unused.
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
  ara::core::Result<std::size_t> ExportPublicly(ara::crypto::WritableMemRegion output,
                                                FormatId format_id) const noexcept final {
    static_cast<void>(output);
    static_cast<void>(format_id);
    return ara::core::Result<std::size_t>::FromError(ara::crypto::SecurityErrc::kUnsupported);
  };
  /*!
   * \brief Get version of the OCSP response format.
   * \return OCSP response format version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint32_t Version() const noexcept final { return 0; };
};

}  // namespace x509
}  // namespace proxy
}  // namespace client
}  // namespace crypto
#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROXY_X509_OCSP_REQUEST_H_
