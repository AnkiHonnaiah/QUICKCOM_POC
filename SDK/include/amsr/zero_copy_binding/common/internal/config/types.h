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
 *         \brief  Define commonly used types.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingCommon::Configuration
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_TYPES_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <limits>
#include "amsr/core/string.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"

namespace amsr {
namespace zero_copy_binding {
namespace common {
namespace internal {

namespace config {

/*!
 * \brief Type definition for the ZeroCopyBinding service ID.
 */
struct ServiceId {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = service_discovery::ServiceInstanceIdentifier::ServiceId;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding InstanceId.
 */
struct InstanceId {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = service_discovery::ServiceInstanceIdentifier::InstanceId;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding MajorVersion.
 */
struct MajorVersion {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = service_discovery::ServiceInstanceIdentifier::MajorVersion;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding MinorVersion.
 */
struct MinorVersion {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = service_discovery::ServiceInstanceIdentifier::MinorVersion;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding ShortnamePath.
 */
struct ShortnamePath {  // VCA_ZEROCOPYBINDING_MOLE_1298
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = ::amsr::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding InstanceSpecifier.
 */
struct InstanceSpecifier {  // VCA_ZEROCOPYBINDING_MOLE_1298
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = ::amsr::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding IntegrityLevel.
 */
struct IntegrityLevel {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Type definition for the ZeroCopyBinding EventId.
 */
struct EventId {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief Wild-card for searching 'ALL' instances of a service.
 */
static constexpr InstanceId kInstanceIdAll{service_discovery::RequiredServiceInstanceIdentifier::kInstanceIdAll};

/*!
 * \brief Wild-card for searching 'any' minor versions of a service instance.
 */
static constexpr MinorVersion kMinorVersionAny{
    MinorVersion{service_discovery::RequiredServiceInstanceIdentifier::kMinorVersionAny}};

}  // namespace config

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_TYPES_H_
