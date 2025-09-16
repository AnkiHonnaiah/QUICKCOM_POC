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
 *        \brief  Implementation of safe conversion between integral-like types.
 *         \unit amsr::charconv::SafeConversion
 *
 *********************************************************************************************************************/
#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_INTERNAL_SAFE_CONVERSION_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_INTERNAL_SAFE_CONVERSION_H_

#include <cstdint>
#include <limits>
#include <sstream>
#include <type_traits>

#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "amsr/generic/generic_error_domain.h"
#include "vac/language/compile_time.h"

namespace amsr {
namespace charconv {

namespace internal {

/*!
 * \brief List of allowed conversion types.
 */
template <typename T>
using AllowedConversionType = std::is_integral<T>;

// VECTOR NC AutosarC++17_10-M5.0.4: MD_CharacterConversion_M5.0.4_change_in_signedness_positive
/*!
 * \brief The maximum finite value representable by the numeric type T as maximum-width unsigned value.
 * \tparam T Data type for which its maximum-width unsigned value should be determined.
 */
template <typename T>
constexpr static std::uintmax_t kMax{static_cast<std::uintmax_t>(std::numeric_limits<T>::max())};

// VECTOR NC AutosarC++17_10-M5.0.4: MD_CharacterConversion_M5.0.4_change_in_signedness_negative
/*!
 * \brief The lowest finite value representable by the numeric type T as maximum-width signed value.
 * \tparam T Data type for which its maximum-width signed value should be determined.
 */
template <typename T>
constexpr static std::intmax_t kLowest{static_cast<std::intmax_t>(std::numeric_limits<T>::lowest())};

/*!
 * \brief Whether a positive value can always be safely casted from From to To.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 */
template <typename From, typename To>
constexpr static bool kIsAlwaysWithinMax{kMax<From> <= kMax<To>};

/*!
 * \brief Whether a negative value can always be safely casted from From to To.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 */
template <typename From, typename To>
constexpr static bool kIsAlwaysWithinLowest{kLowest<From> >= kLowest<To>};

/*!
 * \brief Whether a positive or negative value can always be safely casted from From to To.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 */
template <typename From, typename To>
constexpr static bool kInfalliblyConvertible{kIsAlwaysWithinMax<From, To> && kIsAlwaysWithinLowest<From, To>};

// VECTOR NC AutosarC++17_10-A2.11.4: MD_CharacterConversion_A2.11.4_name_reused
/*!
 * \brief Checks if a value of type From is smaller or equal to the maximum finite value representable of To.
 * \details Specialization for unsigned types.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value which should be casted.
 * \returns \c true if value is smaller or equal to maximum finite value representable by To, \c false
 * otherwise.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename From, typename To, std::enable_if_t<std::is_unsigned<From>::value, bool> = true>
constexpr static auto IsWithinMax(From from) noexcept -> bool {
  return static_cast<std::uintmax_t>(from) <= kMax<To>;
}

/*!
 * \brief Checks if a value of type From is smaller or equal to the maximum finite value representable of To.
 * \details Specialization for signed types.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value which should be casted.
 * \returns \c true if value is negative or smaller or equal to maximum finite value representable by To, \c false
 * otherwise.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename From, typename To, std::enable_if_t<std::is_signed<From>::value, bool> = true>
constexpr static auto IsWithinMax(From from) noexcept -> bool {
  return (from < static_cast<From>(0)) ? true : (static_cast<std::uintmax_t>(from) <= kMax<To>);
}

// VECTOR NC AutosarC++17_10-A2.11.4: MD_CharacterConversion_A2.11.4_name_reused
/*!
 * \brief Checks if a value of type From is greater or equal to the lowest finite value representable of To.
 * \details Specialization for unsigned types.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value which should be casted.
 * \returns \c true.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename From, typename To, std::enable_if_t<std::is_unsigned<From>::value, bool> = true>
constexpr static auto IsWithinLowest(From from) noexcept -> bool {
  static_cast<void>(from);  // always within lowest independent of from
  return true;
}

/*!
 * \brief Checks if a value of type From is greater or equal to the lowest finite value representable of To.
 * \details Specialization for signed types.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value which should be casted.
 * \returns \c true if value is positive or smaller or equal to lowest finite value representable by To, \c false
 * otherwise.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename From, typename To, std::enable_if_t<std::is_signed<From>::value, bool> = true>
constexpr static auto IsWithinLowest(From from) noexcept -> bool {
  return (from >= static_cast<From>(0)) ? true : (static_cast<std::intmax_t>(from) >= kLowest<To>);
}

}  // namespace internal

/*!
 * \brief Casts from to type To if the cast can never fail.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value to convert.
 * \return Casted value.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto Convert(From from) noexcept -> To {
  static_assert(internal::AllowedConversionType<To>::value, "To type not supported.");
  static_assert(internal::AllowedConversionType<From>::value, "From type not supported.");
  return static_cast<To>(from);
}

/*!
 * \brief Checks if a value can be converted to To, i.e. is in range of To.
 * \details Specialization for infallible conversions.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value to convert.
 * \return Always \c true.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto IsConvertible(From from) noexcept -> bool {
  static_assert(internal::AllowedConversionType<To>::value, "To type not supported.");
  static_assert(internal::AllowedConversionType<From>::value, "From type not supported.");
  static_cast<void>(from);  // return value independent of from
  return true;
}

/*!
 * \brief Checks if a value can be converted to To, i.e. is in range of To.
 * \details Specialization for conversions which can fail.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value to convert.
 * \return \c true if converted value is in range of To, otherwise \c false.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<not internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto IsConvertible(From from) noexcept -> bool {
  static_assert(internal::AllowedConversionType<To>::value, "To type not supported.");
  static_assert(internal::AllowedConversionType<From>::value, "From type not supported.");

  bool const max_fits{internal::kIsAlwaysWithinMax<From, To> ? true : internal::IsWithinMax<From, To>(from)};
  bool const lowest_fits{internal::kIsAlwaysWithinLowest<From, To> ? true : internal::IsWithinLowest<From, To>(from)};

  return max_fits && lowest_fits;
}

/*!
 * \brief Casts from to type To or aborts.
 * \details Specialization for infallible conversions.
 * \tparam From Original data type.
 * \tparam To Destination data type.
 * \param[in] from Value to convert.
 * \return Casted value.
 * \context ANY
 * \pre Value to be converted has to be in range of To.
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto ConvertOrAbort(From from) noexcept -> To {
  return static_cast<To>(from);
}

/*!
 * \brief Casts from to type To or aborts.
 * \details Specialization for not infallible conversions.
 * \tparam To Type to be casted to.
 * \tparam From Type of num.
 * \param[in] from Value to convert.
 * \return Casted value.
 * \context ANY
 * \pre Value to be converted has to be in range of To.
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<not internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto ConvertOrAbort(From from) noexcept -> To {
  if (!IsConvertible<To>(from)) {
    amsr::core::Abort("amsr::charconv::ConvertOrAbort(From): Value out of target range.");
  }

  // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_cast_may_truncate_value_general
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_cast_may_truncate_value_general
  return static_cast<To>(from);
}

/*!
 * \brief Tries to convert \c from to type To.
 * \details Specialization for infallible conversions.
 * \tparam To Type to be casted to.
 * \tparam From Type of num.
 * \param[in] from Value to convert.
 * \return Casted value.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto TryConvert(From from) noexcept -> amsr::core::Result<To> {
  return amsr::core::Result<To>{static_cast<To>(from)};
}

/*!
 * \brief Tries to convert \c from to type To.
 * \details Specialization for not infallible conversions.
 * \tparam To Type to be casted to.
 * \tparam From Type of num.
 * \param[in] from Value to convert.
 * \return Casted value.
 * \context ANY
 * \pre -
 * \error GenErrc::kRuntime If conversion failed.
 * \threadsafe TRUE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::charconv::SafeConversion
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <typename To, typename From, std::enable_if_t<not internal::kInfalliblyConvertible<From, To>, char> = 0>
constexpr auto TryConvert(From from) noexcept -> amsr::core::Result<To> {
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_CharacterConversion_M0.3.1_cast_may_truncate_value_general
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_CharacterConversion_A4.7.1_cast_may_truncate_value_general
  return (IsConvertible<To>(from)) ? amsr::core::Result<To>::FromValue(static_cast<To>(from))
                                   : amsr::core::Result<To>::FromError(generic::GenErrc::kRuntime);
}

}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_INTERNAL_SAFE_CONVERSION_H_
