/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  transporter_event_handler.h
 *        \brief  Transporter callback interface.
 *
 *      \details  There are various events which may occur during an IPC connection.
 *                This interface defines callback functions to handle these events.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_EVENT_HANDLER_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_EVENT_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/types.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Transporter callback interface.
 * \details There are various events which may occur during an IPC connection.
 * This interface defines callback functions to handle these events.
 * \vprivate Component Private
 */
class TransporterEventHandler {
 public:
  /*!
   * \brief Destroys the object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual ~TransporterEventHandler() noexcept = default;

  /*!
   * \brief Constructs the object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  TransporterEventHandler() noexcept = default;

  /*!
   * \brief Handles message received event.
   * \param[in] data data.
   * \param[in] e_uid Effective user id of the process which sent the message.
   * \param[in] p_uid Process id of the process which sent the message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnReceived(ReadOnlyMemRegion data, UserId e_uid, ProcessId p_uid) noexcept = 0;

  /*!
   * \brief Handles message sent event.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnSent() noexcept = 0;

  /*!
   * \brief Handles disconnection event.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void OnDisconnect() noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \vprivate Component Private
   */
  TransporterEventHandler(TransporterEventHandler const& /*other*/) noexcept = default;

  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  TransporterEventHandler(TransporterEventHandler&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator.
   * \return reference to this
   * \vprivate Component Private
   */
  TransporterEventHandler& operator=(TransporterEventHandler const& /*other*/) & noexcept = default;

  /*!
   * \brief default moves assignment operator.
   * \return reference to this
   * \vprivate Component Private
   */
  TransporterEventHandler& operator=(TransporterEventHandler&& /*other*/) & noexcept = default;
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_EVENT_HANDLER_H_
