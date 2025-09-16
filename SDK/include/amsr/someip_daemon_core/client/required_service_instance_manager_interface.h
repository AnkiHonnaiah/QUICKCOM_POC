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
/**        \file  required_service_instance_manager_interface.h
 *        \brief  Required Service Instance Manager Interface abstract class.
 *
 *      \details  This class has been made for testing purposes (for mocking).
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon_core/client/required_service_instance_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_interface.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief RequiredServiceInstanceManagerInterface.
 */
class RequiredServiceInstanceManagerInterface {
 protected:
  /*!
   * \brief shared pointer to the Required Service Instance Interface.
   */
  using RequiredServiceInstanceInterfaceSharedPtr = std::shared_ptr<RequiredServiceInstanceInterface>;

  /*!
   * \brief Map of locally required service instances.
   */
  using ServiceMap = ara::core::Map<configuration::types::SomeIpServiceInstanceIdCommunication,
                                    RequiredServiceInstanceInterfaceSharedPtr>;

 public:
  /*!
   * \brief   Constructor.
   * \steady  FALSE
   */
  RequiredServiceInstanceManagerInterface() noexcept = default;

  /*!
   * \brief   Destructor.
   * \steady  FALSE
   */
  virtual ~RequiredServiceInstanceManagerInterface() noexcept = default;

  RequiredServiceInstanceManagerInterface(RequiredServiceInstanceManagerInterface const&) = delete;
  RequiredServiceInstanceManagerInterface(RequiredServiceInstanceManagerInterface&&) = delete;
  RequiredServiceInstanceManagerInterface& operator=(RequiredServiceInstanceManagerInterface const&) & = delete;
  RequiredServiceInstanceManagerInterface& operator=(RequiredServiceInstanceManagerInterface&&) & = delete;

  /*!
   * \brief Return the required service instance with the given ID.
   *
   * \param[in] service_id        Service ID of the required service instance.
   * \param[in] instance_id       Instance ID of the required service instance.
   * \param[in] major_version     Major Version of the required service instance.
   * \param[in] minor_version     Minor Version of the required service instance.
   *
   * \return Shared pointer to the RequiredServiceInstanceInterface, or nullptr if not found.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual RequiredServiceInstanceInterfaceSharedPtr GetRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id) const noexcept = 0;

  /*!
   * \brief Return "true" if emplacement is successful, else "false".
   *
   * \param[in] service_deployment_id  SOME/IP Service deployment ID of the required service instance.
   * \param[in] instance_id            SOME/IP Service Instance ID of the required service instance.
   * \param[in] rsi_interface_ptr      The Required Service Instance Interface pointer.
   *
   * \return boolean value.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual bool EmplaceRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
      RequiredServiceInstanceInterfaceSharedPtr const& rsi_interface_ptr) noexcept = 0;

  /*!
   * \brief Erase the Required Service Instance from the container.
   *
   * \param[in] service_deployment_id  SOME/IP Service deployment ID of the required service instance.
   * \param[in] instance_id            SOME/IP Service Instance ID of the required service instance.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void EraseRequiredServiceInstance(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_MANAGER_INTERFACE_H_
