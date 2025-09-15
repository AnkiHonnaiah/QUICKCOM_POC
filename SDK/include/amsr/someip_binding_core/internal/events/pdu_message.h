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
/*!        \file  pdu_message.h
 *        \brief  Data type class for PDU notification message.
 *      \details  This class holds the PDU notification message which can be used in reception or sending path.
 *                The class also contains helper functionality to create the data type from an incoming message.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SomeIpTypes
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_PDU_MESSAGE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_PDU_MESSAGE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someipd_app_protocol/internal/pdu_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief   Data type class for PDU notification message.
 * \details This class holds the PDU notification message, which can be used in reception or sending path.
 *          The class also contains helper functionality to create the data type from an incoming message.
 */
class PduMessage final {
 public:
  /*!
   * \brief Type definition for the SOME/IP Service ID
   */
  using ServiceId = std::uint16_t;

  /*!
   * \brief Type definition for the SOME/IP Instance ID
   */
  using InstanceId = std::uint16_t;

  /*!
   * \brief Type definition for the PDU ID
   */
  using PduId = std::uint32_t;

  /*!
   * \brief Type definition for the SOME/IP Session ID
   * */
  using SessionId = std::uint16_t;

  /*!
   * \brief Type definition for the SOME/IP Event ID
   */
  using EventId = std::uint16_t;

  /*!
   * \brief Time stamp Type definition.
   */
  using TimeStamp = ::amsr::someip_protocol::internal::TimeStamp;

  /*!
   * \brief Named constructor of PduMessage.
   *
   * \details This function creates a PduMessage instance using the incoming packet and instance ID.
   *          The ownership of the received packet will be adapted to be managed by shared pointer.
   *
   * \param[in] message      Memory buffer containing a valid PDU message.
   * \param[in] instance_id  Instance ID of the message.
   *
   * \return PduMessage object containing the complete PDU Event. No error is being returned.
   * \steady  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::Result<PduMessage> CreatePduMessage(::amsr::someipd_app_protocol::internal::PduMessage&& message,
                                                         InstanceId const instance_id) noexcept;

  /*!
   * \brief Constructor
   *
   * \param[in] buffer             Buffer containing payload + header of the message.
   * \param[in] service_id         Service ID.
   * \param[in] instance_id        Instance ID.
   * \param[in] event_id           Event ID.
   * \param[in] time_stamp         Optional time stamp.
   * \pre                          buffer must not be a nullptr.
   * \steady  TRUE
   */
  PduMessage(DataBufferSharedPtr buffer, ServiceId service_id, InstanceId const instance_id, EventId const event_id,
             amsr::core::Optional<TimeStamp> const time_stamp) noexcept;

  /*!
   * \brief Default destructor
   *
   * \context Reactor
   * \steady  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~PduMessage() noexcept = default;

  /*!
   * \brief Default move constructor
   * \details A move constructor is needed to allow this datatype to be returned
   *          from PduMessage::CreatePduMessage. Move operation is only used for this use case and
   *          therefore there is no moved-from object that could be in an invalid state.
   * \context Reactor
   * \steady  TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  PduMessage(PduMessage&&) noexcept = default;

  PduMessage(PduMessage const&) = delete;
  PduMessage& operator=(PduMessage const&) & = delete;
  PduMessage& operator=(PduMessage&&) & = delete;

  /*!
   * \brief Getter for the buffer containing header + payload of the SOME/IP event message.
   *
   * \return shared pointer containing the buffer.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_binding_core::internal::DataBufferSharedPtr const GetBuffer() const noexcept;

  /*!
   * \brief Getter for the buffer containing header + payload of the SOME/IP event message.
   *
   * \return shared pointer containing the buffer.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<TimeStamp> GetTimeStamp() const noexcept;

  /*!
   * \brief Getter for the Service Id.
   *
   * \return Service ID.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ServiceId GetServiceId() const noexcept;

  /*!
   * \brief Getter for the Instance Id.
   *
   * \return Instance ID.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  InstanceId GetInstanceId() const noexcept;

  /*!
   * \brief Getter for the Event Id.
   *
   * \return Event ID.
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  EventId GetEventId() const noexcept;

 private:
  /*!
   * \brief A data buffer containing a complete SOME/IP message with a header.
   */
  DataBufferSharedPtr const buffer_;

  /*!
   * \brief A SOME/IP service identifier
   */
  ServiceId const service_id_;

  /*!
   * \brief A SOME/IP service identifier
   */
  InstanceId const instance_id_;

  /*!
   * \brief A SOME/IP method or event identifier
   */
  EventId const event_id_;

  /*!
   * \brief A time stamp for when the message was received.
   */
  amsr::core::Optional<TimeStamp> const time_stamp_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_PDU_MESSAGE_H_
