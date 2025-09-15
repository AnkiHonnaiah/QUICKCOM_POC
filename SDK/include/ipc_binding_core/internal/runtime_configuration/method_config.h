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
 *        \brief  Configuration class for methods.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_METHOD_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_METHOD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief Enum classifying a method.
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
enum class MethodType : std::uint8_t {
  kRegular = 0,     /*!< Regular Method (ClientServerOperation) */
  kFieldGetter = 1, /*!< Field Getter */
  kFieldSetter = 2, /*!< Field Setter */
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_IPCBINDING_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief   Configuration for methods.
 * \details Configuration class storing the configuration parameters for an method.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class MethodConfig final {
 public:
  /*! Name alias for method identifier type. */
  using MethodId = ipc_protocol::MethodId;

  /*!
   * \brief Construct an empty method configuration.
   *
   * \param[in] method_type  Type of method.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  explicit MethodConfig(MethodType method_type = MethodType::kRegular) noexcept;

  /*!
   * \brief Destroy method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~MethodConfig() noexcept = default;

  /*!
   * \brief Copy-Construct a method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  MethodConfig(MethodConfig const&) = default;

  /*!
   * \brief Copy-Assign a method configuration.
   *
   * \return Reference to the copy-assigned method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(MethodConfig const&) & noexcept -> MethodConfig& = default;

  /*!
   * \brief Move-Construct a method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  MethodConfig(MethodConfig&&) noexcept = default;

  /*!
   * \brief Move-Assign a method configuration.
   *
   * \return Reference to the move-assigned method configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(MethodConfig&&) & noexcept -> MethodConfig& = default;

  /*!
   * \brief   Set the id of the method.
   * \details The id defines the id of the method.
   *
   * \param[in] id  The method id.
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
  void SetId(MethodId const id) noexcept;

  /*!
   * \brief Get the id of the method.
   *
   * \return The id of the method.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetId() const noexcept -> MethodId;

  /*!
   * \brief Gets the shortname.
   *
   * \return The shortname.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetShortname() const noexcept -> Shortname;

  /*!
   * \brief Sets the shortname.
   *
   * \param[in] shortname  The shortname.
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
  void SetShortname(Shortname const& shortname) noexcept;

  /*!
   * \brief Gets the method type.
   *
   * \return The method type.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetMethodType() const noexcept -> MethodType;

  /*!
   * \brief Gets whether the method is fire & forget.
   *
   * \return Whether the method is fire & forget.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  auto IsFireAndForget() const noexcept -> bool;

  /*!
   * \brief Sets whether the method is fire & forget.
   *
   * \param[in] is_fire_and_forget  Whether the method is fire & forget.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec requires true; \endspec
   */
  void SetIsFireAndForget(bool is_fire_and_forget) noexcept;

  /*!
   * \brief Gets whether tracing is enabled for sending a method request.
   *
   * \return Whether tracing is enabled for sending a method request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto IsTraceRequestSendEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for sending a method request.
   *
   * \param[in] enabled  Whether tracing is enabled for sending a method request.
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
  void SetTraceRequestSendEnabled(bool enabled) noexcept;

  /*!
   * \brief Gets whether tracing is enabled for receiving a method request.
   *
   * \return Whether tracing is enabled for receiving a method request.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto IsTraceRequestReceivedEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for receiving a method request.
   *
   * \param[in] enabled  Whether tracing is enabled for receiving a method request.
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
  void SetTraceRequestReceivedEnabled(bool enabled) noexcept;

  /*!
   * \brief Gets whether tracing is enabled for sending a method response.
   *
   * \return Whether tracing is enabled for sending a method response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto IsTraceResponseSendEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for sending a method response.
   *
   * \param[in] enabled  Whether tracing is enabled for sending a method response.
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
  void SetTraceResponseSendEnabled(bool enabled) noexcept;

  /*!
   * \brief Gets whether tracing is enabled for receiving a method response.
   *
   * \return Whether tracing is enabled for receiving a method response.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto IsTraceResponseReceivedEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for receiving a method response.
   *
   * \param[in] enabled  Whether tracing is enabled for receiving a method response.
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
  void SetTraceResponseReceivedEnabled(bool enabled) noexcept;

 private:
  /*! The method identifier. */
  MethodId id_{};

  /*! Short name for the method. */
  Shortname shortname_{};

  /*! Type of method. */
  MethodType method_type_;

  /*! Whether the method is fire & forget. */
  bool is_fire_and_forget_{false};

  /*! Whether tracing is enabled for sending a method request. */
  bool trace_request_send_enabled_{false};

  /*! Whether tracing is enabled for receiving a method request. */
  bool trace_request_received_enabled_{false};

  /*! Whether tracing is enabled for sending a method response. */
  bool trace_response_send_enabled_{false};

  /*! Whether tracing is enabled for receiving a method response. */
  bool trace_response_received_enabled_{false};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_METHOD_CONFIG_H_
