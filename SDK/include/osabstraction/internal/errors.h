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
 *        \brief  Common error related header
 *
 *********************************************************************************************************************/
#ifndef LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INTERNAL_ERRORS_H_
#define LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INTERNAL_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <type_traits>

namespace osabstraction {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
/*!
 * \brief API error in operating system service.
 *
 * \details
 * Most operating system APIs use -1 to report the failure of the service call.
 */
constexpr int const kSystemCallFailed{-1};

// VECTOR Next Construct AutosarC++17_10-A15.4.4: MD_OSA_A15.4.4_template_operator_noexcept
/*!
 * \brief Returns whether the returned value of a system call indicates a failure.
 * \param[in] system_call_retval  Return value of a system call.
 * \tparam    RetvalType          The type of the return value of a system call. Must be int or ssize_t.
 * \return true when the system call failed, false otherwise.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 */
template <typename RetvalType>
inline bool HasSystemCallFailed(RetvalType const system_call_retval) noexcept {
  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
  // VECTOR Next Construct AutosarC++17_10-M27.0.1: MD_OSA_M27.0.1_PosixApi
  static_assert(std::is_same<typename std::remove_cv<RetvalType>::type, int>::value ||
                    std::is_same<typename std::remove_cv<RetvalType>::type, ssize_t>::value,
                "HasSystemCallFailed() only accepts types int or ssize_t as parameter.");

  return system_call_retval < 0;
}

/*!
 * \brief Type safe error number
 * \details
 * Contains the error code usually stored in the POSIX global variable errno.
 *
 * \vprivate Vector component internal
 */
struct OsErrorNumber {
  /*! \brief Error number of an operating system service call. */
  std::int32_t value;
};

/*!
 * \brief Returns the last service calls error number.
 * \return Last system calls error number.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \unit            osabstraction::Error
 */
inline OsErrorNumber GetErrorNumber() noexcept {
  // VECTOR Next Line AutosarC++17_10-M19.3.1: MD_OSA_M19.3.1_PosixApi
  return OsErrorNumber{errno};
}

/*!
 * \brief Resets the error number to 0.
 * \context         ANY
 * \pre             -
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \unit            osabstraction::Error
 */
inline void ClearErrorNumber() noexcept {
  // VECTOR Next Line AutosarC++17_10-M19.3.1: MD_OSA_M19.3.1_PosixApi
  errno = 0;  // VCA_OSA_STL_CALL_PASS_PTR_R
}

}  // namespace internal
}  // namespace osabstraction

#endif  // LIB_UTILS_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INTERNAL_ERRORS_H_
