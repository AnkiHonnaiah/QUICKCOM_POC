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
/*!        \file  security_events_reporter_interface.h
 *        \brief  Interface for reporting security events.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SECURITY_EVENTS_REPORTER_INTERFACE_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SECURITY_EVENTS_REPORTER_INTERFACE_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"
#include "ara/crypto/common/base_id_types.h"

namespace crypto {
namespace server {

/*!
 * \brief           Interface for reporting security events.
 */
class SecurityEventsReporterInterface {
 public:
  /*!
   * \brief         Data type representing the context data to be serialized and used in the event reporting.
   * \vprivate      Component Private
   */
  struct ContextData {
    amsr::ipc::Credentials user_id{};        /*!< Effective process user id of the client trying to use the key slot. */
    ara::crypto::LogicalSlotUid slot_uuid{}; /*!< The key slot uuid that the client is trying to use. */
  };
  /*!
   * \brief         Construct an new EventReport instance.
   */
  SecurityEventsReporterInterface() noexcept = default;

  /*!
   * \brief         Destroy this EventReporter instance.
   */
  virtual ~SecurityEventsReporterInterface() noexcept = default;

  // SecurityEventsReporterInterface is not copy constructable
  SecurityEventsReporterInterface(SecurityEventsReporterInterface const&) noexcept = delete;

  // SecurityEventsReporterInterface is not copy assignable
  auto operator=(SecurityEventsReporterInterface const&) & noexcept -> SecurityEventsReporterInterface& = delete;

  /*!
   * \brief         Report the keyAccessDenied security event to the IDSM.
   * \param[in]     context_data
   *                The data to report as context.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \error         ara::crypto::SecurityErrc::kRuntimeFault Unable to report event.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  virtual auto ReportKeyAccessDenied(ContextData const& context_data) noexcept -> void = 0;

 protected:
  /*!
   * \brief         Move construct an SecurityEventsReporterInterface instance.
   */
  SecurityEventsReporterInterface(SecurityEventsReporterInterface&&) noexcept = default;

  /*!
   * \brief         Move construct an SecurityEventsReporterInterface instance.
   */
  auto operator=(SecurityEventsReporterInterface&&) & noexcept -> SecurityEventsReporterInterface& = default;
};

}  // namespace server
}  // namespace crypto

#endif  //  LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SECURITY_EVENTS_REPORTER_INTERFACE_H_
