/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 * \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 * \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Header file for heapified container class for TimerManager.
 *         \unit  amsr::SteadyTimer
 *
 *      \details -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_HEAP_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_HEAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <functional>  // std::reference_wrapper
#include "amsr/steady_timer/types.h"
#include "ara/core/vector.h"
#include "osabstraction/time/types.h"

namespace amsr {
namespace steady_timer {
namespace internal {
class Timer;

/*!
 * \brief         Container class to manage a heap sorted vector of timers objects.
 */
class TimerHeap final {
 public:
  /*! \brief Default constructor. */
  TimerHeap() noexcept = default;
  /*! \brief Default destructor. */
  ~TimerHeap() noexcept = default;  // VCA_OSA_MOLE_1298
  /*! \brief Deleted copy constructor. */
  TimerHeap(TimerHeap const&) = delete;
  /*! \brief Deleted assignment operator. */
  TimerHeap& operator=(TimerHeap const&) = delete;
  /*! \brief Deleted move constructor */
  TimerHeap(TimerHeap&&) noexcept = delete;
  /*! \brief Deleted move assignment operator */
  TimerHeap& operator=(TimerHeap&&) noexcept = delete;

  /*!
   * \brief            Increases the internal allocation counter.
   * \details          This method allocates additional memory for the underlying vector if necessary.
   * \context          Any
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void AllocateSlot() noexcept;

  /*!
   * \brief            Decreases the internal allocation counter.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void DeallocateSlot() noexcept;

  /*!
   * \brief            Adds a timer to the heap container.
   * \param[in]        timer The timer to add.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void AddTimer(Timer& timer) noexcept;

  /*!
   * \brief            Removes a timer from the heap container.
   * \details          Caller must ensure that timer is part of the container otherwise behavior is undefined.
   * \param[in]        timer The timer to remove.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void RemoveTimer(Timer& timer) noexcept;

  /*!
   * \brief            Returns the first timer from the container.
   * \details          User must ensure that the container is not empty otherwise behavior is undefined.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void PopTimer() noexcept;

  /*!
   * \brief            Returns the first timer from the container.
   * \details          User must ensure that the container is not empty otherwise behavior is undefined.
   * \return           The first timer.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Timer* GetFirstTimer() noexcept;

  /*!
   * \brief            Heapifies the container.
   * \context          Any
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void UpdateHeap() noexcept;

  /*!
   * \brief            Returns the earliest expiry time.
   * \return           The earliest expiry time.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   */
  TimePoint GetEarliestExpiryTime() const noexcept;

 private:
  /*!
   * \brief            Updates the earliest expiry time.
   * \context          ANY
   * \pre              -
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \threadsafe       FALSE
   * \steady           TRUE
   */
  void UpdateEarliestExpiryTime() noexcept;

  /*! \brief            Alias for vector of timers. */
  using TimerVector = ara::core::Vector<std::reference_wrapper<Timer>>;
  /*! \brief            Vector of timers. */
  TimerVector timers_{};
  /*! \brief            Number of allocated timer slots. */
  size_t allocated_timers_{0};
  /*! \brief            Point in time of the earliest expiry time. */
  TimePoint earliest_expiry_time_{kInfinity};
};
}  // namespace internal
}  // namespace steady_timer
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_STEADY_TIMER_INTERNAL_TIMER_HEAP_H_
