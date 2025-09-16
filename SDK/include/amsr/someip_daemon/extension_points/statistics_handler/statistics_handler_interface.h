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
/*!        \file  statistics_handler_interface.h
 *        \brief  Defining statistics handler interface to be used in the extension point.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_STATISTICS_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_STATISTICS_HANDLER_INTERFACE_H_
#define LIB_SOMEIP_STATISTICS_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_STATISTICS_HANDLER_INTERFACE_H_

#include <cstdint>
#include <stdexcept>
#include "ara/core/string.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace statistics_handler {

/*!
 * \brief Type definition for the SOME/IP Service ID
 */
using ServiceId = std::uint16_t;

/*!
 * \brief Type definition for the SOME/IP Instance ID
 */
using InstanceId = std::uint16_t;

/*!
 * \brief Type-alias for an IP address.
 */
using IpAddress = ara::core::String;

/*!
 * \brief Type definition for the SOME/IP Port.
 */
using Port = std::uint16_t;

/*!
 * \brief Interface for class supporting all error/statistics reporting for SOME/IP Daemon.
 * \details WARNING: Calls to the methods defined in this class will be done in the main (reactor) thread of
 *                   SomeIpDaemon. If any of those calls is blocking, the complete Daemon will block.
 *                   Therefore, if more processing is required, the implementation shall schedule that in their
 *                   own dispatched thread.
 * \trace SPEC-10144905
 * \trace SPEC-10144906
 * \trace SPEC-10144907
 * \trace SPEC-10144908
 * \trace SPEC-10144909
 * \trace SPEC-10144910
 * \trace SPEC-10144911
 * \trace SPEC-10144912
 * \trace SPEC-10144913
 * \trace SPEC-10144914
 * \trace SPEC-10144915
 * \trace SPEC-10144916
 * \trace SPEC-10144917
 * \trace SPEC-10144918
 * \trace SPEC-10144919
 * \trace SPEC-10144920
 * \trace SPEC-10144921
 * \trace SPEC-10144922
 * \trace SPEC-10144923
 * \trace SPEC-10144924
 * \trace SPEC-10144925
 * \trace SPEC-10144926
 * \trace SPEC-10144927
 * \trace SPEC-10144928
 * \trace SPEC-10144929
 * \trace SPEC-10144930
 * \trace SPEC-10144931
 * \trace SPEC-10144932
 * \trace SPEC-10144933
 * \trace SPEC-10144934
 * \trace SPEC-10144935
 * \trace SPEC-10144936
 * \trace SPEC-10144937
 * \trace SPEC-10144938
 * \trace SPEC-10144939
 * \trace SPEC-10144940
 * \trace SPEC-10144941
 * \trace SPEC-10144942
 * \trace SPEC-10144943
 * \trace SPEC-10144944
 */
class StatisticsHandlerInterface {
 public:
  /*!
   * \brief Define defaultructor.
   * \steady FALSE
   */
  StatisticsHandlerInterface() = default;
  /*!
   * \brief Define default destructor.
   * \steady FALSE
   */
  virtual ~StatisticsHandlerInterface() = default;

  StatisticsHandlerInterface(StatisticsHandlerInterface&) = delete;
  StatisticsHandlerInterface(StatisticsHandlerInterface&&) = delete;
  StatisticsHandlerInterface& operator=(StatisticsHandlerInterface&) & = delete;
  StatisticsHandlerInterface& operator=(StatisticsHandlerInterface&&) & = delete;

  /*!
   * \brief Report Invalid SOME/IP message header.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpHeader() noexcept = 0;

  /*!
   * \brief   Report SOME/IP message with invalid protocol version.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpProtocolVersion() noexcept = 0;

  /*!
   * \brief   Report SOME/IP message with invalid interface version.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpInterfaceVersion() noexcept = 0;

  /*!
   * \brief   Report SOME/IP message with invalid Service ID.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpServiceId() noexcept = 0;

  /*!
   * \brief   Report SOME/IP message with invalid Method ID.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpMethodId() noexcept = 0;

  /*!
   * \brief   Report invalid SOME/IP message. If the message is invalid due to invalid Protocol version, Interface
   *          version, Service Id or Method Id the corresponding API in this class should be used instead of this one.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpMessage() noexcept = 0;

  /*!
   * \brief   Report invalid SOME/IP Service Discovery message. This API is triggered only if the SOME/IP header of the
   *          SD message is correct but the body cannot be deserialized and no entry will be evaluated.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSomeIpSdMessage() noexcept = 0;

  /*!
   * \brief   Report that a Subscription Negative ACK entry has been received.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked.
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportSubscribeEventgroupNackEntry() noexcept = 0;

  /*!
   * \brief   Report that an invalid offer service entry has been received.
   *          An offer service entry is considered invalid if the entry is discarded at any point due validation,
   *          consistency or access rights reasons.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked.
   * \param[in] service_id  Service ID of the service this entry is concerned with.
   * \param[in] instance_id Instance ID of the service this entry is concerned with.
   * \param[in] ip_address IP address of the SOME/IP message sender.
   * \param[in] port Port number of the SOME/IP message sender.
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidOfferServiceEntry(ServiceId const, InstanceId const, IpAddress const&,
                                              Port const) noexcept = 0;

  /*!
   * \brief   Report that an invalid SD Entry has been received. This includes all SD entries other than
   *          offer service entry.
   *          A SD entry is considered invalid if the entry is discarded at any point due validation,
   *          consistency or access rights reasons.
   * \details This method is executed on the same reactor thread as the complete SOME/IP Daemon. This means that if
   *          this method is blocked, the whole SOME/IP Daemon will be blocked.
   * \pre -
   * \context Network
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual void ReportInvalidSdEntry() noexcept = 0;
};

}  // namespace statistics_handler
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_SOMEIP_STATISTICS_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_STATISTICS_HANDLER_INTERFACE_H_
