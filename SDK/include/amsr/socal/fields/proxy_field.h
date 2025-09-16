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
/**        \file  proxy_field.h
 *        \brief  Proxy Field
 *
 *      \details  ProxyField templated class represents a field of a proxy. For proxy instance, ProxyField shall be
 *                instantiated for all fields defined in this Proxy. Each Proxy/Field type combination represents a
 *                different ProxyField class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_FIELDS_PROXY_FIELD_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_FIELDS_PROXY_FIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <type_traits>

#include "amsr/socal/internal/fields/proxy_field_config.h"
#include "amsr/socal/methods/proxy_method.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace socal {
namespace fields {

// clang-format off
/*!
 * \brief    Generic template class which can be used to create fields for a proxy.
 * \details  The following APIs are not considered to be threadsafe against each other, or
 *           against any other API in this instance:
 *           -  Subscribe(),
 *           -  Unsubscribe(),
 *           -  GetSubscriptionState(),
 *           -  SetSubscriptionStateChangeHandler(),
 *           -  UnsetSubscriptionStateChangeHandler(),
 *           -  SetReceiveHandler(),
 *           -  UnsetReceiveHandler()
 * \tparam   FieldConfig Is a model of the configuration in the arxml with info about
 *           hasSetter, hasGetter and hasNotifier.
 *
 * Example usage:
 *
 * """
 * using MyFieldConfig = ProxyFieldConfig<NotifierConfig, GetterConfig, SetterConfig, ...>;
 * """
 *
 * \trace SPEC-4980391
 * \unit Socal::Proxy::ProxyField
 */
// clang-format on
template <typename FieldConfig>
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class ProxyField final : public FieldConfig::NotificationType {
  /*!
   * \brief Type alias for future executor class type.
   */
  using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

 public:
  /*!
   * \brief Type-alias for the data type of this field.
   * \vpublic
   */
  using FieldType = typename FieldConfig::FieldType;

  /*!
   * \brief Type alias for proxy event backend.
   */
  using ProxyEventBackendPtr = typename FieldConfig::ProxyEventBackendPtrType;

  /*!
   * \brief The configuration template-class will take care of this and either inserts a concrete event or a null-type,
   *        if hasNotifier is set to false.
   */
  using Notification = typename FieldConfig::NotificationType;

  /*!
   * \brief Constructor.
   * \param[in] proxy_event_backend   A pointer to the proxy event backend. The given pointer must not be null and must
   *                                  be valid until the current object's lifetime.
   * \param[in] future_executor       Executor of the callbacks registered in the context of Future::then.
   * \param[in] proxy_setter_backend  The proxy method backend interface for the setter.
   * \param[in] proxy_getter_backend  The proxy method backend interface for the getter.
   * \param[in] service_name          The service name for this proxy field.
   * \param[in] field_name            The field name of this proxy field.
   * \param[in] instance_id           The instance identifier to be used with this proxy field.
   * \pre      -
   * \context  App
   * \vprivate Vector component internal API.
   * \steady FALSE
   * \spec
   *   requires proxy_event_backend != nullptr;
   *   requires future_executor != nullptr;
   * \endspec
   */
  ProxyField(ProxyEventBackendPtr proxy_event_backend, ExecutorInterfacePtr future_executor,
             typename FieldConfig::SetterBackendType proxy_setter_backend,
             typename FieldConfig::GetterBackendType proxy_getter_backend, std::string const& service_name,
             std::string const& field_name, ::ara::com::InstanceIdentifier const& instance_id) noexcept
      : Notification{proxy_event_backend, service_name, field_name, instance_id},
        get_{proxy_getter_backend, future_executor},    // VCA_SOCAL_VALID_PROXY_GETTER_AND_SETTER
        set_{proxy_setter_backend, future_executor} {}  // VCA_SOCAL_VALID_PROXY_GETTER_AND_SETTER

  /*!
   * \brief The getter allows to request the actual value of the service provider.
   * \return The current field value wrapped in a future object. In polling runtime processing mode, it is not allowed
   *         to register a Future::then() callback on the returned future object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980387, SPEC-4980388
   */
  template <typename Config = FieldConfig>
  auto Get() noexcept -> typename std::enable_if<Config::HasGetter, ::ara::core::Future<FieldType>>::type {
    return get_();
  }

  /*!
   * \brief Request to modify the field value. It's up to the service provider to accept the modification request.
   * \param[in] value  The value for a property.
   * \return The current field value wrapped in a future object. In polling runtime processing mode, it is not allowed
   *         to register a Future::then() callback on the returned future object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980389, SPEC-4980390
   */
  template <typename Config = FieldConfig>
  auto Set(FieldType const& value) noexcept ->
      typename std::enable_if<Config::HasSetter, ::ara::core::Future<FieldType>>::type {
    // VCA_SOCAL_CALLING_STL_APIS
    return set_(value);
  }

 private:
  /*!
   * \brief Getter method of this field as an object.
   */
  typename FieldConfig::GetterMethodType get_;

  /*!
   * \brief Setter method of this field as an object.
   */
  typename FieldConfig::SetterMethodType set_;
};

}  // namespace fields
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_FIELDS_PROXY_FIELD_H_
