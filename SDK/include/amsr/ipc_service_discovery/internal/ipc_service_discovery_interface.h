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
/*! \file
 *  \brief IpcServiceDiscovery user interface.
 *  \unit IpcServiceDiscovery::UserInterface::Interface
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_INTERNAL_IPC_SERVICE_DISCOVERY_H_
#define LIB_IPC_SERVICE_DISCOVERY_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_INTERNAL_IPC_SERVICE_DISCOVERY_H_

#include "amsr/ipc_service_discovery/common/internal/ipc_service_discovery_error_domain.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/base/service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/types/types.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace ipc_service_discovery {
namespace internal {

/*!
 * \brief Identifier used to identify provided service instances uniquely within a binding type.
 */
using ProvidedServiceInstanceIdentifier = common::internal::ProvidedServiceInstanceIdentifier;

/*!
 * \brief Identifier used to identify required service instances uniquely within a binding type.
 */
using RequiredServiceInstanceIdentifier = common::internal::RequiredServiceInstanceIdentifier;

/*!
 * \brief Universal identifier that has to be specialized to a provided or required identifier before usage.
 */
using ServiceInstanceIdentifier = common::internal::ServiceInstanceIdentifier;

/*!
 * \brief State of a service instance for reporting if the instance is currently provided or not.
 */
using ProvidedState = common::internal::ProvidedState;

/*!
 * \brief Endpoint for a provided service instance.
 */
using ServiceInstanceEndpoint = common::internal::ServiceInstanceEndpoint;

/*!
 * \brief Unicast address used for connecting to an endpoint.
 */
using UnicastAddress = common::internal::UnicastAddress;

/*!
 * \brief Type of the binding using the service discovery.
 */
using BindingType = common::internal::BindingType;

/*!
 * \brief Type of the error domain of the service discovery.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_IPCSERVICEDISCOVERY_M3.4.1_typeCanBeDeclaredLocally
using IpcServiceDiscoveryErrorDomain = common::internal::IpcServiceDiscoveryErrorDomain;

/*!
 * \brief Type of the error code of the service discovery.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_IPCSERVICEDISCOVERY_M3.4.1_typeCanBeDeclaredLocally
using IpcServiceDiscoveryErrc = common::internal::IpcServiceDiscoveryErrc;

/*!
 * \brief Type of the exception of the service discovery.
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_IPCSERVICEDISCOVERY_M3.4.1_typeCanBeDeclaredLocally
using IpcServiceDiscoveryException = common::internal::IpcServiceDiscoveryException;

/*!
 * \brief Global getter function for error domain.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCSERVICEDISCOVERY_M7.3.6_usingForCommonLibrary
using common::internal::GetIpcServiceDiscoveryErrorDomain;

/*!
 * \brief Global factory function for error code.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_IPCSERVICEDISCOVERY_M7.3.6_usingForCommonLibrary
using common::internal::MakeErrorCode;

/*!
 * \brief Interface of the callback function that indicates that a listened service instance changed its state.
 * \details It is not allowed to call any API other than PollService() of the IpcServiceDiscovery within this callback.
 * \param[in] required_service_instance_identifier The required service instance.
 * \param[in] service_instance_endpoint Endpoint of the provided service instance.
 * \param[in] provided_state The state update of the provided service instance.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product Private
 */
using ListenServiceCallback = common::internal::ListenServiceCallback;

/*!
 * \brief Interface for the common usage of the IpcServiceDiscovery between different backend types.
 * \details The interface is implemented by the concrete backend bindings, which then pass on calls
 *          to the respective backend. The backends have a common error domain to report relevant
 *          errors to the user. Backend-specific errors are not reported and shall be handled by the
 *          respective backend implementation.
 * \vprivate Product Private
 * \trace DSGN-IpcServiceDiscovery-UserInterface
 */
class IpcServiceDiscoveryInterface {
 public:
  /*!
   * \brief Destructor for the IpcServiceDiscovery.
   * \details The user needs to ensure that all system resources used by the IpcServiceDiscovery are properly released
   *          before calling the destructor. The user can accomplish this by ensuring the Deinitialize() function
   *          completed execution or by destroying the instance before calling Initialize().
   * \context ANY
   * \pre Need to be in an uninitialized state. Either before calling Initialize() or after Deinitialize().
   *      This is checked by the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual ~IpcServiceDiscoveryInterface() noexcept = default;

  IpcServiceDiscoveryInterface(IpcServiceDiscoveryInterface const&) = delete;
  IpcServiceDiscoveryInterface(IpcServiceDiscoveryInterface&&) noexcept = delete;
  auto operator=(IpcServiceDiscoveryInterface const&) & -> IpcServiceDiscoveryInterface& = delete;
  auto operator=(IpcServiceDiscoveryInterface&&) & noexcept -> IpcServiceDiscoveryInterface& = delete;

  /*!
   * \brief Initialization of the IpcServiceDiscovery. Needs to be called before using any other API
            of the IpcServiceDiscovery.
   * \details The initialization will execute the code needed to initialize a concrete backend. The
              API is synchronous and thus enables the user to execute other API calls even though
              the concrete backend might still be in its initialization phase. Any service updates
              might be cached locally and will not be lost should the backend not be able to send
              the updates to remote listening clients.
   * \context ANY
   * \pre Initialize() function not yet called or Deinitialize() called beforehand.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-Initialize
   */
  virtual void Initialize() noexcept = 0;

  /*!
   * \brief Deinitialization of the IpcServiceDiscovery. Needs to be called before calling the destructor.
   * \details The deinitialization will ensure that all the system resources used by a concrete backend
   *          implementation are released in an orderly fashion. In order to be sure that the IpcServiceDiscovery
   *          is not executing any callbacks, this API needs to be called in the reactor context and needs
   *          to complete its execution before calling the destructor. Therefore, it is not allowed to
   *          call this function inside the ListenServiceCallback provided to the service discovery by
   *          the user.
   * \context REACTOR
   * \pre Initialize() function needs to be called beforehand.
   * \pre Shall only be executed in the reactor context, or after the reactor loop has been stopped. This cannot be
   *      checked by the implementation.
   * \pre Shall only be executed outside of any ListenServiceCallback. This is checked by
   *      the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-Deinitialize
   */
  virtual void Deinitialize() noexcept = 0;

  /*!
   * \brief Offer a service instance to other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated. See
   *          the returned error codes for a detailed description of the protocol violation. Note that some errors
   *          might only be able to be detected by asynchronous actions performed by the concrete backend
   *          implementation. In this case, the update will simply be ignored by the implementation.
   * \param[in] provided_service_instance_identifier ServiceInstanceIdentifier for the service instance the user wants
   *                                                 to offer to the IpcServiceDiscovery participants. Must not contain
   *                                                 any wildcard values.
   * \param[in] unicast_address                      UnicastAddress for the service instance the user wants to offer to
   *                                                 the IpcServiceDiscovery participants. Participants can then connect
   *                                                 to this address once they received the offer.
   * \return Success if the update is valid or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided                  The given ServiceInstanceIdentifier was already
   *                                                                   provided (only if protocol validation is enabled.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier was
   *                                                                   already provided but with a different
   *                                                                   unicast address (only if protocol validation
   *                                                                   is enabled).
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked by the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-OfferService
   */
  virtual auto OfferService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier,
                            UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Stop offering a service instance to other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated. See
   *          the returned error codes for a detailed description of the protocol violation. Note that some errors
   *          might only be able to be detected by asynchronous actions performed by the concrete backend
   *          implementation. In this case, the update will simply be ignored by the implementation.
   * \param[in] provided_service_instance_identifier ServiceInstanceIdentifier for the service instance, the user
   *                                                 wants to stop offering to the IpcServiceDiscovery participants.
   *                                                 Must not contain any wildcard values.
   * \param[in] unicast_address                      UnicastAddress for the service instance the user wants to stop
   *                                                 offering to the IpcServiceDiscovery participants. Participants then
   *                                                 verify the update using the received address using the address
   *                                                 received in the previous offer.
   * \return Success if the update is valid or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier is
   *                                                                   provided but with a different unicast address.
   * \error IpcServiceDiscoveryErrc::kNotProvided                      The given ServiceInstanceIdentifier is
   *                                                                   currently not provided.
   * \error IpcServiceDiscoveryErrc::kNeverProvided                    The given ServiceInstanceIdentifier was
   *                                                                   never provided.
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked by the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-StopOfferService
   */
  virtual auto StopOfferService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier,
                                UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Listen to a service instance that may be offered by other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated. See
   *          the returned error codes for a detailed description of the protocol violation. Note that some errors
   *          might only be able to be detected by asynchronous actions performed by the concrete backend
   *          implementation. In this case, the update will simply be ignored by the implementation.
   * \param[in] required_service_instance_identifier ServiceInstanceIdentifier for the service instance the user
   *                                                 wants to listen to. May contain any wildcard values.
   * \return Success if the update is valid or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kAlreadyRequired  The given ServiceInstanceIdentifier is already required.
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked by the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-ListenService
   */
  virtual auto ListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept
      -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Listen to a service instance that may be offered by other IpcServiceDiscovery participants and
            call the given callback once any updates for the listened service instance are received.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated. See
   *          the returned error codes for a detailed description of the protocol violation. Note that some errors
   *          might only be able to be detected by asynchronous actions performed by the concrete backend
   *          implementation. In this case, the update will simply be ignored by the implementation.
   * \param[in] required_service_instance_identifier ServiceInstanceIdentifier for the service instance the user
   *                                                 wants to listen to. May contain any wildcard values.
   * \param[in] listen_service_callback              ListenServiceCallback for the service instance the user
   *                                                 wants to listen to. The callback is executed once an update
   *                                                 for the listened service is received which matches the given
   *                                                 ServiceInstanceIdentifier.
   * \return Success if the update is valid or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kAlreadyRequired  The given ServiceInstanceIdentifier is already required.
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked by the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-ListenService
   */
  virtual auto ListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier,
                             ListenServiceCallback listen_service_callback) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Stop listening to a service instance that may be offered by other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated. See
   *          the returned error codes for a detailed description of the protocol violation. Note that some errors
   *          might only be able to be detected by asynchronous actions performed by the concrete backend
   *          implementation. In this case, the update will simply be ignored by the implementation.
   * \param[in] required_service_instance_identifier ServiceInstanceIdentifier for the service instance the user
   *                                                 wants to stop listening to. May contain any wildcard values.
   * \return Success if the update is valid or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNotRequired     The given ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired   The given ServiceInstanceIdentifier was never required.
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked by the implementation.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-StopListenService
   */
  virtual auto StopListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept
      -> ::ara::core::Result<void> = 0;

  /*!
   * \brief Poll a service instance that may be offered by other IpcServiceDiscovery participants.
   * \details The user needs to ensure to have called ListenService for the same required service instance
   *          with or without an attached callback for a call to this function to make sense. Otherwise, an error
   *          will be returned.
   * \param[in] required_service_instance_identifier ServiceInstanceIdentifier for the service instance the user
   *                                                 wants to poll. May contain any wildcard values.
   * \return A vector containing all matching endpoints on success or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNotRequired    A matching ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  A matching ServiceInstanceIdentifier has never been required.
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-PollService
   */
  virtual auto PollService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept
      -> ::ara::core::Result<::ara::core::Vector<ServiceInstanceEndpoint>> = 0;

  /*!
   * \brief Poll a service instance that may be provided by other IpcServiceDiscovery participants to get the current
   *        UnicastAddress.
   * \details The user needs to ensure to have called ListenService for a matching required service instance
   *          with or without an attached callback or locally provided the service itself for a call to this function to
   *          make sense. Otherwise, an error will be returned. If the service is currently offered the result contains
   *          Optional with the UnicastAddress as value otherwise it is empty.
   * \param[in] provided_service_instance_identifier ServiceInstanceIdentifier for the service instance the user wants
                                                     to poll.
   * \return A result containing a Optional with the UnicastAddress of the service if service currrently provided,
   *         otherwise empty or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNotRequired    A matching ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  A matching ServiceInstanceIdentifier has never been required.
   * \context ANY
   * \pre Initialize() function needs to be called beforehand.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace CREQ-IpcServiceDiscovery-PollService
   */
  virtual auto PollService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept
      -> ::ara::core::Result<ara::core::Optional<UnicastAddress>> = 0;

 protected:
  /*!
   * \brief Protected constructor for initialization of common values across all backends.
   * \param[in] binding_type BindingType for the IpcServiceDiscovery.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit IpcServiceDiscoveryInterface(BindingType binding_type) noexcept : binding_type_{binding_type} {}

  /*! Binding type of this IpcServiceDiscovery instance. */
  BindingType const binding_type_{};
};

}  // namespace internal
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_INTERNAL_IPC_SERVICE_DISCOVERY_H_
