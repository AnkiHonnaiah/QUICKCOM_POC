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
/*!        \file  someip_daemon_core/iam/iam_adapter.h
 *        \brief  SOME/IP daemon specific adapter for providing identity and access management (IAM).
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the IAM component to allow optionally
 *                enabling / disabling the component this adapter is defined. In order to provide actual IAM services
 *                it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_IAM_IAM_ADAPTER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_IAM_IAM_ADAPTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/log/logging.h"

namespace amsr {
namespace someip_daemon_core {
namespace iam {

/*!
 * \brief Identity and access management adapter.
 */
class IamAdapter final : public IamInterface {
 public:
  IamAdapter() = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief          Destructs instance of IamAdapter
   * \pre            -
   * \reentrant      FALSE
   * \steady         FALSE
   */
  ~IamAdapter() noexcept override = default;
  IamAdapter(IamAdapter const&) = delete;
  IamAdapter(IamAdapter&&) = delete;
  IamAdapter& operator=(IamAdapter const&) & = delete;
  IamAdapter& operator=(IamAdapter&&) & = delete;

  /*!
   * \brief Constructor of IamAdapter.
   * \param[in] config A "somipd-posix.json" configuration needed to look up the events contained
   * in an event group
   * \param[in] iam    The actual implementation.
   * \pre -
   * \context   Init
   * \reentrant False
   * \steady    FALSE
   *
   */
  IamAdapter(someip_daemon_core::configuration::Configuration const& config,
             std::unique_ptr<someip_daemon::extension_points::iam::IamInterface> iam) noexcept;

  // ----- Remote/Ingress checks -----

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckFindService
   *
   * \steady FALSE
   */
  bool CheckFindService(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                        IamInterface::MinorVersion const minor_version, IamInterface::InstanceId const instance_id,
                        IamInterface::NetworkEndpoint const& sd_sender) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckOfferServices
   *
   * \steady TRUE
   */
  bool CheckOfferService(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                         IamInterface::MinorVersion const minor_version, IamInterface::InstanceId const instance_id,
                         IamInterface::NetworkEndpoint const& sd_sender,
                         ara::core::Optional<IamInterface::NetworkEndpoint> const& tcp_endpoint,
                         ara::core::Optional<IamInterface::NetworkEndpoint> const& udp_endpoint) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckEventgroup
   *
   * \steady TRUE
   */
  bool CheckEventgroup(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                       IamInterface::InstanceId const instance_id, IamInterface::EventgroupId const eventgroup_id,
                       IamInterface::NetworkEndpoint const& sd_sender,
                       ara::core::Optional<IamInterface::NetworkEndpoint> const& tcp_endpoint,
                       ara::core::Optional<IamInterface::NetworkEndpoint> const& udp_endpoint) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckMethod
   *
   * \steady TRUE
   */
  bool CheckMethod(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                   IamInterface::InstanceId const instance_id, IamInterface::MethodId const method_id,
                   IamInterface::NetworkEndpoint const& sender) noexcept override;

  // ----- Local/Egress checks -----

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckFindService
   *
   * \steady FALSE
   */
  bool CheckFindService(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                        IamInterface::MinorVersion const minor_version, IamInterface::InstanceId const instance_id,
                        IamInterface::ApplicationId const application_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckOfferService
   *
   * \steady FALSE
   */
  bool CheckOfferService(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                         IamInterface::MinorVersion const minor_version, IamInterface::InstanceId const instance_id,
                         IamInterface::ApplicationId const application_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckEventSubscription
   *
   * \steady TRUE
   */
  bool CheckEventSubscription(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                              IamInterface::InstanceId const instance_id, IamInterface::EventId const event_id,
                              IamInterface::ApplicationId const application_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \copydoc amsr::someip_daemon_core::iam::IamInterface::CheckMethod
   *
   * \steady TRUE
   */
  bool CheckMethod(IamInterface::ServiceId const service_id, IamInterface::MajorVersion const major_version,
                   IamInterface::InstanceId const instance_id, IamInterface::MethodId const method_id,
                   IamInterface::ApplicationId const application_id) noexcept override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Checks if IAM is enabled or not.
   * \pre -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   * \return "true" if IAM is enabled, "false" otherwise.
   */
  bool IsEnabled() const noexcept override;

 private:
  /*!
   * \brief A configuration.
   */
  someip_daemon_core::configuration::Configuration const& config_;

  /*!
   * \brief IAM member.
   */
  std::unique_ptr<someip_daemon::extension_points::iam::IamInterface> iam_;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription, ara::core::StringView{"IamAdapter"}};
};

}  // namespace iam
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_IAM_IAM_ADAPTER_H_
