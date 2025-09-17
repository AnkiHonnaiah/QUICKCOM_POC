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
/**        \file  amsr/someip_binding_core/internal/error_domain.h
 *        \brief  Definition for SOME/IP Binding specific error domain.
 *        \unit  SomeIpBinding::SomeIpBindingCore::ErrorDomain
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_ERROR_DOMAIN_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "ara/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief SOME/IP binding internal error codes.
 */
enum class SomeIpBindingErrc : amsr::core::ErrorDomain::CodeType {
  /*!
   * \brief default error code
   */
  error_not_ok = 0U,

  /*!
   * \brief Malformed InstanceIdentifierString.
   */
  malformed_instance_identifier_string = 1U,

  /*!
   * \brief JSON configuration parsing failure.
   */
  json_parsing_failure = 2U,

  /*!
   * \brief JSON configuration file not found or loadable.
   */
  json_loading_failure = 3U,

  /*!
   * \brief JSON configuration is invalid or incomplete.
   */
  invalid_json_config = 4U,

  /*!
   * \brief Initialization/Deinitialization is performed in wrong order.
   */
  wrong_init_sequence = 5U,

  /*!
   * \brief JSON configuration key not found parsing failure.
   */
  json_parsing_key_not_found_failure = 6U,

  /*!
   * \brief JSON configuration unexpected event parsing failure.
   */
  json_parsing_unexpected_event_failure = 7U,

  /*!
   * \brief JSON configuration unexpected enumeration value parsing failure.
   */
  json_parsing_unsupported_runtime_processing_mode_failure = 8U,

  /*!
   * \brief JSON configuration number parsing failure.
   */
  json_parsing_number_parsing_failure = 9U,

  /*!
   * \brief JSON configuration string parsing failure.
   */
  json_parsing_string_parsing_failure = 10U,

  /*!
   * \brief JSON configuration duplicate e2e profile shortname in e2e profile list failure.
   */
  json_parsing_duplicate_e2e_profile_shortname_in_list_failure = 11U,

  /*!
   * \brief JSON configuration duplicate or missing shortname failure.
   */
  json_parsing_duplicate_or_missing_shortname_failure = 12U,

  /*!
   * \brief JSON configuration duplicate or missing profile_name failure.
   */
  json_parsing_duplicate_or_missing_profile_name_failure = 13U,

  /*!
   * \brief JSON configuration duplicate or missing max_delta_counter failure.
   */
  json_parsing_duplicate_or_missing_max_delta_counter_failure = 14U,

  /*!
   * \brief JSON configuration duplicate or missing min_ok_state_init failure.
   */
  json_parsing_duplicate_or_missing_min_ok_state_init_failure = 15U,

  /*!
   * \brief JSON configuration duplicate or missing max_error_state_init failure.
   */
  json_parsing_duplicate_or_missing_max_error_state_init_failure = 16U,

  /*!
   * \brief JSON configuration duplicate or missing min_ok_state_valid failure.
   */
  json_parsing_duplicate_or_missing_min_ok_state_valid_failure = 17U,

  /*!
   * \brief JSON configuration duplicate or missing max_error_state_valid failure.
   */
  json_parsing_duplicate_or_missing_max_error_state_valid_failure = 18U,

  /*!
   * \brief JSON configuration duplicate or missing min_ok_state_invalid failure.
   */
  json_parsing_duplicate_or_missing_min_ok_state_invalid_failure = 19U,

  /*!
   * \brief JSON configuration duplicate or missing max_error_state_invalid failure.
   */
  json_parsing_duplicate_or_missing_max_error_state_invalid_failure = 20U,

  /*!
   * \brief JSON configuration duplicate or missing window_size_valid failure.
   */
  json_parsing_duplicate_or_missing_window_size_valid_failure = 21U,

  /*!
   * \brief JSON configuration duplicate or missing window_size_init failure.
   */
  json_parsing_duplicate_or_missing_window_size_init_failure = 22U,

  /*!
   * \brief JSON configuration duplicate or missing window_size_invalid failure.
   */
  json_parsing_duplicate_or_missing_window_size_invalid_failure = 23U,

  /*!
   * \brief JSON configuration duplicate  clear_from_valid_to_invalid failure.
   */
  json_parsing_duplicate_clear_from_valid_to_invalid_failure = 24U,

  /*!
   * \brief JSON configuration duplicate transit_to_invalid_extended failure.
   */
  json_parsing_duplicate_transit_to_invalid_extended_failure = 25U,

  /*!
   * \brief JSON configuration unsupported E2E profile configuration failure.
   */
  json_parsing_unsupported_e2e_profile_configuration_failure = 26U,

  /*!
   * \brief JSON configuration e2e profile shortname not found failure.
   */
  json_parsing_e2e_profile_shortname_not_found_failure = 27U
};

/*!
 * \brief ara::com specific exception.
 */
class SomeIpBindingException : public ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all SOME/IP Binding internal errors.
 */
class SomeIpBindingErrorDomain final : public amsr::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   */
  using Errc = SomeIpBindingErrc;
  /*!
   * \brief Exception type of the domain.
   */
  using Exception = SomeIpBindingException;

  /*!
   * \brief Constructor. Initializes the domain id with SomeIpBindingErrorDomain::kId.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr SomeIpBindingErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Name() const noexcept final { return "SomeIpBinding"; }

  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  StringType Message(CodeType error_code) const noexcept final {
    static std::array<char const*, 28U> constexpr kMessages{{
        "Unknown Error Code",                                           // error_not_ok
        "Malformed InstanceIdentifierString",                           // malformed_instance_identifier_string
        "JSON parsing failure",                                         // json_parsing_failure
        "Component JSON configuration file not found or loadable",      // json_loading_failure
        "Component JSON configuration is invalid or incomplete",        // invalid_json_config
        "Initialization/Deinitialization is performed in wrong order",  // wrong_init_sequence
        "JSON configuration key not found parsing failure",             // json_parsing_key_not_found_failure
        "JSON configuration unexpected event parsing failure",          // json_parsing_unexpected_event_failure
        "JSON configuration unexpected enumeration value parsing failure",  // json_parsing_unsupported_runtime_processing_mode_failure
        "JSON configuration number parsing failure",                        // json_parsing_number_parsing_failure
        "JSON configuration string parsing failure",                        // json_parsing_string_parsing_failure
        "JSON configuration duplicate e2e profile shortname in e2e profile list failure",  // json_parsing_duplicate_e2e_profile_shortname_in_list_failure
        "JSON configuration duplicate or missing shortname failure",  // json_parsing_duplicate_or_missing_shortname_failure
        "JSON configuration duplicate or missing profile_name failure",  // json_parsing_duplicate_or_missing_profile_name_failure
        "JSON configuration duplicate or missing max_delta_counter failure",  // json_parsing_duplicate_or_missing_max_delta_counter_failure
        "JSON configuration duplicate or missing min_ok_state_init failure",  // json_parsing_duplicate_or_missing_min_ok_state_init_failure
        "JSON configuration duplicate or missing max_error_state_init failure",  // json_parsing_duplicate_or_missing_max_error_state_init_failure
        "JSON configuration duplicate or missing min_ok_state_invalid failure",  // json_parsing_duplicate_or_missing_min_ok_state_valid_failure
        "JSON configuration duplicate or missing max_error_state_valid failure",  // json_parsing_duplicate_or_missing_max_error_state_valid_failure
        "JSON configuration duplicate or missing min_ok_state_invalid failure",  // json_parsing_duplicate_or_missing_min_ok_state_invalid_failure
        "JSON configuration duplicate or missing max_error_state_invalid failure",  // json_parsing_duplicate_or_missing_max_error_state_invalid_failure
        "JSON configuration duplicate or missing window_size_valid failure",  // json_parsing_duplicate_or_missing_window_size_valid_failure
        "JSON configuration duplicate or missing window_size_init failure",  // json_parsing_duplicate_or_missing_window_size_init_failure
        "JSON configuration duplicate or missing window_size_invalid failure",  // json_parsing_duplicate_or_missing_window_size_invalid_failure
        "JSON configuration duplicate clear_from_valid_to_invalid failure",  // json_parsing_duplicate_clear_from_valid_to_invalid_failure
        "JSON configuration duplicate transit_to_invalid_extended failure",  // json_parsing_duplicate_transit_to_invalid_extended_failure
        "JSON configuration unsupported E2E profile configuration failure",  // json_parsing_unsupported_e2e_profile_configuration_failure
        "JSON configuration e2e profile shortname not found failure"  // json_parsing_e2e_profile_shortname_not_found_failure
    }};

    static_assert(std::is_trivially_destructible<decltype(kMessages)>::value,
                  "Destructor of object with static storage duration must be trivial!");
    static_assert(std::numeric_limits<std::size_t>::max() >=
                      static_cast<std::size_t>(std::numeric_limits<ErrorDomain::CodeType>::max()),
                  "ErrorDomain::CodeType must not exceed std::size_t.");

    std::size_t index{0U};
    if (error_code > 0) {
      std::size_t const code{static_cast<std::size_t>(error_code)};
      if (code < kMessages.size()) {
        index = code;
      }
    }
    return kMessages[index];
  }

  /*!
   * \brief Throws the given errorCode as Exception.
   * \param[in] error_code error code to be thrown.
   * \throws SomeIpBindingException for all error codes.
   * \remark if the adaptive application is compiled without exceptions, this function call will terminate.
   * \pre          -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0x5D2205FC4794C7AE};
};

/*!
 * \brief Global SomeBindingErrorDomain instance.
 */
constexpr SomeIpBindingErrorDomain kSomeBindingErrorDomain;

/*!
 * \brief Get reference to the global SomeBindingErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre          -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr amsr::core::ErrorDomain const& GetSomeIpBindingErrorDomain() noexcept {
  return kSomeBindingErrorDomain;
}

/*!
 * \brief Creates an error code from SomeIpBindingErrorDomain.
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 * \return The constructed error code.
 * \pre          -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \spec
 *   requires true;
 * \endspec
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(SomeIpBindingErrorDomain::Errc const code,
                                                     SomeIpBindingErrorDomain::SupportDataType const data,
                                                     char const* message) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetSomeIpBindingErrorDomain(),
                               data, message);
}

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_ERROR_DOMAIN_H_
