/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  SpeedMonitorExecutable/include/services/speedlimitservice/speedlimitservice_skeleton.h
 *        \brief  Skeleton for service 'SpeedLimitService'.
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_socal
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

#ifndef SPEEDMONITOREXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_H_
#define SPEEDMONITOREXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_H_

/*!
 * \trace SPEC-4980239
 */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/definitions.h"
#include "amsr/socal/events/skeleton_event.h"
#include "amsr/socal/fields/skeleton_field.h"
#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method.h"
#include "amsr/socal/internal/methods/skeleton_method.h"
#include "amsr/socal/skeleton.h"
#include "ara/core/future.h"
#include "ara/core/instance_specifier.h"
#include "services/speedlimitservice/SpeedLimitService_skeleton_backend_interface.h"
#include "services/speedlimitservice/SpeedLimitService_types.h"
#include "services/speedlimitservice/speedlimitservice_common.h"

/*!
 * \trace SPEC-4980240
 */
namespace services {
namespace speedlimitservice {
/*!
 * \trace SPEC-4980241
 */
namespace skeleton {
/*!
 * \brief Forward declaration for inserting as a type into the template class SkeletonEvent
 */
class SpeedLimitServiceSkeleton;

/*!
 * \trace SPEC-4980244
 */
namespace methods {

}  // namespace methods

/*!
 * \trace SPEC-4980243
 */
namespace events {

/*!
 * \brief Type alias for service event 'SpeedOverLimitEvent', that is part of the skeleton.
 *
 * \trace SPEC-4980342
 */
using SpeedOverLimitEvent = ::amsr::socal::events::SkeletonEvent<
    services::speedlimitservice::skeleton::SpeedLimitServiceSkeleton,
    ::datatypes::OverLimitEnum,
    services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface,
    ::amsr::socal::internal::events::SkeletonEventManagerInterface<::datatypes::OverLimitEnum>,
    &services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface::GetEventManagerSpeedOverLimitEvent>;

}  // namespace events

/*!
 * \trace SPEC-4980245
 */
namespace fields {

}  // namespace fields

/*!
 * \brief Skeleton interface class for the service 'SpeedLimitService'.
 *
 * \vpublic
 * \trace SPEC-4980341
 * \unit Socal::Skeleton::Skeleton
 * \complexity The generated skeleton class may contain multiple Field, Methods or Events. This
 *             eventually heading towards an increased number of inter unit calls. The calltree complexity and weighted
 *             complexity are slightly above threshold value with a low risk.
 */
// VECTOR NC Metric-OO.WMC.One: MD_SOCAL_Metric.OO.WMC.One_GeneratedClass
class SpeedLimitServiceSkeleton
    : public ::amsr::socal::Skeleton<services::speedlimitservice::SpeedLimitService,
                                          services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface,
                                          ::amsr::socal::kRuntimeProcessingMode> {
 public:
// ---- Constructors / Destructors -----------------------------------------------------------------------------------
/*!
 * \brief Exception-less pre-construction of SpeedLimitService.
 *
 * \param[in] instance_id The InstanceIdentifier of the provided service instance.
 *                        The given InstanceIdentifier must fulfill the following preconditions:
 *                        - Must be configured in the ARXML model.
 *                        - Must belong to the service interface.
 * \param[in] mode The mode of the service implementation for processing service method invocations.
 *                 Default: Event-driven processing.
 *                 Preconditions to be fulfilled:
 *                 - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool must have exactly one
 *                   worker thread configured.
 * \param[in] max_requests The max number of method requests that can be queued per skeleton instance.
 *                  Default: 0 - No upper limit for incoming method requests.
 *                  Usage hint:
 *                  - For kEvent/kEventSingleThreaded mode, the sum of values used for all skeleton instances must not
 *                    exceed the corresponding threadpool's max_number_of_tasks to avoid memory wastage. If a User-Defined
 *                    Thread Pool is not configured for this skeleton, then the P-Port Default Thread Pool is used
 *                    which has a max task limit of 1024.
 * \return Result<ConstructionToken> Result containing construction token from which a skeleton object can be
 *         constructed.
 * \error  ComErrc::kGrantEnforcementError If the request is refused by IAM.
 * \error  ComErrc::kExceedQueueSize If the given max_requests value exceeds the max_number_of_tasks configured for the
 *                                   threadpool.
 * \error  ComErrc::kCommunicationLinkError If there was a connection failure.
 * \pre No other service skeleton for the same instance must exist concurrently. After destruction of an already
 *      created skeleton object, the instantiation for the same service instance will be possible.
 * \pre If the given processing mode is set to kEventSingleThread, the assigned threadpool must be configured  with
 *      only one thread.
 * \pre The given instance identifier must be configured in the ARXML model.
 * \pre The given instance identifier must belong to the service interface.
 * \context App
 * \steady FALSE
 * \threadsafe FALSE against all overloads of Preconstruct.
 * \reentrant FALSE against all overloads of Preconstruct.
 * \vpublic
 * \synchronous TRUE
 * \trace SPEC-8053551, SPEC-8053550
 */
static ConstructionResult Preconstruct(
    ::ara::com::InstanceIdentifier instance_id,
    ::ara::com::MethodCallProcessingMode const mode = ::ara::com::MethodCallProcessingMode::kEvent,
    std::size_t const max_requests = 0) noexcept;

/*!
 * \brief Exception-less pre-construction of SpeedLimitServiceSkeleton.
 *
 * \param[in] instance The InstanceSpecifier of the provided service interface.
 *                     The given InstanceSpecifier must fulfill the following preconditions:
 *                     - Must be configured in the ARXML model.
 *                     - Must belong to the service interface.
 * \param[in] mode The mode of the service implementation for processing service method invocations.
 *                 Default: Event-driven processing.
 *                 Preconditions to be fulfilled:
 *                 - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool must have exactly one
 *                   worker thread configured.
 * \param[in] max_requests The max number of method requests that can be queued per skeleton instance.
 *                  Default: 0 - No upper limit for incoming method requests.
 *                  Usage hint:
 *                  - For kEvent/kEventSingleThreaded mode, the sum of values used for all skeleton instances must not
 *                    exceed the corresponding threadpool's max_number_of_tasks to avoid memory wastage. If a User-Defined
 *                    Thread-Pool is not configured for this skeleton, then the P-Port Default Thread Pool is used which
 *                    has a max task limit of 1024.
 * \return Result<ConstructionToken> Result containing construction token from which a skeleton object can be
 *         constructed.
 * \error  ComErrc::kGrantEnforcementError If the request is refused by IAM.
 * \error  ComErrc::kExceedQueueSize If the given max_requests value exceeds the max_number_of_tasks configured for the
 *                                   threadpool.
 * \error  ComErrc::kCommunicationLinkError If there was a connection failure.
 * \pre No other service skeleton for the same instance must exist concurrently. After destruction of an already
 *      created skeleton object, the instantiation for the same service instance will be possible.
 * \pre If the given processing mode is set to kEventSingleThread, the assigned threadpool must be configured  with
 *      only one thread.
 * \pre The given instance specifier must be configured in the ARXML model.
 * \pre The given instance specifier must belong to the service interface.
 * \context App
 * \steady FALSE
 * \threadsafe FALSE against all overloads of Preconstruct.
 * \reentrant FALSE against all overloads of Preconstruct.
 * \vpublic
 * \synchronous TRUE
 * \trace SPEC-8053553, SPEC-8053550
 */
static ConstructionResult Preconstruct(
    ::ara::core::InstanceSpecifier instance,
    ::ara::com::MethodCallProcessingMode const mode = ::ara::com::MethodCallProcessingMode::kEvent,
    std::size_t const max_requests = 0) noexcept;

/*!
 * \brief Exception-less pre-construction of SpeedLimitServiceSkeleton.
 *
 * \param[in] instance_identifiers The container of instance identifiers of the provided instances of a service,
 *                                 each instance element needed to distinguish different instances of exactly the same
 *                                 service in the system.
 *                                 The given InstanceIdentifierContainer must fulfill the following preconditions:
 *                                 - Every InstanceIdentifier of the container must be configured in the ARXML model.
 *                                 - Every InstanceIdentifier of the container must belong to the service interface.
 *                                 - The container must not be empty.
 *                                 - All elements of the container must be unique (no duplicates).
 * \param[in] mode The mode of the service implementation for processing service method invocations.
 *                 Default: Event-driven processing.
 *                 Preconditions to be fulfilled:
 *                 - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool must have exactly one
 *                   worker thread configured.
 * \param[in] max_requests The max number of method requests that can be queued per skeleton instance.
 *                  Default: 0 - No upper limit for incoming method requests.
 *                  Usage hint:
 *                  - For kEvent/kEventSingleThreaded mode, the sum of values used for all skeleton instances must not
 *                    exceed the corresponding Thread-Pools's max_number_of_tasks to avoid memory wastage. If a User-Defined
 *                    Thread Pool is not configured for this skeleton, then the P-Port Default Thread Pool is used which
 *                    has a max task limit of 1024.
 * \return Result<ConstructionToken> Result containing construction token from which a skeleton object can be
 *         constructed.
 * \error  ComErrc::kGrantEnforcementError If the request is refused by IAM.
 * \error  ComErrc::kExceedQueueSize If the given max_requests value exceeds the max_number_of_tasks configured for the
 *                                   threadpool.
 * \error  ComErrc::kCommunicationLinkError If there was a connection failure.
 * \pre No other service skeleton for the same instance must exist concurrently. After destruction of an already
 *      created skeleton object, the instantiation for the same service instance will be possible.
 * \pre If the given processing mode is set to kEventSingleThread, the assigned threadpool must be configured  with
 *      only one thread.
 * \pre The given InstanceIdentifierContainer shall not be empty and shall not contain duplicates.
 * \pre The given instance identifiers must be configured in the ARXML model.
 * \pre The given instance identifiers must belong to the service interface.
 * \context App
 * \steady FALSE
 * \threadsafe FALSE against all overloads of Preconstruct.
 * \reentrant FALSE against all overloads of Preconstruct.
 * \vpublic
 * \synchronous TRUE
 * \trace SPEC-8053555, SPEC-8053550
 */
static ConstructionResult Preconstruct(
    ::ara::com::InstanceIdentifierContainer instance_identifiers,
    ::ara::com::MethodCallProcessingMode const mode = ::ara::com::MethodCallProcessingMode::kEvent,
    std::size_t const max_requests = 0) noexcept;

  /*!
   * \brief   Exception-less constructor of SpeedLimitServiceSkeleton.
   * \remark Skeleton instances created by the application must be destroyed before calling the 
   *         ara::core::Deinitialize() API. Any violations to this will lead to process termination 
   *         through ara::core::Abort().
   * \param[in] token  ConstructionToken created with Preconstruct() API.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053551, SPEC-8053553, SPEC-8053555
   */
  explicit SpeedLimitServiceSkeleton(ConstructionToken&& token) noexcept;

  /*!
   * \brief Delete default constructor.
   */
  SpeedLimitServiceSkeleton() = delete;
  /*!
   * \brief Delete copy constructor.
   */
  SpeedLimitServiceSkeleton(SpeedLimitServiceSkeleton const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  SpeedLimitServiceSkeleton(SpeedLimitServiceSkeleton&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  SpeedLimitServiceSkeleton& operator=(SpeedLimitServiceSkeleton const&) & = delete;
  /*!
   * \brief Delete move assignment.
   */
  SpeedLimitServiceSkeleton& operator=(SpeedLimitServiceSkeleton&&) & = delete;

  /*!
   * \brief Constructor of SpeedLimitServiceSkeleton.
   *
   * \remark Skeleton instances created by the application must be destroyed before calling the 
   *         ara::core::Deinitialize() API. Any violations to this will lead to process termination 
   *         through ara::core::Abort().
   *
   * \param[in] instance The identifier of a specific instance of a service, needed to distinguish different instances
   *                     of exactly the same service in the system.
   *
   * \param[in] mode The mode of the service implementation for processing service method invocations.
   *                 Default: Event-driven processing
   *
   * \pre No other service skeleton for the same instance must exist concurrently. After destruction of an already
   *      created skeleton object, the instantiation for the same service instance will be possible.
   *
   * \context App
   * \steady FALSE
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   * \synchronous TRUE
   * \trace SPEC-4980351, SPEC-4980356
   */
   explicit SpeedLimitServiceSkeleton(
      ::ara::com::InstanceIdentifier instance,
      ::ara::com::MethodCallProcessingMode mode = ::ara::com::MethodCallProcessingMode::kEvent) noexcept;

  /*!
   * \brief Constructor of SpeedLimitServiceSkeleton.
   *
   * \remark Skeleton instances created by the application must be destroyed before calling the 
   *         ara::core::Deinitialize() API. Any violations to this will lead to process termination 
   *         through ara::core::Abort().
   *
   * \param[in] instance The InstanceSpecifier of a specific service instance, needed to distinguish different instances
   *                     of exactly the same service in the system.
   *
   * \param[in] mode The mode of the service implementation for processing service method invocations.
   *                 Default: Event-driven processing
   *
   * \pre No other service skeleton for the same instance must exist concurrently. After destruction of an already
   *      created skeleton object, the instantiation for the same service instance will be possible.
   *
   * \context App
   * \steady FALSE
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   * \synchronous TRUE
   * \trace SPEC-8053552, SPEC-4980356
   */
   explicit SpeedLimitServiceSkeleton(
      ::ara::core::InstanceSpecifier instance,
      ::ara::com::MethodCallProcessingMode mode = ::ara::com::MethodCallProcessingMode::kEvent) noexcept;

  /*!
   * \brief Constructor of SpeedLimitServiceSkeleton.
   *
   * \remark Skeleton instances created by the application must be destroyed before calling the 
   *         ara::core::Deinitialize() API. Any violations to this will lead to process termination 
   *         through ara::core::Abort().
   *
   * \param[in] instance_identifiers The container of instances of a service, each instance element needed to
   *                                 distinguish different instances of exactly the same service in the system.
   *
   * \param[in] mode The mode of the service implementation for processing service method invocations.
   *                 Default: Event-driven processing
   *
   * \pre No other service skeleton for the same instance must exist concurrently. After destruction of an already
   *      created skeleton object, the instantiation for the same service instance will be possible.
   *
   * \context App
   * \steady FALSE
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   * \synchronous TRUE
   * \trace SPEC-8053554, SPEC-4980356
   */
   explicit SpeedLimitServiceSkeleton(
      ::ara::com::InstanceIdentifierContainer instance_identifiers,
      ::ara::com::MethodCallProcessingMode mode = ::ara::com::MethodCallProcessingMode::kEvent) noexcept;

  /*!
   * \brief Destroys the skeleton.
   * \pre         The service must have been stopped via StopOfferService() prior to the destruction.
   * \context     App
   * \threadsafe  FALSE for same class instance, FALSE for different instances.
   *              FALSE against all overloads of Preconstruct API for same/different class instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   *              FALSE against all overloads of Preconstruct API for same/different class instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual ~SpeedLimitServiceSkeleton() noexcept;

  // ---- Methods --------------------------------------------------------------------------------------------------

  // ---- Events ---------------------------------------------------------------------------------------------------

  // VECTOR NC VectorC++-V11.0.2: MD_SOCAL_V11-0-2_public_member_in_generated_code
  /*!
   * \brief The skeleton event 'SpeedOverLimitEvent' which can be used by application developer.
   * \details 
   * Data of type ::datatypes::OverLimitEnum
   * \vpublic
   */
  events::SpeedOverLimitEvent SpeedOverLimitEvent;

  // ---- Fields ---------------------------------------------------------------------------------------------------

 private:
  /*!
   * \brief Type alias for the base class.
   */
  using Base = ::amsr::socal::Skeleton<services::speedlimitservice::SpeedLimitService, services::speedlimitservice::internal::SpeedLimitServiceSkeletonBackendInterface, ::amsr::socal::kRuntimeProcessingMode>;

  /*!
   * \copydoc Base::DoFieldInitializationChecks()
   */
  void DoFieldInitializationChecks() noexcept override;

};  // class SpeedLimitServiceSkeleton

}  // namespace skeleton
}  // namespace speedlimitservice
}  // namespace services

#endif  // SPEEDMONITOREXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_H_

