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
 *      \brief      Serializers for verbose types.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_VERBOSE_SERIALIZE_VERBOSE_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_VERBOSE_SERIALIZE_VERBOSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/serialize.h"
#include "amsr/log/internal/verbose/types.h"

namespace amsr {
namespace log {
namespace internal {
namespace verbose {
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::literals::operator""_sv;
namespace detail {
/*!
 * \brief Serialize a size of a string for DLT verbose mode.
 *
 * \param buffer The buffer to serialize into.
 * \param value  The value to serialize.
 * \param msg    The message to print in case an error occurs.
 * \return       The serializer result.
 * \spec
 *   requires true;
 * \endspec
 */
inline auto SerializeSize(ByteView buffer, core::StringView value, vac::container::CStringView msg) noexcept
    -> SerResult {
  constexpr std::size_t kNullterminationSize{1};
  return native::Serialize(buffer, static_cast<std::uint16_t>(kNullterminationSize + value.size()), msg);
}

/*!
 * \brief Serialize a description for DLT verbose mode.
 *
 * \param buffer      The buffer to serialize into.
 * \param description The description to serialize.
 * \return            The serializer result.
 * \spec
 *   requires true;
 * \endspec
 */
inline auto SerializeDescription(ByteView buffer, core::StringView description) noexcept -> SerResult {
  SerResult result{buffer};
  if (not description.empty()) {
    Assert(description.size() < std::numeric_limits<std::uint16_t>::max(),
           "DLT does not support overly long string sizes for descriptions."_sv);
    result =
        SerializeSize(buffer, description, "Serialize description size."_sv).AndThen([&description](ByteView view) {
          return internal::Serialize(view, description, "Serialize description."_sv);
        });
  }
  return result;
}
}  // namespace detail

/*!
 * \brief Serialize the string for DLT verbose mode.
 *
 * \param buffer The buffer to serialize into.
 * \param value  The value to serialize.
 * \return       The serializer result.
 *
 * \pre   Length of DLT string can not be greater than uint16_t. [PRS_Dlt_00156]
 *
 */
inline auto Serialize(ByteView buffer, DltString value) noexcept -> SerResult {
  // VCA_LOGAPI_OBJECT_IS_VALID
  core::StringView const msg{value.Get()};
  Assert(msg.size() < std::numeric_limits<std::uint16_t>::max(), "String size can not exceed max of a uint16_t."_sv);

  TypeInfo info{};
  if (value.HasDescription()) {
    info.SetVari();
  }
  info.SetString();

  return Ok(buffer)
      .AndThen([&info](ByteView view) {
        return native::Serialize(view, info.GetSerializable(), "Serialize DLT String Type Info."_sv);
      })
      .AndThen([&msg](ByteView view) { return detail::SerializeSize(view, msg, "Serialize DLT String size."_sv); })
      .AndThen([&value](ByteView view) { return detail::SerializeDescription(view, value.GetDescription()); })
      .AndThen([&msg](ByteView view) { return internal::Serialize(view, msg, "Serialize DLT String."_sv); });
}
}  // namespace verbose
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_VERBOSE_SERIALIZE_VERBOSE_H_
