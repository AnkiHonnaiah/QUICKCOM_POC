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
 *        \brief  Common types used by TraceConfig.
 *      \details
 *         \unit  ComTrace::TraceConfig
 *
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_CONFIG_TYPES_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_CONFIG_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/core/string_view.h"
#include "ara/core/string.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief Type-alias for StringView.
 */
using StringView = ::amsr::core::StringView;

/*!
 * \brief Type definition for the shortname.
 */
struct Shortname {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = ara::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for Shortname.
 *
 * \param[in] left   First Shortname to compare.
 * \param[in] right  Second Shortname to compare.
 *
 * \return True if first object value equals the second object value, otherwise false.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto operator==(Shortname const& left, Shortname const& right) noexcept -> bool;

/*!
 * \brief Type definition for the shortname hash.
 */
class ShortnameHash {
 public:
  /*!
   * \brief Function call operator for Shortname.
   *
   * \param[in] shortname  Shortname to hash.
   *
   * \return Hashed shortname.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator()(Shortname const& shortname) const -> std::size_t;
};

/*!
 * \brief Type definition for the shortname path.
 */
struct ShortnamePath {
  /*!
   * \brief Underlying data type of the value.
   */
  using type = ara::core::String;

  /*!
   * \brief Data member to hold the value.
   */
  type value;
};

/*!
 * \brief Equal compare operator for Shortname path.
 *
 * \param[in] left   First Shortname path to compare.
 * \param[in] right  Second Shortname path to compare.
 *
 * \return True if first object value equals the second object value, otherwise false.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
auto operator==(ShortnamePath const& left, ShortnamePath const& right) noexcept -> bool;

/*!
 * \brief Type definition for the shortname path hash.
 */
class ShortnamePathHash {
 public:
  /*!
   * \brief Function call operator for Shortname path.
   *
   * \param[in] shortname_path  Shortname path to hash.
   *
   * \return Hashed shortname path.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator()(ShortnamePath const& shortname_path) const -> std::size_t;
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_CONFIG_TYPES_H_
