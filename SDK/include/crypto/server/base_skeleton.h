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
/*!        \file  base_skeleton.h
 *        \brief  This file contains an interface for server side skeleton objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/types.h"
#include "ara/crypto/common/mem_region.h"

namespace crypto {
namespace server {
/*!
 * \brief Interface for server side skeleton objects
 * \trace CREQ-Crypto-CryptoProvider_ProcessIsolation
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
class BaseSkeleton {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  BaseSkeleton() = default;

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~BaseSkeleton() noexcept = default;

  /*!
   * \brief Entry point to process new data received by the transporter
   * \details The properties of this method (thread-safety, errors, reentrant, synchronous) depend on the underlying
   * implementation.
   * \param[in] input_buffer Raw data containing the received value to be deserialized
   * \param[out] output_buffer Destination buffer for the serialized message with return value of the invoked message
   * \param[in] e_uid Effective user id of the process which sent the message.
   * \return Subspan of the message containing the response
   * \vprivate Component Private
   */
  virtual auto ProcessMessage(ara::crypto::ReadOnlyMemRegion input_buffer, ara::crypto::WritableMemRegion output_buffer,
                              amsr::sec::ipc::UserId e_uid) noexcept
      -> amsr::core::Result<ara::crypto::ReadOnlyMemRegion> = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  BaseSkeleton(BaseSkeleton const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  BaseSkeleton(BaseSkeleton&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  BaseSkeleton& operator=(BaseSkeleton const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  BaseSkeleton& operator=(BaseSkeleton&& /*other*/) & noexcept = default;
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_H_
