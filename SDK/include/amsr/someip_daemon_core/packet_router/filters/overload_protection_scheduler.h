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
/**        \file  overload_protection_scheduler.h
 *        \brief  Overload Protection Scheduler
 *
 *      \details  This class is responsible for protecting applications from being overloaded with packets.
 *                This is done by configuring a minimum period between two consecutive packets, and delaying (and
 *                potentially dropping) the following packets that are received during the protection period.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_FILTERS_OVERLOAD_PROTECTION_SCHEDULER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_FILTERS_OVERLOAD_PROTECTION_SCHEDULER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>

#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {
namespace filters {

/*!
 * \brief   Packet scheduler for overload protection.
 * \details This class is responsible for:
 *          - Keeping track of the state of the overload protection (active/deactivated).
 *          - Local storage of the last scheduled packet during an overload protection period.
 *          - Passing the local stored packet to the given callback upon timeout.
 *
 *  Usage example:
 *  When a new packet arrives:
 *    if(IsProtectionActive())
 *      Schedule(instance_id, packet)
 *    else
 *      ActivateProtection()
 *      Process/Forward the packet
 */
class OverloadProtectionScheduler final {
 public:
  /*!
   * \brief Represents a SOME/IP message.
   */
  using Packet = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Represents a PDU message.
   */
  using PduPacket = std::shared_ptr<someip_protocol::internal::PduMessage>;

  /*!
   * \brief The signature for an action to be executed when the timeout expires.
   */
  using ScheduleCallbackType =
      vac::language::UniqueFunction<void(someip_protocol::internal::InstanceId const& instance_id, Packet&)>;

  /*!
   * \brief The signature for an action to be executed when the timeout expires.
   */
  using SchedulePduCallbackType =
      vac::language::UniqueFunction<void(someip_protocol::internal::InstanceId const& instance_id, PduPacket&)>;

  /*!
   * \brief       Constructs the overload protection scheduler.
   * \details     The timer does not start with the constructor.
   * \param[in]   timer_manager       A timer manager.
   * \param[in]   protection_interval The overload protection time.
   * \param[in]   callback            Callback to be executed upon timeouts.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  OverloadProtectionScheduler(amsr::steady_timer::TimerManagerInterface* timer_manager,
                              std::chrono::nanoseconds const protection_interval,
                              ScheduleCallbackType&& callback) noexcept;

  /*!
   * \brief       Constructs the overload protection scheduler.
   * \details     The timer does not start with the constructor.
   * \param[in]   timer_manager       A timer manager.
   * \param[in]   protection_interval The overload protection time.
   * \param[in]   callback            Callback to be executed upon timeouts.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  OverloadProtectionScheduler(amsr::steady_timer::TimerManagerInterface* timer_manager,
                              std::chrono::nanoseconds const protection_interval,
                              SchedulePduCallbackType&& callback) noexcept;

  OverloadProtectionScheduler(OverloadProtectionScheduler const&) = delete;
  OverloadProtectionScheduler& operator=(OverloadProtectionScheduler const&) & = delete;
  OverloadProtectionScheduler(OverloadProtectionScheduler&&) = delete;
  OverloadProtectionScheduler& operator=(OverloadProtectionScheduler&&) & = delete;

  /*!
   * \brief       Destroys the overload protection scheduler.
   * \pre         -
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~OverloadProtectionScheduler() noexcept;

  /*!
   * \brief       Indicates whether the overload protection is active.
   * \return      True if the overload protection is active, false otherwise.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool IsProtectionActive() const noexcept;

  /*!
   * \brief       Activates the overload protection and starts the timer.
   * \pre         ActivateProtection() shall only be called if IsProtectionActive() returns false.
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void ActivateProtection() noexcept;

  /*!
   * \brief       Stores locally the passed packet.
   * \details     The stored packet will be used as parameter for the callback when the timer expires.
   *              Subsequent calls to this method will overwrite the locally stored packet.
   *              This function does not start the protection timer, so the protection must be separately activated
   *              in order for the scheduled packet to be forwarded upon timer expiration.
   * \param[in]   instance_id SOME/IP instance_id.
   * \param[in]   packet      A SOME/IP packet.
   * \param[in]   is_field    Boolean indicating whether the event is a field.
   * \return      True if a previously scheduled packet was dropped, false otherwise.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  bool Schedule(someip_protocol::internal::InstanceId const instance_id, Packet const& packet,
                bool const is_field) noexcept;

  /*!
   * \brief       Stores locally the passed packet.
   * \details     The stored packet will be used as parameter for the callback when the timer expires.
   *              Subsequent calls to this method will overwrite the locally stored packet.
   *              This function does not start the protection timer, so the protection must be separately activated
   *              in order for the scheduled packet to be forwarded upon timer expiration.
   * \param[in]   instance_id SOME/IP instance_id.
   * \param[in]   packet A PDU packet.
   * \return      True if a previously scheduled packet was dropped, false otherwise.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  bool Schedule(someip_protocol::internal::InstanceId const instance_id, PduPacket const& packet) noexcept;

 private:
  /*!
   * \brief       Constructs the overload protection scheduler.
   * \details     The timer does not start with the constructor.
   * \param[in]   timer_manager       A timer manager.
   * \param[in]   protection_interval The overload protection time.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  OverloadProtectionScheduler(amsr::steady_timer::TimerManagerInterface* timer_manager,
                              std::chrono::nanoseconds const protection_interval) noexcept;
  /*!
   * \brief Wrapper structure of a scheduled SOME/IP event message, containing the packet and the instance id.
   */
  struct ScheduledMessage {
    /*!
     * \brief The SOME/IP instance id.
     */
    someip_protocol::internal::InstanceId instance_id{};

    /*!
     * \brief Shared pointer to the memory buffer containing the last scheduled packet.
     */
    Packet packet{nullptr};

    /*!
     * \brief Boolean indicating whether the event is a field.
     */
    bool const is_field{};
  };

  /*!
   * \brief Wrapper structure of a scheduled PDU event message, containing the packet and the instance id.
   */
  struct ScheduledPduMessage {
    /*!
     * \brief The SOME/IP instance id.
     */
    someip_protocol::internal::InstanceId instance_id{};

    /*!
     * \brief Shared pointer to the memory buffer containing the last scheduled packet.
     */
    PduPacket packet{nullptr};
  };

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief       Executed when timer expires.
   * \details     If a packet was scheduled: calls callback, resets packet and re-starts timer.
   *              If no packet was scheduled: deactivates protection.
   * \pre         -
   * \context     Timer
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void HandleTimer() noexcept;

  /*!
   * \brief   Flag to indicate whether the overload protection is active.
   * \details The flag must be activated from outside by calling ActivateProtection().
   *          The flag will be deactivated from HandleTimer() upon timeout.
   */
  bool protection_active_{false};

  /*!
   * \brief Overload protection time.
   */
  std::chrono::nanoseconds const protection_interval_;

  /*!
   * \brief The timer.
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> timer_;

  /*!
   * \brief   The scheduled message.
   * \details This variable shall be optional as the protection can be active without scheduling a message.
   */
  ara::core::Optional<ScheduledMessage> scheduled_message_{};

  /*!
   * \brief   The scheduled PDU message.
   * \details This variable shall be optional as the protection can be active without scheduling a message.
   */
  ara::core::Optional<ScheduledPduMessage> scheduled_pdu_message_{};

  /*!
   * \brief Callback that will be called upon timeout.
   */
  ScheduleCallbackType callback_{nullptr};

  /*!
   * \brief Callback that will be called upon timeout.
   */
  SchedulePduCallbackType pdu_callback_{nullptr};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kSomeIpDLoggerContextId,
                                                    someip_daemon_core::logging::kSomeIpDLoggerContextDescription,
                                                    ara::core::StringView{"OverloadProtectionScheduler"}};
};

}  // namespace filters
}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_FILTERS_OVERLOAD_PROTECTION_SCHEDULER_H_
