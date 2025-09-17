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
/**     \file
 *      \brief      Contains the IPC header for verbose messages.
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::LogVerbose
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_LOG_VERBOSE_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_LOG_VERBOSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <tuple>
#include <utility>

#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/combine.h"
#include "amsr/log/internal/deserialize.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/message/ipc_message_type.h"
#include "amsr/log/internal/serialize.h"
#include "amsr/log/internal/time.h"
#include "amsr/log/internal/tuple.h"
#include "amsr/log/internal/types.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Log verbose Message type
 * \details We only support extended headers at the moment
 */
class LogVerbose final {
  /*! \brief The default log level. */
  LogLevel level_;
  /*! \brief The application name. */
  core::String app_id_;
  /*! \brief The context id. */
  core::String context_id_;
  /*! \brief The current time in DLT format. */
  DltTime time_stamp_;

 public:
  /*!
   * \brief Create a non verbose message
   *
   * \param ll         Contains the default log level
   * \param app_id     Contains the app 'id' as a string
   * \param ctx_id     Contains the context 'id' as a string
   * \param time_stamp The timestamp that is used for the message. Defaults to now.
   * \spec
   *   requires true;
   * \endspec
   */
  LogVerbose(LogLevel ll, core::String&& app_id, core::String&& ctx_id, DltTime time_stamp = GetDltTimeStamp()) noexcept
      : level_(ll), app_id_{std::move(app_id)}, context_id_{std::move(ctx_id)}, time_stamp_{time_stamp} {}

  /*!
   * \brief Default copy constructor
   * \spec
   *   requires true;
   * \endspec
   */
  LogVerbose(LogVerbose const& lhs) noexcept = default;

  /*!
   * \brief Default move constructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  LogVerbose(LogVerbose&&) noexcept = default;
  /*!
   * \brief Default copy assignment
   *
   * \return a reference to the copied into object
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(LogVerbose const&) & noexcept -> LogVerbose& = default;
  /*!
   * \brief Default move assignment
   *
   * \return a reference to the moved into object
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(LogVerbose&&) & noexcept -> LogVerbose& = default;
  /*!
   * \brief Default destructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~LogVerbose() noexcept = default;

  /*!
   * \brief Get the total serialized length of the message
   *
   * \return The serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  auto SerializedLength() const noexcept -> std::size_t {
    return sizeof(IpcMessageType) + StaticSerializedLength() + this->DynamicSerializedLength();
  }

  /*!
   * \brief Returns only the dynamic serialized length
   *
   * \return The dynamic serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  auto DynamicSerializedLength() const noexcept -> std::size_t {
    return DltStringLength(this->app_id_) + DltStringLength(this->context_id_);
  }

  /*!
   * \brief Returns only the static serialized length
   *
   * \return The static serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  static auto StaticSerializedLength() noexcept -> std::size_t { return sizeof(LogLevel) + sizeof(DltTime); }

  /*!
   * \brief Gets the message type for this message
   *
   * \return Verbose message type
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static auto GetMessageType() noexcept -> IpcMessageType { return IpcMessageType::kVerbose; }

  /*!
   * \brief Returns the log level
   *
   * \return The log level
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLogLevel() const noexcept -> LogLevel { return this->level_; }

  /*!
   * \brief Returns the app id
   * \return The app id
   */
  auto GetApplicationId() const noexcept -> core::String const& { return this->app_id_; }

  /*!
   * \brief Returns the context id
   *
   * \return The context id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextId() const noexcept -> core::String const& { return this->context_id_; }

  /*!
   * \brief Returns the context id
   *
   * \return The context id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextId() noexcept -> core::String& { return this->context_id_; }

  /*!
   * \brief Returns the time stamp of the message.
   *
   * \return The time stamp.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetTimeStamp() const noexcept -> DltTime { return this->time_stamp_; }

 private:
  /*!
   * \brief Get the length of a DLT serialized string
   * \details DLT requires a trailing \0
   *
   * \param s The C++ string
   * \return  The length of the string in DLT format
   * \spec
   *   requires true;
   * \endspec
   */
  static auto DltStringLength(core::String const& s) noexcept -> std::size_t { return s.length() + 1; }
};

/*!
 * \brief Deserialize a buffer to a Verbose message
 *
 * \param de The deserializer that can deserialize simple values
 * \return   The deserialized message and the remaining buffer
 * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
 * \spec
 *   requires true;
 * \endspec
 */
inline auto DeserializeLogVerbose(Deserializer& de) noexcept -> Result<LogVerbose> {
  using vac::container::literals::operator""_sv;

  Result<LogVerbose> result{Err(LogErrc::kBufferTooSmall, "Deserialize LogVerbose message")};
  if (!de.Finished()) {
    Result<void> const type_result{de.IpcMessageType()
                                       .Filter([](IpcMessageType ty) { return ty == IpcMessageType::kVerbose; },
                                               Err(LogErrc::kRuntimeError, "Incorrect Message Type for LogVerbose!"))
                                       .Drop()};

    if (type_result) {
      // clang-format off
      result = Combine(
          de.LogLevel(),
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.UInt64(); })
      .Map([](std::tuple<LogLevel, core::String, core::String, DltTime>&& tup) {
        return MakeFromTuple<LogVerbose>(std::move(tup));
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
 * \param buffer The buffer to serialize into
 * \param msg    The register message to serialize
 * \return       The remaining buffer
 * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not serialize}
 * \spec
 *   requires true;
 * \endspec
 */
inline auto Serialize(ByteView buffer, LogVerbose const& msg) noexcept -> SerResult {
  return Ok(buffer)
      .AndThen([](ByteView buffer_view) {
        return Serialize(buffer_view, LogVerbose::GetMessageType(), "LogVerbose: Serialize IpcMessageType"_sv);
      })
      .AndThen([&msg](ByteView buffer_view) {
        return Serialize(buffer_view, msg.GetLogLevel(), "LogVerbose: Serialize LogLevel"_sv);
      })
      .AndThen([&msg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, msg.GetApplicationId(), "LogVerbose: Serialize Application ID"_sv);
      })
      .AndThen([&msg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, msg.GetContextId(), "LogVerbose: Serialize Context ID"_sv);
      })
      .AndThen([&msg](ByteView buffer_view) {
        return Serialize(buffer_view, msg.GetTimeStamp(), "LogVerbose: Serialize timestamp"_sv);
      });
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_LOG_VERBOSE_H_
