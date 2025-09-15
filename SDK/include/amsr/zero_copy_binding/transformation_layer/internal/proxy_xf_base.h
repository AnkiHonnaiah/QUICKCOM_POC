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
 *         \brief  Contains common functionality for the ProxyXf classes that does not have to be generated.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_XF_BASE_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_XF_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <initializer_list>
#include <tuple>

#include "amsr/socal/internal/service_discovery/proxy_service_discovery_listener.h"
#include "amsr/socal/skeleton_connection_state.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_event_service_update_listener.h"
#include "ara/core/vector.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief Base class for the generated ProxyXf classes with the static functionality.
 *
 * \unit ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ProxyXf
 */
class ProxyXfBase : public ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryListener {
 protected:
  using ConfigView = ::amsr::zero_copy_binding::common::internal::config::ConfigView;

 public:
  /*!
   * \brief         Construct a new ProxyXfBase object.
   * \param[in]     zero_copy_binding_config_view   A view to the binding's config struct.
   *                                                The LifeCycleManagerXf has to exist for the object's entire
   *                                                lifetime to ensure its validity.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  explicit ProxyXfBase(ConfigView zero_copy_binding_config_view) noexcept;

  /*!
   * \brief         Destructor.
   * \details       Do not use event_container_ here, as the pointers will be invalid at this point.
   * \pre           -
   * \context       ANY
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
  virtual ~ProxyXfBase() noexcept = default;

  ProxyXfBase() = delete;
  ProxyXfBase(ProxyXfBase const&) = delete;
  ProxyXfBase(ProxyXfBase&&) = delete;
  auto operator=(ProxyXfBase const&) -> ProxyXfBase& = delete;
  auto operator=(ProxyXfBase&&) -> ProxyXfBase& = delete;

  /*!
   * \brief         Get the connection state between proxy and skeleton.
   * \return        ::amsr::socal::SkeletonConnectionState
   * \pre           -
   * \context       ANY
   * \threadsafe    TRUE for different backend instances, FALSE otherwise.
   * \reentrant     TRUE for different backend instances, FALSE otherwise.
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  auto ReadSkeletonConnectionStateImpl() noexcept -> ::amsr::socal::SkeletonConnectionState;

  /*!
   * \brief         Notify each event about the incoming offer service.
   * \context       Reactor
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  void OnServiceOffered() noexcept final;

  /*!
   * \brief         Notify each event about the incoming stop offer service.
   * \context       Reactor
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  void OnServiceStopped() noexcept final;

 protected:
  /*!
   * \brief         Initialize the event container with pointers to the passed events.
   * \param[in]     event_backends  The event backends that shall be stored in the container.
   * \context       App
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  void InitializeEventContainer(std::initializer_list<ProxyEventServiceUpdateListener*> event_backends) noexcept;

 private:
  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

  /*!
   * \brief A view to the binding's configuration data.
   */
  ConfigView const zero_copy_binding_config_view_;

  /*!
   * \brief A container collecting the events such that they can be iterated over.
   */
  ::ara::core::Vector<ProxyEventServiceUpdateListener*> event_container_{};

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ProxyXf__ProxyXfBase, ProxyXfBaseStoresCorrectEvents);
  FRIEND_TEST(UT__ProxyXf__ProxyXfGenerated, ConstructionSuccessful);
  FRIEND_TEST(UT__ProxyXf__ProxyXfGenerated, GetEventBackends);
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_XF_BASE_H_
