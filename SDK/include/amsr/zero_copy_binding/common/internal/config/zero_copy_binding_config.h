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
 *         \brief  ZeroCopyBinding configuration types.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingCommon::Configuration
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_ZERO_COPY_BINDING_CONFIG_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_ZERO_COPY_BINDING_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/string.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "ara/core/vector.h"

namespace amsr {
namespace zero_copy_binding {
namespace common {
namespace internal {

namespace config {

/*!
 * \brief A short name.
 */
struct ShortName {  // VCA_ZEROCOPYBINDING_MOLE_1298
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
 * \brief A single event.
 */
struct Event {  // VCA_ZEROCOPYBINDING_MOLE_1298
  /*!
   * \brief The event id.
   */
  EventId event_id{};

  /*!
   * \brief The event short name.
   */
  ShortName short_name{};

  /*!
   * \brief Flag for enabling tracing for ReadSample() at runtime.
   */
  bool trace_read_sample_enabled{false};

  /*!
   * \brief Flag for enabling tracing for SendAllocate() at runtime.
   */
  bool trace_send_allocate_enabled{false};
};

/*!
 * \brief The number of connections.
 */
struct NumConnections {
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
 * \brief A connection endpoint.
 */
struct ConnectionEndpoint {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = std::uint32_t;

  /*!
   * \brief Data member to hold the domain.
   */
  Type domain;

  /*!
   * \brief Data member to hold the port.
   */
  Type port;
};

/*!
 * \brief A single resource limit.
 */
struct SingleResourceLimit {
  /*!
   * \brief Underlying data type of the value.
   */
  using Type = std::uint32_t;

  /*!
   * \brief Data member to hold the maximum number of slots.
   */
  Type max_slots;

  /*!
   * \brief Data member to hold the maximum number of connections.
   */
  Type max_connections;
};

/*!
 * \brief The resource limits.
 */
struct ResourceLimits {
  /*!
   * \brief QM resource limit.
   */
  SingleResourceLimit qm{};

  /*!
   * \brief ASIL_A resource limit.
   */
  SingleResourceLimit asil_a{};

  /*!
   * \brief ASIL_B resource limit.
   */
  SingleResourceLimit asil_b{};

  /*!
   * \brief ASIL_C resource limit.
   */
  SingleResourceLimit asil_c{};

  /*!
   * \brief ASIL_D resource limit.
   */
  SingleResourceLimit asil_d{};
};

/*!
 * \brief The type of initialization.
 */
struct Initialization {
  /*!
   * \brief Underlying data type of the value.
   */
  enum class Type : std::uint8_t { kNoInitialization, kZeroInitialization, kConstructorInitialization };

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief The type of memory technology.
 */
struct MemoryTechnology {
  /*!
   * \brief Underlying data type of the value.
   */
  enum class Type : std::uint8_t { kSharedMemory, kCustomMemory };

  /*!
   * \brief Data member to hold the value.
   */
  Type value;
};

/*!
 * \brief The instance specific provided event configuration.
 */
struct ProvidedInstanceSpecificEventConfig {
  /*!
   * \brief The provided event id.
   */
  EventId event_id{};

  /*!
   * \brief The number of allowed connections of this event.
   */
  NumConnections num_connections{};

  /*!
   * \brief The event specific resource limits
   */
  ResourceLimits resource_limits{};

  /*!
   * \brief The initialization type when providing a pointer to the shared memory area to the sender.
   */
  Initialization initialization{};

  /*!
   * \brief The memory technology used when allocating memory for the event.
   */
  MemoryTechnology allocation_memory_technology{};
};

/*!
 * \brief The instance specific required event configuration.
 */
struct RequiredInstanceSpecificEventConfig {
  /*!
   * \brief The required event id.
   */
  EventId event_id{};

  /*!
   * \brief The memory technology used when allocating memory for the event.
   */
  MemoryTechnology allocation_memory_technology{};
};

/*!
 * \brief A provided service instance.
 */
// VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
struct ProvidedServiceInstance {
  /*!
   * \brief  The service instance id.
   */
  InstanceId instance_id{};

  /*!
   * \brief The service instance specifier.
   */
  InstanceSpecifier instance_specifier{};

  /*!
   * \brief The connection endpoint.
   */
  ConnectionEndpoint connection_endpoint{};

  /*!
   * \brief A vector of all event configurations of this provided service instance.
   */
  ::ara::core::Vector<ProvidedInstanceSpecificEventConfig> pv_instance_specific_event_config{};
};

/*!
 * \brief A required service instance.
 */
struct RequiredServiceInstance {  // VCA_ZEROCOPYBINDING_MOLE_1298
  /*!
   * \brief The service instance id.
   */
  InstanceId instance_id{};

  /*!
   * \brief The service instance specifier.
   */
  InstanceSpecifier instance_specifier{};

  /*!
   * \brief The expected minimum integrity level of the server.
   */
  IntegrityLevel expected_minimum_server_integrity_level{};

  /*!
   * \brief A vector of all event configurations of this required service instance.
   */
  ::ara::core::Vector<RequiredInstanceSpecificEventConfig> rq_instance_specific_event_config{};
};

/*!
 * \brief A single service configuration.
 */
// VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
struct Service {
  /*!
   * \brief The service id.
   */
  ServiceId service_id{};

  /*!
   * \brief The service shortname path.
   */
  ShortnamePath shortname_path{};

  /*!
   * \brief The major version.
   */
  MajorVersion major_version{};

  /*!
   * \brief The minor version.
   */
  MinorVersion minor_version{};

  /*!
   * \brief Flag determining whether the service is considered to be generic.
   */
  bool is_generic{false};

  /*!
   * \brief A vector of all provided service instances for this service.
   */
  ::ara::core::Vector<ProvidedServiceInstance> provided_service_instances{};

  /*!
   * \brief A vector of all required service instances for this service.
   */
  ::ara::core::Vector<RequiredServiceInstance> required_service_instances{};

  /*!
   * \brief A vector of all configured events for this service.
   */
  ::ara::core::Vector<Event> events{};
};

/*!
 * \brief The configuration struct for ZeroCopyBinding.
 * \unit  ZeroCopyBinding::ZeroCopyBindingCommon::Configuration
 */
// VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
struct ZeroCopyBindingConfig {
  /*!
   * \brief A vector of all configured services.
   */
  ::ara::core::Vector<Service> services{};

  /*!
   * \brief Flag for enabling tracing at runtime.
   */
  bool trace_enabled{false};
};

}  // namespace config

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_ZERO_COPY_BINDING_CONFIG_H_
