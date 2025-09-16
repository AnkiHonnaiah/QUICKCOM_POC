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
/*!        \file
 *         \brief  Class describing validation info being sent upon connection establishment.
 *
 *       \details
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_CONNECTION_VALIDATION_INFO_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_CONNECTION_VALIDATION_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief Connection validation information.
 * \unit ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ConnectionValidationInfo
 */
class ConnectionValidationInfo {
 public:
  explicit ConnectionValidationInfo(common::internal::config::ConfigView const& config_view) noexcept;

  /*!
   * \brief       Check if validation info is in line with the provided service.
   * \param[in]   service The service to compare to.
   * \return      True if service ID, major version and minor version are equal, false otherwise.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto InLineWith(common::internal::config::Service const& service) const noexcept -> bool;

  /*!
   * \brief       Get the event Id of this validation info.
   * \return      The event Id of this validation info.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetEventId() const noexcept -> common::internal::config::EventId::Type;

  /*!
   * \brief       Set the trace class flag of the connection validation info.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  AMSR_LG_BETA void EnableAddToTraceClass() noexcept;

  /*!
   * \brief       Get the trace class flag of the connection validation info.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsAddToTraceClass() const noexcept -> bool;

 private:
  /*!
   * \brief The EventId of the event backend that requested to connect.
   */
  common::internal::config::EventId::Type const event_id_;

  /*!
   * \brief The ServiceId of the service instance that the event belongs to.
   */
  common::internal::config::ServiceId::Type const service_id_;

  /*!
   * \brief The MajorVersion of the service instance that the event belongs to.
   */
  common::internal::config::MajorVersion::Type const major_version_;

  /*!
   * \brief The MinorVersion of the service instance that the event belongs to.
   */
  common::internal::config::MinorVersion::Type const minor_version_;

  /*!
   * \brief Flag signalizing that tracing for the connection is enabled.
   */
  bool add_to_trace_class_{false};
};

static_assert(std::is_trivially_copyable<ConnectionValidationInfo>::value,
              "ConnectionValidationInfo must be trivially copyable");

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_CONNECTION_VALIDATION_INFO_H_
