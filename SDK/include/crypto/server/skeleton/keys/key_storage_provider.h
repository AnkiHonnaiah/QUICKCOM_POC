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
/*!        \file  skeleton/keys/key_storage_provider.h
 *        \brief  Skeleton implementation for ara::crypto::keys::KeyStorageProvider.
 *
 *      \details  Server side implementation of the ara::crypto::keys::KeyStorageProvider interface that delegates
 *                client calls to a server side implementation that performs key storage related tasks.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_KEYS_KEY_STORAGE_PROVIDER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_KEYS_KEY_STORAGE_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "crypto/server/base_skeleton_impl.h"

namespace crypto {
namespace server {
namespace skeleton {
namespace keys {

/*!
 * \brief Skeleton implementation for ara::crypto::keys::KeyStorageProvider
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class KeyStorageProvider : public BaseSkeletonImpl<ara::crypto::keys::KeyStorageProvider,
                                                   amsr::sec::ipc::ara_crypto_keys_KeyStorageProvider> {
 public:
  /*!
   * Constructor calling Base constructor from BaseSkeletonImpl
   * \copydetails BaseSkeletonImpl::BaseSkeletonImpl
   * \vprivate Component Private
   */
  using BaseSkeletonImpl::BaseSkeletonImpl;

  /*!
   * \brief default destructor
   * \vprivate Component Private
   */
  ~KeyStorageProvider() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  KeyStorageProvider(KeyStorageProvider const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  KeyStorageProvider(KeyStorageProvider&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  KeyStorageProvider& operator=(KeyStorageProvider const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  KeyStorageProvider& operator=(KeyStorageProvider&& /*other*/) & noexcept = delete;

  /*!
   * \copydoc BaseSkeletonImpl::DispatchMethod
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRpcInvalidArgument Error in CallImplementation.
   * \error ara::crypto::SecurityErrc::kRpcUnknownTask Dispatch called with unknown task.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ara::core::Result<ara::crypto::ReadOnlyMemRegion> DispatchMethod(
      amsr::sec::ipc::ara_crypto_keys_KeyStorageProvider task, ara::crypto::ReadOnlyMemRegion input_buffer,
      ara::crypto::WritableMemRegion output_buffer) noexcept override;
};

}  // namespace keys
}  // namespace skeleton
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_KEYS_KEY_STORAGE_PROVIDER_H_
