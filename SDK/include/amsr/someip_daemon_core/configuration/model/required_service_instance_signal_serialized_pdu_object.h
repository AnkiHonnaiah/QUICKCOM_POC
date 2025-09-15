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
/*!        \file  required_service_instance_signal_serialized_pdu_object.h
 *        \brief  Configuration object for 'RequiredServiceInstanceSignalSerializedPdu'.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDU_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDU_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'required service instance signal serialized PDU' object which is
 * filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class RequiredServiceInstanceSignalSerializedPduObject final {
 public:
  /*!
   * \brief PDU ID element.
   */
  using PduIdElement = CfgElement<someip_protocol::internal::PduId>;

  /*!
   * \brief Event ID element.
   */
  using EventIdElement = CfgElement<someip_protocol::internal::EventId>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] pdu Reference to the POD structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit RequiredServiceInstanceSignalSerializedPduObject(
      ConfigurationTypesAndDefs::SignalSerializedPdu& pdu) noexcept
      : pdu_id_{pdu.pdu_id_}, event_id_{pdu.event_id_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceSignalSerializedPduObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceSignalSerializedPduObject(RequiredServiceInstanceSignalSerializedPduObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceSignalSerializedPduObject(RequiredServiceInstanceSignalSerializedPduObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(
      RequiredServiceInstanceSignalSerializedPduObject const&) & -> RequiredServiceInstanceSignalSerializedPduObject& =
      delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(RequiredServiceInstanceSignalSerializedPduObject&& other) & noexcept
      -> RequiredServiceInstanceSignalSerializedPduObject& = default;

  /*!
   * \brief Set the PDU ID.
   * \param[in] pdu_id PDU ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPduId(someip_protocol::internal::PduId const& pdu_id) noexcept { pdu_id_.SetElement(pdu_id); }

  /*!
   * \brief Getter for PDU ID.
   * \return Reference to the PDU ID of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PduIdElement const& GetPduId() const noexcept { return pdu_id_; }

  /*!
   * \brief Set the Event ID.
   * \param[in] event_id Event ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventId(someip_protocol::internal::EventId const& event_id) noexcept { event_id_.SetElement(event_id); }

  /*!
   * \brief Getter for the Event ID.
   * \return Reference to the Event ID of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  EventIdElement const& GetEventId() const noexcept { return event_id_; }

 private:
  /*!
   * \brief PDU ID element.
   */
  PduIdElement pdu_id_;

  /*!
   * \brief Event ID element.
   */
  EventIdElement event_id_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SIGNAL_SERIALIZED_PDU_OBJECT_H_
