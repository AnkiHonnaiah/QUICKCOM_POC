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
 *               \file    executable_controller_base.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "executable_controller_base.h"

#include <csignal>

#include "ara/core/initialization.h"
#include "ara/core/string.h"

namespace oneapplication {
namespace common {

AraLogSupport::AraLogSupport(const char* init_cxt_id, const char* shutdown_cxt_id, const char* controller_cxt_id) :
    init_logger_{ara::log::CreateLogger(ara::core::StringView(init_cxt_id),
                                        ara::core::StringView("Context for the initialization flow"))},
    shutdown_logger_{ara::log::CreateLogger(ara::core::StringView(shutdown_cxt_id),
                                            ara::core::StringView("Context for the shutdown flow"))},
    controller_logger_{ara::log::CreateLogger(ara::core::StringView(controller_cxt_id),
                                              ara::core::StringView("Context for the controller normal flow"))} {
}

AraLogSupport::~AraLogSupport() {
}

void ExecutableControllerBase::Run() noexcept {

  InitializeSignalHandling();

  DoPreInitialize();

  DoInitialize();

  DoPostInitialize();

  DoPreStart();

  DoStart();

  DoPostStart();

  DoExecute();

  DoPreShutdown();

  DoShutdown();

  DoPostShutdown();
}


void ExecutableControllerBase::DoPreInitialize() {

  // Initialize the Autosar Adaptive Platform (Microsar Adaptive)
  // Configuration of LogAndTrace, Aracom and other platform clusters is done in configuration files
  // generated from the ARXML Model. Location of these files are defined by the platform.
  // Initialize ARA (e.g. logging)

  ara::core::Result<void> init_result{ara::core::Initialize()};

  if (!init_result.HasValue()) {
    ara::core::String msg { "ara::core::Initialize() failed "};
    msg += "\nResult contains: " + init_result.Error().Message().ToString() +
        ", " + init_result.Error().UserMessage().ToString();
    ara::core::Abort(msg.c_str());
  }

  uptr_log_support_ = std::make_unique<AraLogSupport>(init_cxt_id_, shutdown_cxt_id_, controller_cxt_id_);

  uptr_log_support_->GetInitLogger().LogInfo() << "ExecutionaControllerBase::DoPreInitialize: Adaptive Microsar has been successfully initiated. Logging is now allowed";
}

void ExecutableControllerBase::DoInitialize() {
}

void ExecutableControllerBase::DoPostInitialize() {
}

void ExecutableControllerBase::DoPreStart() {
}

void ExecutableControllerBase::DoStart() {
}

void ExecutableControllerBase::DoPostStart() {
  /* Spawn a new signal handler thread. */
  list_of_active_threads_[0] = std::thread{&ExecutableControllerBase::SignalHandlerThread, this};
}

void ExecutableControllerBase::DoExecute() {
}

void ExecutableControllerBase::DoPreShutdown() {
}

void ExecutableControllerBase::DoShutdown() {
}

void ExecutableControllerBase::DoPostShutdown() {

  uptr_log_support_->GetShutdownLogger().LogInfo() << "ExecutableControllerBase::DoPostShutdown: Begin Post Shutdown. Microsar Adaptive is closing down and threads are shutdown. Calls towards platform not allowed after this.";

  // Shutdown the Autosar Adaptive Platform (Microsar Adaptive)
  // Operation calls towards the platform is not allowed after this operation is called.
  ara::core::Deinitialize();

  // Kill the signal handling thread. To be able to do join below
  pthread_kill(signal_pthread_id_, SIGTERM);

  // Wait till other threads have joined.
  for (std::thread& thread : list_of_active_threads_) {
    if (thread.joinable()) thread.join();
  }
}


void ExecutableControllerBase::InitializeSignalHandling() noexcept {
  bool success{true};
  sigset_t signals;

  /* Block all signals except the SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGSEGV signals because blocking them will lead to
   * undefined behavior. Their default handling shall not be changed (dependent on underlying POSIX environment, usually
   * process is killed and a dump file is written). Signal mask will be inherited by subsequent threads. */
  success &= (0 == sigfillset(&signals));
  success &= (0 == sigdelset(&signals, SIGABRT));
  success &= (0 == sigdelset(&signals, SIGBUS));
  success &= (0 == sigdelset(&signals, SIGFPE));
  success &= (0 == sigdelset(&signals, SIGILL));
  success &= (0 == sigdelset(&signals, SIGSEGV));
  success &= (0 == pthread_sigmask(SIG_SETMASK, &signals, nullptr));

  if (!success) {
    ara::core::Abort("InitializeSignalHandling failed.");
  }
}

void ExecutableControllerBase::SignalHandlerThread() {
  sigset_t signal_set;

  // Storing the pthread id for this thread. Will be used to kill this thread.
  // Killed by calling pthread_kill(signal_pthread_id_, SIGTERM)
  signal_pthread_id_ = pthread_self();

  /* #10 empty the set of signals. */
  if (0 != sigemptyset(&signal_set)) {
    uptr_log_support_->GetInitLogger().LogFatal() << "ExecutableControllerBase::SignalHandlerThread: Executable controller could not clear signal set.";
    ara::core::Abort("Empty signal set failed.");
  }
  /* #20 add SIGTERM to signal set. */
  if (0 != sigaddset(&signal_set, SIGTERM)) {
    uptr_log_support_->GetInitLogger().LogFatal() << "ExecutableControllerBase::SignalHandlerThread: Executable controller could not add signal to signalset: SIGTERM";
    ara::core::Abort("Adding SIGTERM failed.");
  }
  /* #21 add SIGINT to signal set. */
  if (0 != sigaddset(&signal_set, SIGINT)) {
    uptr_log_support_->GetInitLogger().LogFatal() << "ExecutableControllerBase::SignalHandlerThread: Executable controller could not add signal to signalset: SIGINT";
    ara::core::Abort("Adding SIGINT failed.");
  }

  /* #30 wait until SIGTERM or SIGINT signal received. */
  std::int32_t sig{-1};

  do {
    if (0 != sigwait(&signal_set, &sig)) {
      uptr_log_support_->GetInitLogger().LogFatal() << "Exectuable controller called sigwait() with invalid signalset";
      ara::core::Abort("ExecutableControllerBase::SignalHandlerThread: Waiting for SIGTERM or SIGINT failed.");
    }

    uptr_log_support_->GetShutdownLogger().LogInfo() << "ExecutableControllerBase::SignalHandlerThread: Executable received SIGTERM or SIGINT, requesting shutdown of the Executable. Signal received = " << sig;

    if ((sig == SIGTERM) || (sig == SIGINT)) {
      /* #35 request application exit. (SignalHandler initiate the shutdown!) */
      shutdown_requested_ = true;
    }
  } while (!shutdown_requested_);
}

void ExecutableControllerBase::WaitForShutdown() {
  // Store the current point of time in next_run.
  std::chrono::steady_clock::time_point next_run{std::chrono::steady_clock::now()};

  // Define a time interval of 5000 milliseconds.
  std::chrono::steady_clock::duration duration{std::chrono::milliseconds(wait_for_shutdown_loop_wait_time_ms_)};

  if (wait_for_shutdown_loop_count_ == 0) {
    while (!shutdown_requested_) {
      // Do some work here.
      // Calculate the next point of time to be rescheduled.
      next_run += duration;
      // Blocks the execution of the current thread until specified point of time (next_run) has been reached.
      std::this_thread::sleep_until(next_run);
    }
  } else {
    uint32_t loop_count = 0;
    while ( (!shutdown_requested_) && (loop_count < wait_for_shutdown_loop_count_)) {
      // Do some work here.
      // Calculate the next point of time to be rescheduled.
      next_run += duration;
      // Blocks the execution of the current thread until specified point of time (next_run) has been reached.
      std::this_thread::sleep_until(next_run);
      loop_count++;
    }
  }
}

}  // namespace common
}  // namespace oneapplication
