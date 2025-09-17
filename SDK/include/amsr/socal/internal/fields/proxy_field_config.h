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
/**        \file  proxy_field_config.h
 *        \brief  Configuration for ProxyField.
 *
 *      \details  Configuration required to template ProxyField class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_PROXY_FIELD_CONFIG_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_PROXY_FIELD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <type_traits>

#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "amsr/socal/internal/methods/proxy_method_backend_interface.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace socal {
namespace internal {
namespace fields {

/*!
 * \brief This class contains the parameters' definitions for ProxyFieldConfig.
 *
 * \unit Socal::Proxy::ProxyField
 */
class ProxyFieldParams final {
 public:
  /*!
   * \brief Parametrize hasNotifier option of a field.
   * \note  This is a handy way to extend the template-parameter-list. If Option is set to true, the user must ensure to
   *        provide a struct like this:
   *
   *        """ HasNotifier<true, ProxyEvent<std::uint32_t>> """
   *
   *        If no notifier is necessary, the user is possible to set the type like this:
   *
   *        """ HasNotifier<false> """
   *
   *        Without the need to give a second template-parameter, because it defaults to void.
   * \tparam Active  Set this template-parameter to true, if this value is set to true in the arxml - false if not.
   * \tparam Event   If the notifier is set to true, provide an Event structure. If no notification is active, it
   *                 defaults to a default template-parameter of void.
   */
  template <bool Active, typename Event = typename std::enable_if<!Active, void>::type>
  struct HasNotifier {
    /*!
     * \brief This enum is used to store parameters set in the template parameters.
     */
    enum {
      /*!
       * \brief Contains the value of hasNotifier from the generator model.
       */
      active = Active
    };
    /*!
     * \brief Either this is the concrete Event type or void, if hasNotifier is set to false.
     */
    using type = Event;
  };

  /*!
   * \brief Parametrize hasGetter option of a field.
   * \note  This is a handy way to extend the template-parameter-list. If Option is set to true, the user must ensure to
   *        provide a struct like this:
   *
   *        """ HasGetter<true, ProxyMethod<ConcreteMethod, ...> """
   *
   *        If no getter is necessary, it is sufficient to use:
   *
   *        """ HasGetter<false> """
   *
   *        Without the need to give a second template-parameter, because it defaults to void.
   * \tparam Active     Set this template-parameter to true, if this value is set to true in the arxml - false if not.
   * \tparam GetMethod  If the getter is set to true, provide a method structure. If getter isn't active, it defaults to
   *                    a default template-parameter of "void".
   */
  template <bool Active, typename GetMethod = typename std::enable_if<!Active, void>::type>
  struct HasGetter {
    /*!
     * \brief This enum is used to store parameters set in the template parameters.
     */
    enum {
      /*!
       * \brief Contains the value of hasGetter from the generator model.
       */
      active = Active
    };

    /*!
     * \brief Type alias to forward the generated type for the get method. Either this is a concrete Method type
     *        definition or void, if hasGetter is set to false in the generator model.
     */
    using type = GetMethod;
  };

  /*!
   * \brief Parametrize hasSetter option of a field.
   * \note  This is a handy way to extend the template-parameter-list. If Option is set to true, the user must ensure to
   *        provide a struct like this:
   *
   *        """ HasSetter<true, ProxyMethod<ConcreteMethod, ...> """
   *
   *        If no setter is necessary, it is sufficient to use:
   *
   *        """ HasSetter<false> """
   *
   *        Without the need to give a second template-parameter, because it defaults to void.
   * \tparam Active     Set this template-parameter to true, if this value is set to true in the arxml - false if not.
   * \tparam SetMethod  If the setter is set to true, provide a method structure. If getter isn't active, it defaults to
   *                    a default template-parameter of "void".
   */
  template <bool Active, typename SetMethod = typename std::enable_if<!Active, void>::type>
  struct HasSetter {
    /*!
     * \brief This enum is used to store parameters set in the template parameters.
     */
    enum {
      /*!
       * \brief Contains the value of hasSetter from the generator model.
       */
      active = Active
    };

    /*!
     * \brief Type alias to forward the generated type for the set method. Either this is a concrete Method type
     *        definition or void, if hasSetter is set to false in the generator model.
     */
    using type = SetMethod;
  };
};

// clang-format off
/*!
 * \brief Parameter-set for one field
 *
 * \tparam Notification The configured notification type (contains info if hasNotifier is set to
 *          true / false)
 * \tparam Getter The configured getter type (contains info if hasGetter is set to
 *          true / false)
 * \tparam Setter The configured setter type (contains info if hasSetter is set to
 *          true / false)
 * \tparam FieldDataType Represents the data type of the field.

 *
 * Example usage:
 *
 * """
 * using NotifierConfig = HasNotifier<true, ProxyEvent<std::uint32_t>>;
 * using GetterConfig = HasGetter<false>;
 * using SetterConfig = HasSetter<false>;
 * using MyFieldConfig = FieldParameters<NotifierConfig, GetterConfig, SetterConfig, ...>;
 * """
 * \unit Socal::Proxy::ProxyField
 */
// clang-format on
template <typename Notification, typename Getter, typename Setter, typename FieldDataType>
struct ProxyFieldConfig {
  /*!
   * \brief Type-alias for the data type of this field.
   */
  using FieldType = FieldDataType;

  /*!
   * \brief Store the switches from the template parameterization to be able to use it in other structures.
   */
  enum {
    /*!
     * \brief Switch for HasNotification from the generator model.
     */
    HasNotification = Notification::active,
    /*!
     * \brief Switch for HasGetter from the generator model.
     */
    HasGetter = Getter::active,
    /*!
     * \brief Switch for HasSetter from the generator model.
     */
    HasSetter = Setter::active
  };

  /*!
   * \brief Type alias for proxy event backend.
   */
  using ProxyEventBackendInterfaceType =
      typename std::add_pointer<::amsr::socal::internal::events::ProxyEventBackendInterface<FieldType>>::type;

  /*!
   * \brief With the std::conditional we choose between ProxyEventBackendInterface or a void* type for initialization.
   */
  using ProxyEventBackendPtrType =
      typename std::conditional<HasNotification, ProxyEventBackendInterfaceType, void*>::type;

  /*!
   * \brief   This is used if hasNotifier option is set to false.
   * \details Provides a way to initialize the members with some empty structure in the field template class, because we
   *          can not enable_if member initialization in the constructor.
   *
   * \unit Socal::Proxy::ProxyField
   */
  class DummyProxyEvent {
   public:
    /*!
     * \brief Delete default constructor.
     * \steady FALSE
     */
    DummyProxyEvent() = delete;
    /*!
     * \brief Define destructor.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    virtual ~DummyProxyEvent() noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
    /*!
     * \brief Delete copy constructor.
     */
    DummyProxyEvent(DummyProxyEvent const&) = delete;
    /*!
     * \brief Delete move constructor.
     */
    DummyProxyEvent(DummyProxyEvent&&) = delete;
    /*!
     * \brief Delete copy assignment.
     */
    DummyProxyEvent& operator=(DummyProxyEvent const&) & = delete;
    /*!
     * \brief Delete move assignment.
     */
    DummyProxyEvent& operator=(DummyProxyEvent&&) & = delete;

    /*!
     * \brief Constructor is used to call it in the constructor of the proxy field itself for bypassing if an option is
     *        not active.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    explicit DummyProxyEvent(ProxyEventBackendPtrType, std::string const&, std::string const&,
                             ::ara::com::InstanceIdentifier const&) noexcept {}
  };

  /*!
   * \brief   This is used as a method backend if hasGetter or hasSetter option is set to false.
   * \details Provides a way to initialize the members with some empty structure in the field template class, because we
   *          can not enable_if member initialization in the constructor.
   * \steady FALSE
   *
   * \unit Socal::Proxy::ProxyField
   */
  class DummyProxyMethodBackend final {};

  /*!
   * \brief   This is used as a dummy method if hasGetter or hasSetter option is set to false.
   * \details Provides a way to initialize the members with some empty structure in the field template class, because we
   *          can not enable_if member initialization in the constructor.
   *
   * \unit Socal::Proxy::ProxyField
   */
  class DummyProxyMethod final {
    /*!
     * \brief Type alias for future executor class type.
     */
    using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

   public:
    /*!
     * \brief Constructor is used to call it in the constructor of the proxy field itself for bypassing if an option is
     *        not active.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    explicit DummyProxyMethod(DummyProxyMethodBackend, ExecutorInterfacePtr) noexcept {}
  };

  /*!
   * \brief With the std::conditional we choose between a concrete Event or a "null" type for initialization.
   */
  using NotificationType =
      typename std::conditional<HasNotification, typename Notification::type, DummyProxyEvent>::type;

  /*!
   * \brief With the std::conditional we choose between a concrete getter method or a "null" type for initialization.
   */
  using GetterMethodType = typename std::conditional<HasGetter, typename Getter::type, DummyProxyMethod>::type;

  /*!
   * \brief With the std::conditional we choose between a concrete setter method or a "null" type for initialization.
   */
  using SetterMethodType = typename std::conditional<HasSetter, typename Setter::type, DummyProxyMethod>::type;

  /*!
   * \brief Conditional to choose between a ProxyMethodBackendInterface to pass to the constructor of the concrete
   *        setter method or a placeholder bool to pass to the dummy method constructor.
   */
  using SetterBackendType =
      typename std::conditional<HasSetter,
                                ::amsr::socal::internal::methods::ProxyMethodBackendInterface<FieldType, FieldType>&,
                                DummyProxyMethodBackend>::type;

  /*!
   * \brief Conditional to choose between a ProxyMethodBackendInterface to pass to the constructor of the concrete
   *        getter method or a placeholder bool to pass to the dummy method constructor.
   */
  using GetterBackendType =
      typename std::conditional<HasGetter, ::amsr::socal::internal::methods::ProxyMethodBackendInterface<FieldType>&,
                                DummyProxyMethodBackend>::type;
};

}  // namespace fields
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_PROXY_FIELD_CONFIG_H_
