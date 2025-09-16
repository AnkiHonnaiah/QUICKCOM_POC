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
 *        \brief   Base class for stateful streams.
 *        \details The class should not be used via StatefulInputStream and StatefulOutputStream by other components.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "amsr/core/result.h"
#include "amsr/iostream/io_format.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/types.h"

namespace amsr {
namespace stream {

/*!
 * \brief Base class for stateful streams.
 * \tparam T Stream interface type.
 * \unit amsr::stream::StatefulStream
 * \vprivate Product Private
 */
template <typename T>
class StatefulStream {
 public:
  /*! Stream state type definition. */
  using State = amsr::core::Result<void>;

  /*!
   * \brief Constructs a stateful stream based on given stream reference.
   * \param[in] sfstream_ref Reference to the underlying stream.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit StatefulStream(T& sfstream_ref) noexcept : result_{State::FromValue()}, stream_ref_{sfstream_ref} {}

  // StatefulStream is not copy constructible because this implies multiple independent stream statuses.
  StatefulStream(StatefulStream const& sfstream) = delete;

  // StatefulStream is not copy assignable because this implies multiple independent stream statuses.
  StatefulStream& operator=(StatefulStream const& sfstream) & = delete;

  /*!
   * \brief Destructor.
   * \spec requires true; \endspec
   * \steady UNKNOWN
   */
  virtual ~StatefulStream() noexcept = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Get the position from the underlying 'stream'-object (e.g. a file or a string).
   * \return The current position, measured in bytes from the beginning of the file/string.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed (checked by FileStream::Tell).
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition Position in stream cannot be determined.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept {
    return this->IfValid([this]() {
      return this->GetStream().Tell();  // VCA_IOSTREAM_STREAM_REF, VCA_IOSTREAM_NO_CONTRACT_VIOLATED
    });
  }

  /*!
   * \brief Changes the position of the underlying 'stream'-object (e.g. a file or a string).
   * \param[in] offset Offset to change the position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file/string.
   * - kEnd: offset is a count of characters from the end of the file/string.
   * - kCur: offset is a count of characters from the current file/string position.
   * \return The updated position in the file/string, measured in bytes from the beginning of the file/string.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not seekable (both checked by
   * FileStream::Seek).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits (can be caused by \c errno EOVERFLOW) or
   * whence argument invalid (can be caused by \c errno EINVAL), or file size exceeds file size limit caused
   * by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept {
    return this->IfValid([this, offset, whence]() {
      return this->GetStream().Seek(offset, whence);  // VCA_IOSTREAM_STREAM_REF, VCA_IOSTREAM_NO_CONTRACT_VIOLATED
    });
  }

  /*!
   * \brief Gets the internal state of the stream.
   * \return The internal state of the stream.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \synchronous TRUE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  State GetState() const noexcept { return result_; }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IoStream_M9.3.3_const_method_objects_modified
  /*!
   * \brief Clears the state of the stream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \synchronous TRUE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Clear() noexcept { result_.EmplaceValue(); }

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
  void SetShowBase() noexcept {
    IfValidDo([this]() { return this->GetStream().SetShowBase(); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void NoShowBase() noexcept {
    IfValidDo([this]() { return this->GetStream().NoShowBase(); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void SetBase(Base base) noexcept {
    IfValidDo([this, base]() { return this->GetStream().SetBase(base); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void SetUppercase() noexcept {
    IfValidDo([this]() { return this->GetStream().SetUppercase(); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void SetLowercase() noexcept {
    IfValidDo([this]() { return this->GetStream().SetLowercase(); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void SetFillChar(char const ch) noexcept {
    IfValidDo([this, ch]() { return this->GetStream().SetFillChar(ch); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void SetWidth(std::uint8_t const output_width) noexcept {
    IfValidDo([this, output_width]() { return this->GetStream().SetWidth(output_width); });  // VCA_IOSTREAM_STREAM_REF
  }

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
  void SetFloatingPointFormat(FloatingPointFormat const fp_format) noexcept {
    IfValidDo([this, fp_format]() {
      return this->GetStream().SetFloatingPointFormat(fp_format);  // VCA_IOSTREAM_STREAM_REF
    });
  }

 protected:
  // VECTOR NC AutosarC++17_10-M9.3.1: MD_IoStream_M9.3.1_nonconst
  /*!
   * \brief Provides access to the underlying stream itself.
   * \return A reference to the underlying stream instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \synchronous TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  T& GetStream() const noexcept {
    return stream_ref_;  // VCA_IOSTREAM_STREAM_REF
  }

  /*!
   * \brief Member access operator overload for more convenient access to the underlying stream.
   * \return A pointer to the underlying stream.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \synchronous TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  T* operator->() const noexcept { return &this->GetStream(); }

  /*!
   * \brief Member access operator overload for more convenient access to the underlying stream.
   * \return A reference to the underlying stream.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \synchronous TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  T& operator*() const noexcept {
    return this->GetStream();  // VCA_IOSTREAM_STREAM_REF
  }

  /*!
   * \brief Move constructor
   * \param[in] sfstream Instance of StatefulStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  StatefulStream(StatefulStream&& sfstream) noexcept = default;

  /*!
   * \brief Move assignment
   * \param[in] sfstream Instance of StatefulStream
   * \return A reference to the stream
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \synchronous TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  StatefulStream& operator=(StatefulStream&& sfstream) & noexcept = default;

  // VECTOR NC AutosarC++17_10-A7.1.5: MD_IoStream_A7.1.5_auto_type_specifier
  /*!
   * \brief Executes the function and saves the Result if the previous actions were successful.
   * \tparam Fn Type of function.
   * \param[in] fn Function to execute.
   * \return The latest results value or an error.
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - If no error occurred before:
   *   - Execute the function and set the state of the stream from its return value.
   * \endinternal
   */
  template <typename Fn>
  auto IfValid(Fn const& fn) const noexcept -> auto{  // NOLINT(whitespace/braces)
    /*!
     * \brief Define for std::result_of<Fn()>::type
     */
    using R = typename std::result_of<Fn()>::type;
    R res{R::FromError(StreamErrc::kUnexpected)};

    if (this->result_.HasValue()) {
      // this->result_ must be checked again and set because it might contain an error.
      res = fn();
      this->result_ = res.And(this->result_);
    }

    return this->result_ ? res : R::FromError(this->result_.Error());
  }

  /*!
   * \brief Executes the function and saves the Result if the previous actions were successful.
   * \tparam Fn Type of function.
   * \param[in] fn Function to execute.
   * \context ANY
   * \pre Callable does not throw exceptions and remains valid during function call.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - If no error occurred before:
   *   - Execute the function and set the state of the stream from its return value.
   * \endinternal
   */
  template <typename Fn>
  void IfValidVoid(Fn const& fn) const noexcept {
    if (this->result_.HasValue()) {
      // this->result_ must be checked again and set because it might contain an error.
      /*!
       * \brief Define for std::result_of<Fn()>::type
       */
      using Re = typename std::result_of<Fn()>::type;
      Re const res{fn()};  // VCA_IOSTREAM_LAMBDA_VALID
      this->result_ = res.And(this->result_);
    }
  }

  /*!
   * \brief Executes the function if the previous actions were successful.
   * \tparam Fn Type of function.
   * \param[in] fn Function to execute.
   * \context ANY
   * \pre Callable does not throw exceptions.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - If no error occurred before:
   *   - Execute the function.
   * \endinternal
   */
  template <typename Fn>
  void IfValidDo(Fn const& fn) const noexcept {
    if (this->result_.HasValue()) {
      fn();
    }
  }

 private:
  /*! State of the stream. Contains the first error that occurred. */
  mutable State result_;

  /*! Reference to the stream used by this instance. */
  std::reference_wrapper<T> stream_ref_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__GoodStream_SuccessfulFunction);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__GoodStream_FailingFunction);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__BadStream_SuccessfulFunction);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, IfValidVoid__BadStream_FailingFunction);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, OperatorStar);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, OperatorArrow);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, MoveConstructor);

  /*! Friend test declaration. */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  template <typename S>
  FRIEND_TEST(UT__StatefulStream, MoveAssignment);
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_STREAM_H_
