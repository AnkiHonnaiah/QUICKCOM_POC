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
/*!        \file
 *        \brief  Contains the content data object.
 *      \details
 *
 *         \unit LogAndTrace::ConfigParser::ContentData
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_CONTENT_DATA_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_CONTENT_DATA_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstdint>
#include <utility>

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/core/variant.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/log_level.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

/*!
 * \brief The configuration value.
 */
// VCA_LOGAPI_MOLE_1298
class ContentData {
  /*! \brief The only supported integer type. */
  using IntType = std::int64_t;
  /*! \brief The internal container for supported data. */
  core::Variant<bool, IntType, double, LogLevel, core::String> data_{};

 public:
  /*!
   * \brief Constructs a new Content Data object.
   * \spec
   *   requires true;
   * \endspec
   */
  ContentData() noexcept = default;
  /*!
   * \brief Constructs a new Content Data object using a bool.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_LOG_AutosarC++17_10-A12.1.5
  explicit ContentData(bool value) noexcept : data_{value} {}
  /*!
   * \brief Constructs a new Content Data object using an int.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ContentData(IntType value) noexcept : data_{value} {}
  /*!
   * \brief Constructs a new Content Data object using a double.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ContentData(double value) noexcept : data_{value} {}
  /*!
   * \brief Constructs a new Content Data object using a log level.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ContentData(LogLevel value) noexcept : data_{value} {}
  /*!
   * \brief Constructs a new Content Data object using a string view.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ContentData(core::StringView value) noexcept
      // VCA_LOGAPI_CHECKED_CONTRACT
      : data_{core::String{value}} {}

  // Setters
  /*!
   * \brief Inserts a bool into the value.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(bool const b) & noexcept -> ContentData& {
    this->data_.emplace<bool>(b);
    return *this;
  }

  /*!
   * \brief Inserts an integer into the value.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(IntType const i) & noexcept -> ContentData& {
    // VCA_LOGAPI_VALID_OBJECT
    this->data_.emplace<IntType>(i);
    // VCA_LOGAPI_VALID_OBJECT
    return *this;
  }

  /*!
   * \brief Inserts a float into the value.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(double const d) & noexcept -> ContentData& {
    this->data_.emplace<double>(d);
    return *this;
  }

  /*!
   * \brief Inserts a log level into the value.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(LogLevel const ll) & noexcept -> ContentData& {
    this->data_.emplace<LogLevel>(ll);
    return *this;
  }

  /*!
   * \brief Inserts a string into the value.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(core::String&& s) & noexcept -> ContentData& {
    // VCA_LOGAPI_VALID_OBJECT
    this->data_.emplace<core::String>(std::move(s));
    // VCA_LOGAPI_VALID_OBJECT
    return *this;
  }

  /*!
   * \brief Inserts a string into the value.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(core::StringView s) & noexcept -> ContentData& {
    this->data_.emplace<core::String>(std::move(s));
    return *this;
  }

  // Getters
  /*!
   * \brief Extracts a saved bool.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetBool() const& noexcept -> bool {
    // VCA_LOGAPI_VALID_OBJECT
    return core::get<bool>(this->data_);
  }

  /*!
   * \brief Tries to extract a saved bool.
   *
   * \tparam    Errors... The types of arguments given to this function.
   *
   * \param[in] errors    The arguments used for constructing the error.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename... Errors>
  auto TryGetBool(Errors&&... errors) const& noexcept -> core::Result<bool> {
    // VCA_LOGAPI_VALID_OBJECT
    return TryGetInternal<bool, bool>(std::forward<Errors>(errors)...);
  }

  /*!
   * \brief Extracts a saved int.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetInt() const& noexcept -> IntType {
    // VCA_LOGAPI_VALID_OBJECT
    return core::get<IntType>(this->data_);
  }

  /*!
   * \brief   Gets the integer constrained by lower & upper bound.
   *
   * \tparam  Out         The output integer type.
   *                      Must be convertible from ContentData::IntType.
   *                      Must also be signed or unsigned integer.
   * \tparam  Errors...   The types of arguments to construct an amsr::core::ErrorCode.
   *
   * \param   lower       The lower bound that's included.
   * \param   upper       The upper bound that's included.
   * \param   errors      The arguments used for constructing an amsr::core::ErrorCode.
   *
   * \return  The converted value or the error.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Out = IntType, typename... Errors>
  auto GetConstrainedInt(IntType lower, IntType upper, Errors&&... errors) const& noexcept -> Result<Out> {
    IntType const res{this->GetInt()};
    // VECTOR NC AutosarC++17_10-A4.7.1: MD_LOG_AutosarC++17_10-A4.7.1
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1
    return ((res >= lower) && (res <= upper)) ? Result<Out>::FromValue(static_cast<Out>(res))
                                              : Result<Out>::FromError(std::forward<Errors>(errors)...);
  }

  /*!
   * \brief Extracts a saved double.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetFloat() const& noexcept -> double { return core::get<double>(this->data_); }

  /*!
   * \brief Extracts a saved log level.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLogLevel() const& noexcept -> LogLevel {
    // VCA_LOGAPI_VALID_OBJECT
    return core::get<LogLevel>(this->data_);
  }

  /*!
   * \brief Tries to extract a saved log level.
   *
   * \tparam    Errors... The types of arguments to construct an amsr::core::ErrorCode.
   *
   * \param[in] errors    The arguments used for constructing an amsr::core::ErrorCode.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename... Errors>
  auto TryGetLogLevel(Errors&&... errors) const& noexcept -> Result<LogLevel> {
    // VCA_LOGAPI_VALID_OBJECT
    return TryGetInternal<LogLevel, LogLevel>(std::forward<Errors>(errors)...);
  }

  /*!
   * \brief Extracts a saved string.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetString() const& noexcept -> core::String const& {
    // VCA_LOGAPI_VALID_OBJECT
    return core::get<core::String>(this->data_);
  }

  /*!
   * \brief Tries to extract a saved string.
   *
   * \tparam    Errors... The types of arguments to construct an amsr::core::ErrorCode.
   *
   * \param[in] errors    The arguments used for constructing an amsr::core::ErrorCode.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename... Errors>
  auto TryGetString(Errors&&... errors) const& noexcept -> Result<core::StringView> {
    return TryGetInternal<core::StringView, core::String>(std::forward<Errors>(errors)...);
  }

 private:
  /*!
   * \brief     Tries to extract a saved value.
   *
   * \tparam    out       The resulting type.
   *                      Must be either a bool, amsr::log::LogLevel, or an amsr::core::StringView.
   * \tparam    Stored    The Log type that is stored.
   *                      Must be either a bool, amsr::log::LogLevel, or an amsr::core::String.
   * \tparam    Errors... The types of arguments given to this function.
   * \param[in] errors    The arguments used for constructing an amsr::core::ErrorCode.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Out, typename Stored, typename... Errors>
  auto TryGetInternal(Errors&&... errors) const& noexcept -> core::Result<Out> {
    using RetVal = Result<Out>;
    Stored const* const ptr{core::get_if<Stored>(&this->data_)};
    return (ptr == nullptr) ? RetVal::FromError(std::forward<Errors>(errors)...) :
                            // VCA_LOGAPI_VALID_OBJECT
               RetVal::FromValue(*ptr);
  }
};

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_CONTENT_DATA_H_
