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
/*!        \file  amsr/someip_binding_core/internal/server_manager.h
 *        \brief  ara::com SOME/IP Binding Server Manager.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::ServerManager
 *
 *      \details  ServerManager serves as an anchor between the SOME/IP protocol via POSIX and the
 *                ara::com related high-level layers.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_MANAGER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/request_no_return.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_handler_interface.h"
#include "amsr/someip_binding_core/internal/polling_utilities.h"
#include "amsr/someip_binding_core/internal/server_handler_interface.h"
#include "amsr/someip_binding_core/internal/server_manager_interface.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_server_error.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/message_verification.h"

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Realizes a concrete Binding for the skeleton communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
class ServerManager final : public ServerManagerInterface, public ServerHandlerInterface {
 public:
  ServerManager(ServerManager const&) = delete;
  ServerManager(ServerManager&&) = delete;
  ServerManager& operator=(ServerManager const&) & = delete;
  ServerManager& operator=(ServerManager&&) & = delete;

  /*!
   * \brief Constructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  ServerManager();
  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ServerManager() noexcept final = default;

  /*!
   * \brief       Registers one service binding on construction of a ServiceSkeletonXf.
   * \param[in]   psid Uniquely identify a service binding with a service id, instance id, major_version,
   *              minor_version.
   *              On method request / event request, a look-up through the registered skeleton bindings happens, to
   *              delegate the request to the correct service binding object.
   * \param[in]   server The service provider.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RegisterProvidedServiceInstance(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& psid,
      ::amsr::someip_binding_core::internal::methods::SkeletonMethodHandlerInterface& server) final;

  /*!
   * \brief       Actively de-register one service binding (e.g. after StopOfferService gets called).
   * \param[in]   psid The key to search for and the associated value to erase from the list.
   * \pre         service is already registered
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady      FALSE
   * \trace SPEC-10144401
   */
  void DeregisterProvidedServiceInstance(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& psid) noexcept final;

 private:
  /*!
   * \brief Registered server binding objects. This back-link is needed for the routing of method requests,
   * event requests.
   */
  using ServersMap = std::map<::amsr::someip_binding_core::internal::ProvidedServiceInstanceId,
                              ::amsr::someip_binding_core::internal::methods::SkeletonMethodHandlerInterface&>;

  /*!
   * \brief Error message allocator.
   */
  FlexibleUniqueMemoryBufferAllocator error_message_memory_buffer_allocator_{};

  /*!
   * \copydoc amsr::someip_binding_core::internal::ServerHandlerInterface::OnRequest
   * \trace SPEC-10144318
   */
  ::amsr::core::Result<void> OnRequest(
      ::amsr::someip_binding_core::internal::methods::Request&& request) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::ServerHandlerInterface::OnRequestNoReturn
   * \trace SPEC-10144318
   */
  ::amsr::core::Result<void> OnRequestNoReturn(
      ::amsr::someip_binding_core::internal::methods::RequestNoReturn&& request) noexcept final;

  /*!
   * \brief Map of service providers.
   */
  ServersMap servers_map_{};

  /*!
   * \brief Lock that protects the map of service providers.
   */
  std::mutex servers_map_lock_{};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_MANAGER_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::ServerManager::servers_map_lock_
 *                Ensures that a server is only stored in the map if it is currently offered.
 *
 * \protects ::amsr::someip_binding_core::internal::ServerManager::servers_map_
 *
 * \usedin  ::amsr::someip_binding_core::internal::ServerManager::RegisterProvidedServiceInstance
 *          ::amsr::someip_binding_core::internal::ServerManager::DeregisterProvidedServiceInstance
 *          ::amsr::someip_binding_core::internal::ServerManager::HandleMethodRequest
 *
 * \length  MEDIUM Limited to a map lookup, an emplacement if a server is added to the map or
 *                 a removal of a server entry from the map.
 *                 The exclusive area starts and ends always in the smallest possible scope within the protected
 *                 methods. This exclusive area covers calls to at most one method. The complexity is logarithmic
 *                 in the size of the container.
 * \endexclusivearea
 */
