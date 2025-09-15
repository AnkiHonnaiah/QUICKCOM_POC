// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ara/crypto/common/exceptions.h
 *        \brief  Interfaces for generic exceptions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_EXCEPTIONS_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_EXCEPTIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "ara/crypto/common/internal/base_id_types_without_uuid.h"
#include "ara/crypto/common/std_api.h"
#include "vac/language/throw_or_terminate.h"

namespace ara {
namespace crypto {
/*!
 * \brief Enumeration of all Security Error Code values that may be reported by ara::crypto.
 * \details Storage type: 32 bit signed integer (\see ara::core::ErrorDomain::CodeType).
 * \vpublic
 */
enum class SecurityErrc : ara::core::ErrorDomain::CodeType {
  kNoError = 0,  // No error

  ErrorClass = 0x1000000,    // Reserved (a multiplier of error class IDs)
  ErrorSubClass = 0x10000,   // Reserved (a multiplier of error sub-class IDs)
  ErrorSubSubClass = 0x100,  // Reserved (a multiplier of error sub-sub-class IDs)

  kResourceFault = 1 * ErrorClass,                   // Generic resource fault
  kBusyResource = kResourceFault + 1,                // Specified resource is busy
  kInsufficientResource = kResourceFault + 2,        // Insufficient capacity of specified resource
  kUnreservedResource = kResourceFault + 3,          // Specified resource was not reserved
  kBadAlloc = kResourceFault + (1 * ErrorSubClass),  // Cannot allocate requested resources

  kLogicFault = 2 * ErrorClass,                          // Generic logic fault
  kInvalidArgument = kLogicFault + (1 * ErrorSubClass),  // An invalid argument value is provided
  kUnknownIdentifier = kInvalidArgument + 1,             // Unknown identifier is provided
  kInsufficientCapacity = kInvalidArgument + 2,          // Insufficient capacity of the output buffer
  kInvalidInputSize = kInvalidArgument + 3,              // Invalid size of an input buffer
  kIncompatibleArguments = kInvalidArgument + 4,         // Provided values of arguments are incompatible
  kInOutBuffersIntersect = kInvalidArgument + 5,         // Input and output buffers are intersect
  kBelowBoundary = kInvalidArgument + 6,                 // Provided value is below the lower boundary
  kAboveBoundary = kInvalidArgument + 7,                 // Provided value is above the upper boundary

  kUnsupported =
      kInvalidArgument + (1 * ErrorSubSubClass),  // Unsupported request (due to limitations of the implementation)
  kInvalidUsageOrder = kLogicFault + (2 * ErrorSubClass),  // Invalid usage order of the interface
  kUninitializedContext = kInvalidUsageOrder + 1,          // Context of the interface was not initialized
  kProcessingNotStarted = kInvalidUsageOrder + 2,          // Data processing was not started yet
  kProcessingNotFinished = kInvalidUsageOrder + 3,         // Data processing was not finished yet

  kRuntimeFault = 3 * ErrorClass,            // Generic runtime fault
  kUnsupportedFormat = kRuntimeFault + 1,    // Unsupported serialization format for this object type
  kBruteForceRisk = kRuntimeFault + 2,       // Operation is prohibited due to a risk of a brute force attack
  kContentRestrictions = kRuntimeFault + 3,  // The operation violates content restrictions of the target container
  kBadObjectReference = kRuntimeFault + 4,   // Incorrect reference between objects
  kLockedByReference =
      kRuntimeFault + 5,  // An object stored in the container is locked due to a reference from another one
  kContentDuplication = kRuntimeFault + 6,  // Provided content already exists in the target storage
  kTimeout = kRuntimeFault + 7,             // An operation did not complete in the admissible time span

  kUnexpectedValue = kRuntimeFault + (1 * ErrorSubClass),      // Unexpected value of an argument is provided
  kIncompatibleObject = kUnexpectedValue + 1,                  // The provided object is incompatible with
                                                               // requested operation or its configuration
  kIncompleteArgState = kUnexpectedValue + 2,                  // Incomplete state of an argument
  kEmptyContainer = kUnexpectedValue + 3,                      // Specified container is empty
  kBadObjectType = kUnexpectedValue + (1 * ErrorSubSubClass),  // Provided object has unexpected type
  kUsageViolation = kRuntimeFault + (2 * ErrorSubClass),       // Violation of allowed usage for the object
  kAccessViolation = kRuntimeFault + (3 * ErrorSubClass),      // Access rights violation

  // Attention! Deviation: The following error codes are not part of the AUTOSAR specification.
  // Misc
  kVariableKeyLength = kInvalidArgument + 8,  // The key length for the provided key is variable

  // RPC error codes
  kRpcError = 9 * ErrorClass,                    // Generic RPC Error
  kRpcRuntimeFault = kRpcError + 1,              // Generic runtime fault in the RPC framework
  kRpcBadObjectReference = kRpcError + 2,        // Incorrect reference between objects in the RPC framework
  kRpcUnexpectedValue = kRpcError + 3,           // Unexpected value of an argument is provided in the RPC framework
  kRpcUsageViolation = kRpcError + 4,            // Violation of allowed usage for the object in the RPC framework
  kRpcInvalidArgument = kRpcError + 5,           // An invalid argument value is provided in the RPC framework
  kRpcInsufficientCapacity = kRpcError + 6,      // Insufficient capacity of the output buffer in the RPC framework
  kRpcInvalidInputSize = kRpcError + 7,          // Invalid size of an input buffer in the RPC framework
  kRpcSerializationError = kRpcError + 8,        // Error during message serialization in the RPC framework
  kRpcDeserializationError = kRpcError + 9,      // Error during message deserialization in the RPC framework
  kRpcUnknownTask = kRpcError + 10,              // The message contained a TaskId which is not recognized
  kRpcEmptyMessage = kRpcError + 11,             // The message was empty
  kRpcUnknownObjectIdentifier = kRpcError + 12,  // No object found for given object identifier

  kRpcClientError = kRpcError + ErrorSubClass,
  kRpcClientSerializationError =
      kRpcClientError + 1,  // Error during message serialization in the client's RPC framework
  kRpcClientDeserializationError =
      kRpcClientError + 2,  // Error during message deserialization in the client's RPC framework
  kRpcClientCommunicationError = kRpcClientError + 3,  // Error during communication with server
  kRpcConnectionTimeout = kRpcClientError + 4,         // The connection timed out

  kRpcServerError = kRpcError + (2 * ErrorSubClass),
  kRpcServerSerializationError =
      kRpcServerError + 1,  // Error during message serialization in the server's RPC framework
  kRpcServerDeserializationError =
      kRpcServerError + 2,  // Error during message deserialization in the server's RPC framework
  kRpcServerSkeletonCreationFailed = kRpcServerError + 3,  // Could not create Skeleton on the server
};

/*!
 * \brief Extract the "Exception Class" identifier part from the Error Code value.
 * \param[in] code error code value for parsing.
 * \return "Security Exception Class" identifier.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vpublic
 */
constexpr inline SecurityErrc GetErrorClass(SecurityErrc code) noexcept {
  // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
  return static_cast<SecurityErrc>(static_cast<std::uint32_t>(code) & 0xffffff00LU);
}

/*!
 * \brief Forward declaration of the later defined SecurityErrorDomain.
 * \vpublic
 */
class SecurityErrorDomain;

/*!
 * \brief An interface of a Security exception.
 * \vpublic
 */
class SecurityException : public ara::core::Exception {
 public:
  /*!
   * \brief Constructs a SecurityException.
   * \vpublic
   */
  using ara::core::Exception::Exception;
};

/*!
 * \brief An interface of a Resource fault exception.
 * \vpublic
 */
class ResourceException : public SecurityException {
 public:
  /*!
   * \brief Constructs a ResourceException.
   * \vpublic
   */
  using SecurityException::SecurityException;
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief An interface of the Bad Allocation exception.
 * \vpublic
 */
class BadAllocException : public ResourceException {
 public:
  /*!
   * \brief Constructs a BadAllocException.
   * \vpublic
   */
  using ResourceException::ResourceException;
};

/*!
 * \brief An interface of a general Security Logic Error exception.
 * \details The CryptoStack should throw this exception if the incorrectness of the API call must be obvious for the
 *          consumer code even before the call execution.
 * \vpublic
 */
class LogicException : public SecurityException {
 public:
  /*!
   * \brief Constructs a LogicException.
   * \vpublic
   */
  using SecurityException::SecurityException;
};

/*!
 * \brief An interface of the Invalid Argument exception.
 * \details The CryptoStack should throw this exception if a consumer code passes to a method some invalid arguments,
 *          and their incorrectness can be detected at compile time.
 * \vpublic
 */
class InvalidArgumentException : public SecurityException {
 public:
  /*!
   * \brief Constructs a InvalidArgumentException.
   * \vpublic
   */
  using SecurityException::SecurityException;

  /*!
   * \brief Get index of the Invalid Argument.
   * \return 1-based index of the invalid argument (0 is reserved for implicit this pointer)
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  std::uint8_t GetBadArgumentIndex() const noexcept {
    return static_cast<std::uint8_t>(static_cast<std::uint8_t>(Error().SupportData()) & 0xffU);
  }
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief An interface of the Crypto Unsupported method/argument exception.
 * \details A Crypto Provider may have partial support of some specific algorithms or transformations and don't
 *          implement support of specific use-cases, some optional arguments or even supplementary methods. But
 *          all such restrictions should be carefully documented (in the Crypto Provider's manual) and brought to
 *          the developer attention! In a case when an application calls such unsupported API the Crypto Provider
 *          must throw this exception.
 * \vpublic
 */
class UnsupportedException : public InvalidArgumentException {
 public:
  /*!
   * \brief Constructs a UnsupportedException.
   * \vpublic
   */
  using InvalidArgumentException::InvalidArgumentException;
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief An interface of a general Security Logic Error exception.
 * \details The CryptoStack should throw this exception if the incorrectness of an API call can be detected
 *          at compile time.
 * \vpublic
 */
class InvalidUsageOrderException : public LogicException {
 public:
  /*!
   * \brief Constructs a InvalidUsageOrderException.
   * \vpublic
   */
  using LogicException::LogicException;
};

/*!
 * \brief An interface of a general Security Runtime Error exception.
 * \details The CryptoStack should throw this exception if the incorrectness of an API call can be detected
 *          at runtime only.
 * \vpublic
 */
class RuntimeException : public SecurityException {
 public:
  /*!
   * \brief Constructs a RuntimeException.
   * \vpublic
   */
  using SecurityException::SecurityException;
};

/*!
 * \brief An interface of the Unexpected Value exception.
 * \details The CryptoStack should throw this exception if a consumer code pass to a method some non-expected
 *          values, but their incorrectness can be detected at runtime only.
 * \vpublic
 */
class UnexpectedValueException : public RuntimeException {
 public:
  /*!
   * \brief Constructs a UnexpectedValueException.
   * \vpublic
   */
  using RuntimeException::RuntimeException;

  /*!
   * \brief Get index of the argument with the Unexpected Value.
   * \return 1-based index of the Unexpected Value argument (0 is reserved for implicit this pointer)
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  std::uint8_t GetBadArgumentIndex() const noexcept {
    return static_cast<std::uint8_t>(static_cast<std::uint8_t>(Error().SupportData()) & 0xffU);
  }
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Class of the Bad Crypto Object Cast exceptions.
 * \details A method must throw this exception when application needs (expects) to get one type of a crypto object,
 *          but actually another type is delivered by a method call.
 * \vpublic
 */
class BadObjectTypeException : public UnexpectedValueException {
 public:
  /*!
   * \brief Constructs a BadObjectTypeException.
   * \vpublic
   */
  using UnexpectedValueException::UnexpectedValueException;

  /*!
   * \brief Get the needed/expected object type in an operation throwed up this exception.
   * \return Crypto Object type expected in the operation reported this error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  CryptoObjectType GetNeededType() const noexcept {
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
    // VECTOR NC AutosarC++17_10-A4.7.1:MD_CRYPTO_AutosarC++17_10-A4.7.1_cast_may_truncate_value
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_cast_may_truncate_value
    return static_cast<CryptoObjectType>(static_cast<std::uint16_t>(Error().SupportData()) & 0xffU);
  }

  /*!
   * \brief Get the actual object type in the operation throwed up this exception.
   * \return Actual Crypto Object type in the operation reported this error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  CryptoObjectType GetActualType() const noexcept {
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
    // VECTOR NC AutosarC++17_10-A4.7.1:MD_CRYPTO_AutosarC++17_10-A4.7.1_cast_may_truncate_value
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_CRYPTO_AutosarC++17_10-M0.3.1_cast_may_truncate_value
    return static_cast<CryptoObjectType>((static_cast<std::uint32_t>(Error().SupportData()) >> 16U) & 0xffU);
  }
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief AllowedUsageFlags type definition.
 * \vpublic
 */
using AllowedUsageFlags = internal::AllowedUsageFlags;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief An interface of the Cryptography Usage Violation exceptions.
 * \details A Crypto Provider must throw this exception when application tries to violate the usage restrictions
 *          assigned to a Crypto Object. For more details see \c AllowedUsageFlags and related constants.
 * \vpublic
 */
class UsageViolationException : public RuntimeException {
 public:
  /*!
   * \brief Constructs a UsageViolationException.
   * \vpublic
   */
  using RuntimeException::RuntimeException;

  /*!
   * \brief Get actual "allowed usage flags" of the object (provided as an argument to the call) granted to this actor
   *        (application/process).
   * \return Actually allowed usage flags of the target object
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  AllowedUsageFlags GetAllowedUsage() const noexcept { return static_cast<AllowedUsageFlags>(Error().SupportData()); }
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Security Access Violation exception class.
 * \details The Key Storage Provider must throw this exception when an application tries violate access rights
 *          assigned to a key slot.
 * \vpublic
 */
class AccessViolationException : public RuntimeException {
 public:
  /*!
   * \brief Constructs a AccessViolationException.
   * \vpublic
   */
  using RuntimeException::RuntimeException;
};

/*!
 * \brief Security Error Domain class.
 * \details This class represents an error domain responsible for all errors/exceptions that may be reported by
 *          public APIs in the ara::crypto namespace.
 * \vpublic
 */
class SecurityErrorDomain final : public ara::core::ErrorDomain {
 public:
  /*!
   * \brief Security error domain identifier.
   * \vpublic
   */
  constexpr static ara::core::ErrorDomain::IdType kId{0x8000000000000080ull};

  /*!
   * \brief Error code type definition.
   * \vpublic
   */
  using Errc = SecurityErrc;

  // VECTOR NC AutosarC++17_10-M0.1.5: MD_CRYPTO_AutosarC++17_10-M0.1.5_unused_types_in_standard
  /*!
   * \brief Exception type definition.
   * \vpublic
   */
  using Exception = SecurityException;

  /*!
   * \brief Constructs a SecurityErrorDomain.
   * \vpublic
   */
  constexpr SecurityErrorDomain() noexcept : ara::core::ErrorDomain(kId) {}

  /*!
   * \brief Return the name of this error domain.
   * \return the name as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  char const* Name() const noexcept final { return "Security"; }

  // VECTOR NC Metric-McCabe_Complexity: MD_CRYPTO_Metric-McCabe_Complexity
  // VECTOR NC Metric-HIS.VG: MD_CRYPTO_Metric-HIS.VG
  /*!
   * \brief Return a textual representation of the given error code.
   * \details The return value is undefined if the \a error_code did not originate from this error domain.
   * \param[in] error_code the domain-specific error code.
   * \return the text as a null-terminated string, never nullptr.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  char const* Message(ara::core::ErrorDomain::CodeType error_code) const noexcept final {
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
    Errc const code{static_cast<Errc>(error_code)};
    char const* ret{};
    switch (code) {
      case Errc::kResourceFault:
        ret = "Generic resource fault.";
        break;
      case Errc::kBusyResource:
        ret = "Specified resource is busy.";
        break;
      case Errc::kInsufficientResource:
        ret = "Insufficient capacity of specified resource.";
        break;
      case Errc::kUnreservedResource:
        ret = "Specified resource was not reserved.";
        break;
      case Errc::kBadAlloc:
        ret = "Cannot allocate requested resources.";
        break;
      case Errc::kLogicFault:
        ret = "Generic logic fault.";
        break;
      case Errc::kInvalidArgument:
        ret = "An invalid argument value is provided.";
        break;
      case Errc::kUnknownIdentifier:
        ret = "Unknown identifier is provided.";
        break;
      case Errc::kInsufficientCapacity:
        ret = "Insufficient capacity of the output buffer.";
        break;
      case Errc::kInvalidInputSize:
        ret = "Invalid size of an input buffer.";
        break;
      case Errc::kIncompatibleArguments:
        ret = "Provided values of arguments are incompatible.";
        break;
      case Errc::kInOutBuffersIntersect:
        ret = "Input and output buffers are intersect.";
        break;
      case Errc::kBelowBoundary:
        ret = "Provided value is below the lower boundary.";
        break;
      case Errc::kAboveBoundary:
        ret = "Provided value is above the upper boundary.";
        break;
      case Errc::kUnsupported:
        ret = "Unsupported request (due to limitations of the implementation).";
        break;
      case Errc::kInvalidUsageOrder:
        ret = "Invalid usage order of the interface.";
        break;
      case Errc::kUninitializedContext:
        ret = "Context of the interface was not initialized.";
        break;
      case Errc::kProcessingNotStarted:
        ret = "Data processing was not started yet.";
        break;
      case Errc::kProcessingNotFinished:
        ret = "Data processing was not finished yet.";
        break;
      case Errc::kRuntimeFault:
        ret = "Generic runtime fault.";
        break;
      case Errc::kUnsupportedFormat:
        ret = "Unsupported serialization format for this object type.";
        break;
      case Errc::kBruteForceRisk:
        ret = "Operation is prohibitted due to a risk of a brute force attack.";
        break;
      case Errc::kContentRestrictions:
        ret = "The operation violates content restrictions of the target container.";
        break;
      case Errc::kBadObjectReference:
        ret = "Incorrect reference between objects.";
        break;
      case Errc::kLockedByReference:
        ret = "An object stored in the container is locked due to a reference from another one.";
        break;
      case Errc::kContentDuplication:
        ret = "Provided content already exists in the target storage.";
        break;
      case Errc::kUnexpectedValue:
        ret = "Unexpected value of an argument is provided.";
        break;
      case Errc::kIncompatibleObject:
        ret = "The provided object is incompatible with requested operation or its configuration.";
        break;
      case Errc::kIncompleteArgState:
        ret = "Incomplete state of an argument.";
        break;
      case Errc::kEmptyContainer:
        ret = "Specified container is empty.";
        break;
      case Errc::kBadObjectType:
        ret = "Provided object has unexpected type.";
        break;
      case Errc::kUsageViolation:
        ret = "Violation of allowed usage for the object.";
        break;
      case Errc::kAccessViolation:
        ret = "Access rights violation.";
        break;
      default:
        ret = "Unknown error.";
        break;
    }
    return ret;
  }

  // VECTOR NC Metric-HIS.VG: MD_CRYPTO_Metric-HIS.VG
  /*!
   * \brief Throws the given error_code as Exception.
   * \details If the code is compiled without exceptions, this function call will terminate.
   * \param[in] error_code error code to be thrown.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  [[noreturn]] void ThrowAsException(ara::core::ErrorCode const& error_code) const noexcept(false) final {
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
    Errc const error_class{GetErrorClass(static_cast<Errc>(error_code.Value()))};
    // VECTOR NC AutosarC++17_10-M6.4.3: MD_CRYPTO_AutosarC++17_10-M6.4.3_switch_throw
    // VECTOR NC AutosarC++17_10-M6.4.5: MD_CRYPTO_AutosarC++17_10-M6.4.3_switch_throw
    switch (error_class) {
      case Errc::kResourceFault:
        vac::language::ThrowOrTerminate<ResourceException>(error_code);
      case Errc::kBadAlloc:
        vac::language::ThrowOrTerminate<BadAllocException>(error_code);
      case Errc::kLogicFault:
        vac::language::ThrowOrTerminate<LogicException>(error_code);
      case Errc::kInvalidArgument:
        vac::language::ThrowOrTerminate<InvalidArgumentException>(error_code);
      case Errc::kInvalidUsageOrder:
        vac::language::ThrowOrTerminate<InvalidUsageOrderException>(error_code);
      case Errc::kUnsupported:
        vac::language::ThrowOrTerminate<UnsupportedException>(error_code);
      case Errc::kRuntimeFault:
        vac::language::ThrowOrTerminate<RuntimeException>(error_code);
      case Errc::kUnexpectedValue:
        vac::language::ThrowOrTerminate<UnexpectedValueException>(error_code);
      case Errc::kBadObjectType:
        vac::language::ThrowOrTerminate<BadObjectTypeException>(error_code);
      case Errc::kUsageViolation:
        vac::language::ThrowOrTerminate<UsageViolationException>(error_code);
      case Errc::kAccessViolation:
        vac::language::ThrowOrTerminate<AccessViolationException>(error_code);
      default:
        vac::language::ThrowOrTerminate<SecurityException>(error_code);
    }
  }
};

/*!
 * \brief Internal namespace.
 */
namespace internal {
// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief Global SecurityErrorDomain instance.
 * \vprivate Component Private
 */
constexpr SecurityErrorDomain kSecurityErrorDomain;
}  // namespace internal

/*!
 * \brief Singleton factory function of the \c SecurityErrorDomain.
 * \return constant reference to the single instance of the \c SecurityErrorDomain
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr ara::core::ErrorDomain const& GetSecurityErrorDomain() { return internal::kSecurityErrorDomain; }

/*!
 * \brief Makes Error Code instances from the Security Error Domain.
 * \param[in] code An error code identifier from the \c SecurityErrc enumeration.
 * \param[in] data Supplementary data for the error description.
 * \param[in] message Additional error message supplied by user-code.
 * \return The returned ErrorCode instance always references to SecurityErrorDomain.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
inline constexpr ara::core::ErrorCode MakeErrorCode(SecurityErrorDomain::Errc code,
                                                    ara::core::ErrorDomain::SupportDataType data,
                                                    char const* message = nullptr) {
  return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetSecurityErrorDomain(), data,
                              message);
}

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_EXCEPTIONS_H_
