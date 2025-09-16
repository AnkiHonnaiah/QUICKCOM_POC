/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *               \file    executable_controller_base.h
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/
#ifndef INCLUDE_EXECUTABLE_CONTROLLER_BASE_H
#define INCLUDE_EXECUTABLE_CONTROLLER_BASE_H

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include "ara/log/logging.h"

namespace oneapplication {
namespace common {

constexpr uint_least8_t kNumOfThreads{4}; /*!< Number of Threads used in Application. */

/*!
 * \brief Class for providing support for logging in differnt contexts.
 */
class AraLogSupport {

 public:

  AraLogSupport(const char * init_cxt_id, const char * shutdown_cxt_id, const char * controller_cxt_id);
  ~AraLogSupport();

/*!
 * \brief Returns the logger that is used in the context of initialization sequence.
 */
  ara::log::Logger& GetInitLogger() {
    return init_logger_;
  }
/*!
 * \brief Returns the logger that is used in the context of shutdown sequence.
 */
  ara::log::Logger& GetShutdownLogger() {
    return shutdown_logger_;
  }
/*!
 * \brief Returns the logger that is used in the context of the controller module.
 */
  ara::log::Logger& GetControllerLogger() {
    return controller_logger_;
  }

 private:

  ara::log::Logger& init_logger_;
  ara::log::Logger& shutdown_logger_;
  ara::log::Logger& controller_logger_;

};

/*!
 * \brief The base class of the controller module for executables.
 */
class ExecutableControllerBase {

 public:
  /*!
   * \brief The entry point for running the logical flow of the executable.
   */
  void Run() noexcept;

  /*!
   * \brief Destructor of the ControllerBase class as a pure virtual function.
   */
  virtual ~ExecutableControllerBase() = default;

 protected:  // protected methods
  /*!
   * \brief Execute the pre initialization phase tasks.
   */
  virtual void DoPreInitialize();
  /*!
   * \brief Execute the initialization phase tasks.
   */
  virtual void DoInitialize();
  /*!
   * \brief Execute the post initialization phase tasks.
   */
  virtual void DoPostInitialize();
  /*!
   * \brief Execute the pre start phase tasks.
   */
  virtual void DoPreStart();
  /*!
   * \brief Execute the start phase tasks.
   */
  virtual void DoStart();
  /*!
   * \brief Execute the post start phase tasks.
   */
  virtual void DoPostStart();
  /*!
   * \brief Execute the execute (main logic) phase tasks.
   */
  virtual void DoExecute();

  /*!
   * \brief Execute the pre shutdown phase tasks.
   */
  virtual void DoPreShutdown();
  /*!
   * \brief Execute the shutdown phase tasks.
   */
  virtual void DoShutdown();
  /*!
   * \brief Execute the post shutdown phase tasks.
   */
  virtual void DoPostShutdown();

  /*!
   * \brief Waits in a loop until shutdown is requested.
   */
  void WaitForShutdown();

 private:
   
   /*!
   * \brief Entry point of the thread receiving system signals.
   */
  void InitializeSignalHandling() noexcept;

  void SignalHandlerThread();

 protected:
  /*!
   * \brief Container to store all threads spawned by this application.
   */
  std::array<std::thread, kNumOfThreads> list_of_active_threads_;
  std::unique_ptr<AraLogSupport> uptr_log_support_{};

    /*!
   * \brief Flag to identify whether the application was requested to terminate, i.e. has received a SIGTERM.
   */
    std::atomic_bool shutdown_requested_;

 private:
  /*!
   * \brief Storing the signal handling thread id to be able to send signals to it internally.
   *
   */
  pthread_t signal_pthread_id_{};

  uint8_t wait_for_shutdown_loop_count_  = 0;

  uint32_t wait_for_shutdown_loop_wait_time_ms_ = 5000;

  const char * init_cxt_id_{"INIT"};

  const char * shutdown_cxt_id_{"DOWN"};

  const char * controller_cxt_id_{"CTRL"};


};

}  // namespace common
}  // namespace oneapplication


#endif //INCLUDE_EXECUTABLE_CONTROLLER_BASE_H
