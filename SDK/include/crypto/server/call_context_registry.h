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
/*!        \file  call_context_registry.h
 *        \brief  Registry holding the current call context of the caller in context of RPC.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CALL_CONTEXT_REGISTRY_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CALL_CONTEXT_REGISTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/types.h"
#include "ara/core/result.h"

namespace crypto {
namespace server {

/*!
 * \brief Registry holding the current call context of the caller in context of RPC.
 * \vprivate Component Private
 */
class CallContextRegistry final {
 public:
  /*!
   * \brief Sets the user id of the current call context.
   * \param[in] user_id The user id of the current caller.
   * \return void.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetUserId(amsr::ipc::Credentials const& user_id) noexcept -> void;

  /*!
   * \brief Sets the process id of the current call context.
   * \param[in] process_id The process id of the current caller.
   * \return void.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto SetProcessId(osabstraction::process::ProcessId const& process_id) noexcept -> void;

  /*!
   * \brief Gives access to the user id of the current call context.
   * \return The user id of the current caller.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetUserId() const noexcept -> amsr::ipc::Credentials const&;

  /*!
   * \brief Gives access to the process id of the current call context.
   * \return The process id of the current caller.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetProcessId() const noexcept -> osabstraction::process::ProcessId const&;

 private:
  amsr::ipc::Credentials user_id_{};               /*!< The user id of the current caller */
  osabstraction::process::ProcessId process_id_{}; /*!< The process ID of the current caller */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CALL_CONTEXT_REGISTRY_H_
