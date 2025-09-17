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
/*!        \file  required_service_instance_registry.h
 *        \brief  This file contains the definition of the RequiredServiceInstanceRegistry class.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_REGISTRY_REQUIRED_SERVICE_INSTANCE_REGISTRY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_REGISTRY_REQUIRED_SERVICE_INSTANCE_REGISTRY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/configuration/types/service_interface_deployment.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_registry {

/*!
 * \brief The KeyedServiceRegistry class registers, deregisters and looks up registered instance id's with a key.
 * \details The Key type is expected to have the equivalence operator implemented.
 */
class RequiredServiceInstanceRegistry final {
 public:
  /*!
   * \brief Default constructor.
   *
   * \steady FALSE
   */
  RequiredServiceInstanceRegistry() noexcept = default;
  /*!
   * \brief Default destructor.
   *
   * \steady FALSE
   */
  ~RequiredServiceInstanceRegistry() noexcept;

  RequiredServiceInstanceRegistry(RequiredServiceInstanceRegistry const&) noexcept = delete;
  RequiredServiceInstanceRegistry(RequiredServiceInstanceRegistry&&) noexcept = delete;
  RequiredServiceInstanceRegistry& operator=(RequiredServiceInstanceRegistry const&) noexcept = delete;
  RequiredServiceInstanceRegistry& operator=(RequiredServiceInstanceRegistry&&) noexcept = delete;

  /*!
   * \brief A using for the Key type for the registry.
   */
  using Key =
      std::pair<amsr::net::ip::Udp::Endpoint, configuration::types::SomeIpServiceInterfaceDeploymentIdCommunication>;

  /*!
   * \brief A using for the Value type for the registry.
   */
  using Value = someip_protocol::internal::InstanceId;

  /*!
   * \brief Registers an entry of a given key to the registry.
   * \param[in] key The key that belongs to the associated value.
   * \param[in] entry The entry to the registry.
   *
   * \steady  FALSE
   */
  void Register(Key const& key, Value const entry) noexcept;

  /*!
   * \brief Deregisters an entry for a given key to a registry.
   * \param[in] key The key to access an existing registry.
   *
   * \steady  FALSE
   */
  void Deregister(Key const& key) noexcept;

  /*!
   * \brief Gets the Instance ID with the passed key.
   * \param[in] key A key used to access an existing registry.
   *
   * \steady TRUE
   * \return The instance ID or no value if the key does not map to any existing entry.
   * \error SomeIpReturnCode::kUnknownService The key does not match to any registry.
   */
  ara::core::Result<someip_protocol::internal::InstanceId, someip_protocol::internal::SomeIpReturnCode> GetInstanceId(
      Key const& key) const noexcept;

  /*!
   * \brief Gets the Instance ID with the passed key. It ignores the major version within the key.
   * \param[in] key A key used to access an existing registry.
   *
   * \steady TRUE
   * \return The instance ID or no value if the key does not map to any existing entry.
   */
  ara::core::Optional<someip_protocol::internal::InstanceId> GetPduInstanceId(Key const& key) const noexcept;

 private:
  /*!
   * A using for the key-value pair to represent an entry to the registry
   */
  using Entry = std::pair<Key, Value>;

  /*!
   * \brief The registry for required service instances.
   */
  ara::core::Vector<Entry> registry_{};
};

}  // namespace service_registry
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_REGISTRY_REQUIRED_SERVICE_INSTANCE_REGISTRY_H_
