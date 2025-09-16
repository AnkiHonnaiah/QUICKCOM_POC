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
 *        \brief  Declaration of the OutputIntegrityStream class
 *         \unit  amsr::iointegritystream::internal::stream::OutputIntegrityStream
 *   \complexity  The complexity is increased by many inter module calls which are necessary for the correct
 *                functionality of this unit and does not introduce any risk.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_OUTPUT_INTEGRITY_STREAM_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_OUTPUT_INTEGRITY_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <climits>
#include <limits>
#include "amsr/charconv/to_textual.h"
#include "amsr/iointegritystream/internal/stream/integrity_provider.h"
#include "amsr/iostream/io_format.h"
#include "amsr/iostream/stringstream/output_string_stream.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoIntegrityStream_Metric-OO.WMC.One
/*!
 * \brief           Adapter class that can perform integrity write
 * \details         OutputIntegrityStream is an adapter implementation on top of an integrity provider.
 */
class OutputIntegrityStream final : public amsr::stream::OutputStream {
 public:
  /*!
   * \brief           Constructor
   * \param[in]       integrity_provider
   *                  Reference to the integrity provider that provide integrity write implementation.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  explicit OutputIntegrityStream(IntegrityProvider& integrity_provider) noexcept;

  /*!
   * \brief           Move constructor
   * \param[in]       other
   *                  Other.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  OutputIntegrityStream(OutputIntegrityStream&& other) noexcept = default;

  /*!
   * \brief           Destructor
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  ~OutputIntegrityStream() noexcept final = default;  // VCA_IOIS_DEFAULT_DESTRUCTOR

  OutputIntegrityStream(OutputIntegrityStream const& output_integrity_file_stream) = delete;
  OutputIntegrityStream& operator=(OutputIntegrityStream const& output_integrity_file_stream) = delete;
  OutputIntegrityStream& operator=(OutputIntegrityStream&& output_integrity_file_stream) = delete;

  /*!
   * \copydoc         IntegrityProvider::SetFormat(amsr::stream::ShowBase, std::uint8_t, char, amsr::stream::LetterCase)
   */
  void SetFormat(amsr::stream::ShowBase show, std::uint8_t output_width, char fill_ch,
                 amsr::stream::LetterCase letter_case) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetFormat(amsr::stream::ShowBase, std::uint8_t, char)
   */
  void SetFormat(amsr::stream::ShowBase show, std::uint8_t output_width, char fill_ch) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetCharconvFormat
   */
  amsr::charconv::ExtendedFormat const& GetCharconvFormat() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetShowBase
   */
  void SetShowBase() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::ShowBasePrefix
   */
  bool ShowBasePrefix() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::NoShowBase
   */
  void NoShowBase() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetBase
   */
  void SetBase(amsr::stream::Base base) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetBase
   */
  amsr::stream::Base GetBase() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetCharconvBase
   */
  amsr::charconv::NumberBase GetCharconvBase() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::ToCharconvBase
   */
  charconv::NumberBase ToCharconvBase(amsr::stream::Base base) const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetUppercase
   */
  void SetUppercase() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetLowercase
   */
  void SetLowercase() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetLetterCase
   */
  void SetLetterCase(amsr::stream::LetterCase const letter_case) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetLetterCase
   */
  amsr::stream::LetterCase GetLetterCase() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetCharconvLetterCase
   */
  amsr::charconv::LetterCase GetCharconvLetterCase() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetFillChar
   */
  void SetFillChar(char const ch) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetFillChar
   */
  char GetFillChar() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetWidth
   */
  void SetWidth(std::uint8_t const output_width) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetWidth
   */
  std::uint8_t GetWidth() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::SetFloatingPointFormat
   */
  void SetFloatingPointFormat(amsr::stream::FloatingPointFormat const fp_format) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetCharconvFloatingPointFormat
   */
  amsr::charconv::FPFormat GetCharconvFloatingPointFormat() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Tell
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(char)
   */
  amsr::core::Result<std::int64_t> Write(char data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type bool.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(bool const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int8_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int8_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int8_t.
   * \param[in]       base
   *                  Base of the arithmetic value to be written
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int8_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int16_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int16_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int16_t.
   * \param[in]       base
   *                  Base of the arithmetic value to be written
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int16_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int32_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int32_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int32_t.
   * \param[in]       base
   *                  Base of the arithmetic value to be written
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int32_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int64_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int64_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::int64_t.
   * \param[in]       base
   *                  Base of the arithmetic value to be written
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::int64_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint8_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint8_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint8_t.
   * \param[in]       base
   *                  Base of the arithmetic value to be written
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint8_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint16_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint16_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint16_t.
   * \param[in]       base
   *                  Base of the arithmatic value to be wriiten
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint16_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint32_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint32_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint32_t.
   * \param[in]       base
   *                  Base of the arithmatic value to be wriiten
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint32_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint64_t.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint64_t const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type std::uint64_t.
   * \param[in]       base
   *                  Base of the arithmatic value to be wriiten
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(std::uint64_t const& data, amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type float.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(float const& data) noexcept final;

  /*!
   * \brief           Write an arithmetic value to the stream
   * \param[in]       data
   *                  An arithmetic value of type double.
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(double const& data) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(amsr::core::StringView)
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::StringView data) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(vac::container::CStringView)
   */
  amsr::core::Result<std::int64_t> Write(vac::container::CStringView data) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(amsr::core::Span<char const>)
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(amsr::core::Span<vac::language::byte const>)
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<vac::language::byte const> data) noexcept final;

  // clang-format off
  /*!
   * \copydoc         IntegrityProvider::Write(std::uint64_t,
   *                  vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)>)
   */
  amsr::core::Result<std::uint64_t> Write(
      std::uint64_t max_count,
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept final;
  // clang-format on

  /*!
   * \copydoc         IntegrityProvider::Write(vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)>)
   */
  amsr::core::Result<std::uint64_t> Write(
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(amsr::core::Span<std::uint8_t const>, amsr::stream::Base)
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data,
                                         amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Inserts character into stream
   * \param[in]       data
   *                  The character to be inserted.
   * \return          Result of type void.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<void> Put(char data) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Seek
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, amsr::stream::SeekDirection whence) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::IsOpen
   */
  bool IsOpen() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Sync
   */
  amsr::core::Result<void> Sync() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Flush
   */
  amsr::core::Result<void> Flush() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::GetRemainingBufferSize
   */
  amsr::core::Span<char>::index_type GetRemainingBufferSize() const noexcept final;

 private:
  /*!
   * \brief           Write an arithmetic value to the file stream
   * \tparam          ArithmeticType
   *                  An arithmetic data type.
   * \param[in]       data
   *                  An arithmetic value.
   * \param[in]       base
   *                  Base value of the arithmatic value to be written
   * \return          The result as number of bytes written as std::int64_t int
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * #10 Write data by converting to textual representation.
   * \endinternal
   */
  template <typename IntType, std::enable_if_t<std::is_integral<IntType>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> Write(IntType const& data, amsr::stream::Base base) noexcept {
    amsr::core::Result<std::int64_t> result{amsr::stream::StreamErrc::kInvalidArgument};
    bool const show_base_prefix{ShowBasePrefix()};  // VCA_IOIS_UNDEFINED_FUNCTION
    if (show_base_prefix) {
      switch (base) {
        case amsr::stream::Base::kHex:
          result = WritePrefixedHex(data);
          break;
        case amsr::stream::Base::kDecimal:
          result = WriteByCallingToTextual(data, base);
          break;
        case amsr::stream::Base::kBinary:
          result = WritePrefixedBinary(data);
          break;
      }
    } else {
      result = WriteByCallingToTextual(data, base);
    }

    return result;
  }

  /*!
   * \brief           Writes an boolean value starting at the current write position of the output stream
   * \param[in]       data
   *                  A boolean value.
   * \return          The result as number of bytes written as std::int64_t integer.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * #10 Convert boolean value to textual representation. #20 Write to file if conversion succeeded.
   * \endinternal
   */
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(bool const& data) noexcept {
    amsr::core::Result<std::int64_t> result{amsr::stream::StreamErrc::kInsufficientResources};

    // #10 Convert number to textual representation
    constexpr std::uint8_t kFalseLiteralSize{5};
    amsr::core::Array<char, kFalseLiteralSize> buffer{};

    // VCA_IOIS_UNDEFINED_FUNCTION
    amsr::charconv::ToTextual<bool> to_textual{amsr::core::Span<char>{buffer}, data};
    // VCA_IOIS_UNDEFINED_FUNCTION, VCA_IOIS_VALID_PARAMETERS
    amsr::core::Result<amsr::core::Span<char>> conversion_result{to_textual.Convert()};

    // #20 Write to file
    if (conversion_result.HasValue()) {  // COV_IOIS_TOTEXTUAL_BUFFER_LARGE_ENOUGH
      amsr::core::Result<std::int64_t> write_result{Write(conversion_result.Value())};  // VCA_IOIS_UNDEFINED_FUNCTION
      result.Swap(write_result);
    }

    return result;
  }

  /*!
   * \brief           Writes an integral value starting at the current write position of the output stream
   * \tparam          IntType
   *                  An integral data type.
   * \param[in]       data
   *                  An integral value.
   * \param[in]       base
   *                  Base value of the arithmetic value to be written.
   * \return          The result as number of bytes written as std::int64_t integer.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * #10 Convert integral number to textual representation. #20 Write to file if conversion succeeded.
   * \endinternal
   */
  template <
      typename IntType,
      std::enable_if_t<std::is_integral<IntType>::value && (!std::is_same<IntType, bool>::value), std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(IntType const& data,
                                                           amsr::stream::Base const base) noexcept {
    amsr::core::Result<std::int64_t> result{amsr::stream::StreamErrc::kInsufficientResources};

    // #10 Convert number to textual representation
    constexpr std::uint8_t kMaxBinarySize{std::numeric_limits<std::uint8_t>::max()};
    amsr::core::Array<char, kMaxBinarySize> buffer{};

    // VCA_IOIS_UNDEFINED_FUNCTION
    amsr::charconv::ToTextual<IntType> to_textual{amsr::core::Span<char>{buffer}, static_cast<IntType>(data)};
    // VCA_IOIS_UNDEFINED_FUNCTION
    charconv::NumberBase const charconv_base{ToCharconvBase(base)};
    amsr::core::Result<amsr::core::Span<char>> conversion_result{
        // VCA_IOIS_UNDEFINED_FUNCTION, VCA_IOIS_VALID_PARAMETERS
        to_textual.Convert(charconv_base, GetCharconvFormat())};

    // #20 Write to file
    if (conversion_result.HasValue()) {  // COV_IOIS_TOTEXTUAL_BUFFER_LARGE_ENOUGH
      amsr::core::Result<std::int64_t> write_result{Write(conversion_result.Value())};  // VCA_IOIS_UNDEFINED_FUNCTION
      result.Swap(write_result);
    }

    return result;
  }

  /*!
   * \brief           Writes a floating point value starting at the current write position of the output stream
   * \tparam          FP
   *                  A floating point data type.
   * \param[in]       data
   *                  A floating point value.
   * \return          The number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * #10 Convert floating point number to textual representation. #20 Write to file if conversion succeeded.
   * \endinternal
   */
  template <typename FP, std::enable_if_t<std::is_floating_point<FP>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(FP const& data) noexcept {
    amsr::core::Result<std::int64_t> result{amsr::stream::StreamErrc::kInsufficientResources};

    // #10 Convert floating point number to textual representation
    amsr::core::Array<char, amsr::charconv::kFPMaxLength> conv_buffer{};
    amsr::core::Span<char> const conv_span{conv_buffer};

    // VCA_IOIS_UNDEFINED_FUNCTION
    amsr::charconv::ToTextual<FP> to_textual{conv_span, static_cast<FP>(data)};
    // VCA_IOIS_UNDEFINED_FUNCTION
    amsr::core::Result<amsr::core::Span<char>> conversion_result{to_textual.Convert(GetCharconvFloatingPointFormat())};

    // #20 Write to file
    if (conversion_result.HasValue()) {  // COV_IOIS_TOTEXTUAL_BUFFER_LARGE_ENOUGH
      amsr::core::Result<std::int64_t> write_result{Write(conversion_result.Value())};  // VCA_IOIS_UNDEFINED_FUNCTION
      result.Swap(write_result);
    }

    return result;
  }

  /*!
   * \brief           Writes an integral value in hexadecimal format to the stream, prefixed with 0x
   * \tparam          IntType
   *                  An integral data type.
   * \param[in]       data
   *                  An integral value.
   * \return          The number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * #10 Write to temporary array (because otherwise minus is positioned after prefix for negative numbers). #20 Write
   * to file if writing to array succeeded. #30 Set error otherwise.
   * \endinternal
   */
  template <typename IntType, std::enable_if_t<std::is_integral<IntType>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixedHex(IntType const& data) noexcept {
    // #10 Write to temporary array (because otherwise minus is positioned after prefix for negative numbers).
    constexpr std::size_t kNbHexDigits{sizeof(data) * 2};
    constexpr std::size_t kSize{kNbHexDigits + amsr::stream::HexPrefix::kHexPrefixSize +
                                1};         // + 1 in case of minus sign
    ara::core::Vector<char> buffer(kSize);  // VCA_IOIS_FUNC_CONTRACT
    amsr::core::Span<char> const buffer_span{buffer};

    amsr::stream::stringstream::OutputStringStream oss{buffer_span};  // VCA_IOIS_UNDEFINED_FUNCTION
    // VECTOR NC AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_IoIntegrityStream_A5.0.1_operandHasNoSideEffect
    oss.SetFormat(amsr::stream::ShowBase::kYes, GetWidth(), GetFillChar(),  // VCA_IOIS_UNDEFINED_FUNCTION
                  GetLetterCase());                                         // VCA_IOIS_UNDEFINED_FUNCTION
    return oss
        .Write(data, amsr::stream::Base::kHex)  // VCA_IOIS_VALID_PARAMETERS
        .AndThen([this, &buffer_span](std::int64_t bytes_written_result) {
          return Write(buffer_span.subspan(0, static_cast<std::size_t>(bytes_written_result)));  // COV_IOIS_WRITE_ERROR
        });
  }  // VCA_IOIS_VALID_PARAMETERS

  /*!
   * \brief           Writes an integral value in binary format to the stream, prefixed with 0b
   * \tparam          Number
   *                  An integral data type.
   * \param[in]       data
   *                  An integral value.
   * \return          The number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             Filestream must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   *
   * \internal
   * #10 Write to temporary buffer. #20 Write to file if writing to buffer succeeded.
   * \endinternal
   */
  template <typename Number, std::enable_if_t<std::is_integral<Number>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixedBinary(Number const& data) noexcept {
    constexpr std::uint8_t kNbBinaryDigits{static_cast<std::uint8_t>(sizeof(data) * CHAR_BIT)};
    std::uint32_t const size{
        static_cast<std::uint32_t>(std::max(kNbBinaryDigits, GetWidth())) +  // VCA_IOIS_UNDEFINED_FUNCTION
        amsr::stream::BinaryPrefix::kBinaryPrefixSize};
    ara::core::Vector<char> buffer(size);  // VCA_IOIS_FUNC_CONTRACT
    amsr::core::Span<char> const buffer_span{buffer};

    amsr::stream::stringstream::OutputStringStream oss{buffer_span};  // VCA_IOIS_UNDEFINED_FUNCTION
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_IoIntegrityStream_A5.0.1_operandHasNoSideEffect
    // VECTOR NC VectorC++-V5.0.1: MD_IoIntegrityStream_V5.0.1_operandHasNoSideEffect
    oss.SetFormat(amsr::stream::ShowBase::kYes, GetWidth(), GetFillChar(),  // VCA_IOIS_UNDEFINED_FUNCTION
                  GetLetterCase());                                         // VCA_IOIS_UNDEFINED_FUNCTION

    return oss
        .Write(data, amsr::stream::Base::kBinary)  // VCA_IOIS_FUNC_CONTRACT
        .AndThen([this, &buffer_span](std::int64_t bytes_written_result) {
          return Write(buffer_span.subspan(0, static_cast<std::size_t>(bytes_written_result)));  // COV_IOIS_WRITE_ERROR
        });
  }

  /*!
   * \copydoc         IntegrityProvider::Write(char const*)
   */
  amsr::core::Result<std::int64_t> Write(char const*) noexcept final;

 private:
  /*!
   * \brief           Constructor that abstracts the IntegriyProvider class. Called by the public constructor
   * \details         This is also an entrypoint for mocking the interface.
   * \param[in]       integrity_provider
   *                  Reference to the integrity provider that provide integrity write implementation.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  explicit OutputIntegrityStream(amsr::stream::InputOutputStream& integrity_provider) noexcept;

  /*!
   * \brief           The integrity provider that provides the actual integrity implementation
   */
  amsr::stream::InputOutputStream& integrity_provider_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  FRIEND_TEST(UT__OutputIntegrityStream, WriteCharpointer__dummy);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  template <typename T>
  FRIEND_TEST(UT__OutputIntegrityStream__Integer_F, WriteByCallingToTextual);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  template <typename T>
  FRIEND_TEST(UT__OutputIntegrityStream__Integer_F, WriteByCallingToTextual__BufferTooSmall);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  FRIEND_TEST(UT__OutputIntegrityStream, WriteByCallingToTextual__BufferTooSmall);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for using private constructor and mocking the IntegrityStream
   */
  friend class UT__OutputIntegrityStream;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  FRIEND_TEST(MUT__Stream__OutputIntegrityStream, WriteCharpointer__dummy);
};

}  // namespace stream
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_OUTPUT_INTEGRITY_STREAM_H_

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_IOIS_TOTEXTUAL_BUFFER_LARGE_ENOUGH
//   \ACCEPT TX
//   \REASON Sanity check. The supplied buffer is always large enough as it is set to the maximum. Since ToTextual
//           function convert returns a Result, it is good practice to check HasValue() before accessing Value().
//
// \ID COV_IOIS_WRITE_ERROR
//   \ACCEPT XX
//   \REASON The error case is hard to cover in tests, since the buffer passed to the OutputStringStream is ensured to
//           be large enough. Therefore it could only fail when the stream runs into an obscure error case like Tell()
//           failing, which is hard to provoke. Errors will anyways be passed back directly to the caller of Write(),
//           therefore there is no risk.
//
// COV_JUSTIFICATION_END
