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
/*!        \file  message_error_handler.h
 *        \brief  Error handler parsing server response messages and aborts if an error occurred in the IPC framework
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_MESSAGE_ERROR_HANDLER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_MESSAGE_ERROR_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/message_deserializer.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace client {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief  Error handler parsing server response messages and aborts if an error occurred in the IPC framework
 * \vprivate Component Private
 */
class MessageErrorHandler {
 public:
  /*!
   * \brief Error handler for response messages
   * \details Aborts if any error occurred in the IPC framework
   * \param[in] basic_task Basic task of the message
   * \param[in] detail_task Detail task of the message
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void HandleErrorMessage(amsr::sec::ipc::Task basic_task, amsr::sec::ipc::Task detail_task) const noexcept;

 private:
  crypto::common::util::Logger logger_{common::util::kClientLoggingContext, ""_sv}; /*!< Logging instance */
  amsr::sec::ipc::MessageDeserializer message_deserializer_{}; /*!< Deserializer for error messages */
};

}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_MESSAGE_ERROR_HANDLER_H_
