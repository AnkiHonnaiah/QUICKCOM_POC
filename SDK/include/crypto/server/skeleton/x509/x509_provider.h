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
/*!        \file  skeleton/x509/x509_provider.h
 *        \brief  Skeleton implementation for ara::crypto::x509::X509Provider.
 *
 *      \details  Implementation of skeleton class that forwards the calls requested by proxy side to the real
 *                implementation and processes the response.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_X509_X509_PROVIDER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_X509_X509_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/x509/x509_provider.h"
#include "crypto/common/util/contract.h"
#include "crypto/server/base_skeleton_impl.h"
#include "crypto/server/config/daemon_config.h"
#include "vac/testing/test_adapter.h"

namespace crypto {
namespace server {
namespace skeleton {
namespace x509 {

/*!
 * \brief Server side implementation for the skeleton of ara::crypto::x509::X509Provider.
 * \vprivate Component Private
 */
class X509Provider
    : public BaseSkeletonImpl<ara::crypto::x509::X509Provider, amsr::sec::ipc::ara_crypto_x509_X509Provider> {
 public:
  /*!
   * \brief Creates a new X509Provider skeleton
   * \param[in] implementation The object to forward the method invocations to
   * \param[in] object_registration Registration for newly created objects
   * \param[in] skeleton_registration Registration and provider for BaseSkeleton objects
   * \param[in] transporter Transporter interface for client/server communication
   * \param[in] deserializer Deserializer for deserializing the received data into messages
   * \param[in] serializer Serializer for serializing the message containing return value
   * \param[in] message_builder Builder for messages containing the return value
   * \param[in] skeleton_builder Builder for creating skeletons required if new objects are created as result of
   * method invocation
   * \param[in] config Daemon configuration
   * \vprivate Component Private
   */
  X509Provider(ara::crypto::x509::X509Provider& implementation, crypto::server::ObjectRegistration& object_registration,
               crypto::server::BaseSkeletonRegistration& skeleton_registration,
               amsr::sec::ipc::Transporter& transporter, crypto::server::ServerMessageDeserializer& deserializer,
               amsr::sec::ipc::MessageSerializer& serializer, amsr::sec::ipc::MessageBuilder& message_builder,
               crypto::server::skeleton::SkeletonBuilder& skeleton_builder, config::DaemonConfig const& config) noexcept
      : BaseSkeletonImpl<ara::crypto::x509::X509Provider, amsr::sec::ipc::ara_crypto_x509_X509Provider>(
            implementation, object_registration, skeleton_registration, transporter, deserializer, serializer,
            message_builder, skeleton_builder, false),
        ca_connector_id_{config.GetX509Config().GetAccessConfig().GetCaConnectorId()},
        trustmaster_id_{config.GetX509Config().GetAccessConfig().GetTrustmasterId()} {}

  /*!
   * \brief default destructor
   * \vprivate Component Private
   */
  ~X509Provider() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  X509Provider(X509Provider const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  X509Provider(X509Provider&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509Provider& operator=(X509Provider const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509Provider& operator=(X509Provider&& /*other*/) & noexcept = delete;

  /*!
   * \copydoc BaseSkeletonImpl::DispatchMethod
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRpcInvalidArgument Error in CallImplementation.
   * \error ara::crypto::SecurityErrc::kRpcUnknownTask Dispatch called with unknown task.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \error ara::crypto::SecurityErrc::kRpcUsageViolation No implementation yet for task.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::ReadOnlyMemRegion> DispatchMethod(
      amsr::sec::ipc::ara_crypto_x509_X509Provider task, ara::crypto::ReadOnlyMemRegion input_buffer,
      ara::crypto::WritableMemRegion output_buffer) noexcept override;

 private:
  std::uint64_t ca_connector_id_{0}; /*!< CA connector ID */
  std::uint64_t trustmaster_id_{0};  /*!< Trustmaster ID */

  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__client__proxy__x509__X509Provider, SetPendingStatus);
  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__client__proxy__x509__X509Provider, SetPendingStatus__NoPermission);
  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__client__proxy__x509__X509Provider, SetAsRootOfTrust);
  /*! Friend declarations for testing purposes */
  FRIEND_TEST(UT__client__proxy__x509__X509Provider, SetAsRootOfTrust__NoPermission);
};

}  // namespace x509
}  // namespace skeleton
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_X509_X509_PROVIDER_H_
