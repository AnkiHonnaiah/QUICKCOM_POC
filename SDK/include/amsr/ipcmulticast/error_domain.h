/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief Definition for the multicast specific error domain.
 *         \unit amsr::ipcmulticast::common::IpcMulticastErrorDomain
 *
 *********************************************************************************************************************/
#ifndef LIB_IPCMULTICAST_COMMON_INCLUDE_AMSR_IPCMULTICAST_ERROR_DOMAIN_H_
#define LIB_IPCMULTICAST_COMMON_INCLUDE_AMSR_IPCMULTICAST_ERROR_DOMAIN_H_

#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace ipcmulticast {

/*!
 * \brief IpcMulticast internal error codes.
 */
enum class IpcMulticastErrc : amsr::core::ErrorDomain::CodeType {
  kErrorUnknown = 0,        /*!< Default error code. */
  kInvalidApiState,         /*!< API call is not allowed in the current state. */
  kInternalConnectionError, /*!< Internal connection error. */
  kInvalidArgument,         /*!< Argument is invalid. */
  kBusy,                    /*!< Service is temporary unavailable. */
};

/*!
 * \brief Exception type for IpcMulticastErrorDomain.
 */
class IpcMulticastException final : public ara::core::Exception {
 public:
  /*! Inherit constructor. */
  using Exception::Exception;
};

/*!
 * \brief Error domain for ipc multicast errors.
 */
class IpcMulticastErrorDomain final : public amsr::core::ErrorDomain {
 public:
  /*! Error code enum of the domain. */
  using Errc = IpcMulticastErrc;
  /*! Exception type of the domain. */
  using Exception = IpcMulticastException;

  /*!
   * \brief Constructor. Initializes the domain id with CommonErrorDomain::kId.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  constexpr IpcMulticastErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief       Default destructor.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  ~IpcMulticastErrorDomain() noexcept = default;

  IpcMulticastErrorDomain(IpcMulticastErrorDomain const&) = delete;
  IpcMulticastErrorDomain(IpcMulticastErrorDomain&&) = delete;
  auto operator=(IpcMulticastErrorDomain const&) & -> IpcMulticastErrorDomain& = delete;
  auto operator=(IpcMulticastErrorDomain&&) & -> IpcMulticastErrorDomain& = delete;

  /*!
   * \brief       Return the name for this error domain.
   *
   * \return      "IpcMulticast".
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto Name() const noexcept -> StringType final { return "IpcMulticast"; }

  /*!
   * \brief       Return the textual description for the given error code.
   *
   * \param[in]   error_code   The domain-specific error code.
   *
   * \return      he error message text as a null-terminated string, never nullptr.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  auto Message(ErrorDomain::CodeType error_code) const noexcept -> StringType final;

  /*!
   * \brief       Throws the given errorCode as Exception.
   *
   * \param[in]   error_code   Error code to be thrown.
   *
   * \throws      IpcMulticastException for all error codes.
   *
   * \remark      If the is compiled without exceptions, this function call will terminate.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   */
  [[noreturn]] void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<Exception>(error_code);
  }

 private:
  /*! ID of the error domain. */
  static constexpr ErrorDomain::IdType kId{0x4aab73efb90d1256};
};

namespace internal {
/*! Global ErrorDomain instance. */
constexpr IpcMulticastErrorDomain kIpcMulticastErrorDomain;
}  // namespace internal

/*!
 * \brief       Gets the reference to the global IpcMulticastErrorDomain instance.
 *
 * \return      IpcMulticast error domain instance.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 */
inline constexpr auto GetIpcMulticastErrorDomain() noexcept -> amsr::core::ErrorDomain const& {
  return internal::kIpcMulticastErrorDomain;
}

/*!
 * \brief       Creates an ErrorCode from the IpcMulticastErrorDomain.
 *
 * \param[in]   code      The specific error code value.
 * \param[in]   data      The vendor defined support data.
 * \param[in]   message   An optional user message for this error.
 *
 * \return      The created error code.
 * \remark      The lifetime of the message should exceed the lifetime of the ErrorCode. Otherwise
 *              ErrorCode::UserMessage may return dangling reference.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 */
inline constexpr auto MakeErrorCode(IpcMulticastErrorDomain::Errc code, amsr::core::ErrorDomain::SupportDataType data,
                                    IpcMulticastErrorDomain::StringType message) noexcept -> amsr::core::ErrorCode {
  return {static_cast<amsr::core::ErrorDomain::CodeType>(code), GetIpcMulticastErrorDomain(), data, message};
}

/*!
 * \brief       Creates an ErrorCode from the IpcMulticastErrorDomain.
 *
 * \param[in]   code      The specific error code value.
 * \param[in]   message   An optional user message for this error.
 *
 * \return      The created error code.
 * \remark      The lifetime of the message should exceed the lifetime of the ErrorCode. Otherwise
 *              ErrorCode::UserMessage may return dangling reference.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 */
inline constexpr auto MakeErrorCode(IpcMulticastErrorDomain::Errc code, IpcMulticastErrorDomain::StringType message)
    -> amsr::core::ErrorCode {
  return MakeErrorCode(code, 0, message);
}

/*!
 * \brief           Constructs a error with IpcMulticastErrc code and another error's support data and user message.
 *
 * \param[in]       old_error          ErrorCode whose support data and user message shall be used.
 * \param[in]       new_error_code     Error enum value that the new error code shall have.
 *
 * \return          Error code with replaced error enum.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
inline auto ReplaceErrorCode(amsr::core::ErrorCode old_error, IpcMulticastErrc new_error_code) noexcept
    -> amsr::core::ErrorCode {
  // VCA_IPCMULTICAST_CALLING_METHOD_ON_COPY_BY_VALUE_PARAMETER
  return MakeErrorCode(new_error_code, old_error.SupportData(), old_error.UserMessage().data());
}

}  // namespace ipcmulticast
}  // namespace amsr

#endif  // LIB_IPCMULTICAST_COMMON_INCLUDE_AMSR_IPCMULTICAST_ERROR_DOMAIN_H_
