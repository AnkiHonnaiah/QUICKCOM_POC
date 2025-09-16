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
/**        \file  request_response_delay.h
 *        \brief  Request-Response Delay
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUEST_RESPONSE_DELAY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUEST_RESPONSE_DELAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief  Request-Response Delay
 *         Delay between reception of an SD message (Find, Offer or Subscribe) until response shall be sent (Offer,
 *         Subscribe, Subscribe ACK)
 */
struct RequestResponseDelay {
  /*!
   * \brief The minimum delay of a response to a multicast request.
   */
  std::chrono::nanoseconds min_value;

  /*!
   * \brief The maximum delay of a response to a multicast request
   */
  std::chrono::nanoseconds max_value;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUEST_RESPONSE_DELAY_H_
