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
 *        \brief  Defines an error domain for stream classes
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STREAM_ERROR_DOMAIN_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STREAM_ERROR_DOMAIN_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>

#include "amsr/core/array.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "amsr/core/result.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace stream {

/*!
 * \brief Stream error code enum.
 * \vprivate Product Private
 */
enum class StreamErrc : amsr::core::ErrorDomain::CodeType {
  // LibIoStream error codes
  kUnexpected = 1,            /*!< unexpected error */
  kInvalidPosition = 2,       /*!< position in stream can't be determined */
  kEndOfFile = 3,             /*!< end of file */
  kInvalidArgument = 4,       /*!< invalid argument */
  kInsufficientResources = 5, /*!< not enough buffer space or not enough memory */
  kIoError = 6,               /*!< I/O error */
  kInvalidAccess = 7,         /*!< invalid access, used only in IoIntegrityStream */
  kNoSuchFileOrDirectory = 8, /*!< no such file or directory, used in Persistency */
  kNotSupported = 9,          /*!< function not supported */
};

/*!
 * \brief Exception type for StreamErrorDomain.
 * \details
 * \vprivate Component Private
 */
class StreamException final : public amsr::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   * \vprivate Component Private
   */
  using Exception::Exception;
};

/*!
 * \brief Error domain for stream system errors.
 * \unit amsr::stream::StreamErrorDomain
 * \vprivate Product Private
 */
class StreamErrorDomain final : public amsr::core::ErrorDomain {
  /*! ID of the domain */
  static constexpr amsr::core::ErrorDomain::IdType kId{0x24a8f68494035d5b};

 public:
  /*!
   * \brief Error code enum of the domain.
   * \vprivate Component Private
   */
  using Errc = StreamErrc;
  /*!
   * \brief Exception type of the domain.
   * \vprivate Component Private
   */
  using Exception = StreamException;

  /*!
   * \brief Constructs instance of class.
   * \details Initializes the domain id with StreamErrorDomain::kId.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  constexpr StreamErrorDomain() noexcept : amsr::core::ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return The name as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::ErrorDomain::StringType Name() const noexcept final {
    return "Stream";
  }  // COV_STREAM_ERROR_DOMAIN_NAME_FUNCTION

  /*!
   * \brief Returns a textual representation of the given error code.
   * \details The return value is undefined if the \c error_code did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::ErrorDomain::StringType Message(amsr::core::ErrorDomain::CodeType error_code) const noexcept final {
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<amsr::core::ErrorDomain::CodeType>::max()),
                  "amsr::core::ErrorDomain::CodeType must not exceed std::size_t.");

    constexpr static amsr::core::Array<amsr::core::ErrorDomain::StringType, 10> kMessages{{
        // LibIoStream error codes
        "Unknown error code",                      // dummy message as there is no error == 0
        "Unexpected error encountered",            // kUnexpected
        "Position in stream can't be determined",  // kInvalidPosition
        "End of file",                             // kEndOfFile
        "Invalid argument",                        // kInvalidArgument
        "Insufficient resources",                  // kInsufficientResources
        "I/O error",                               // kIoError
        "Invalid access",                          // kInvalidAccess
        "No such file or directory",               // kNoSuchFileOrDirectory
        "Function not supported"                   // kNotSupported
    }};

    std::size_t index{0};
    if (error_code > 0) {  // COV_ERROR_CODE_POSITIVE
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kMessages.size()) {  // COV_ERROR_CODE_WITHIN_ARRAY
        index = code;
      }
    }
    return kMessages[index];
  }  // COV_STREAM_ERROR_DOMAIN_MESSAGE_FUNCTION

  /*!
   * \brief Throws the given ErrorCode as Exception.
   * \param[in] error_code error code to be thrown.
   * \remark If the code is compiled without exceptions, this function call will terminate.
   * \context ANY
   * \pre -
   * \throws StreamException for all error codes.
   * \threadsafe FALSE
   * \exceptionsafety Exception in all cases
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }  // COV_STREAM_ERROR_DOMAIN_THROW_AS_EXCEPTION_FUNCTION
};   // namespace stream

namespace internal {
/*!
 * \brief Global StreamErrorDomain instance.
 * \vprivate Component Private
 */
constexpr StreamErrorDomain kStreamErrorDomain;
}  // namespace internal

/*!
 * \brief Returns a reference to the global StreamErrorDomain instance.
 * \return StreamErrorDomain instance.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::stream::StreamErrorDomain
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
inline constexpr amsr::core::ErrorDomain const& GetStreamDomain() noexcept {
  return internal::kStreamErrorDomain;  // VCA_IOSTREAM_REF_TO_GLOBAL_VAR
}

/*!
 * \brief Creates an error code from StreamErrorDomain
 * \param[in] code The specific error code
 * \param[in] support_data Vendor defined support data
 * \param[in] user_message An optional message for this error
 * \return The constructed error code
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \unit amsr::stream::StreamErrorDomain
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(StreamErrc code,
                                                     amsr::core::ErrorDomain::SupportDataType support_data = {},
                                                     amsr::core::ErrorDomain::StringType user_message = nullptr) {
  return {static_cast<amsr::core::ErrorDomain::CodeType>(code), GetStreamDomain(), support_data, user_message};
}

/*!
 * \brief Creates an error code from StreamErrorDomain
 * \param[in] code The specific error code
 * \param[in] message An optional message for this error
 * \return The constructed error code
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(StreamErrorDomain::Errc code, char const* message) {
  return {static_cast<amsr::core::ErrorDomain::CodeType>(code), GetStreamDomain(), 0, message};
}

}  // namespace stream
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_STREAM_ERROR_DOMAIN_NAME_FUNCTION
//   \ACCEPT XX
//   \REASON Tooling issue (TAR-7127). Tested with TEST_F(UT__StreamErrorDomain, Name).
// \ID COV_STREAM_ERROR_DOMAIN_MESSAGE_FUNCTION
//   \ACCEPT XX
//   \REASON Tooling issue (TAR-7127). Tested with TEST_F(UT__StreamErrorDomain, Message__ValidErrorCode).
// \ID COV_STREAM_ERROR_DOMAIN_THROW_AS_EXCEPTION_FUNCTION
//   \ACCEPT XX
//   \REASON Tooling issue (TAR-7127). Tested with TEST_F(UT__StreamErrorDomain, ThrowAsException).
// \ID COV_ERROR_CODE_WITHIN_ARRAY
//   \ACCEPT XX
//   \REASON Tooling issue (TAR-7127). Both conditions tested.
//           TX: TEST_F(UT__StreamErrorDomain, Message__ValidErrorCode)
//           XF: TEST_F(UT__StreamErrorDomain, Message__InvalidNegativeErrorCode)
// \ID COV_ERROR_CODE_POSITIVE
//   \ACCEPT XX
//   \REASON Tooling issue (TAR-7127). Both conditions tested.
//           TX: TEST_F(UT__StreamErrorDomain, Message__ValidErrorCode)
//           XF: TEST_F(UT__StreamErrorDomain, Message__InvalidPositiveErrorCode)
// COV_JUSTIFICATION_END

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STREAM_ERROR_DOMAIN_H_
