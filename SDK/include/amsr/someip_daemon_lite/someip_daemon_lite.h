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
/*!        \file   someip_daemon_lite/someip_daemon_lite.h
 *         \brief  Direct access provider to local SOME/IP client (delegator).
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::SomeIpDaemonLite
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_DAEMON_LITE_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_DAEMON_LITE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/someip_daemon_lite/data_types.h"
#include "amsr/someip_daemon_lite/internal/someip_daemon_lite_impl.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_lite {

/*!
 * \brief Direct access provider to local SOME/IP client.
 * \vpublic
 */
class SomeIpDaemonLite final {
 public:
  /*!
   * \brief Create class instance.
   * \param[in] required_service_instance_manager The required service instance manager from the private
   *            SomeIpDaemonCore library.
   * \vpublic
   *
   * \return \p ara::core::Result with an instance of "SomeIpDaemonLite" or an error.
   * \error SomeIpDaemonLiteErrc::* No error is returned.
   *
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static ara::core::Result<SomeIpDaemonLite> Create(
      someip_daemon_core::client::RequiredServiceInstanceManagerInterface const&
          required_service_instance_manager) noexcept;

  /*!
   * \brief Constructor.
   * \param[in] implementation The SomeIpDaemonLite implementation.
   * \vprivate Vector product internal API.
   * \steady   FALSE
   */
  explicit SomeIpDaemonLite(amsr::UniquePtr<internal::SomeIpDaemonLiteImpl>&& implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  ~SomeIpDaemonLite() noexcept = default;

  /*!
   * \brief Move constructor.
   * \details Needed for compatibility with \p ara::core::Result.
   *          Member \p implementation_ is set to "nullptr" after the invocation of this function.
   * \vprivate Vector product internal API.
   * \steady   FALSE
   */
  SomeIpDaemonLite(SomeIpDaemonLite&&) noexcept = default;

  SomeIpDaemonLite(SomeIpDaemonLite const&) = delete;
  SomeIpDaemonLite& operator=(SomeIpDaemonLite const&) = delete;
  SomeIpDaemonLite& operator=(SomeIpDaemonLite&&) = delete;

  /*!
   * \brief Add service instance to the list of required instances.
   * \vpublic
   * \param[in] service_id Service ID.
   * \param[in] required_instance_id Required instance ID.
   * \param[in] major_interface_version Major interface version.
   * \param[in] minor_interface_version Minor interface version.
   *
   * \return \p ara::core::Result with an instance of "RequiredServiceInstance" or an error.
   * \error SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable Required service instance not available.
   *
   * \pre \p required_instance_id shall not represent all IDs.
   * \pre This call shall not be invoked on a moved instance.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<RequiredServiceInstance> CreateRequiredServiceInstance(
      ServiceId const service_id, RequiredInstanceId const& required_instance_id,
      MajorInterfaceVersion const major_interface_version,
      MinorInterfaceVersion const minor_interface_version) const noexcept;

 private:
  /*!
   * \brief Pointer to the implementation's instance.
   */
  amsr::UniquePtr<internal::SomeIpDaemonLiteImpl> implementation_;
};

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_SOMEIP_DAEMON_LITE_H_
