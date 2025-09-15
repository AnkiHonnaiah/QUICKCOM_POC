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
 *        \brief  Defines the error domain for the Vector Thread Library.
 *
 *********************************************************************************************************************/

#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_ERROR_DOMAIN_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace thread {

/*!
 * \brief Enumeration for all Error Code values of the Vector Thread Library.
 * \vpublic
 */
enum class ThreadErrc : ara::core::ErrorDomain::CodeType {
  kThreadCreationFailed = 1,
  kInsufficientResources = 2,
  kInsufficientPermissions = 3,
  kInvalidConfig = 4,
  kNonPosixError = 5,
  kJoinThreadFailed = 6,
  kThreadNameOutOfRange = 7,
  kInvalidThreadHandle = 8,
  kThreadConfigJsonValidationFailed = 9,
  kThreadConfigJsonFileOpenFailed = 10,
  kThreadConfigJsonFileParsingFailed = 11
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_THREAD_M3.4.1_CanBeDeclaredLocally
/*!
 * \brief An interface of the Vector Thread Library Exception.
 * \unit amsr::thread::Error
 * \vpublic
 */
class ThreadException : public ara::core::Exception {
 public:
  using Exception::Exception;
};

/*!
 * \brief Thread Error Domain class.
 * \details This class represents an error domain responsible for all errors occurring in the Vector Thread Library.
 * \unit amsr::thread::Error
 * \vpublic
 */
class ThreadErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Thread error domain identifier (unique domain ID).
   */
  constexpr static ara::core::ErrorDomain::IdType kId{0x36dca92f0074c47f};

  /*!
   * \brief Error code type definition.
   * \vpublic
   */
  using Errc = ThreadErrc;

  /*!
   * \brief Constructor for ThreadErrorDomain.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr ThreadErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name for this error domain.
   * \return The name as a null-terminated string, never nullptr.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Name() const noexcept final { return "Thread"; };

  /*!
   * \brief Return the textual description for the given error code.
   * \param[in] error_code The domain specific error code.
   * \return The text as a null-terminated string, never nullptr.
   * \context           ANY
   * \pre               -
   * \threadsafe        TRUE
   * \reentrant         TRUE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Message(ara::core::ErrorDomain::CodeType error_code) const noexcept final;

  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_THREAD_M0.1.8_ThrowAsException
  /*!
   * \brief Throws the given ErrorCode as Exception.
   * \param[in] error_code The ErrorCode to be thrown.
   * \details If the code is compiled w/o exceptions, this function will call terminate instead.
   * \context           ANY
   * \pre               -
   * \throws            ThreadException for all error codes.
   * \threadsafe        TRUE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<ThreadException>(error_code);
  }
};

/*!
 * \brief Internal namespace.
 */
namespace internal {
/*!
 * \brief Global ThreadErrorDomain instance.
 */
constexpr ThreadErrorDomain kThreadErrorDomain;
}  // namespace internal

/*!
 * \brief Factory function of the ThreadErrorDomain.
 * \return Constant reference to the single instance of the ThreadErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           TRUE
 * \synchronous         TRUE
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ara::core::ErrorDomain const& GetThreadErrorDomain() { return internal::kThreadErrorDomain; }

/*!
 * \brief Make ErrorCode instances from the Thread Error Domain.
 * \param[in] code An error code identifier from the ThreadErrc enumeration.
 * \param[in] data Supplementary data for the error description.
 * \param[in] message Additional error message supplied by user code.
 * \return ErrorCode instance always references to ThreadErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(ThreadErrorDomain::Errc code,
                                                    ara::core::ErrorDomain::SupportDataType data,
                                                    char const* message = nullptr) {
  return {static_cast<ara::core::ErrorDomain::CodeType>(code), GetThreadErrorDomain(), data, message};
}

/*!
 * \brief Make ErrorCode instances from the Thread Error Domain.
 * \param[in] code An error code identifier from the ThreadErrc enumeration.
 * \param[in] message Additional error message supplied by user code.
 * \return ErrorCode instance always references to ThreadErrorDomain.
 * \context             ANY
 * \pre                 -
 * \threadsafe          TRUE
 * \reentrant           FALSE
 * \synchronous         TRUE
 * \vpublic
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(ThreadErrorDomain::Errc code, char const* message = nullptr) {
  return {static_cast<ara::core::ErrorDomain::CodeType>(code), GetThreadErrorDomain(), {}, message};
}

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_ERROR_DOMAIN_H_
