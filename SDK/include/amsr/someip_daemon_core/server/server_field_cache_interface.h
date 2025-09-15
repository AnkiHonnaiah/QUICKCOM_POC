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
/**        \file  server_field_cache_interface.h
 *        \brief  Server Field Cache Interface.
 *
 *      \details  Provides an API to return the caller a pointer to a cached field for a specific event id.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_FIELD_CACHE_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_FIELD_CACHE_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>  // shared_ptr
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief Interface for a server field cache.
 */
class ServerCacheInterface {
 public:
  /*!
   * \brief default constructor.
   * \steady FALSE
   */
  ServerCacheInterface() = default;

  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  virtual ~ServerCacheInterface() noexcept = default;

  ServerCacheInterface(ServerCacheInterface const&) = delete;
  ServerCacheInterface(ServerCacheInterface&&) = delete;
  ServerCacheInterface& operator=(ServerCacheInterface const&) & = delete;
  ServerCacheInterface& operator=(ServerCacheInterface&&) & = delete;

  /*!
   * \brief Returns the pointer to the cached field for a specific event id .
   *
   * \param[in] event_id    The Event id.
   *
   * \pre -
   * \context Network
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual std::shared_ptr<someip_protocol::internal::SomeIpMessage> const GetCacheField(
      someip_protocol::internal::EventId const event_id) const noexcept = 0;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_FIELD_CACHE_INTERFACE_H_
