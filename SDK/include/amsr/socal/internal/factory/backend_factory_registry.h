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
/**        \file  backend_factory_registry.h
 *        \brief  Registry class for managing the proxy/skeleton backend factories.
 *      \details  Provides APIs to register/deregister the backend factories and to create the backends.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_BACKEND_FACTORY_REGISTRY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_BACKEND_FACTORY_REGISTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/socal/internal/factory/backend_factory_registry_interface.h"
#include "amsr/socal/internal/factory/proxy_backend_factory_interface.h"
#include "amsr/socal/internal/factory/skeleton_backend_factory_interface.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/com_error_domain.h"

namespace amsr {
namespace socal {
namespace internal {
namespace factory {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;

/*!
 * \brief Registry class for managing the proxy/skeleton backend factories.
 * \tparam BackendFactoryType      The proxy/skeleton backend factory type.
 * \tparam InstanceIdentifierType  Type of the instance identifier.
 *                                 'RequiredInstanceIdentifier' for proxy registry, and
 *                                 'ProvidedInstanceIdentifier' for skeleton registry.
 *
 * \unit Socal::BackendFactoryRegistry
 */
template <typename BackendFactoryType, typename InstanceIdentifierType>
class BackendFactoryRegistry final
    : public BackendFactoryRegistryInterface<BackendFactoryType, InstanceIdentifierType> {
 public:
  /*!
   * \brief Type alias for the base class.
   */
  using BaseType = BackendFactoryRegistryInterface<BackendFactoryType, InstanceIdentifierType>;

  /*!
   * \brief Type alias for the proxy/skeleton backend factory pointer.
   */
  using BackendFactoryPtr = typename BaseType::BackendFactoryPtr;

  /*!
   * \brief proxy/skeleton backend factory container type.
   */
  using BackendFactoryMap = std::map<InstanceIdentifierType, BackendFactoryPtr>;

  /*!
   * \brief Type alias for the proxy/skeleton backend interface pointer.
   */
  using BackendInterfacePtr = typename BackendFactoryType::BackendInterfacePtr;

  /*!
   * \brief Constructs the BackendFactoryRegistry instance.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  BackendFactoryRegistry() noexcept = default;

  BackendFactoryRegistry(BackendFactoryRegistry const&) = delete;
  BackendFactoryRegistry(BackendFactoryRegistry&&) = delete;
  BackendFactoryRegistry& operator=(BackendFactoryRegistry const&) & = delete;
  BackendFactoryRegistry& operator=(BackendFactoryRegistry&&) & = delete;

  /*!
   * \brief Destroys the BackendFactoryRegistry instance.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~BackendFactoryRegistry() noexcept = default;

  /*!
   * \copydoc BaseType::RegisterFactory()
   * \internal
   * - If the given instance ID already exists in the factory map, return an error.
   * - Otherwise, store the given instance ID and the factory pointer mapping.
   * \endinternal
   */
  ::amsr::core::Result<void> RegisterFactory(InstanceIdentifierType const& instance_id,
                                             BackendFactoryPtr factory_ptr) noexcept final {
    ::amsr::core::Result<void> result{::ara::com::ComErrc::kErrorNotOk};
    if (factory_ptr == nullptr) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Nullptr check failed!";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    std::pair<typename BackendFactoryMap::iterator, bool> const emplace_result{
        // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_PASSING_ARGUMENT_AFTER_CHECK
        factory_map_.emplace(instance_id, std::move(factory_ptr))};

    if (emplace_result.second) {
      result.EmplaceValue();
    } else {
      logger_.LogError(
          [&instance_id](::amsr::socal::internal::logging::LogStream& s) {
            // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
            ::amsr::core::StringView const instance_id_sv{instance_id.ToString()};
            s << "Factory already registered for the instance ID: " << instance_id_sv;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \copydoc BaseType::DeregisterFactory()
   * \internal
   * - If the given instance ID does not exists in the factory map, return an error.
   * - Otherwise, remove the given instance ID and its corresponding factory pointer from the map.
   * \endinternal
   */
  ::amsr::core::Result<void> DeregisterFactory(InstanceIdentifierType const& instance_id) noexcept final {
    ::amsr::core::Result<void> result{::ara::com::ComErrc::kErrorNotOk};
    typename BackendFactoryMap::size_type const erase_result{
        factory_map_.erase(instance_id)};  // VCA_SOCAL_CALLING_STL_APIS
    if (erase_result > 0) {
      result.EmplaceValue();
    } else {
      logger_.LogError(
          [&instance_id](::amsr::socal::internal::logging::LogStream& s) {
            // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
            ::amsr::core::StringView const instance_id_sv{instance_id.ToString()};
            s << "Factory not registered for the instance ID: " << instance_id_sv;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \brief Creates a skeleton backend.
   * \tparam    IIDType      Type of the instance identifier. Always defaulted to 'ProvidedInstanceIdentifier'.
   * \param[in] instance_id  The Provided instance ID for which the backend shall be created.
   * \return Result containing a pointer to the created backend or an error. The returned pointer must not be null.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Create the backend from the factory corresponding to the given provided instance ID.
   * - Validate the created backend pointer.
   * \endinternal
   */
  template <typename IIDType = InstanceIdentifierType,
            typename = std::enable_if_t<std::is_same<IIDType, ProvidedInstanceIdentifier>::value>>
  auto CreateBackend(ProvidedInstanceIdentifier const& instance_id) const noexcept
      -> ::amsr::core::Result<BackendInterfacePtr> {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
    // VCA_SOCAL_DEREFERENCING_SMART_POINTER_WITH_RUNTIME_CHECK
    ::amsr::core::Result<BackendInterfacePtr> result{GetFactory(instance_id)->CreateBackend()};
    ValidateTheCreatedBackend(result, instance_id);
    return result;
  }

  /*!
   * \brief Creates a proxy backend.
   * \tparam    IIDType               Type of the instance identifier. Always defaulted to 'RequiredInstanceIdentifier'.
   * \param[in] required_instance_id  The required instance ID for which the factory shall be retrieved.
   * \param[in] provided_instance_id  The provided instance ID for which the backend shall be created.
   * \return Result containing a pointer to the created backend or an error. The returned pointer must not be null.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \error ComErrc::kNetworkBindingFailure   The network binding reports a failure.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Retrieve the factory corresponding to the given required instance ID.
   * - Create the backend for the given provided instance ID from the retrieved factory.
   * - Validate the created backend pointer.
   * \endinternal
   */
  template <typename IIDType = InstanceIdentifierType,
            typename = std::enable_if_t<std::is_same<IIDType, RequiredInstanceIdentifier>::value>>
  auto CreateBackend(RequiredInstanceIdentifier const& required_instance_id,
                     ProvidedInstanceIdentifier const& provided_instance_id) const noexcept
      -> ::amsr::core::Result<BackendInterfacePtr> {
    ::amsr::core::Result<BackendInterfacePtr> result{
        GetFactory(required_instance_id)
            // VECTOR NL AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
            ->CreateBackend(provided_instance_id)};  // VCA_SOCAL_DEREFERENCING_SMART_POINTER_WITH_RUNTIME_CHECK
    ValidateTheCreatedBackend(result, provided_instance_id);
    return result;
  }

 private:
  /*!
   * \brief Gets the registered backend factory for the given instance ID.
   * \param[in] instance_id  Instance identifier for which the factory shall be retrieved.
   * \return Pointer to the registered backend factory, if exists.
   * \pre     -
   * \context App
   * \steady FALSE
   * \internal
   * - If the factory was not registered for the given instance ID, abort the application.
   * \endinternal
   */
  BackendFactoryType* GetFactory(InstanceIdentifierType const& instance_id) const noexcept {
    typename BackendFactoryMap::const_iterator const itr{factory_map_.find(instance_id)};
    if (itr == factory_map_.end()) {
      logger_.LogFatalAndAbort(
          [&instance_id](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
            ::amsr::core::StringView const instance_id_sv{instance_id.ToString()};
            // VCA_SOCAL_CALLING_STL_APIS
            s << "No factory found for the given instance identifier: " << instance_id_sv;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return itr->second.get();
  }

  /*!
   * \brief Validates the backend created by the factory.
   * \param[in] result       Result of the backend creation.
   * \param[in] instance_id  Instance identifier for which the backend was created.
   * \pre     -
   * \context App
   * \steady FALSE
   * \internal
   * - If the created backend pointer is null, abort the application.
   * \endinternal
   */
  void ValidateTheCreatedBackend(::amsr::core::Result<BackendInterfacePtr> const& result,
                                 ProvidedInstanceIdentifier const& instance_id) const noexcept {
    if (result.HasValue() && (result.Value() == nullptr)) {
      logger_.LogFatalAndAbort(
          [&instance_id](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
            ::amsr::core::StringView const instance_id_sv{instance_id.ToString()};
            // VCA_SOCAL_CALLING_STL_APIS
            s << "The created backend pointer is null for the given instance identifier: " << instance_id_sv;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Map to store the registered proxy/skeleton factories and their corresponding instance IDs.
   * \note  No need to protect this resource, as the write operations are performed only in Init/Deinit phase and read
   *        operations are performed in runtime phase.
   */
  BackendFactoryMap factory_map_{};

  /*!
   * \brief Logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "BackendFactoryRegistry"_sv};
};

/*!
 * \brief Registry class for managing the proxy backend factories.
 * \tparam ProxyBackendInterfaceType  The proxy backend interface type.
 */
template <typename ProxyBackendInterfaceType>
using ProxyBackendFactoryRegistry =
    BackendFactoryRegistry<ProxyBackendFactoryInterface<ProxyBackendInterfaceType>, RequiredInstanceIdentifier>;

/*!
 * \brief Registry class for managing the skeleton backend factories.
 * \tparam SkeletonBackendInterfaceType  The skeleton backend interface type.
 */
template <typename SkeletonBackendInterfaceType>
using SkeletonBackendFactoryRegistry =
    BackendFactoryRegistry<SkeletonBackendFactoryInterface<SkeletonBackendInterfaceType>, ProvidedInstanceIdentifier>;

}  // namespace factory
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_BACKEND_FACTORY_REGISTRY_H_
