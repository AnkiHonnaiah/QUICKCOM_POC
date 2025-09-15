/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       set_log_level.h
 *      \brief      Represents a SetLogLevel Ipc message type
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::SetLogLevel
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_SET_LOG_LEVEL_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_SET_LOG_LEVEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/internal/combine.h"
#include "amsr/log/internal/deserialize.h"
#include "amsr/log/internal/message/ipc_message_type.h"
#include "amsr/log/internal/serialize.h"
#include "amsr/log/internal/tuple.h"
#include "amsr/log/internal/types.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Set Log Level Message type
 */
class SetLogLevel final {
  /*! \brief The application name. */
  core::String application_id_;
  /*! \brief The context name. */
  core::String context_id_;
  /*! \brief The log level. */
  LogLevel log_level_;
  /*! \brief The log mode or sink. */
  LogMode log_mode_;
  /*! \brief The ECU id, if applicable. */
  core::String ecu_id_;

 public:
  /*!
   * \brief Create a set log level message
   *
   * \param application_id Contains the application 'id' as a string
   * \param context_id     Contains the context 'id' as a string
   * \param log_level      Contains the wanted new log level
   * \spec
   *   requires true;
   * \endspec
   */
  SetLogLevel(core::String&& application_id, core::String&& context_id, LogLevel log_level, LogMode log_mode) noexcept
      : SetLogLevel(std::move(application_id), std::move(context_id), log_level, log_mode, ""_sv) {}

  /*!
   * \brief Create a set log level message
   *
   * \param application_id Contains the application 'id' as a string
   * \param context_id     Contains the context 'id' as a string
   * \param log_level      Contains the wanted new log level
   * \param ecu_id         Contains the ECU 'id' as a string
   * \spec
   *   requires true;
   * \endspec
   */
  SetLogLevel(core::StringView application_id, core::StringView context_id, LogLevel log_level, LogMode log_mode,
              core::StringView ecu_id) noexcept
      : application_id_{std::move(application_id)},
        context_id_{std::move(context_id)},
        log_level_{log_level},
        log_mode_{log_mode},
        ecu_id_{ecu_id} {}

  /*!
   * \brief Deserialize a buffer to a SetLogLevel message
   *
   * \param de The deserializer that can deserialize simple values
   * \return   The deserialized message and the remaining buffer
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Deserialize(Deserializer& de) noexcept -> Result<SetLogLevel>;

  /*!
   * \brief Get the total serialized length of the message
   *
   * \return The serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  auto SerializedLength() const noexcept -> std::size_t {
    return sizeof(IpcMessageType) + StaticSerializedLength() + DynamicSerializedLength();
  }

  /*!
   * \brief Serialized length of dynamic values. Adds 1 to strings for zero termination.
   *
   * \return The combined serialized length of dynamic values.
   * \spec
   *   requires true;
   * \endspec
   */
  auto DynamicSerializedLength() const noexcept -> std::size_t {
    return (application_id_.size() + 1) + (context_id_.size() + 1) + (ecu_id_.size() + 1);
  }

  /*!
   * \brief Returns only the static serialized length.
   *
   * \return The static serialized length.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static auto StaticSerializedLength() noexcept -> std::size_t {
    return sizeof(log_level_) + sizeof(log_mode_);
  }

  /*!
   * \brief Gets the message type for this message
   *
   * \return SetLogLevel type
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static auto GetMessageType() noexcept -> IpcMessageType { return IpcMessageType::kSetLogLevel; }

  /*!
   * \brief Gets the application id
   *
   * \return The application id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetApplicationId() const noexcept -> core::String const& { return this->application_id_; }

  /*!
   * \brief Gets the non-const application id
   *
   * \return The application id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetApplicationId() noexcept -> core::String& { return this->application_id_; }

  /*!
   * \brief Gets the context id
   *
   * \return The context id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextId() const noexcept -> core::String const& { return this->context_id_; }

  /*!
   * \brief Gets the non-const context id
   *
   * \return The context id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextId() noexcept -> core::String& { return this->context_id_; }

  /*!
   * \brief Gets the log level
   *
   * \return The log level
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLogLevel() const noexcept -> LogLevel const& { return this->log_level_; }

  /*!
   * \brief Gets the log mode/sink
   *
   * \return The log mode/sink
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLogMode() const noexcept -> LogMode const& { return this->log_mode_; }

  /*!
   * \brief Gets the targeted ECU id
   *
   * \return The ECU id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetEcuId() const noexcept -> core::String const& { return this->ecu_id_; }
};

/*!
 * \brief Deserialize a SetLogLevel message.
 *
 * \param de The deserializer that can deserialize simple values.
 * \return   The deserialized message.
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-LogAndTrace-Ipc
 */
inline auto SetLogLevel::Deserialize(Deserializer& de) noexcept -> Result<SetLogLevel> {
  Result<SetLogLevel> result{Err(LogErrc::kBufferTooSmall, "Deserialize SetLogLevel message")};
  if (!de.Finished()) {
    Result<void> const type_result{de.IpcMessageType()
                                       .Filter([](IpcMessageType ty) { return ty == IpcMessageType::kSetLogLevel; },
                                               Err(LogErrc::kRuntimeError, "Incorrect Message Type for SetLogLevel!"))
                                       .Drop()};

    if (type_result) {
      // clang-format off
      result = Combine(
          de.String(),
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.LogLevel(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.LogMode(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); })
      .Map([](std::tuple<core::String, core::String, LogLevel, LogMode, core::String>&& tup){
        return MakeFromTuple<SetLogLevel>(std::move(tup));
      });
      // clang-format on
    } else {
      result.EmplaceError(type_result.Error());
    }
  }
  return result;
}

/*!
 * \brief Serialize the message
 *
 * \param buffer  The buffer to serialize into
 * \param message The SetLogLevel message to serialize
 * \return        The remaining buffer
 * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not serialize}
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-LogAndTrace-Ipc
 */
inline auto Serialize(ByteView buffer, SetLogLevel const& message) noexcept -> SerResult {
  return Ok(buffer)
      .AndThen([](ByteView buffer_view) {
        return Serialize(buffer_view, SetLogLevel::GetMessageType(), "SetLogLevel: Serialize IpcMessageType"_sv);
      })
      .AndThen([&message](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, message.GetApplicationId(), "SetLogLevel: Serialize Application ID"_sv);
      })
      .AndThen([&message](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, message.GetContextId(), "SetLogLevel: Serialize Context ID"_sv);
      })
      .AndThen([&message](ByteView buffer_view) {
        return Serialize(buffer_view, message.GetLogLevel(), "SetLogLevel: Serialize LogLevel"_sv);
      })
      .AndThen([&message](ByteView buffer_view) {
        return Serialize(buffer_view, message.GetLogMode(), "SetLogLevel: Serialize LogMode"_sv);
      })
      .AndThen([&message](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, message.GetEcuId(), "SetLogLevel: Serialize ECU ID"_sv);
      });
}

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_SET_LOG_LEVEL_H_
