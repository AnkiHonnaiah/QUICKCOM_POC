/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!
 *       \file  timer_extension.h
 *      \brief  This file contains the declaration of the Timer extension callback interface and
 *              the realization of the amsr::timer::Timer interface in the TimerExtension class.
 *              It also contains the wrapper for the old (not steady) and new (steady) timers,
 *              which will be removed after the timer migration is completed.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_TIMER_EXTENSION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_TIMER_EXTENSION_H_

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <memory>
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/steady_timer/types.h"
#include "amsr/thread/this_thread.h"
#include "amsr/timer/timer.h"
#include "amsr/timer/timer_manager.h"
#include "ara/core/abort.h"
#include "ara/core/variant.h"

namespace amsr {
namespace tls {
namespace internal {
namespace common {

/*!
 * \brief Type alias for the timer manager used by TLS.
 * \details This type is temporary, it shall be removed when the migration to the new timer manager is complete.
 */
using TlsTimerManager = ara::core::Variant<amsr::timer::TimerManager*, amsr::steady_timer::TimerManagerInterface*>;

/*!
 * \brief Interface for the internal timer extension implementation
 * \vprivate This class is component private.
 * \trace CREQ-SecCom-Timeouts, DSGN-SecCom-Timeouts
 */
class TimerExtensionCallbackInterface {
 public:
  /*! * \brief Default constructible */
  TimerExtensionCallbackInterface() = default;

  /*! \brief Not copy constructible */
  TimerExtensionCallbackInterface(TimerExtensionCallbackInterface const&) = delete;

  /*! \brief Not move constructible */
  TimerExtensionCallbackInterface(TimerExtensionCallbackInterface&&) = delete;

  /*! \brief Not copy assignable */
  TimerExtensionCallbackInterface& operator=(TimerExtensionCallbackInterface const&) = delete;

  /*! \brief Not move assignable */
  TimerExtensionCallbackInterface& operator=(TimerExtensionCallbackInterface&&) = delete;

  /*! \brief Virtual destructor */
  virtual ~TimerExtensionCallbackInterface() = default;

  /*!
   * \brief OnTimerEvent callback which is triggered by a timer expiry event.
   * \return bool True if successful, otherwise false
   * \vprivate This method is component private.
   */
  virtual bool OnTimerEvent() = 0;
};

/*!
 * \brief Implementation of the amsr::timer::Timer interface
 * \details TimerExtension implements the virtual HandleTimer method provided by the interface
 * \vprivate This class is component private.
 */
class TimerExtension : public amsr::timer::Timer {
 public:
  /*!
   * \brief Constructor for TimerExtension
   * \param[in] timer_manager A pointer to a TimerManager to associate this timer with
   * \param[in] ref_to_callback A reference to the callback interface to call for timer events
   */
  TimerExtension(amsr::timer::TimerManager* timer_manager, TimerExtensionCallbackInterface& ref_to_callback)
      : amsr::timer::Timer{timer_manager}, ref_registered_callback_{ref_to_callback} {}

  /*!
   * \brief Not copy constructible.
   */
  TimerExtension(TimerExtension const&) = delete;

  /*!
   * \brief Not move assignable.
   */
  void operator=(TimerExtension const&&) = delete;

  /*!
   * \brief Not move constructible.
   */
  TimerExtension(TimerExtension const&&) = delete;

  /*!
   * \brief Not copy assignable.
   */
  void operator=(TimerExtension const&) = delete;

  /*! \brief The default destructor */
  ~TimerExtension() override = default;

  /*!
   * \brief Calls the registered callback OnTimerEvent
   * \return bool Returns the result of the OnTimerEvent call
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  bool HandleTimer() final { return ref_registered_callback_.OnTimerEvent(); }

 private:
  /*! \brief Reference of Timer Extension Callback Interface */
  TimerExtensionCallbackInterface& ref_registered_callback_;
};

/*!
 * \brief Container and wrapper for the timer.
 * \details It uses either the old (not steady) or the new (steady) timer.
 * \vprivate component
 */
class TlsTimer final {
 public:
  /*!
   * \brief Constructor.
   * \details We cast to void the result of "OnTimerEvent" because since the timer is not periodic,
   *          its result has no value.
   * \param[in] timer_manager The timer manager. It can be either the old (not steady) or the new (steady) one.
   * \param[in] ref_to_callback A reference to the timer callback.
   */
  TlsTimer(TlsTimerManager timer_manager, TimerExtensionCallbackInterface& ref_to_callback) noexcept {
    if (ara::core::holds_alternative<amsr::timer::TimerManager*>(timer_manager)) {
      old_timer_ =
          std::make_unique<TimerExtension>(ara::core::get<amsr::timer::TimerManager*>(timer_manager), ref_to_callback);
    } else {
      new_timer_ = ara::core::get<amsr::steady_timer::TimerManagerInterface*>(timer_manager)
                       ->CreateTimer([&ref_to_callback]() { static_cast<void>(ref_to_callback.OnTimerEvent()); },
                                     amsr::steady_timer::MissedTimeoutBehavior::kDiscardMissedTimeouts);
    }
  }

  /*!
   * \brief Destructor.
   * \details In order to destroy the new timer, we must first stop it and then wait for the callback to stop executing,
   *          since the callback may be executed on another thread.
   *          We wait for maximum 1 sec before destruction and abort otherwise.
   */
  ~TlsTimer() noexcept {
    if (new_timer_ != nullptr) {
      new_timer_->Stop();
      for (std::uint16_t i{0U}; i < 1000U; ++i) {
        if (!new_timer_->IsCallbackExecuting()) {
          break;
        }
        amsr::thread::SleepFor(std::chrono::milliseconds(1));
      }
      if (new_timer_->IsCallbackExecuting()) {
        ara::core::Abort("TlsTimer::dtor(): the callback is still executing more than 1 sec after stopping the timer.");
      }
    }
  }

  // No copy/move constructors/operators.
  TlsTimer(TlsTimer const&) = delete;
  TlsTimer(TlsTimer&&) = delete;
  void operator=(TlsTimer const&) = delete;
  void operator=(TlsTimer&&) = delete;

  /*!
   * \brief Start the timer.
   * \details If the timer has already been scheduled, it will be rescheduled.
   * \param[in] timeout The duration in milliseconds after which this timer should fire.
   */
  void Start(std::chrono::milliseconds timeout) noexcept {
    if (old_timer_ != nullptr) {
      old_timer_->SetOneShot(timeout);
      old_timer_->Start();
    } else if (new_timer_ != nullptr) {
      new_timer_->Start(/* one shot */ amsr::core::Optional<amsr::steady_timer::Duration>(
                            amsr::steady_timer::Duration{osabstraction::time::TimeStamp{timeout}}),
                        /* period */ {});
    } else {
      ara::core::Abort("TlsTimer::Start(): unexpected internal state: timer not set.");
    }
  }

  /*!
   * \brief Stop the timer.
   */
  void Stop() noexcept {
    if (old_timer_ != nullptr) {
      old_timer_->Stop();
    } else if (new_timer_ != nullptr) {
      new_timer_->Stop();
    } else {
      ara::core::Abort("TlsTimer::Stop(): unexpected internal state: timer not set.");
    }
  }

 private:
  /*!
   * \brief A unique pointer to the old timer manager.
   */
  std::unique_ptr<TimerExtension> old_timer_{nullptr};

  /*!
   * \brief A unique pointer to the new timer manager.
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> new_timer_{nullptr};

  /*!
   * \brief For accessing old_timer_.
   */
  FRIEND_TEST(Cdd__TimerTestFixture, NonSteadyTimer);

  /*!
   * \brief For accessing old_timer_.
   */
  FRIEND_TEST(Cdd__TimerTestFixture, NonSteadyTimer_Stop);

  /*!
   * \brief For accessing new_timer_.
   */
  FRIEND_TEST(Cdd__TimerTestFixture, SteadyTimer);

  /*!
   * \brief For accessing new_timer_.
   */
  FRIEND_TEST(Cdd__TimerTestFixture, SteadyTimer_Stop);

  /*!
   * \brief For accessing old_timer_.
   */
  FRIEND_TEST(UT__TlsTimer__NonSteady, Start);

  /*!
   * \brief For accessing old_timer_.
   */
  FRIEND_TEST(UT__TlsTimer__NonSteady, Stop);

  /*!
   * \brief For accessing new_timer_.
   */
  FRIEND_TEST(UT__TlsTimer__Steady, Start);

  /*!
   * \brief For accessing new_timer_.
   */
  FRIEND_TEST(UT__TlsTimer__Steady, Stop);

  /*!
   * \brief For accessing old_timer_.
   */
  FRIEND_TEST(UT__TlsTimer__DeathTest, Stop);

  /*!
   * \brief For accessing old_timer_.
   */
  FRIEND_TEST(UT__TlsTimer__DeathTest, Start);
};

}  // namespace common
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_TIMER_EXTENSION_H_
