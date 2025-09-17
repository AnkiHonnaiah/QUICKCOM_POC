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
 *        \brief  Manages a memory segment where the textual representation of the converted number is stored.
 *
 *********************************************************************************************************************/

#ifndef LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_TO_TEXTUAL_BUFFER_H_
#define LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_TO_TEXTUAL_BUFFER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"

namespace amsr {
namespace charconv {
namespace internal {

/*!
 * \brief Manages a memory segment where the textual representation of the converted number is stored.
 * \unit amsr::charconv::internal::ToTextualBuffer
 * \vprivate Component Private
 */
class ToTextualBuffer {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] buffer_in Memory segment with textual representation.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit ToTextualBuffer(amsr::core::Span<char> buffer_in) noexcept;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] buffer Instance of ToTextualBuffer
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  ToTextualBuffer(ToTextualBuffer&& buffer) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] buffer Instance of ToTextualBuffer
   * \return Moved ToTextualBuffer
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  ToTextualBuffer& operator=(ToTextualBuffer&& buffer) & noexcept = default;

  // ToTextualBuffer is not copy constructible because it would break exclusive management of the resource.
  ToTextualBuffer(ToTextualBuffer const& buffer) = delete;

  // ToTextualBuffer is not copy assignable because it would break exclusive management of the resource.
  ToTextualBuffer& operator=(ToTextualBuffer const& buffer) & = delete;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual ~ToTextualBuffer() noexcept = default;  // VCA_CHARCONV_DESTRUCTOR_MOLE_1298

  /*!
   * \brief Copys literal to buffer when prerequisites for memcpy are met.
   * \param[in] literal Textual representation to copy to buffer.
   * \return Span of textual representation in buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error CharconvErrc::kValueTooLarge In case the value is larger than the specified range.
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char>> DoSafeMemcpy(amsr::core::StringView literal) noexcept;

  /*!
   * \brief Resets the current position to beginning of buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void Reset() noexcept;

  /*!
   * \brief Inserts \c ch at the current position and advances the current position.
   * \param[in] ch Character to write to buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void Add(char ch) noexcept;

  /*!
   * \brief Get size of buffer.
   * \return Size of buffer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::size_t GetSize() const noexcept;

  /*!
   * \brief Get contents written to buffer from beginning to the current position.
   * \return Span on buffer from beginning to current position (excluded).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char> GetBuffer() const noexcept;

 private:
  /*!
   * \brief Checks if preconditions for memcpy are fulfilled.
   * \param[in] literal Textual representation to be copied to buffer.
   * \return \c true if preconditions are fulfilled, otherwise \c false.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool FulfillsMemcpyPrerequisites(amsr::core::StringView literal) const noexcept;

  /*! Complete memory segment. */
  amsr::core::Span<char> buffer_{};

  /*! Pointer to current location in buffer. */
  char* current_;
};

}  // namespace internal
}  // namespace charconv
}  // namespace amsr

#endif  // LIB_CHARCONV_COMMON_INCLUDE_AMSR_CHARCONV_COMMON_INTERNAL_TO_TEXTUAL_BUFFER_H_
