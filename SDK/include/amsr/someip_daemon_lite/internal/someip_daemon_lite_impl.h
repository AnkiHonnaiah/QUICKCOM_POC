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
/*!        \file   internal/someip_daemon_lite_impl.h
 *         \brief  Direct access provider to local SOME/IP client (implementation).
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpDaemonLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_DAEMON_LITE_IMPL_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_DAEMON_LITE_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/someip_daemon_core/client/required_service_instance_manager_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_lite/required_instance_id.h"
#include "amsr/someip_daemon_lite/required_service_instance.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief Direct access provider to SOME/IP bindings core (implementation).
 * \vprivate Component Private
 */
class SomeIpDaemonLiteImpl final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] uptr_someip_daemon_core Unique pointer to an instance of SOME/IP daemon core.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  explicit SomeIpDaemonLiteImpl(someip_daemon_core::client::RequiredServiceInstanceManagerInterface const&
                                    required_service_instance_manager) noexcept;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   * \steady   FALSE
   */
  ~SomeIpDaemonLiteImpl() noexcept = default;

  SomeIpDaemonLiteImpl(SomeIpDaemonLiteImpl&&) = delete;
  SomeIpDaemonLiteImpl(SomeIpDaemonLiteImpl const&) = delete;
  SomeIpDaemonLiteImpl& operator=(SomeIpDaemonLiteImpl const&) = delete;
  SomeIpDaemonLiteImpl& operator=(SomeIpDaemonLiteImpl&&) = delete;

  /*!
   * \brief Add service instance to the list of required instances.
   * \param[in] service_id Service ID.
   * \param[in] required_instance_id Required instance ID.
   * \param[in] major_interface_version Major interface version.
   * \param[in] minor_interface_version Minor interface version.
   *
   * \return \p ara::core::Result with an instance of "RequiredServiceInstance" or an error.
   * \error SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable Required service instance not available.
   *
   * \pre \p required_instance_id shall not represent all IDs.
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  ara::core::Result<RequiredServiceInstance> CreateRequiredServiceInstance(
      ServiceId const service_id, RequiredInstanceId const& required_instance_id,
      MajorInterfaceVersion const major_interface_version,
      MinorInterfaceVersion const minor_interface_version) const noexcept;

 private:
  /*!
   * \brief Reference to the required service instance manager.
   */
  someip_daemon_core::client::RequiredServiceInstanceManagerInterface const& required_service_instance_manager_;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                          someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                          ara::core::StringView{"SomeIpDaemonLiteImpl"}};
};

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SOMEIP_DAEMON_LITE_IMPL_H_
