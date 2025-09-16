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
/*!        \file   someip_daemon_lite/required_service_instance.h
 *         \brief  Required service instance (delegator).
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::RequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_REQUIRED_SERVICE_INSTANCE_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_REQUIRED_SERVICE_INSTANCE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/someip_daemon_lite/internal/required_service_instance_impl.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_lite {

/*!
 * \brief Required service instance.
 * \details Receives notifications when service instances are found.
 *          Allows creation of clients based on discovered service instance IDs.
 * \vpublic
 */
class RequiredServiceInstance final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] implementation The required service instance implementation
   * \vprivate Vector product internal API.
   * \steady FALSE
   */
  explicit RequiredServiceInstance(amsr::UniquePtr<internal::RequiredServiceInstanceImpl>&& implementation) noexcept;

  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstance() noexcept = default;

  /*!
   * \brief Move constructor.
   * \details Needed for compatibility with \p ara::core::Result.
   *          Member \p implementation_ is set to "nullptr" after the invocation of this function.
   * \vprivate Vector product internal API.
   * \steady FALSE
   */
  RequiredServiceInstance(RequiredServiceInstance&&) noexcept = default;

  RequiredServiceInstance(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance const&) = delete;
  RequiredServiceInstance& operator=(RequiredServiceInstance&&) = delete;

  /*!
   * \brief Create a client based on a provided instance ID.
   * \vpublic
   * \param[in] instance_id Provided instance ID.
   *
   * \return \p ara::core::Result with an instance of "SomeIpClient" or an error.
   * \error SomeIpDaemonLiteErrc::kSystemConfigurationNotAvailable Remote server not found.
   * \error SomeIpDaemonLiteErrc::kRuntimeResourceOutOf Maximum number of simultaneous clients reached.
   *
   * \pre \p instance_id shall be the same as the one used to create this required service instance
   *      except when the ID represents all IDs.
   * \pre This call shall not be invoked on a moved instance.
   *
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Result<SomeIpClient> CreateSomeIpClient(ProvidedInstanceId const instance_id) const noexcept;

 private:
  /*!
   * \brief Pointer to the implementation's instance.
   */
  amsr::UniquePtr<internal::RequiredServiceInstanceImpl> implementation_;
};

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_REQUIRED_SERVICE_INSTANCE_H_
