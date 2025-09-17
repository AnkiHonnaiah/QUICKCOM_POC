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
/*!        \file  amsr/asn1/structure/asn1_primitive_types.h
 *        \brief  Representation of ASN.1 primitive values
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ASN1_PRIMITIVE_TYPES_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ASN1_PRIMITIVE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <regex>
#include "amsr/asn1/structure/general_type.h"
#include "amsr/asn1/structure/numerical_type.h"
#include "amsr/asn1/validator/internal/validator.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace asn1 {
namespace structure {
/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Boolean = bool;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Integer = NumericalType<std::uint8_t>;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using NumberOfUnusedBits = std::uint8_t;

/*!
 * \brief Type alias for BIT STRING. The first element must not include the byte indicating the number of unused bits.
 *  That number is to be specified in the second argument.
 * \vprivate Product Private
 */
using BitString = std::pair<GeneralType<std::uint8_t>, NumberOfUnusedBits>;

/*!
 * \brief Informationless data type.
 * \vprivate Product Private
 */
class Null {
 public:
  /*!
   * \brief Default constructor.
   * \vprivate Product Private
   */
  Null() = default;

  /*!
   * \brief Default destructor.
   * \vprivate Product Private
   */
  virtual ~Null() = default;

  /*!
   * \brief Equality operator for Null type. Nulls are theoretically never equal, but this method returns
   *   true nonetheless. The reasoning behind this is that it is intended to be used as an "is null" check for tests and
   *    collections. Comparing two null lvalues is probably always wrong.
   *  \return \c true
   */
  auto operator==(Null const&) const noexcept -> bool { return true; };

  /*!
   * \brief Inequality operator for Null type. See equality operator for more information on equality between NULLs.
   * \param[in] other Other value
   * \return \c false
   */
  auto operator!=(Null const& other) const noexcept -> bool { return !(*this == other); };

  /*!
   * \brief Copy constructor.
   * \param other The instance to copy.
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_null_ctors_not_protected
  Null(Null const& other) = default;

  /*!
   * \brief Default move constructor.
   * \param other The instance to move.
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_null_ctors_not_protected
  Null(Null&& other) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \param other The instance to copy.
   * \return \c *this
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_null_ctors_not_protected
  auto operator=(Null const& other) & -> Null& = default;

  /*!
   * \brief Default move assignment operator.
   * \param other The instance to move.
   * \return \c *this
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A12.8.6: MD_VAASN1_AutosarC++17_10-A12.8.6_null_ctors_not_protected
  auto operator=(Null&& other) & noexcept -> Null& = default;
};

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using OctetString = GeneralType<std::uint8_t>;

/*!
 * \brief Raw data type for ANY fallback.
 * \vpublic
 */
class RawData : public GeneralType<std::uint8_t> {
 public:
  /*!
   * \brief Inherit constructors
   */
  using GeneralType<std::uint8_t>::GeneralType;
};

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Oid = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Real = double;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using ObjectDescriptor = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Utf8String = GeneralType<std::uint8_t>;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using RelativeOid = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Time = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using PrintableString = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using VideotexString = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using GraphicString = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Date = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using TimeOfDay = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using DateTime = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Enumerated = NumericalType<std::uint8_t>;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using GeneralString = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using Duration = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using ContextSpecificData = GeneralType<std::uint8_t>;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_VAASN1_AutosarC++17_10-M3.4.1_global
using ApplicationData = GeneralType<std::uint8_t>;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
// VECTOR NC AutosarC++17_10-M3.4.1: MD_VAASN1_AutosarC++17_10-M3.4.1_global
using PrivateData = GeneralType<std::uint8_t>;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using OIDInternationalized = amsr::core::String;

/*!
 * \brief Type alias.
 * \vprivate Product Private
 */
using RelativeOIDInternationalized = amsr::core::String;

/*!
 * \brief Class for BaseString.
 * \vprivate Product Private
 */
class BaseString {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  BaseString() noexcept = default;

  /*!
   * \brief Constructor.
   * \param[in] string BaseString in String format.
   * \vprivate Product Private
   */
  explicit BaseString(amsr::core::String string) : data_{std::move(string)} {}

  /*!
   * \brief Constructor.
   * \param[in] string BaseString in StringView format.
   * \vprivate Product Private
   */
  explicit BaseString(amsr::core::StringView string) : BaseString{amsr::core::String{string}} {}

  /*!
   * \brief Getter string data.
   * \return stored string data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetData() const noexcept -> amsr::core::StringView { return data_; }

  /*!
   * \brief Overloaded operator ().
   * \return Reference to stored string data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  explicit operator amsr::core::String&() { return data_; }

  /*!
   * \brief Overloaded operator ().
   * \return Const reference to stored string data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  explicit operator amsr::core::String const&() const { return data_; }

  /*!
   * \brief Equality operator.
   * \param[in] other Value to be compared.
   * \return \c true if same, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator==(BaseString const& other) const noexcept -> bool { return (data_ == other.data_); }

  /*!
   * \brief Comparison operator.
   * \param[in] other Other value.
   * \return \c true if not same, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto operator!=(BaseString const& other) const noexcept -> bool { return (data_ != other.data_); }

  /*!
   * \brief Method to get the size of the string.
   * \return size of the string data.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto size() const -> std::size_t { return data_.size(); }

  /*!
   * \brief Destructor.
   */
  virtual ~BaseString() = default;

 protected:
  /*!
   * \brief Copy Constructor.
   * \return -
   * \param[in] string BaseString to copy.
   */
  BaseString(BaseString const& string) : data_(string.data_) {}

  /*!
   * \brief Move constructor.
   * \return -
   */
  BaseString(BaseString&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   * \return -
   */
  auto operator=(BaseString const&) & noexcept -> BaseString& = default;

  /*!
   * \brief Default Move assignment.
   * \return -
   */
  auto operator=(BaseString&&) & noexcept -> BaseString& = default;

 private:
  amsr::core::String data_{}; /*!< Data representation for BaseString. */
};

/*!
 * \brief Class for Ia5String
 */
class Ia5String : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

/*!
 * \brief Class for VisibleString
 */
class VisibleString : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

/*!
 * \brief Class for NumericString.
 */
class NumericString : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

/*!
 * \brief Class for T61String.
 */
class T61String : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

/*!
 * \brief Class for BMPString.
 */
class BMPString : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

/*!
 * \brief Class for UniversalString.
 */
class UniversalString : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

/*!
 * \brief Class for GeneralizedTime.
 */
class GeneralizedTime : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;

  /*!
   * \brief Validates the format of the generalized time string.
   * \details Values are locally checked (month is between 01 and 12, day is between 01 and 31, etc.), but months
   *   are not cross-checked with days, so e.g. Feb 31st will be deemed valid. Leap seconds can only occur at the end
   *   of June or December, but it is not checked whether the given year actually had leap seconds. Consequently, leap
   *   years are not checked. ISO8601 would dictate this, so this validation is weaker than the standard. Fractionals
   *   are only allowed for seconds because fractional hours or minutes seem unconventional enough to indicate an error.
   *   DER specifics (trailing zeroes in fractionals, UTC must be Z...) are not explicitly checked here.
   * \return true iff the string conforms to \c YYYYMMDDhh[mm[ss[(.|,)f{1,4}]]][((+|-)hh[mm])|Z] and leap seconds, if
   *   they occur, occur on June 30th or December 31st at 23:59:60.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto IsValid() const noexcept -> bool {
    amsr::core::StringView::iterator iter{GetData().begin()};
    bool retval{CheckMandatories(iter)};

    // If there is input left, check for minutes.
    bool has_valid_minute{false};

    if (retval) {
      has_valid_minute = CheckMinute(iter);
    }

    // If there is input left after a valid minute, check for seconds.
    if (has_valid_minute) {
      static_cast<void>(CheckSecond(iter));
    }

    // If there is input left, check for zone time offset.
    if (retval && (iter < GetData().end())) {
      retval = CheckZoneTimeOffset(iter);
    }

    // If there is input left, the string is invalid.
    if (iter < GetData().end()) {
      retval = false;
    }

    return retval;
  }

 private:
  /*!
   * \brief Checks whether there are the mandatory parts of a GeneralizedTime structure at the current position.
   *  \c iter is advanced past the end of the data.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  auto CheckMandatories(amsr::core::StringView::iterator& iter) const noexcept -> bool {
    // YYYYMMDDhh is at least 10 characters long.
    constexpr std::size_t kMinLength{10u};
    bool retval{GetData().size() >= kMinLength};

    if (retval) {
      retval = CheckYear(iter);
    }
    if (retval) {
      retval = CheckMonth(iter);
    }
    if (retval) {
      retval = CheckDay(iter);
    }
    if (retval) {
      retval = CheckHour(iter);
    }

    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 year at the current position. \c iter is advanced past the end of
   *   the data.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre \c iter has at least 4 characters left
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  static auto CheckYear(amsr::core::StringView::iterator& iter) noexcept -> bool {
    // The year can be any four-digit number.
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAASN1_AutosarC++17_10-M5.0.15_pointer_arithmetic
    bool const retval{std::count_if(iter, iter + 4, [](char dig) { return (dig >= '0') && (dig <= '9'); }) == 4};
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAASN1_AutosarC++17_10-M5.0.15_pointer_arithmetic
    iter = std::next(iter, 4);
    // NOTE: This does not reset the iterator in case of an invalid value because this warrants an early exit.
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 month at the current position.
   * \details On success, the iterator is moved one past the end of the checked value.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre \c iter has at least 2 characters left
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  static auto CheckMonth(amsr::core::StringView::iterator& iter) noexcept -> bool {
    bool retval{false};

    amsr::core::StringView::const_reference first_digit{*iter};
    amsr::core::StringView::const_reference second_digit{*std::next(iter)};

    // The month must be between 01 and 12.
    if (first_digit == '0') {
      retval = (second_digit >= '1') && (second_digit <= '9');
    } else if (first_digit == '1') {
      retval = (second_digit >= '0') && (second_digit <= '2');
    } else {
      // Value was not valid to begin with.
    }
    // NOTE: This does not reset the iterator in case of an invalid value because this warrants an early exit.
    iter = std::next(iter, 2);
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 day at the current position.
   * \details On success, the iterator is moved one past the end of the checked value.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre \c iter has at least 2 characters left
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  static auto CheckDay(amsr::core::StringView::iterator& iter) noexcept -> bool {
    bool retval{false};

    amsr::core::StringView::const_reference first_digit{*iter};
    amsr::core::StringView::const_reference second_digit{*std::next(iter)};

    // The day must be between 01 and 31.
    if (first_digit == '0') {
      retval = (second_digit >= '1') && (second_digit <= '9');
    } else if ((first_digit == '1') || (first_digit == '2')) {
      retval = (second_digit >= '0') && (second_digit <= '9');
    } else if (first_digit == '3') {
      retval = (second_digit >= '0') && (second_digit <= '1');
    } else {
      // Value was not valid to begin with.
    }
    // NOTE: This does not reset the iterator in case of an invalid value because this warrants an early exit.
    iter = std::next(iter, 2);
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 hour at the current position.
   * \details On success, the iterator is moved one past the end of the checked value.
   * \param[in, out] iter The current iterator
   * \param[in] for_zonetime Whether this hour is used for a zone time offset. In that case, 24 is allowed.
   * \return True iff the check was successful.
   * \context ANY
   * \pre \c iter has at least 2 characters left
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  static auto CheckHour(amsr::core::StringView::iterator& iter, bool for_zonetime = false) noexcept -> bool {
    bool retval{false};

    amsr::core::StringView::const_reference first_digit{*iter};
    amsr::core::StringView::const_reference second_digit{*std::next(iter)};

    // The hour must be between 00 and 23.
    if ((first_digit == '0') || (first_digit == '1')) {
      retval = (second_digit >= '0') && (second_digit <= '9');
    } else if (first_digit == '2') {
      // For zone time offsets, formally, +-24 is allowed.
      retval = (second_digit >= '0') && ((second_digit <= '3') || (for_zonetime && (second_digit <= '4')));
    } else {
      // Value was not valid to begin with.
    }
    // NOTE: This does not reset the iterator in case of an invalid value because this warrants an early exit.
    iter = std::next(iter, 2);
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 minute at the current position.
   * \details On success, the iterator is moved one past the end of the checked value. On failure, it is reset to
   *   where it was before.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  auto CheckMinute(amsr::core::StringView::iterator& iter) const noexcept -> bool {
    bool retval{std::distance(iter, GetData().end()) >= 2};
    amsr::core::StringView::iterator const start_pos{iter};

    if (retval) {
      amsr::core::StringView::const_reference first_digit{*iter};
      amsr::core::StringView::const_reference second_digit{*std::next(iter)};

      // If a minute is present, it must be between 00 and 59.
      if ((first_digit >= '0') && (first_digit <= '5')) {
        retval = (second_digit >= '0') && (second_digit <= '9');
        iter = std::next(iter, 2);
      }
    }
    // Reset the iterator if an invalid value was detected so that it can be re-interpreted.
    if (!retval) {
      iter = start_pos;
    }
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 second at the current position.
   * \details On success, the iterator is moved one past the end of the checked value. On failure, it is reset to
   *   where it was before. Leap seconds (a value of 60) may occur on June 30th or December 31st at 23:59:60.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  auto CheckSecond(amsr::core::StringView::iterator& iter) const noexcept -> bool {
    bool retval{std::distance(iter, GetData().end()) >= 2};
    amsr::core::StringView::iterator const start_pos{iter};

    if (retval) {
      amsr::core::StringView::const_reference first_digit{*iter};
      amsr::core::StringView::const_reference second_digit{*std::next(iter)};

      // If a second is present, it must be between 00 and 59, or 60 in case of leap seconds.
      if ((first_digit >= '0') && (first_digit <= '5')) {
        retval = (second_digit >= '0') && (second_digit <= '9');
      } else if (first_digit == '6') {
        retval = second_digit == '0';
        amsr::core::StringView const month_downto_minute{GetData().substr(4u, 8u)};
        bool const is_leap_second_date{(month_downto_minute == "06302359") || (month_downto_minute == "12312359")};
        retval = retval && is_leap_second_date;
      } else {
        // Value was not valid to begin with.
        retval = false;
      }
      iter = std::next(iter, 2);
    }

    // Reset the iterator if an invalid value was detected so that it can be re-interpreted.
    if (!retval) {
      iter = start_pos;
    }

    if (retval && (iter < GetData().end())) {
      // Fractionals need at least a decimal point and one digit.
      retval = CheckFractionals(iter);
    }
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 second fractional part at the current position.
   * \details On success, the iterator is moved one past the end of the checked value. On failure, it is reset to
   *   where it was before.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  auto CheckFractionals(amsr::core::StringView::iterator& iter) const noexcept -> bool {
    // NOTE: ISO 8601 doesn't specify a max amount of fractionals, but 4 places should suffice.
    // There must be at least a decimal point and one digit.
    bool retval{std::distance(iter, GetData().end()) >= 2};
    amsr::core::StringView::iterator const start_pos{iter};

    if (retval) {
      amsr::core::StringView::const_reference decimal_point{*iter};
      amsr::core::StringView::const_reference first_digit{*std::next(iter)};

      // If fractional parts are present, they consist of up to four digits.
      if ((decimal_point == '.') || (decimal_point == ',')) {
        // Mandatory digit
        retval = (first_digit >= '0') && (first_digit <= '9');
        // Advance iter to optional digits
        iter = std::next(iter, 2);
        // Up to three more digits
        std::uint8_t digit_count{0};

        while (retval && (iter < GetData().end()) && (*iter >= '0') && (*iter <= '9')) {
          iter = std::next(iter);
          if (++digit_count > 3) {
            retval = false;
            break;
          }
        }
      }
    }
    // Reset the iterator if an invalid value was detected so that it can be re-interpreted.
    if (!retval) {
      iter = start_pos;
    }
    return retval;
  }

  /*!
   * \brief Checks whether there is a valid ISO8601 zone time offset at the current position.
   * \details On success, the iterator is moved one past the end of the checked value. On failure, it is reset to
   *   where it was before.
   * \param[in, out] iter The current iterator
   * \return True iff the check was successful.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_VAASN1_AutosarC++17_10-A8.4.4_output_parameter_simpler_than_struct
  auto CheckZoneTimeOffset(amsr::core::StringView::iterator& iter) const noexcept -> bool {
    // jharazny: Why do I say "zone time" instead of "time zone"?
    // In my research, I came to the conclusion that "time zone" means two things:
    //  - One of 24 digons ("Kugelzweiecke") as physicists would use to describe timezones. This is geographical.
    //  - A region specified in an IANA database, like "Europe/Berlin" that may change over time and contain DST.
    //    This is political and historical.
    // A "zone time" is just an offset from UTC. For example, "Europe/Berlin" has a zone time of +1 hour during summer.
    // Zone times are not geographical or political, but numerical. Zone times can not be used to deduce time zones in
    // the IANA sense.
    bool retval{std::distance(iter, GetData().end()) >= 1};
    amsr::core::StringView::iterator const start_pos{iter};
    // A time zone offset can be 'Z' or [+-]hh(mm)?
    if (retval) {
      if (*iter == 'Z') {
        // Zulu time; retval is already true.
      } else if ((*iter == '+') || (*iter == '-')) {
        // Explicit offset
        iter = std::next(iter);
        // Mandatory hour from 00 to 24.
        retval = CheckHour(iter, true);
        // NOTE: Values outside of -12 to +14 are implausible, but formally accepted.
        // If there is input left, check for minutes.
        if (retval && (iter < GetData().end())) {
          retval = CheckMinute(iter);
        }
      } else {
        // Value was not valid to begin with.
        retval = false;
      }
      iter = std::next(iter);
    }
    // Reset the iterator if an invalid value was detected so that it can be re-interpreted.
    if (!retval) {
      iter = start_pos;
    }
    return retval;
  }
};

/*!
 * \brief Class for UtcTime.
 */
class UtcTime : public structure::BaseString {
 public:
  /*!
   * \brief Import constructor from parent.
   */
  using BaseString::BaseString;
};

}  // namespace structure

namespace context {
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::Boolean>;
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <>
class Context<structure::Integer>;
}  // namespace context

namespace validator {
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <typename B>
class Validator<B, context::EnableFor<B, structure::Boolean>>;
/*!
 * \brief Forward-declare a template specialization for the type defined in this file.
 */
// VECTOR NC AutosarC++17_10-M14.7.3: MD_VAASN1_AutosarC++17_10-M14.7.3_template_specialization_adheres_to_A14.7.2
template <typename I>
class Validator<I, context::EnableFor<I, structure::Integer>>;
}  // namespace validator
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_STRUCTURE_ASN1_PRIMITIVE_TYPES_H_
