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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  event_reporter_interface.h
 *        \brief  Interface to report security events.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_EVENT_REPORTER_INTERFACE_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_EVENT_REPORTER_INTERFACE_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/idsm/common.h"

namespace crypto {
namespace server {

/*!
 * \brief           Interface to report security events.
 */
class EventReporterInterface {
 public:
  /*!
   * \brief         Construct an new EventReport instance.
   */
  EventReporterInterface() noexcept = default;

  /*!
   * \brief         Destroy this EventReporter instance.
   */
  virtual ~EventReporterInterface() noexcept = default;

  // EventReporterInterface is not copy constructable
  EventReporterInterface(EventReporterInterface const&) noexcept = delete;

  // EventReporterInterface is not copy assignable
  auto operator=(EventReporterInterface const&) & noexcept -> EventReporterInterface& = delete;

  /*!
   * \brief         Report an IdsM security event.
   * \param[in]     context_data
   *                Context data
   * \param[in]     timestamp
   *                Application provided timestamp
   * \param[in]     count
   *                The count of the reported event
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual void ReportEvent(ara::idsm::ContextDataType const& context_data, ara::idsm::TimestampType timestamp,
                           ara::idsm::CountType count) noexcept = 0;

 protected:
  /*!
   * \brief         Move construct an EventReporterInterface instance.
   */
  EventReporterInterface(EventReporterInterface&&) noexcept = default;

  /*!
   * \brief         Move construct an EventReporterInterface instance.
   */
  auto operator=(EventReporterInterface&&) & noexcept -> EventReporterInterface& = default;
};

}  // namespace server
}  // namespace crypto

#endif  //  LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_EVENT_REPORTER_INTERFACE_H_
