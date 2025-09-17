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
/**     \file       register_app.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::RegisterApp
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_REGISTER_APP_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_REGISTER_APP_H_

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
 * \brief Registration Application Message type
 */
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
class RegisterApp final {
  /*! \brief The application name. */
  core::String application_id_;
  /*! \brief The application description. */
  core::String application_description_;
  /*! \brief The current time in DLT format. */
  DltTime time_stamp_;

 public:
  /*!
   * \brief Create a registration message
   *
   * \param id          Contains the application 'id' as a string
   * \param description Contains the application description
   * \param time_stamp  The timestamp that is used for the message. Defaults to now.
   * \spec
   *   requires true;
   * \endspec
   */
  RegisterApp(core::String&& id, core::String&& description, DltTime time_stamp = GetDltTimeStamp()) noexcept
      : application_id_{std::move(id)}, application_description_{std::move(description)}, time_stamp_{time_stamp} {}

  /*!
   * \brief Deserialize a buffer to a RegisterApp message
   *
   * \param de The deserializer that can deserialize simple values
   * \return   The deserialized message and the remaining buffer
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  static auto Deserialize(Deserializer& de) noexcept -> Result<RegisterApp>;

  /*!
   * \brief Default move constructor
   * \spec
   *   requires true;
   * \endspec
   */
  RegisterApp(RegisterApp&&) noexcept = default;
  /*!
   * \brief Default move assignment
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(RegisterApp&&) & noexcept -> RegisterApp& = default;
  /*!
   * \brief Default destructor
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~RegisterApp() noexcept = default;

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
   * \brief Returns only the dynamic serialized length
   *
   * \return The dynamic serialized length
   * \spec
   *   requires true;
   * \endspec
   */
  auto DynamicSerializedLength() const noexcept -> std::size_t {
    return DltStringLength(application_id_) + DltStringLength(application_description_);
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
  constexpr static auto GetMessageType() noexcept -> IpcMessageType { return IpcMessageType::kRegisterApp; }

  /*!
   * \brief Returns the application id
   *
   * \return The application id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetApplicationId() const noexcept -> core::String const& { return this->application_id_; }

  /*!
   * \brief Returns the application id
   *
   * \return The application id
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetApplicationId() noexcept -> core::String& { return this->application_id_; }

  /*!
   * \brief Returns the application description
   *
   * \return The application description
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetApplicationDescription() const noexcept -> core::String const& { return this->application_description_; }

  /*!
   * \brief Returns the application description
   *
   * \return The application description
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetApplicationDescription() noexcept -> core::String& { return this->application_description_; }

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
  RegisterApp(RegisterApp const&) noexcept = default;
  /*!
   * \brief Private copy assignment
   *
   * \return a reference to the copied into object
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(RegisterApp const&) & noexcept -> RegisterApp& = default;

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

inline auto RegisterApp::Deserialize(Deserializer& de) noexcept -> Result<RegisterApp> {
  Result<RegisterApp> result{Err(LogErrc::kBufferTooSmall, "Deserialize RegisterApp message")};
  if (!de.Finished()) {
    Result<void> const type_result{de.IpcMessageType()
                                       .Filter([](IpcMessageType ty) { return ty == IpcMessageType::kRegisterApp; },
                                               Err(LogErrc::kRuntimeError, "Incorrect Message Type for RegisterApp!"))
                                       .Drop()};

    if (type_result) {
      // clang-format off
      result = Combine(
          de.String(),
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.String(); },
          // VCA_LOGAPI_OBJECT_IS_VALID
          [&de]() { return de.UInt64(); })
      .Map([](std::tuple<core::String, core::String,
                         DltTime>&& tup) { return MakeFromTuple<RegisterApp>(std::move(tup)); });
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
 */
inline auto Serialize(ByteView buffer, RegisterApp const& reg) noexcept -> SerResult {
  return Ok(buffer)
      .AndThen([](ByteView buffer_view) {
        return Serialize(buffer_view, RegisterApp::GetMessageType(), "RegisterApp: Serialize IpcMessageType"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, reg.GetApplicationId(), "RegisterApp: Serialize Application ID"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        // VCA_LOGAPI_OBJECT_IS_VALID
        return Serialize(buffer_view, reg.GetApplicationDescription(),
                         "RegisterApp: Serialize application description"_sv);
      })
      .AndThen([&reg](ByteView buffer_view) {
        return Serialize(buffer_view, reg.GetTimeStamp(), "RegisterApp: Serialize timestamp"_sv);
      });
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_REGISTER_APP_H_
