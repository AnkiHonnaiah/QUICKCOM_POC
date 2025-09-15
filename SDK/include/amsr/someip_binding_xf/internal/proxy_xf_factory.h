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
 *        \brief  Factory for ProxyXf elements.
 *        \unit   SomeIpBinding::SomeIpBindingXf::ProxyXfFactory
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_PROXY_XF_FACTORY_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_PROXY_XF_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include "amsr/core/optional.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding/internal/configuration/method_config.h"
#include "amsr/someip_binding/internal/configuration/notifier_config.h"
#include "amsr/someip_binding/internal/configuration/operation_config.h"
#include "amsr/someip_binding/internal/someipbinding_proxy_event_xf_initializer.h"
#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_core/internal/logging/string_stream.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf.h"
#include "amsr/someip_binding_xf/internal/methods/proxy_fire_and_forget_method_xf.h"
#include "amsr/someip_binding_xf/internal/methods/proxy_method_xf.h"
#include "amsr/someip_binding_xf/internal/ser_deser/event_deserializer_interface.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_serializer_wrapper.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_deserializer_wrapper.h"
#include "amsr/someip_binding_xf/internal/ser_deser/s2s_event_deserializer_wrapper.h"
#include "amsr/someip_binding_xf/internal/ser_deser/someip_event_deserializer_wrapper.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {

/*!
 * \brief Factory class for all ProxyXf members (Events, Fields, Methods)
 */
class ProxyXfFactory final {
 public:
  /*!
   * \brief Map used to contain events E2E protection Props.
   */
  using E2EPropsMap =
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>>;

  /*!
   * \brief Application Error map type definition.
   */
  using ApplicationErrorMap = std::map<amsr::core::ErrorDomain::IdType,
                                       ::amsr::someip_binding_xf::internal::methods::ApplicationErrorDomain> const;

  /*!
   * \tparam SampleType Event data type
   * \brief Xf class for Proxy Event
   */
  template <typename SampleType>
  using EventXf = ::amsr::someip_binding_xf::internal::events::ProxyEventXf<SampleType>;

  /*!
   * \tparam Output Method Output type
   * \tparam ArgsT Method input arguments types
   * \brief Xf class for Proxy Method
   */
  template <typename Output, typename... ArgsT>
  using MethodXf = ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf<Output, ArgsT...>;

  /*!
   * \tparam ArgsT Method input arguments types
   * \brief Xf class for Proxy Fire and Forget Method
   */
  template <typename... ArgsT>
  using FireAndForgetMethodXf = ::amsr::someip_binding_xf::internal::methods::ProxyFireAndForgetMethodXf<ArgsT...>;

  /*!
   * \tparam FieldType Field data type
   * \brief Xf class for Proxy Field Setter
   */
  template <typename FieldType>
  using FieldSetterXf = ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf<FieldType, FieldType>;

  /*!
   * \tparam FieldType Field data type
   * \brief Xf class for Proxy Field Getter
   */
  template <typename FieldType>
  using FieldGetterXf = ::amsr::someip_binding_xf::internal::methods::ProxyMethodXf<FieldType>;

  /*!
   * \tparam FieldType Field data type
   * \brief Xf class for Proxy Field Notifier
   */
  template <typename FieldType>
  using FieldNotifierXf = ::amsr::someip_binding_xf::internal::events::ProxyEventXf<FieldType>;

  /*!
   * \brief  Create Transformation layer class for the given event.
   *
   * \tparam SampleType                  Event Sample Type.
   * \tparam PayloadDeserializer         Event Deserializer class.
   *
   * \param[in] config_wrapper           Configuration of SOME/IP Binding model.
   * \param[in] event_name               The modelled shortname for the event to be created.
   * \param[in] proxy_identity           ID of the proxy (service ID, major version, instance ID, client ID)
   * \param[in] local_client             The LocalClient.
   * \param[in] e2e_props_map            E2E Event properties for all E2E Events (can be empty if no E2E events are
   *                                     configured).
   * \param[in] e2e_profile_config_map   Reference to the E2EProfileConfig map.
   *
   * \return UniquePtr with the created EventXf.
   *
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  template <typename SampleType, typename PayloadDeserializer>
  static auto CreateEventXf(
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::core::StringView const& event_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_binding_core::internal::LocalClient const& local_client, E2EPropsMap const& e2e_props_map,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const&
          e2e_profile_config_map) noexcept -> std::unique_ptr<EventXf<SampleType>> {
    // Find Event Configuration
    ::amsr::someip_binding::internal::configuration::Shortname const event_shortname{event_name.data()};

    ::amsr::someip_binding::internal::configuration::EventConfig const& event_config{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        config_wrapper.GetEventConfig(event_shortname)};

    // Build Event ID
    someip_binding_core::internal::SomeIpEventIdentity const event_identity{
        proxy_identity.GetServiceId(), proxy_identity.GetMajorVersion(), proxy_identity.GetInstanceId(),
        event_config.GetEventId()};

    // Find selected Serialization protocol (SOME/IP or S2S)
    ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization{
        event_config.GetSerialization()};

    // Create Deserializer
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<ser_deser::EventDeserializerInterface<SampleType>> deserializer_ptr{};
    if (serialization == ::amsr::someip_binding::internal::configuration::EventConfig::Serialization::signal_based) {
      deserializer_ptr = std::make_unique<::amsr::someip_binding_xf::internal::ser_deser::S2sEventDeserializerWrapper<
          SampleType, PayloadDeserializer>>();
    } else {
      deserializer_ptr =
          std::make_unique<::amsr::someip_binding_xf::internal::ser_deser::SomeIpEventDeserializerWrapper<
              SampleType, PayloadDeserializer>>();
    }

    // Collect E2E Parameterization data
    amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
        e2e_parametrization_data{
            ::amsr::someip_binding::internal::SomeIpBindingProxyEventXfInitializer::
                GetProxyEventXfE2EParametrizationData(event_config, e2e_props_map, e2e_profile_config_map)};

    // In case of S2S events, check if extension flag is enabled
    amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionRx> const
        pdu_header_extn{event_config.GetPduHeaderExtensionRxFlag().value_or(static_cast<bool>(false))};

    // Get ClientEvent
    amsr::core::Result<std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent>> client_event_result{
        local_client.GetEvent(event_config.GetEventId())};

    if (!client_event_result.HasValue()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"ProxyXfFactory"},
              [&event_config](amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Invalid Configuration. Requested event ";
                // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
                s << std::to_string(static_cast<std::int16_t>(ara::log::HexFormat(event_config.GetEventId()).value));
                s << " is not available.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<EventXf<SampleType>> proxy_event_xf{std::make_unique<EventXf<SampleType>>(
        event_identity, std::move(client_event_result.Value()), std::move(deserializer_ptr),
        std::move(e2e_parametrization_data), serialization, pdu_header_extn)};

    return std::move(proxy_event_xf);
  }

  /*!
   * \brief  Create Transformation layer class for the given field notifier.
   *
   * \tparam FieldType                   Field notifier Sample Type.
   * \tparam PayloadDeserializer         Field notifier Deserializer class.
   *
   * \param[in] config_wrapper           Configuration of SOME/IP Binding model.
   * \param[in] field_name               The modelled shortname for the field to be created.
   * \param[in] proxy_identity           ID of the proxy (service ID, major version, instance ID, client ID)
   * \param[in] local_client             The LocalClient.
   * \param[in] e2e_props_map            E2E Event properties for all E2E Events (can be empty if no E2E events are
   *                                     configured).
   * \param[in] e2e_profile_config_map   Reference to the E2EProfileConfig map.
   *
   * \return UniquePtr with the created EventXf.
   *
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  template <typename FieldType, typename PayloadDeserializer>
  static auto CreateFieldNotifierXf(
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::core::StringView const& field_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_binding_core::internal::LocalClient const& local_client, E2EPropsMap const& e2e_props_map,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const&
          e2e_profile_config_map) noexcept -> std::unique_ptr<FieldNotifierXf<FieldType>> {
    using DeserializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::SomeIpEventDeserializerWrapper<FieldType, PayloadDeserializer>;

    // Find Field notifier Configuration
    ::amsr::someip_binding::internal::configuration::Shortname const field_shortname{field_name.data()};

    ::amsr::someip_binding::internal::configuration::NotifierConfig const& field_notifier_config{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        config_wrapper.GetNotifierConfig(field_shortname)};

    // Build Event ID
    someip_binding_core::internal::SomeIpEventIdentity const event_identity{
        proxy_identity.GetServiceId(), proxy_identity.GetMajorVersion(), proxy_identity.GetInstanceId(),
        field_notifier_config.GetNotifierId()};

    // Create Deserializer
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<ser_deser::EventDeserializerInterface<FieldType>> deserializer_ptr{
        std::make_unique<DeserializerWrapper>()};

    // Collect E2E Parameterization data
    amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
        e2e_parametrization_data{
            ::amsr::someip_binding::internal::SomeIpBindingProxyEventXfInitializer::
                GetProxyEventXfE2EParametrizationData(field_notifier_config, e2e_props_map, e2e_profile_config_map)};

    // Fields do not support S2S
    ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization{
        ::amsr::someip_binding::internal::configuration::EventConfig::Serialization::someip};
    amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionRx> const
        pdu_header_extn{};

    // Get ClientEvent
    amsr::core::Result<std::unique_ptr<amsr::someip_binding_core::internal::events::ClientEvent>> client_event_result{
        local_client.GetEvent(field_notifier_config.GetNotifierId())};

    if (!client_event_result.HasValue()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"ProxyXfFactory"},
              [&field_notifier_config](amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Invalid Configuration. Requested event ";
                // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
                s << std::to_string(
                    static_cast<std::int16_t>(ara::log::HexFormat(field_notifier_config.GetNotifierId()).value));
                s << " is not available.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<FieldNotifierXf<FieldType>> proxy_event_xf{std::make_unique<FieldNotifierXf<FieldType>>(
        event_identity, std::move(client_event_result.Value()), std::move(deserializer_ptr),
        std::move(e2e_parametrization_data), serialization, pdu_header_extn)};

    return std::move(proxy_event_xf);
  }

  /*!
   * \brief  Create Transformation layer class for the given method.
   *
   * \tparam ResponseDeserializer        Response Deserializer class.
   * \tparam OutputType                  Method response data Type.
   * \tparam RequestSerializer           Request Serializer class.
   * \tparam ArgsT                       Method request data Types.
   *
   * \param[in] config_wrapper           Configuration of SOME/IP Binding model.
   * \param[in] method_name              The modelled shortname for the event to be created.
   * \param[in] proxy_identity           ID of the proxy (service ID, major version, instance ID, client ID)
   * \param[in] local_client             The LocalClient.
   * \param[in] application_error_map    application errors that are configured for this method.
   *
   * \return UniquePtr with the created MethodXf.
   *
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  template <typename ResponseDeserializer, typename OutputType, typename RequestSerializer, typename... ArgsT>
  static auto CreateMethodXf(
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::core::StringView const& method_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_binding_core::internal::LocalClient& local_client,
      ProxyXfFactory::ApplicationErrorMap const& application_error_map) noexcept
      -> std::unique_ptr<MethodXf<OutputType, ArgsT...>> {
    using ResponseDeserializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodResponseDeserializerWrapper<OutputType,
                                                                                          ResponseDeserializer>;
    using RequestSerializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodRequestSerializerWrapper<RequestSerializer, ArgsT...>;
    using TxBufferAllocator = ::amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator;

    // Find method Configuration
    ::amsr::someip_binding::internal::configuration::Shortname const method_shortname{method_name.data()};

    ::amsr::someip_binding::internal::configuration::MethodConfig const& method_config{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        config_wrapper.GetMethodConfig(method_shortname)};

    // Create Request Serializer / Response Deserializer
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<RequestSerializerWrapper> request_serializer{std::make_unique<RequestSerializerWrapper>()};
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::shared_ptr<ResponseDeserializerWrapper> response_deserializer_ptr{
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::make_shared<ResponseDeserializerWrapper>()};

    // Create Buffer Allocator for method request serialization
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    std::shared_ptr<TxBufferAllocator> tx_buffer_allocator{std::make_shared<TxBufferAllocator>()};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<MethodXf<OutputType, ArgsT...>> proxy_method_xf{std::make_unique<MethodXf<OutputType, ArgsT...>>(
        local_client, proxy_identity.GetServiceId(), proxy_identity.GetInstanceId(), proxy_identity.GetMajorVersion(),
        proxy_identity.GetClientId(), method_config.GetMethodId(), method_name, application_error_map,
        std::move(request_serializer), response_deserializer_ptr, tx_buffer_allocator)};

    return std::move(proxy_method_xf);
  }

  /*!
   * \brief  Create Transformation layer class for the given fire and forget method.
   *
   * \tparam RequestSerializer           Request Serializer class.
   * \tparam ArgsT                       Method request data Types.
   *
   * \param[in] config_wrapper           Configuration of SOME/IP Binding model.
   * \param[in] method_name              The modelled shortname for the event to be created.
   * \param[in] proxy_identity           ID of the proxy (service ID, major version, instance ID, client ID)
   * \param[in] local_client             The LocalClient.
   *
   * \return UniquePtr with the created ProxyFireAndForgetMethodXf.
   *
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  template <typename RequestSerializer, typename... ArgsT>
  static auto CreateFireAndForgetMethodXf(
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::core::StringView const& method_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_binding_core::internal::LocalClient& local_client) noexcept
      -> std::unique_ptr<FireAndForgetMethodXf<ArgsT...>> {
    using RequestSerializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodRequestSerializerWrapper<RequestSerializer, ArgsT...>;
    using TxBufferAllocator = ::amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator;

    // Find method configuration
    ::amsr::someip_binding::internal::configuration::Shortname const method_shortname{method_name.data()};

    ::amsr::someip_binding::internal::configuration::MethodConfig const& method_config{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        config_wrapper.GetMethodConfig(method_shortname)};

    // Create Request Serializer
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<RequestSerializerWrapper> request_serializer{std::make_unique<RequestSerializerWrapper>()};

    // Create Buffer Allocator for method request serialization
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    std::shared_ptr<TxBufferAllocator> tx_buffer_allocator{std::make_shared<TxBufferAllocator>()};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<FireAndForgetMethodXf<ArgsT...>> proxy_fnf_method_xf{
        std::make_unique<FireAndForgetMethodXf<ArgsT...>>(
            local_client, proxy_identity.GetServiceId(), proxy_identity.GetInstanceId(),
            proxy_identity.GetMajorVersion(), proxy_identity.GetClientId(), method_config.GetMethodId(), method_name,
            std::move(request_serializer), tx_buffer_allocator)};

    return std::move(proxy_fnf_method_xf);
  }

  /*!
   * \brief Dummy serializer for Field Getter Request (no args)
   */
  class FieldGetterSerializer {
   public:
    /*!
     * \brief Return the payload size
     * \return 0 (no args for Field getter)
     */
    static std::size_t GetRequiredBufferSize() noexcept { return 0; }

    /*!
     * \brief Dummy serialize API
     * \param[in] writer  Never used. Defined to respect the API.
     */
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPBINDING_AutosarC++17_10-M0.1.8_Void_function_no_side_effect
    static void Serialize(::amsr::someip_protocol::internal::serialization::Writer const& writer) noexcept {
      // do nothing
      static_cast<void>(writer);
    }
  };

  /*!
   * \brief  Create Transformation layer class for the given field getter.
   *
   * \tparam FieldType                   Field data Type.
   * \tparam ResponseDeserializer        Response Deserializer class.
   *
   * \param[in] config_wrapper           Configuration of SOME/IP Binding model.
   * \param[in] method_name              The modelled shortname for the event to be created.
   * \param[in] proxy_identity           ID of the proxy (service ID, major version, instance ID, client ID)
   * \param[in] local_client             The LocalClient.
   *
   * \return UniquePtr with the created MethodXf.
   *
   * \pre -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   */
  template <typename FieldType, typename ResponseDeserializer>
  static auto CreateFieldGetterXf(
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::core::StringView const& method_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_binding_core::internal::LocalClient& local_client) noexcept
      -> std::unique_ptr<FieldGetterXf<FieldType>> {
    using ResponseDeserializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodResponseDeserializerWrapper<FieldType,
                                                                                          ResponseDeserializer>;

    using RequestSerializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodRequestSerializerWrapper<FieldGetterSerializer>;
    using TxBufferAllocator = ::amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator;

    // Find field getter configuration
    ::amsr::someip_binding::internal::configuration::Shortname const field_shortname{method_name.data()};

    ::amsr::someip_binding::internal::configuration::OperationConfig const& field_getter_config{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        config_wrapper.GetGetterConfig(field_shortname)};

    // Fields do not support application errors
    ProxyXfFactory::ApplicationErrorMap const application_error_map{};

    // Create Response Deserializer / Dummy Request Serializer
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<RequestSerializerWrapper> request_serializer{std::make_unique<RequestSerializerWrapper>()};
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::shared_ptr<ResponseDeserializerWrapper> response_deserializer_ptr{
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::make_shared<ResponseDeserializerWrapper>()};

    // Create Buffer Allocator for method request serialization
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    std::shared_ptr<TxBufferAllocator> tx_buffer_allocator{std::make_shared<TxBufferAllocator>()};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<FieldGetterXf<FieldType>> proxy_method_xf{std::make_unique<FieldGetterXf<FieldType>>(
        local_client, proxy_identity.GetServiceId(), proxy_identity.GetInstanceId(), proxy_identity.GetMajorVersion(),
        proxy_identity.GetClientId(), field_getter_config.GetOperationId(), method_name, application_error_map,
        std::move(request_serializer), response_deserializer_ptr, tx_buffer_allocator)};

    return std::move(proxy_method_xf);
  }

  /*!
   * \brief  Create Transformation layer class for the given field setter.
   *
   * \tparam FieldType                   Method response data Type.
   * \tparam ResponseDeserializer        Response Deserializer class.
   * \tparam RequestSerializer           Request Serializer class.
   *
   * \param[in] config_wrapper           Configuration of SOME/IP Binding model.
   * \param[in] method_name              The modelled shortname for the event to be created.
   * \param[in] proxy_identity           ID of the proxy (service ID, major version, instance ID, client ID)
   * \param[in] local_client             The LocalClient.
   *
   * \return UniquePtr with the created MethodXf.
   *
   * \pre -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   */
  template <typename FieldType, typename ResponseDeserializer, typename RequestSerializer>
  static auto CreateFieldSetterXf(
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::core::StringView const& method_name,
      ::amsr::someip_binding_core::internal::ProxyBindingIdentity const& proxy_identity,
      ::amsr::someip_binding_core::internal::LocalClient& local_client) noexcept
      -> std::unique_ptr<FieldSetterXf<FieldType>> {
    using ResponseDeserializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodResponseDeserializerWrapper<FieldType,
                                                                                          ResponseDeserializer>;
    using RequestSerializerWrapper =
        ::amsr::someip_binding_xf::internal::ser_deser::MethodRequestSerializerWrapper<RequestSerializer, FieldType>;
    using TxBufferAllocator = ::amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator;

    // Find field setter configuration
    ::amsr::someip_binding::internal::configuration::Shortname const field_shortname{method_name.data()};

    ::amsr::someip_binding::internal::configuration::OperationConfig const& field_getter_config{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        config_wrapper.GetSetterConfig(field_shortname)};

    // Fields do not support application errors
    ProxyXfFactory::ApplicationErrorMap const application_error_map{};

    // Create Request Serializer / Response Deserializer
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<RequestSerializerWrapper> request_serializer{std::make_unique<RequestSerializerWrapper>()};
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::shared_ptr<ResponseDeserializerWrapper> response_deserializer_ptr{
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        std::make_shared<ResponseDeserializerWrapper>()};

    // Create Buffer Allocator for method request serialization
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    std::shared_ptr<TxBufferAllocator> tx_buffer_allocator{std::make_shared<TxBufferAllocator>()};

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
    std::unique_ptr<FieldSetterXf<FieldType>> proxy_method_xf{std::make_unique<FieldSetterXf<FieldType>>(
        local_client, proxy_identity.GetServiceId(), proxy_identity.GetInstanceId(), proxy_identity.GetMajorVersion(),
        proxy_identity.GetClientId(), field_getter_config.GetOperationId(), method_name, application_error_map,
        std::move(request_serializer), response_deserializer_ptr, tx_buffer_allocator)};

    return std::move(proxy_method_xf);
  }
};

}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_PROXY_XF_FACTORY_H_
