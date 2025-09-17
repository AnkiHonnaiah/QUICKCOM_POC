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
 *        \brief  Specifies format properties for IO.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_IO_FORMAT_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_IO_FORMAT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/charconv/type_helper.h"

namespace amsr {
namespace stream {

/*!
 * \brief Specifies base for numbers.
 * \vprivate Product Private
 */
enum class Base : std::uint8_t {
  kDecimal = static_cast<std::underlying_type_t<Base>>(charconv::NumberBase::kDecimal),
  kHex = static_cast<std::underlying_type_t<Base>>(charconv::NumberBase::kHex),
  kBinary = static_cast<std::underlying_type_t<Base>>(charconv::NumberBase::kBinary)
};

/*!
 * \brief Specifies if base of numbers should be displayed or not.
 * \vprivate Product Private
 */
enum class ShowBase : std::uint8_t { kNo = 0, kYes = 1 };

/*!
 * \brief Specifies format for floating point output.
 * \vprivate Product Private
 */
enum class FloatingPointFormat : std::uint8_t {
  kFixed = static_cast<std::underlying_type_t<FloatingPointFormat>>(charconv::FPFormat::kFixed),
  kScientific = static_cast<std::underlying_type_t<FloatingPointFormat>>(charconv::FPFormat::kScientific),
  kGeneral = static_cast<std::underlying_type_t<FloatingPointFormat>>(charconv::FPFormat::kGeneral)
};

/*!
 * \brief Specifies the case of the letters A to E in hexadecimal strings.
 * \vprivate Product Private
 */
enum class LetterCase : std::uint8_t {
  kLowercase = static_cast<std::underlying_type_t<LetterCase>>(charconv::LetterCase::kLowerCase),
  kUppercase = static_cast<std::underlying_type_t<LetterCase>>(charconv::LetterCase::kUpperCase)
};

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_io_format
/*!
 * \brief Specifies format properties for IO.
 * \unit amsr::stream::IOFormat
 * \vprivate Product Private
 */
class IOFormat {
 public:
  /*!
   * \brief Constructs instance of class.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  IOFormat() noexcept = default;

 protected:
  // IOFormat not copy constructible because FileStreams are not copy constructible.
  IOFormat(IOFormat const&) = delete;

  // IOFormat not copy assignable because FileStreams are not copy assignable.
  IOFormat& operator=(IOFormat const&) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] io_format Instance of IOFormat.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  IOFormat(IOFormat&& io_format) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] io_format Instance of IOFormat.
   * \return The moved IoFormat.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  IOFormat& operator=(IOFormat&& io_format) & noexcept = default;

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual ~IOFormat() noexcept = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Sets format parameters for a non-decimal base.
   * \param[in] show Specifies if base to be shown for number.
   * \param[in] output_width Width of input / output.
   * \param[in] fill_ch Character to fill in case does not fill width.
   * \param[in] letter_case Specifies case for alpha-numerics.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetFormat(ShowBase show, std::uint8_t output_width, char fill_ch, LetterCase letter_case) noexcept;

  /*!
   * \brief Sets format parameters for a non-decimal base.
   * \param[in] show Specifies if base to be shown for number.
   * \param[in] output_width Width of input / output.
   * \param[in] fill_ch Character to fill in case does not fill width.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetFormat(ShowBase show, std::uint8_t output_width, char fill_ch) noexcept;

  /*!
   * \brief Gets the format of the initialized stream.
   * \return Format of the number.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Vector product internal API
   * \steady UNKNOWN
   */
  virtual amsr::charconv::ExtendedFormat const& GetCharconvFormat() const noexcept;

  /*!
   * \brief Enables show base for numbers.
   * \details Binary numbers are prefixed with 0b, hexadecimal ones with 0x.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetShowBase() noexcept;

  /*!
   * \brief Queries whether base should be shown.
   * \return \c true, if base should be shown, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Vector product internal API
   * \steady UNKNOWN
   */
  virtual bool ShowBasePrefix() const noexcept;

  /*!
   * \brief Sets the base to be not shown for numbers.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void NoShowBase() noexcept;

  /*!
   * \brief Sets the base for numbers.
   * \param[in] base Specifies base for number.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetBase(Base base) noexcept;

  /*!
   * \brief Get the base of number.
   * \return Base
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Vector product internal API
   * \steady UNKNOWN
   */
  virtual Base GetBase() const noexcept;

  /*!
   * \brief Get the charconv base format of number.
   * \return Base of the number which is in charconv::NumberBase format
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Vector product internal API
   * \steady UNKNOWN
   */
  virtual amsr::charconv::NumberBase GetCharconvBase() const noexcept;

  /*!
   * \brief Converts base to charconv::NumberBase.
   * \param[in] base Base.
   * \return Corresponding charconv value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component internal API
   * \steady UNKNOWN
   */
  virtual charconv::NumberBase ToCharconvBase(Base base) const noexcept;

  /*!
   * \brief Enables uppercase for alpha-numeric representations.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetUppercase() noexcept;

  /*!
   * \brief Enables lowercase for alpha-numeric representations.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetLowercase() noexcept;

  /*!
   * \brief Sets case for alpha-numeric representations.
   * \param[in] letter_case Letter case.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetLetterCase(LetterCase const letter_case) noexcept;

  /*!
   * \brief Gets letter case for alpha-numeric numbers.
   * \return Letter case.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Vector product internal API
   * \steady UNKNOWN
   */
  virtual LetterCase GetLetterCase() const noexcept;

  /*!
   * \brief Gets letter case for alpha-numeric numbers in charconv format.
   * \return Letter case.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual charconv::LetterCase GetCharconvLetterCase() const noexcept;

  /*!
   * \brief Sets fill character if output does not fill width.
   * \param[in] ch Fill character.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetFillChar(char const ch) noexcept;

  /*!
   * \brief Gets fill character if output does not fill width.
   * \return Fill character.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual char GetFillChar() const noexcept;

  /*!
   * \brief Sets width of output.
   * \param[in] output_width Width.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetWidth(std::uint8_t const output_width) noexcept;

  /*!
   * \brief Gets width of output.
   * \return Width.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual std::uint8_t GetWidth() const noexcept;

  /*!
   * \brief Sets format of floating point output.
   * \param[in] fp_format Format for the floating point.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual void SetFloatingPointFormat(FloatingPointFormat const fp_format) noexcept;

  /*!
   * \brief Gets the floating point format.
   * \return Floating point format.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::charconv::FPFormat GetCharconvFloatingPointFormat() const noexcept;

 protected:
  // VECTOR NC VectorC++-V11.0.2: MD_IoStream_V11.0.2_ProtectedAttributes
  /*! Base of the numbers. */
  charconv::NumberBase charconv_base_{charconv::NumberBase::kDecimal};

  // VECTOR NC VectorC++-V11.0.2: MD_IoStream_V11.0.2_ProtectedAttributes
  /*! Flag to indicate whether base should be shown. */
  ShowBase show_{ShowBase::kNo};

  // VECTOR NC VectorC++-V11.0.2: MD_IoStream_V11.0.2_ProtectedAttributes
  /*! Formatting parameters for non-floating-point numbers. */
  charconv::ExtendedFormat charconv_fmt_{0, charconv::LetterCase::kUpperCase, ' '};

  // VECTOR NC VectorC++-V11.0.2: MD_IoStream_V11.0.2_ProtectedAttributes
  /*! Format for floating-point numbers. */
  charconv::FPFormat charconv_fp_format_{charconv::FPFormat::kGeneral};

 private:
  /*! Friend test declaration. */
  FRIEND_TEST(UT__IOFormat, MoveConstructor);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__IOFormat, MoveAssignment);
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_IO_FORMAT_H_
