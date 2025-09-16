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
 *        \brief  Declaration of the IntegrityProvider class
 *         \unit  amsr::iointegritystream::internal::stream::IntegrityProvider
 *   \complexity  The complexity is increased by many inter module calls which are necessary for the correct
 *                functionality of this unit and does not introduce any risk.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INTEGRITY_PROVIDER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INTEGRITY_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/type_helper.h"
#include "amsr/iointegritystream/internal/stream/integrity_engine.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoIntegrityStream_Metric-OO.WMC.One
/*!
 * \brief           Decorator for a raw amsr::stream::InputOutputStream
 * \details         Provide enhanced integrity read/write functions over a raw stream.
 */
class IntegrityProvider final : public amsr::stream::InputOutputStream {
 public:
  /*!
   * \brief           Constructor
   * \param[in]       integrity_data
   *                  References the validation data.
   * \param[in]       io_raw_stream
   *                  References raw stream that is to be decorated.
   * \param[in]       cache_buffer_
   *                  Buffer that is to be used for caching.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   */
  explicit IntegrityProvider(IntegrityFileData& integrity_data, amsr::stream::InputOutputStream& io_raw_stream,
                             amsr::core::Span<vac::language::byte> cache_buffer) noexcept;

  /*!
   * \brief           Move constructor
   * \param[in]       other
   *                  Other object to move.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  IntegrityProvider(IntegrityProvider&& other) noexcept = default;

  /*!
   * \brief           Destructor
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \reentrant       FALSE
   */
  ~IntegrityProvider() noexcept final = default;  // VCA_IOIS_DEFAULT_DESTRUCTOR

  IntegrityProvider(IntegrityProvider const&) = delete;
  IntegrityProvider& operator=(IntegrityProvider const&) = delete;
  IntegrityProvider& operator=(IntegrityProvider&& other) = delete;

  /*!
   * \copydoc         IntegrityEngine::EnableAppendMode
   */
  void EnableAppendMode() noexcept;

  /*!
   * \brief           Sets format parameters for a non-decimal base
   * \param[in]       show
   *                  Specifies if base to be shown for number.
   * \param[in]       output_width
   *                  Width of input / output.
   * \param[in]       fill_ch
   *                  Character to fill in case does not fill width.
   * \param[in]       letter_case
   *                  Specifies case for alpha-numerics.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetFormat(amsr::stream::ShowBase show, std::uint8_t output_width, char fill_ch,
                 amsr::stream::LetterCase letter_case) noexcept final;

  /*!
   * \brief           Sets format parameters for a non-decimal base
   * \param[in]       show
   *                  Specifies if base to be shown for number.
   * \param[in]       output_width
   *                  Width of input / output.
   * \param[in]       fill_ch
   *                  Character to fill in case does not fill width.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetFormat(amsr::stream::ShowBase show, std::uint8_t output_width, char fill_ch) noexcept final;

  /*!
   * \brief           Gets the format of the initialized stream
   * \return          Format of the number.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::charconv::ExtendedFormat const& GetCharconvFormat() const noexcept final;

  /*!
   * \brief           Enables show base for numbers
   * \details         Binary numbers are prefixed with 0b, hexadecimal ones with 0x.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetShowBase() noexcept final;

  /*!
   * \brief           Queries whether base should be shown
   * \return          \c true, if base should be shown, \c false otherwise.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  bool ShowBasePrefix() const noexcept final;

  /*!
   * \brief           Sets the base to be not shown for numbers
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void NoShowBase() noexcept final;

  /*!
   * \brief           Sets the base for numbers
   * \param[in]       base
   *                  Specifies base for number.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetBase(amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Get the base of number
   * \return          Base
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::stream::Base GetBase() const noexcept final;

  /*!
   * \brief           Get the charconv base format of number
   * \return          Base of the number which is in charconv::NumberBase format
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::charconv::NumberBase GetCharconvBase() const noexcept final;

  /*!
   * \brief           Converts base to charconv::NumberBase
   * \param[in]       base
   *                  Base.
   * \return          Corresponding charconv value.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  charconv::NumberBase ToCharconvBase(amsr::stream::Base base) const noexcept final;

  /*!
   * \brief           Enables uppercase for alpha-numeric representations
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetUppercase() noexcept final;

  /*!
   * \brief           Enables lowercase for alpha-numeric representations
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetLowercase() noexcept final;

  /*!
   * \brief           Sets case for alpha-numeric representations
   * \param[in]       letter_case
   *                  Letter case.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetLetterCase(amsr::stream::LetterCase const letter_case) noexcept final;

  /*!
   * \brief           Gets letter case for alpha-numeric numbers
   * \return          Letter case.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::stream::LetterCase GetLetterCase() const noexcept final;

  /*!
   * \brief           Gets letter case for alpha-numeric numbers in charconv format
   * \return          Letter case.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::charconv::LetterCase GetCharconvLetterCase() const noexcept final;

  /*!
   * \brief           Sets fill character if output does not fill width
   * \param[in]       ch
   *                  Fill character.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetFillChar(char const ch) noexcept final;

  /*!
   * \brief           Gets fill character if output does not fill width
   * \return          Fill character.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  char GetFillChar() const noexcept final;

  /*!
   * \brief           Sets width of output
   * \param[in]       output_width
   *                  Width.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetWidth(std::uint8_t const output_width) noexcept final;

  /*!
   * \brief           Gets width of output
   * \return          Width.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  std::uint8_t GetWidth() const noexcept final;

  /*!
   * \brief           Sets format of floating point output
   * \param[in]       fp_format
   *                  Format for the floating point.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetFloatingPointFormat(amsr::stream::FloatingPointFormat const fp_format) noexcept final;

  /*!
   * \brief           Gets the floating point format
   * \return          Floating point format.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::charconv::FPFormat GetCharconvFloatingPointFormat() const noexcept final;

  /*!
   * \brief           Return open status of associated file
   * \return          True if file is open, false if associated file is not open.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  bool IsOpen() const noexcept final;

  /*!
   * \brief           Read retrieves the content of the associated file and fills the given buffer up to the size of it
   * \param[out]      buffer
   *                  Memory to save the read bytes to.
   * \return          A span of the given memory which is either the same size, if enough data was retrieved from the
   *                  file, or smaller, if end of file was reached.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(
      amsr::core::Span<vac::language::byte> buffer) noexcept final;

  /*!
   * \brief           Read retrieves the content of the associated file and fills the given buffer up to the size of it
   * \param[out]      buffer
   *                  Memory to save the read chars to.
   * \return          A span of the given memory which is either the same size, if enough data was retrieved from the
   *                  file, or smaller, if end of file was reached.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept final;

  /*!
   * \brief           Reads up to \c count bytes from the stream without additional copy
   * \details         Returns a view on the internal buffer. This view may be smaller than the requested amount of data.
   *                  The data is only guaranteed to be available until the next call to a non-const member function of
   *                  the stream. If an invalid block is encountered, the offset remains at its beginning and an error
   *                  is returned.
   * \param[in]       count
   *                  Maximum number of bytes to be read.
   * \return          A span of at most size \c count (empty span if EOF is reached).
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \pre             buffer size >= block size
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<amsr::core::Span<char const>> Read(std::uint64_t count) noexcept final;

  /*!
   * \brief           Reads up to \c count bytes from the stream without additional copy and provides them in a callback
   * \details         Returns the number of read characters. This number may be smaller than the requested amount of
   *                  data. The data provided is only guaranteed to be available until the next callback/call to a non-
   *                  const member function of the stream.
   * \param[in]       count
   *                  Maximum number of bytes to be read.
   * \param[in]       callback
   *                  The callback that is called on every buffer view.
   * \return          Number of read characters.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Read(
      std::uint64_t count, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept final;

  /*!
   * \brief           Reads from the stream until \c pred returns \c false for a character
   * \details         Returns a view on the internal buffer. This view may not contain all matching characters. To
   *                  retrieve all matching characters, the method should be called repeatedly until it returns zero
   *                  bytes. The data are only guaranteed to be available until the next call to a non-const member
   *                  function of the stream happens. If an invalid block is encountered, the offset remains at its
   *                  beginning and an error is returned.
   * \param[in]       pred
   *                  Predicate function to determine if a character shall still be read.
   * \return          A number of bytes inside the internal buffer (zero bytes if \c pred returns false or if EOF is
   *                  reached).
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<amsr::core::Span<char const>> ReadWhile(
      vac::language::FunctionRef<bool(char)> pred) noexcept final;

  /*!
   * \brief           Reads from the file until any \c delimiter is found
   * \details         Returns the delimiter found or EOF. The views found in between are provided via \c callback. The
   *                  data is only guaranteed to be available until the next callback / call to a non-const member
   *                  function of the stream. The found delimiter is not consumed, meaning that for repeated calls to
   *                  ReadUntil the file position needs to be advanced by at least 1.
   * \param[in]       delimiter
   *                  A set of delimiters that stop reading.
   * \param[in]       callback
   *                  The callback that is called for every view read.
   * \return          The delimiter that was found or if it was not EOF.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> ReadUntil(
      amsr::core::StringView delimiter,
      vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept final;

  /*!
   * \brief           Checks if end-of-file is reached
   * \return          \c true, if yes, \c false, if no.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  bool IsEof() const noexcept final;

  /*!
   * \brief           Get the current file position
   * \return          The current file position, measured in bytes from the beginning of the file.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operation Tell fails.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept final;

  /*!
   * \brief           Changes the file position of the file
   * \param[in]       offset
   *                  Offset to change the file position with.
   * \param[in]       whence
   *                  Specifies how the offset is interpreted:
   *                  - kBeg: offset is a count of characters from the beginning of the file.
   *                  - kEnd: offset is a count of characters from the end of the file.
   *                  - kCur: offset is a count of characters from the current file position.
   * \return          The updated file position in the file, measured in bytes from the beginning of the file.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operation Seek fails.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, amsr::stream::SeekDirection whence) noexcept final;

  /*!
   * \brief           Gets one character from the current read position of the associated file
   * \return          The character or an error code.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Get() noexcept final;

  /*!
   * \brief           Peeks at the next character in the file without changing the current position.
   *                  - Reads one byte out of the file.
   *                  - Resets the position in the file
   * \return          The peeked byte or an error code.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Peek() noexcept final;

  /*!
   * \brief           Makes the most recently extracted character available again
   * \return          Result of type void.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operation UnGet fails.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<void> UnGet() noexcept final;

  /*!
   * \brief           Finds the first character from the current offset (inclusive) in the stream matching the predicate
   * \details         Moves the stream offset to the first match or to EOF. If an invalid block is encountered, the
   *                  offset remains at its beginning and an error is returned.
   * \param[in]       pred
   *                  Function determining if a character is a match.
   * \return          Offset of the found character.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Find/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred) noexcept final;

  /*!
   * \brief           Finds the first character from the current offset (inclusive) in the stream that is contained in
   *                  the set
   * \details         Moves the stream offset to the first match or to EOF.
   * \param[in]       set
   *                  Set of characters to look for.
   * \return          Offset of the found character.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::kEndOfFile
   *                  Unexpected EOF is encountered.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Find/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept final;

  /*!
   * \brief           Finds the first character in the stream from the current offset (inclusive) that is not contained
   *                  in set
   * \details         Moves the stream offset to the first mismatch or to EOF.
   * \param[in]       set
   *                  Set of characters.
   * \return          Offset of the first mismatch.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Find/Read fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept final;

  /*!
   * \brief           Write a character to the stream
   * \param[in]       data
   *                  A character.
   * \return          The number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(char data) noexcept final;

  /*!
   * \brief           Write a basic string view to the stream
   * \param[in]       data
   *                  A basic string view.
   * \return          The number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::StringView data) noexcept final;

  /*!
   * \brief           Write a basic C string view to the stream
   * \param[in]       data
   *                  A basic C string view.
   * \return          The number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(vac::container::CStringView data) noexcept final;

  /*!
   * \brief           Write a span of bytes to the stream
   * \param[in]       data
   *                  The span to write.
   * \return          The number of written bytes.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<vac::language::byte const> data) noexcept final;

  /*!
   * \brief           Write a span of characters to the stream
   * \param[in]       data
   *                  The span to write.
   * \return          The number of written bytes.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept final;

  /*!
   * \brief           Write a number of characters to the file stream by a provided callback
   * \details         The callback might be called multiple times with different views into the stream. Warning: The
   *                  callback might be called less often than requested (even zero times).
   * \param[in]       max_count
   *                  Maximum number of characters to be written.
   * \param[in]       callback
   *                  The callback that gets access to the internal buffer. The return value indicates the number of
   *                  bytes written into the buffer. In case the return value is equal in size to the provided buffer
   *                  (and max_count not reached) the stream will try to call again.
   * \return          The actual number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Write(
      std::uint64_t max_count,
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept final;

  /*!
   * \brief           Write data to the file stream by a provided callback
   * \details         The callback might be called multiple times with different views into the stream. Warning: The
   *                  callback might be called less often than requested (even zero times).
   * \param[in]       callback
   *                  The callback that gets access to the internal buffer. The return value indicates the number of
   *                  bytes written into the buffer. In case the return value is equal in size to the provided buffer
   *                  the stream will try to call again.
   * \return          The actual number of bytes written.
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::uint64_t> Write(
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept final;

  /*!
   * \brief           Writes a byte array in hexadecimal format to the stream
   * \param[in]       data
   *                  A span for a the byte array.
   * \param[in]       base
   *                  Base of the arithmetic value to be written
   * \return          The number of bytes written
   * \error           amsr::stream::StreamErrc::kInvalidAccess
   *                  Integrity of protected file is invalid.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operations Tell/Seek/Read/Write fail.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data,
                                         amsr::stream::Base base) noexcept final;

  /*!
   * \brief           Transfers all modified in-core data of the file referred to by the file descriptor to the disk
   * \return          Result with value void.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operation Sync fails.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<void> Sync() noexcept final;

  /*!
   * \brief           Flushes buffer to file
   * \return          Result with value void.
   * \error           amsr::stream::StreamErrc::*
   *                  Underlying stream operation Flush fails.
   * \context         ANY
   * \pre             \c io_raw_stream_ must be open.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<void> Flush() noexcept final;

  /*!
   * \brief           Get the number of remaining characters in buffer
   * \details         For file streams the buffer is flushed when the last element is written.
   * \return          The number of characters left.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Span<char>::index_type GetRemainingBufferSize() const noexcept final;

 protected:
  /*!
   * \brief           Do not use Write(char const*)
   * \details         Without this overload, a function with a char* parameter will call the overload Write(uint8_t). To
   *                  prevent this unintended behavior, this overload is made non-public to cause a compile error.
   * \return          Dummy value.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  amsr::core::Result<std::int64_t> Write(char const* /*data*/) noexcept final;

 private:
  /*!
   * \brief           Reference to validation data
   */
  IntegrityFileData& integrity_data_;

  /*!
   * \brief           The raw stream to be decorated
   */
  amsr::stream::InputOutputStream& io_raw_stream_;

  /*!
   * \brief           Engine for read and write with integrity check
   */
  IntegrityEngine integrity_engine_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityProvider, WriteCharpointer__dummy);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend declaration for testing
   */
  FRIEND_TEST(UT__IntegrityFileStream__FileStreamManager, IOFormat);
};

}  // namespace stream
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_INTEGRITY_PROVIDER_H_
