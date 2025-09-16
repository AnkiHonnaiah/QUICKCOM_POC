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
/*!        \file  idsm_event_reporter.h
 *        \brief  Event reporter implementation reporting to ara::idsm.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_IDSM_EVENT_REPORTER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_IDSM_EVENT_REPORTER_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/idsm/event_reporter.h"

#include "amsr/core/optional.h"
#include "crypto/server/event_reporter_interface.h"

namespace crypto {
namespace server {

/*!
 * \brief           Event reporter implementation reporting to ara::idsm.
 */
class IdsmEventReporter final : public EventReporterInterface {
 public:
  /*!
   * \brief         Construct an IdsmEventReporter instance.
   * \param[in]     instance_specifier
   *                an instance specifier for the IDSM event reporter
   * \context       ANY
   * \pre           -
   */
  explicit IdsmEventReporter(amsr::core::InstanceSpecifier const& instance_specifier) noexcept
      : EventReporterInterface(), event_reporter_{instance_specifier} {}

  /*!
   * \brief         Destroy this IdsmEventReporter instance.
   */
  ~IdsmEventReporter() noexcept final = default;

  // IdsmEventReporter is not copy constructable
  IdsmEventReporter(IdsmEventReporter const&) noexcept = delete;

  /*!
   * \brief         Move construct an IdsmEventReporter instance.
   */
  IdsmEventReporter(IdsmEventReporter&&) noexcept = default;

  // IdsmEventReporter is not copy assignable
  auto operator=(IdsmEventReporter const&) & noexcept -> IdsmEventReporter& = delete;

  /*!
   * \brief         Move assign an IdsmEventReporter instance.
   */
  auto operator=(IdsmEventReporter&&) & noexcept -> IdsmEventReporter& = default;

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
  void ReportEvent(ara::idsm::ContextDataType const& context_data, ara::idsm::TimestampType timestamp,
                   ara::idsm::CountType count) noexcept final {
    if (event_reporter_.has_value()) {
      event_reporter_->ReportEvent(context_data, timestamp, count);
    }
  }

 private:
  /*!
   * \brief         Default construct an IdsmEventReporter instance.
   */
  IdsmEventReporter() noexcept = default;

  amsr::core::Optional<ara::idsm::EventReporter> event_reporter_; /*!< IdsM event reporter instance. */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__server__IdsmEventReporter, ReportEvent);
};

}  // namespace server
}  // namespace crypto

#endif  //  LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_IDSM_EVENT_REPORTER_H_
