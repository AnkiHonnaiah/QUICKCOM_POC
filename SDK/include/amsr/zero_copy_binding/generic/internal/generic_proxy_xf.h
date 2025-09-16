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
 *         \brief  Generic proxy backend for the ZeroCopyBinding.
 *         \unit   ZeroCopyBinding::GenericProxy
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_PROXY_XF_H_
#define LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_PROXY_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>
#include <set>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_proxy.h"
#include "amsr/socal/internal/generic_proxy_backend_interface.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/core/internal/unique_number_provider.h"
#include "amsr/zero_copy_binding/generic/internal/generic_proxy_event_backend.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_event_service_update_listener.h"
#include "ara/core/map.h"

namespace amsr {
namespace zero_copy_binding {
namespace generic {
namespace internal {

/*!
 * \brief The ZeroCopyBinding generic proxy backend.
 */
class AMSR_LG_BETA GenericProxyXf final
    : public ::amsr::socal::internal::GenericProxyBackendInterface,
      public ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryListener {
 public:
  /*!
   * \brief Type of the Socal generic proxy class.
   */
  using ProxyClass = ::amsr::socal::GenericProxy;

  /*!
   * \brief Type of backend interface implemented by this class.
   */
  using ProxyBackendInterface = ::amsr::socal::internal::GenericProxyBackendInterface;

  /*!
   * \brief Type of the service discovery used by the Socal generic proxy class.
   */
  using ProxyServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscovery;

  /*!
   * \brief         Construct a new GenericProxyXf object.
   * \param[in]     zero_copy_binding_config_view     A view to the binding's config struct.
   *                                                  It must remain valid for the object's entire lifetime.
   * \param[in]     life_cycle_manager_core           A pointer to the LifeCycleManagerCore.
   *                                                  The LifeCycleManagerCore has to exist for the object's entire
   *                                                  lifetime.
   * \param[in]     provided_instance_id              The provided instance identifier of the skeleton counterpart.
   * \context       App
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  GenericProxyXf(
      common::internal::config::ConfigView zero_copy_binding_config_view,
      std::shared_ptr<::amsr::zero_copy_binding::core::internal::LifeCycleManagerCore> const& life_cycle_manager_core,
      ::amsr::socal::internal::ProvidedInstanceIdentifier const& provided_instance_id) noexcept;

  /*!
   * \brief         Destroys the GenericProxyXf object.
   * \details       Stops to listen to the Socal service discovery before the event backends are destructed.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  ~GenericProxyXf() noexcept final;

  GenericProxyXf() = delete;
  GenericProxyXf(GenericProxyXf const&) = delete;
  GenericProxyXf(GenericProxyXf&&) = delete;
  auto operator=(GenericProxyXf const&) -> GenericProxyXf& = delete;
  auto operator=(GenericProxyXf&&) -> GenericProxyXf& = delete;

  /*!
   * \brief         Get the backend for the given event.
   * \param[in]     short_name  The shortname of the event.
   * \return        A reference to a generic event backend.
   * \pre           The passed shortname has to be among the ones returned by GetAllEvents().
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  auto GetEventBackend(StringView const& short_name) noexcept -> GenericProxyEventBackendInterface& final;

  /*!
   * \brief         Get the shortnames of all the configured events.
   * \return        A set containing shortnames of all the configured events.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  auto GetAllEvents() const noexcept -> std::set<StringView> final;

  /*!
   * \brief         Notify each event about the incoming offer service.
   * \context       Reactor
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  void OnServiceOffered() noexcept final;

  /*!
   * \brief         Notify each event about the incoming stop offer service.
   * \context       Reactor
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   */
  void OnServiceStopped() noexcept final;

  /*!
   * \brief         Get the backend for the given field. Not supported for the ZeroCopyBinding.
   * \param[in]     short_name  The shortname of the field.
   * \return        This method does not return as it aborts.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  [[noreturn]] auto GetFieldBackend(StringView const& short_name) noexcept -> GenericProxyFieldBackendInterface& final;

  /*!
   * \brief         Get the backend for the given fire-and-forget method. Not supported for the ZeroCopyBinding.
   * \param[in]     short_name  The shortname of the fire-and-forget method.
   * \return        This method does not return as it aborts.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  [[noreturn]] auto GetFireAndForgetMethodBackend(StringView const& short_name) noexcept
      -> GenericProxyFireAndForgetMethodBackendInterface& final;

  /*!
   * \brief         Get the backend for the given method. Not supported for the ZeroCopyBinding.
   * \param[in]     short_name  The shortname of the method.
   * \return        This method does not return as it aborts.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  [[noreturn]] auto GetMethodBackend(StringView const& short_name) noexcept
      -> GenericProxyMethodBackendInterface& final;

  /*!
   * \brief         Get the shortnames of all the configured fields.
   * \return        A set containing shortnames of all the configured fields.
   *                For ZeroCopyBinding the returned set is always empty.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  auto GetAllFields() const noexcept -> std::set<StringView> final;

  /*!
   * \brief         Get the shortnames of all the configured fire-and-forget methods.
   * \return        A set containing shortnames of all the configured fire-and-forget methods.
   *                For ZeroCopyBinding the returned set is always empty.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  auto GetAllFireAndForgetMethods() const noexcept -> std::set<StringView> final;

  /*!
   * \brief         Get the shortnames of all the configured methods.
   * \return        A set containing shortnames of all the configured methods.
   *                For ZeroCopyBinding the returned set is always empty.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   *                FALSE against any other API of this class.
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        FALSE
   */
  auto GetAllMethods() const noexcept -> std::set<StringView> final;

 private:
  /*!
   * \brief  The Logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

  /*!
   * \brief  A map containing backends for all configured events.
   * \details  Shall not be altered after the constructor.
   */
  ::ara::core::Map<StringView, GenericProxyEventBackend> event_map_;

  /*!
   * \brief  The provided instance identifier of the skeleton counterpart.
   */
  ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id_;

  /*!
   * \brief  The proxy's instance id.
   */
  std::size_t const proxy_instance_id_;

  /*!
   * \brief  A counter needed to determine the proxy instance id.
   */
  static core::internal::UniqueNumberProvider instance_counter_;
};

}  // namespace internal
}  // namespace generic
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_GENERIC_PROXY_XF_H_
