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
/*!        \file  skeleton_builder.h
 *        \brief  Factory class for creation of skeleton classes depending on the underlying implementation type.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_SKELETON_BUILDER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_SKELETON_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_deserializer.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/crypto/cryp/authn_stream_cipher_ctx.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/key_agreement_private_ctx.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "ara/crypto/x509/cert_sign_request.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/ocsp_response.h"
#include "crypto/server/base_skeleton.h"
#include "crypto/server/base_skeleton_registration.h"
#include "crypto/server/config/daemon_config.h"
#include "crypto/server/object_registration.h"
#include "crypto/server/server_message_deserializer.h"

namespace crypto {
namespace server {
namespace skeleton {

// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * Builder class for creation of skeleton classes depending on the underlying implementation type.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class SkeletonBuilder {
 public:
  /*!
   * \brief Constructor
   * \param[in] transporter Transporter interface to be passed to skeletons
   * \param[in] deserializer Deserialiter to be passed to skeletons
   * \param[in] serializer Serializer to be passed to skeletons
   * \param[in] message_builder Message builder to be passed to skeletons
   * \param[in] object_registration Object registration to be passed to skeletons
   * \param[in] skeleton_registration Skeleton registration to be passed to skeletons
   * \param[in] config Daemon configuration
   * \vprivate Component Private
   */
  SkeletonBuilder(amsr::sec::ipc::Transporter& transporter, crypto::server::ServerMessageDeserializer& deserializer,
                  amsr::sec::ipc::MessageSerializer& serializer, amsr::sec::ipc::MessageBuilder& message_builder,
                  crypto::server::ObjectRegistration& object_registration,
                  crypto::server::BaseSkeletonRegistration& skeleton_registration,
                  config::DaemonConfig const& config) noexcept
      : transporter_{transporter},
        deserializer_{deserializer},
        serializer_{serializer},
        message_builder_{message_builder},
        object_registration_{object_registration},
        skeleton_registration_{skeleton_registration},
        config_{config} {}

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  SkeletonBuilder(SkeletonBuilder const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  SkeletonBuilder(SkeletonBuilder&& /*other*/) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  SkeletonBuilder& operator=(SkeletonBuilder const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  SkeletonBuilder& operator=(SkeletonBuilder&& /*other*/) & noexcept = delete;

  /*!
   * \brief Destroys the object.
   * \vprivate Component Private
   */
  virtual ~SkeletonBuilder() noexcept = default;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::CryptoProvider& instance) noexcept;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(ara::crypto::x509::X509DN& instance) noexcept;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::x509::OcspResponse& instance) noexcept;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::x509::X509Provider& instance) noexcept;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(ara::crypto::x509::Certificate& instance) noexcept;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::x509::AttributeCertificate& instance) noexcept;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_parameters_should_stay_as_they_are
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::x509::CertSignRequest& instance) noexcept;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::keys::KeyStorageProvider& instance) noexcept;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::StreamCipherCtx& instance) noexcept;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::AuthnStreamCipherCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::MessageAuthnCodeCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::HashFunctionCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::RandomGeneratorCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::KeyDerivationFunctionCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(ara::crypto::cryp::SecretSeed& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(ara::crypto::cryp::Signature& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::SymmetricKey& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::SignerPrivateCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::VerifierPublicCtx& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::DomainParameters& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(ara::crypto::TrustedContainer& instance) noexcept;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from.
   *                     ObjectType of given instance (CryptoObject::GetObjectType()) must be supported.
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::CryptoObject& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -

   * \exceptionsafety STRONG
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::X509CertRequest& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -

   * \exceptionsafety STRONG
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::X509Signature& instance) noexcept;
  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -

   * \exceptionsafety STRONG
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::X509PublicKeyInfo& instance) noexcept;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -

   * \exceptionsafety STRONG
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::unique_ptr<crypto::server::BaseSkeleton> MakeSkeleton(
      ara::crypto::cryp::X509RequestSignerCtx& instance) noexcept;

  /*!
   * \brief Factory method for skeleton instantiation
   * \param[in] instance The instance to build unique pointer from
   * \return Returns a unique pointer to the given instance
   * \context ANY
   * \pre -
   * \throw std::bad_alloc Memory allocation failed.
   * \exceptionsafety STRONG
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto MakeSkeleton(ara::crypto::cryp::KeyAgreementPrivateCtx& instance) noexcept
      -> std::unique_ptr<crypto::server::BaseSkeleton>;

 private:
  amsr::sec::ipc::Transporter& transporter_;                        /*!< Connection. */
  crypto::server::ServerMessageDeserializer& deserializer_;         /*!< Message deserializer. */
  amsr::sec::ipc::MessageSerializer& serializer_;                   /*!< Message serializer. */
  amsr::sec::ipc::MessageBuilder& message_builder_;                 /*!< Message builder. */
  crypto::server::ObjectRegistration& object_registration_;         /*!< Storage for deserialized objects. */
  crypto::server::BaseSkeletonRegistration& skeleton_registration_; /*!< Registration for skeletons. */
  config::DaemonConfig const& config_;                              /*!< Daemon config */
};

}  // namespace skeleton
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_SKELETON_BUILDER_H_
