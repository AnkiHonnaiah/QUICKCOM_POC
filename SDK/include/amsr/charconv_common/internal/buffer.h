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
 *        \brief  Manages a memory segment with textual representation(s) to parse.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BUFFER_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BUFFER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/type_helper.h"
#include "amsr/charconv_common/internal/common_utility.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Manages a memory segment with textual representation(s) to parse.
 * \unit amsr::charconv::internal::Buffer
 * \vprivate Component Private
 */
class Buffer {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer_in Memory segment with textual representation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit Buffer(amsr::core::StringView buffer_in) noexcept;

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] buffer Instance of Buffer
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  Buffer(Buffer&& buffer) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] buffer Instance of Buffer
   * \return Moved Buffer
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  Buffer& operator=(Buffer&& buffer) & noexcept = default;

  // Buffer is not copy constructible because it would break exclusive management of the resource.
  Buffer(Buffer const& buffer) = delete;

  // Buffer is not copy assignable because it would break exclusive management of the resource.
  Buffer& operator=(Buffer const& buffer) & = delete;

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual ~Buffer() noexcept = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  /*!
   * \brief Gets pointer to next character to parse.
   * \return The described pointer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  char const* GetCurrentPointer() const noexcept;

 protected:
  /*!
   * \brief Gets span of characters from characters left to be parsed.
   * \param[in] pos Start of view.
   * \param[in] count Size of view.
   * \return The described span.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char const> GetCurrentBufferView(std::size_t pos, std::size_t count) const noexcept;

  /*!
   * \brief Checks if buffer is not empty.
   * \return \c true if buffer is not empty, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool AreCharsLeft() const noexcept;

  /*!
   * \brief Gets number of characters left to be parsed.
   * \return Number of characters left to be parsed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::size_t GetNumberOfCharsLeft() const noexcept;

  /*!
   * \brief Gets next character and removes it from the list of characters left to parse.
   * \details Character is considered to be parsed.
   * \return Next character to be parsed.
   * \context ANY
   * \pre At least one character has to be left to parse.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  char GetChar() noexcept;

  /*!
   * \brief Gets next character without removing it from the list of characters left to parse.
   * \return Next character to be parsed.
   * \context ANY
   * \pre At least one character has to be left to parse.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  char PeekChar() const noexcept;

  /*!
   * \brief Advances current buffer by \c count.
   * \param[in] count Number of characters to skip.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void Advance(std::size_t count) noexcept;

  /*!
   * \brief Starts parsing at beginning.
   * \context ANY
   * \pre At least one character has to be left to parse.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void ResetCurrentBuffer() noexcept;

  /*!
   * \brief Checks if \c literal matches boolean value string.
   * \param[in] value The boolean value to return.
   * \return On success, returns the boolean value.
   * \context ANY
   * \pre -
   * \error CharconvErrc::kInvalidArgument If range is not large enough or \c literal does not match boolean value
   * string.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<bool> ParseBool(bool value) noexcept;

  /*!
   * \brief Advances until first non-whitespace character is found
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \trace SPEC-9718151
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void SkipLeadingWhitespace() noexcept;

  /*!
   * \brief Checks if the first element of the character sequence is a minus sign.
   * \return \c true if first contains a minus sign, \c false otherwise.
   * \context ANY
   * \pre At least one character has to be left to parse.
   * \threadsafe FALSE
   * \reentrant TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool ParseLeadingMinusSign() noexcept;

 private:
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, Constructor);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, MoveConstructor);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, MoveAssignment);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, GetCurrentPointer);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, GetCurrentBufferView);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, AreCharsLeft);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, GetNumberOfCharsLeft);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, GetChar__NonEmptyBuffer);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, GetChar__EmptyBuffer);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, PeekChar__NonEmptyBuffer);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, PeekChar__EmptyBuffer);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, Advance);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ResetCurrentBuffer);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseBool__True);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseBool__False);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseBool__NoMatch);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseBool__BufferTooShort);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, SkipLeadingWhitespace__NotOnlyWhitespace);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, SkipLeadingWhitespace__OnlyWhitespace);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseLeadingMinusSign__HasMinusSign);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseLeadingMinusSign__NoMinusSign);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__Buffer, ParseLeadingMinusSign__EmptyBuffer);

  /*! Complete memory segment. */
  amsr::core::StringView buffer_;

  /*! Memory segment that still needs to be parsed. */
  amsr::core::StringView current_buffer_;
};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_BUFFER_H_
