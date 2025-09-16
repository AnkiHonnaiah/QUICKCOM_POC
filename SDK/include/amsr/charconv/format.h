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
 *        \brief  Formats arguments according to a given format string.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_FORMAT_H_
#define LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_FORMAT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/charconv/to_textual.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {

/*!
 * \brief Formats given arguments according to a provided format string.
 * \unit amsr::charconv::Format
 * \vprivate Product Private
 */
class Format final {
 public:
  /*!
   * \brief Constructs an instance of the Format class with the given buffer.
   * \param[in] buffer_in Memory segment for the formatted string.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit Format(amsr::core::Span<char> buffer_in) noexcept : buffer_{buffer_in} {}

  /*!
   * \brief Move constructs instance of class.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  Format(Format&&) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \return Moved Format
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  auto operator=(Format&&) & noexcept -> Format& = default;

  // Format is not copy constructible because it would break exclusive management of the resource.
  Format(Format const&) = delete;

  // Format is not copy constructible because it would break exclusive management of the resource.
  auto operator=(Format const&) -> Format& = delete;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~Format() noexcept = default;

  /*!
   * \brief Get the maximum length of the buffer.
   * \return The maximum length of the buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  auto GetMaxLength() const noexcept -> std::size_t;

  /*!
   * \brief Replaces each '{}' in the given format string with the textual representation of the respective argument.
   * \tparam Args Types of the given arguments.
   *              Types are limited to bool, integer types, floating point types, char or void pointers,
   *              amsr::core::StringView and durations in ns, us, ms, s, m or h.
   * \param[in] fmt  The format.
   * \param[in] args Arguments to place in the format string.
   * \return Textual representation of the arguments according to the format.
   * \internal
   * - write format with arguments
   * - if successful
   *   - return text
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant FALSE
   * \trace DSGN-CharacterConversion-Format
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  template <typename... Args>
  auto Apply(amsr::core::StringView fmt, Args const&... args) noexcept -> amsr::core::Result<amsr::core::StringView> {
    amsr::core::Result<amsr::core::StringView> result{CharconvErrc::kValueTooLarge};
    char const* const first{buffer_.data()};
    bool is_truncated{Write(fmt, args...)};        // VCA_CHARCONV_FORMAT_APPLY_REF_PARAM
    if ((!is_truncated) && (fmt.length() > 0U)) {  // COV_CHARCONV_RemainingFormatAvailable
      is_truncated = Write(fmt);
    }
    if (!is_truncated) {
      // VECTOR NC AutosarC++17_10-M5.0.9: MD_CharacterConversion_M5.0.9_conversion_changes_signedness
      std::size_t const length{static_cast<std::size_t>(buffer_.data() - first)};
      amsr::core::StringView string_result{amsr::core::StringView{first, length}};
      result.EmplaceValue(string_result);
    }
    return result;
  }

 private:
  /*!
   * \brief Replaces each '{}' in the given format string with the textual representation of the respective
   * argument.
   * \tparam T Type of the first argument.
   * \tparam Args Types of the given arguments.
   * \param[in] fmt The format.
   * \param[in] value First argument.
   * \param[in] args Remaining arguments.
   * \return true if the resulting text is truncated, false otherwise.
   * \internal
   * - write format with first argument
   * - if successful
   *   - write format with further arguments
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  template <typename T, typename... Args>
  auto Write(amsr::core::StringView& fmt, T const& value, Args const&... args) noexcept -> bool {
    bool is_truncated{Write(fmt, value)};  // VCA_CHARCONV_FORMAT_APPLY_REF_PARAM_VALID
    if (!is_truncated) {
      is_truncated = Write(fmt, args...);  // VCA_CHARCONV_FORMAT_APPLY_REF_PARAM_VALID
    }
    return is_truncated;
  }

  /*!
   * \brief Replaces the first '{}' in the given format string with the textual representation of the argument.
   * \tparam T Type of the argument.
   * \param[in] fmt The format.
   * \param[in] value The argument.
   * \return true if the resulting text is truncated, false otherwise.
   * \internal
   * - check if a placeholder can be found in format string
   * - if not
   *   - write only format string
   * - otherwise
   *   - write format from beginning to first placeholder
   *   - if successful
   *     - write argument
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  template <typename T>
  auto Write(amsr::core::StringView& fmt, T const& value) noexcept -> bool {
    std::size_t const placeholder_pos{fmt.find("{}")};  // VCA_CHARCONV_BASICSTRING_FIND_CONTRACT
    bool is_truncated{false};
    if (placeholder_pos == amsr::core::StringView::npos) {
      is_truncated = Write(fmt);
    } else {
      is_truncated = Write(amsr::core::StringView(fmt.data(), placeholder_pos));
      if (!is_truncated) {
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_arithmetic_computation_may_cause_wrap_around
        // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_arithmetic_computation_may_cause_wrap_around
        fmt.remove_prefix(placeholder_pos + 2U);
        is_truncated = Write(value);  // VCA_CHARCONV_FORMAT_APPLY_REF_PARAM_VALID
      }
    }
    return is_truncated;
  }

  /*!
   * \brief Converts an integer type value to a textual representation in the given base and writes it to the buffer.
   * \tparam T The integer type.
   * \param[in] value The integer value.
   * \param[in] base The base.
   * \return true if the textual representation is too large, false otherwise.
   * \internal
   * - convert value to text
   * - if successful
   *   - decrease buffer size
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  template <typename T, std::enable_if_t<std::is_integral<T>::value && (!std::is_same<T, bool>::value) &&
                                             (!std::is_same<char, T>::value),
                                         bool> = true>
  auto Write(T value, NumberBase base = NumberBase::kDecimal) noexcept -> bool {
    bool is_truncated{false};
    ToTextual<T> converter{buffer_, value};
    amsr::core::Result<amsr::core::Span<char>> conversion_result{converter.Convert(base)};
    if (conversion_result.HasValue()) {
      buffer_ = buffer_.subspan(conversion_result.Value().size());
    } else {
      is_truncated = true;
    }
    return is_truncated;
  }

  /*!
   * \brief Converts a floating point type value to a textual representation and writes it to the buffer.
   * \tparam T The floating point type.
   * \param[in] value The floating point value.
   * \return true if the textual representation is too large, false otherwise.
   * \internal
   * - convert value to text
   * - if successful
   *   - decrease buffer size
   * \endinternal
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   */
  template <typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
  auto Write(T value) noexcept -> bool {
    bool is_truncated{false};
    ToTextual<T> converter{buffer_, value};
    amsr::core::Result<amsr::core::Span<char>> conversion_result{converter.Convert()};
    if (conversion_result.HasValue()) {
      buffer_ = buffer_.subspan(conversion_result.Value().size());
    } else {
      is_truncated = true;
    }
    return is_truncated;
  }

  /*!
   * \brief Writes a char to the buffer.
   * \param[in] value The character to write.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(char value) noexcept -> bool;

  /*!
   * \brief Writes a string view to the buffer.
   * \details \p value is empty after writing it to the buffer.
   * \param[in] value The text to write.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(amsr::core::StringView& value) noexcept -> bool;

  /*!
   * \brief Writes a string view to the buffer.
   * \param[in] value The text to write.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(amsr::core::StringView const& value) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a boolean to the buffer.
   * \param[in] value The boolean value.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(bool value) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a char pointer to the buffer.
   * \param[in] ptr The char pointer.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(char const* ptr) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a void pointer to the buffer.
   * \param[in] ptr The void pointer.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(void const* ptr) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a duration in nanoseconds to the buffer.
   * \param[in] duration The duration in nanoseconds.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(std::chrono::nanoseconds const& duration) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a duration in microseconds to the buffer.
   * \param[in] duration The duration in microseconds.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(std::chrono::microseconds const& duration) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a duration in milliseconds to the buffer.
   * \param[in] duration The duration in milliseconds.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(std::chrono::milliseconds const& duration) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a duration in seconds to the buffer.
   * \param[in] duration The duration in seconds.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(std::chrono::seconds const& duration) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a duration in minutes to the buffer.
   * \param[in] duration The duration in minutes.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(std::chrono::minutes const& duration) noexcept -> bool;

  /*!
   * \brief Writes a textual representation of a duration in hours to the buffer.
   * \param[in] duration The duration in hours.
   * \return true if the text is truncated, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE for different instances of this class
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  auto Write(std::chrono::hours const& duration) noexcept -> bool;

  /*! Memory segment. */
  amsr::core::Span<char> buffer_;
};

}  // namespace charconv
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_CHARCONV_RemainingFormatAvailable
//   \ACCEPT  XF xf
//   \REASON  In case of no arguments, the format string is completely written to the result with the call to Write().
//   Thus, fmt.length() cannot be larger than zero. The code is covered with all other instantiations, i. e. for all
//   calls with one or more arguments.
// COV_JUSTIFICATION_END

#endif  // LIB_CHARCONV_VECTOR_INCLUDE_AMSR_CHARCONV_FORMAT_H_
