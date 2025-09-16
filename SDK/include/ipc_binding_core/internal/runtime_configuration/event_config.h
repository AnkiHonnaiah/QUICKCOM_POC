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
 *        \brief  Configuration class for events.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_EVENT_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_EVENT_CONFIG_H_

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
 * \brief Enum classifying an event.
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
enum class EventType : std::uint8_t {
  kRegular = 0,       /*!< Regular Event (VariableDataPrototype) */
  kFieldNotifier = 1, /*!< Field Notifier */
};

/*!
 * \brief   Configuration for events.
 * \details Configuration class storing the configuration parameters for an event.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class EventConfig final {
 public:
  /*! Name alias for event identifier type. */
  using EventId = ipc_protocol::EventId;

  /*!
   * \brief Construct an empty event configuration.
   *
   * \param[in] event_type  Type of event.
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
  explicit EventConfig(EventType event_type = EventType::kRegular) noexcept;

  /*!
   * \brief Destroy event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~EventConfig() noexcept = default;

  /*!
   * \brief Copy-Construct an event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventConfig(EventConfig const&) = default;

  /*!
   * \brief Copy-Assign an event configuration.
   *
   * \return Reference to the copy-assigned event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(EventConfig const&) & noexcept -> EventConfig& = default;

  /*!
   * \brief Move-Construct an event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  EventConfig(EventConfig&&) noexcept = default;

  /*!
   * \brief Move-Assign an event configuration.
   *
   * \return Reference to the move-assigned event configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(EventConfig&&) & noexcept -> EventConfig& = default;

  /*!
   * \brief   Set the id of the event.
   * \details The id defines the id of the event.
   *
   * \param[in] id  The event id.
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
  void SetId(EventId const id) noexcept;

  /*!
   * \brief Get the id of the event.
   *
   * \return The id of the event.
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
  auto GetId() const noexcept -> EventId;

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
   * \brief Gets the event type.
   *
   * \return The event type.
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
  auto GetEventType() const noexcept -> EventType;

  /*!
   * \brief Gets whether tracing is enabled for the Skeleton::Send(SampleType) API.
   *
   * \return Whether tracing is enabled for the Skeleton::Send(SampleType) API.
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
  auto IsTraceSendEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for the Skeleton::Send(SampleType) API.
   *
   * \param[in] enabled  Whether tracing is enabled for the Skeleton::Send(SampleType) API.
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
  void SetTraceSendEnabled(bool enabled);

  /*!
   * \brief Gets whether tracing is enabled for the Skeleton::Send(SampleAllocateePtr) API.
   *
   * \return Whether tracing is enabled for the Skeleton::Send(SampleAllocateePtr) API.
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
  auto IsTraceSendAllocateEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for the Skeleton::Send(SampleAllocateePtr) API.
   *
   * \param[in] enabled  Whether tracing is enabled for the Skeleton::Send(SampleAllocateePtr) API.
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
  void SetTraceSendAllocateEnabled(bool enabled);

  /*!
   * \brief Gets whether tracing is enabled for ReadSample API.
   *
   * \return Whether tracing is enabled for ReadSample API.
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
  auto IsTraceReadSampleEnabled() const noexcept -> bool;

  /*!
   * \brief Sets whether tracing is enabled for ReadSample API.
   *
   * \param[in] enabled  Whether tracing is enabled for ReadSample API.
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
  void SetTraceReadSampleEnabled(bool enabled);

 private:
  /*! The event identifier. */
  EventId id_{};

  /*! Short name for the event. */
  Shortname shortname_{};

  /*! Type of event. */
  EventType event_type_;

  /*! Whether tracing is enabled for the Skeleton::Send(SampleType) API. */
  bool trace_send_enabled_{false};

  /*! Whether tracing is enabled for the Skeleton::Send(SampleAllocateePtr) API. */
  bool trace_send_allocate_enabled_{false};

  /*! Whether tracing is enabled for ReadSample API. */
  bool trace_send_read_sample_enabled_{false};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_EVENT_CONFIG_H_
