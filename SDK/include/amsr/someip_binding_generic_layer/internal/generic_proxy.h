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
/*!        \file   someip_binding_generic_layer/internal/generic_proxy.h
 *         \brief  Creates and connects generic proxy classes with the backend.
 *         \unit   SomeIpBinding::SomeIpBindingGenericLayer::GenericProxy
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <set>
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/generic_proxy_backend_interface.h"
#include "amsr/someip_binding_core/internal/local_client.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_generic_layer/internal/events/generic_proxy_event.h"
#include "amsr/someip_binding_generic_layer/internal/fields/generic_proxy_field.h"
#include "amsr/someip_binding_generic_layer/internal/methods/generic_proxy_fire_and_forget_method.h"
#include "amsr/someip_binding_generic_layer/internal/methods/generic_proxy_method.h"
#include "ara/core/map.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief   GenericProxy class
 * \details Owner of the GenericProxy backends for events, fields and methods (Method implementation still missing)
 */
class GenericProxy final : public amsr::socal::internal::GenericProxyBackendInterface {
  /*!
   * \brief Alias for GenericProxyEventMap
   */
  using GenericProxyEventMap = ara::core::Map<amsr::core::String, events::GenericProxyEvent>;

  /*!
   * \brief Alias for GenericProxyFieldMap
   */
  using GenericProxyFieldMap = ara::core::Map<amsr::core::String, fields::GenericProxyField>;

  /*!
   * \brief Alias for GenericProxyFireAndForgetMethodMap
   */
  using GenericProxyFireAndForgetMethodMap =
      ara::core::Map<amsr::core::String, methods::GenericProxyFireAndForgetMethod>;

  /*!
   * \brief Alias for GenericProxyMethodMap
   */
  using GenericProxyMethodMap = ara::core::Map<amsr::core::String, methods::GenericProxyMethod>;

 public:
  /*!
   * \brief Constructor.
   * \param[in] someip_client_identity  Unique ID of the client.
   * \param[in] local_client            The local client.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GenericProxy(someip_binding_core::internal::ProxyBindingIdentity someip_client_identity,
               someip_binding_core::internal::LocalClient&& local_client);

  /*!
   * \brief Destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~GenericProxy() final;

  GenericProxy(GenericProxy&&) = delete;
  GenericProxy(GenericProxy const&) = delete;
  auto operator=(GenericProxy const&) -> GenericProxy& = delete;
  auto operator=(GenericProxy&&) -> GenericProxy& = delete;
  /*!
   * \brief   Get the backend for the given event.
   * \details To ensure thread-safety, for each event a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the event.
   * \return A generic event backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend
   * object.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
                 FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetEventBackend(::amsr::core::StringView const& short_name) noexcept -> GenericProxyEventBackendInterface& final;

  /*!
   * \brief   Get the backend for the given field.
   * \details To ensure thread-safety, for each field a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the field.
   * \return A generic field backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend
   *         object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetFieldBackend(::amsr::core::StringView const& short_name) noexcept -> GenericProxyFieldBackendInterface& final;

  /*!
   * \brief   Get the backend for the given fire-and-forget method.
   * \details To ensure thread-safety, for each fire-and-forget method a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the fire-and-forget method.
   * \return A generic fire-and-forget method backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetFireAndForgetMethodBackend(::amsr::core::StringView const& short_name) noexcept
      -> GenericProxyFireAndForgetMethodBackendInterface& final;

  /*!
   * \brief   Get the backend for the given method.
   * \details To ensure thread-safety, for each method a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the method.
   * \return A generic method backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetMethodBackend(::amsr::core::StringView const& short_name) noexcept
      -> GenericProxyMethodBackendInterface& final;

  /*!
   * \brief Get the short-names of all the configured events.
   * \return A set containing short-names of all the configured events. The returned StringView objects must be
   *         valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllEvents() const noexcept -> std::set<::amsr::core::StringView> final;

  /*!
   * \brief Get the short-names of all the configured fields.
   * \return A set containing short-names of all the configured fields. The returned StringView objects must be
   *         valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllFields() const noexcept -> std::set<::amsr::core::StringView> final;

  /*!
   * \brief Get the short-names of all the configured fire-and-forget methods.
   * \return A set containing short-names of all the configured fire-and-forget methods. The returned StringView objects
   *         must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllFireAndForgetMethods() const noexcept -> std::set<::amsr::core::StringView> final;

  /*!
   * \brief Get the short-names of all the configured methods.
   * \return A set containing short-names of all the configured methods. The returned StringView objects must be valid
   *         at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetAllMethods() const noexcept -> std::set<::amsr::core::StringView> final;

 private:
  /*!
   * \brief Creates the GenericProxy backends for events and stores them in its corresponding maps.
   */
  void FillEventBackendMaps() noexcept;

  /*!
   * \brief Creates the GenericProxy backends for fields and stores them in its corresponding maps.
   */
  void FillFieldBackendMaps() noexcept;

  /*!
   * \brief Creates the GenericProxy backends for methods and fire and forget methods and stores them in its
   *        corresponding maps.
   */
  void FillMethodBackendMaps() noexcept;

  /*!
   * \brief local client
   */
  amsr::someip_binding_core::internal::LocalClient local_client_;

  /*!
   * \brief Generic proxy event map
   */
  GenericProxyEventMap generic_proxy_event_map_{};

  /*!
   * \brief Generic proxy field map
   */
  GenericProxyFieldMap generic_proxy_field_map_{};

  /*!
   * \brief Generic proxy fire and forget method map
   */
  GenericProxyFireAndForgetMethodMap generic_proxy_fire_and_forget_method_map_{};

  /*!
   * \brief Generic proxy method map
   */
  GenericProxyMethodMap generic_proxy_method_map_{};

  /*!
   * \brief Memory buffer allocator.
   */
  amsr::SharedPtr<vac::memory::allocator::MemoryBufferAllocator> method_buffer_allocator_{
      amsr::AllocateShared<amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator>(
          ara::core::PolymorphicAllocator<amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator>{})
          .Value()};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_H_
