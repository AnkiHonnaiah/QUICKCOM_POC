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
/*!        \file  tracing/tracing_interface.h
 *        \brief  SOME/IP daemon specific interface for providing tracing about communication and SD messages.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this interface is defined.
 *                In order to provide actual tracing services, it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_TRACING_INTERFACE_H_
#define LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_TRACING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "amsr/someip_daemon/extension_points/tracing/tracing_datatypes.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace tracing {

/*!
 * \brief Tracing interface.
 * \vpublic
 */
class TracingInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  TracingInterface() noexcept = default;

  virtual ~TracingInterface() noexcept = default;
  TracingInterface(TracingInterface const&) = delete;
  TracingInterface(TracingInterface&&) = delete;
  TracingInterface& operator=(TracingInterface const&) & = delete;
  TracingInterface& operator=(TracingInterface&&) & = delete;

  /*!
   * \brief     Called every time a SOME/IP event notification has been received
   * \details   This API is called once per adaptive application that is subscribed to the event.
   *
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceSomeIpEventReceiveSync(SomeIpEventReceiveMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a PDU event notification has been received
   * \details   This API is called once per adaptive application that is subscribed to the event.
   *
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TracePduEventReceiveSync(PduEventReceiveMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a method request has been received.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodRequestReceiveSync(MethodRequestReceiveMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a method response has been received.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodResponseReceiveSync(MethodResponseReceiveMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a SOME/IP event notification has been transmitted.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceSomeIpEventSendSync(SomeIpEventSendMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a PDU event notification has been transmitted.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TracePduEventSendSync(PduEventSendMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a method request has been transmitted.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodRequestSendSync(MethodRequestSendMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a method response has been transmitted.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   */
  virtual void TraceMethodResponseSendSync(MethodResponseSendMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a remote service is offered.
   * \details   This API is called once per adaptive application that is requesting the service.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void TraceServiceInstanceUpSync(ServiceInstanceUpdateMetaData const& metadata) noexcept = 0;

  /*!
   * \brief     Called every time a remote service is stopped.
   * \details   This API is called once per adaptive application that is requesting the service.
   * \vpublic
   * \param[in] metadata Metadata associated with the trace API.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void TraceServiceInstanceDownSync(ServiceInstanceUpdateMetaData const& metadata) noexcept = 0;
};

}  // namespace tracing
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_TRACING_INTERFACE_H_
