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
 *  \brief Binding of the centralized backend to the IpcServiceDiscovery interface.
 *  \unit IpcServiceDiscovery::UserInterface::CentralizedBinding
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_INTERNAL_IPC_SERVICE_DISCOVERY_CENTRALIZED_H_
#define LIB_IPC_SERVICE_DISCOVERY_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_INTERNAL_IPC_SERVICE_DISCOVERY_CENTRALIZED_H_

#include <mutex>
#include "amsr/ipc_service_discovery/centralized/internal/sd_client/sd_client.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "amsr/ipc_service_discovery/common/internal/types/types.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/reactor1_interface.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/timer_manager_interface.h"
#include "amsr/ipc_service_discovery/internal/ipc_service_discovery_interface.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"

namespace amsr {
namespace ipc_service_discovery {
namespace internal {

/*!
 * \brief Binding for the implementation for the IpcServiceDiscovery using the centralized backend.
 * \vprivate Product Private
 * \trace DSGN-IpcServiceDiscovery-CommunicationTransparency
 */
class IpcServiceDiscoveryCentralized final : public IpcServiceDiscoveryInterface {
 public:
  /*!
   * \brief Constructor for the IpcServiceDiscovery using the centralized backend.
   * \param[in] binding_type  BindingType for the IpcServiceDiscovery.
   * \param[in] reactor       Reference to the reactor the service discovery should use. Must be valid for the entire
   *                          lifetime of the IpcServiceDiscovery.
   * \param[in] timer_manager Reference to the timer manager the service discovery should use.
   *                          Must be valid for the entire lifetime of the IpcServiceDiscovery.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  IpcServiceDiscoveryCentralized(BindingType binding_type,
                                 external::internal::osabstraction::Reactor1Interface& reactor,
                                 external::internal::osabstraction::TimerManagerInterface& timer_manager) noexcept;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::~IpcServiceDiscoveryInterface
   */
  ~IpcServiceDiscoveryCentralized() noexcept final;

  IpcServiceDiscoveryCentralized(IpcServiceDiscoveryCentralized const&) = delete;
  IpcServiceDiscoveryCentralized(IpcServiceDiscoveryCentralized&&) noexcept = delete;
  auto operator=(IpcServiceDiscoveryCentralized const&) & -> IpcServiceDiscoveryCentralized& = delete;
  auto operator=(IpcServiceDiscoveryCentralized&&) & noexcept -> IpcServiceDiscoveryCentralized& = delete;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::Initialize
   */
  void Initialize() noexcept final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::Deinitialize
   */
  void Deinitialize() noexcept final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::OfferService
   */
  auto OfferService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier,
                    UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::StopOfferService
   */
  auto StopOfferService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier,
                        UnicastAddress const& unicast_address) noexcept -> ::ara::core::Result<void> final;
  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::ListenService(RequiredServiceInstanceIdentifier const&)
   */
  auto ListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept
      -> ::ara::core::Result<void> final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::ListenService(RequiredServiceInstanceIdentifier const&, ListenServiceCallback)
   */
  auto ListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier,
                     ListenServiceCallback listen_service_callback) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::StopListenService
   */
  auto StopListenService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) noexcept
      -> ::ara::core::Result<void> final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::PollService(RequiredServiceInstanceIdentifier const&) const
   */
  auto PollService(RequiredServiceInstanceIdentifier const& required_service_instance_identifier) const noexcept
      -> ::ara::core::Result<::ara::core::Vector<ServiceInstanceEndpoint>> final;

  /*!
   * \copydoc ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryInterface::PollService(ProvidedServiceInstanceIdentifier const&) const
   */
  auto PollService(ProvidedServiceInstanceIdentifier const& provided_service_instance_identifier) const noexcept
      -> ::ara::core::Result<ara::core::Optional<UnicastAddress>> final;

 private:
  /*!
   * \brief State to track if IpcServiceDiscovery has been initialized.
   */
  enum class State : std::uint8_t {
    kUninitialized = 0, /*!< Uninitialized. */
    kInitialized = 1    /*!< Initialized. */
  };

  /*! Reference to the reactor */
  external::internal::osabstraction::Reactor1Interface& reactor_;

  /*! Reference to the timer manager */
  external::internal::osabstraction::TimerManagerInterface& timer_manager_;

  /*! Mutex to protect internal state */
  mutable std::mutex mutex_{};

  /*! Internal state */
  State state_{State::kUninitialized};

  /*! Optional containing the SdClient of the centralized backend */
  ::ara::core::Optional<centralized::internal::SdClient> sd_client_opt_{};

  /*! Abort message for initialized precondition violation. */
  static constexpr char const* kDeathInitialized{"IpcServiceDiscovery initialized. Precondition violated."};

  /*! Abort message for uninitialized precondition violation. */
  static constexpr char const* kDeathNotInitialized{"IpcServiceDiscovery not initialized. Precondition violated."};

  /*! Abort message for deinitialization within callback precondition violation. */
  static constexpr char const* kDeathDeinitializeWithinCallback{
      "Deinitialize called within a callback that was provided to the IpcServiceDiscovery. Precondition violated."};
};

/*!
 * \exclusivearea   ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::mutex_
 *                  Ensures consistency while reading/modifying the internal state.
 * \protects        ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::state_
 * \usedin
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::~IpcServiceDiscoveryCentralized
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::Initialize
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::Deinitialize
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::OfferService
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::StopOfferService
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::ListenService
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::StopListenService
 *                  ::amsr::ipc_service_discovery::internal::IpcServiceDiscoveryCentralized::PollService
 * \length          LONG
 *                  Send messages and calling callbacks when mutex locked.
 *                  Depth call-tree > 1.
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_INTERNAL_IPC_SERVICE_DISCOVERY_CENTRALIZED_H_
