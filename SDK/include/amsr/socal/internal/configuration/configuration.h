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
/**        \file  socal/internal/configuration/configuration.h
 *        \brief  Defining ara::com Configuration.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_CONFIGURATION_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <map>
#include <set>
#include <string>
#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "ara/core/instance_specifier.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Each Runtime may have its specific processing mode.
 *        - kPolling: In processing mode kPolling no reactor thread is handling events within the bindings. The
 *          application developer must poll for incoming data by calling ProcessPolling() which does *one step* for each
 *          binding.
 *        - kThreadDriven: In threaded mode, the reactor is provided by the middleware and does a blocking read.
 *          Incoming requests will be routed to the application immediately.
 */
enum class RuntimeProcessingMode : std::uint32_t { kPolling = 0U, kThreadDriven = 1U };

/*!
 * \brief Alias for readability.
 */
using PoolId = std::uint8_t;

/*!
 * \brief Type-alias for the number of threads.
 */
using ThreadCount = std::uint8_t;

/*!
 * \brief Type for the number of tasks.
 */
using TaskCount = std::uint32_t;

/*!
 * \brief Type alias for assignment name.
 */
using AssignmentName = std::string;

/*!
 * \brief Type alias for bsw thread configuration name.
 */
using BswThreadConfigurationName = std::string;

/*!
 * \brief Type alias for the thread pool assignment container.
 */
using ThreadPoolAssignmentContainer = ::ara::core::Vector<AssignmentName>;

/*!
 * \brief Pool-ID of the default thread-pools for the runtime.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr PoolId kDefaultThreadPoolId{0U};

/*!
 * \brief Pool-ID of the vComContinuation thread-pool for the runtime.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr PoolId kContinuationThreadPoolId{0U};

/*!
 * \brief Number of threads of the default thread-pools.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr ThreadCount kDefaultThreadPoolNumThreads{1U};

/*!
 * \brief Number of threads of the vComContinuation thread-pool.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr ThreadCount kContinuationThreadPoolNumThreads{1U};

/*!
 * \brief Maximum number of tasks of the default thread-pools.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr TaskCount kDefaultThreadPoolMaxTask{1024U};

/*!
 * \brief Maximum number of tasks of the vComContinuation thread-pool.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr TaskCount kContinuationThreadPoolMaxTask{2048U};

/*!
 * \brief Minimum number of threads of a pool.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr ThreadCount kMinThreadsPerThreadPool{1U};

/*!
 * \brief Prefix of the name of the default thread pool.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr vac::container::CStringView kRPortDefaultThreadPoolPrefix{"vComRDef"_sv};

// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr vac::container::CStringView kPPortDefaultThreadPoolPrefix{"vComPDef"_sv};

/*!
 * \brief Prefix of the name of the vComContinuation thread-pool.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr vac::container::CStringView kContinuationThreadPoolPrefix{"vComCont"_sv};

/*!
 * \brief The name of the configuration element of vComContinuation thread.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr vac::container::CStringView kContinuationThreadConfigName{"vComContinuation"_sv};

/*!
 * \brief The default name of the thread of the default thread pools.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr vac::container::CStringView kDefaultThreadConfigName{"vComDefault"_sv};

/*!
 * \brief Prefix of the names of user defined thread pools.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_global
static constexpr vac::container::CStringView kUserDefinedThreadPoolPrefix{"vComPool"_sv};

/*!
 * \brief Struct representation of a thread pool.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
struct ThreadPoolConfig {
  /*!
   * \brief The ID of the thread pool.
   */
  PoolId pool_id{};

  /*!
   * \brief Number of threads in the thread pool.
   */
  ThreadCount num_threads{};

  /*!
   * \brief Maximum number of tasks in the thread pool.
   */
  TaskCount max_tasks{};

  /*!
   * \brief The container of assignment configurations of the thread pool.
   */
  ThreadPoolAssignmentContainer assignment_configs{};

  /*!
   * \brief The name of the bsw thread configuration.
   */
  // VCA_SOCAL_CALLING_STL_APIS
  BswThreadConfigurationName bsw_thread_configuration{""};

  /*!
   * \brief The prefix of this thread pool's name, needed for debugging purposes.
   */
  // VCA_SOCAL_CALLING_STL_APIS
  std::string pool_prefix{kUserDefinedThreadPoolPrefix.c_str()};

  /*!
   * \brief Get the pool identifier of this configuration.
   * \return The ID of the thread pool.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  PoolId GetPoolId() const noexcept;

  /*!
   * \brief Get the number of worker threads configured.
   * \return The number of threads in the thread pool.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  ThreadCount GetNumberOfWorkerThreads() const noexcept;

  /*!
   * \brief Get the name of the bsw thread configuration.
   * \return The name of the bsw thread configuration.
   * \spec requires true; \endspec
   */
  // VECTOR NC VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  BswThreadConfigurationName GetBswThreadConfigurationName() const noexcept;

  /*!
   * \brief Set the number of worker threads.
   * \param[in] threads  The number of threads to set.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  void SetNumberOfWorkerThreads(ThreadCount const threads) noexcept;

  /*!
   * \brief Get the number of maximum enqueueable tasks.
   * \return The maximum number of tasks in the thread pool.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  TaskCount GetMaxTasks() const noexcept;

  /*!
   * \brief Set the maximum number of tasks.
   * \param[in] tasks  The number of tasks to set.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  void SetMaxTasks(TaskCount const tasks) noexcept;

  /*!
   * \brief Set the name of the bsw thread configuration.
   * \param[in] thread_config_sv  Name of the bsw thread configuration.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  void SetBswThreadConfigurationName(::amsr::core::StringView const thread_config_sv) noexcept;

  /*!
   * \brief Get the thread pool assignment configurations.
   * \return The container of assignment configurations of the thread pool.
   * \spec requires true; \endspec
   */
  // VECTOR NC VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  ThreadPoolAssignmentContainer const& GetAssignmentConfigs() const noexcept;

  /*!
   * \brief Add a thread-pool assignment configuration.
   * \param[in] assignment  The assignment configuration to be added.
   * \spec requires true; \endspec
   */
  // VECTOR NC VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  void AddAssignmentConfig(AssignmentName assignment) noexcept;

  /*!
   * \brief Get the prefix for the name of this thread pool.
   * \return The prefix of the thread pool.
   * \pre     -
   * \context ANY
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VECTOR NC VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  std::string GetPoolPrefix() const noexcept;

  /*!
   * \brief Set the thread pool id of the thread pool.
   * \param[in] id  The thread pool id.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  void SetThreadPoolId(PoolId const id) noexcept;

  /*!
   * \brief Set the thread pool prefix of the thread pool.
   * \param[in] prefix  The thread pool prefix.
   * \spec requires true; \endspec
   */
  // VECTOR NL VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  void SetThreadPoolPrefix(vac::container::CStringView const prefix) noexcept;
};

/*!
 * \brief Container type to store the instance specifiers.
 */
using InstanceSpecifierContainer = std::set<std::string>;

/*!
 * \brief Wrapper type to store provided and required instance specifiers.
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
struct InstanceSpecifierPair {
  /*!
   * \brief List of provided instance specifiers.
   */
  InstanceSpecifierContainer provided_instance_specifiers_{};

  /*!
   * \brief List of required instance specifiers.
   */
  InstanceSpecifierContainer required_instance_specifiers_{};
};

/*!
 * \brief Container to store the service interfaces configuration object.
 */
using ServiceInterfacesConfig = std::map<std::string, InstanceSpecifierPair>;

/*!
 * \brief Type-alias for the thread pool configuration objects.
 */
using ThreadPoolConfigContainer = ::ara::core::Vector<ThreadPoolConfig>;

/*!
 * \brief Type-alias for the optional default thread pool configuration object.
 */
using DefaultThreadPoolConfig = ::amsr::core::Optional<ThreadPoolConfig>;

/*!
 * \brief ara::com Configuration.
 *
 * \unit Socal::Configuration::ConfigurationProvider
 */
// VECTOR NC VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private
class Configuration {  // VCA_SOCAL_CALLING_STL_APIS
 public:
  /*!
   * \brief Get configured thread pools.
   * \return Container of thread pool configurations.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ThreadPoolConfigContainer const& GetThreadPools() const noexcept;

  /*!
   * \brief Add a new ThreadPool configuration object.
   * \param[in] thread_pool_config  ThreadPool config to be added.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void AddThreadPool(ThreadPoolConfig const& thread_pool_config) noexcept;

  /*!
   * \brief Add a new configuration object for service interfaces.
   * \param[in] service_interfaces_config  service interfaces config to be added.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void AddServiceInterfaces(ServiceInterfacesConfig&& service_interfaces_config) noexcept;

  /*!
   * \brief Get configured service interfaces.
   * \return Container of service interfaces configurations.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ServiceInterfacesConfig const& GetServiceInterfaces() const noexcept;

  /*!
   * \brief Add a new P-Port Default ThreadPool configuration object.
   * \param[in] p_port_default_thread_pool_config  P-Port Default ThreadPool config to be added.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void AddPPortDefaultThreadPool(ThreadPoolConfig const& p_port_default_thread_pool_config) noexcept;

  /*!
   * \brief Add a new R-Port Default ThreadPool configuration object.
   * \param[in] r_port_default_thread_pool_config  R-Port Default ThreadPool config to be added.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void AddRPortDefaultThreadPool(ThreadPoolConfig const& r_port_default_thread_pool_config) noexcept;

  /*!
   * \brief Get configured P-Port default thread pool.
   * \return An object of P-Port default thread pool configuration.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  DefaultThreadPoolConfig const& GetPPortDefaultThreadPool() const noexcept;

  /*!
   * \brief Get configured R-Port default thread pool.
   * \return An object of R-Port default thread pool configuration.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  DefaultThreadPoolConfig const& GetRPortDefaultThreadPool() const noexcept;

 protected:
  /*!
   * \brief Container for all thread pool configs.
   */
  ThreadPoolConfigContainer thread_pool_config_container_{};

  /*!
   * \brief Configuration element for P-Port default thread pool.
   */
  DefaultThreadPoolConfig p_port_default_thread_pool_{};

  /*!
   * \brief Configuration element for R-Port default thread pool.
   */
  DefaultThreadPoolConfig r_port_default_thread_pool_{};

  /*!
   * \brief Optional for service interfaces configuration object.
   */
  ServiceInterfacesConfig service_interfaces_config_{};
};

}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_CONFIGURATION_H_
