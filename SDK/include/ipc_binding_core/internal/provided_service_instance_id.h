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
/**        \file
 *        \brief  Unique identifier representing a ProvidedServiceInstance ARXML model element.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PROVIDED_SERVICE_INSTANCE_ID_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PROVIDED_SERVICE_INSTANCE_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief   Unique identifier representing a ProvidedServiceInstance ARXML model element.
 * \details As a concrete service instance offered by an service skeleton is represented the InstanceId must not be
 *          'ALL' and the MinorVersion must not be 'ANY'.
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
class ProvidedServiceInstanceId final {
 public:
  /*!
   * \brief Construct a ProvidedServiceInstanceId from dedicated identifiers.
   *
   * \param[in] service_id     ServiceInterface ID.
   * \param[in] instance_id    Instance ID. Must not be 0xFFFFFFFF (ALL).
   * \param[in] major_version  Major Version.
   * \param[in] minor_version  Minor Version. Must not be 0xFFFFFFFF (ANY).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  ProvidedServiceInstanceId(ipc_protocol::ServiceId const service_id, ipc_protocol::InstanceId const instance_id,
                            ipc_protocol::MajorVersion const major_version,
                            ipc_protocol::MinorVersion const minor_version) noexcept;

  /*!
   * \brief Default copy constructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ProvidedServiceInstanceId(ProvidedServiceInstanceId const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ProvidedServiceInstanceId(ProvidedServiceInstanceId&&) noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~ProvidedServiceInstanceId() noexcept = default;

  auto operator=(ProvidedServiceInstanceId const&) noexcept -> ProvidedServiceInstanceId& = delete;

  /*!
   * \brief Default move assingment constructor.
   *
   * \return The ProvidedServiceInstanceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  auto operator=(ProvidedServiceInstanceId&&) & noexcept -> ProvidedServiceInstanceId& = default;

  /*!
   * \brief Equal compare operator.
   *
   * \param[in] other  Other ProvidedServiceInstanceId to compare with.
   *
   * \return True if the instance is equal to the other instance, false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator==(ProvidedServiceInstanceId const& other) const noexcept -> bool;

  /*!
   * \brief Less than compare operator.
   *
   * \param[in] other  Other ProvidedServiceInstanceId to compare with.
   *
   * \return True if instance is less than the other instance, false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator<(ProvidedServiceInstanceId const& other) const noexcept -> bool;

  /*!
   * \brief Get the ServiceId.
   *
   * \return The ServiceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto ServiceId() const noexcept -> ipc_protocol::ServiceId;

  /*!
   * \brief Get the InstanceId.
   *
   * \return The InstanceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto InstanceId() const noexcept -> ipc_protocol::InstanceId;

  /*!
   * \brief Get the MajorVersion.
   *
   * \return The MajorVersion.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \spec requires true; \endspec
   */
  auto MajorVersion() const noexcept -> ipc_protocol::MajorVersion;

  /*!
   * \brief Get the MinorVersion.
   *
   * \return The MinorVersion.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto MinorVersion() const noexcept -> ipc_protocol::MinorVersion;

 private:
  /*!
   * \brief The ServiceId.
   */
  ipc_protocol::ServiceId service_id_;

  /*!
   * \brief The InstanceId.
   */
  ipc_protocol::InstanceId instance_id_;

  /*!
   * \brief The MajorVersion.
   */
  ipc_protocol::MajorVersion major_version_;

  /*!
   * \brief The MinorVersion.
   */
  ipc_protocol::MinorVersion minor_version_;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PROVIDED_SERVICE_INSTANCE_ID_H_
