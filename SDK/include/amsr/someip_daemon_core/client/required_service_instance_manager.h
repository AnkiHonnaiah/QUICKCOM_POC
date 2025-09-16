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
/**        \file  required_service_instance_manager.h
 *        \brief  Required Service Instance Manager.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/required_service_instance_interface.h"
#include "amsr/someip_daemon_core/client/required_service_instance_manager_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_interface.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RequiredServiceInstanceManager.
 */
class RequiredServiceInstanceManager final : public RequiredServiceInstanceManagerInterface {
 public:
  /*!
   * \brief 	Default constructor.
   * \steady 	FALSE
   */
  RequiredServiceInstanceManager() = default;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief 	Destructor.
   * \steady 	FALSE
   */
  ~RequiredServiceInstanceManager() noexcept override;

  RequiredServiceInstanceManager(RequiredServiceInstanceManager const&) = delete;
  RequiredServiceInstanceManager(RequiredServiceInstanceManager&&) = delete;
  RequiredServiceInstanceManager& operator=(RequiredServiceInstanceManager const&) & = delete;
  RequiredServiceInstanceManager& operator=(RequiredServiceInstanceManager&&) & = delete;

  /*!
   * \brief Return the required service instance with the given ID.
   *
   * \param[in] service_instance_id     A  service instance id containing service_id, instance_id, major_version and
   *                                    minor_version.
   *
   * \return Shared pointer to the RequiredServiceInstanceInterface, or nullptr if not found.
   *
   * \context 	App
   * \reentrant FALSE
   * \steady 		FALSE
   */
  RequiredServiceInstanceInterfaceSharedPtr GetRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id) const noexcept final;

  /*!
   * \brief Return true if emplacement is success else false.
   *
   * \param[in] service_instance_id    SOME/IP Service instance ID of the required service instance.
   * \param[in] rsi_interface_ptr      The Required Service Instance Interface pointer.
   *
   * \return boolean value.
   *
   * \context 	App
   * \reentrant FALSE
   * \steady 		FALSE
   */
  bool EmplaceRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
      RequiredServiceInstanceInterfaceSharedPtr const& rsi_interface_ptr) noexcept final;

  /*!
   * \brief Erase the Required Service Instance from the container.
   *
   * \param[in] service_deployment_id  SOME/IP Service Instance ID of the required service instance.
   *
   * \context 	App
   * \reentrant FALSE
   * \steady 		FALSE
   */
  void EraseRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id) noexcept final;

 private:
  /*!
   * \brief Find matching required service instance.
   *
   * \details Matching required service instance must have the exact same service ID, and either the same instance ID or
   *          instance ID = ALL.
   * \param[in] service_instance_id   Service ID.
   *
   * \return iterator to the matching service instance, or service_instances_.end() if not found.
   * \steady FALSE
   */
  typename ServiceMap::const_iterator FindMatchingServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id) const noexcept;

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kRequiredServiceInstanceManagerLoggerContextId,
      someip_daemon_core::logging::kRequiredServiceInstanceManagerLoggerContextDescription,
      ara::core::StringView{"RequiredServiceInstanceManager"}};

  /*!
   * \brief Map of required service instances.
   */
  ServiceMap service_instances_{};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_MANAGER_H_
