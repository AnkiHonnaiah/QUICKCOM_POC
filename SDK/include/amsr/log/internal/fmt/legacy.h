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
 *      \brief      Contains the legacy string formatter.
 *      \details
 *
 *      \unit       LogAndTrace::Logging::LegacyFormat
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_FMT_LEGACY_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_FMT_LEGACY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/charconv_error_domain.h"
#include "amsr/charconv/to_textual.h"
#include "amsr/core/span.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/time.h"
#include "amsr/log/internal/to_string.h"
#include "amsr/log/internal/types/message_meta_data.h"
#include "amsr/log/internal/types/typed_message_builder.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace log {
namespace internal {
namespace fmt {
namespace legacy {
/*!
 * \brief Serializes a named value.
 *
 * \tparam T    The type to serialize.
 *              Must meet the requirements of amsr::log::internal::details::Named<T>.
 * \param os    The stream to write into.
 * \param fn    The function to serialize the value itself.
 * \param value The named value.
 *
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
void SerializeNamed(stream::StatefulOutputStream& os,
                    vac::language::FunctionRef<void(stream::StatefulOutputStream&)> fn,
                    detail::Named<T> const& value) noexcept {
  using vac::container::literals::operator""_sv;

  bool const has_name{value.GetName() != nullptr};
  if (has_name) {
    os.Write("["_sv);
    // VCA_LOGAPI_VALID_OBJECT
    os.Write(amsr::core::StringView{*value.GetName()});
    os.Write(": "_sv);
  }
  fn(os);
  if (has_name) {
    os.Write("]"_sv);
  }
}

/*!
 * \brief Serializes a named value with a unit.
 *
 * \tparam  T       The type to serialize.
 *                  Must be either the type SInt, UInt or Float.
 * \param   os      The stream to write into.
 * \param   fn      The function to serialize the value itself.
 * \param   value   The named value with a unit.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
template <typename T>
void SerializeNameAndUnit(stream::StatefulOutputStream& os,
                          vac::language::FunctionRef<void(stream::StatefulOutputStream&)> fn, T const& value) noexcept {
  using vac::container::literals::operator""_sv;

  bool const has_name{value.GetName() != nullptr};
  bool const has_unit{value.GetUnit() != nullptr};

  if (has_name) {
    os.Write("["_sv);
    // VCA_LOGAPI_VALID_OBJECT
    os.Write(amsr::core::StringView(*value.GetName()));
    os.Write(": "_sv);
  }
  fn(os);
  if (has_name) {
    if (has_unit) {
      // VCA_LOGAPI_VALID_OBJECT
      os.Write(amsr::core::StringView{*value.GetUnit()});
    }
    os.Write("]"_sv);
  }
}

/*!
 * \brief Serializes a bool value.
 *
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, Bool const& value) noexcept {
  auto serializer_fn = [&value](stream::StatefulOutputStream& o) {
    // VCA_LOGAPI_CHECKED_CONTRACT
    o.Write(value.GetData());
  };
  SerializeNamed(os, serializer_fn, value);
}

/*!
 * \brief Serializes a value as binary.
 * \tparam T The type to serialize as binary.
 *           Must be an unsigned integer.
 * \details Is a workaround until iostream provides this functionality itself.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
template <typename T>
void SerializeBinary(stream::StatefulOutputStream& os, T value) noexcept {
  // We need one byte per possible bit.
  core::Array<char, (sizeof(T) * 8) + 1> buf{};
  charconv::ToTextual<T> to_textual{core::Span<char>{buf}, value};
  amsr::core::Result<amsr::core::Span<char>> conversion_result{
      to_textual.Convert(charconv::NumberBase::kBinary, charconv::extended_format_default)};
  internal::Assert(conversion_result.HasValue(), vac::container::CStringView{"Could not serialize binary"});

  os.Write(conversion_result.Value());
}

/*!
 * \brief Serialize an unsigned int as a binary value.
 * \details Is a workaround until iostream provides this functionality itself.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void SerializeBinary(stream::StatefulOutputStream& os, UInt const& value) noexcept {
  switch (value.GetNumBytes()) {
    case detail::IntSize::kBits8:
      SerializeBinary(os, static_cast<std::uint8_t>(value.GetData()));
      break;
    case detail::IntSize::kBits16:
      SerializeBinary(os, static_cast<std::uint16_t>(value.GetData()));
      break;
    case detail::IntSize::kBits32:
      SerializeBinary(os, static_cast<std::uint32_t>(value.GetData()));
      break;
    default:  // case kBits64:
      SerializeBinary(os, static_cast<std::uint64_t>(value.GetData()));
      break;
  }
}

/*!
 * \brief Serialize a signed int.
 * \details Is a workaround until iostream provides this functionality itself.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, SInt const& value) noexcept {
  auto serializer_fn = [&value](stream::StatefulOutputStream& o) { o.Write(value.GetData()); };
  SerializeNameAndUnit(os, serializer_fn, value);  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
}

/*!
 * \brief Serialize an unsigned int.
 * \details Is a workaround until iostream provides this functionality itself.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, UInt const& value) noexcept {
  auto serializer_fn = [&value](stream::StatefulOutputStream& o) {
    switch (value.GetBase()) {
      case detail::IntBase::kHex:
        o.Write(value.GetData(), stream::Base::kHex);
        break;
      case detail::IntBase::kBin:
        // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        SerializeBinary(o, value);
        break;
      default:  // case kDec
        o.Write(value.GetData());
        break;
    }
  };
  SerializeNameAndUnit(os, serializer_fn, value);  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
}

/*!
 * \brief Serialize a floating point number.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, Float const& value) noexcept {
  auto serializer_fn = [&value](stream::StatefulOutputStream& o) {
    if (value.IsDouble()) {
      o.Write(value.GetData());
    } else {
      o.Write(static_cast<float>(value.GetData()));
    }
  };
  SerializeNameAndUnit(os, serializer_fn, value);  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
}

/*!
 * \brief Serialize a string.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, String const& value) noexcept {
  auto serializer_fn = [&value](stream::StatefulOutputStream& o) {
    // VCA_LOGAPI_CHECKED_CONTRACT
    o.Write(amsr::core::StringView(*value.GetData()));
  };
  SerializeNamed(os, serializer_fn, value);  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
}

/*!
 * \brief Serialize a raw buffer.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, Raw const& value) noexcept {
  auto serializer_fn = [&value](stream::StatefulOutputStream& o) {
    // VECTOR NC AutosarC++17_10-A5.2.4: MD_LOG_AutosarC++17_10-A5.2.4_external_API
    core::Span<std::uint8_t> const span{reinterpret_cast<std::uint8_t*>(value.GetData()->data()),
                                        value.GetData()->size()};
    Drop(stream::Uppercase(o));
    o.Write(span, stream::Base::kHex);
  };
  SerializeNamed(os, serializer_fn, value);  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
}

/*!
 * \brief Serialize an error code.
 * \param os The stream to write into.
 * \param value The value to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void Serialize(stream::StatefulOutputStream& os, NamedError const& value) noexcept {
  using vac::container::literals::operator""_sv;

  auto serializer_fn = [&value](stream::StatefulOutputStream& o) {
    core::ErrorCode const& ec{value.GetData()};
    o.Write(R"({"Domain":")"_sv);
    // VCA_LOGAPI_CHECKED_CONTRACT
    core::StringView const domain{ec.Domain().Name()};
    o.Write(domain);
    o.Write(R"(", "Code":)"_sv);
    o.Write(ec.Value());
    o.Write(R"(, "Msg":")"_sv);
    // VCA_LOGAPI_VALID_OBJECT
    core::StringView const message{ec.Message()};
    o.Write(message);
    // VCA_LOGAPI_VALID_OBJECT
    core::StringView const user_message{ec.UserMessage()};
    o.Put('"');
    if (not user_message.empty()) {
      o.Write(R"(, "User":")"_sv);
      o.Write(user_message);
      o.Put('"');
    }
    // VCA_LOGAPI_OBJECT_IS_VALID
    if (ec.SupportData() != 0) {
      o.Write(R"(, "Data":)"_sv);
      // VCA_LOGAPI_CHECKED_CONTRACT
      o.Write(ec.SupportData());
    }
    o.Write("}"_sv);
  };
  // VCA_LOGAPI_VALID_OBJECT
  SerializeNamed(os, serializer_fn, value);
}

/*!
 * \brief Serialize a complete message.
 * \param os The stream to write into.
 * \param message The message to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void WriteMessage(stream::StatefulOutputStream& os, Message const message) noexcept {
  for (LogType const& lt : message) {
    // VCA_LOGAPI_VALID_OBJECT
    Visit([&os](auto&& part) { Serialize(os, part); }, lt);
  }
}

/*!
 * \brief Serialize a log message prefix.
 * \param os The stream to write into.
 * \param meta_data The meta data to serialize.
 * \spec
 *   requires true;
 * \endspec
 */
inline void WriteLogPrefix(stream::StatefulOutputStream& os, MessageMetaData meta_data) noexcept {
  using vac::container::literals::operator""_sv;

  TimestampTimePoint const msg_time{meta_data.GetTimePoint()};

  std::uint64_t const t{DurationAsMicroseconds(msg_time.time_since_epoch())};
  os.Write("["_sv);

  os.Write(t);
  os.Write("]["_sv);
  Id const app_id{meta_data.GetApplicationId()};
  os.Write(amsr::core::MakeSpan(app_id));
  os.Write(" "_sv);
  core::StringView const ctx_id{meta_data.GetContext()};
  os.Write(ctx_id);
  os.Write("]["_sv);
  LogLevel const log_level{meta_data.GetSeverity()};
  os.Write(internal::GetSeverityString(log_level));

  if (Optional<Ref<internal::Location const>> location{meta_data.GetLocation()}) {
    internal::Location const& loc{location.value().Get()};
    os.Write("]["_sv);
    // VCA_LOGAPI_VALID_OBJECT
    Serialize(os, loc.file);
    os.Write(":"_sv);
    // VCA_LOGAPI_VALID_OBJECT
    Serialize(os, loc.line);
  }

  os.Write("] "_sv);
}

/*!
 * \brief Creates a complete legacy message for use with legacy log sink interfaces.
 * \param os The stream to write into.
 * \param meta_data The meta data to serialize.
 * \param message The message to serialize.
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline void CreateLegacyMessage(stream::StatefulOutputStream& os, MessageMetaData meta_data, Message message) noexcept {
  using vac::container::literals::operator""_sv;

  if (Optional<Ref<internal::Location const>> location{meta_data.GetLocation()}) {
    internal::Location const& loc{location.value().Get()};
    os.Write("["_sv);
    // VCA_LOGAPI_VALID_OBJECT
    fmt::legacy::Serialize(os, loc.file);
    os.Write(":"_sv);
    // VCA_LOGAPI_VALID_OBJECT
    fmt::legacy::Serialize(os, loc.line);
    os.Write("] "_sv);
  }

  internal::fmt::legacy::WriteMessage(os, message);
  os.Write("\0"_sv);
}
}  // namespace legacy
}  // namespace fmt
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_FMT_LEGACY_H_
