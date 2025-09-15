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
/**     \file
 *      \brief      Provide functionality to unwrap results.
 *      \details    Provides unwrapping results without exceptions of undefined behavior.
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_UNWRAP_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_UNWRAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/loguti/abort.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace loguti {
/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \param   result The result to unwrap
 * \param   ctx_id The context ID to use if logging is active.
 * \param   message The message to write in case of an error
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
inline void Unwrap(amsr::core::Result<void> result, vac::container::CStringView ctx_id,
                   vac::container::CStringView message) noexcept {
  if (not result) {
    Abort(ctx_id, result.Error(), message);
  }
}

/*!
 * \brief   Unwraps the result and logs on error before aborting

 * \param   result The result to unwrap
 * \param   message The message to write in case of an error
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
inline void Unwrap(amsr::core::Result<void> result, vac::container::CStringView message) noexcept {
  Unwrap(result, internal::kVctrCtxId, message);
}

/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \tparam  T         The type in the result.
 *                    T must comply to the template parameter requirements of amsr::core::Result<T>.
 *
 * \param   result    The result to unwrap
 * \param   ctx_id    The context ID to use if logging is active.
 * \param   message   The message to write in case of an error
 *
 * \return  The unwrapped value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
template <typename T>
auto Unwrap(amsr::core::Result<T>&& result, vac::container::CStringView ctx_id,
            vac::container::CStringView message) noexcept -> T {
  Unwrap(result.Drop(), ctx_id, message);
  return std::move(result).Value();
}

/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \tparam  T         The type in the result.
 *                    T must comply to the template parameter requirements of amsr::core::Result<T>.
 *
 * \param   result    The result to unwrap
 * \param   message   The message to write in case of an error
 *
 * \return  The unwrapped value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
template <typename T>
auto Unwrap(amsr::core::Result<T>&& result, vac::container::CStringView message) noexcept -> T {
  return Unwrap(std::move(result), internal::kVctrCtxId, message);
}

/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \tparam  T         The type in the result.
 *                    T must comply to the template parameter requirements of amsr::core::Result<T>.
 *
 * \param   result    The result to unwrap
 * \param   ctx_id    The context ID to use if logging is active.
 * \param   message   The message to write in case of an error
 *
 * \return  The unwrapped value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
template <typename T>
auto Unwrap(amsr::core::Result<T>& result, vac::container::CStringView ctx_id,
            vac::container::CStringView message) noexcept -> T& {
  Unwrap(result.Drop(), ctx_id, message);
  return result.Value();
}

/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \tparam  T         The type in the result.
 *                    T must comply to the template parameter requirements of amsr::core::Result<T>.
 *
 * \param   result    The result to unwrap
 * \param   message   The message to write in case of an error
 *
 * \return  The unwrapped value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
template <typename T>
auto Unwrap(amsr::core::Result<T>& result, vac::container::CStringView message) noexcept -> T& {
  return Unwrap(result, internal::kVctrCtxId, message);
}

/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \tparam  T         The type in the result.
 *                    T must comply to the template parameter requirements of amsr::core::Result<T>.
 *
 * \param   result    The result to unwrap
 * \param   ctx_id    The context ID to use if logging is active.
 * \param   message   The message to write in case of an error
 *
 * \return  The unwrapped value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
template <typename T>
auto Unwrap(amsr::core::Result<T> const& result, vac::container::CStringView ctx_id,
            vac::container::CStringView message) noexcept -> T const& {
  Unwrap(result.Drop(), ctx_id, message);
  return result.Value();
}

/*!
 * \brief   Unwraps the result and logs on error before aborting
 *
 * \tparam  T         The type in the result.
 *                    T must comply to the template parameter requirements of amsr::core::Result<T>.
 *
 * \param   result    The result to unwrap
 * \param   message   The message to write in case of an error
 *
 * \return  The unwrapped value
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Vector product internal API.
 * \unit amsr::loguti::Unwrap
 */
template <typename T>
auto Unwrap(amsr::core::Result<T> const& result, vac::container::CStringView message) noexcept -> T const& {
  return Unwrap(result, internal::kVctrCtxId, message);
}
}  // namespace loguti
}  // namespace amsr

#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_UNWRAP_H_
