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
 *        \brief  Read-only interface for generator version.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_GENERATOR_VERSION_CONFIG_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_GENERATOR_VERSION_CONFIG_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "amsr/comtrace/internal/config/config_types.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief   Read-only interface for generator version.
 * \details Class storing the generator version. Stored configuration parameters:
 *          - Name: The name of generator that created the JSON file.
 *          - CommitId: The commit ID of generator that created the JSON file.
 *
 * \unit ComTrace::TraceConfig
 */
class GeneratorVersionConfigInterface {
 public:
  auto operator=(GeneratorVersionConfigInterface const&) noexcept -> GeneratorVersionConfigInterface& = delete;
  GeneratorVersionConfigInterface(GeneratorVersionConfigInterface&&) noexcept = delete;
  auto operator=(GeneratorVersionConfigInterface&&) noexcept -> GeneratorVersionConfigInterface& = delete;

  /*!
   * \brief Destroy generator version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~GeneratorVersionConfigInterface() noexcept = default;

  /*!
   * \brief Get the generator name.
   *
   * \return Name of the generator.
   *
   * \pre       Config object was filled by config loader.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto GetName() const noexcept -> StringView = 0;

  /*!
   * \brief Get the commit ID.
   *
   * \return Commit ID.
   *
   * \pre       Config object was filled by config loader.
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual auto GetCommitId() const noexcept -> StringView = 0;

 protected:
  /*!
   * \brief Construct an empty generator version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GeneratorVersionConfigInterface() noexcept = default;

  /*!
   * \brief Copy-Construct a generator version configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  GeneratorVersionConfigInterface(GeneratorVersionConfigInterface const&) noexcept = default;
};

/*! Type name alias for reference of generator version config. */
using GeneratorVersionConfigInterfaceRef = std::reference_wrapper<GeneratorVersionConfigInterface const>;

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_GENERATOR_VERSION_CONFIG_INTERFACE_H_
