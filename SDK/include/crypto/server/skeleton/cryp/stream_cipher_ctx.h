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
/*!        \file  skeleton/cryp/stream_cipher_ctx.h
 *        \brief  Skeleton implementation for ara::crypto::cryp::StreamCipherCtx
 *      \details  Implementation of skeleton class that forwards the calls requested by proxy side to the real
 *                implementation and processes the response.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_CRYP_STREAM_CIPHER_CTX_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_CRYP_STREAM_CIPHER_CTX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/protocol.h"
#include "ara/crypto/cryp/stream_cipher_ctx.h"
#include "crypto/server/base_skeleton_impl.h"

namespace crypto {
namespace server {
namespace skeleton {
namespace cryp {

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Skeleton class for Method dispatching
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class StreamCipherCtx
    : public BaseSkeletonImpl<ara::crypto::cryp::StreamCipherCtx, amsr::sec::ipc::ara_crypto_cryp_StreamCipherCtx> {
 public:
  /*!
   * Constructor calling Base constructor from BaseSkeletonImpl
   * \copydetails BaseSkeletonImpl::BaseSkeletonImpl
   * \vprivate Component Private
   */
  using BaseSkeletonImpl::BaseSkeletonImpl;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  StreamCipherCtx(StreamCipherCtx const& /*other*/) = default;

  /*!
   * \brief Default Destructor
   * \vprivate Component Private
   */
  ~StreamCipherCtx() noexcept override = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  StreamCipherCtx(StreamCipherCtx&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  StreamCipherCtx& operator=(StreamCipherCtx const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  StreamCipherCtx& operator=(StreamCipherCtx&& /*other*/) & noexcept = delete;

 protected:
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
      amsr::sec::ipc::ara_crypto_cryp_StreamCipherCtx task, ara::crypto::ReadOnlyMemRegion input_buffer,
      ara::crypto::WritableMemRegion output_buffer) noexcept override;
};
}  // namespace cryp
}  // namespace skeleton
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_CRYP_STREAM_CIPHER_CTX_H_
