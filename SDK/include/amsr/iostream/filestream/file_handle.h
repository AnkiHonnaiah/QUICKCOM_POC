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
 *        \brief  Handle identifying a file.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_HANDLE_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_HANDLE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <climits>
#include <limits>
#include <memory>
#include <type_traits>

#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/core/posix_error_domain.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/filestream/file_permission.h"
#include "amsr/iostream/filestream/open_mode.h"
#include "amsr/iostream/filestream/posix_abstraction_interface.h"
#include "amsr/iostream/filestream/std_io_file_descriptor.h"
#include "amsr/iostream/types.h"
#include "amsr/iostream/utility.h"

namespace amsr {
namespace stream {
namespace filestream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_file_handle
/*!
 * \brief An instance of a FileHandle identifies a file.
 * \details -
 * \unit amsr::stream::filestream::FileHandle
 * \vprivate Component Private
 */
class FileHandle final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] abstraction Posix abstraction.
   * \context ANY
   * \pre Aborts if \c abstraction is nullptr.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit FileHandle(std::unique_ptr<PosixAbstractionInterface>&& abstraction) noexcept;

  // unique_ptr is not copy constructable
  FileHandle(FileHandle const&) = delete;

  // unique_ptr is not copy assignable
  FileHandle& operator=(FileHandle const&) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] fh Instance of FileHandle
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  FileHandle(FileHandle&& fh) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] fh Instance of FileHandle
   * \return Moved file handle
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  FileHandle& operator=(FileHandle&& fh) & noexcept = default;  // VCA_IOSTREAM_STD_UNIQUE_PTR_MOVE_ASSIGNMENT_CONTRACT

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  ~FileHandle() noexcept = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Opens the associated file of the stream with the given flags and modes.
   * \param[in] file_name File name of file to open.
   * \param[in] flags Bitwise-inclusive OR combination of following list: kRead, kWrite, kReadWrite, kAppend, kCreate,
   * kExcl, kSync, kTruncate. Exactly one of the first three values (file access modes) shall be specified.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * EMFILE, ENFILE or ENOSPC.
   * \error StreamErrc::kInvalidArgument One argument or the combination of arguments is invalid. Underlying cause is
   * one of \c errno EEXIST, EISDIR, ELOOP, ENAMETOOLONG, ENOTDIR, EOVERFLOW, EROFS, EINVAL, EACCES or ENXIO.
   * \error StreamErrc::kNoSuchFileOrDirectory Caused by \c errno ENOENT.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Open(amsr::core::StringView file_name, OpenModeType flags) noexcept;

  /*!
   * \brief Opens the associated file of the stream with the given flags and modes.
   * \param[in] file_name File name of file to open.
   * \param[in] flags Bitwise-inclusive OR combination of following list (exactly one of the first three values (file
   * access modes) shall be specified): kRead, kWrite, kReadWrite, kAppend, kCreate, kExcl, kSync, kTruncate.
   * \param[in] permission Only used when the file is created. Defines the permission bits. See
   * https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html for explanation.
   * \return Result with value void or embedded posix error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * EMFILE, ENFILE or ENOSPC.
   * \error StreamErrc::kInvalidArgument One argument or the combination of arguments is invalid. Underlying cause is
   * one of \c errno EEXIST, EISDIR, ELOOP, ENAMETOOLONG, ENOTDIR, EOVERFLOW, EROFS, EINVAL or EACCES.
   * \error StreamErrc::kNoSuchFileOrDirectory Caused by \c errno ENOENT.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Open(amsr::core::StringView file_name, OpenModeType flags,
                                FilePermission permission) noexcept;

  /*!
   * \brief Opens a file stream to STDOUT / STDERR.
   * \param[in] std_file_descriptor File handle to open (STDOUT_FILENO / STDERR_FILENO).
   * \return Valid result if operation succeeded, otherwise error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is \c errno EMFILE.
   * \error StreamErrc::kInvalidArgument File descriptor limit reached caused by \c errno EINVAL, or invalid
   * return value caused by \c errno EOVERFLOW.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> OpenStdIoHandle(StdIoFileDescriptor const std_file_descriptor) noexcept;

  /*!
   * \brief Closes a file.
   * \details Calling close on an already closed file, does not have any effect (no error).
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Close() noexcept;

  /*!
   * \brief Synchronizes with file if necessary.
   * \param[in] requires_sync Whether data has to be synchronized.
   * \return Result which is valid if operation succeeded.
   * \context ANY
   * \pre Aborts if file handle is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument File descriptor does not allow this operation caused by \c errno
   * EINVAL.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Sync(FsNeedsSync requires_sync) const noexcept;

  /*!
   * \brief Changes the file position of the file.
   * \param[in] offset Offset to change the file position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file.
   * - kEnd: offset is a count of characters from the end of the file.
   * - kCur: offset is a count of characters from the current file position.
   * \return The updated file position in the file, measured in bytes from the beginning of the file.
   * \context ANY
   * \pre Aborts if file handle is closed.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits caused by \c errno EOVERFLOW or \c whence
   * argument invalid caused by \c errno EINVAL or EOVERFLOW.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept;

  /*!
   * \brief Write a span to file.
   * \tparam T Data type to write.
   * \param[in] data Span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 while not all requested characters are written
   *     #20 call Posix write and process returned value
   *     #30 break if error occurred or all data has been written
   * #40 update file size and position in file
   * \endinternal
   */
  template <typename T,
            std::enable_if_t<std::is_same<T, char const>::value || std::is_same<T, vac::language::byte const>::value,
                             std::int32_t> = 0>
  auto WriteToFile(amsr::core::Span<T> data) noexcept -> amsr::core::Result<std::uint64_t> {
    using vac::container::literals::operator""_sv;
    amsr::core::Result<std::size_t> result{StreamErrc::kUnexpected};
    std::size_t written_chars{0};
    std::size_t remaining_chars{data.size()};

    // #10
    while (remaining_chars > 0) {
      // #20
      // VCA_IOSTREAM_NO_NULL_POINTER_CLASS_MEMBER, VCA_IOSTREAM_UNDETERMINED_POINTS_TO_SET_SUBSPAN
      std::int64_t const posix_result{posix_->Write(data.subspan(written_chars, remaining_chars))};
      result = HandlePosixWriteResult(posix_result);

      // #30
      if (!result.HasValue()) {
        break;
      }

      std::size_t const written_chars_in_call{result.Value()};
      if (written_chars_in_call > remaining_chars) {
        if (written_chars_in_call <=
            static_cast<std::size_t>(std::numeric_limits<amsr::core::ErrorDomain::SupportDataType::CodeType>::max())) {
          result.EmplaceError(
              MakeErrorCode(StreamErrc::kUnexpected,
                            static_cast<amsr::core::ErrorDomain::SupportDataType::CodeType>(written_chars_in_call),
                            "More bytes written than expected"_sv.c_str()));
        } else {
          result.EmplaceError(MakeErrorCode(StreamErrc::kUnexpected, 0, "More bytes written than expected"_sv.c_str()));
        }
        break;
      }

      written_chars += written_chars_in_call;
      remaining_chars -= written_chars_in_call;
    }

    // #40
    bool const was_successful{remaining_chars == 0};
    if (was_successful) {
      pos_in_file_ += written_chars;
      file_size_ = amsr::charconv::ConvertOrAbort<std::size_t>(std::max(pos_in_file_, file_size_));
      result.EmplaceValue(written_chars);
    }

    return result.Map([](std::size_t chars) { return amsr::charconv::Convert<std::uint64_t>(chars); });
  }

  /*!
   * \brief Repeatedly calls Posix read until either all requested data is read, EOF is reached or an error occurred.
   * \tparam T Data type to read.
   * \param[out] data Memory to save the read bytes to.
   * \param[in] offset Start of buffer measured in bytes from beginning of file.
   * \return A span of the given memory which is either the same size, if enough data was retrieved from the file, or
   * smaller, if end of file was reached.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #05 Calculate maximum possible amount to read.
   * #10 While not all requested characters are read:
   *     #20 Call posix read.
   *     #30 Break if at EOF or error occurred.
   *     #40 Abort if more data was read than requested.
   * #50 If successful: update position in file.
   * \endinternal
   */
  template <typename T, std::enable_if_t<std::is_same<T, char>::value || std::is_same<T, vac::language::byte>::value,
                                         std::int32_t> = 0>
  auto ReadFromFile(amsr::core::Span<T> data, std::uint64_t offset) noexcept
      -> amsr::core::Result<amsr::core::Span<T>> {
    amsr::core::Result<amsr::core::Span<T>> result{
        amsr::core::Result<amsr::core::Span<T>>::FromError(StreamErrc::kUnexpected)};
    std::size_t remaining_chars{data.size()};
    std::size_t chars_read{0};
    std::int64_t read_result{-1};  // init to something different from 0

    // #05
    std::size_t const file_size_left_to_read{GetCurrentFileSize() - offset};
    if (remaining_chars > file_size_left_to_read) {
      remaining_chars = file_size_left_to_read;
    }

    // #10
    while ((remaining_chars > 0) && (read_result != 0)) {
      // #20
      amsr::core::Span<T> const buffer_to_fill{data.subspan(chars_read, remaining_chars)};
      // VCA_IOSTREAM_NO_NULL_POINTER_CLASS_MEMBER, VCA_IOSTREAM_UNDETERMINED_POINTS_TO_SET_SUBSPAN
      std::int64_t const posix_result{posix_->Read(buffer_to_fill)};
      amsr::core::Result<std::int64_t> const handled_read_result{HandlePosixReadResult(posix_result)};

      // #30
      if (!handled_read_result.HasValue()) {
        result.EmplaceError(handled_read_result.Error());
        break;
      }

      // #40
      read_result = handled_read_result.Value();
      std::size_t const chars_just_read{amsr::charconv::ConvertOrAbort<std::size_t>(read_result)};
      if (chars_just_read > remaining_chars) {
        Abort(__FILE__, __LINE__, amsr::core::StringView{"More data read than allowed."});
      }

      chars_read += chars_just_read;
      remaining_chars -= chars_just_read;
    }

    // #50
    bool const was_successful{(remaining_chars == 0) || (read_result == 0)};
    if (was_successful) {
      pos_in_file_ += chars_read;
      result.EmplaceValue(amsr::core::Span<T>{data.subspan(0, chars_read)});
    }

    return result;
  }

  /*!
   * \brief Flushes buffer to file.
   * \param[in] data Data to flush.
   * \param[in] offset Position to start writing the data to.
   * \return Result with value if successful.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Flush(amsr::core::Span<char const> data, std::uint64_t offset) noexcept;

  /*!
   * \brief Returns the size of a file.
   * \details The size of a closed file is 0.
   * \return The file size.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::uint64_t GetCurrentFileSize() const noexcept;

 private:
  /*!
   * \brief Sets file name.
   * \param[in] file Name of the file (including path).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void SetFileName(amsr::core::StringView file) noexcept;

  /*!
   * \brief Sets the initial length of the file.
   * \return Successfully if file size can be determined.
   * \context ANY
   * \pre Aborts if file handle is closed (checked by FileHandle::Seek).
   * \threadsafe FALSE
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> SetInitialFileSize() noexcept;

  /*!
   * \brief Checks if file is open and sets file properties or sets \c StreamErrc depending on \c errno.
   * \param[in] om Open mode.
   * \param[in] successful Whether Posix call succeeded.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Caused by \c errno ENOBUFS, ENOSPC, ENFILE or EMFILE.
   * \error StreamErrc::kInvalidArgument Caused by \c errno EEXIST, EFBIG, ENAMETOOLONG, EINVAL, EISDIR, ENXIO, ENOTDIR,
   * EACCES, EROFS, ELOOP or EOVERFLOW.
   * \error StreamErrc::kNoSuchFileOrDirectory Caused by \c errno ENOENT.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> HandlePosixOpenResult(OpenMode om, bool successful) noexcept;

  /*!
   * \brief Resets file properties.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  void ResetFileProperties() noexcept;

  /*!
   * \brief Converts \c errno of PosixWrite call into \c StreamErrc.
   * \details Result values are passed on unmodified.
   * \param[in] write_result Result of PosixWrite call.
   * \return The number of characters written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::size_t> HandlePosixWriteResult(std::int64_t const write_result) const noexcept;

  /*!
   * \brief Converts \c errno of PosixRead call into \c StreamErrc.
   * \details Result values are passed on unmodified.
   * \param[in] read_result Result of PosixRead call.
   * \return The number of characters read.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> HandlePosixReadResult(std::int64_t read_result) const noexcept;

  /*!
   * \brief Converts \c errno of PosixLseek call into \c StreamErrc.
   * \details Result values are passed on unmodified.
   * \param[in] lseek_result Result of PosixLseek call.
   * \return The updated file position.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits caused by \c errno EOVERFLOW or whence
   * argument invalid caused by \c errno EINVAL.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> HandlePosixLseekResult(std::int64_t lseek_result) noexcept;

  /*!
   * \brief Converts \c errno of PosixFdatasync call into \c StreamErrc.
   * \param[in] successful Whether Posix call succeeded.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument File descriptor does not allow this operation caused by \c errno
   * EINVAL.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> HandlePosixFdatasyncResult(bool successful) const noexcept;

  /*!
   * \brief Converts \c errno of PosixFcntl call into \c StreamErrc.
   * \param[in] successful Whether Posix call succeeded.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is \c errno EMFILE.
   * \error StreamErrc::kInvalidArgument File descriptor limit reached caused by \c errno EINVAL, or invalid
   * return value caused by \c errno EOVERFLOW.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> HandlePosixFcntlResult(bool successful) const noexcept;

  /*!
   * \brief Converts \c errno of PosixClose call into \c StreamErrc.
   * \param[in] successful Whether Posix call succeeded.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> HandlePosixCloseResult(bool successful) noexcept;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__FileHandle__Test, MoveConstruct);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__FileHandle__Test, MoveAssign);

  /*! Name of the file to handle */
  amsr::core::String file_name_{};

  /*! Current position in file */
  std::uint64_t pos_in_file_{0};

  /*! Length of file. */
  std::uint64_t file_size_{0};

  /*! Posix Abstraction */
  std::unique_ptr<PosixAbstractionInterface> posix_{nullptr};
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_HANDLE_H_
