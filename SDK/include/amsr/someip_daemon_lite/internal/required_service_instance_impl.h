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
/*!        \file   internal/required_service_instance_impl.h
 *         \brief  Required service instance (implementation).
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::RequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/someip_daemon_core/client/required_service_instance_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_lite/data_types.h"
#include "amsr/someip_daemon_lite/internal/required_service_instance_id.h"
#include "amsr/someip_daemon_lite/internal/service_discovery_offer_service_observer.h"
#include "amsr/someip_daemon_lite/provided_instance_id.h"
#include "amsr/someip_daemon_lite/required_instance_id.h"
#include "amsr/someip_daemon_lite/someip_client.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief Required service instance (internal implementation).
 * \details Allows creation of SOME/IP clients.
 * \vprivate Component Private
 */
class RequiredServiceInstanceImpl final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] client_manager Client manager.
   * \param[in] service_id Service ID.
   * \param[in] required_instance_id Required instance ID.
   * \param[in] major_interface_version Major interface version.
   * \param[in] minor_interface_version Minor interface version.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  RequiredServiceInstanceImpl(
      std::shared_ptr<someip_daemon_core::client::RequiredServiceInstanceInterface> sptr_required_service_instance,
      ServiceId const service_id, RequiredInstanceId const& required_instance_id,
      MajorInterfaceVersion const major_interface_version,
      MinorInterfaceVersion const minor_interface_version) noexcept;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   * \steady   FALSE
   */
  ~RequiredServiceInstanceImpl() noexcept;

  RequiredServiceInstanceImpl(RequiredServiceInstanceImpl&&) = delete;
  RequiredServiceInstanceImpl(RequiredServiceInstanceImpl const&) = delete;
  RequiredServiceInstanceImpl& operator=(RequiredServiceInstanceImpl const&) = delete;
  RequiredServiceInstanceImpl& operator=(RequiredServiceInstanceImpl&&) = delete;

  /*!
   * \brief Create a client based on a provided instance ID.
   * \param[in] instance_id Provided instance ID.
   *
   * \return \p ara::core::Result with an instance of SOME/IP client or an error.
   * \error SomeIpDaemonLiteErrc::* Translate and forwards the error from
   *        someip_daemon_core::client::RequiredServiceInstance::RequestService().
   *
   * \pre \p instance_id shall be the same as the one used to create this required
   *      service instance except when the required service instance is created
   *      with an ID that represents all IDs.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   * \steady      FALSE
   */
  ara::core::Result<SomeIpClient> CreateSomeIpClient(ProvidedInstanceId const instance_id) noexcept;

 private:
  /*!
   * \brief Shared reference towards an instance of a required service.
   */
  std::shared_ptr<someip_daemon_core::client::RequiredServiceInstanceInterface> sptr_required_service_instance_;

  /*!
   * \brief Required service instance.
   */
  RequiredServiceInstanceId required_service_instance_id_;

  /*!
   * \brief Observer (callbacks' receiver) for service discovery related events.
   */
  ServiceDiscoveryOfferServiceObserver offer_service_observer_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger const logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                          someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                          ara::core::StringView{"RequiredServiceInstanceImpl"}};
};

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_
