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
 *        \brief  Interface class for creating timestamps used in metadata.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_TIMESTAMP_GENERATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_TIMESTAMP_GENERATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace meta_data_manager {
/*!
 * \brief           Abstract class to create timestamps
 * \details         Abstract class to create current point using clock type.
 */
template <class TimeStampType>
class TimeStampGenerator {
 public:
  /*!
   * \brief           Constructor
   * \steady          TRUE
   */
  TimeStampGenerator() noexcept = default;

  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  virtual ~TimeStampGenerator() noexcept = default;

  /*!
   * \brief           Delete copy constructor
   * \steady          TRUE
   */
  TimeStampGenerator(TimeStampGenerator const&) = delete;

  /*!
   * \brief           Delete move constructor
   * \steady          TRUE
   */
  TimeStampGenerator(TimeStampGenerator&&) = delete;

  /*!
   * \brief           Delete copy assignment operator
   * \steady          TRUE
   */
  TimeStampGenerator& operator=(TimeStampGenerator const&) = delete;

  /*!
   * \brief           Delete move assignment operator
   * \steady          TRUE
   */
  TimeStampGenerator& operator=(TimeStampGenerator&&) = delete;

  /*!
   * \brief           Returns the current time
   * \details         Abstract function that will be implemented by child classes to return the current time as a clock
   *                  type.
   * \return          Current generated timestamp.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \trace           DSGN-SomeIpDaemon-MetaData
   * \steady          TRUE
   */
  virtual TimeStampType Generate() const = 0;
};

}  // namespace meta_data_manager
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_TIMESTAMP_GENERATOR_H_
