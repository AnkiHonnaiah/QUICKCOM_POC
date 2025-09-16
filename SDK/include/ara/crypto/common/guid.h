// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
/*!        \file  ara/crypto/common/guid.h
 *        \brief  Definition of Universally Unique Identifier (UUID) or Globally Unique Identifier (GUID) type.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_GUID_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_GUID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>

#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {

class Uuid;

constexpr bool operator==(Uuid const& lhs, Uuid const& rhs) noexcept;

constexpr bool operator!=(Uuid const& lhs, Uuid const& rhs) noexcept;

constexpr bool operator<(Uuid const& lhs, Uuid const& rhs) noexcept;

constexpr bool operator<=(Uuid const& lhs, Uuid const& rhs) noexcept;

constexpr bool operator>(Uuid const& lhs, Uuid const& rhs) noexcept;

constexpr bool operator>=(Uuid const& lhs, Uuid const& rhs) noexcept;

/*!
 * \brief UUID data structure.
 * \vpublic
 */
class Uuid final {
  /*!
   * \brief Compares two UUIDs byte-wise for equality.
   * \param[in] lhs The left hand side to compare.
   * \param[in] rhs The right hand side to compare with.
   * \return whether the UUIDs are equal
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  friend constexpr bool operator==(Uuid const& lhs, Uuid const& rhs) noexcept {
    bool equal{true};

    for (std::size_t i{0}; i < lhs.data_.size(); ++i) {
      equal = equal && (lhs.data_[i] == rhs.data_[i]);
    }

    return equal;
  }

  /*!
   * \brief Compares two UUIDs byte-wise for difference.
   * \param[in] lhs The left hand side to compare.
   * \param[in] rhs The right hand side to compare with.
   * \return whether the UUIDs are not equal
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  friend constexpr bool operator!=(Uuid const& lhs, Uuid const& rhs) noexcept { return !(lhs == rhs); }

  /*!
   * \brief Determines byte-wise if a UUID is smaller than another UUID.
   * \param[in] lhs The left hand side to compare.
   * \param[in] rhs The right hand side to compare with.
   * \return Whether the UUID is smaller than the other UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  friend constexpr bool operator<(Uuid const& lhs, Uuid const& rhs) noexcept {
    bool smaller{false};

    for (std::size_t i{0}; i < lhs.data_.size(); ++i) {
      if (lhs.data_[i] < rhs.data_[i]) {
        smaller = true;
        break;
      } else if (lhs.data_[i] > rhs.data_[i]) {
        break;
      } else {
        // For lhs.data_[i] equals rhs.data_[i] the next element is compared.
      }
    }

    return smaller;
  }

  /*!
   * \brief Determines byte-wise if a UUID is smaller than or equal to another UUID.
   * \param[in] lhs The left hand side to compare.
   * \param[in] rhs The right hand side to compare with.
   * \return Whether the UUID is smaller than or equal to the other UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  friend constexpr bool operator<=(Uuid const& lhs, Uuid const& rhs) noexcept { return !(lhs > rhs); }

  /*!
   * \brief Determines byte-wise if a UUID is greater than another UUID.
   * \param[in] lhs The left hand side to compare.
   * \param[in] rhs The right hand side to compare with.
   * \return Whether the UUID is greater than the other UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  friend constexpr bool operator>(Uuid const& lhs, Uuid const& rhs) noexcept {
    bool greater{false};

    for (std::size_t i{0}; i < lhs.data_.size(); ++i) {
      if (lhs.data_[i] > rhs.data_[i]) {
        greater = true;
        break;
      } else if (lhs.data_[i] < rhs.data_[i]) {
        break;
      } else {
        // For lhs.data_[i] equals rhs.data_[i] the next element is compared.
      }
    }

    return greater;
  }

  /*!
   * \brief Determines byte-wise if a UUID is greater than or equal to another UUID.
   * \param[in] lhs The left hand side to compare.
   * \param[in] rhs The right hand side to compare with.
   * \return Whether the UUID is greater than or equal to the other UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  friend constexpr bool operator>=(Uuid const& lhs, Uuid const& rhs) noexcept { return !(lhs < rhs); }

 public:
  /*!
   * \brief Length of a UUID string in 8-4-4-4-12 representation.
   * \vpublic
   */
  constexpr static size_t kUuidStringLength{8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12};

  /*!
   * \brief byte length of a UUID.
   * \vpublic
   */
  constexpr static std::uint32_t kUuidByteLength{16};

  /*!
   * \brief typedef for raw data of UUID.
   * \vpublic
   */
  using UuidData = ara::core::Span<std::uint8_t const>;

  /*!
   * \brief typedef for string representation of UUID.
   * \vpublic
   */
  using UuidString = ara::core::String;

  /*!
   * \brief Alias for the char type.
   * \vpublic
   */
  using CharacterType = char;

  /*!
   * \brief Generates an empty UUID containing zeros
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr Uuid() = default;

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_special_member_function
  /*!
   * \brief Destructs the UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  ~Uuid() noexcept = default;

  /*!
   * \brief Copies a UUID
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr Uuid(Uuid const&) = default;

  /*!
   * \brief Moves a UUID
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr Uuid(Uuid&&) noexcept = default;

  /*!
   * \brief Copy assigns a UUID
   * \return A reference to the assignee
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Uuid& operator=(Uuid const&) & = default;

  /*!
   * \brief Moves assigns a UUID
   * \return A reference to the assignee
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  Uuid& operator=(Uuid&&) & noexcept = default;

  /*!
   * \brief Converts the UUID to a string with appropriate format. Note: Letters are lower case.
   * \return String representation of the UUID.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   * \internal
   * - Loop over the \c data_ array
   *   - Convert each byte to its hexadecimal representation and write it to the stringstream.
   *   - If the current position should be a dash in a UUID string add write a "-" to the stringstream.
   * - Return the string created by the stringstream.
   * \endinternal
   */
  UuidString ToString() const {
    UuidString str{"00000000-0000-0000-0000-000000000000"};
    std::size_t str_pos{0};

    for (std::uint8_t i{0}; i < static_cast<std::uint8_t>(data_.size()); ++i) {
      constexpr static std::array<std::int8_t, 16> hexval{
          {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}};
      std::size_t high_byte_index{};
      std::size_t lower_byte_index{};
      // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
      high_byte_index = static_cast<std::size_t>(data_[i]) >> static_cast<std::size_t>(4);
      // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
      lower_byte_index = static_cast<std::size_t>(data_[i]) & static_cast<std::size_t>(0x0f);
      str[str_pos++] = hexval[high_byte_index];
      str[str_pos++] = hexval[lower_byte_index];

      if ((str_pos == 8) ||                            // position after first hex block
          (str_pos == (8 + 1 + 4)) ||                  // position after second hex block
          (str_pos == (8 + 1 + 4 + 1 + 4)) ||          // position after third hex block
          (str_pos == (8 + 1 + 4 + 1 + 4 + 1 + 4))) {  // position after fourth hex block
        str[str_pos++] = '-';                          // Add '-' between hex blocks
      }
    }

    return str;
  }

  /*!
   * \brief Returns the underlying data structure of the UUID
   * \return The UUID data
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr Uuid::UuidData Data() const noexcept { return UuidData{data_}; }

  /*!
   * \brief check whether the generator's identifier is the "Nil UUID" (according to RFC4122).
   * \return true if this identifier is the "Nil UUID"
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  constexpr bool IsNil() const noexcept {
    bool result{true};

    for (std::size_t i{0}; i < kUuidByteLength; ++i) {
      result = result && (data_[i] == 0);
    }

    return result;
  }

  /*!
   * \brief Deserializes a given UUID serialization string.
   * \param[in] input Serialization string.
   * \return The deserialized UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE As long as \c input is not modified.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the serialization string is invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  static constexpr ara::core::Result<Uuid> From(ara::core::StringView const& input) noexcept {
    using R = ara::core::Result<Uuid>;
    Uuid uuid{};

    return (uuid.Parse(input).HasValue()) ? R::FromValue(uuid)
                                          : R::FromError(ara::crypto::SecurityErrc::kInvalidArgument);
  }

  /*!
   * \brief Deserializes a given UUID serialization string.
   * \tparam length Length of the input UUID string.
   * \param[in] input Serialization string.
   * \return The deserialized UUID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE As long as \c input is not modified.
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the serialization string is invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A18.1.1: MD_CRYPTO_AutosarC++17_10-A18.1.1_c_style_array_for_raw_uuid_string
  template <std::size_t length>
  static constexpr ara::core::Result<Uuid> const From(char const (&input)[length]) noexcept {
    static_assert((length - 1) == kUuidStringLength, "Invalid length for UUID string.");
    // VECTOR NL AutosarC++17_10-M5.2.12: MD_CRYPTO_AutosarC++17_10-M5.2.12_checked_pointer_decay
    return From(ara::core::StringView{input, ara::crypto::Uuid::kUuidStringLength});
  }

 private:
  /*!
   * \brief Container for the binary data of the UUID
   * \vprivate Component Private
   */
  std::array<std::uint8_t, kUuidByteLength> data_{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  /*!
   * \brief Checks if a given character is valid in the context of hexadecimal notation.
   * \param[in] a Character.
   * \return True iff the character is a valid hexadecimal digit.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static constexpr bool IsHexDigit(char a) noexcept {
    return (a == '0') || (a == '1') || (a == '2') || (a == '3') || (a == '4') || (a == '5') || (a == '6') ||
           (a == '7') || (a == '8') || (a == '9') || (a == 'a') || (a == 'b') || (a == 'c') || (a == 'd') ||
           (a == 'e') || (a == 'f') || (a == 'A') || (a == 'B') || (a == 'C') || (a == 'D') || (a == 'E') || (a == 'F');
  }

  // VECTOR NC Metric-HIS.VG: MD_CRYPTO_Metric-HIS.VG
  /*!
   * \brief Parses one hex character.
   * \param[in] a Hex character.
   * \return Numeric value of the hex character (0-15).
   * \context ANY
   * \pre \c a must be a valid hex character.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static constexpr std::uint8_t ParseHex(char const a) noexcept {
    std::uint8_t result{16};

    if (a == '0') {
      result = 0;
    } else if (a == '1') {
      result = 1;
    } else if (a == '2') {
      result = 2;
    } else if (a == '3') {
      result = 3;
    } else if (a == '4') {
      result = 4;
    } else if (a == '5') {
      result = 5;
    } else if (a == '6') {
      result = 6;
    } else if (a == '7') {
      result = 7;
    } else if (a == '8') {
      result = 8;
    } else if (a == '9') {
      result = 9;
    } else if ((a == 'a') || (a == 'A')) {
      result = 10;
    } else if ((a == 'b') || (a == 'B')) {
      result = 11;
    } else if ((a == 'c') || (a == 'C')) {
      result = 12;
    } else if ((a == 'd') || (a == 'D')) {
      result = 13;
    } else if ((a == 'e') || (a == 'E')) {
      result = 14;
    } else if ((a == 'f') || (a == 'F')) {
      result = 15;
    } else {
      result = 16;  // Making compliant w.r.t AutosarC++17_10-M6.4.3
    }

    return result;
  }

  /*!
   * \brief Parses two hex characters.
   * \param[in] a Most significant hex character.
   * \param[in] b Least significant hex character.
   * \return Numeric value of the hex character.
   * \context ANY
   * \pre \c a and \c b must be valid hex characters.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  static constexpr std::uint8_t ParseHex(CharacterType const a, CharacterType const b) noexcept {
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_CRYPTO_AutosarC++17_10-A4.7.1_cast_may_truncate_value
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_cast_may_truncate_value
    return static_cast<std::uint8_t>(static_cast<std::uint8_t>((ParseHex(a) << static_cast<std::uint8_t>(4))) |
                                     ParseHex(b));
  }

  /*!
   * \brief Parses a serialized UUID.
   * \param[in] input serialized UUID.
   * \context ANY
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the input is invalid.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  constexpr ara::core::Result<void> Parse(ara::core::StringView const& input) noexcept {
    ara::core::Result<void> result{};

    if (input.size() != kUuidStringLength) {
      result = ara::core::Result<void>{ara::crypto::SecurityErrc::kInvalidArgument};
    } else {
      ara::core::StringView::const_iterator input_iterator{input.cbegin()};
      bool result_has_value{result.HasValue()};  // Making compliant w.r.t AutosarC++17_10-M6.5.6
      for (std::size_t i{0}; (i < kUuidStringLength) && result_has_value; ++i) {
        if ((i == 8) || (i == 13) || (i == 18) || (i == 23)) {
          if (*input_iterator != '-') {
            result = ara::core::Result<void>{ara::crypto::SecurityErrc::kInvalidArgument};
          }
        } else if (!IsHexDigit(*input_iterator)) {
          result = ara::core::Result<void>{ara::crypto::SecurityErrc::kInvalidArgument};
        } else {
          // intentionally left empty
        }
        result_has_value = result.HasValue();
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
      }

      if (result.HasValue()) {
        input_iterator = input.cbegin();

        char first_digit{*input_iterator};
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        char second_digit{*input_iterator};
        std::get<0>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<1>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<2>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<3>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<4>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<5>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<6>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<7>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<8>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<9>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<10>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<11>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<12>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<13>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<14>(data_) = ParseHex(first_digit, second_digit);
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;

        first_digit = *input_iterator;
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_CRYPTO_AutosarC++17_10-M5.0.16_pointer_outside_bound
        ++input_iterator;
        second_digit = *input_iterator;
        std::get<15>(data_) = ParseHex(first_digit, second_digit);
      }
    }

    return result;
  }
};

/*!
 * \brief The Globally Unique Identifier (GUID) is an alias of Universally Unique Identifier (UUID).
 * \vpublic
 */
using Guid = Uuid;

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_GUID_H_
