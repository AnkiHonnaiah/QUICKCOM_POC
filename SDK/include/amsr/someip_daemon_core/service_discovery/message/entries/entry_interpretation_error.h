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
/**        \file  entry_interpretation_error.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_ENTRY_INTERPRETATION_ERROR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_ENTRY_INTERPRETATION_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace entries {

/*!
 * \brief SD message interpretation error messages
 */
enum class SdEntryInterpretationError : ara::core::ErrorDomain::CodeType {
  kWrongEntryType = 0U, /*! Wrong API used for a given entry type */
  kInvalidEntryId = 1U  /*! Invalid entry id */
};

/*!
 * \brief Error messages
 */
// VECTOR NC AutosarC++17_10-M3.4.1:MD_SomeIpDaemon_AutosarC++17_10_M3.4.1_global
static std::array<char const *, 2U> constexpr kSdEntryInterpretationErrorMessages{
    {"Wrong entry type", "The content of the entry id is invalid"}};

/*!
 * \brief ara::com specific exception.
 */
class SdEntryInterpretationException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding internal errors.
 */
class SdEntryInterpretationErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = SdEntryInterpretationError;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = SdEntryInterpretationException;

  /*!
   * \brief Constructor. Initializes the domain id with SomeIpBindingErrorDomain::kId.
   * \steady FALSE
   */
  constexpr SdEntryInterpretationErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SdEntryInterpretationErrorDomain() = default;

  SdEntryInterpretationErrorDomain(SdEntryInterpretationErrorDomain const &) = delete;
  SdEntryInterpretationErrorDomain(SdEntryInterpretationErrorDomain &&) = delete;
  SdEntryInterpretationErrorDomain &operator=(SdEntryInterpretationErrorDomain const &) & = delete;
  SdEntryInterpretationErrorDomain &operator=(SdEntryInterpretationErrorDomain &&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \pre          -
   * \context     Network
   * \steady      FALSE
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  StringType Name() const noexcept override { return "SdEntryInterpretation"; }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     Network
   * \steady      FALSE
   * \synchronous TRUE
   */
  StringType Message(CodeType error_code) const noexcept override {
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kSdEntryInterpretationErrorMessages.size()) {
        index = code;
      }
    }
    return kSdEntryInterpretationErrorMessages[index];
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Throw an exception based on an error code or abort.
   * \param[in] error_code Error code to cause a throw or an abort.
   * \throws SomeIpBindingException for all error codes.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre          -
   * \context     Network
   * \steady      FALSE
   * \synchronous TRUE
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const &error_code) const noexcept(false) override {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0x5D2205FC4794C7AF};
};

/*!
 * \brief Global SdEntryInterpretationErrorDomain instance.
 */
constexpr SdEntryInterpretationErrorDomain kSdEntryInterpretationErrorDomain;

/*!
 * \brief Get reference to the global SdEntryInterpretationErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     Network
 * \steady      FALSE
 * \synchronous TRUE
 */
inline constexpr ara::core::ErrorDomain const &GetSdEntryInterpretationErrorDomain() noexcept {
  return kSdEntryInterpretationErrorDomain;
}

/*!
 * \brief Creates an error code from SomeIpBindingErrorDomain.
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 * \return The constructed error code.
 * \pre          -
 * \context     ANY
 * \steady      FALSE
 * \synchronous TRUE
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(SdEntryInterpretationErrorDomain::Errc const code,
                                                    SdEntryInterpretationErrorDomain::SupportDataType const data,
                                                    char const *message) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code),
                              GetSdEntryInterpretationErrorDomain(), data, message);
}

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_ENTRY_INTERPRETATION_ERROR_H_
