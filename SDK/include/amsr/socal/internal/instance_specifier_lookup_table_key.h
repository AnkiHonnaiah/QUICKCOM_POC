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
/**        \file  instance_specifier_lookup_table_key.h
 *        \brief  Define an entry used in an InstanceSpecifierLookupTable.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_SPECIFIER_LOOKUP_TABLE_KEY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_SPECIFIER_LOOKUP_TABLE_KEY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "amsr/socal/internal/types.h"
#include "ara/core/instance_specifier.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief   Key for the look-up table wrapping the InstanceSpecifier along with with service shortname path.
 * \details Following info is stored:
 *          - InstanceSpecifier
 *          - The short name path of the service interface.
 *
 * \unit Socal::InstanceSpecifierLookupTable::InstanceSpecifierLookupTable
 */
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class InstanceSpecifierLookupTableKey {
 public:
  /*!
   * \brief Construct InstanceSpecifierLookupTableKey.
   * \param[in] instance_specifier      The instance specifier.
   * \param[in] service_shortname_path  The shortname path of this service interface.
   * \pre     -
   * \context Init
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit InstanceSpecifierLookupTableKey(::ara::core::InstanceSpecifier const& instance_specifier,
                                           ServiceShortNamePath const& service_shortname_path = ""_sv) noexcept;

  /*!
   * \brief Overloads < operator.
   * \param[in] other  InstanceSpecifierLookupTableKey to compare.
   * \return true if *this < other else false
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool operator<(InstanceSpecifierLookupTableKey const& other) const noexcept;

  /*!
   * \brief Getter for the instance specifier.
   * \return the instance specifier.
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::core::InstanceSpecifier GetInstanceSpecifier() const noexcept;

  /*!
   * \brief Get the service shortname path.
   * \return The service shortname path.
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ServiceShortNamePath const GetServiceShortnamePath() const noexcept;

 private:
  /*!
   * \brief Instance specifier.
   */
  ::ara::core::InstanceSpecifier instance_specifier_;

  /*!
   * \brief Service Shortname.
   */
  std::string const service_shortname_path_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_SPECIFIER_LOOKUP_TABLE_KEY_H_
