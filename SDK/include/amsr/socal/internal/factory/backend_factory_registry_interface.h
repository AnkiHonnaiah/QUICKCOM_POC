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
/**        \file  backend_factory_registry_interface.h
 *        \brief  Interface class that provides APIs to register/deregister the proxy/skeleton backend factories.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_BACKEND_FACTORY_REGISTRY_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_BACKEND_FACTORY_REGISTRY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/socal/internal/factory/proxy_backend_factory_interface.h"
#include "amsr/socal/internal/factory/skeleton_backend_factory_interface.h"
#include "amsr/socal/internal/internal_instance_identifier.h"

namespace amsr {
namespace socal {
namespace internal {
namespace factory {

/*!
 * \brief Interface class that provides APIs to register/deregister the proxy/skeleton backend factories.
 * \tparam BackendFactoryType      The proxy/skeleton backend factory type.
 * \tparam InstanceIdentifierType  Type of the instance identifier.
 *                                 'RequiredInstanceIdentifier' for proxy registry, and
 *                                 'ProvidedInstanceIdentifier' for skeleton registry.
 *
 * \unit Socal::BackendFactoryRegistry
 */
template <typename BackendFactoryType, typename InstanceIdentifierType>
class BackendFactoryRegistryInterface {
 public:
  /*!
   * \brief The proxy/skeleton backend factory pointer type.
   */
  using BackendFactoryPtr = std::unique_ptr<BackendFactoryType>;

  /*!
   * \brief Constructs the BackendFactoryRegistryInterface.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  BackendFactoryRegistryInterface() noexcept = default;

  BackendFactoryRegistryInterface(BackendFactoryRegistryInterface const&) = delete;
  BackendFactoryRegistryInterface(BackendFactoryRegistryInterface&&) = delete;
  BackendFactoryRegistryInterface& operator=(BackendFactoryRegistryInterface const&) & = delete;
  BackendFactoryRegistryInterface& operator=(BackendFactoryRegistryInterface&&) & = delete;

  /*!
   * \brief Register a proxy/skeleton backend factory.
   * \param[in] instance_id  Instance identifier for which the factory shall be registered.
   * \param[in] factory_ptr  The backend factory pointer that shall be used to create the proxy/skeleton backend.
   *                         This must not be nullptr.
   * \return Result containing no value or an error.
   * \error ara::com::ComErrc::kErrorNotOk  If a factory is already registered for the given instance id.
   * \pre         -
   * \context     BindingInit
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires factory_ptr != nullptr; \endspec
   */
  virtual ::amsr::core::Result<void> RegisterFactory(InstanceIdentifierType const& instance_id,
                                                     BackendFactoryPtr factory_ptr) noexcept = 0;

  /*!
   * \brief Deregister a proxy/skeleton backend factory.
   * \param[in] instance_id  Instance identifier for which the factory shall be deregistered.
   * \return Result containing no value or an error.
   * \error ara::com::ComErrc::kErrorNotOk  If no factory is registered for the given instance identifier.
   * \pre         -
   * \context     BindingShutdown
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual ::amsr::core::Result<void> DeregisterFactory(InstanceIdentifierType const& instance_id) noexcept = 0;

 protected:
  /*!
   * \brief Destroys the BackendFactoryRegistryInterface.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~BackendFactoryRegistryInterface() noexcept = default;
};

/*!
 * \brief Interface class that provides APIs to register/deregister the proxy backend factories.
 * \tparam ProxyBackendInterfaceType  The proxy backend interface type.
 */
template <typename ProxyBackendInterfaceType>
using ProxyBackendFactoryRegistryInterface =
    BackendFactoryRegistryInterface<ProxyBackendFactoryInterface<ProxyBackendInterfaceType>,
                                    RequiredInstanceIdentifier>;

/*!
 * \brief Interface class that provides APIs to register/deregister the skeleton backend factories.
 * \tparam SkeletonBackendInterfaceType  The skeleton backend interface type.
 */
template <typename SkeletonBackendInterfaceType>
using SkeletonBackendFactoryRegistryInterface =
    BackendFactoryRegistryInterface<SkeletonBackendFactoryInterface<SkeletonBackendInterfaceType>,
                                    ProvidedInstanceIdentifier>;

}  // namespace factory
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_BACKEND_FACTORY_REGISTRY_INTERFACE_H_
