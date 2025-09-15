/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       borrowed_reactor.h
 *      \brief
 *      \details
 *
 *      \unit 	    LogAndTrace::Common::ReactorHandling::BorrowedReactor
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_BORROWED_REACTOR_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_BORROWED_REACTOR_H_

#include "amsr/timer/timer_manager.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/ref.h"

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief An externally owned Reactor that is just borrowed
 */
class BorrowedReactor final {
  /*!
   * \brief The reactor that runs the complete ipc communication
   */
  Ref<osabstraction::io::reactor1::Reactor1Interface> reactor_;

  ::amsr::timer::TimerManager* timer_manager_;

 public:
  // VECTOR NC VectorC++-V12.1.4: MD_LOG_AutosarC++17_10-A12.1.4_implicit_conversion
  /*!
   * \brief Construct the reactor from the osabstraction Reactor implementation
   * \param ref The osabstraction reactor
   * \param tm The associated TimerManager. Defaults to no used manager.
   * \spec
   *   requires true;
   * \endspec
   */
  BorrowedReactor(osabstraction::io::reactor1::Reactor1Interface& ref,  // NOLINT(hicpp-explicit-conversions)
                  ::amsr::timer::TimerManager* tm = nullptr) noexcept
      : reactor_{ref}, timer_manager_{tm} {}

  /*!
   * \brief Returns a mutable reference to the underlying reactor
   * \return a mutable reference to the underlying reactor
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() noexcept -> osabstraction::io::reactor1::Reactor1Interface& { return this->reactor_; }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_LOG_AutosarC++17_10-M9.3.3_interior_mutability
  /*!
   * \brief Gets the TimerManager for the underlying reactor
   * \return A reference to the TimerManager
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTimerManager() & noexcept -> ::amsr::timer::TimerManager& {
    using vac::container::literals::operator""_sv;
    AssertDebug(this->timer_manager_ != nullptr, "TimerManager is null!"_sv);
    // VCA_LOGAPI_VALID_OBJECT
    return *this->timer_manager_;
  };
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_BORROWED_REACTOR_H_
