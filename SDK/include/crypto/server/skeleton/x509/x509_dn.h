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
/*!        \file  skeleton/x509/x509_dn.h
 *        \brief  Skeleton implementation for ara::crypto::x509::X509DN.
 *
 *      \details  Implementation of skeleton class that forwards the calls requested by proxy side to the real
 *                implementation and processes the response.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_X509_X509_DN_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_X509_X509_DN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "crypto/server/base_skeleton_impl.h"

namespace crypto {
namespace server {
namespace skeleton {
namespace x509 {

/*!
 * \brief Server side implementation for the skeleton of ara::crypto::x509::X509DN.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class X509DN final : public BaseSkeletonImpl<ara::crypto::x509::X509DN, amsr::sec::ipc::ara_crypto_x509_X509DN> {
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
  ~X509DN() noexcept final = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  X509DN(X509DN const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  X509DN(X509DN&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509DN& operator=(X509DN const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  X509DN& operator=(X509DN&& /*other*/) & noexcept = delete;

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
      amsr::sec::ipc::ara_crypto_x509_X509DN task, ara::crypto::ReadOnlyMemRegion input_buffer,
      ara::crypto::WritableMemRegion output_buffer) noexcept final;
};

}  // namespace x509
}  // namespace skeleton
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SKELETON_X509_X509_DN_H_
