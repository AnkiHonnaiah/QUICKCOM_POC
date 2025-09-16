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
/**        \file  server_field_cache.h
 *        \brief  Server Field Cache.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_FIELD_CACHE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_FIELD_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>  // std::shared_ptr
#include <set>
#include "amsr/someip_daemon_core/server/server_field_cache_interface.h"
#include "ara/core/map.h"
#include "ara/core/memory_resource.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief ServerFieldCache.
 * \details
 */
class ServerFieldCache final : public ServerCacheInterface {
 private:
  /*!
   * \brief Container of events configured for a provided service instance that are configured as fields
   */
  using ProvidedFieldEvents =
      std::set<amsr::someip_protocol::internal::EventId, std::less<amsr::someip_protocol::internal::EventId>,
               ara::core::PolymorphicAllocator<amsr::someip_protocol::internal::EventId>>;

 public:
  /*!
   * \brief constructor.
   *
   * \param[in] server_field_events    A container with a list of configured field notifications.
   */
  explicit ServerFieldCache(ProvidedFieldEvents const& server_field_events) noexcept;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ServerFieldCache() noexcept override = default;

  ServerFieldCache(ServerFieldCache const&) = delete;
  ServerFieldCache(ServerFieldCache&&) = delete;
  ServerFieldCache& operator=(ServerFieldCache const&) & = delete;
  ServerFieldCache& operator=(ServerFieldCache&&) & = delete;

  /*!
   * \brief Add the cached field message for the specific event id.
   *
   * \param[in] event_id    The Event id.
   * \param[in] packet      A SOME/IP message.
   *
   * \return  True if given event id represents field, false otherwise
   *          The return value is used to avoid the duplication of memory in the unit.
   *          This can be used not only to cache field notifications but also to check
   *          whether an event is configured as a field or not.
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   *
   */
  bool AddFieldCache(someip_protocol::internal::EventId const event_id,
                     std::shared_ptr<someip_protocol::internal::SomeIpMessage> packet) noexcept;

  /*!
   * \brief Check if all field notification are initialized
   *
   * \return True if all configured field notifications are initialized, false otherwise
   *
   * \pre          -
   * \context      App
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   */
  bool AreInitialFieldNotificationsCached() const noexcept;

  /*!
   * \brief Reset all field notification cached messages
   *
   * \pre          -
   * \context      App, Shutdown
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   *
   */
  void ClearFieldCache() noexcept;

 private:
  /*!
   * \brief Returns the pointer to the cached field for a specific event id .
   *
   * \param[in] event_id    The Event id.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  std::shared_ptr<someip_protocol::internal::SomeIpMessage> const GetCacheField(
      someip_protocol::internal::EventId const event_id) const noexcept override;

  /*!
   * \brief   Field provider value cache table.
   * \details Stores the values of the event-mapped-fields, to be sent to new field subscribers.
   */
  ara::core::Map<someip_protocol::internal::EventId, std::shared_ptr<someip_protocol::internal::SomeIpMessage>>
      server_field_cache_map_{};
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_SERVER_FIELD_CACHE_H_
