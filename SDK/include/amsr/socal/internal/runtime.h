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
/*!        \file  socal/internal/runtime.h
 *        \brief  Contains Runtime class
 *      \details  Contains Init/Shutdown logic for communication components inside Socal. Holds the internal logic for
 *                polling the reactor. Provides access to the reactor, thread pool as well as the instance specifier
 *                lookup tables for provided and required instance specifiers.
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_RUNTIME_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>

#include "amsr/core/instance_specifier.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/reactor_constructor_interface.h"
#include "amsr/socal/internal/runtime_interface.h"
#include "amsr/steady_timer/timer_manager_factory_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/com/instance_identifier.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

// VECTOR NC Metric-OO.WMC.One: MD_Socal_Metric-OO.WMC.methods_per_class_is_slightly_above_limit
/*!
 * \brief The Runtime class initializes necessary binding instances and the LifecycleManager.
 * \vprivate Vector component internal API.
 *
 * \unit Socal::Runtime::Runtime
 */
class Runtime final : public RuntimeInterface {
 public:
  /*!
   * \brief Type alias for internal Configuration type.
   */
  using ConfigurationType = ::amsr::socal::internal::configuration::Configuration;

  /*!
   * \brief Returns an singleton wrapped instance of Runtime class.
   * \return Singleton wrapped Runtime instance.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  static ::amsr::generic::Singleton<Runtime>& getSingletonInstance() noexcept;

  /*!
   * \brief Destructor invokes the shutdown sequence if not already de-initialized.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~Runtime() noexcept;

  Runtime(Runtime const&) = delete;
  Runtime(Runtime&&) noexcept = delete;
  Runtime& operator=(Runtime&&) & noexcept = delete;
  void operator=(Runtime const&) = delete;

  /*!
   * \brief Initializes internal runtime components.
   * \param[in] reactor_constructor    A unique pointer to a helper object that is used for construction of a Reactor1.
   * \param[in] timer_manager_factory  Reference to the timer manager factory.
   * \param[in] config                 "ara::com" specific configuration and binding-related parameters.
   * \return "amsr::core::Result" with no value and no error.
   * \pre         "amsr::socal::internal::Runtime" must not be already initialized.
   * \pre         The argument 'reactor_constructor' must be valid and not a nullptr.
   * \pre         Static memory must have been allocated.
   * \pre         All instance specifiers from the configuration must be known.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> InitializeCommunication(
      std::unique_ptr<::amsr::socal::internal::ReactorConstructorInterface> reactor_constructor,
      ::amsr::steady_timer::TimerManagerFactoryInterface const& timer_manager_factory,
      ConfigurationType const& config) noexcept;

  /*!
   * \brief Shuts down the runtime.
   * \return "amsr::core::Result" with no value and no error.
   * \pre         "amsr::socal::internal::Runtime" must be initialized.
   * \pre         "StopFindService()" must have been called for all active "StartFindService()" requests.
   * \pre         Proxies and skeletons must have been stopped.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::Result<void> DeInitializeCommunication() noexcept;

  /*!
   * \brief   Trigger handling of pending reactor events (received or transmitted packets, expired timers).
   * \details This API can also be called internally by the SomeIpBinding in the cases where the communication to the
   *          SOME/IP Daemon must be synchronous (e.g. Skeleton::OfferService, Proxy::ctor, ProxyEvent::Subscribe,
   *          ara::core::Initialize). Hence the API must be implemented as threadsafe as the reactor1 HandleEvents() API
   *          is not threadsafe.
   * \return True if any packet (timer handling or any rx/tx task) has been processed, false otherwise.
   * \pre         "amsr::socal::internal::Runtime" must be initialized. Runtime processing mode must be configured to
   *              'kPolling'.
   * \context     App | Init.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool ProcessPolling() noexcept;

  /*!
   * \brief Returns a reference to the underlying P-Port instance specifier lookup table.
   * \return The instance of the "InstanceSpecifierLookupTable".
   * \pre         "amsr::socal::internal::Runtime" must be initialized.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::socal::internal::InstanceSpecifierLookupTable const& GetProvidedInstanceSpecifierLookupTable() const noexcept;

  /*!
   * \brief Returns a reference to the underlying R-Port instance specifier lookup table.
   * \return The instance of the "InstanceSpecifierLookupTable".
   * \pre         "amsr::socal::internal::Runtime" must be initialized.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::socal::internal::InstanceSpecifierLookupTable const& GetRequiredInstanceSpecifierLookupTable() const noexcept;

  /*!
   * \brief   Returns reference to internal mutex.
   * \details This will allow bindings to synchronize against the parallel reactor events (that might be triggered
   *          during the ProcessPolling API) in Polling processing mode.
   * \return reference to the mutex used internally to guarantee ProcessPolling thread safety.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  std::mutex& GetPollingModeLock() noexcept;

  /*!
   * \brief Returns reference to the abstract ara::com configuration model.
   * \return reference to the ara::com configuration model
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::socal::internal::configuration::Configuration const& GetConfig() const noexcept;

  /*!
   * \brief Returns reference to the os abstraction Reactor1 object.
   * \return reference to the reactor object
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_falsenegative
  osabstraction::io::reactor1::Reactor1& GetReactor() noexcept final;

  /*!
   * \copydoc RuntimeInterface::GetSteadyTimerManager()
   */
  ::amsr::steady_timer::TimerManagerInterface& GetSteadyTimerManager() const noexcept final;

  /*!
   * \brief Map an InstanceIdentifier to the corresponding P-Port InstanceSpecifier.
   * \param[in] instance_specifier   The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected
   *                                 format: "<binding_name>:<binding specific instance ID>".
   * \return True if the entry is updated successfully, false otherwise.
   * \pre         -
   * \context     BindingInit
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool MapProvidedInstanceSpecifierToInstanceId(
      ::amsr::core::InstanceSpecifier const& instance_specifier,
      ::amsr::socal::internal::ProvidedInstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Map an InstanceIdentifier to the corresponding R-Port InstanceSpecifier.
   * \param[in] instance_specifier   The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected
   *                                 format: "<binding_name>:<binding specific instance ID>".
   * \return True if the entry is updated successfully, false otherwise.
   * \pre         -
   * \context     BindingInit
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool MapRequiredInstanceSpecifierToInstanceId(
      ::amsr::core::InstanceSpecifier const& instance_specifier,
      ::amsr::socal::internal::RequiredInstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Removes the given InstanceIdentifier from the provided instance specifier table.
   * \param[in] instance_specifier   The instance specifier which will be the key for the entry to be removed.
   * \param[in] instance_identifier  The instance identifier which will be the value for the entry to be removed.
   * \return True if the instance identifier entry is deleted, false otherwise.
   * \pre         -
   * \context     BindingShutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool RemoveProvidedInstanceSpecifierEntry(::amsr::core::InstanceSpecifier const& instance_specifier,
                                            ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Removes the given InstanceIdentifier from the required instance specifier table.
   * \param[in] instance_specifier   The instance specifier which will be the key for the entry to be removed.
   * \param[in] instance_identifier  The instance identifier which will be the value for the entry to be removed.
   * \return True if the instance identifier entry is deleted, false otherwise.
   * \pre         -
   * \context     BindingShutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool RemoveRequiredInstanceSpecifierEntry(::amsr::core::InstanceSpecifier const& instance_specifier,
                                            ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief Checks if the configured runtime processing mode is RuntimeProcessingMode::kPolling.
   * \return True is the runtime processing mode is equal to RuntimeProcessingMode::kPolling, false otherwise.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  static bool IsProcessingModePolling() noexcept;

  /*!
   * \brief Default initializes the Runtime.
   * \pre         -
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  Runtime() = default;

 private:
  /*!
   * \brief Internal shared initialization.
   * \param[in] timer_manager_factory  Reference to the timer manager factory.
   * \pre         Runtime must not be already initialized.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void InitializeInternal(::amsr::steady_timer::TimerManagerFactoryInterface const& timer_manager_factory) noexcept;

  /*!
   * \brief Generated method to initialize all static memory.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void InitializeLifecycleManager() noexcept;

  /*!
   * \brief Generated method to deinitialize all static memory.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void DeinitializeLifecycleManager() noexcept;

  /*!
   * \brief Trigger handling of pending reactor events (received or transmitted packets, expired timers) in polling
   *        mode.
   * \return True if any packet (timer handling or any rx/tx task) has been processed, false otherwise.
   * \pre         Runtime processing mode must be configured to 'kPolling'.
   * \context     App | Init.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool ProcessPollingInternal() noexcept;

  /*!
   * \brief   Create reactor and timer manager.
   * \details Amsr::core::Abort is called if reactor construction fails.
   * \param[in] timer_manager_factory  Reference to the timer manager factory.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void CreateReactorAndTimerManager(
      ::amsr::steady_timer::TimerManagerFactoryInterface const& timer_manager_factory) noexcept;

  /*!
   * \brief Fill all configured instance specifiers to the lookup tables.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void InitializeInstanceSpecifierLookupTables() noexcept;

  /*!
   * \brief An instance of a logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Runtime"_sv};

  /*!
   * \brief Lookup table storing mappings between P-Port InstanceSpecifiers and InstanceIdentifiers.
   */
  ::amsr::socal::internal::InstanceSpecifierLookupTable provided_instance_specifier_table_{logger_};

  /*!
   * \brief Lookup table storing mappings between R-Port InstanceSpecifiers and InstanceIdentifiers.
   */
  ::amsr::socal::internal::InstanceSpecifierLookupTable required_instance_specifier_table_{logger_};

  /*!
   * \brief Abstract ara::com configuration model.
   */
  ::amsr::socal::internal::configuration::Configuration config_{};

  /*!
   * \brief Constructs and holds a reactor instance.
   */
  std::unique_ptr<::amsr::socal::internal::ReactorConstructorInterface> reactor_constructor_{};

  /*!
   * \brief The steady timer manager. This object must be destroyed before destroying the reactor_constructor_.
   */
  std::shared_ptr<::amsr::steady_timer::TimerManagerInterface> steady_timer_manager_{};

  /*!
   * \brief   Signalizes if the Runtime instance is still alive for multi-threaded applications.
   * \details Currently the Runtime object construction and Initialization is done at two different steps. This flag is
   *          used to make sure the Runtime is not only constructed but also initialized.
   */
  std::atomic<bool> is_running_{false};

  /*!
   * \brief   Lock to synchronize execution of ProcessPolling.
   * \details Due to threading design the reactor must never be triggered concurrently.
   */
  std::mutex process_polling_lock_{};

  /*!
   * \brief A constant to represent the runtime processing mode.
   */
  static configuration::RuntimeProcessingMode const runtime_processing_mode_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_RUNTIME_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::internal::Runtime::process_polling_lock_
 * Used to synchronize execution of ProcessPolling with binding specific reactor sync tasks.
 *
 * \protects ::amsr::socal::internal::Runtime::ProcessPolling() call against the
 *           ::amsr::ipc_binding_core::internal::ReactorSyncTask::operator()().
 * \usedin ::amsr::socal::internal::Runtime::ProcessPollingInternal
 * \exclude All other methods of the ::amsr::socal::internal::Runtime object.
 * \length MEDIUM synchronizes execution of the reactor.
 * \endexclusivearea
 */
// clang-format on
