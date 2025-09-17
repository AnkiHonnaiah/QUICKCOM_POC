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
/*!        \file  default_statistics_handler.h
 *        \brief  Class implementing the StatisticsHandlerInterface with default behaviour.
 *
 *********************************************************************************************************************/
#ifndef LIB_DEFAULT_STATISTICS_HANDLER_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_DEFAULT_STATISTICS_HANDLER_H_
#define LIB_DEFAULT_STATISTICS_HANDLER_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_DEFAULT_STATISTICS_HANDLER_H_

#include <cstdint>
#include "ara/core/string.h"

#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace statistics_handler {

class DefaultStatisticsHandler : public StatisticsHandlerInterface {
 public:
  DefaultStatisticsHandler(DefaultStatisticsHandler const&) = delete;
  DefaultStatisticsHandler(DefaultStatisticsHandler&&) = delete;
  DefaultStatisticsHandler& operator=(DefaultStatisticsHandler const&) & = delete;
  DefaultStatisticsHandler& operator=(DefaultStatisticsHandler&&) & = delete;

  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  DefaultStatisticsHandler() noexcept = default;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~DefaultStatisticsHandler() noexcept override = default;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpHeader
   * \steady TRUE
   */
  void ReportInvalidSomeIpHeader() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpProtocolVersion
   * \steady TRUE
   */
  void ReportInvalidSomeIpProtocolVersion() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpInterfaceVersion
   * \steady TRUE
   */
  void ReportInvalidSomeIpInterfaceVersion() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpServiceId
   * \steady TRUE
   */
  void ReportInvalidSomeIpServiceId() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpMethodId
   * \steady TRUE
   */
  void ReportInvalidSomeIpMethodId() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpMessage
   * \steady TRUE
   */
  void ReportInvalidSomeIpMessage() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSomeIpSdMessage
   * \steady TRUE
   */
  void ReportInvalidSomeIpSdMessage() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportSubscribeEventgroupNackEntry
   * \steady FALSE
   */
  void ReportSubscribeEventgroupNackEntry() noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidOfferServiceEntry
   * \steady TRUE
   */
  void ReportInvalidOfferServiceEntry(ServiceId const service_id, InstanceId const instance_id,
                                      IpAddress const& ip_address, Port const port) noexcept override;

  /*!
   * \copydoc
   * extension_points::statistics_handler::StatisticsHandlerInterface::ReportInvalidSdEntry
   * \steady TRUE
   */
  void ReportInvalidSdEntry() noexcept override;
};
}  // namespace statistics_handler
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_DEFAULT_STATISTICS_HANDLER_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_STATISTICS_HANDLER_DEFAULT_STATISTICS_HANDLER_H_
