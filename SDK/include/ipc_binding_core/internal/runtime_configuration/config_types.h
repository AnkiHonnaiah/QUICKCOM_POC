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
/*!        \file
 *        \brief  Types for the basic configuration structs.
 *        \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_CONFIG_TYPES_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_CONFIG_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "ara/core/string.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief Enumeration defining the available runtime processing modes.
 */
enum class RuntimeProcessingMode : std::uint8_t {
  /*!
   * \brief Runtime processing mode 'Polling'.
   */
  polling = 0,

  /*!
   * \brief Runtime processing mode 'ThreadDriven'.
   */
  thread_driven = 1
};

/*!
 * \brief Type definition for the shortname.
 *
 * \steady FALSE
 */
// VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
struct Shortname {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = ara::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for Shortname.
 *
 * \param[in] left   First Shortname to compare.
 * \param[in] right  Second Shortname to compare.
 *
 * \return True if first object value equals the second object value, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 */
auto operator==(Shortname const& left, Shortname const& right) noexcept -> bool;

/*!
 * \brief Type definition for the shortname path.
 *
 * \steady FALSE
 */
// VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
struct ShortnamePath {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = ara::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for Shortname path.
 *
 * \param[in] left   First Shortname path to compare.
 * \param[in] right  Second Shortname path to compare.
 *
 * \return True if first object value equals the second object value, otherwise false.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 * \steady    FALSE
 */
auto operator==(ShortnamePath const& left, ShortnamePath const& right) noexcept -> bool;

/*!
 * \brief Type definition for the instance specifier.
 *
 * \steady FALSE
 */
// VCA_IPCB_EXTERNAL_SAFE_FUNCTION_WITH_PARAMETERS
struct InstanceSpecifier {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = ara::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Type definition for the instance id hash.
 */
class InstanceIdHash {
 public:
  /*!
   * \brief Function call operator for instance ID.
   *
   * \param[in] id  Instance id to hash.
   *
   * \return Hashed instance id.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator()(ipc_protocol::InstanceId const id) const -> std::size_t {
    return std::hash<ipc_protocol::InstanceId::type>()(id.value);
  }
};

/*!
 * \brief Type definition for the event id hash.
 */
class EventIdHash {
 public:
  /*!
   * \brief Function call operator for event ID.
   *
   * \param[in] id  Event id to hash.
   *
   * \return Hashed event id.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator()(ipc_protocol::EventId const id) const noexcept -> std::size_t {
    return std::hash<ipc_protocol::EventId::type>()(id.value);
  }
};

/*!
 * \brief Type definition for the method id hash.
 */
class MethodIdHash {
 public:
  /*!
   * \brief Function call operator for method ID.
   *
   * \param[in] id  Method id to hash.
   *
   * \return Hashed method id.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator()(ipc_protocol::MethodId const id) const -> std::size_t {
    return std::hash<ipc_protocol::MethodId::type>()(id.value);
  }
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_CONFIG_TYPES_H_
