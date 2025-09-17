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
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_LOGGER_PREFIX_GENERATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_LOGGER_PREFIX_GENERATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/someip_daemon_core/configuration/types/required_service_instance_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "ara/core/array.h"
#include "ara/core/span.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace logging {

/*!
 * \brief Logger prefix generator
 * \details Generates logger prefixes to log the class name and its attributes
 */
class LoggerPrefixGenerator final {
 public:
  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name                               The class name
   * \param[in] required_service_instance_deployment_id  The service interface deployment id.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName> (ServiceId: <sid>, Version: <Maj>.<Min>"
   *            Example:
   *              - "Class1 (ServiceId: 0x32, Version: 0x5.0x6). MinimumMinorVersion"
   *              - "Class2 (ServiceId: 0x32, Version: 0x5.ANY). ExactOrAnyMinorVersion"
   *
   * \pre       -
   * \context   ANY
   * \steady    FALSE
   *
   * \internal
   * - Create string streams to store service id and minor version.
   * - If the given minor version is 'ANY'
   *   - Assign minor version string stream to 'ANY'.
   * - Otherwise, update the minor version string stream to the given minor version.
   * - Create a string stream containing a string version of the version driven find behavior.
   * - Prepare final string stream containing service id, major version, minor version and the version driven
   *   find behavior in the specified format and return the string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(ara::core::StringView const class_name,
                                           configuration::types::RequiredSomeIpServiceInstanceDeploymentId const&
                                               required_service_instance_deployment_id) noexcept {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    context_id_builder << &::amsr::stream::SetShowBase;
    if (required_service_instance_deployment_id.service_interface_version.minor_version.GetMinorVersion() ==
        someip_protocol::internal::kMinorVersionAny) {
      context_id_builder.Write(ara::core::StringView{"ANY"});
    } else {
      context_id_builder.Write(
          required_service_instance_deployment_id.service_interface_version.minor_version.GetMinorVersion(),
          ::amsr::stream::Base::kHex);
    }
    ara::core::String const minor_version_string{stream.AsStringView()};
    stream.Reset();

    if (required_service_instance_deployment_id.service_interface_version.minor_version
            .GetVersionDrivenFindBehavior() ==
        configuration::ConfigurationTypesAndDefs::VersionDrivenFindBehavior::kExactOrAnyMinorVersion) {
      context_id_builder.Write(ara::core::StringView{"ExactOrAnyMinorVersion"});
    } else {
      context_id_builder.Write(ara::core::StringView{"MinimumMinorVersion"});
    }
    ara::core::String const version_driven_find_behavior_string{stream.AsStringView()};
    stream.Reset();

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << ara::core::StringView{" (ServiceId: "};
    context_id_builder << required_service_instance_deployment_id.service_interface_id;
    context_id_builder << ara::core::StringView{", Version: "};
    context_id_builder << required_service_instance_deployment_id.service_interface_version.major_version;
    context_id_builder << ara::core::StringView{"."};
    context_id_builder << minor_version_string;
    context_id_builder << ara::core::StringView{"). "};
    context_id_builder << version_driven_find_behavior_string;
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name                       The class name
   * \param[in] service_interface_deployment_id  The service interface deployment id.
   * \param[in] instance_id                      The service instance id.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName> (ServiceId: <sid>, InstanceId <iid>, Version: <Maj>.<Min>"
   *            Example:
   *              - "Class1 (ServiceId: 0x32, InstanceId: 0x1, Version: 0x5.0x6)"
   *              - "Class2 (ServiceId: 0x32, InstanceId: ALL, Version: 0x5.ANY)"
   *
   * \pre       -
   * \context   ANY
   * \steady    FALSE
   *
   * \internal
   * - Create three string streams to store service id and minor version.
   * - If the given minor version is 'ANY'
   *   - Assign minor version string stream to 'ANY'.
   * - If the given instance ID is 'ALL'
   *   - Assign instance ID string stream to 'ALL'.
   * - Otherwise, update the minor version string stream to the given minor version.
   * - Otherwise, update the instance ID string stream to the given instance ID.
   * - Convert and store minor version and instance ID string stream to string.
   * - Prepare final string stream containing service id, instance id, major version and minor version in the
   *   specified format and return the string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(
      ara::core::StringView const class_name,
      configuration::types::SomeIpServiceInterfaceDeploymentId const& service_interface_deployment_id,
      someip_protocol::internal::InstanceId const instance_id) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    // Minor Version String
    context_id_builder << &::amsr::stream::SetShowBase;
    if (service_interface_deployment_id.service_interface_version.minor_version ==
        someip_protocol::internal::kMinorVersionAny) {
      context_id_builder.Write(ara::core::StringView{"ANY"});
    } else {
      context_id_builder.Write(service_interface_deployment_id.service_interface_version.minor_version,
                               ::amsr::stream::Base::kHex);
    }
    ara::core::String const minor_version_string{stream.AsStringView()};
    stream.Reset();

    if (instance_id == someip_protocol::internal::kInstanceIdAll) {
      context_id_builder.Write(ara::core::StringView{"ALL"});
    } else {
      context_id_builder.Write(instance_id, ::amsr::stream::Base::kHex);
    }
    ara::core::String const instance_id_string{stream.AsStringView()};
    stream.Reset();

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << ara::core::StringView{" (ServiceId: "};
    context_id_builder << service_interface_deployment_id.service_interface_id;
    context_id_builder << ara::core::StringView{", InstanceId: "};
    context_id_builder << instance_id_string;
    context_id_builder << ara::core::StringView{", Version: "};
    context_id_builder << service_interface_deployment_id.service_interface_version.major_version;
    context_id_builder << ara::core::StringView{"."};
    context_id_builder << minor_version_string;
    context_id_builder << ')';
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name                               The class name
   * \param[in] required_service_instance_deployment_id  The service interface deployment id.
   * \param[in] instance_id                              The service instance id.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName> (ServiceId: <sid>, InstanceId <iid>, Version: <Maj>.<Min>"
   *            Example:
   *              - "Class1 (ServiceId: 0x32, InstanceId: 0x1, Version: 0x5.0x6). MinimumMinorVersion"
   *              - "Class2 (ServiceId: 0x32, InstanceId: ALL, Version: 0x5.ANY). ExactOrAnyMinorVersion"
   *
   * \pre       -
   * \context   ANY
   * \steady    FALSE
   *
   * \internal
   * - Create three string streams to store service id and minor version.
   * - If the given minor version is 'ANY'
   *   - Assign minor version string stream to 'ANY'.
   * - If the given instance ID is 'ALL'
   *   - Assign instance ID string stream to 'ALL'.
   * - Otherwise, update the minor version string stream to the given minor version.
   * - Otherwise, update the instance ID string stream to the given instance ID.
   * - Convert and store minor version and instance ID string stream to string.
   * - Prepare final string stream containing service id, instance id, major version and minor version in the
   *   specified format and return the string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(
      ara::core::StringView const class_name,
      configuration::types::RequiredSomeIpServiceInstanceDeploymentId const& required_service_instance_deployment_id,
      someip_protocol::internal::InstanceId const instance_id) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    // Minor Version String
    context_id_builder << &::amsr::stream::SetShowBase;
    if (required_service_instance_deployment_id.service_interface_version.minor_version.GetMinorVersion() ==
        someip_protocol::internal::kMinorVersionAny) {
      context_id_builder.Write(ara::core::StringView{"ANY"});
    } else {
      context_id_builder.Write(
          required_service_instance_deployment_id.service_interface_version.minor_version.GetMinorVersion(),
          ::amsr::stream::Base::kHex);
    }
    ara::core::String const minor_version_string{stream.AsStringView()};
    stream.Reset();

    if (required_service_instance_deployment_id.service_interface_version.minor_version
            .GetVersionDrivenFindBehavior() ==
        configuration::ConfigurationTypesAndDefs::VersionDrivenFindBehavior::kExactOrAnyMinorVersion) {
      context_id_builder.Write(ara::core::StringView{"ExactOrAnyMinorVersion"});
    } else {
      context_id_builder.Write(ara::core::StringView{"MinimumMinorVersion"});
    }
    ara::core::String const version_driven_find_behavior_string{stream.AsStringView()};
    stream.Reset();

    if (instance_id == someip_protocol::internal::kInstanceIdAll) {
      context_id_builder.Write(ara::core::StringView{"ALL"});
    } else {
      context_id_builder.Write(instance_id, ::amsr::stream::Base::kHex);
    }
    ara::core::String const instance_id_string{stream.AsStringView()};
    stream.Reset();

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << ara::core::StringView{" (ServiceId: "};
    context_id_builder << required_service_instance_deployment_id.service_interface_id;
    context_id_builder << ara::core::StringView{", InstanceId: "};
    context_id_builder << instance_id_string;
    context_id_builder << ara::core::StringView{", Version: "};
    context_id_builder << required_service_instance_deployment_id.service_interface_version.major_version;
    context_id_builder << ara::core::StringView{"."};
    context_id_builder << minor_version_string;
    context_id_builder << ara::core::StringView{"). "};
    context_id_builder << version_driven_find_behavior_string;
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name                       The class name
   * \param[in] service_interface_deployment_id  The service interface deployment id.
   * \param[in] instance_id                      The service instance id.
   * \param[in] address                          The IP address.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName> (ServiceId: <sid>, InstanceId: <iid>, Version: <Maj>.<Min>, Address: <add>"
   *            Example:
   *              - "Class1 (ServiceId: 0x32, InstanceId: 0x1, Version: 0x5.0x6, Local Address: 192.168.0.1)"
   *              - "Class2 (ServiceId: 0x32, InstanceId: ALL, Version: 0x5.ANY, Local Address: 192.168.0.20)"
   *
   * \pre       -
   * \context   ANY
   * \steady    FALSE
   *
   * \internal
   * - Create string streams to store service id, minor version, and address.
   * - If the given minor version is 'ANY'
   *   - Assign minor version string stream to 'ANY'.
   * - Otherwise, update the minor version string stream to the given minor version.
   * - Convert and store minor version string stream to string.
   * - If the given instance ID is 'ALL'
   *   - Assign instance ID string stream to 'ALL'.
   * - Otherwise, update the instance ID string stream to the given instance ID.
   * - Convert and store instance ID and address string stream to string.
   * - Prepare final string stream containing service id, instance id, major version, minor version, and address in
   * the specified format and return the string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(
      ara::core::StringView const class_name,
      configuration::types::SomeIpServiceInterfaceDeploymentId const& service_interface_deployment_id,
      someip_protocol::internal::InstanceId const instance_id, someip_protocol::internal::IpAddress const address) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    // Minor Version String
    context_id_builder << &::amsr::stream::SetShowBase;
    if (service_interface_deployment_id.service_interface_version.minor_version ==
        someip_protocol::internal::kMinorVersionAny) {
      context_id_builder.Write(ara::core::StringView{"ANY"});
    } else {
      context_id_builder.Write(service_interface_deployment_id.service_interface_version.minor_version,
                               ::amsr::stream::Base::kHex);
    }
    ara::core::String const minor_version_string{stream.AsStringView()};
    stream.Reset();

    if (instance_id == someip_protocol::internal::kInstanceIdAll) {
      context_id_builder.Write(ara::core::StringView{"ALL"});
    } else {
      context_id_builder.Write(instance_id, ::amsr::stream::Base::kHex);
    }
    ara::core::String const instance_id_string{stream.AsStringView()};
    stream.Reset();

    context_id_builder.Write(ara::core::StringView{address});
    ara::core::String const address_string{stream.AsStringView()};
    stream.Reset();

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << ara::core::StringView{" (ServiceId: "};
    context_id_builder << service_interface_deployment_id.service_interface_id;
    context_id_builder << ara::core::StringView{", InstanceId: "};
    context_id_builder << instance_id_string;
    context_id_builder << ara::core::StringView{", Version: "};
    context_id_builder << service_interface_deployment_id.service_interface_version.major_version;
    context_id_builder << ara::core::StringView{"."};
    context_id_builder << minor_version_string;
    context_id_builder << ara::core::StringView{", Local Address: "};
    context_id_builder << address_string;
    context_id_builder << ')';
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name      The class name
   * \param[in] interface_name  The interface name.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName>(Interface name)"
   *            Example:
   *              - "Class1(Test Interface)"
   *
   * \pre       -
   * \context   ANY
   * \steady    FALSE
   *
   * \internal
   * - Create string stream to store class name and interface name.
   * - Prepare final string stream containing class name and interface name in the
   *   specified format and return the string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(ara::core::StringView const class_name,
                                           ara::core::StringView const interface_name) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << '(';
    context_id_builder << interface_name;
    context_id_builder << ')';
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name                       The class name
   * \param[in] service_interface_deployment_id  The service interface deployment id.
   * \param[in] instance_id                      The service instance id.
   * \param[in] client_id                        The service client id.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName> (ServiceId: <sid>, InstanceId <iid>, Version: <Maj>.<Min>, ClientId: <cid>"
   *            Example:
   *              - "Class1 (ServiceId: 0x32, InstanceId: 0x1, Version: 0x5.0x6, ClientId: 0x01)"
   *              - "Class2 (ServiceId: 0x32, InstanceId: ALL, Version: 0x5.ANY, ClientId: 0x05)"
   *
   * \pre       -
   * \context   ANY
   * \steady    FALSE
   *
   * \internal
   * - Create string streams to store service id, minor version, and client id.
   * - If the given minor version is 'ANY'
   *   - Assign minor version string stream to 'ANY'.
   * - Otherwise, update the minor version string stream to the given minor version.
   * - Convert and store minor version string stream to string.
   * - If the given instance ID is 'ALL'
   *   - Assign instance ID string stream to 'ALL'.
   * - Otherwise, update the instance ID string stream to the given instance ID.
   * - Convert and store instance ID string stream to string.
   * - Update the client ID string stream to the given client ID.
   * - Convert and store client ID string stream to string.
   * - Prepare final string stream containing service id, instance id, major version, minor version, and client id in
   * the specified format and return the string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(
      ara::core::StringView const class_name,
      configuration::types::SomeIpServiceInterfaceDeploymentId const& service_interface_deployment_id,
      someip_protocol::internal::InstanceId const instance_id, someip_protocol::internal::ClientId const client_id) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    // Minor Version String
    context_id_builder << &::amsr::stream::SetShowBase;
    if (service_interface_deployment_id.service_interface_version.minor_version ==
        someip_protocol::internal::kMinorVersionAny) {
      context_id_builder.Write(ara::core::StringView{"ANY"});
    } else {
      context_id_builder.Write(service_interface_deployment_id.service_interface_version.minor_version,
                               ::amsr::stream::Base::kHex);
    }
    ara::core::String const minor_version_string{stream.AsStringView()};
    stream.Reset();

    // Instance ID String
    if (instance_id == someip_protocol::internal::kInstanceIdAll) {
      context_id_builder.Write(ara::core::StringView{"ALL"});
    } else {
      context_id_builder.Write(instance_id, ::amsr::stream::Base::kHex);
    }
    ara::core::String const instance_id_string{stream.AsStringView()};
    stream.Reset();

    // Client ID String
    context_id_builder.Write(client_id, ::amsr::stream::Base::kHex);
    ara::core::String const client_id_string{stream.AsStringView()};
    stream.Reset();

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << ara::core::StringView{" (ServiceId: "};
    context_id_builder << service_interface_deployment_id.service_interface_id;
    context_id_builder << ara::core::StringView{", InstanceId: "};
    context_id_builder << instance_id_string;
    context_id_builder << ara::core::StringView{", Version: "};
    context_id_builder << service_interface_deployment_id.service_interface_version.major_version;
    context_id_builder << ara::core::StringView{"."};
    context_id_builder << minor_version_string;
    context_id_builder << ara::core::StringView{", ClientId: "};
    context_id_builder << client_id_string;
    context_id_builder << ')';
    return ara::core::String{stream.AsStringView()};
  }

  /*!
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param[in] class_name              The class name
   * \param[in] application_identifier  The Unique application identifier.
   * \return    A string representing the logger context identifier for given SOME/IP service parameters.
   *            Format: "<ClassName> (App Id: <aid>)"
   *            Example:
   *              - "Class1 (App Id: 0x5)"
   *
   * \pre       -
   * \context   Application
   * \steady    FALSE
   *
   * \internal
   * - Create string stream to store class name and application Id.
   * - Prepare final string stream containing class name and application Id in the specified format and return the
   *   string value of the stream.
   * \endinternal
   */
  static ara::core::String GetLoggerPrefix(ara::core::StringView const class_name,
                                           amsr::ipc::Credentials const& application_identifier) {
    constexpr std::size_t kBufferSize{200};
    ara::core::Array<char, kBufferSize> buffer{};
    ::amsr::stream::stringstream::OutputStringStream stream{ara::core::Span<char>{buffer}};
    ::amsr::stream::StatefulOutputStream context_id_builder{stream};

    context_id_builder << &::amsr::stream::SetShowBase << &::amsr::stream::Hex;
    context_id_builder << class_name;
    context_id_builder << ara::core::StringView{" (App Id: "};
    context_id_builder << application_identifier;
    context_id_builder << ')';
    return ara::core::String{stream.AsStringView()};
  }
};

}  // namespace logging
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_LOGGER_PREFIX_GENERATOR_H_
