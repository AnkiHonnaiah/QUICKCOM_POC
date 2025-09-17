/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  client_id_generator_interface.h
 *        \brief  Interface class to manage the client Ids.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_ID_GENERATOR_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_ID_GENERATOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief Interface for client Ids management.
 */
class ClientIdGeneratorInterface {
 public:
  /*!
   * \brief   Default constructor.
   * \steady  FALSE
   */
  ClientIdGeneratorInterface() noexcept = default;

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  virtual ~ClientIdGeneratorInterface() noexcept = default;

  ClientIdGeneratorInterface(ClientIdGeneratorInterface const &) = delete;
  ClientIdGeneratorInterface(ClientIdGeneratorInterface &&) = delete;
  ClientIdGeneratorInterface &operator=(ClientIdGeneratorInterface const &) & = delete;
  ClientIdGeneratorInterface &operator=(ClientIdGeneratorInterface &&) & = delete;

  /*!
   * \brief Generate client Id.
   *
   * \return The generated client Id.
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual ara::core::Result<amsr::someip_protocol::internal::ClientId> GenerateClientId() noexcept = 0;

  /*!
   * \brief Release client Id.
   *
   * \param[in] client_id   A SOME/IP client identifier.
   *
   * \return "true" if the client Id released successfully, and "false" otherwise.
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual bool ReleaseClientId(amsr::someip_protocol::internal::ClientId const client_id) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_ID_GENERATOR_INTERFACE_H_
