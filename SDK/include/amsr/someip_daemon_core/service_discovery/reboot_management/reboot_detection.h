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
/**        \file  reboot_detection.h
 *        \brief  Reboot detection
 *
 *      \details  Maintains the session id and reboot flag for unicast and multicast messages for reboot detection
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_REBOOT_DETECTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_REBOOT_DETECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message.h"
#include "amsr/someip_daemon_core/service_discovery/reboot_management/reboot_management_types.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/message.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace reboot_management {

/*!
 * \brief Maintains the session id and reboot flag for unicast and multicast messages for reboot detection.
 */
class RebootDetection final {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  RebootDetection() = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  ~RebootDetection() noexcept = default;

  RebootDetection(RebootDetection const &) = delete;
  RebootDetection(RebootDetection &&) = delete;
  RebootDetection &operator=(RebootDetection const &) & = delete;
  RebootDetection &operator=(RebootDetection &&) & = delete;

  /*!
   * \brief Perform reboot detection checks and updates the reboot manager.
   *
   * \details The reboot parameters (session id and reboot flag), that are extracted from SD message, are saved into the
   *          reboot manager, and compared against the latest saved ones.
   *          In case reboot is detected, reboot manager is also updated (if reboot is detected via unicast SD message,
   *          the corresponding multicast counters are reset, and vise versa).
   *
   * \param[in] reboot_key The key composed by address and port.
   * \param[in] is_multicast Specifies whether a SOME/IP-SD message is a multicast one.
   * \param[in] sd_message The received SOME/IP SD message.
   *
   * \return true if reboot is detected, false otherwise.
   * \pre -
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   */
  bool PerformRebootDetection(RebootDetectionKey const &reboot_key, bool const is_multicast,
                              message::ServiceDiscoveryMessage const &sd_message) noexcept;

 private:
  /*!
   * \brief Process the reboot detection.
   *
   * \param[in] old_reboot_param  last reboot_param (session_id and reboot flag) value received.
   * \param[in] new_reboot_param  values of the current packet (session_id and reboot flag)
   * received
   *
   * \return true if reboot detected otherwise false
   * \pre -
   * \context Network
   * \steady TRUE
   * \reentrant FALSE
   * \trace SPEC-4981507
   */
  bool IsRebootDetected(reboot_management::RebootParam const &old_reboot_param,
                        reboot_management::RebootParam const &new_reboot_param) const noexcept;

  /*!
   * \brief Adds a new entry or updates an existing entry in the sd_multicast_receiver_reboot_management_map and returns
   * the last set session id and reboot flag.
   * \param[in] reboot_key   The key composed by address and port.
   * \param[in] session_id   identifier for the specific message receiver.
   * \param[in] reboot_flag  flag for reboot detection.
   *
   * \return RebootParam next session id and reboot flag.
   *
   * \context Network
   * \steady TRUE
   */
  RebootParam GetLastRebootParamOnMulticastMessageReception(RebootDetectionKey const &reboot_key,
                                                            amsr::someip_protocol::internal::SessionId const session_id,
                                                            bool const reboot_flag) noexcept;

  /*!
   * \brief Adds a new entry or updates an existing entry in the sd_unicast_receiver_reboot_management_map and returns
   * the last set session id and reboot flag.
   * \param[in] reboot_key   The key composed by address and port.
   * \param[in] session_id   identifier for the specific message receiver.
   * \param[in] reboot_flag  flag for reboot detection.
   *
   * \return RebootParam last session id and reboot flag.
   *
   * \context Network
   * \steady TRUE
   */
  RebootParam GetLastRebootParamOnUnicastMessageReception(RebootDetectionKey const &reboot_key,
                                                          amsr::someip_protocol::internal::SessionId const session_id,
                                                          bool const reboot_flag) noexcept;

  /*!
   * \brief Resets the session id and reboot flag for an existing entry in the
   * sd_unicast_receiver_reboot_management_map. When reboot is detected for a provider daemon on reception of multicast
   * message, we reset the session id and reboot flag for unicast from that provider to avoid duplicate notification of
   * reboot.
   *
   * \param[in] reboot_key The key composed by address and port.
   *
   * \context Network
   * \steady TRUE
   */
  void ResetUnicastReceiverRebootParam(RebootDetectionKey const &reboot_key) noexcept;

  /*!
   * \brief Resets the session id and reboot flag for an existing entry in the
   *        sd_multicast_receiver_reboot_management_map.
   *        When reboot is detected for a provider daemon on reception of unicast message, we reset the session id and
   * reboot flag for multicast from that provider to avoid duplicate notification of reboot.
   *
   * \param[in] reboot_key The key composed by address and port.
   *
   * \context Network
   * \steady TRUE
   */
  void ResetMulticastReceiverRebootParam(RebootDetectionKey const &reboot_key) noexcept;

  /*!
   * \brief Maps the information of unicast message receiver to the session_id and reboot flag.
   * \trace SPEC-4981506
   */
  RebootManagementMap sd_unicast_receiver_reboot_management_map_{};

  /*!
   * \brief Maps the information of multicast message receiver to the session_id and reboot flag.
   * \trace SPEC-4981506
   */
  RebootManagementMap sd_multicast_receiver_reboot_management_map_{};

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription, ara::core::StringView{"RebootDetection"}};

  /*!
   * \brief Friend test to access the private member functions
   */
  FRIEND_TEST(UT__RebootDetection, ResetUnicastReceiverRebootParam);

  /*!
   * \brief Friend test to access the private member functions
   */
  FRIEND_TEST(UT__RebootDetection, ResetMulticastReceiverRebootParam);

  /*!
   * \brief Friend test to access the private member functions
   */
  FRIEND_TEST(UT__RebootDetection, ReceiveNewMulticastMessage);

  /*!
   * \brief Friend test to access the private member functions
   */
  FRIEND_TEST(UT__RebootDetection, ReceiveNewUnicastMessage);
};

}  // namespace reboot_management
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_REBOOT_DETECTION_H_
