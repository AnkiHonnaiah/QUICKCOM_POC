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
/*!        \file  thread_runtime_config.h
 *        \brief  Runtime configuration for Thread library.
 *        \unit   amsr::thread::ConfigurableThreads::RuntimeConfiguration
 *
 *********************************************************************************************************************/

#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_THREAD_RUNTIME_CONFIG_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_THREAD_RUNTIME_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <vector>
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/thread/thread_config_types.h"

namespace amsr {
namespace thread {
namespace internal {
/*!
 * \brief  Singleton that manages thread runtime configuration.
 */
class ThreadRuntimeConfig {
 public:
  /*!
   * \brief           Gets a reference to the singleton instance.
   *
   * \return          Reference to the singleton instance.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  static auto GetInstance() noexcept -> ThreadRuntimeConfig&;

  /*!
   * \brief           Checks if ThreadRuntimeConfig is initialized correctly.
   *
   * \return          Returns true if ThreadRuntimeConfig is intialized correctly, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  bool IsInitialized() noexcept {
    std::lock_guard<std::mutex> const lock{mutex_};
    return initialized_;
  }

  /*!
   * \brief           Initializes ThreadRuntimeConfig.
   *
   * \details         Has to be called once before any ThreadRuntimeConfig function is allowed to be called with
   *                  the exceptions of GetInstance(). The runtime configuration is not initialized again if it was
   *                  already initialized when this function is called and success is returned.
   *                  It aborts execution if AMSR_THREAD_CONFIGURABLE_SETTINGS is not defined correctly.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  void Initialize() noexcept;

  /*!
   * \brief           Deinitializes the ThreadRuntimeConfig.
   *
   * \details         Shall be called once during deinitialization sequence.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  void Deinitialize() noexcept;

  /*!
   * \brief           Returns whether AMSR_THREAD_CONFIGURABLE_SETTINGS env var was "ON" or not.
   *
   * \return          Returns true if AMSR_THREAD_CONFIGURABLE_SETTINGS env var is defined with "ON", false otherwise.
   *
   * \context         ANY
   *
   * \pre             ThreadRuntimeConfig is initialized.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  bool IsThreadConfigurableSettingsEnvVarEnabled() noexcept {
    std::lock_guard<std::mutex> const lock{mutex_};
    return is_configurable_settings_enabled_;
  }

  /*!
   * \brief           Searches for ThreadConfig matches identifier.
   *
   * \param[in]       identifier The identifier for the thread configuration.
   *
   * \return          Returns matching ThreadConfig if identifier exists, otherwise returns an error.
   *
   * \error           ThreadErrc::kInvalidConfig if identifier doesn't exist in the thread_config_list.
   *
   * \context         -
   *
   * \pre             ThreadRuntimeConfig is initialized.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  amsr::core::Result<ThreadConfig> SearchThreadConfigList(amsr::core::StringView identifier) noexcept;

  /*!
   * \brief           Move parsed ThreadConfig instances to ThreadConfigList.
   *
   * \param[in]       parsed_thread_configs The parsed thread configuration for all processes for this executable.
   *
   * \error           ThreadErrc::kInsufficientResources if ThreadConfigList size becomes larger than kMaxThreadConfigs.
   *
   * \context         InitPhase
   *
   * \pre             ThreadRuntimeConfig is initialized.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  amsr::core::Result<void> AddToThreadConfigList(ThreadConfigList& parsed_thread_configs) noexcept;

  ThreadRuntimeConfig(ThreadRuntimeConfig const&) noexcept = delete;
  ThreadRuntimeConfig& operator=(ThreadRuntimeConfig const&) noexcept = delete;
  ThreadRuntimeConfig(ThreadRuntimeConfig&& other) noexcept = delete;
  ThreadRuntimeConfig& operator=(ThreadRuntimeConfig&&) noexcept = delete;

 private:
  /*!
   * \brief           Initializes ThreadRuntimeConfig instance members with default values.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  ThreadRuntimeConfig() noexcept = default;

  /*!
   * \brief           Deinitializes ThreadRuntimeConfig instance if it was not already deinitialized.
   *
   * \context         Process termination.
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  ~ThreadRuntimeConfig() noexcept { Deinitialize(); };  // VCA_THREAD_STL_CALL_OBJ_DTOR

  /*!
   * \brief    Flag that signals if the ThreadRuntimeConfig is initialized or not.
   * \details  true if the runtime is initialized, false otherwise.
   */
  bool initialized_{false};

  /*!
   * \brief    Mutex that protects the ThreadRuntimeConfig.
   */
  std::mutex mutex_{};

  /*!
   * \brief    List for parsed threads configurations
   */
  ThreadConfigList thread_config_list_{};

  /*!
   * \brief    Flag to store the env variable state.
   */
  bool is_configurable_settings_enabled_{false};
};

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_THREAD_RUNTIME_CONFIG_H_
