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
/*!        \file
 *        \brief  Dynamic clock implements a PTP hardware clock.
 *
 *      \details  Linux supports dynamic clocks.
 *
 *         \unit  amsr::ptp::DynamicClock
 *
 *********************************************************************************************************************/

#ifndef LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_TIME_DYNAMIC_CLOCK_H_
#define LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_TIME_DYNAMIC_CLOCK_H_

#include <ctime>
#include <memory>
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/net_utils/network_stack_id.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/time/clock_interface.h"
#include "osabstraction/time/os_types.h"
#include "osabstraction/time/types.h"

namespace amsr {
namespace ptp {
namespace time {

/*!
 * \brief Constructs a dynamic clock.
 *
 * \trace           DSGN-Osab-PtpClockLinuxDynamicClocks
 */
class DynamicClock : public ::osabstraction::time::ClockInterface {
 public:
  /*!
   * \brief Deleted copy constructor
   */
  DynamicClock(DynamicClock const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  DynamicClock(DynamicClock&& other) noexcept = delete;

  // VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_OSA_A10.2.1_DeletedAssignmentOperator
  /*!
   * \brief Deleted assignment operator.
   */
  auto operator=(DynamicClock const&) -> DynamicClock& = delete;

  /*!
   * \brief Deleted move operator.
   */
  auto operator=(DynamicClock&& other) & noexcept -> DynamicClock& = delete;

  /*!
   * \brief Destroys the dynamic clock.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~DynamicClock() noexcept override = default;

  /*!
   * \brief Construct a dynamic clock.
   *
   * \details OsAbstraction only supports dynamic clocks on Linux and QNX.
   *
   * \param[in] clock_identification   Identifier of the PHC. On Linux this string is a path to a character device that
   *                                   can be used as clock (e.g /dev/ptp0). On QNX this string is the name of the
   *                                   interface whose PHC shall be used as clock. This function only checks if the
   *                                   device can be opened. It does not check if the opened file descriptor may be used
   *                                   as clock. This check will be done on the first Now() call.
   * \param[in] netstack_id            Identifier of the network stack that manages the interface to which the PHC
   *                                   belongs. Only used on QNX, defaults to default network stack ID.
   *
   * \return Created clock (never nullptr)
   *
   * \error  osabstraction::OsabErrc::kApiError               (PikeOS only) Dynamic clock is not implemented by
   *                                                          OsAbstraction for this operating system.
   * \error  osabstraction::OsabErrc::kSystemEnvironmentError Creating the dynamic clock failed. Check the support data
   *                                                          for the POSIX error number.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Create(::amsr::core::StringView clock_identification,
                     amsr::net_utils::NetworkStackId const& netstack_id = amsr::net_utils::NetworkStackId{}) noexcept
      -> ::amsr::core::Result<std::unique_ptr<DynamicClock>>;

  /*!
   * \brief Returns the passed time.
   *
   * \details
   * Returns the time passed since an unspecified point in the past.
   *
   * \return          The passed time.
   *
   * \error           osabstraction::OsabErrc::kUnexpected              Unexpected error that is mapped to no category.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Dynamic clock does not exist (anymore) or does
   *                                                                    not support getting the current time or reading
   *                                                                    the time from the dynamic clock device failed.
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
   * \vprivate        Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Now() const noexcept -> ::amsr::core::Result<::osabstraction::time::TimeStamp> override;

 private:
  /*!
   * \brief Constructs a dynamic clock from a passed clock ID.
   *
   * \param[in]       clock_file_descriptor FD to an open dynamic clock character device.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \threadsafe      FALSE
   */
  explicit DynamicClock(::osabstraction::io::FileDescriptor&& clock_file_descriptor) noexcept;

  /*!
   * \brief Clock Id of the dynamic clock.
   */
  ::osabstraction::time::ClockId clock_id_;

  /*!
   * \brief File descriptor to the character device.
   */
  ::osabstraction::io::FileDescriptor fd_;
};

}  // namespace time
}  // namespace ptp
}  // namespace amsr

#endif  // LIB_PTP_IMPL_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_PTP_TIME_DYNAMIC_CLOCK_H_
