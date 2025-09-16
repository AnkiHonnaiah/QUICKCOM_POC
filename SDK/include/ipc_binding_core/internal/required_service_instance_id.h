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
 *        \brief  Unique identifier representing a RequiredServiceInstance ARXML model element.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_ID_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief   Unique identifier representing a RequiredServiceInstance ARXML model element.
 * \details As the configuration aspect is represented by a RequiredServiceInstance the InstanceId can be 'ALL' and the
 *          MinorVersion can be 'ANY'.
 *
 * \unit IpcBinding::IpcBindingCore::CoreUtility
 */
class RequiredServiceInstanceId final {
 public:
  /*!
   * \brief Construct a RequiredServiceInstanceId from dedicated identifiers.
   *
   * \param[in] service_id     ServiceInterface ID.
   * \param[in] instance_id    Instance ID. Can be 0xFFFFFFFF (ALL).
   * \param[in] major_version  Major Version.
   * \param[in] minor_version  Minor Version. Can be 0xFFFFFFFF (ANY).
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  RequiredServiceInstanceId(ipc_protocol::ServiceId const service_id, ipc_protocol::InstanceId const instance_id,
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
  RequiredServiceInstanceId(RequiredServiceInstanceId const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  RequiredServiceInstanceId(RequiredServiceInstanceId&&) noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~RequiredServiceInstanceId() noexcept = default;

  auto operator=(RequiredServiceInstanceId const&) noexcept -> RequiredServiceInstanceId& = delete;

  auto operator=(RequiredServiceInstanceId&&) noexcept -> RequiredServiceInstanceId& = delete;

  /*!
   * \brief Equal compare operator.
   *
   * \param[in] other  Other RequiredServiceInstanceId to compare with.
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
  auto operator==(RequiredServiceInstanceId const& other) const noexcept -> bool;

  /*!
   * \brief Less than compare operator.
   *
   * \param[in] other  Other RequiredServiceInstanceId to compare with.
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
  auto operator<(RequiredServiceInstanceId const& other) const noexcept -> bool;

  /*!
   * \brief Check if this RequiredServiceInstanceId matches with a concrete ProvidedServiceInstanceId.
   *
   * \param[in] provided_service_instance_id  The ProvidedServiceInstanceId to be matched.
   *
   * \return True if the Ids are matching, otherwise false. This matchers considers the potential InstanceId=ALL and
   *         MinorVersion=ANY criterias:
   *         - Any InstanceId of the ProvidedServiceInstance is matching if RequiredServiceInstance.InstanceId is ALL.
   *         - Any MinorVersion of the ProvidedServiceInstance is matching if the RequiredServiceInstance.MinorVersion
   *           is ANY.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto Matches(ProvidedServiceInstanceId const& provided_service_instance_id) const noexcept -> bool;

  /*!
   * \brief Get the ServiceId.
   *
   * \return The ServiceId.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
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
   * \steady    FALSE
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
   * \steady    FALSE
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

  /*!
   * \brief Check if the RequiredServiceInstance is using a InstanceId with value ALL.
   *
   * \return True if the InstanceId is ALL, otherwise false.
   *
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto HasInstanceIdAll() const noexcept -> bool;

  /*!
   * \brief Check if the RequiredServiceInstance is using a MinorVersion with value ANY.
   *
   * \return True if the MinorVersion is ANY, otherwise false.
   *
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto HasMinorVersionAny() const noexcept -> bool;

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

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REQUIRED_SERVICE_INSTANCE_ID_H_
