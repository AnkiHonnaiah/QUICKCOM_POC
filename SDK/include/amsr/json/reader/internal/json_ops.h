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
/**       \file     json_ops.h
 *        \brief    Collection of all operations on a JsonData object.
 *
 *        \details  Provides operations for stream based input data.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_JSON_OPS_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_JSON_OPS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/string_view.h"
#include "amsr/iostream/input_stream.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/json/reader/internal/config/json_reader_cfg.h"
#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/reader_fwd.h"
#include "amsr/json/util/json_error_domain.h"
#include "vac/iterators/range.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace json {
namespace internal {
/*! \brief Contains either the character value or an EOF value. */
class OptChar {
  /*! \brief The optional character value */
  std::int64_t char_;

 public:
  /*!
   * \brief Construct the object with a value.
   * \param val The value to construct with.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit OptChar(std::int64_t val) noexcept : char_{val} {}

  /*!
   * \brief Returns true in case EOF was encountered.
   * \return True if EOF was encountered.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto EofFound() const& noexcept -> bool { return this->char_ == -1; }

  /*!
   * \brief Returns true in case a value is contained.
   * \return True if a value is contained.
   * \spec
   *   requires true;
   * \endspec
   */
  auto HasValue() const& noexcept -> bool { return not EofFound(); }

  /*!
   * \brief Returns the character contained.
   * \return The contained character.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Value() const& noexcept -> char {
    AssertCondition(this->char_ != -1, "OptChar::Value: OptChar does not hold a value.");
    return static_cast<char>(this->char_);
  }

  /*!
   * \brief Compare the OptChar with a character for equality.
   * \param candidate The character to compare with the stored one.
   * \return True in case a character is contained and the value is equal.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator==(char const candidate) const noexcept -> bool {
    return this->HasValue() && (this->Value() == candidate);
  }
};

// VECTOR NC Metric-OO.WMC.One: MD_JSON_Metric.OO.WMC.One_json_ops
/*!
 * \brief A handler for operations on a JsonData object.
 */
class JsonOps final {
 public:
  /*!
   * \brief Initializes JsonOps.
   * \param json_data to operate on.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit JsonOps(JsonData& json_data) noexcept;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move constructor.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  JsonOps(JsonOps&&) noexcept = default;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move assignment.
   * \return A reference to the moved into object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(JsonOps&&) & noexcept -> JsonOps& = default;

  /*! \brief Deleted copy constructor. */
  JsonOps(JsonOps const&) = delete;
  /*! \brief Deleted copy assignment. */
  auto operator=(JsonOps const&) & -> JsonOps& = delete;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default Destructor.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~JsonOps() noexcept = default;

  /*!
   * \brief Moves the cursor back one character.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void UnGet() noexcept;

  /*!
   * \brief Returns the character at the current position and moves the cursor to the next character.
   * \return The character at the current position.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto Take() noexcept -> char;

  /*!
   * \brief Tries to take the character at the current position and moves the cursor to the next character.
   * \return The character if inside stream bounds, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the stream has ended}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto TryTake() noexcept -> Result<char>;

  /*!
   * \brief Moves the cursor from the current position to the next position.
   * \return True if the move succeeded.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto Move() noexcept -> bool;

  /*!
   * \brief Gets the current position.
   * \return The current position.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Tell() const noexcept -> Result<std::uint64_t>;

  /*!
   * \brief Moves the cursor to the next position if the passed character is equal to the current character.
   * \param character to compare to.
   * \return True if the characters are equal and the cursor has been moved to the next position, otherwise false.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto Skip(char character) noexcept -> bool;

  /*!
   * \brief Checks if the next characters are equal to the passed string.
   * \param string that is expected.
   * \param error_msg in case the check fails. Must live until the error object is evaluated.
   * \return The empty Result if the string was found, or the error.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if the passed string is empty or it could not be found.}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto CheckString(amsr::core::StringView string, vac::container::CStringView error_msg) noexcept -> Result<void>;

  /*!
   * \brief Returns if the next characters are equal to the passed string.
   * \param string that is expected.
   * \return True if the string was found else false.
   *
   * \error{JsonErrorDomain, JsonErrc::kInvalidJson, if any error occurred.}
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto ReadString(amsr::core::StringView string) noexcept -> Result<bool>;

  /*!
   * \brief Skips all valid whitespace characters.
   * \return True in case we can continue, false in case end of file occurred.
   *
   * \details Valid JSON whitespace characters are space, newline, carriage return, and tab.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto SkipWhitespace() noexcept -> bool;

  /*!
   * \brief Reads num_to_read characters and executes the action for each span of characters.
   * \param num_to_read The number of characters to read.
   * \param callback Callback to call per view.
   * \return The number of characters read.
   *
   * \context ANY
   * \pre callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto Read(std::uint64_t num_to_read, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept
      -> Result<std::uint64_t>;

  /*!
   * \brief Reads exactly num_to_read characters and executes the action.
   * \details Buffers the data if necessary. The callback is either executed once (requested amount of characters has
   *          been read) or never (less characters available or EOF).
   * \param num_to_read The number of characters to read.
   * \param callback Callback to call once all characters have been read.
   * \return An empty Result or an error if less characters are available.
   *
   * \context ANY
   * \pre callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ReadExactly(std::uint64_t num_to_read,
                   vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept -> Result<void>;

  /*!
   * \brief Reads characters as long as the delimiter is not found and executes the action for each span of characters.
   * \param delimiter to decide if the character should be skipped.
   * \param callback Callback to call per view.
   * \return Either EOF or the delimiter that was found.
   *
   * \context ANY
   * \pre callback does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  auto ReadUntil(amsr::core::StringView delimiter,
                 vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept -> Result<OptChar>;

  /*!
   * \brief Returns a reference to the Json file.
   * \return Reference to the Json file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() & noexcept -> JsonData&;

  /*!
   * \brief Returns a reference to the Json file.
   * \return Const reference to the Json file.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetJsonDocument() const& noexcept -> JsonData const&;

 private:
  /*!
   * \brief Rewind the position of the document if a condition is fulfilled.
   * \param condition The condition to check before seeking.
   * \param num The number of bytes to rewind.
   * \return nothing or the error occurred.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto RewindIf(bool condition, std::size_t num) noexcept -> Result<void>;

  /*!
   * \brief Get direct access to the input stream.
   * \return Reference to the input stream.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetStream() & noexcept -> stream::InputStream&;

  /*!
   * \brief Get direct access to the input stream.
   * \return Const reference to the input stream.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetStream() const& noexcept -> stream::InputStream const&;

  /*!
   * \brief JsonData to operate on.
   */
  Ref<JsonData> data_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_JSON_OPS_H_
