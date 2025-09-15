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
 *        \brief  Class decleration for TcpReceiver
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_RECEIVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_RECEIVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

class TcpEndpoint;

/*!
 * \brief           Class decleration for TcpReceiver
 */
class TcpReceiver final {
 public:
  /*!
   * \brief           Constructor of TcpReceiver
   * \param[in]       endpoint
   *                  A reference to the TCP endpoint this receiver belongs to.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpReceiver(TcpEndpoint& endpoint) noexcept;

  /*!
   * \brief           Destructs instance of TcpReceiver
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpReceiver() noexcept;

  TcpReceiver(TcpReceiver&&) = delete;
  TcpReceiver(TcpReceiver const&) = delete;
  TcpReceiver& operator=(TcpReceiver&&) = delete;
  TcpReceiver& operator=(TcpReceiver const&) = delete;

 private:
  /*!
   * \brief           A reference to the TCP endpoint this receiver belongs to
   */
  TcpEndpoint& endpoint_;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_RECEIVER_H_
