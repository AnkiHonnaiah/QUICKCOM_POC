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
/**     \file       register_context.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::RegisterContext
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_REGISTER_CONTEXT_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_REGISTER_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <tuple>
#include <utility>

#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/combine.h"
#include "amsr/log/internal/deserialize.h"
#include "amsr/log/internal/message/ipc_message_type.h"
#include "amsr/log/internal/serialize.h"
#include "amsr/log/internal/time.h"
#include "amsr/log/internal/tuple.h"
#include "amsr/log/internal/types.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Registration Context Message type
 */
class RegisterContext final {
  /*! \brief The application name. */
  core::String app_id_;
  /*! \brief The context name. */
  core::String context_id_;
  /*! \brief The context description. */
  core::String context_description_;
  /*! \brief The current time in DLT format. */
  DltTime time_stamp_;

 public:
  /*!
   * \brief Create a registration message
   *
   * \param app_id      Contains the app 'id' as a string
   * \param ctx_id      Contains the context 'id' as a string
   * \param description Contains the application description
   * \param time_stamp  The timestamp that is used for the message. Defaults to now.
   * \spec
   *   requires true;
   * \endspec
   */
  RegisterContext(core::String&& app_id, core::String&& ctx_id, core::String&& description,
                  DltTime time_stamp = GetDltTimeStamp()) noexcept
      : app_id_{std::move(app_id)},
        context_id_{std::move(ctx_id)},
        context_description_{std::move(description)},
        time_stamp_{time_stamp} {}

  /*!
   * \brief Deserialize a buffer to a RegisterContext message
   *
   * \param de The deserializer that can deserialize simple values
   * \return   The deserialized message and the remaining buffer
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   *
   * \trace DSGN-LogAndTrace-Ipc
   */
  static auto Deserialize(Deserializer& de) noexcept -> Result<RegisterContext>;

  /*!
   * \brief Default move constructor
   * \spec
   *   requires true;
   * \endspec
   */
  RegisterContext(RegisterContext&&) noexcept = default;
  /*!
   * \brief Default move assignment
   *
   * \return A reference to the moved into object
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(RegisterContext&&) & noexcept -> RegisterContext& = default;
  /*!
   * \brief Default destructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~RegisterContext() noexcept = default;

  /*!
   * \brief Get the total serialized length of the message
   *
   * \return The serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  auto SerializedLength() const noexcept -> std::size_t {
    return sizeof(IpcMessageType) + this->StaticSerializedLength() + this->DynamicSerializedLength();
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
    return DltStringLength(this->app_id_) + DltStringLength(this->context_id_) +
           DltStringLength(this->context_description_);
  }

  /*!
   * \brief Returns only the static serialized length
   *
   * \return The static serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  static auto StaticSerializedLength() noexcept -> std::size_t { return sizeof(DltTime); }

  /*!
   * \brief Gets the message type for this message
   *
   * \return Register App type
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static auto GetMessageType() noexcept -> IpcMessageType { return IpcMessageType::kRegisterContext; }

  /*!
   * \brief Returns the app id
   * \return The app id
   */
  auto GetApplicationId() noexcept -> core::String& { return this->app_id_; }

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
   * \brief Returns the context description
   *
   * \return The context description
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextDescription() const noexcept -> core::String const& { return this->context_description_; }

  /*!
   * \brief Returns the context description
   *
   * \return The context description
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetContextDescription() noexcept -> core::String& { return this->context_description_; }

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
   * \brief Private copy constructor
   * \spec
   *   requires true;
   * \endspec
   */
  RegisterContext(RegisterContext const&) noexcept = default;
  /*!
   * \brief Private copy assignment
   *
   * \return A reference to the copied into object
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(RegisterContext const&) & -> RegisterContext& = default;

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

inline auto RegisterContext::Deserialize(Deserializer& de) noexcept -> Result<RegisterContext> {
  Result<RegisterContext> result{Err(LogErrc::kBufferTooSmall, "Deserialize RegisterContext message")};
  if (!de.Finished()) {
    Result<void> const type_result{
        de.IpcMessageType()
            .Filter([](IpcMessageType ty) { return ty == IpcMessageType::kRegisterContext; },
                    Err(LogErrc::kRuntimeError, "Incorrect Message Type for RegisterContext!"))
            .Drop()};

    if (type_result) {
      // clang-format off
      result = Combine(
          de.String(),
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.UInt64(); })
      .Map([](std::tuple<core::String, core::String, core::String, DltTime>&& tup) {
        return MakeFromTuple<RegisterContext>(std::move(tup));
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
 * \param reg    The register message to serialize
 * \return       The remaining buffer
 * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not serialize}
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace DSGN-LogAndTrace-Ipc
 */
inline auto Serialize(ByteView buffer, RegisterContext const& reg) noexcept -> SerResult {
  return Ok(buffer)
      .AndThen([](ByteView buffer_view) {
        return Serialize(buffer_view, RegisterContext::GetMessageType(),
                         "RegisterContext: Serialize IpcMessageType"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, reg.GetApplicationId(), "RegisterContext: Serialize Application ID"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, reg.GetContextId(), "RegisterContext: Serialize Context ID"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, reg.GetContextDescription(), "RegisterContext: Serialize context description"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        return Serialize(buffer_view, reg.GetTimeStamp(), "RegisterContext: Serialize timestamp"_sv);
      });
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_REGISTER_CONTEXT_H_
