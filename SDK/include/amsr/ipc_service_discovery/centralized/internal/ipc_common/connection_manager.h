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
 *  \brief Class for accessing a SafeIpc connection and managing buffers for it.
 *  \unit IpcServiceDiscovery::Centralized::IpcCommon
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_COMMON_CONNECTION_MANAGER_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_COMMON_CONNECTION_MANAGER_H_

#include "amsr/ipc_service_discovery/centralized/internal/ipc_common/receive_buffer_container.h"
#include "amsr/ipc_service_discovery/common/internal/object_pool/object_pool.h"
#include "amsr/ipc_service_discovery/external/internal/libosabstraction/connection.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Class for accessing a SafeIpc connection and managing buffers for it.
 * \details The ConnectionManager owns a SafeIpc Connection object and provides ReceiveBufferContainer for the reception
 *          of SafeIpc messages. The buffers are drawn from an ObjectPool, to which the ConnectionManager has a
 *          reference.
 * \vprivate Component Private
 */
class ConnectionManager final {
 public:
  /*!
   * \brief Object pool for receive buffer containers.
   */
  using ReceiveBufferPool = ObjectPool<ReceiveBufferContainer>;

  /*!
   * \brief Smart pointer type for receive buffers in the ReceiveBufferPool.
   */
  using ReceiveBufferContainerPtr = ReceiveBufferPool::ObjectUniquePtr;

  /*!
   * \brief Create a new ConnectionManager.
   * \param[in] connection A rvalue reference to the SafeIpc connection. The constructor will take ownership of the
   *                       connection, and the connection is only destroyed once the ConnectionManager is destroyed.
   * \param[in] buffer_pool A reference to a ReceiveBufferPool holding ReceiveBufferContainer. The user has to ensure
   *                        that the ReceiveBufferPool is alive and valid for the entirety of the ConnectionManager's
   *                        lifetime.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Initialize all members.
   * \endinternal
   */
  ConnectionManager(external::internal::osabstraction::Connection&& connection, ReceiveBufferPool& buffer_pool) noexcept
      // VCA_IPCSERVICEDISCOVERY_SPC_15_CONNECTION_MOVE_CONSTRUCTOR
      : connection_{std::move(connection)}, buffer_pool_{buffer_pool} {}

  ConnectionManager() = delete;
  ConnectionManager(ConnectionManager const& other) = delete;
  // VCA_IPCSERVICEDISCOVERY_SPC_15_CONNECTION_MOVE_CONSTRUCTOR
  ConnectionManager(ConnectionManager&& other) noexcept = default;
  auto operator=(ConnectionManager const& other) -> ConnectionManager& = delete;
  auto operator=(ConnectionManager&& other) -> ConnectionManager& = delete;
  // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_THIS_OBJECT
  ~ConnectionManager() noexcept {
    // VCA_IPCSERVICEDISCOVERY_LNG_03_CALLING_NONSTATIC_METHOD_ON_CONNECTION
    if (connection_.IsInUse()) {
      Abort("Not able to destruct while connection still in use.");  // COV_IpcSD_utility
    }
    // VCA_IPCSERVICEDISCOVERY_SPC_15_LNG_03_CONNECTION_DESTRUCTOR, VCA_IPCSERVICEDISCOVERY_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT
  }

  /*!
   * \brief Obtain a reference to the managed SafeIpc connection.
   * \return A reference to the managed SafeIpc connection.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Return the managed connection.
   * \endinternal
   */
  auto GetConnection() noexcept -> external::internal::osabstraction::Connection& { return connection_; }

  /*!
   * \brief Obtain a reference to the acquired ReceiveBufferContainer.
   * \details The ReceiveBufferContainer needs to be acquired before calling this function. This function aborts
   *          if it has not been acquired beforehand.
   * \return A reference to the acquired ReceiveBufferContainer.
   * \context ANY
   * \pre AcquireBufferContainer() has to be called to acquire the buffer.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - If the buffer pointer is nullptr, abort.
   *  - Return the ReceiveBufferContainer otherwise.
   * \endinternal
   */
  auto GetBufferContainer() const noexcept -> ReceiveBufferContainer& {
    if (buffer_container_ptr_ == nullptr) {
      Abort("Buffer was not acquired. Precondition violated.");  // COV_IpcSD_utility
    }
    // VCA_IPCSERVICEDISCOVERY_LNG_02_VALID_BUFFER_CONTAINER_POINTER
    return *buffer_container_ptr_;
  }

  /*!
   * \brief Acquire a buffer from the ReceiveBufferPool.
   * \return A reference to the acquired ReceiveBufferContainer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - Release the buffer if there was one acquired already.
   *  - Acquire the buffer.
   *  - Return the buffer.
   * \endinternal
   */
  auto AcquireBufferContainer() noexcept -> ReceiveBufferContainer& {
    ReleaseBufferContainer();
    buffer_container_ptr_ = buffer_pool_.Acquire();
    // VCA_IPCSERVICEDISCOVERY_LNG_02_VALID_BUFFER_CONTAINER_POINTER
    return *buffer_container_ptr_;
  }

  /*!
   * \brief Release the acquired buffer and return it to the ReceiveBufferPool. This function has no effect
   *        if there was no buffer acquired before.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \internal
   *  - If the buffer pointer is not nullptr, reset the unique pointer.
   * \endinternal
   */
  void ReleaseBufferContainer() noexcept {
    if (buffer_container_ptr_ != nullptr) {
      buffer_container_ptr_.reset();
    }
  }

 private:
  /*! The managed SafeIpc connection. */
  external::internal::osabstraction::Connection connection_;

  /*! The reference to the ReceiveBufferPool. */
  ReceiveBufferPool& buffer_pool_;

  /*! The unique pointer to the acquired ReceiveBufferContainer. */
  ReceiveBufferContainerPtr buffer_container_ptr_{buffer_pool_.CreateEmptyObjectUniquePtr()};

  /*! Friend declarations for testing purposes. */
  FRIEND_TEST(UT__IpcCommon__ConnectionManager, GetConnection);
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_IPC_COMMON_CONNECTION_MANAGER_H_
