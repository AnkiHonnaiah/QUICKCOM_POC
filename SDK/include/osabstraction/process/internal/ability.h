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
/**        \file  libosabstraction-linux/include/osabstraction/process/internal/ability.h
 *        \brief  Ability implementation for Linux.
 *        \unit   osabstraction::Process_Linux
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_ABILITY_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_ABILITY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "ara/core/vector.h"
#include "osabstraction/osab_error_domain.h"
#include "osabstraction/process/ability_configuration.h"
#include "osabstraction/process/process_types.h"

namespace osabstraction {
namespace process {

namespace internal {
/*!
 * \brief Ability class
 */
class Ability final {
 public:
  /*!
   * \brief     Dummy ability creation implementation.
   * \param[in] name    Dummy parameter.
   * \param[in] configs Dummy parameter.
   * \param[in] inherit Dummy parameter.
   * \param[in] lock    Dummy parameter.
   * \return          Always an error.
   * \error           osabstraction::OsabErrc::kApiError              Functionality not implemented (Linux).
   * \context         InitPhase
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector component internal API
   */
  static ::amsr::core::Result<Ability> Create(AbilityName const& name,
                                              ara::core::Vector<AbilityConfigurationEntry> const& configs,
                                              AbilityInheritance const& inherit, AbilityLock const& lock) noexcept;

  // Disable copying to avoid dynamic memory allocation at runtime.
  Ability(Ability const&) = delete;
  Ability& operator=(Ability const&) = delete;

  /*!
   * \brief Move constructs an ability.
   *
   * \steady          FALSE
   */
  Ability(Ability&&) noexcept = default;

  /*!
   * \brief  Move assigns an ability.
   * \return A reference to the modified Ability object.
   * \steady          FALSE
   */
  Ability& operator=(Ability&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \steady          FALSE
   */
  ~Ability() noexcept = default;

 private:
  /*!
   * \brief     Constructs Ability object.
   */
  Ability() noexcept = default;
};

}  // namespace internal
}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_ABILITY_H_
