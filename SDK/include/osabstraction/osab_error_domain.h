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
/**        \file  osab_error_domain.h
 *        \brief  Common error codes in OS Abstraction.
 *        \unit   osabstraction::Error
 *
 *      \details  Vector defined error codes for OS Abstraction.
 *
 *********************************************************************************************************************/

#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_OSAB_ERROR_DOMAIN_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_OSAB_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace osabstraction {

/*!
 * \brief    Osab error code enum.
 *
 * \vprivate Vector product internal API
 */
enum class OsabErrc : ::amsr::core::ErrorDomain::CodeType {
  /*!
   * \brief    One of the given parameters is invalid or there has been an invalid call sequence of Osab functions.
   */
  kApiError = 1,

  /*!
   * \brief    A handle is invalid.
   */
  kInvalidHandle = 2,

  /*!
   * \brief    A resource could not be acquired or allocated.
   */
  kResource = 3,

  /*!
   * \brief    An unrecoverable error was encountered.
   */
  kFatal = 4,

  /*!
   * \brief    Process creation failed.
   */
  kProcessCreationFailed = 5,

  /*!
   * \brief    Process creation failed. Calling process must be terminated.
   */
  kProcessCreationFailedFatal = 6,

  /*!
   * \brief    Process creation failed because of a conflicting operation in another thread.
   */
  kProcessCreationFailedMultithreaded = 7,

  /*!
   * \brief    By itself valid address can not be used for the requested service.
   */
  kAddressError = 8,

  /*!
   * \brief    Address temporarily or permanently unavailable.
   */
  kAddressNotAvailable = 9,

  /*!
   * \brief    Service can currently not be used, usage may be possible later.
   */
  kBusy = 10,

  /*!
   * \brief    Communication partner is not reachable.
   */
  kDisconnected = 11,

  /*!
   * \brief    The system environment is not in the expected state.
   */
  kSystemEnvironmentError = 12,

  /*!
   * \brief    Service did not complete within specified timeout, event did not occur.
   */
  kTimeout = 13,

  /*!
   * \brief    Data did not fit into provided buffer and was truncated.
   */
  kTruncation = 14,

  /*!
   * \brief    Lack of permissions.
   */
  kInsufficientPrivileges = 15,

  /*!
   * \brief   Already connected.
   */
  kAlreadyConnected = 16,

  /*!
   * \brief   The object does not exist.
   */
  kDoesNotExist = 17,

  /*!
   * \brief   The object does already exist.
   */
  kAlreadyExists = 18,

  /*!
   * \brief    Object is not initialized.
   */
  kUninitialized = 19,

  /*!
   * \brief    Communication protocol error.
   */
  kProtocolError = 20,

  /*!
   * \brief    Unsupported size.
   */
  kSize = 21,

  /*!
   * \brief    An unexpected error was detected.
   */
  kUnexpected = 22
};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief    Exception type for OsabErrorDomain.
 * \vprivate Vector product internal API
 */
class OsabException final : public ::amsr::core::Exception {
 public:
  /*!
   * \brief    Construct a new OsabException object with a specific ErrorCode.
   * \vprivate Vector product internal API
   */
  using Exception::Exception;
};

/*!
 * \brief    Error domain for all OS Abstraction error codes.
 * \details  An amsr::core::ErrorDomain cannot be copied or moved.
 * \vprivate Vector product internal API
 */
class OsabErrorDomain final : public ::amsr::core::ErrorDomain {
  /*!
   * \brief ID of the OS Abstraction error domain.
   */
  static constexpr ErrorDomain::IdType kId{0x64c62e540e14ba70};

 public:
  /*!
   * \brief    Constructor. Initializes the domain id with OsabErrorDomain::kId.
   *
   * \reentrant   TRUE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr OsabErrorDomain() noexcept : ErrorDomain(kId) {}

  OsabErrorDomain(OsabErrorDomain const&) noexcept = delete;
  OsabErrorDomain(OsabErrorDomain&&) noexcept = delete;
  OsabErrorDomain& operator=(OsabErrorDomain const&) & noexcept = delete;
  OsabErrorDomain& operator=(OsabErrorDomain&&) & noexcept = delete;

  /*!
   * \brief    Return the name of the OS Abstraction error domain.
   *
   * \return      Name of the OS Abstraction error domain.
   *
   * \reentrant   TRUE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  char const* Name() const noexcept final { return "Osab"; }

  /*!
   * \brief       Return a textual representation of the given error code.
   *
   * \param[in]   error_code OS Abstraction error code
   *
   * \return      The text as a null-terminated C-string, never nullptr.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   TRUE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  char const* Message(ErrorDomain::CodeType error_code) const noexcept final {
    constexpr static std::array<char const*, 23> kMessages{
        {"Unknown error code.",
         "One of the given parameters is invalid.",
         "A handle is invalid.",
         "A resource could not be acquired or allocated.",
         "An unrecoverable error was encountered.",
         "Creating a child process failed.",
         "Creating a child process failed. The context of the calling process has been irreversibly corrupted, the "
         "process must not continue its operation.",
         "Process creation failed because of a conflicting operation in another thread.",
         "An input address can not be used for the requested service but is by itself valid.",
         "An input address is temporarily or permanently unavailable.",
         "The requested service can currently not be used but usage may become possible later without user "
         "intervention required.",
         "A communication partner could not be reached.",
         "An error occurred due to the system environment not being in the expected state.",
         "The service did not complete within the specified timeout because an event it waited for did not occur.",
         "Some data did not fit into a provided buffer and was truncated.",
         "Lack of permissions during execution of the requested service.",
         "The object is already in a connected state.",
         "The object does not exist.",
         "The object does already exist.",
         "The object is used while not initialized.",
         "Error in communication protocol has occurred.",
         "The given size is not supported by this service.",
         "Some error that is not expected to happen within the respective context was detected."}};

    // VECTOR Next Line AutosarC++17_10-M0.1.2: MD_OSA_M0.1.2_dead_branch_false_positive
    if ((error_code < 0) || (error_code >= static_cast<std::int32_t>(kMessages.size()))) {
      error_code = 0;
    }

    // VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_OsabErrorDomainMessage
    // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_OSA_M0.3.1_OsabErrorDomainMessage
    // VECTOR Next Construct AutosarC++17_10-A4.7.1: MD_OSA_A4.7.1_OsabErrorDomainMessage
    // VECTOR Next Line AutosarC++17_10-M5.0.16: MD_OSA_M5.0.16_OsabErrorDomainMessage
    return kMessages[static_cast<std::size_t>(error_code)];
  }

  /*!
   * \brief       Throws the given error code as OsabException.
   *
   * \details     If the executable is compiled without exceptions, this function call will terminate.
   *
   * \param[in]   error_code error code to be thrown.
   *
   * \throws      OsabException for all error codes.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   TRUE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      FALSE
   *
   * \vprivate    Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void ThrowAsException(::amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<OsabException>(error_code);
  }

  /*!
   * \brief    Destructor.
   *
   * \details  This dtor is non-virtual (and trivial) so that this class can be a literal type. While this class has
   *           virtual functions, no polymorphic destruction is needed.
   *
   * \reentrant   TRUE
   * \synchronous TRUE
   * \threadsafe  TRUE
   * \steady      TRUE
   *
   * \vprivate Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~OsabErrorDomain() noexcept = default;
};

namespace internal {
/*!
 * \brief Global OsabErrorDomain instance.
 */
constexpr OsabErrorDomain kOsabErrorDomain;
}  // namespace internal

/*!
 * \brief    Returns a reference to the global OsabErrorDomain instance.
 *
 * \return      Reference to global OsabErrorDomain instance.
 *
 * \reentrant   TRUE
 * \synchronous TRUE
 * \threadsafe  TRUE
 * \steady      TRUE
 *
 * \vprivate Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ::amsr::core::ErrorDomain const& GetOsabDomain() { return internal::kOsabErrorDomain; }

/*!
 * \brief       Creates an ErrorCode from the OsabErrorDomain.
 *
 * \param[in]   code    error code enum value
 * \param[in]   data    support data.
 * \param[in]   message user message.
 *
 * \return      The created error code.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   TRUE
 * \synchronous TRUE
 * \threadsafe  TRUE
 * \steady      TRUE
 *
 * \vprivate    Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ::amsr::core::ErrorCode MakeErrorCode(OsabErrc code, ::amsr::core::ErrorDomain::SupportDataType data,
                                                       char const* message) {
  return ::amsr::core::ErrorCode(static_cast<::amsr::core::ErrorDomain::CodeType>(code), GetOsabDomain(), data,
                                 message);
}

/*!
 * \brief       Creates an ErrorCode from the OsabErrorDomain.
 *
 * \param[in]   code    error code enum value
 * \param[in]   data    support data.
 *
 * \return      The created error code.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   TRUE
 * \synchronous TRUE
 * \threadsafe  TRUE
 * \steady      TRUE
 *
 * \vprivate    Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ::amsr::core::ErrorCode MakeErrorCode(OsabErrc code, ::amsr::core::ErrorDomain::SupportDataType data) {
  return ::amsr::core::ErrorCode(static_cast<::amsr::core::ErrorDomain::CodeType>(code), GetOsabDomain(), data,
                                 nullptr);
}

/*!
 * \brief       Creates an ErrorCode from the OsabErrorDomain.
 *
 * \param[in]   code    error code enum value
 * \param[in]   message user message.
 *
 * \return      The created error code.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   TRUE
 * \synchronous TRUE
 * \threadsafe  TRUE
 * \steady      TRUE
 *
 * \vprivate    Vector product internal API
 */
inline constexpr ::amsr::core::ErrorCode MakeErrorCode(OsabErrc code, char const* message) {
  return ::amsr::core::ErrorCode(static_cast<::amsr::core::ErrorDomain::CodeType>(code), GetOsabDomain(), 0, message);
}

/*!
 * \brief       Creates an ErrorCode from the OsabErrorDomain.
 *
 * \param[in]   code    error code enum value
 *
 * \return      The created error code.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   TRUE
 * \synchronous TRUE
 * \threadsafe  TRUE
 * \steady      TRUE
 *
 * \vprivate    Vector product internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr ::amsr::core::ErrorCode MakeErrorCode(OsabErrc code) {
  return ::amsr::core::ErrorCode(static_cast<::amsr::core::ErrorDomain::CodeType>(code), GetOsabDomain(), 0, nullptr);
}

}  // namespace osabstraction

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_OSAB_ERROR_DOMAIN_H_
