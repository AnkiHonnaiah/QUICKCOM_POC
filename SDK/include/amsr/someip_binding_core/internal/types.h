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
/**        \file  amsr/someip_binding_core/internal/types.h
 *        \brief  SOME/IP binding specific types.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SomeIpTypes
 *
 *      \details  -
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_TYPES_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/shared_ptr.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/memory/allocator/deterministic_memory_buffer_allocator.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/memory/non_zeroing_allocator.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Alias for the memory buffer.
 */
using MemoryBuffer = ::vac::memory::allocator::MemoryBuffer;

/*!
 * \brief Shared pointer type for the memory buffer.
 */
using DataBufferSharedPtr = amsr::SharedPtr<MemoryBuffer const>;

/*!
 * \brief Memory buffer pointer alias.
 */
using MemoryBufferPtr = ::vac::memory::allocator::MemoryBufferPtr;

/*!
 * \brief FlexibleUniqueMemoryBufferAllocator alias.
 */
using FlexibleUniqueMemoryBufferAllocator = ::vac::memory::allocator::flexible::FlexibleUniqueMemoryBufferAllocator<
    vac::memory::NonZeroingAllocator<std::uint8_t>>;

/*!
 * \brief MemoryBufferAllocator alias.
 */
using MemoryBufferAllocator = ::vac::memory::allocator::MemoryBufferAllocator;

/*!
 * \brief DeterministicMemoryBufferAllocator alias.
 */
using DeterministicMemoryBufferAllocator = vac::memory::allocator::deterministic::DeterministicMemoryBufferAllocator;

/*!
 * \brief PolymorphicAllocator alias.
 */
using PacketAllocator = ara::core::PolymorphicAllocator<std::uint8_t>;

/*!
 * \brief FlexiblePolymorphicUniqueMemoryBuffer alias.
 */
using FlexiblePolymorphicUniqueMemoryBuffer =
    vac::memory::allocator::flexible::FlexibleUniqueMemoryBuffer<PacketAllocator>;

/*!
 * \brief DeterministicMemoryBufferAllocator alias.
 */
using DeterministicMemoryBufferAllocator = vac::memory::allocator::deterministic::DeterministicMemoryBufferAllocator;

/*!
 * \brief Uniquely identify an event by a combination of service id, major version, instance id, event id.
 */
class SomeIpEventIdentity {
 public:
  /*!
   * \brief  constructor.
   * \param[in] service_id    Service ID of a SOME/IP service.
   * \param[in] major_version Major Version of a SOME/IP service.
   * \param[in] instance_id   Instance ID of a SOME/IP service.
   * \param[in] event_id      Event ID of a SOME/IP service.
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SomeIpEventIdentity(::amsr::someip_protocol::internal::ServiceId const service_id,
                      ::amsr::someip_protocol::internal::MajorVersion const major_version,
                      ::amsr::someip_protocol::internal::InstanceId const instance_id,
                      ::amsr::someip_protocol::internal::EventId const event_id) noexcept
      // VCA_SOMEIPBINDING_SOMEIP_EVENT_IDENTITY_CONSTRUCTOR
      : service_id_{service_id}, major_version_{major_version}, instance_id_{instance_id}, event_id_{event_id} {}

  /*!
   * \brief Overloads < operator
   *
   * \param[in] event_identity The SomeIpEventIdentity to compare against.
   *
   * \return True if event_id is greater than the compared SomeIpEventIdentity's event_id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator<(SomeIpEventIdentity const& event_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, event_id_) <
            std::tie(event_identity.service_id_, event_identity.instance_id_, event_identity.event_id_));
  }

  /*!
   * \brief Getter for Service ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief Getter for Instance ID
   * \return Instance ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::InstanceId GetInstanceId() const noexcept { return instance_id_; }

  /*!
   * \brief Getter for Event ID
   * \return Event ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::EventId GetEventId() const noexcept { return event_id_; }

  /*!
   * \brief Getter for Major version
   * \return Major version
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MajorVersion GetMajorVersion() const noexcept { return major_version_; }

 private:
  /*!
   * \brief Service ID of a SOME/IP service
   */
  ::amsr::someip_protocol::internal::ServiceId service_id_;
  /*!
   * \brief SOME/IP major version
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version_;
  /*!
   * \brief Instance ID of a SOME/IP service
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id_;
  /*!
   * \brief SOME/IP event identifier
   */
  ::amsr::someip_protocol::internal::EventId event_id_;
};

/*!
 * \brief Type definition for ID of SomeIpRequiredServiceInstance.
 */
class RequiredServiceInstanceId {
 public:
  /*!
   * \brief  constructor.
   * \param[in] service_id    Service ID of a SOME/IP service.
   * \param[in] major_version Major version of a SOME/IP service.
   * \param[in] minor_version Minor version of a SOME/IP service.
   * \param[in] instance_id   Instance ID of a SOME/IP service.
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  RequiredServiceInstanceId(::amsr::someip_protocol::internal::ServiceId const service_id,
                            ::amsr::someip_protocol::internal::MajorVersion const major_version,
                            ::amsr::someip_protocol::internal::MinorVersion const minor_version,
                            ::amsr::someip_protocol::internal::InstanceId const instance_id) noexcept
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      : service_id_{service_id},
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        major_version_{major_version},
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        minor_version_{minor_version},
        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        instance_id_{instance_id} {}

  /*!
   * \brief Overloads < operator
   *
   * \param[in] instance_identity The RequiredServiceInstanceId to compare against.
   *
   * \return True if any member of instance_identity is greater than the corresponding member of this id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator<(RequiredServiceInstanceId const& instance_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_, minor_version_) <
            std::tie(instance_identity.service_id_, instance_identity.instance_id_, instance_identity.major_version_,
                     instance_identity.minor_version_));
  }

  /*!
   * \brief Define == operator
   *
   * \param[in] instance_identity The RequiredServiceInstanceId to compare against.
   *
   * \return True if each member of instance_identity is equal to the corresponding member of this id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(RequiredServiceInstanceId const& instance_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_, minor_version_) ==
            std::tie(instance_identity.service_id_, instance_identity.instance_id_, instance_identity.major_version_,
                     instance_identity.minor_version_));
  }

  /*!
   * \brief Getter for Service ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief Getter for Instance ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::InstanceId GetInstanceId() const noexcept { return instance_id_; }

  /*!
   * \brief Getter for Major Version
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MajorVersion GetMajorVersion() const noexcept { return major_version_; }

  /*!
   * \brief Getter for Minor Version
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MinorVersion GetMinorVersion() const noexcept { return minor_version_; }

 private:
  /*!
   * \brief SOME/IP service ID
   */
  ::amsr::someip_protocol::internal::ServiceId service_id_;

  /*!
   * \brief SOME/IP service interface major version
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version_;

  /*!
   * \brief SOME/IP service interface minor version
   */
  ::amsr::someip_protocol::internal::MinorVersion minor_version_;

  /*!
   * \brief SOME/IP service instance ID
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id_;
};

/*!
 * \brief Type definition for ID of SomeIpProvidedServiceInstance.
 */
class ProvidedServiceInstanceId {
 public:
  /*!
   * \brief  constructor.
   * \param[in] service_id    Service ID of a SOME/IP service.
   * \param[in] major_version Major version of a SOME/IP service.
   * \param[in] minor_version Minor version of a SOME/IP service.
   * \param[in] instance_id   Instance ID of a SOME/IP service.
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ProvidedServiceInstanceId(::amsr::someip_protocol::internal::ServiceId const service_id,
                            ::amsr::someip_protocol::internal::MajorVersion const major_version,
                            ::amsr::someip_protocol::internal::MinorVersion const minor_version,
                            ::amsr::someip_protocol::internal::InstanceId const instance_id) noexcept
      : service_id_{service_id},
        major_version_{major_version},
        minor_version_{minor_version},
        instance_id_{instance_id} {}

  /*!
   * \brief Overloads < operator
   *
   * \param[in] instance_identity The ProvidedServiceInstanceId to compare against.
   *
   * \return True if any member of instance_identity is greater than the corresponding member of this id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator<(ProvidedServiceInstanceId const& instance_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_, minor_version_) <
            std::tie(instance_identity.service_id_, instance_identity.instance_id_, instance_identity.major_version_,
                     instance_identity.minor_version_));
  }

  /*!
   * \brief Define == operator
   *
   * \param[in] instance_identity The ProvidedServiceInstanceId to compare against.
   *
   * \return True if each member of instance_identity is equal to the corresponding member of this id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(ProvidedServiceInstanceId const& instance_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_, minor_version_) ==
            std::tie(instance_identity.service_id_, instance_identity.instance_id_, instance_identity.major_version_,
                     instance_identity.minor_version_));
  }

  /*!
   * \brief Getter for Service ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief Getter for Instance ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::InstanceId GetInstanceId() const noexcept { return instance_id_; }

  /*!
   * \brief Getter for Major Version
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MajorVersion GetMajorVersion() const noexcept { return major_version_; }

  /*!
   * \brief Getter for Minor Version
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MinorVersion GetMinorVersion() const noexcept { return minor_version_; }

 private:
  /*!
   * \brief SOME/IP service ID
   */
  ::amsr::someip_protocol::internal::ServiceId service_id_;

  /*!
   * \brief SOME/IP service interface major version
   */
  ::amsr::someip_protocol::internal::MajorVersion major_version_;

  /*!
   * \brief SOME/IP service interface minor version
   */
  ::amsr::someip_protocol::internal::MinorVersion minor_version_;

  /*!
   * \brief SOME/IP service instance ID
   */
  ::amsr::someip_protocol::internal::InstanceId instance_id_;
};

/*!
 * \brief Identifies a skeleton method request handler that has to be registered.
 */
struct RequiredSkeletonMethodHandler {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] method_identifier   Method ID.
   * \param[in] method_is_no_return True if the method is a no return method for fire and forget use.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit RequiredSkeletonMethodHandler(someip_protocol::internal::MethodId method_identifier,
                                         bool method_is_no_return)
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      : method_id{method_identifier},
        // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
        is_no_return(method_is_no_return) {}

  /*!
   * \brief SOME/IP method ID for which a method handler has to be registered.
   */
  someip_protocol::internal::MethodId method_id;

  /*!
   * \brief Indicated if a no return handler for fire and forget methods need to be added or a normal request handler.
   * True means no return is active.
   */
  bool is_no_return;
};

/*!
 * \brief Class to uniquely identify a proxy binding with a tuple of service id, instance id, client id.
 */
class ProxyBindingIdentity final {
 public:
  /*!
   * \brief  constructor.
   * \param[in] service_id    Service ID of a SOME/IP service.
   * \param[in] major_version Major version of a SOME/IP service.
   * \param[in] instance_id   Instance ID of a SOME/IP service.
   * \param[in] client_id     Client ID of a SOME/IP service.
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ProxyBindingIdentity(::amsr::someip_protocol::internal::ServiceId const service_id,
                       ::amsr::someip_protocol::internal::MajorVersion const major_version,
                       ::amsr::someip_protocol::internal::InstanceId const instance_id,
                       ::amsr::someip_protocol::internal::ClientId const client_id)

      : service_id_{service_id}, major_version_{major_version}, instance_id_{instance_id}, client_id_{client_id} {}

  /*!
   * \brief Getter for Service ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ServiceId GetServiceId() const noexcept { return service_id_; }

  /*!
   * \brief  Getter for Major version
   * \return Major version
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::MajorVersion GetMajorVersion() const noexcept { return major_version_; }

  /*!
   * \brief Getter for Instance ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::InstanceId GetInstanceId() const noexcept { return instance_id_; }

  /*!
   * \brief Getter for Client ID
   * \return Service ID
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::amsr::someip_protocol::internal::ClientId GetClientId() const noexcept { return client_id_; }

  /*!
   * \brief Overloads < operator
   *
   * \param[in] instance_identity The ProxyBindingIdentity to compare against.
   *
   * \return True if any member of instance_identity is greater than the corresponding member of this id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator<(ProxyBindingIdentity const& instance_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_, client_id_) <
            std::tie(instance_identity.service_id_, instance_identity.instance_id_, instance_identity.major_version_,
                     instance_identity.client_id_));
  }

  /*!
   * \brief Define == operator
   *
   * \param[in] instance_identity The ProxyBindingIdentity to compare against.
   *
   * \return True if each member of instance_identity is equal to the corresponding member of this id, else false
   * \pre          -
   * \context      ANY
   * \threadsafe   TRUE
   * \reentrant    TRUE
   * \synchronous  TRUE
   * \steady       TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(ProxyBindingIdentity const& instance_identity) const noexcept {
    return (std::tie(service_id_, instance_id_, major_version_, client_id_) ==
            std::tie(instance_identity.service_id_, instance_identity.instance_id_, instance_identity.major_version_,
                     instance_identity.client_id_));
  }

 private:
  /*!
   * \brief The service id
   */
  ::amsr::someip_protocol::internal::ServiceId const service_id_;
  /*!
   * \brief The Major Version.
   */
  ::amsr::someip_protocol::internal::MajorVersion const major_version_;
  /*!
   * \brief The instance id
   */
  ::amsr::someip_protocol::internal::InstanceId const instance_id_;
  /*!
   * \brief The client id
   */
  ::amsr::someip_protocol::internal::ClientId const client_id_;
};

/*!
 * \brief Enumeration defining the serialization mode.
 */
enum class EventSerialization : std::uint8_t {
  kSignalBased, /*!< Use signal based serialization. */
  kSomeip       /*!< Use someip serialization. */
};

/*!
 * \brief Struct to hold the event information.
 */
// VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
struct EventInfo {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] event_shortname   Event shortname.
   * \param[in] event_identifier  Event ID.
   * \param[in] serialization     Event serialization type.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit EventInfo(::amsr::core::String event_shortname, ::amsr::someip_protocol::internal::EventId event_identifier,
                     EventSerialization serialization)
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      : shortname{std::move(event_shortname)},
        // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
        event_id(event_identifier),
        // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
        serialization_type(serialization) {}  // COV_SOMEIPBINDING_TYPES

  /*!
   * \brief Event shortname.
   */
  ::amsr::core::String shortname;

  /*!
   * \brief The event id.
   */
  ::amsr::someip_protocol::internal::EventId event_id;

  /*!
   * \brief The serialization type.
   */
  EventSerialization serialization_type;
};

/*!
 * \brief Struct to hold the event information.
 */
// VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
struct ServiceEventInformation {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief     Put a new event into the collection, constructed in-place from the given arguments.
   * \param[in] event_shortname   Event shortname.
   * \param[in] event_identifier  Event ID.
   * \param[in] serialization     Event serialization type.
   * \pre       -
   */
  void AddEventInfo(::amsr::core::String event_shortname, ::amsr::someip_protocol::internal::EventId event_identifier,
                    EventSerialization serialization) noexcept {  // COV_SOMEIPBINDING_TYPES
    // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
    events.emplace_back(event_shortname, event_identifier, serialization);
  }

  /*!
   * \brief Event information collection.
   */
  ::ara::core::Vector<EventInfo> events;
};

/*!
 * \brief Struct to hold the method information.
 */
// VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
struct MethodInfo {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] method_shortname  Method shortname.
   * \param[in] method_identifier Method ID.
   * \param[in] is_fire_and_forget_flag Flag that indicates if this is a regular method or a Fire And Forget method.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit MethodInfo(::amsr::core::String method_shortname,
                      ::amsr::someip_protocol::internal::MethodId method_identifier, bool is_fire_and_forget_flag)
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      : shortname{std::move(method_shortname)},
        // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
        method_id(method_identifier),
        // VCA_SOMEIPBINDING_ACCESSING_OBJECT_POSSIBLY_INVALID
        is_fire_and_forget(is_fire_and_forget_flag) {}  // COV_SOMEIPBINDING_TYPES

  /*!
   * \brief Method shortname.
   */
  ::amsr::core::String shortname;

  /*!
   * \brief The method id.
   */
  ::amsr::someip_protocol::internal::MethodId method_id;

  /*!
   * \brief The isFireAndForget flag.
   */
  bool is_fire_and_forget;
};

/*!
 * \brief Struct to hold the method information.
 */
// VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
struct ServiceMethodInformation {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief     Put a new method into the collection, constructed in-place from the given arguments.
   * \param[in] method_shortname  Method shortname.
   * \param[in] method_identifier Method ID.
   * \param[in] is_fire_and_forget_flag Flag that indicates if this is a regular method or a Fire And Forget method.
   * \pre       -
   */
  void AddMethodInfo(::amsr::core::String method_shortname,
                     ::amsr::someip_protocol::internal::MethodId method_identifier,
                     bool is_fire_and_forget_flag) noexcept {  // COV_SOMEIPBINDING_TYPES
    // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
    methods.emplace_back(method_shortname, method_identifier, is_fire_and_forget_flag);
  }

  /*!
   * \brief Method information collection.
   */
  ::ara::core::Vector<MethodInfo> methods;
};

/*!
 * \brief Struct to hold the field notifier information.
 */
struct FieldNotifierInfo {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] event_identifier  Event ID.
   * \param[in] serialization     Event serialization type.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit FieldNotifierInfo(::amsr::someip_protocol::internal::EventId event_identifier,
                             EventSerialization serialization)
      : event_id{event_identifier}, serialization_type{serialization} {}  // COV_SOMEIPBINDING_TYPES

  /*!
   * \brief The event id.
   */
  ::amsr::someip_protocol::internal::EventId event_id;

  /*!
   * \brief The serialization type.
   */
  EventSerialization serialization_type;
};

/*!
 * \brief Struct to hold the field getter information.
 */
struct FieldGetterInfo {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] method           Method ID.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit FieldGetterInfo(::amsr::someip_protocol::internal::MethodId method)
      : method_id{method} {}  // COV_SOMEIPBINDING_TYPES

  /*!
   * \brief The method id.
   */
  ::amsr::someip_protocol::internal::MethodId method_id;
};

/*!
 * \brief Struct to hold the field setter information.
 */
struct FieldSetterInfo {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] method           Method ID.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  explicit FieldSetterInfo(::amsr::someip_protocol::internal::MethodId method)
      : method_id{method} {}  // COV_SOMEIPBINDING_TYPES

  /*!
   * \brief The method id.
   */
  ::amsr::someip_protocol::internal::MethodId method_id;
};

/*!
 * \brief Struct to hold the field information.
 */
// VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
struct FieldInfo {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief  Constructor.
   * \param[in] field_shortname Shortname.
   * \param[in] notifier        Optional field notifier.
   * \param[in] getter          Optional field getter.
   * \param[in] setter          Optional field setter.
   * \pre          -
   * \context      ANY
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   */
  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
  explicit FieldInfo(::amsr::core::String field_shortname, ::amsr::core::Optional<FieldNotifierInfo> notifier,
                     ::amsr::core::Optional<FieldGetterInfo> getter, amsr::core::Optional<FieldSetterInfo> setter)
      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      : shortname{std::move(field_shortname)},
        // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
        notifier_info(std::move(notifier)),
        // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
        getter_info(std::move(getter)),
        // VCA_SOMEIPBINDING_FINDINGS_IN_CONSTRUCTORS_MEMBER_INITIALIZATION
        setter_info(std::move(setter)) {}  // COV_SOMEIPBINDING_TYPES

  /*!
   * \brief Field shortname.
   */
  ::amsr::core::String shortname;

  /*!
   * \brief Optional field notifier information.
   */
  ::amsr::core::Optional<FieldNotifierInfo> notifier_info;

  /*!
   * \brief Optional getter method information.
   */
  ::amsr::core::Optional<FieldGetterInfo> getter_info;

  /*!
   * \brief Optional setter method information.
   */
  ::amsr::core::Optional<FieldSetterInfo> setter_info;
};

/*!
 * \brief The service field information.
 */
// VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
struct ServiceFieldInformation {
  // VECTOR NC VectorC++-V11.0.3: MD_SOMEIPBINDING_VectorC++-V11.0.3_struct_public_data_members
  /*!
   * \brief     Put a new field into the collection, constructed in-place from the given arguments.
   * \param[in] field_shortname Shortname.
   * \param[in] notifier        Optional field notifier.
   * \param[in] getter          Optional field getter.
   * \param[in] setter          Optional field setter.
   * \pre       -
   */
  void AddFieldInfo(::amsr::core::String field_shortname, ::amsr::core::Optional<FieldNotifierInfo> notifier,
                    ::amsr::core::Optional<FieldGetterInfo> getter,
                    amsr::core::Optional<FieldSetterInfo> setter) noexcept {  // COV_SOMEIPBINDING_TYPES
    // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
    fields.emplace_back(field_shortname, notifier, getter, setter);
  }

  /*!
   * \brief Field information collection of a service instance.
   */
  ::ara::core::Vector<FieldInfo> fields;
};

/*!
 * \brief Struct to hold the service information.
 */
struct ServiceInformation {
  /*!
   * \brief The service event information.
   */
  ServiceEventInformation event_information;

  /*!
   * \brief The service method information.
   */
  ServiceMethodInformation method_information;

  /*!
   * \brief The service field information.
   */
  ServiceFieldInformation field_information;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPBINDING_TYPES
//   \ACCEPT  XX
//   \REASON  Trivial functions are reported as uncovered but are covered in tests, e.g.:
//            UT__BindingCoreInitializer_ServiceInformationMapping.
//            WIZRD-1802 is tracking this issue and will investigate a possible tooling issue.
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_TYPES_H_
