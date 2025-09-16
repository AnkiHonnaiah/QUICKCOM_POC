/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ipc_error_domain.h
 *        \brief  Common error codes in context of IPC.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_ERROR_DOMAIN_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/error_domain.h"
#include "amsr/core/exception.h"
#include "vac/language/throw_or_terminate.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief    IPC error code enum.
 * \vprivate Product Private
 */
enum class IpcErrc : amsr::core::ErrorDomain::CodeType {
  /*!
   * \brief The given message is too large to process.
   */
  kMessageTooLarge = 1
};

/*!
 * \brief    Exception type for IpcErrorDomain.
 * \vprivate Product Private
 */
class IpcException final : public amsr::core::Exception {
 public:
  /*!
   * \brief    Construct a new IpcException object with a specific ErrorCode.
   * \vprivate Product Private
   */
  using Exception::Exception;
};

/*!
 * \brief    Error domain for all IPC error codes.
 * \vprivate Product Private
 */
class IpcErrorDomain final : public amsr::core::ErrorDomain {
  /*!
   * \brief Security error domain identifier.
   * \vpublic
   */
  constexpr static ErrorDomain::IdType kId{0xbc5c66eb74c2ff73};

 public:
  /*!
   * \brief    Constructor. Initializes the domain id with IpcErrorDomain::kId.
   * \vprivate Product Private
   */
  constexpr IpcErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief    Destructor.
   *
   * \details  This dtor is non-virtual (and trivial) so that this class can be a literal type. While this class has
   *           virtual functions, no polymorphic destruction is needed.
   *
   * \vprivate Product Private
   */
  ~IpcErrorDomain() noexcept = default;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  IpcErrorDomain(IpcErrorDomain const&) noexcept = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  IpcErrorDomain(IpcErrorDomain&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  IpcErrorDomain& operator=(IpcErrorDomain const&) & noexcept = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  IpcErrorDomain& operator=(IpcErrorDomain&&) & noexcept = delete;

  /*!
   * \brief    Return the name of the IPC error domain.
   * \return   "SecIpc"
   * \vprivate Product Private
   */
  char const* Name() const noexcept final { return "SecIpc"; }

  /*!
   * \brief       Return a textual representation of the given error code.
   *
   * \param[in]   error_code IPC error code
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
   *
   * \vprivate Product Private
   */
  char const* Message(ErrorDomain::CodeType error_code) const noexcept final {
    char const* message{"Unknown error code."};
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
    if (static_cast<IpcErrc>(error_code) == IpcErrc::kMessageTooLarge) {
      message = "The message is too large to be processed.";
    }
    return message;
  }

  /*!
   * \brief       Throws the given error code as IpcException.
   *
   * \details     If the is compiled without exceptions, this function call will terminate.
   *
   * \param[in]   error_code error code to be thrown.
   *
   * \throws      IpcException for all error codes.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   TRUE
   * \synchronous TRUE
   * \threadsafe  TRUE
   *
   * \vprivate Product Private
   */
  void ThrowAsException(amsr::core::ErrorCode const& error_code) const noexcept(false) final {
    vac::language::ThrowOrTerminate<IpcException>(error_code);
  }
};

namespace internal {
/*!
 * \brief Global IpcErrorDomain instance.
 */
constexpr IpcErrorDomain g_IpcErrorDomain;
}  // namespace internal

/*!
 * \brief    Returns a reference to the global IpcErrorDomain instance.
 * \return   Global IpcErrorDomain instance.
 * \vprivate Product Private
 */
inline constexpr amsr::core::ErrorDomain const& GetSecIpcErrorDomain() { return internal::g_IpcErrorDomain; }

/*!
 * \brief       Creates an ErrorCode from the IpcErrorDomain.
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
 *
 * \vprivate Product Private
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(IpcErrc code, amsr::core::ErrorDomain::SupportDataType data,
                                                     char const* message) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetSecIpcErrorDomain(), data,
                               message);
}

/*!
 * \brief       Creates an ErrorCode from the IpcErrorDomain.
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
 *
 * \vprivate Product Private
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(IpcErrc code, amsr::core::ErrorDomain::SupportDataType data) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetSecIpcErrorDomain(), data,
                               nullptr);
}

/*!
 * \brief       Creates an ErrorCode from the IpcErrorDomain.
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
 *
 * \vprivate Product Private
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(IpcErrc code, char const* message) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetSecIpcErrorDomain(), 0,
                               message);
}

/*!
 * \brief       Creates an ErrorCode from the IpcErrorDomain.
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
 *
 * \vprivate Product Private
 */
inline constexpr amsr::core::ErrorCode MakeErrorCode(IpcErrc code) {
  return amsr::core::ErrorCode(static_cast<amsr::core::ErrorDomain::CodeType>(code), GetSecIpcErrorDomain(), 0,
                               nullptr);
}

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_ERROR_DOMAIN_H_
