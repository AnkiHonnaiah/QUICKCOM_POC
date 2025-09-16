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
/**        \file
 *        \brief  Defines the message types to realize the IPC protocol.
 *        \unit IpcBinding::IpcBindingCore::IpcProtocol
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_PROTOCOL_TYPES_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_PROTOCOL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <limits>

#include "ara/core/vector.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace ipc_protocol {

// ---- Base protocol attribute types ----------------------------------------------------------------------------------
/*!
 * \brief Type definition for the IPC service ID.
 */
struct ServiceId {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for ServiceId.
 *
 * \param[in] left   First ServiceId to compare.
 * \param[in] right  Second ServiceId to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(ServiceId const& left, ServiceId const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for ServiceId.
 *
 * \param[in] left   First ServiceId to compare.
 * \param[in] right  Second ServiceId to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(ServiceId const& left, ServiceId const& right) noexcept -> bool;

/*!
 * \brief Type definition for the IPC instance ID.
 */
struct InstanceId {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for InstanceId.
 *
 * \param[in] left   First InstanceId to compare.
 * \param[in] right  Second InstanceId to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(InstanceId const& left, InstanceId const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for InstanceId.
 *
 * \param[in] left   First InstanceId to compare.
 * \param[in] right  Second InstanceId to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(InstanceId const& left, InstanceId const& right) noexcept -> bool;

/*!
 * \brief Type definition for major version.
 */
struct MajorVersion {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for MajorVersion.
 *
 * \param[in] left   First MajorVersion to compare.
 * \param[in] right  Second MajorVersion to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(MajorVersion const& left, MajorVersion const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for MajorVersion.
 *
 * \param[in] left   First MajorVersion to compare.
 * \param[in] right  Second MajorVersion to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(MajorVersion const& left, MajorVersion const& right) noexcept -> bool;

/*!
 * \brief Type definition for minor version.
 */
struct MinorVersion {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for MinorVersion.
 *
 * \param[in] left   First MinorVersion to compare.
 * \param[in] right  Second MinorVersion to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(MinorVersion const& left, MinorVersion const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for MinorVersion.
 *
 * \param[in] left   First MinorVersion to compare.
 * \param[in] right  Second MinorVersion to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(MinorVersion const& left, MinorVersion const& right) noexcept -> bool;

/*!
 * \brief Type definition for the IPC method ID.
 */
struct MethodId {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for MethodId.
 *
 * \param[in] left   First MethodId to compare.
 * \param[in] right  Second MethodId to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(MethodId const& left, MethodId const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for MethodId.
 *
 * \param[in] left   First MethodId to compare.
 * \param[in] right  Second MethodId to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(MethodId const& left, MethodId const& right) noexcept -> bool;

/*!
 * \brief Type definition for the IPC event ID.
 */
struct EventId {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for EventId.
 *
 * \param[in] left   First EventId to compare.
 * \param[in] right  Second EventId to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(EventId const& left, EventId const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for EventId.
 *
 * \param[in] left   First EventId to compare.
 * \param[in] right  Second EventId to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(EventId const& left, EventId const& right) noexcept -> bool;

/*!
 * \brief Type definition for IPC client ID.
 */
struct ClientId {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for ClientId.
 *
 * \param[in] left   First ClientId to compare.
 * \param[in] right  Second ClientId to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(ClientId const& left, ClientId const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for ClientId.
 *
 * \param[in] left   First ClientId to compare.
 * \param[in] right  Second ClientId to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(ClientId const& left, ClientId const& right) noexcept -> bool;

/*!
 * \brief Type definition for IPC session ID.
 */
struct SessionId {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = std::uint32_t;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for SessionId.
 *
 * \param[in] left   First SessionId to compare.
 * \param[in] right  Second SessionId to compare.
 *
 * \return True if the compared objects are equal, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator==(SessionId const& left, SessionId const& right) noexcept -> bool;

/*!
 * \brief Less than compare operator for SessionId.
 *
 * \param[in] left   First SessionId to compare.
 * \param[in] right  Second SessionId to compare.
 *
 * \return True if first object is less than the second object, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 *
 * \spec requires true; \endspec
 */
auto operator<(SessionId const& left, SessionId const& right) noexcept -> bool;

// ---- Tuple protocol types -------------------------------------------------------------------------------------------

/*!
 * \brief Identifier for every service instance consisting of ServiceId, InstanceId and MajorVersion.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class ServiceInstanceIdentifier final {
 public:
  /*!
   * \brief Construct a ServiceInstanceIdentifier.
   *
   * \param[in] service_id     ServiceInterface ID.
   * \param[in] instance_id    Instance ID.
   * \param[in] major_version  Major Version.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ServiceInstanceIdentifier(ipc_protocol::ServiceId const service_id, ipc_protocol::InstanceId const instance_id,
                            ipc_protocol::MajorVersion const major_version) noexcept
      : service_id_{service_id}, instance_id_{instance_id}, major_version_{major_version} {}

  /*!
   * \brief Copy-Construct a ServiceInstanceIdentifier.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ServiceInstanceIdentifier(ServiceInstanceIdentifier const&) noexcept = default;

  /*!
   * \brief Move-Construct a ServiceInstanceIdentifier.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ServiceInstanceIdentifier(ServiceInstanceIdentifier&&) noexcept = default;

  /*!
   * \brief Move-Assign a ServiceInstanceIdentifier.
   *
   * \return Reference to the move-assigned ServiceInstanceIdentifier.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto operator=(ServiceInstanceIdentifier&&) & noexcept -> ServiceInstanceIdentifier& = default;

  ServiceInstanceIdentifier() noexcept = delete;
  auto operator=(ServiceInstanceIdentifier const&) noexcept -> ServiceInstanceIdentifier& = delete;

  /*!
   * \brief Destroy ServiceInstanceIdentifier.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~ServiceInstanceIdentifier() = default;

  /*!
   * \brief Get the ServiceId.
   *
   * \return The ServiceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto ServiceId() const noexcept -> ipc_protocol::ServiceId { return service_id_; }

  /*!
   * \brief Get the InstanceId.
   *
   * \return The InstanceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto InstanceId() const noexcept -> ipc_protocol::InstanceId { return instance_id_; }

  /*!
   * \brief Get the MajorVersion.
   *
   * \return The MajorVersion.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto MajorVersion() const noexcept -> ipc_protocol::MajorVersion { return major_version_; }

  /*!
   * \brief Compares this service instance identifier with the given one for equality.
   *
   * \param[in] other  Other service instance identifier to compare with.
   *
   * \return True if both the service instance identifiers are equal, false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator==(ServiceInstanceIdentifier const& other) const noexcept -> bool;

  /*!
   * \brief Less than comparison operator for the class ServiceInstanceIdentifier.
   *
   * \param[in] other  Other service instance to compare with.
   *
   * \return True if every element of the class is smaller than the given one, false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator<(ServiceInstanceIdentifier const& other) const noexcept -> bool;

  /*!
   * \brief Compare this service instance ID with the given service ID, instance ID and major version for equality.
   *
   * \param[in] service_id     The service ID to compare.
   * \param[in] instance_id    The instance ID to compare.
   * \param[in] major_version  The major version to compare.
   *
   * \return True if all IDs / versions are equal, false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto Equal(ipc_protocol::ServiceId const service_id, ipc_protocol::InstanceId const instance_id,
             ipc_protocol::MajorVersion const major_version) const noexcept -> bool;

 private:
  /*!
   * \brief Service identifier.
   */
  ipc_protocol::ServiceId service_id_;

  /*!
   * \brief Instance identifier.
   */
  ipc_protocol::InstanceId instance_id_;

  /*!
   * \brief Service major version.
   */
  ipc_protocol::MajorVersion major_version_;
};

/*!
 * \brief A vector of service instances.
 */
using ServiceInstanceContainer = ara::core::Vector<ServiceInstanceIdentifier>;

/*!
 * \brief Wild-card for searching 'ALL' instances of a service.
 */
static constexpr InstanceId kInstanceIdAll{std::numeric_limits<InstanceId::type>::max()};

/*!
 * \brief Wild-card for searching 'any' minor versions of a service instance.
 */
static constexpr MinorVersion kMinorVersionAny{MinorVersion{std::numeric_limits<MinorVersion::type>::max()}};

/*!
 * \brief Type definition for subscription state of an event.
 */
enum class SubscriptionState : uint8_t { kSubscribed = 0U, kNotSubscribed = 1U, kSubscriptionPending = 2U };

/*!
 * \brief Service discovery event subscription state entry.
 */
struct EventSubscriptionState {
  /*!
   * \brief IPC service identifier.
   */
  ServiceId service_id_;
  /*!
   * \brief IPC service instance identifier.
   */
  InstanceId instance_id_;
  /*!
   * \brief IPC event identifier.
   */
  EventId event_id_;
  /*!
   * \brief An event subscription state.
   */
  SubscriptionState state_;
};

/*!
 * \brief Type definition for provided service state.
 */
enum class ServiceState : std::uint8_t { kServiceUp = 0U, kServiceDown = 1U };

}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_PROTOCOL_TYPES_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_FunctionCallCoverage_unused_utility_function
//   \ACCEPT  XX
//   \REASON  The API is existing to provide a complete basic utility class.
//            A coverage gap is reported for the API which is never directly used by other units.
//            The API is verified with unit tests. Therefore no further component or multi unit tests are necessary.
// COV_JUSTIFICATION_END
