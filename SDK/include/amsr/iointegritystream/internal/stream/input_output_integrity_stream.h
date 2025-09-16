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
 *        \brief  Declaration of the InputOutputIntegrityStream class
 *         \unit  amsr::iointegritystream::internal::stream::InputOutputIntegrityStream
 *   \complexity  The complexity is increased by many inter module calls which are necessary for the correct
 *                functionality of this unit and does not introduce any risk.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INPUT_OUTPUT_INTEGRITY_STREAM_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INPUT_OUTPUT_INTEGRITY_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iointegritystream/internal/stream/integrity_provider.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoIntegrityStream_Metric-OO.WMC.One
/*!
 * \brief           Class that can perform integrity read/write
 * \details         InputOutputIntegrityStream does nothing but only bridge API calls to the integrity provider.
 */
class InputOutputIntegrityStream final : public amsr::stream::InputOutputStream {
 public:
  /*!
   * \brief           Constructor
   * \param[in]       integrity_provider
   *                  Reference to the integrity provider that provide integrity read/write implementation.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   */
  explicit InputOutputIntegrityStream(IntegrityProvider& integrity_provider) noexcept;

  /*!
   * \brief           Move constructor
   * \param[in]       other
   *                  Other object to move.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  InputOutputIntegrityStream(InputOutputIntegrityStream&& other) noexcept = default;

  /*!
   * \brief           Destructor
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   */
  ~InputOutputIntegrityStream() noexcept final = default;  // VCA_IOIS_DEFAULT_DESTRUCTOR

  InputOutputIntegrityStream(InputOutputIntegrityStream const&) = delete;
  InputOutputIntegrityStream& operator=(InputOutputIntegrityStream const&) = delete;
  InputOutputIntegrityStream& operator=(InputOutputIntegrityStream&& other) = delete;

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
   * \copydoc         IntegrityProvider::IsOpen
   */
  bool IsOpen() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Tell
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Seek
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, amsr::stream::SeekDirection whence) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Read(amsr::core::Span<vac::language::byte>)
   */
  amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(
      amsr::core::Span<vac::language::byte> buffer) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Read(amsr::core::Span<char>)
   */
  amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Read(std::uint64_t)
   */
  amsr::core::Result<amsr::core::Span<char const>> Read(std::uint64_t count) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Read(std::uint64_t, vac::language::FunctionRef<void(amsr::core::StringView)>)
   */
  amsr::core::Result<std::uint64_t> Read(
      std::uint64_t count, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::ReadWhile
   */
  amsr::core::Result<amsr::core::Span<char const>> ReadWhile(
      vac::language::FunctionRef<bool(char)> pred) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::ReadUntil
   */
  amsr::core::Result<std::int64_t> ReadUntil(
      amsr::core::StringView delimiter,
      vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Find(vac::language::FunctionRef<bool(char)>)
   */
  amsr::core::Result<std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Find(amsr::core::StringView)
   */
  amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::FindNot
   */
  amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Get
   */
  amsr::core::Result<std::int64_t> Get() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Peek
   */
  amsr::core::Result<std::int64_t> Peek() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::UnGet
   */
  amsr::core::Result<void> UnGet() noexcept final;

  /*!
   * \copydoc         IntegrityProvider::IsEof
   */
  bool IsEof() const noexcept final;

  /*!
   * \copydoc         IntegrityProvider::Write(char)
   */
  amsr::core::Result<std::int64_t> Write(char data) noexcept final;

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

 protected:
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
  explicit InputOutputIntegrityStream(amsr::stream::InputOutputStream& integrity_provider) noexcept;
  /*!
   * \brief           The integrity provider that provides the actual integrity implementation
   */
  amsr::stream::InputOutputStream& integrity_provider_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for using private constructor and mocking the IntegrityStream
   */
  friend class UT__InputOutputIntegrityStream;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for constructing InputOutputIntegrityStream with IntegrityProvider
   */
  FRIEND_TEST(UT__InputOutputIntegrityStream, IntegrityProvider);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for dummy overload function
   */
  FRIEND_TEST(UT__InputOutputIntegrityStream, WriteCharpointer__dummy);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for char pointer Write function
   */
  FRIEND_TEST(MUT__Stream__InputOutputIntegrityStream, WriteCharpointer__dummy);
};

}  // namespace stream
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INPUT_OUTPUT_INTEGRITY_STREAM_H_
