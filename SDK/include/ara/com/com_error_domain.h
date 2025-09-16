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
/*!        \file  com_error_domain.h
 *        \brief  Definition for ComErrc, ComException and ComErrorDomain.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_COM_ERROR_DOMAIN_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_COM_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara {
namespace com {

/*!
 * \brief Ara::com specific error codes.
 * \vpublic
 *
 * \trace SPEC-8053547
 */
enum class ComErrc : ::ara::core::ErrorDomain::CodeType {
  /*!
   * \brief Default error code.
   */
  kErrorNotOk = 0U,

  /*!
   * \brief The requested service is not available.
   */
  kServiceNotAvailable = 1U,

  /*!
   * \brief Application holds more SamplePtrs than committed in Subscribe().
   */
  kMaxSamplesReached = 2U,

  /*!
   * \brief The network binding failed.
   */
  kNetworkBindingFailure = 3U,

  /*!
   * \brief Request was refused by Grant enforcement layer.
   */
  kGrantEnforcementError = 4U,

  /*!
   * \brief Failure detected when unregistering a handler.
   */
  kUnsetHandlerFailure = 8U,

  /*!
   * \brief Allocation of memory for event sample has failed.
   */
  kSampleAllocationFailure = 9U,

  /*!
   * \brief Service not offered.
   */
  kServiceNotOffered = 11,

  /*!
   * \brief Communication link is broken.
   */
  kCommunicationLinkError = 12U,

  /*!
   * \brief Communication Stack Error.
   */
  kCommunicationStackError = 14U,

  /*!
   * \brief Provided maxSampleCount not realizable.
   */
  kMaxSampleCountNotRealizable = 16U,

  /*!
   * \brief Failed to register handler.
   */
  kSetHandlerFailure = 21U,

  // ------ internal -------

  /*!
   * \brief A user provided instance identifier is not known.
   */
  kUnknownInstanceIdentifier = 22U,

  /*!
   * \brief A user provided instance specifier is not known.
   */
  kUnknownInstanceSpecifier = 23U,

  /*!
   * \brief JSON configuration parsing failure.
   */
  kJsonParsingFailed = 24U,

  /*!
   * \brief Method arguments cannot be retrieved for a skeleton method call.
   */
  kMethodArgumentsNotRetrieved = 25U,

  /*!
   * \brief Initialization/Deinitialization is performed in wrong order.
   */
  kWrongInitSequence = 26U,

  /*!
   * \brief Allocate used while multiple provided service instances are registered.
   */
  kAllocateUnsupportedWithSetup = 27U,

  /*!
   * \brief Invalid API call.
   */
  kInvalidAPICall = 28U,

  /*!
   * \brief Incoming method request threshold exceeds the thread pool queue size.
   */
  kExceedQueueSize = 29U
};

/*!
 * \brief ara::com specific exception.
 * \vpublic
 */
class ComException : public ::ara::core::Exception {
 public:
  /*!
   * \brief Inherit constructor.
   * \vpublic
   */
  using Exception::Exception;
};

/*!
 * \brief Error Domain for all ara::com specific errors.
 * \vpublic
 *
 * \trace SPEC-8053545, SPEC-8053546
 * \unit  Socal::ComErrorDomain
 */
class ComErrorDomain final : public ::ara::core::ErrorDomain {
 public:
  /*!
   * \brief Error code enum of the domain.
   * \vpublic
   */
  using Errc = ComErrc;
  /*!
   * \brief Exception type of the domain.
   * \vpublic
   */
  using Exception = ComException;

  /*!
   * \brief Constructor. Initializes the domain id with ComErrorDomain::kId.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  constexpr ComErrorDomain() noexcept : ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  StringType Name() const noexcept final;

  /*!
   * \brief   Return a textual representation of the given error code.
   * \details The return value is undefined if the \a errorCode did not originate from this error domain.
   * \param[in] error_code  The domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  StringType Message(CodeType error_code) const noexcept final;

  /*!
   * \brief   Throws the given errorCode as Exception.
   * \details If the adaptive application is compiled without exceptions, this function call will terminate.
   * \param[in] error_code  Error code to be thrown.
   * \throws ComException  For all error codes.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, FALSE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  [[noreturn]] void ThrowAsException(::ara::core::ErrorCode const& error_code) const noexcept(false) final;

 private:
  /*!
   * \brief ID of the domain.
   */
  static constexpr ErrorDomain::IdType kId{0x0603201515092018U};
};

namespace internal {
/*!
 * \brief Global ComErrorDomain instance.
 */
constexpr ComErrorDomain kComErrorDomain;

/*!
 * \brief ComErrorDomain a reference to the global ComErrorDomain instance.
 * \return CommonErrorDomain instance.
 * \pre     -
 * \context App
 * \steady TRUE
 * \spec requires true; \endspec
 */
inline constexpr ::ara::core::ErrorDomain const& GetComDomain() noexcept { return internal::kComErrorDomain; }

}  // namespace internal

/*!
 * \brief Creates an error code from ComErrorDomain.
 * \param[in] code     The specific error code.
 * \param[in] data     Vendor defined support data.
 * \param[in] message  An optional message for this error.
 * \return The constructed error code.
 * \pre      -
 * \context  App
 * \vprivate
 * \steady TRUE
 * \spec requires true; \endspec
 */
inline constexpr ::ara::core::ErrorCode MakeErrorCode(ComErrorDomain::Errc code, ComErrorDomain::SupportDataType data,
                                                      char const* message) noexcept {
  return ::ara::core::ErrorCode(static_cast<::ara::core::ErrorDomain::CodeType>(code), internal::GetComDomain(), data,
                                message);
}

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_COM_ERROR_DOMAIN_H_
