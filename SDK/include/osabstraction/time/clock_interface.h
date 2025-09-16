/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Provides access to different clocks.
 *        \unit   osabstraction::time_CommonClocks_Linux
 *        \unit   osabstraction::time_CommonClocks_PikeOS
 *        \unit   osabstraction::time_CommonClocks_QNX
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_CLOCK_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_CLOCK_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/time/types.h"

namespace osabstraction {
namespace time {

/*!
 * \brief Interface for clock implementations.
 *
 * \vpublic
 */
class ClockInterface {
 public:
  /*!
   * \brief       Constructs a clock interface.
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \steady          TRUE
   *
   * \vprivate    Vector product internal API
   */
  ClockInterface() noexcept = default;

  /*!
   * \brief Returns the passed time.
   *
   * \details
   * Returns the time passed since an unspecified point in the past. The point in the past depends on the used clock
   * (for example system start-up or Epoch).
   *
   * \return          The passed time.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  The clock does not exist (anymore) or does not
   *                                                                    support getting the current time or reading
   *                                                                    the time from the clock device failed.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-ClockInterface
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  virtual auto Now() const noexcept -> ::amsr::core::Result<TimeStamp> = 0;

  /*!
   * \brief Destroys the clock interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-ClockInterface
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClockInterface() noexcept = default;

 protected:
  /*!
   * \brief Copy constructs the clock interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-ClockInterface
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ClockInterface(ClockInterface const&) noexcept = default;

  /*!
   * \brief Move constructs the clock interface.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-ClockInterface
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ClockInterface(ClockInterface&&) noexcept = default;

  /*!
   * \brief Copy assigns the clock interface.
   *
   * \return
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-ClockInterface
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  ClockInterface& operator=(ClockInterface const&) & noexcept = default;

  /*!
   * \brief Move assigns the clock interface.
   *
   * \return
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-ClockInterface
   *
   * \vprivate        Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(ClockInterface&&) & noexcept -> ClockInterface& = default;
};

}  // namespace time
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_TIME_CLOCK_INTERFACE_H_
