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
 *      \brief      Verbose types.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_VERBOSE_TYPES_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_VERBOSE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace log {
namespace internal {
namespace verbose {
// VECTOR NC AutosarC++17_10-M7.3.6: MD_LOG_AutosarC++17_10-M7.3.6_using_literal
using vac::container::literals::operator""_sv;

/*! \brief Type info for verbose mode as specified in DLT PRS. */
class TypeInfo {
  /*! \brief the serialized representation. */
  std::uint32_t serialized_;

 public:
  /*!
   * \brief The type information constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit TypeInfo(std::uint32_t serialized = 0) noexcept : serialized_{serialized} {}

  /*!
   * \brief Gets the serialized size of the type info object.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static auto Size() noexcept -> std::size_t { return sizeof(std::uint32_t); }
  /*!
   * \brief Gets the underlying serializable type.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSerializable() const noexcept -> std::uint32_t { return this->serialized_; }
  /*!
   * \brief Sets the type info to be a UTF8 String.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetString() & noexcept {
    // Set String
    this->SetBit(9);
    // Set UTF-8
    this->SetBit(15);
  }
  /*!
   * \brief Checks if type contains a String.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsString() const & noexcept -> bool { return this->HasBit(9); }
  /*!
   * \brief Set the VARI flag to signify a description/unit.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetVari() & noexcept { this->SetBit(11); }
  /*!
   * \brief Checks if VARI flag is set.
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasVari() const & noexcept -> bool { return this->HasBit(11); }

 private:
  /*!
   * \brief Sets a single bit in the u32.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetBit(std::uint32_t idx) & noexcept {
    this->serialized_ = static_cast<std::uint32_t>(this->serialized_ | static_cast<std::uint32_t>(1ul << idx));
  }
  /*!
   * \brief Checks a single bit in the u32.
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasBit(std::uint32_t idx) const & noexcept -> bool {
    return static_cast<bool>(static_cast<std::uint32_t>(this->serialized_ >> idx) & 1ul);
  }
};

/*! \brief A Dlt string argument with optional description. */
class DltString {
  /*! \brief The string data. */
  core::StringView data_;
  /*! \brief The string description. */
  core::StringView description_;

 public:
  /*!
   * \brief Creates the string argument from data & optionally the description.
   *
   * \param data        The string data to send.
   * \param description The optional description to send.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit DltString(core::StringView data, core::StringView description = ""_sv) noexcept
      : data_{data}, description_{description} {}
  /*!
   * \brief Gets the data for this DLT string.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() const noexcept -> core::StringView { return this->data_; }
  /*!
   * \brief Gets the description for this DLT string.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetDescription() const noexcept -> core::StringView { return this->description_; }
  /*!
   * \brief Returns true in case a description is contained.
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasDescription() const noexcept -> bool { return not this->description_.empty(); }
  /*!
   * \brief Calculates the buffer need for this type.
   * \spec
   *   requires true;
   * \endspec
   */
  auto BufferNeed() const noexcept -> std::size_t {
    constexpr std::size_t string_size_size{sizeof(std::uint16_t)};
    constexpr std::size_t string_termination_size{1};

    std::size_t need{verbose::TypeInfo::Size()};
    need += string_size_size;
    need += this->data_.size();
    need += string_termination_size;
    if (this->HasDescription()) {
      need += string_size_size;
      need += this->description_.size();
      need += string_termination_size;
    }
    return need;
  }
};
}  // namespace verbose
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_VERBOSE_TYPES_H_
