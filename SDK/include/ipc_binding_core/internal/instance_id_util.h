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
 *        \brief  Helper class to create ara::com instance identifier.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_INSTANCE_ID_UTIL_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_INSTANCE_ID_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "ara/com/instance_identifier.h"
#include "ara/core/result.h"
#include "vac/container/string_literals.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief
 *
 * \unit IpcBinding::IpcBinding::InstanceIdUtil
 */
class InstanceIdUtil final {
 public:
  /*!
   * \brief Type-alias for the IpcProtocol ServiceId.
   */
  using ServiceId = ::amsr::ipc_binding_core::internal::ipc_protocol::ServiceId;

  /*!
   * \brief Type-alias for the IpcProtocol InstanceId.
   */
  using InstanceId = ::amsr::ipc_binding_core::internal::ipc_protocol::InstanceId;

  /*!
   * \brief Type-alias for the IpcProtocol MajorVersion.
   */
  using MajorVersion = ::amsr::ipc_binding_core::internal::ipc_protocol::MajorVersion;

  /*!
   * \brief Type-alias for the IpcProtocol MajorVersion.
   */
  using MinorVersion = ::amsr::ipc_binding_core::internal::ipc_protocol::MinorVersion;

  /*!
   * \brief Type-alias for the ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type-alias for the ProvidedServiceInstanceId.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  InstanceIdUtil() noexcept = delete;
  InstanceIdUtil(InstanceIdUtil const&) noexcept = delete;
  auto operator=(InstanceIdUtil const&) noexcept -> InstanceIdUtil& = delete;
  InstanceIdUtil(InstanceIdUtil&&) noexcept = delete;
  auto operator=(InstanceIdUtil&&) noexcept -> InstanceIdUtil& = delete;

  /*!
   * \brief   Destroy the ID translator.
   * \details Destructor for this utility class (static APIs only) provided to avoid potential undefined behavior in
   *          case of future extension (e.g. non-static members).
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~InstanceIdUtil() noexcept = default;

  /*!
   * \brief   Builder function of ara::com instance identifier.
   * \details The function prepares and returns instance identifier string from a given parameters.
   *
   * \param[in] service_id     IPC service id.
   * \param[in] instance_id    IPC Instance id.
   * \param[in] major_version  IPC major version.
   * \param[in] minor_version  IPC minor version.
   *
   * \return Instance identifier in the format : IpcBinding:<>:<>:<>:<>.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto BuildAraComInstanceIdentifier(ServiceId service_id, InstanceId instance_id, MajorVersion major_version,
                                            MinorVersion minor_version) noexcept -> ::ara::com::InstanceIdentifier;

  /*!
   * \brief   Builder function of ara::com instance identifier.
   * \details The function prepares and returns instance identifier string from a given provided service instance.
   *
   * \param[in] provided_instance_id  Provided service instance id.
   *
   * \return Instance identifier in the format : IpcBinding:<>:<>:<>:<>.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto BuildAraComInstanceIdentifier(ProvidedServiceInstanceId const& provided_instance_id) noexcept
      -> ::ara::com::InstanceIdentifier;

  /*!
   * \brief   Builder function of ara::com instance identifier.
   * \details The function prepares and returns instance identifier string from a given required service instance.
   *
   * \param[in] required_instance_id  Required service instance id.
   *
   * \return Instance identifier in the format : IpcBinding:<>:<>:<>:<>.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto BuildAraComInstanceIdentifier(RequiredServiceInstanceId const& required_instance_id) noexcept
      -> ::ara::com::InstanceIdentifier;

  /*!
   * \brief Parse the ProvidedServiceInstanceId from ara::com instance identifier.
   *
   * \param[in] provided_instance_identifier  The ProvidedInstanceIdentifier to be parsed.
   *
   * \return The parsed ProvidedServiceInstanceId.
   *
   * \error IpcBindingErrc::error_not_ok  In case the InstanceIdentifier format is not correct.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  static auto BuildProvidedInstanceIdentifier(
      ::amsr::socal::internal::ProvidedInstanceIdentifier const& provided_instance_identifier) noexcept
      -> ::ara::core::Result<ProvidedServiceInstanceId>;

 private:
  /*!
   * \brief IpcBinding specific string prefix for ara::com InstanceIdentifier.
   */
  static constexpr ::ara::core::StringView kBindingPrefix{"IpcBinding"_sv};

  /*!
   * \brief Colon separator used in ara::com InstanceIdentifier.
   */
  static constexpr ::ara::core::StringView kColonSeparator{":"_sv};

  /*!
   * \brief Parse the ProvidedServiceInstanceId from a ara::com InstanceIdentifier.
   *
   * \param[in] instance_identifier  The string view of InstanceIdentifier to be parsed.
   *
   * \return The parsed ProvidedServiceInstanceId.
   *
   * \error IpcBindingErrc::error_not_ok  In case the InstanceIdentifier format is not correct.
   *
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto ParseAraComInstanceId(::ara::core::StringView const& instance_identifier) noexcept
      -> ::ara::core::Result<ProvidedServiceInstanceId>;

  /*!
   * \brief Reads rest of the substring after colon.
   *
   * \param[in] read_string  The string where substring is read.
   *
   * \return The substring.
   *
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto ReadSubString(::ara::core::StringView read_string) noexcept -> ::ara::core::StringView;

  /*!
   * \brief Covert the underlying string to its integral value and return.
   *
   * \param[in] string_to_parse  The view of the string to be parsed.
   *
   * \return Result containing converted integer or error.
   *
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  static auto SVtoInteger(::ara::core::StringView string_to_parse) noexcept -> ::ara::core::Result<std::uint32_t>;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_INSTANCE_ID_UTIL_H_
