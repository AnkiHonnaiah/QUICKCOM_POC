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
 *        \brief  Handles access to file buffer.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_BUFFER_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_BUFFER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/types.h"
#include "amsr/iostream/utility.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"
#include "vac/language/function_ref.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace stream {
namespace filestream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_buffer
/*!
 * \brief Handles access to file buffer.
 * \unit amsr::stream::filestream::FileBuffer
 * \vprivate Component Private
 */
class FileBuffer {
 public:
  /*!
   * \brief Default buffer size.
   * \vprivate Component Private
   */
  static constexpr std::size_t kDefaultBufferSize{4096U};

  /*!
   * \brief Constructs instance of class.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  FileBuffer() noexcept;

  /*!
   * \brief Constructs a file buffer using an external buffer.
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit FileBuffer(amsr::core::Span<char> buffer) noexcept;

  // FileBuffer is not copy constructible because it would break exclusive management of the resource.
  FileBuffer(FileBuffer const& buffer) = delete;

  // FileBuffer is not copy assignable because it would break exclusive management of the resource.
  FileBuffer& operator=(FileBuffer const& buffer) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] buffer Instance of FileBuffer
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  FileBuffer(FileBuffer&& buffer) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] buffer Instance of FileBuffer
   * \return Moved FileBuffer
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  FileBuffer& operator=(FileBuffer&& buffer) & noexcept =
      default;  // VCA_IOSTREAM_STD_UNIQUE_PTR_MOVE_ASSIGNMENT_CONTRACT

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual ~FileBuffer() noexcept = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Reads up to \c count bytes from the buffer without additional copy or checks.
   * \details Returns a view on the internal buffer. This view may be smaller than the requested amount of data. The
   * data is only guaranteed to be available until the next call to a non-const member function of the buffer.
   * \param[in] count Maximum number of bytes to be read.
   * \return A span of at most size \c count (empty span if EOF is reached).
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char const> ReadWithoutCopy(std::uint64_t count) noexcept;

  /*!
   * \brief Reads from the buffer until \c pred returns \c false for a character.
   * \details Returns a view on the internal buffer. This view may not contain all matching characters. To retrieve all
   * matching characters, the method should be called repeatedly until it returns zero bytes. The
   * data is only guaranteed to be available until the next call to a non-const member function of the buffer.
   * \param[in] pred Predicate function to determine if a character shall still be read.
   * \return A view on the internal buffer, ending when \c pred returns false or if EOF is reached.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char const> ReadWhile(vac::language::FunctionRef<bool(char)> pred) noexcept;

  /*!
   * \brief Reads from the buffer until any \c delimiter is found.
   * \details Returns a view on the internal buffer. This view may not contain all matching characters. To retrieve all
   * matching characters, the method should be called repeatedly until it returns zero bytes. The
   * data is only guaranteed to be available until the next call to a non-const member function of the buffer.
   * \param[in] delimiter A set of delimiters that stop reading.
   * \return A span of internal buffer (empty span if EOF is reached).
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char const> ReadUntil(amsr::core::StringView delimiter) noexcept;

  /*!
   * \brief Finds the first character from the current offset (inclusive) in the buffer matching the predicate.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] pred Function determining if a character is a match.
   * \return Offset of the found character measured from beginning of buffer.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::pair<bool, std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred);

  /*!
   * \brief Gets one character from the current read position in the buffer.
   * \return The character or EOF.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Get() noexcept;

  /*!
   * \brief Peeks at the next character in the buffer without changing the current position.
   * \return The peeked byte.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Peek() const noexcept;

  /*!
   * \brief Write a single character to the buffer.
   * \param[in] data The character.
   * \context ANY
   * \pre Aborts if the buffer is full (checked by SafelyIncrementPosition).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void Write(char data) noexcept;

  /*!
   * \brief Write a number of characters to the buffer by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] max_count Maximum number of characters to be written.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::uint64_t Write(std::uint64_t max_count,
                      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept;

  /*!
   * \brief Write data to the buffer by a provided callback.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::uint64_t Write(vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept;

  /*!
   * \brief Checks if the seeked location is located in buffer.
   * \param[in] offset Offset to change the buffer position with.
   * \return \c true, if the target location is in buffer, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool IsSeekedLocationInBuffer(std::int64_t offset) const noexcept;

  /*!
   * \brief Changes the position in the buffer.
   * \param[in] offset Offset to change the buffer position with.
   * \return The updated position in the buffer, measured in bytes from the beginning of the buffer.
   * \context ANY
   * \pre Aborts if the target position is outside the buffer.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::uint64_t Seek(std::int64_t offset) noexcept;

  /*!
   * \brief Returns the seek position in buffer when seeking from current location.
   * \details If the buffer is not empty, the file descriptor points to a different location than the current position
   * in buffer, so the seek position from current location has to be transformed.
   * \param[in] offset Target offset from current position.
   * \return The target seek position in file.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t GetSeekPositionInFile(std::int64_t offset) const noexcept;

  /*!
   * \brief Reads data from buffer.
   * \tparam T Type of span to read.
   * \param[out] data Memory to save the read bytes to.
   * \return A span of the given memory which is either the same size, if enough data was retrieved from the buffer, or
   * smaller, if end of buffer was reached.
   * \context ANY
   * \pre Aborts if \c data is empty.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument Buffer cannot be copied because of invalid parameters.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Abort if data is empty.
   * #15 If there is data in buffer:
   *     #30 If memcpy can be safely called: copy the data.
   *     #40 Otherwise: set error.
   * \endinternal
   */
  template <typename T>
  auto Read(amsr::core::Span<T> data) noexcept -> amsr::core::Result<amsr::core::Span<T>> {
    Assert(!data.empty(), __FILE__, __LINE__, "Internal error: no data given.");
    std::size_t const count{data.size()};

    amsr::core::Result<amsr::core::Span<T>> result{amsr::core::Span<T>{}};
    if (chars_buffered_ > pos_in_buffer_) {  // #15
      // readjust read size in case more data is requested than available
      std::size_t const chars_to_read{std::min(count, chars_buffered_ - pos_in_buffer_)};
      if (FulfillsMemcpyPrerequisites<T>(data, &buffer_view_[pos_in_buffer_], buffer_view_.end(),
                                         chars_to_read)) {  // #30
        // VCA_IOSTREAM_STD_MEMCPY, VCA_IOSTREAM_IN_BOUND_ACCESS_READ
        std::memcpy(data.data(), &buffer_view_[pos_in_buffer_], chars_to_read);
        SafelyIncrementPosition(chars_to_read);
        result.EmplaceValue(data.subspan(0, chars_to_read));
      } else {  // #40
        result.EmplaceError(StreamErrc::kInvalidArgument,
                            amsr::core::String("FileBuffer::Read: Data cannot be copied to output buffer.").c_str());
      }
    }

    return result;
  }

  /*!
   * \brief Refills internal buffer if necessary and copies \c chars_left characters from internal buffer to \c data.
   * \details Function either aborts or returns value.
   * \tparam T Type of span to read.
   * \param[out] data Memory to save the read bytes to.
   * \param[in] chars_read_so_far Number of characters already read.
   * \return A span of the given memory which is either the same size, if enough data was retrieved from the file, or
   * smaller, if end of file was reached.
   * \context ANY
   * \pre Aborts if data is empty (checked by Read) or not enough data to copy available.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - Abort if remaining chars to read exceed number of available chars in internal buffer.
   * - Read remaining data from buffer.
   * - Return span.
   * \endinternal
   */
  template <typename T>
  auto ReadRemainingData(amsr::core::Span<T> data, std::size_t chars_read_so_far) noexcept
      -> amsr::core::Result<amsr::core::Span<T>> {
    std::size_t const remaining_chars{data.size() - chars_read_so_far};

    amsr::core::StringView const msg{"Internal error occurred."};
    Assert(remaining_chars <= GetRemainingSize(), __FILE__, __LINE__, msg);

    amsr::core::Span<T> const remaining_data{data.subspan(chars_read_so_far, remaining_chars)};
    return Read(remaining_data).Map([chars_read_so_far, data](amsr::core::Span<T> remaining_data_read) {
      std::size_t const total_chars_read{chars_read_so_far + remaining_data_read.size()};
      return data.subspan(0, total_chars_read);
    });
  }

  /*!
   * \brief Write data to the buffer.
   * \tparam T Type of span to write.
   * \param[in] data The span to write.
   * \return Number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument Input and output buffer overlap.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #15 If there is data in buffer OR data does not exceed buffer size:
   *     #20 If there is data to be copied:
   *         #30 If memcpy can be safely called: copy the data.
   *         #40 Otherwise: set error.
   * #60 Return number of characters written.
   * \endinternal
   */
  template <typename T>
  amsr::core::Result<std::uint64_t> Write(amsr::core::Span<T const> data) noexcept {
    amsr::core::Result<std::uint64_t> result{0};
    std::size_t count{data.size()};

    if ((pos_in_buffer_ > 0) || (count < buffer_view_.size())) {  // #15
      count = std::min(
          count, buffer_view_.size() - pos_in_buffer_);  // Adjust write size in case more data is too large for buffer
      if (count > 0) {                                   // #20
        if (FulfillsMemcpyPrerequisites<char>(buffer_view_.subspan(pos_in_buffer_), data.begin(), data.end(),
                                              count)) {  // #30
          // VCA_IOSTREAM_STD_MEMCPY, VCA_IOSTREAM_IN_BOUND_ACCESS_WRITE
          std::memcpy(&buffer_view_[pos_in_buffer_], data.data(), count);
          SafelyIncrementPosition(count);
          result.EmplaceValue(amsr::charconv::ConvertOrAbort<std::uint64_t>(count));
        } else {  // #40
          result.EmplaceError(
              StreamErrc::kInvalidArgument,
              amsr::core::String("FileBuffer::Write: Data cannot be copied to internal buffer.").c_str());
        }
      }
    }

    // #60
    return result.Map([](std::size_t chars) { return amsr::charconv::Convert<std::uint64_t>(chars); });
  }

  /*!
   * \brief Clears buffer if in read mode.
   * \return If in read mode, position before buffer was cleared. Otherwise 0.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::uint64_t SwitchToWriteMode() noexcept;

  /*!
   * \brief Indicates if the stream is currently used for writing.
   * \details The stream can either be in write mode or in read mode at a time. Before switching the mode, written data
   * needs to be flushed.
   * \return \c true if in write mode, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool IsInWriteMode() const noexcept;

  /*!
   * \brief Sets the number of buffered characters e.g. after a buffer refill.
   * \param[in] new_chars_buffered The number of characters now available in the buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void Init(std::uint64_t new_chars_buffered);

  /*!
   * \brief Clear buffer.
   * \details Resets position in buffer and number of characters buffered.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void Clear();
  /*!
   * \brief Returns the size of the internal buffer.
   * \return The size of the internal buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::size_t GetSize() const noexcept;

  /*!
   * \brief Get number of chars buffered.
   * \return Number of chars buffered.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::size_t GetCharsBuffered() const;

  /*!
   * \brief Get the current position in buffer.
   * \return The current position in buffer, measured in bytes from the beginning of the buffer.
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::uint64_t GetPosition() const noexcept;

  /*!
   * \brief Get the number of remaining characters in buffer.
   * \return The number of characters left.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char>::index_type GetRemainingSize() const noexcept;
  /*!
   * \brief Checks if the buffer is empty or completely read.
   * \return \c true, if buffer is empty or completely read, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool IsEmptyOrCompletelyRead() const noexcept;

  /*!
   * \brief Checks if the current position in the buffer is equal to size of the buffer.
   * \return \c true, if the current position equals size of buffer, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool AtEndOfBuffer() const noexcept;

  /*!
   * \brief Returns view on whole buffer.
   * \return View on buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char> GetBufferView() const noexcept;

  /*!
   * \brief Returns view on section of buffer with modified data.
   * \return View on buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char> GetProcessedBuffer() const noexcept;

 private:
  /*!
   * \brief Safely increments \c pos_in_buffer_ by \c count.
   * \param[in] count \c pos_in_buffer_ is incremented by \c count.
   * \context ANY
   * \pre Aborts if updating \c pos_in_buffer_ would exceed the bounds of \c buffer_view_.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void SafelyIncrementPosition(std::size_t count) noexcept;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__FileBuffer__Test, SafelyIncrementPosition__InsideBuffer);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__FileBuffer__Test, SafelyIncrementPosition__OutsideBuffer);

  /*! Optionally used owned buffer. */
  std::unique_ptr<ara::core::Vector<char>> owned_buffer_{nullptr};

  /*! View into the (potentially borrowed) buffer. */
  amsr::core::Span<char> buffer_view_{};

  /*! Current position in buffer, in [0, chars_buffered-1]. */
  std::size_t pos_in_buffer_{0};

  /*! Total number of chars available in buffer, not the number of remaining chars. */
  std::size_t chars_buffered_{0};
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_BUFFER_H_
