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
/*!        \file
 *         \brief  Required service instance (implementation).
 *         \unit   SomeIpBinding::SomeIpBindingLite::Client::SomeIpBindingLiteRequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/required_service_instance.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_lite/provided_instance_id.h"
#include "amsr/someip_binding_lite/someip_client.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief   SOME/IP Binding Lite Required Service Instance (implementation).
 * \details This wrapper class serves as an intermediary between the user of SOME/IP Binding Lite
 *          and the core module's specific implementation. The goal is to present an isolated interface,
 *          shielding users from potential changes in the core module's internals and preserving the
 *          stability of public APIs.
 *          This wrapper primarily manages notifications when service instances are discovered. Additionally,
 *          it facilitates the creation of clients using identified service instance IDs.
 */
class RequiredServiceInstanceImpl final : public internal::ServiceListenerInterfaceType,
                                          public ::std::enable_shared_from_this<RequiredServiceInstanceImpl> {
 public:
  /*!
   * \brief       Constructor.
   *
   * \param[in]   core_required_service_instance_wrapper    The SOME/IP binding core required service instance wrapper.
   * \param[in]   required_service_instance_id              The required service instance ID.
   *
   * \pre -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RequiredServiceInstanceImpl(
      ::amsr::someip_binding_core::internal::RequiredServiceInstance&& core_required_service_instance_wrapper,
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& required_service_instance_id) noexcept;

  /*!
   * \brief       Destructor.
   *
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~RequiredServiceInstanceImpl() noexcept final;

  RequiredServiceInstanceImpl(RequiredServiceInstanceImpl&&) = delete;
  RequiredServiceInstanceImpl(RequiredServiceInstanceImpl const&) = delete;
  RequiredServiceInstanceImpl& operator=(RequiredServiceInstanceImpl const&) = delete;
  RequiredServiceInstanceImpl& operator=(RequiredServiceInstanceImpl&&) = delete;

  /*!
   * \brief       Get the list of found services.
   *
   * \return      A list with provided instance IDs of found services.
   *
   * \pre -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ara::core::Vector<ProvidedInstanceId> GetFoundServices() const noexcept;

  /*!
   * \brief     Create a client based on a provided instance ID.
   *
   * \param[in] instance_id Provided instance ID.
   *
   * \return \p amsr::core::Result with an instance of SOME/IP client or an error.
   * \error SomeIpBindingLiteErrc::kSystemPrivilegesNoAccess If system privileges do not provide resource access.
   * \error SomeIpBindingLiteErrc::kRuntimeBufferDeserializationError If buffer deserialization fails.
   * \error SomeIpBindingLiteErrc::kRuntimeConnectionLost If connection is lost.
   * \error SomeIpBindingLiteErrc::kRuntimeBufferInvalid If an invalid buffer is encountered.
   * \error SomeIpBindingLiteErrc::kUnknown If an unknown error occurs.
   *
   * \pre \p instance_id shall be the same as the one used to create this required
   *      service instance except when the required service instance is created
   *      with an ID that represents all IDs.
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  amsr::core::Result<::amsr::someip_binding_lite::SomeIpClient> CreateClient(
      ProvidedInstanceId const instance_id) noexcept;

  /*!
   * \brief       Callback for the service instance up event.
   *
   * \param[in]   offered_service Service's instance that becomes available.
   *
   * \context     Reactor
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnServiceInstanceUp(internal::ProvidedServiceInstanceIdType const& offered_service) noexcept final;

  /*!
   * \brief       Callback for the service instance down event.
   *
   * \param[in]   stopped_service Service's instance that becomes unavailable.
   *
   * \pre         -
   *
   * \context     Reactor
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void OnServiceInstanceDown(internal::ProvidedServiceInstanceIdType const& stopped_service) noexcept final;

  /*!
   * \brief Register a listener.
   *
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterListener() noexcept;

  /*!
   * \brief Unregister the listener.
   *
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void UnregisterListener() noexcept;

 private:
  /*!
   * \brief The wrapped SOME/IP binding core required service instance.
   */
  ::amsr::someip_binding_core::internal::RequiredServiceInstance core_required_service_instance_wrapper_;

  /*!
   * \brief Required service instance.
   */
  ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const required_service_instance_id_;

  /*!
   * \brief List of found service instance IDs.
   */
  ara::core::Vector<ProvidedInstanceId> found_service_instance_ids_{};

  /*!
   * \brief Lock for avoiding concurrent access to the list of found service instance IDs.
   */
  mutable std::mutex found_service_instance_ids_lock_{};

  /*!
   * \brief The logger.
   */
  someip_binding_core::internal::logging::AraComLogger const logger_;
};

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_REQUIRED_SERVICE_INSTANCE_IMPL_H_

/*!
 * \exclusivearea ::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl::found_service_instance_ids_lock_,
 *                Used for protecting parallel access to the required service instance impl by App (read) and Reactor
 *                (write).
 *
 * \protects ::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl::found_service_instance_ids_,
 *           is the protected resource of a RequiredServiceInstanceImpl object.
 *
 * \usedin ::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl::OnServiceInstanceUp
 * \usedin ::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl::OnServiceInstanceDown
 * \usedin ::amsr::someip_binding_lite::internal::RequiredServiceInstanceImpl::GetFoundServices
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length MEDIUM Limited to a vector lookup, an emplacement on service instance up,
 *                a removal on service instance down or
 *                a complete copy of the vector.
 *                The exclusive area starts and ends always in the smallest possible scope within the protected methods.
 *                This exclusive area covers calls to at most one method. The complexity depends in the size of
 *                the container.
 * \endexclusivearea
 */
