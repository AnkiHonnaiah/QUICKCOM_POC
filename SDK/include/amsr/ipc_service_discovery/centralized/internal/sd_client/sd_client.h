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
/*!       \file
 *        \brief IPC Service Discovery client sends and receives messages from the SdServer.
 *        \unit IpcServiceDiscovery::Centralized::SdClient
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_SD_CLIENT_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_SD_CLIENT_H_

#include <mutex>
#include "amsr/ipc_service_discovery/centralized/internal/config/config.h"
#include "amsr/ipc_service_discovery/centralized/internal/ipc_client/ipc_client.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/message_parser.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_client/provided_service_registries.h"
#include "amsr/ipc_service_discovery/centralized/internal/sd_client/required_service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/service_registry/service_registry.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/buffer_types.h"
#include "amsr/ipc_service_discovery/common/internal/logger/logger.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/types/types.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/timer_manager_interface.h"
#include "ara/core/vector.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

// VECTOR NC Metric-OO.WMC.One: MD_IPCSERVICEDISCOVERY_Metric-OO.WMC.One_weightedMethodsPerClassTooHigh
/*!
 * \brief IPC Service Discovery client sends and receives messages from the SdServer.
 * \details The SdClient uses the IpcClient to establish the connection to the SdServer. The SdClient sends and
            receives messages from the SdServer over the IpcClient. Incoming messages are analyzed and processed.
            Service updates received via dedicated service messages are saved inside the ServiceRegistry. Local
            ServiceRegistry updates are forwarded to the SdServer.
 *
 * \vprivate Component internal.
 */
class SdClient final : private MessageParser {
 public:
  /*!
   * \brief Error code type for the IpcServiceDiscovery.
   */
  using Errc = IpcServiceDiscoveryErrc;

  using ProvidedServiceRegistries = sd_client::ProvidedServiceRegistries;
  using ProvidedServiceRegistry = sd_client::ProvidedServiceRegistry;
  using RequiredServiceRegistry = sd_client::RequiredServiceRegistry;

  using RequiredEntry = RequiredServiceRegistry::Entry;
  using ProvidedEntry = ProvidedServiceRegistry::Entry;

  using RequiredEntryValue = RequiredServiceRegistry::EntryValue;
  using ProvidedEntryValue = ProvidedServiceRegistry::EntryValue;

  using ConstGetAndReturnEntry = ProvidedServiceRegistries::ConstGetAndReturnEntry;
  using ConstProvidedMatchAndGetReturnEntry = ProvidedServiceRegistries::ConstMatchAndGetReturnEntry;
  using ProvidedMatchAndGetReturnEntry = ProvidedServiceRegistries::MatchAndGetReturnEntry;
  using ConstProvidedMatchAndGetReturn = ProvidedServiceRegistries::ConstMatchAndGetReturn;
  using ProvidedMatchAndGetReturn = ProvidedServiceRegistries::MatchAndGetReturn;
  using ProvidedMatchReturn = ProvidedServiceRegistries::MatchReturn;

  using ConstRequiredMatchAndGetReturnEntry = RequiredServiceRegistry::ConstMatchAndGetReturnEntry;
  using RequiredMatchAndGetReturnEntry = RequiredServiceRegistry::MatchAndGetReturnEntry;
  using ConstRequiredMatchAndGetReturn = RequiredServiceRegistry::ConstMatchAndGetReturn;
  using RequiredMatchAndGetReturn = RequiredServiceRegistry::MatchAndGetReturn;

  /*!
   * \brief Create a SdClient.
   * \param[in] reactor The reactor must be valid as long as this IpcClient instance exists.
   * \param[in] timer_manager The timer manager must be valid as long as this IpcClient instance exists.
   * \param[in] binding_type The binding type (ZeroCopy or Ipc).
   * \context ANY
   * \pre
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  SdClient(external::internal::osabstraction::Reactor1Interface& reactor,
           external::internal::osabstraction::TimerManagerInterface& timer_manager, BindingType binding_type) noexcept;

  /*!
   * \brief Destroys the IpcClient and releases all system resources.
   * \details After a close call, there may still be ongoing asynchronous operations and timers. The caller must
   *          assert that all pending asynchronous operations are finished or that they are not executed concurrently.
   *          One way to assert this is by moving the cleanup code into the reactor thread context.
   * \context ANY
   * \pre No ongoing asynchronous operation of the reactor. This is not checked.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ~SdClient() noexcept;

  /*! Delete default constructor, copy and move constructors and operators. */
  SdClient() = delete;
  SdClient(SdClient const&) = delete;
  SdClient(SdClient&&) = delete;
  auto operator=(SdClient const&) -> SdClient& = delete;
  auto operator=(SdClient&&) -> SdClient& = delete;

  /*!
   * \brief Start the asynchronous connection establishment to the SdServer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientConnect
   */
  void Connect() noexcept;

  /*!
   * \brief Disconnect from the SdServer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientClose
   */
  void Close() noexcept;

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
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientOfferService
   */
  auto OfferService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier,
                    UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Stop offering a service instance to other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated.
   *          See the returned error codes for a detailed description of the protocol violation.
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
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientStopOfferService
   */
  auto StopOfferService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier,
                        UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Listen to a service instance that may be offered by other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated.
   *          See the returned error codes for a detailed description of the protocol violation.
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
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  auto ListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Listen to a service instance that may be offered by other IpcServiceDiscovery participants and
            call the given callback once any updates for the listened service instance are received.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated.
   *          See the returned error codes for a detailed description of the protocol violation.
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
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  auto ListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier,
                     ListenServiceCallback listen_service_callback) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Stop listening to a service instance that may be offered by other IpcServiceDiscovery participants.
   * \details The update will be ignored and the user informed should the IpcServiceDiscovery protocol be violated.
   *          See the returned error codes for a detailed description of the protocol violation.
   * \param[in] required_service_instance_identifier ServiceInstanceIdentifier for the service instance the user
   *                                                 wants to stop listening to. May contain any wildcard values.
   * \return Success if the update is valid or an error containing the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNotRequired     The given ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired   The given ServiceInstanceIdentifier was never required.
   * \context ANY
   * \pre Shall only be executed outside of any ListenServiceCallback. This is not checked.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientStopListenService
   */
  auto StopListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Poll a service instance that may be offered by other IpcServiceDiscovery participants.
   * \details The user needs to ensure to have called ListenService for the same required service instance
   *          with or without an attached callback for a call to this function to make sense.
   *          Otherwise, an error will be returned.
   * \param[in] required_service_instance_identifier ServiceInstanceIdentifier for the service instance the user
   *                                                 wants to poll. May contain any wildcard values.
   * \return A vector containing all matching provided endpoints on success or an error containing
   *         the reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNotRequired    A matching ServiceInstanceIdentifier is currently not required.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  A matching ServiceInstanceIdentifier has never been required.
   * \context ANY
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientPollService
   */
  auto PollService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept
      -> ::ara::core::Result<::ara::core::Vector<ServiceInstanceEndpoint>>;

  /*!
   * \brief Poll a service instance that may be offered by other IpcServiceDiscovery participants.
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
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientPollService
   */
  auto PollService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept
      -> ::ara::core::Result<ara::core::Optional<UnicastAddress>>;

  /*!
   * \brief Check if the SdClient is in use.
   * \detail Check if the underlying IpcClient is in use.
   * \return true if the SdClient is in use, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientIsInUse
   */
  auto IsInUse() const noexcept -> bool;

 private:
  /*! Internal states. */
  enum class State : std::uint8_t {
    kNotRegistered, /*!< SdClient is not registered by SdServer. InitMessage was not sent to the SdServer. */
    kRegistered,    /*!< SdClient is registered by SdServer. InitMessage was sent and accepted by the SdServer. */
  };

  /*!
   * \brief Send a message to the IpcServer resp. SdServer with the underlying IpcClient
   *      only if the SdClient is registerd by the SdServer.
   * \tparam Message The type of the message.
   * \param[in] message The message to send.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  template <class Message>
  void SendMessageIfRegistered(Message const& message) noexcept;

  /*!
   * \brief Send a message to the IpcServer resp. SdServer with the underlying IpcClient
   *        and handle all possible returned errors.
   * \tparam Message The type of the message.
   * \param[in] message The message to send.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  template <class Message>
  void SendMessage(Message const& message) noexcept;

  /*!
   * \brief Send the provided service instances of the local ProvidedRegistry and
   *       the required service of the RequiredRegistry to the server.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void SendLocalProvidedAndRequiredRegistry() noexcept;

  /*!
   * \brief Send the provided service instances of the local ProvidedRegistry to the server.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void SendLocalProvidedRegistry() noexcept;

  /*!
   * \brief Send the required service of the RequiredRegistry to the server.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void SendRequiredRegistry() noexcept;

  /*!
   * \brief Trigger a reconnect to the server (SdServer resp. IpcServer).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void Reconnect() noexcept;

  /*!
   * \brief Append the matching required service requests from the given ProvidedServiceRegistry to the given vector.
   * \param[in,out] service_instance_endpoints The vector where to append the matching endpoints.
   * \param[in] provided_service_registry The ProvidedServiceRegistry to match against.
   * \param[in] required_service_instance_identifier The required ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static void AppendMatchingProvidedServiceInstanceEndPoints(
      ::ara::core::Vector<ServiceInstanceEndpoint>& service_instance_endpoints,
      ProvidedServiceRegistry const& provided_service_registry,
      RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept;

  /*!
   * \brief Get a reference to a provided service instance by checking in local and remote provided registries.
   * \param[in] provided_service_instance_identifier The provided ServiceInstanceIdentifier.
   * \return A result containing the const reference to the service instance on success or an error containing the
             reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNeverProvided  A matching ServiceInstanceIdentifier has never been provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetProvidedServiceInstance(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier)
      const noexcept -> ::ara::core::Result<ConstGetAndReturnEntry>;

  /*!
   * \brief Get a reference to a provided service instance.
   * \param[in] provided_service_registry The ProvidedServiceRegistry to search in.
   * \param[in] provided_service_instance_identifier The provided ServiceInstanceIdentifier.
   * \return A result containing the const reference to the service instance on success or an error containing the
             reason for rejection.
   * \error IpcServiceDiscoveryErrc::kNeverProvided  A matching ServiceInstanceIdentifier has never been provided.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto GetProvidedServiceInstanceInternal(
      ProvidedServiceRegistry const& provided_service_registry,
      ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) noexcept
      -> ::ara::core::Result<ConstGetAndReturnEntry>;

  /*!
   * \brief Called if a OfferServiceMessage was received and the message is valid (not corrupted).
   * \param[in] offer_service_message The received OfferServiceMessage.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  The given ServiceInstanceIdentifier was never required.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  auto OnOfferServiceMessageValidMessage(OfferServiceMessage const& offer_service_message) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Called if a StopOfferServiceMessage was received and the message is valid (not corrupted).
   * \param[in] stop_offer_service_message The received StopOfferServiceMessage.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  The given ServiceInstanceIdentifier was never required
   * \error IpcServiceDiscoveryErrc::kNotProvided    The given ServiceInstanceIdentifier is not provided.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  auto OnStopOfferServiceMessageValidMessage(StopOfferServiceMessage const& stop_offer_service_message) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Called if an OfferServiceMessage was received and the service is still required.
   * \details This function triggers the update of the remote ProvidedServiceRegistry and
   *          calls the Callbacks of the required matching service requests.
   * \param[in] service_instance_identifier The ServiceInstanceIdentifier of the remote not provided service instance.
   * \param[in] unicast_address The UnicastAddress of the remote not provided service instance.
   * \param[in] required_matches The required service requests (in required state) matching to the service instance.
   * \error IpcServiceDiscoveryErrc::kAlreadyProvided                  The given ServiceInstanceIdentifier was already
   *                                                                   provided (only if protocol validation is
   *                                                                   enabled).
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier was
   *                                                                   already provided but with a different
   *                                                                   unicast address (only if protocol validation
   *                                                                   is enabled).
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  auto OnOfferServiceMessageServiceStillRequired(ProvidedServiceInstanceIdentifier const& service_instance_identifier,
                                                 UnicastAddress const& unicast_address,
                                                 RequiredMatchAndGetReturn const& required_matches) noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Called if a StopOfferServiceMessage was received and the service is still required.
   * \details This function triggers the update of the remote ProvidedServiceRegistry and
   *          calls the Callbacks of the required matching service requests.
   * \param[in] service_instance_identifier The ServiceInstanceIdentifier of the remote provided service instance.
   * \param[in] unicast_address The UnicastAddress of the remote provided service instance.
   * \param[in] required_matches The required service requests (in required state) matching to the service instance.
   * \error IpcServiceDiscoveryErrc::kProvidedDifferentEndpoint        The given ServiceInstanceIdentifier is
   *                                                                   provided but with a different unicast address.
   * \error IpcServiceDiscoveryErrc::kNotProvided                      The given ServiceInstanceIdentifier is
   *                                                                   currently not provided.
   * \error IpcServiceDiscoveryErrc::kNeverProvided                    The given ServiceInstanceIdentifier was
   *                                                                   never provided.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  auto OnStopOfferServiceMessageServiceStillRequired(
      ProvidedServiceInstanceIdentifier const& service_instance_identifier, UnicastAddress const& unicast_address,
      RequiredMatchAndGetReturn const& required_matches) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Called if a OfferServiceMessage or StopOfferServiceMessage was received but the service
   *        is no longer required.
   * \details If the service is no longer required, the message will be discarded. This is not a protocol error.
   * \tparam ServiceMessage The type of the message (OfferServiceMessage or StopOfferServiceMessage).
   * \param[in] service_message Received OfferServiceMessage or StopOfferServiceMessage.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  template <typename ServiceMessage>
  void OnServiceMessageServiceNoLongerRequired(ServiceMessage const& service_message) const noexcept;

  /*!
   * \brief Called if a OfferServiceMessage or StopOfferServiceMessage was received,
   *        but the service has been provided locally before.
   * \details If the service has been provided locally before, the message will be discarded.
   *          This is not a protocol error. This only means the user has requested a service,
   *          which was provided locally.
   * \tparam ServiceMessage The type of the message (OfferServiceMessage or StopOfferServiceMessage).
   * \param[in] service_message Received OfferServiceMessage or StopOfferServiceMessage.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  template <typename ServiceMessage>
  void OnServiceMessageServiceWasPreviouslyProvidedLocally(ServiceMessage const& service_message) const noexcept;

  /*!
   * \brief Called if a OfferServiceMessage or StopOfferServiceMessage was received,
   *        but the remote ProvidedRegistry is already up-to-date.
   * \details If the service has been provided locally before, the message will be discarded.
   *          This is not a protocol error.
   * \tparam ServiceMessage The type of the message (OfferServiceMessage or StopOfferServiceMessage).
   * \param[in] service_message Received OfferServiceMessage or StopOfferServiceMessage.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  template <typename ServiceMessage>
  void OnOfferServiceMessageNoUpdateOfRemoteRegistry(ServiceMessage const& service_message) const noexcept;

  /*!
   * \brief Called if a OfferServiceMessage or StopOfferServiceMessage was received,
   *        but the processing of the message ends up in an error.
   * \details This function handles and maps the different processing errors.
   * \tparam ServiceMessage The type of the message (OfferServiceMessage or StopOfferServiceMessage).
   * \param[in] service_message Received OfferServiceMessage or StopOfferServiceMessage.
   * \param[in] error_code The ErrorCode of the processing.
   * \error IpcServiceDiscoveryErrc::kNeverRequired  The given ServiceInstanceIdentifier was never required
   *                                                 (for OfferServiceMessage and StopOfferServiceMessage).
   * \error IpcServiceDiscoveryErrc::kNotProvided    The given ServiceInstanceIdentifier is not provided
   *                                                 (only for StopOfferServiceMessage).
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  template <typename ServiceMessage>
  auto HandleOnServiceMessageValidMessageError(ServiceMessage const& service_message,
                                               ::ara::core::ErrorCode error_code) const noexcept
      -> ::ara::core::Result<void>;

  /*!
   * \brief Called if a OfferServiceMessage or StopOfferServiceMessage was received,
   *        which leads to a protocol violation.
   * \details This function triggers a reconnect to the SdServer because of the protocol violation.
   * \param[in] error_code The ErrorCode of the protocol violation.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  void HandleOnServiceMessageProtocolError(::ara::core::ErrorCode error_code) noexcept;

  /*!
   * \brief Called if an unexpected message (InitMessage, ListenServiceMessage, StopListenServiceMessage) was received.
   * \details An unexpected message leads to a protocol violation and a reconnect to the server is triggered.
   * \tparam Message The type of the message (InitMessage, ListenServiceMessage and StopListenServiceMessage).
   * \param[in] message_result Result containing either the parsed message or an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  template <typename Message>
  void HandleUnexpectedMessage(::ara::core::Result<Message> const& message_result) noexcept;

  /*!
   * \brief Called on connected to IpcServer.
   * \details
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void OnConnected() noexcept;

  /*!
   * \brief Called on disconnected from IpcServer.
   * \param[in] close_connection_cause A result containing either a value reporting the reason of the disconnect
   *                                   or an error if caused by an unexpected error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void OnDisconnected(CloseConnectionCause close_connection_cause) noexcept;

  /*!
   * \brief Called on a message received from IpcServer.
   * \param[in] buffer_view The view to the received serialized message.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientCommunication
   */
  void OnMessageReceived(ConstBufferConstView& buffer_view) noexcept;

  /*!
   * \brief Called upon reception of an OfferServiceMessage.
   * \details If the offer is valid, the remote ProvidedServiceRegistry is updated and the ListenServiceCallback is
   *          called for matching service requests. Otherwise, the server has violated the protocol and a reconnect
   *          will be triggered.
   * \param[in] offer_service_message_result Result containing either the parsed OfferServiceMessage or an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  void OnOfferServiceMessage(
      ::ara::core::Result<OfferServiceMessage> const& offer_service_message_result) noexcept final;

  /*!
   * \brief Called upon reception of an StopOfferServiceMessage.
   * \details If the offer is valid, the remote ProvidedServiceRegistry is updated and the ListenServiceCallback is
   *          called for matching service requests. Otherwise, the server has violated the protocol and a reconnect
   *          will be triggered.
   * \param[in] stop_offer_service_message_result Result containing either the parsed StopOfferServiceMessage
   *                                              or an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  void OnStopOfferServiceMessage(
      ::ara::core::Result<StopOfferServiceMessage> const& stop_offer_service_message_result) noexcept final;

  /*!
   * \brief Called upon reception of an ListenServiceMessage.
   * \details The reception of this message will violate the protocol, and a reconnect to the server will be triggered.
   * \param[in] listen_service_message_result Result containing either the parsed ListenServiceMessage or an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  void OnListenServiceMessage(
      ::ara::core::Result<ListenServiceMessage> const& listen_service_message_result) noexcept final;

  /*!
   * \brief Called upon reception of a StopListenServiceMessage.
   * \details The reception of this message will violate the protocol, and a reconnect to the server will be triggered.
   * \param[in] stop_listen_service_message_result Result containing either the parsed StopListenServiceMessage
   *                                               or an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  void OnStopListenServiceMessage(
      ::ara::core::Result<StopListenServiceMessage> const& stop_listen_service_message_result) noexcept final;

  /*!
   * \brief Called upon reception of a InitMessage.
   * \details The reception of this message will violate the protocol, and a reconnect to the server will be triggered.
   * \param[in] init_message_result Result containing either the parsed InitMessage or an error.
   * \context REACTOR
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-ProtocolServerToClient
   */
  void OnInitMessage(::ara::core::Result<InitMessage> const& init_message_result) noexcept final;

  /*!
   * \brief Call the given ListenServiceCallback with the given required ServiceInstanceIdentifier
   *        for all given provided service instance matches.
   * \details The mutex is not held during the ListenServiceCallback call.
   * \param[in] listen_service_callback The ListenServiceCallback to be called for every matching
   *                                    provided service instance.
   * \param[in] provided_matches All provided service instances (in provided state) matching
   *                             to the required ServiceInstanceIdentifier.
   * \param[in] required_service_instance_identifier The required ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  void CallListenServiceCallbackForServiceInstances(
      ListenServiceCallback& listen_service_callback, ConstProvidedMatchAndGetReturn const& provided_matches,
      RequiredServiceInstanceIdentifier required_service_instance_identifier) const noexcept;

  /*!
   * \brief Call the given ListenServiceCallback for all provided service instance matches of the given
   *        ProvidedServiceRegistry.
   * \param[in] listen_service_callback The ListenServiceCallback to be called for every matching service instance.
   * \param[in] provided_service_registry The ProvidedServiceRegistry for all provided service instance matches
   *                                      the ListenServiceCallback should be called.
   * \param[in] required_service_instance_identifier The required ServiceInstanceIdentifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  void CallListenServiceCallbackForServiceInstances(
      ListenServiceCallback& listen_service_callback, ProvidedServiceRegistry const& provided_service_registry,
      RequiredServiceInstanceIdentifier required_service_instance_identifier) const noexcept;

  /*!
   * \brief Call the ListenServiceCallback of all given required matching service requests with the given
   *        service instance (ServiceInstanceEndpoint and ProvidedState).
   * \details The mutex is not held during the ListenServiceCallback call.
   * \param[in] required_matches The required service requests (in required state) matching to the service instance.
   * \param[in] service_instance_endpoint The ServiceInstanceEndpoint of the matching service instance.
   * \param[in] provided_state The ProvidedState of the matching service instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  void CallListenServiceCallbacksForServiceInstance(RequiredMatchAndGetReturn const& required_matches,
                                                    ServiceInstanceEndpoint service_instance_endpoint,
                                                    ProvidedState provided_state) const noexcept;

  /*!
   * \brief Call the ListenServiceCallback with the given service instance of all required service requests matching
   *        to the given provided service instance (ServiceInstanceEndpoint and ProvidedState).
   * \param[in] provided_service_instance_identifier The ServiceInstanceIdentifier of the matching service instance.
   * \param[in] service_instance_endpoint The ServiceInstanceEndpoint of the matching service instance.
   * \param[in] provided_state The ProvidedState of the matching service instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  void CallListenServiceCallbacksForServiceInstance(
      ProvidedServiceInstanceIdentifier provided_service_instance_identifier,
      ServiceInstanceEndpoint service_instance_endpoint, ProvidedState provided_state) noexcept;

  /*!
   * \brief Call the ListenServiceCallback for all required service requests matching to every
   *        provided service instance inside the given provided_service_registry.
   * \param[in] provided_service_registry The ProvidedServiceRegistry for all provided service instance matches
   *                                      the ListenServiceCallback should be called.
   * \param[in] provided_state The ProvidedState of the matching service instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \trace DSGN-IpcServiceDiscovery-Centralized-SdClientListenService
   */
  void CallListenServiceCallbacksForServiceInstances(ProvidedServiceRegistry const& provided_service_registry,
                                                     ProvidedState provided_state) noexcept;

  /*! A mutex used to make the service registry access exclusive. */
  mutable std::mutex read_mutex_{};
  mutable std::mutex write_mutex_{};

  /*! IpcClient connected callback. */
  // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
  IpcClient::ConnectedCallback connected_callback_{[this]() { OnConnected(); }};

  /*! IpcClient disconnected callback. */
  // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
  IpcClient::DisconnectedCallback disconnected_callback_{[this](CloseConnectionCause close_connection_cause) {
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
    OnDisconnected(close_connection_cause);
  }};

  /*! IpcClient receive message callback. */
  IpcClient::ReceiveMessageCallback receive_message_callback_{
      // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION_CALLBACKS
      [this](ConstBufferConstView& buffer_view) { OnMessageReceived(buffer_view); }};

  /*! The BindingType kZeroCopy or kIpc. */
  BindingType binding_type_;

  /*! IpcClient. */
  IpcClient ipc_client_;

  /*! Logger for the SdClient unit. */
  Logger logger_{Constants::kLoggerPrefixSdClient};

  /*! ProvidedServiceRegistries holding the local and remote ProvidedServiceRegistry. */
  ProvidedServiceRegistries provided_service_registries_{};

  /*! RequiredServiceRegistry holding the required services. */
  RequiredServiceRegistry required_service_registry_{};

  /*! Internal state */
  State state_{State::kNotRegistered};
};

/*!
 * \exclusivearea   ::amsr::ipc_service_discovery::internal::SdClient::read_mutex_
 *                  ::amsr::ipc_service_discovery::internal::SdClient::write_mutex_
 *                  Ensures consistency and exclusive access of while reading/modifying the service registries.
 * \protects        ::amsr::ipc_service_discovery::internal::SdClient::provided_service_registries_
 *                  ::amsr::ipc_service_discovery::internal::SdClient::required_service_registry_
 * \usedin          ::amsr::ipc_service_discovery::internal::SdClient::OfferService
 *                  ::amsr::ipc_service_discovery::internal::SdClient::StopOfferService
 *                  ::amsr::ipc_service_discovery::internal::SdClient::ListenService
 *                  ::amsr::ipc_service_discovery::internal::SdClient::StopListenService
 *                  ::amsr::ipc_service_discovery::internal::SdClient::PollService
 *                  ::amsr::ipc_service_discovery::internal::SdClient::OnConnected
 *                  ::amsr::ipc_service_discovery::internal::SdClient::OnDisconnected
 *                  ::amsr::ipc_service_discovery::internal::SdClient::OnMessageReceived
 *                  ::amsr::ipc_service_discovery::internal::SdClient::StopOfferService
 * \exclude         All other public API methods.
 * \length          LONG
 *                  Send/Receive messages and calling callbacks while mutex locked.
 *                  Depth call-tree > 1.
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_SD_CLIENT_SD_CLIENT_H_
