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
/**     \file       deserialize.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::Deserializer
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_DESERIALIZE_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_DESERIALIZE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>

#include "amsr/core/string.h"
#include "amsr/core/vector.h"
#include "amsr/log/internal/algorithm.h"
#include "amsr/log/internal/message/ipc_message_type.h"
#include "amsr/log/internal/types.h"
#include "amsr/log/log_level.h"
#include "amsr/log/log_mode.h"
#include "amsr/log/log_types.h"
#include "ara/core/vector.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Contains all deserializers
 * \trace DSGN-LogAndTrace-Ipc
 */
class Deserializer final {
  /*!
   * \brief The buffer that's deserialized
   */
  ConstByteView buffer_;

 public:
  /*!
   * \brief Constructs the deserializer
   * \param buffer The buffer from which to deserialize
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Deserializer(ConstByteView buffer) noexcept : buffer_{buffer} {}

  /*!
   * \brief Constructs the deserializer
   * \param data Pointer to the buffer
   * \param size Size of the buffer
   * \spec
   *   requires true;
   * \endspec
   */
  Deserializer(Byte const* data, std::size_t size) noexcept : Deserializer(ConstByteView{data, size}) {}

  /*!
   * \brief Get the size of the remaining buffer
   * \spec
   *   requires true;
   * \endspec
   */
  auto Remaining() const noexcept -> std::size_t { return this->buffer_.size(); }

  /*!
   * \brief Check if the buffer is empty
   * \spec
   *   requires true;
   * \endspec
   */
  auto Finished() const noexcept -> bool { return this->Remaining() == 0; }

  /*!
   * \brief Increments the buffer window
   * \param count The number of bytes to move by
   * \spec
   *   requires true;
   * \endspec
   */
  void MoveWindow(std::size_t const count) noexcept { this->buffer_ = this->buffer_.subspan(count); }

  /*!
   * \brief Deserialize to a bool
   *
   * \return The deserializer result
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto Bool() noexcept -> Result<bool> {
    using vac::language::literals::operator""_byte;

    Result<bool> result{Err(LogErrc::kBufferTooSmall, "Deserialize bool")};
    constexpr std::size_t kCount{1};
    if (this->Remaining() >= kCount) {
      bool const b{this->buffer_[0] == 1_byte};

      this->MoveWindow(kCount);
      result.EmplaceValue(b);
    }

    return result;
  }

  /*!
   * \brief Deserialize to a uint16
   *
   * \return The deserializer result
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto UInt16() noexcept -> Result<std::uint16_t> {
    using vac::language::literals::operator""_byte;

    Result<std::uint16_t> result{Err(LogErrc::kBufferTooSmall, "Deserialize uint16")};

    constexpr std::size_t kCount{2};
    if (this->Remaining() >= kCount) {
      std::uint16_t num{0};
      num = static_cast<std::uint16_t>(num | GetByte<0, std::uint16_t>());
      num = static_cast<std::uint16_t>(num | GetByte<1, std::uint16_t>());

      this->MoveWindow(kCount);
      result.EmplaceValue(num);
    }
    return result;
  }

  /*!
   * \brief Deserialize to a uint32
   *
   * \return The deserializer result
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto UInt32() noexcept -> Result<std::uint32_t> {
    using vac::language::literals::operator""_byte;

    Result<std::uint32_t> result{Err(LogErrc::kBufferTooSmall, "Deserialize uint32")};

    constexpr std::size_t kCount{4};
    if (this->Remaining() >= kCount) {
      std::uint32_t num{0};
      num |= GetByte<0, std::uint32_t>();
      num |= GetByte<1, std::uint32_t>();
      num |= GetByte<2, std::uint32_t>();
      num |= GetByte<3, std::uint32_t>();

      this->MoveWindow(kCount);
      result.EmplaceValue(num);
    }
    return result;
  }

  /*!
   * \brief Deserialize to a uint64
   *
   * \return The deserializer result
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto UInt64() noexcept -> Result<std::uint64_t> {
    using vac::language::literals::operator""_byte;

    Result<std::uint64_t> result{Err(LogErrc::kBufferTooSmall, "Deserialize uint64")};

    constexpr std::size_t kCount{8};
    if (this->Remaining() >= kCount) {
      std::uint64_t num{0};
      num |= GetByte<0, std::uint64_t>();
      num |= GetByte<1, std::uint64_t>();
      num |= GetByte<2, std::uint64_t>();
      num |= GetByte<3, std::uint64_t>();
      num |= GetByte<4, std::uint64_t>();
      num |= GetByte<5, std::uint64_t>();
      num |= GetByte<6, std::uint64_t>();
      num |= GetByte<7, std::uint64_t>();

      this->MoveWindow(kCount);
      result.EmplaceValue(num);
    }
    return result;
  }

  /*!
   * \brief Deserialize to a string
   * \details The input string must have one and only one null terminator character
   *
   * \return The deserializer result
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto String() noexcept -> Result<core::String> {
    using vac::language::literals::operator""_byte;

    Result<core::String> result{Err(LogErrc::kBufferTooSmall, "Deserialize string")};

    if (Entry<ConstByteView> const entry{Find(this->buffer_, 0_byte)}) {
      std::size_t const count{entry.Index<std::size_t>()};

      // VECTOR NC AutosarC++17_10-A5.2.4: MD_LOG_AutosarC++17_10-A5.2.4_char_cast
      core::String s{reinterpret_cast<char const*>(this->buffer_.data()), count};

      this->MoveWindow(count + 1);
      result.EmplaceValue(std::move(s));
    }
    return result;
  }

  /*!
   * \brief Deserialize to a vector of byte
   *
   * \return The deserializer result
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
  auto Binary() noexcept -> Result<ara::core::Vector<Byte>> {
    using vac::language::literals::operator""_byte;

    std::size_t const count{this->Remaining()};

    ara::core::Vector<Byte> payload{};
    // VCA_LOGAPI_CHECKED_CONTRACT
    payload.reserve(count);
    CopyContainer(this->buffer_, payload);

    this->MoveWindow(count);

    return Ok(std::move(payload));
  }

  /*!
   * \brief Deserialize a buffer to a IpcMessage Type
   *
   * \return The deserialized message type and the remaining buffer
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto IpcMessageType() noexcept -> Result<internal::IpcMessageType> {
    Result<internal::IpcMessageType> result{Err(LogErrc::kBufferTooSmall, "Deserialize IpcMessageType")};
    constexpr std::size_t kCount{1};
    if (this->Remaining() >= kCount) {
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_ProtocolDefinition
      internal::IpcMessageType type{static_cast<internal::IpcMessageType>(GetByte<0, std::uint8_t>())};
      this->MoveWindow(kCount);
      result.EmplaceValue(type);
    }
    return result;
  }

  /*!
   * \brief Deserialize a buffer to a LogLevel
   *
   * \return The deserialized log level and the remaining buffer
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto LogLevel() noexcept -> Result<::amsr::log::LogLevel> {
    Result<::amsr::log::LogLevel> result{Err(LogErrc::kBufferTooSmall, "Deserialize LogLevel")};
    constexpr std::size_t kCount{1};
    if (this->Remaining() >= kCount) {
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_ProtocolDefinition
      ::amsr::log::LogLevel log_level{static_cast<::amsr::log::LogLevel>(GetByte<0, std::uint8_t>())};
      this->MoveWindow(kCount);
      result.EmplaceValue(log_level);
    }
    return result;
  }

  /*!
   * \brief Deserialize a buffer to a LogMode
   *
   * \return The deserialized log mode and the remaining buffer
   * \error{LogErrorDomain, LogErrc::kBufferTooSmall, Could not deserialize}
   * \spec
   *   requires true;
   * \endspec
   */
  auto LogMode() noexcept -> Result<::amsr::log::LogMode> {
    Result<::amsr::log::LogMode> result{Err(LogErrc::kBufferTooSmall, "Deserialize LogMode")};
    constexpr std::size_t kCount{1};
    if (this->Remaining() >= kCount) {
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_LOG_AutosarC++17_10-A7.2.1_ProtocolDefinition
      ::amsr::log::LogMode log_mode{static_cast<::amsr::log::LogMode>(GetByte<0, std::uint8_t>())};
      this->MoveWindow(kCount);
      result.EmplaceValue(log_mode);
    }
    return result;
  }

  /*!
   * \brief Gets the nth byte of the buffer
   *
   * \tparam N The index to extract.
   *           Shall be of type std::uint64_t.
   * \tparam T The type of value.
   *           Size of T must be larger than N.
   *           Must be an unsigned integer type.
   * \return   The nth byte of the buffer
   * \spec
   *   requires true;
   * \endspec
   */
  template <std::uint64_t N, typename T>
  auto GetByte() const noexcept -> T {
    static_assert(N < sizeof(T), "Deserializer::GetByte: Potential loss of data");
    constexpr std::uint64_t shifter{N * 0x8u};
    return static_cast<T>(static_cast<std::uint64_t>(buffer_[N]) << shifter);
  }
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_DESERIALIZE_H_
