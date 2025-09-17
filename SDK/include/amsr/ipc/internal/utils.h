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
 *        \brief  Provides utility functions used in Safe IPC implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_UTILS_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_UTILS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "osabstraction/osab_error_domain.h"

namespace amsr {
namespace ipc {
namespace internal {

/*!
 * \brief           Converts a boolean value to amsr::core::Result with specified error when it is false.
 *
 * \param[in]       condition Condition to convert.
 * \param[in]       error     Returned result will contain this error code if the condition is false.
 *
 * \return          Empty result if the condition evaluated to true, error otherwise.
 *
 * \error           Error value depends on passed error parameter
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \unit            osabstraction::ipc::IpcUtils_LinuxPikeOs
 * \unit            osabstraction::ipc::IpcUtils_QNX
 */
inline auto CheckOr(bool condition, ::amsr::core::ErrorCode error) noexcept -> ::amsr::core::Result<void> {
  return condition ? ::amsr::core::Result<void>{} : ::amsr::core::Result<void>{error};
}

/*!
 * \brief           Replaces the error code enum value of an OsAbstraction error code while keeping the support data and
 *                  the user message.
 *
 * \param[in]       old_error          ErrorCode whose enum value shall be replaced.
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
 *
 * \unit            osabstraction::ipc::IpcUtils_LinuxPikeOs
 * \unit            osabstraction::ipc::IpcUtils_QNX
 */
inline auto ReplaceErrorCode(::amsr::core::ErrorCode old_error, osabstraction::OsabErrc new_error_code) noexcept
    -> ::amsr::core::ErrorCode {
  return MakeErrorCode(new_error_code, old_error.SupportData(), old_error.UserMessage().data());
}

/*!
 * \brief           Write a formatted null-terminated string to a supplied buffer.
 *
 * \param[out]      buffer    Span containing a character string to write the result.
 * \param[in]       format    Pointer to a null terminated string that is written to the supplied buffer.
 *                            It consists of characters along with optional format specifiers starting with %.
 *
 * \context         ANY
 *
 * \pre             The supplied buffer has to be large enough for the resulting string (+ null termination).
 * \pre             The passed format and the variable argument list have to be valid input for vsnprintf().
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \unit            osabstraction::ipc::IpcUtils_LinuxPikeOs
 * \unit            osabstraction::ipc::IpcUtils_QNX
 */
void FormatString(::amsr::core::Span<char> buffer, char const* format, ...) noexcept;

/*!
 * \brief           Converts an integer value to a value of a different integer type.
 *
 * \details         The function applies a "clipping" algorithm, meaning that values of the source type that do not fit
 *                  into the destination type result in the closest value (closest to the source value) that fits into
 *                  the destination type.
 *
 * \param[in]       value              The integer value to convert
 *
 * \tparam          Dest               The integer type to convert to.
 * \tparam          Source             The original integer type of the value.
 *
 * \return          The value in the destination type.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \unit            osabstraction::ipc::IpcUtils_LinuxPikeOs
 * \unit            osabstraction::ipc::IpcUtils_QNX
 */
template <typename Dest, typename Source>
inline auto ConvertIntegerValue(Source value) noexcept -> Dest {
  Source min{0};
  Source max{0};

  // min and max are basically filled with following values:
  // - max = std::min(std::numeric_limits<Source>::max(), std::numeric_limits<Dest>::max());
  // - min = std::max(std::numeric_limits<Source>::min(), std::numeric_limits<Dest>::min());
  // However, just writing it like this results in type conflicts for the template types of std::min and std::max.
  // Just doing a cast to force one to the type of the other risks data loss. Thus, "unroll" std::min and std::max here
  // to allow doing the check that prevents the data loss.

  if (std::numeric_limits<Source>::max() <= std::numeric_limits<Dest>::max()) {
    max = std::numeric_limits<Source>::max();
  } else {
    // No data loss as of if-check.
    max = static_cast<Source>(std::numeric_limits<Dest>::max());
  }

  // VECTOR NL AutosarC++17_10-M0.1.2: MD_OSA_M0.1.2_IntegerConversionUtility
  if ((!std::is_signed<Dest>::value) || (!std::is_signed<Source>::value)) {
    min = 0;
  } else if (std::numeric_limits<Source>::min() >= std::numeric_limits<Dest>::min()) {
    min = std::numeric_limits<Source>::min();
  } else {
    // No data loss as of if-check.
    min = static_cast<Source>(std::numeric_limits<Dest>::min());
  }

  Source const result_value{std::min(std::max(value, min), max)};

  // No data loss. At this point, the value is limited to at least the boundaries of Dest, probably even more narrow
  // in case the limits of Source are within the limits of Dest.
  return static_cast<Dest>(result_value);
}

}  // namespace internal
}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTERNAL_UTILS_H_
