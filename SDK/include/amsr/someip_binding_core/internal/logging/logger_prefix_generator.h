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
/*!        \file  logger_prefix_generator.h
 *        \brief  Generates a logger prefix
 *        \unit  SomeIpBinding::SomeIpBindingCore::Utilities
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_LOGGER_PREFIX_GENERATOR_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_LOGGER_PREFIX_GENERATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/string.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief Logger prefix generator
 * \details Generates logger prefixes to log the class name and its attributes
 */
class LoggerPrefixGenerator final {
 public:
  /*!
   * \brief Generates a logger prefix with the class name, service id, instance_id, major_version and minor_version
   * \param[in] class_name    The class name
   * \param[in] service_id    The service id
   * \param[in] instance_id   The instance id
   * \param[in] major_version The major version
   * \param[in] minor_version The minor version
   * \return The string with the logger prefix
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(amsr::core::StringView const class_name,
                                            someip_protocol::internal::ServiceId const service_id,
                                            someip_protocol::internal::InstanceId const instance_id,
                                            someip_protocol::internal::MajorVersion const major_version,
                                            someip_protocol::internal::MinorVersion const minor_version) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);

    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);

    prefix_builder << amsr::core::StringView{" (ServiceId: "} << service_id;

    prefix_builder << amsr::core::StringView{", InstanceId: "} << instance_id;

    prefix_builder << amsr::core::StringView{", Version: "} << major_version;

    prefix_builder << '.' << minor_version << ')';

    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a logger prefix with the class name, service id, instance id, major version, client id and method
   * id.
   * \param[in] class_name       The class name
   * \param[in] proxy_identity   Proxy binding identity, containing service id, major version,
   *                             instance id and client id.
   * \param[in] method_id        The method id
   * \return The string with the logger prefix with the format
   *         "ClassName (ServiceId: 0xFFFF, InstanceId: 0xFFFF, MajorVersion: 0xFFFF,
   *         ClientId: 0xFFFF, MethodId: 0xFFFF)"
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetClientMethodLoggerPrefix(
      amsr::core::StringView const class_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_protocol::internal::MethodId const method_id) noexcept {
    constexpr std::size_t kBufferSize{150};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);

    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);

    prefix_builder << amsr::core::StringView{" (ServiceId: "} << proxy_identity.GetServiceId();

    prefix_builder << amsr::core::StringView{", InstanceId: "} << proxy_identity.GetInstanceId();

    prefix_builder << amsr::core::StringView{", MajorVersion: "} << proxy_identity.GetMajorVersion();

    prefix_builder << amsr::core::StringView{", ClientId: "} << proxy_identity.GetClientId();

    prefix_builder << amsr::core::StringView{", MethodId: "} << method_id;

    prefix_builder << ')';

    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a logger prefix for EventXf classes
   * \param[in] class_name             The class name
   * \param[in] someip_event_identity  Data structure containing service_id, instance_id, major_version and event_id
   * \return The string with the logger prefix
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(
      amsr::core::StringView const class_name,
      ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{" (ServiceId: "} << someip_event_identity.GetServiceId();
    prefix_builder << amsr::core::StringView{", InstanceId: "} << someip_event_identity.GetInstanceId();
    prefix_builder << amsr::core::StringView{", MajorVersion: "} << someip_event_identity.GetMajorVersion();
    prefix_builder << amsr::core::StringView{", EventId: "} << someip_event_identity.GetEventId();
    prefix_builder << ')';
    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generate a logger prefix from class name and IPC address.
   * \param[in] class_name  Class name.
   * \param[in] ipc_address_domain IPC address.
   * \param[in] ipc_address_port IPC port.
   * \return Logger prefix as string.
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(amsr::core::StringView const class_name,
                                            std::uint32_t const ipc_address_domain,
                                            std::uint32_t const ipc_address_port) noexcept {
    constexpr std::size_t kBufferSize{100u};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;

    // Max possible length of extra string, "(IPC domain: 4294967295, IPC port: 4294967295)", is 46.
    // Prevent out of buffer access (good enough check in terms of bytes count; 50 + 46 < 100).
    assert(class_name.length() < 50u);  // COV_MSR_INV_STATE_ASSERT

    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);

    prefix_builder << amsr::core::StringView{" (IPC domain: "} << ipc_address_domain;

    prefix_builder << amsr::core::StringView{", IPC port: "} << ipc_address_port;

    prefix_builder << ')';

    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a logger prefix for EventXf classes
   * \param[in] class_name              The class name
   * \param[in] someip_client_identity  Data structure containing service_id, instance_id and client id.
   * \return The string with the logger prefix
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(
      amsr::core::StringView const class_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& someip_client_identity) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{" (ServiceId: "} << someip_client_identity.GetServiceId();
    prefix_builder << amsr::core::StringView{", MajorVersion: "} << someip_client_identity.GetMajorVersion();
    prefix_builder << amsr::core::StringView{", InstanceId: "} << someip_client_identity.GetInstanceId();
    prefix_builder << amsr::core::StringView{", ClientId: "} << someip_client_identity.GetClientId();
    prefix_builder << ')';
    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a logger from a ProvidedServiceInstanceId object
   * \param[in] class_name                    The class name
   * \param[in] provided_service_instance_id  Data structure containing service_id, instance_id, major/minor version.
   * \return The string with the logger prefix
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(amsr::core::StringView const class_name,
                                            ProvidedServiceInstanceId const& provided_service_instance_id) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{" (ServiceId: "} << provided_service_instance_id.GetServiceId();
    prefix_builder << amsr::core::StringView{", InstanceId: "} << provided_service_instance_id.GetInstanceId();
    prefix_builder << amsr::core::StringView{", MajorVersion: "} << provided_service_instance_id.GetMajorVersion();
    prefix_builder << amsr::core::StringView{", MinorVersion: "} << provided_service_instance_id.GetMinorVersion();
    prefix_builder << ')';
    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a logger from a RequiredServiceInstanceId object
   * \param[in] class_name                    The class name
   * \param[in] required_service_instance_id  Data structure containing service id, major version, minor version and
   *                                          instance id.
   * \return The string with the logger prefix
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(
      amsr::core::StringView const class_name,
      ::amsr::someip_binding_core::internal::RequiredServiceInstanceId const& required_service_instance_id) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{" (ServiceId: "} << required_service_instance_id.GetServiceId();
    prefix_builder << amsr::core::StringView{", MajorVersion: "} << required_service_instance_id.GetMajorVersion();
    prefix_builder << amsr::core::StringView{", MinorVersion: "} << required_service_instance_id.GetMinorVersion();
    prefix_builder << amsr::core::StringView{", InstanceId: "} << required_service_instance_id.GetInstanceId();
    prefix_builder << ')';
    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a logger prefix for MethodXf classes
   * \param[in] class_name      The class name
   * \param[in] method_id       Method id.
   * \return The string with the logger prefix
   * \pre -
   * \context ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(amsr::core::StringView const class_name,
                                            ::amsr::someip_protocol::internal::MethodId const method_id) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{" (MethodId: "} << method_id;
    prefix_builder << ')';
    return amsr::core::String{stream.AsStringView()};
  }

  /*!
   * \brief     Generates a logger prefix for a generic key value pair.
   * \param[in] class_name      The class name.
   * \param[in] key             The key name.
   * \param[in] value           The value to be logged in hexadecimal format.
   * \return    The string with the logger prefix.
   * \pre -
   * \context   ANY
   * \reentrant False
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::core::String GetLoggerPrefix(amsr::core::StringView const class_name, amsr::core::StringView const key,
                                            uint16_t value) noexcept {
    constexpr std::size_t kBufferSize{100};
    // VECTOR NL VectorC++-V8.5.0: MD_SOMEIPBINDING_VectorC++-V8.5.0_VariableNotInitialized_WriteBeforeRead
    amsr::core::Array<char, kBufferSize> buffer;
    ::amsr::stream::stringstream::OutputStringStream stream{amsr::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream prefix_builder{stream};

    prefix_builder.Write(class_name);
    prefix_builder << &(::amsr::stream::SetShowBase) << &(::amsr::stream::Hex);
    prefix_builder << amsr::core::StringView{" ("} << key << amsr::core::StringView{": "} << value;
    prefix_builder << ')';
    return amsr::core::String{stream.AsStringView()};
  }
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_LOGGER_PREFIX_GENERATOR_H_
