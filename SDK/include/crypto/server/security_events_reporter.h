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
/*!        \file  security_events_reporter.h
 *        \brief  Security events reporter entry point.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SECURITY_EVENTS_REPORTER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SECURITY_EVENTS_REPORTER_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "crypto/server/security_events_reporter_interface.h"

namespace crypto {
namespace server {

/*!
 * \brief Security events reporter entry point.
 */
class SecurityEventsReporter final {
 public:
  /*!
   * \brief Create a SecurityEventsReporterInterface instance.
   * \return The newly created SecurityEventsReporterInterface instance.
   */
  static auto CreateSecurityEventsReporter() noexcept -> std::unique_ptr<SecurityEventsReporterInterface>;
};

}  // namespace server
}  // namespace crypto

#endif  //  LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SECURITY_EVENTS_REPORTER_H_
