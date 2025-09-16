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
/*!        \file  someip_daemon_core/tracing/tracing_adapter.h
 *        \brief  SOME/IP daemon specific adapter for providing tracing functionality.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this adapter is defined.
 *                In order to provide actual tracing services it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_TRACING_ADAPTER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_TRACING_ADAPTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon/extension_points/tracing/tracing_interface.h"
#include "amsr/someip_daemon_core/tracing/communication_tracing_interface.h"
#include "amsr/someip_daemon_core/tracing/sd_tracing_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace tracing {

/*!
 * \brief Identity and access management adapter.
 */
class TracingAdapter final : public CommunicationTracingInterface, public SdTracingInterface {
 public:
  /*!
   * \brief          Constructor
   * \pre            -
   * \reentrant      FALSE
   * \steady         FALSE
   */
  explicit TracingAdapter(
      std::unique_ptr<someip_daemon::extension_points::tracing::TracingInterface>&& tracing_implementation) noexcept;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief          Destructs instance of TracingAdapter
   * \pre            -
   * \reentrant      FALSE
   * \steady         FALSE
   */
  ~TracingAdapter() noexcept override = default;

  TracingAdapter() = delete;
  TracingAdapter(TracingAdapter const&) = delete;
  TracingAdapter(TracingAdapter&&) = delete;
  TracingAdapter& operator=(TracingAdapter const&) & = delete;
  TracingAdapter& operator=(TracingAdapter&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::NetworkTracingInterface::TraceSomeIpEventReceiveSync
   */
  void TraceSomeIpEventReceiveSync(osabstraction::process::ProcessId const process_id,
                                   someip_protocol::internal::ServiceId const service_id,
                                   someip_protocol::internal::MajorVersion const major_version,
                                   someip_protocol::internal::InstanceId const instance_id,
                                   someip_protocol::internal::EventId const event_id,
                                   std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::NetworkTracingInterface::TracePduEventReceiveSync
   */
  void TracePduEventReceiveSync(osabstraction::process::ProcessId const process_id,
                                someip_protocol::internal::ServiceId const service_id,
                                someip_protocol::internal::InstanceId const instance_id,
                                someip_protocol::internal::EventId const event_id,
                                std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::NetworkTracingInterface::TraceMethodRequestReceiveSync
   */
  void TraceMethodRequestReceiveSync(osabstraction::process::ProcessId const process_id,
                                     someip_protocol::internal::ServiceId const service_id,
                                     someip_protocol::internal::MajorVersion const major_version,
                                     someip_protocol::internal::InstanceId const instance_id,
                                     someip_protocol::internal::MethodId const method_id,
                                     std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::NetworkTracingInterface::TraceMethodResponseReceiveSync
   */
  void TraceMethodResponseReceiveSync(osabstraction::process::ProcessId const process_id,
                                      someip_protocol::internal::ServiceId const service_id,
                                      someip_protocol::internal::MajorVersion const major_version,
                                      someip_protocol::internal::InstanceId const instance_id,
                                      someip_protocol::internal::MethodId const method_id,
                                      std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::ApplicationTracingInterface::TraceSomeIpEventSendSync
   */
  void TraceSomeIpEventSendSync(osabstraction::process::ProcessId const process_id,
                                someip_protocol::internal::ServiceId const service_id,
                                someip_protocol::internal::MajorVersion const major_version,
                                someip_protocol::internal::InstanceId const instance_id,
                                someip_protocol::internal::EventId const event_id,
                                std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::ApplicationTracingInterface::TracePduEventSendSync
   */
  void TracePduEventSendSync(osabstraction::process::ProcessId const process_id,
                             someip_protocol::internal::ServiceId const service_id,
                             someip_protocol::internal::InstanceId const instance_id,
                             someip_protocol::internal::EventId const event_id,
                             std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::ApplicationTracingInterface::TraceMethodRequestSendSync
   */
  void TraceMethodRequestSendSync(osabstraction::process::ProcessId const process_id,
                                  someip_protocol::internal::ServiceId const service_id,
                                  someip_protocol::internal::MajorVersion const major_version,
                                  someip_protocol::internal::InstanceId const instance_id,
                                  someip_protocol::internal::MethodId const method_id,
                                  std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::ApplicationTracingInterface::TraceMethodResponseSendSync
   */
  void TraceMethodResponseSendSync(osabstraction::process::ProcessId const process_id,
                                   someip_protocol::internal::ServiceId const service_id,
                                   someip_protocol::internal::MajorVersion const major_version,
                                   someip_protocol::internal::InstanceId const instance_id,
                                   someip_protocol::internal::MethodId const method_id,
                                   std::size_t const message_size) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::SdTracingInterface::TraceServiceInstanceUpSync
   */
  void TraceServiceInstanceUpSync(osabstraction::process::ProcessId const process_id,
                                  someip_protocol::internal::ServiceId const service_id,
                                  someip_protocol::internal::MajorVersion const major_version,
                                  someip_protocol::internal::InstanceId const instance_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::tracing::SdTracingInterface::TraceServiceInstanceDownSync
   */
  void TraceServiceInstanceDownSync(osabstraction::process::ProcessId const process_id,
                                    someip_protocol::internal::ServiceId const service_id,
                                    someip_protocol::internal::MajorVersion const major_version,
                                    someip_protocol::internal::InstanceId const instance_id) noexcept override;

 private:
  /*!
   * \brief   The actual tracing implementation, provided by the customer.
   * \details In case this feature is disabled, this shall be a nullptr.
   */
  std::unique_ptr<someip_daemon::extension_points::tracing::TracingInterface> tracing_implementation_;
};

}  // namespace tracing
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_TRACING_TRACING_ADAPTER_H_
