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
/*!        \file  proxy_deletion_handler.h
 *        \brief  Deletion handler for proxy objects.
 *
 *      \details  Deletion handler will propagate the proxy deletion to the server using an destroy message.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_DELETION_PROXY_DELETION_HANDLER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_DELETION_PROXY_DELETION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/transporter.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace client {
namespace deletion {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief  Deletion handler for proxy objects
 * \vprivate Component Private
 */
class ProxyDeletionHandler {
 public:
  /*!
   * \brief Constructor
   * \param[in] transporter Transporter to send notification with
   * \param[in] message_serializer Message serializer
   * \vprivate Component Private
   */
  ProxyDeletionHandler(amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageSerializer& message_serializer)
      : transporter_{transporter},
        message_serializer_{message_serializer},
        logger_{common::util::kClientLoggingContext, ""_sv} {}

  /*!
   * \brief Trigger called on deletion of proxy object
   * \param[in] proxy_id Id of the object to be deleted
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void HandleDeletion(amsr::sec::ipc::ProxyId proxy_id) noexcept(false);

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~ProxyDeletionHandler() noexcept = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ProxyDeletionHandler(ProxyDeletionHandler const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ProxyDeletionHandler(ProxyDeletionHandler&& /*other*/) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ProxyDeletionHandler& operator=(ProxyDeletionHandler const& /*other*/) & = delete;

  /*!
   * \brief Default move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ProxyDeletionHandler& operator=(ProxyDeletionHandler&& /*other*/) & noexcept = delete;

 private:
  amsr::sec::ipc::Transporter& transporter_;              /*!< Transporter */
  amsr::sec::ipc::MessageSerializer& message_serializer_; /*!< Message serializer */
  crypto::common::util::Logger logger_;                   /*!< Logging instance */
};

}  // namespace deletion
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_DELETION_PROXY_DELETION_HANDLER_H_
