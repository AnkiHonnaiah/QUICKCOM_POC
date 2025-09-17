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
/*!        \file  thread_pools_manager.h
 *        \brief  Contains the module that manages SOCAL threadpools.
 *      \details  -
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOLS_MANAGER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOLS_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/optional.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/thread_pool.h"
#include "amsr/socal/internal/thread_pool_task.h"
#include "amsr/socal/internal/thread_pools_manager_interface.h"
#include "vac/container/static_vector.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Thread pool management module.
 *
 * \unit Socal::ThreadPool::ThreadPoolsManager
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class ThreadPoolsManager final : public ThreadPoolsManagerInterface {
 public:
  /*!
   * \brief Type alias for the base class.
   */
  using Base = ThreadPoolsManagerInterface;

  /*!
   * \brief Returns a singleton instance of ThreadPoolsManager class.
   * \return Singleton wrapped ThreadPoolsManager instance.
   * \pre         -
   * \context     ANY
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  static ::amsr::generic::Singleton<ThreadPoolsManager>& getSingletonInstance() noexcept;

  /*!
   * \brief Default constructor. The default constructor shall not be used directly by the user. The user shall create
   *        the ThreadPoolsManager object using singleton approach (during the initialization phase).
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ThreadPoolsManager() noexcept = default;

  /*!
   * \brief Delete copy constructor.
   */
  ThreadPoolsManager(ThreadPoolsManager const&) noexcept = delete;

  /*!
   * \brief Delete move constructor.
   */
  ThreadPoolsManager(ThreadPoolsManager&&) noexcept = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  ThreadPoolsManager& operator=(ThreadPoolsManager const&) & noexcept = delete;

  /*!
   * \brief Delete move assignment operator.
   */
  ThreadPoolsManager& operator=(ThreadPoolsManager&&) & noexcept = delete;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT, VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
  ~ThreadPoolsManager() noexcept final = default;

  /*!
   * \brief Initializes the thread-pools.
   * \param[in] instance_specifier_lookup_table  The table used to validate the user-defined thread pool configuration.
   * \param[in] config                           The configuration object containing the thread pool configuration.
   * \pre         -
   * \context     Init
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void InitializeThreadPools(
      ::amsr::socal::internal::InstanceSpecifierLookupTable const& instance_specifier_lookup_table,
      ::amsr::socal::internal::configuration::Configuration const& config = {}) noexcept;

  /*!
   * \brief De-initializes the thread-pools.
   * \pre         -
   * \context     Shutdown
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void DeinitializeThreadPools() noexcept;

  /*!
   * \copydoc Base::EnqueueTask()
   */
  bool EnqueueTask(::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier,
                   ThreadPoolTask&& task) noexcept final;

  /*!
   * \copydoc Base::RemoveTasks()
   */
  void RemoveTasks(::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier,
                   ThreadPoolTask::CreatorKey const key) noexcept final;

  /*!
   * \copydoc Base::GetNumberOfWorkerThreads()
   */
  typename TaskQueue::size_type GetNumberOfWorkerThreads(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept final;

  /*!
   * \copydoc Base::GetPoolId()
   */
  PoolId GetPoolId(::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept final;

  /*!
   * \copydoc Base::GetMaxNumberOfTasks()
   */
  typename configuration::TaskCount GetMaxNumberOfTasks(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept final;

  /*!
   * \copydoc Base::IsThreadPoolAssigned()
   */
  bool IsThreadPoolAssigned(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) const noexcept final;

 private:
  /*!
   * \brief A skeleton calls this method on construction to get the thread-pool assigned to the concrete service
   *        instance. If there's no reference to an user-specific thread-pool for this object, the default thread-pool
   *        is returned as a reference.
   * \param[in] assignment_identifier  The assignment name string to identify the threadpool.
   * \return The thread-pool assigned to this service interface instance. In case of no user-specific assignment, the
   *         default thread-pool is returned as a reference.
   * \pre         InitializeThreadPools() should have been already called.
   * \context     ANY
   * \threadsafe  TRUE - The thread safety is ensured by the design. The modification of the accessed data happens only
   *              in the init and shutdown phase.
   * \reentrant   TRUE - The thread safety is ensured by the design. The modification of the accessed data happens only
   *              in the init and shutdown phase.
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady TRUE
   */
  ::amsr::socal::internal::ThreadPool& RequestThreadPoolAssignment(
      ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& assignment_identifier) noexcept;

  /*!
   * \brief Get default r-port thread pool configured object.
   * \return Default r-port thread pool configuration.
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  static ::amsr::socal::internal::configuration::ThreadPoolConfig GetDefaultRPortThreadPoolConfig() noexcept;

  /*!
   * \brief Get default p-port thread pool configured object.
   * \return Default p-port thread pool configuration.
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  static ::amsr::socal::internal::configuration::ThreadPoolConfig GetDefaultPPortThreadPoolConfig() noexcept;

  /*!
   * \brief Get default configuration of the vComContinuation thread pool.
   * \return Default vComContinuation thread pool configuration.
   * \pre         -
   * \context     Init
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  static ::amsr::socal::internal::configuration::ThreadPoolConfig GetComContinuationThreadPoolConfig() noexcept;

  /*!
   * \brief Validate the InstanceSpecifiers configured in thread pool AssignmentConfig.
   * \param[in] config                           The configuration object containing the thread pool configuration.
   * \param[in] instance_specifier_lookup_table  The table used to validate the user-defined thread pool configuration.
   * \pre         All instance specifiers from the configuration must be known.
   * \pre         All assigned path short names of the config must be valid.
   * \context     Init
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  static void ValidateThreadpoolAssignmentConfig(
      ::amsr::socal::internal::configuration::Configuration const& config,
      ::amsr::socal::internal::InstanceSpecifierLookupTable const& instance_specifier_lookup_table) noexcept;

  /*!
   * \brief An instance of a logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ThreadPoolsManager"_sv};

  /*!
   * \brief   The proxy side single threaded default thread pool.
   * \details The Thread pool is created when runtime is initialized, and destroyed upon calling Shutdown.
   */
  ::amsr::core::Optional<::amsr::socal::internal::ThreadPool> r_port_default_thread_pool_{};

  /*!
   * \brief   The skeleton side single threaded default thread pool.
   * \details The Thread pool is created when runtime is initialized, and destroyed upon calling Shutdown.
   */
  ::amsr::core::Optional<::amsr::socal::internal::ThreadPool> p_port_default_thread_pool_{};

  /*!
   * \brief   Thread-pool for handling Future::then() callbacks from the application, used in context of proxy side
   *          method requests.
   * \details The Thread pool is created when runtime is initialized, and destroyed upon calling Shutdown. This thread-
   *          pool is responsible for processing the callbacks registered by the applications using
   *          ara::core::Future::then().
   */
  ::amsr::core::Optional<::amsr::socal::internal::ThreadPool> com_continuation_thread_pool_{};

  /*!
   * \brief Type alias for shortening of threadpool config assignment name.
   */
  using AssignmentName = ::amsr::socal::internal::configuration::AssignmentName;

  /*!
   * \brief ServiceInstanceAssignment container.
   */
  using ServiceInstanceAssignmentContainer = vac::container::StaticVector<AssignmentName>;

  /*!
   * \brief Each thread-pool contained in the ThreadPoolsManager is assigned to a number of service instances identified
   *        by the service interface name and the ::ara::com::InstanceIdentifier. The thread-pools act as a key. Each
   *        thread-pool as a key has multiple service instances assigned as the value (static vector). Each element of
   *        this vector represents one service instance that refers to the thread-pool to use.
   */
  using ThreadPoolMapElement = std::pair<::amsr::socal::internal::ThreadPool, ServiceInstanceAssignmentContainer>;

  /*!
   * \brief Type-alias for the thread-pool container. The static vector is filled during initialization time once. All
   *        user-specific thread-pools will be emplaced and the service interface instances using these thread-pools
   *        will be assigned to the individual elements.
   */
  using ThreadPoolContainer = vac::container::StaticVector<ThreadPoolMapElement>;

  /*!
   * \brief All user-defined thread-pools held in the ThreadPoolsManager object.
   */
  ThreadPoolContainer thread_pools_{};

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT__ThreadPoolsManager, InitializeThreadPools);

  /*!
   * \brief Friend declaration for test purposes.
   */
  FRIEND_TEST(UT__ThreadPoolsManager, DeinitializeThreadPools);
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_POOLS_MANAGER_H_
