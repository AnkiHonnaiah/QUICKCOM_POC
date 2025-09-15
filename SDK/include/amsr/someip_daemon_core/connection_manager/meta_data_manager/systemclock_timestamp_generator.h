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
 *        \brief  Implementation of timestamp_generator class of type system_clock
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_SYSTEMCLOCK_TIMESTAMP_GENERATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_SYSTEMCLOCK_TIMESTAMP_GENERATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include "timestamp_generator.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace meta_data_manager {
/*!
 * \brief           Type-alias for a system clock
 */
using TimeStampGeneratorClock = std::chrono::system_clock;

/*!
 * \brief           Timestamp_generator class of type system_clock
 * \details         Implementation of timestamp_generator class of type system_clock that can return current time stamp.
 */
class SystemClockTimeStampGenerator : public TimeStampGenerator<TimeStampGeneratorClock::time_point> {
 public:
  /*!
   * \brief           Constructor
   * \steady          TRUE
   */
  SystemClockTimeStampGenerator() noexcept = default;

  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  ~SystemClockTimeStampGenerator() noexcept override = default;

  /*!
   * \brief           Delete copy constructor
   * \steady          TRUE
   */
  SystemClockTimeStampGenerator(SystemClockTimeStampGenerator const&) = delete;

  /*!
   * \brief           Delete move constructor
   * \steady          TRUE
   */
  SystemClockTimeStampGenerator(SystemClockTimeStampGenerator&&) = delete;

  /*!
   * \brief           Delete copy assignment operator
   * \steady          TRUE
   */
  SystemClockTimeStampGenerator& operator=(SystemClockTimeStampGenerator const&) = delete;

  /*!
   * \brief           Delete move assignment operator
   * \steady          TRUE
   */
  SystemClockTimeStampGenerator& operator=(SystemClockTimeStampGenerator&&) = delete;

  /*!
   * \brief           Returns the current time
   * \return          The current value (timepoint) of the system_clock
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       TRUE
   * \trace           DSGN-SomeIpDaemon-MetaData
   * \steady          TRUE
   */
  TimeStampGeneratorClock::time_point Generate() const noexcept override;
};

}  // namespace meta_data_manager
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_SYSTEMCLOCK_TIMESTAMP_GENERATOR_H_
