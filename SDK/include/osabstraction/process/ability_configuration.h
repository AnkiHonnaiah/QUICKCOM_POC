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
/*!       \file   ability_configuration.h
 *        \brief  Public data types which are used for ability configuration by Process API.
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_ABILITY_CONFIGURATION_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_ABILITY_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/process/process_types.h"

namespace osabstraction {
namespace process {

/*!
 * \brief   Data type to specify the name of the ability.
 * \details Implemented only on QNX.
 */
struct AbilityName {  // VCA_OSA_MOLE_1298
  /*!
   * \brief   Name of the ability.
   */
  ::amsr::core::String value;
};

/*!
 * \brief   Data type to specify the inheritance of the ability.
 * \details 0 means no inheritance, 1 means inheritance configured.
 */
using AbilityInheritance = bool;

/*!
 * \brief   Data type to specify if the ability has to be locked.
 * \details 0 means ability will not be locked, 1 means ability will be locked.
 */
using AbilityLock = bool;

/*!
 * \brief   Data type to specify if the ability has to be allowed or not.
 * \details 0 means ability will not be allowed, 1 means ability will be allowed.
 */
using AbilityAllowed = bool;

/*!
 * \brief   Data type to specify the domain of the ability.
 * \details Valid strings are [ROOT, NON_ROOT, ALL].
 */
struct AbilityDomain {
  /*!
   * \brief   Domain of the ability.
   */
  ::amsr::core::String value;
};

/*!
 * \brief   Range of values for a specific ability.
 * \details Implemented only on QNX.
 */
struct AbilityRange {
  /*!
   * \brief   Lower range for the allowed ability values.
   */
  std::uint64_t lower;

  /*!
   * \brief   Upper range for the allowed ability values.
   */
  std::uint64_t upper;
};

/*!
 * \brief Ability configuration entry class
 */
class AbilityConfigurationEntry final {
 public:
  /*!
   * \brief             Constructs a valid AbilityConfigurationEntry object.
   * \param[in]         allowed      Boolean parameter
   *                                 true if the ability has to be allowed for the given domain, false otherwise.
   * \param[in]         domain       String that represents the domain of the ability. Default is "ALL".
   * \param[in]         ranges       Allowed ranges of values for the given domain. Ignored if empty.
   * \context           InitPhase
   * \pre               -
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   * \steady            TRUE
   */
  explicit AbilityConfigurationEntry(AbilityAllowed const& allowed, AbilityDomain domain,
                                     ara::core::Vector<AbilityRange> ranges) noexcept;

  /*!
   * \brief             Deleted default constructor
   */
  AbilityConfigurationEntry() = delete;

  /*!
   * \brief Deleted copy constructor.
   */
  AbilityConfigurationEntry(AbilityConfigurationEntry const&) = delete;

  /*!
   * \brief Deleted copy assignment operato.
   */
  AbilityConfigurationEntry& operator=(AbilityConfigurationEntry const&) = delete;

  /*!
   * \brief Move constructs an AbilityConfigurationEntry.
   *
   * \steady          TRUE
   */
  AbilityConfigurationEntry(AbilityConfigurationEntry&&) noexcept = default;

  /*!
   * \brief Move assigns an AbilityConfigurationEntry.
   * \return  This AbilityConfigurationEntry object.
   *
   * \steady         TRUE
   */
  AbilityConfigurationEntry& operator=(AbilityConfigurationEntry&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \steady         FALSE
   */
  ~AbilityConfigurationEntry() noexcept = default;  // VCA_OSA_EXT_CALL_OBJ_DTOR

  /*!
   * \brief Returns allowed_ flag.
   * \return Reference to const allowed_ flag.
   * \steady            TRUE
   */
  AbilityAllowed const& GetAllowed() const noexcept;

  /*!
   * \brief  Returns domain_ string.
   * \return Reference to const domain_ string.
   * \steady            TRUE
   */
  AbilityDomain const& GetDomain() const noexcept;

  /*!
   * \brief  Returns ranges_ vector.
   * \return Reference to const ranges_ array.
   * \steady            FALSE
   */
  ara::core::Vector<AbilityRange> const& GetRanges() const noexcept;

 private:
  AbilityAllowed allowed_;                 /*!< Stores the allowed property of the ability. */
  AbilityDomain domain_;                   /*!< Stores the domain of the ability. */
  ara::core::Vector<AbilityRange> ranges_; /*!< Stores the ranges of values for the ability. */
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_ABILITY_CONFIGURATION_H_
