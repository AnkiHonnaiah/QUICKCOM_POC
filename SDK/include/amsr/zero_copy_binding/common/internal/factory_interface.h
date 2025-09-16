/*!********************************************************************************************************************
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
 *         \brief  Interface to abstract static code from model-configured code.
 *
 *         \unit   ZeroCopyBinding::ZeroCopyBindingCommon::Factory
 *
 * \complexity This unit exceeds the metric for max inter-unit calls. As this unit is used to abstract static
 *             model-configured code and mocking of external components it has many dependencies to different units of
 *             the ZeroCopyBinding, since many of them use model-configured code and facilities from external
 *             components. As the threshold is only exceeded by 2 the risk is considered low.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_FACTORY_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <iterator>
#include <memory>
#include <vector>

#include "amsr/ipc/acceptor.h"
#include "amsr/ipc/connection.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery_proxy_xf_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery_skeleton_xf_interface.h"
#include "amsr/zero_copy_binding/core/internal/acceptor_wrapper_interface.h"
#include "amsr/zero_copy_binding/core/internal/connection_wrapper_interface.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_client.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_server.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_side_channel.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace zero_copy_binding {

namespace core {
namespace internal {

/*!
 * \brief Declaration of LifeCycleManagerCore.
 */
class LifeCycleManagerCore;

/*!
 * \brief Declaration of ConnectionWrapperInterface.
 */
class ConnectionWrapperInterface;

/*!
 * \brief Declaration of AcceptorWrapperInterface.
 */
class AcceptorWrapperInterface;

}  // namespace internal
}  // namespace core

namespace common {
namespace internal {

class FactoryInterface;

auto MakeFactoryImpl() noexcept -> std::shared_ptr<FactoryInterface>;

/*!
 * \brief FactoryInterface class abstracts the static code from everything that is configurable via AUTOSAR model.
 */
class FactoryInterface {
 public:
  /*!
   * \brief   Default constructor.
   * \context Init
   */
  FactoryInterface() noexcept = default;

  /*!
   * \brief   Default destructor.
   * \context Shutdown
   */
  virtual ~FactoryInterface() noexcept = default;

  FactoryInterface(FactoryInterface const&) = delete;
  auto operator=(FactoryInterface const&) -> FactoryInterface& = delete;
  FactoryInterface(FactoryInterface&&) = delete;
  auto operator=(FactoryInterface&&) -> FactoryInterface& = delete;

  /*!
   * \brief Type of container for SDSkeletonXfs held by LifecycleManagerXf.
   */
  using SDSkeletonXfInterfacesContainerType = std::vector<std::unique_ptr<ServiceDiscoverySkeletonXfInterface>>;

  /*!
   * \brief Type of container inserter for SDSkeletonXfs held by LifecycleManagerXf.
   */
  using SDSkeletonXfInterfacesContainerInserterType = std::back_insert_iterator<SDSkeletonXfInterfacesContainerType>;

  /*!
   * \brief Type of a single initializer function for a provided service instance.
   */
  using ProvidedServiceInstanceInitializerFunction =
      std::function<void(config::ConfigView const&, std::shared_ptr<core::internal::LifeCycleManagerCore> const&,
                         SDSkeletonXfInterfacesContainerInserterType)>;

  /*!
   * \brief       Return a list of function references to initialize provided service instances.
   * \return      List of initializer functions for provided service instances.
   * \context     ANY
   * \pre         The factory object has to outlive the returned reference.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetBindingXfProvidedServiceInstanceInitializerFunctions() const noexcept
      -> std::vector<ProvidedServiceInstanceInitializerFunction> const& = 0;

  /*!
   * \brief Type of container for ProxyXfs held by LifecycleManagerXf.
   */
  using SDProxyXfInterfacesContainerType = std::vector<std::unique_ptr<ServiceDiscoveryProxyXfInterface>>;

  /*!
   * \brief Type of container inserter for ProxyXfs held by LifecycleManagerXf.
   */
  using SDProxyXfInterfacesContainerInserterType = std::back_insert_iterator<SDProxyXfInterfacesContainerType>;

  /*!
   * \brief Type of a single initializer function for a required service instance.
   */
  using RequiredServiceInstanceInitializerFunction =
      std::function<void(config::ConfigView const&, std::shared_ptr<core::internal::LifeCycleManagerCore> const&,
                         SDProxyXfInterfacesContainerInserterType)>;

  /*!
   * \brief       Return a list of function references to initialize required service instances.
   * \return      List of initializer functions for required service instances.
   * \context     ANY
   * \pre         The factory object has to outlive the returned reference.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetBindingXfRequiredServiceInstanceInitializerFunctions() const noexcept
      -> std::vector<RequiredServiceInstanceInitializerFunction> const& = 0;

  /*!
   * \brief Type of a single deinitializer function for a provided service instance.
   */
  using ProvidedServiceInstanceDeinitializerFunction =
      std::function<void(config::ConfigView const&, std::unique_ptr<ServiceDiscoverySkeletonXfInterface>)>;

  /*!
   * \brief       Return a list of function references to deinitialize provided service instances.
   * \return      List of deinitializer functions for provided service instances.
   * \context     ANY
   * \pre         The factory object has to outlive the returned reference.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetBindingXfProvidedServiceInstanceDeinitializerFunctions() const noexcept
      -> std::vector<ProvidedServiceInstanceDeinitializerFunction> const& = 0;

  /*!
   * \brief Type of a single deinitializer function for a required service instance.
   */
  using RequiredServiceInstanceDeinitializerFunction =
      std::function<void(config::ConfigView const&, std::unique_ptr<ServiceDiscoveryProxyXfInterface>)>;

  /*!
   * \brief       Return a list of function references to deinitialize required service instances.
   * \return      List of deinitializer functions for required service instances.
   * \context     ANY
   * \pre         The factory object has to outlive the returned reference.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetBindingXfRequiredServiceInstanceDeinitializerFunctions() const noexcept
      -> std::vector<RequiredServiceInstanceDeinitializerFunction> const& = 0;

  /*!
   * \brief       Return the configuration struct of the binding.
   * \return      The configuration struct of the binding.
   * \context     ANY
   * \pre         The factory object has to outlive the returned config reference.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetBindingConfiguration() const noexcept -> config::ZeroCopyBindingConfig const& = 0;

  /*!
   * \brief       Return the ZeroCopyCommon server builder to be used by the binding.
   * \return      The ZeroCopyCommon server builder used by the binding.
   * \context     ANY
   * \pre -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetZeroCopyCommonServerBuilder() const noexcept
      -> std::shared_ptr<::amsr::zero_copy_common::internal::ServerBuilderInterface> = 0;

  /*!
   * \brief       Return the ZeroCopyCommon client builder to be used by the binding.
   * \return      The ZeroCopyCommon client builder used by the binding.
   * \context     ANY
   * \pre -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetZeroCopyCommonClientBuilder() const noexcept
      -> std::shared_ptr<::amsr::zero_copy_common::internal::ClientBuilderInterface> = 0;

  /*!
   * \brief       Return the reactor to be used by the binding.
   * \return      The reactor used by the binding.
   * \context     ANY
   * \pre -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   */
  virtual auto GetReactor() const noexcept -> ::osabstraction::io::reactor1::Reactor1Interface& = 0;

  /*!
   * \brief         Return the service discovery to be used by the binding.
   * \return        The service discovery used by the binding.
   * \context       ANY
   * \pre -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate
   */
  virtual auto GetServiceDiscovery() const noexcept
      -> std::shared_ptr<service_discovery::ServiceDiscoveryInterface> = 0;

  /*!
   * \brief         Return the connection wrapper to be used by the event backends of the binding.
   * \param[in]     connection  The connection that shall be wrapped.
   * \return        The connection wrapper to be used by event backends of the binding.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate
   */
  virtual auto GetConnectionWrapper(std::unique_ptr<::amsr::ipc::Connection> connection) const noexcept
      -> std::unique_ptr<core::internal::ConnectionWrapperInterface> = 0;

  /*!
   * \brief         Return the acceptor wrapper to be used by the skeleton event manager of the binding.
   * \param[in]     acceptor    The acceptor that shall be wrapped.
   * \return        The acceptor wrapper to be used by skeleton event manager of the binding.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate
   */
  virtual auto GetAcceptorWrapper(std::unique_ptr<::amsr::ipc::Acceptor> acceptor) const noexcept
      -> std::unique_ptr<core::internal::AcceptorWrapperInterface> = 0;

  /*!
   * \brief         Return the side channel to be used by the ZeroCopyCommon server and client.
   * \param[in]     connection         A reference to the connection that shall be wrapped.
   *                                   Must be valid for the entire life time of the returned side channel.
   * \return        The side channel to be used by ZeroCopyCommon.
   * \context       ANY
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate
   */
  virtual auto GetSafeIPCSideChannel(::amsr::ipc::Connection& connection) const noexcept
      -> std::unique_ptr<::amsr::zero_copy_common::internal::SideChannelInterface> = 0;
};

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_FACTORY_INTERFACE_H_
