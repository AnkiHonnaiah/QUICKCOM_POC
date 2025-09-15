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
/**        \file  libosabstraction-posix/include/osabstraction/process/internal/resource_limit.h
 *        \brief  POSIX Resource limit declaration.
 *        \unit   osabstraction::Process_QNX
 *        \unit   osabstraction::Process_Linux
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_RESOURCE_LIMIT_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_RESOURCE_LIMIT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "osabstraction/process/process_types.h"

namespace osabstraction {
namespace process {
namespace internal {

/*!
 * \brief   Data type to specify the identifier of the resource limit.
 */
using ResourceLimitId = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief ResourceLimit class
 */
class ResourceLimit final {
 public:
  /*!
   * \brief           Creates a ResourceLimit object from its identifier string representation.
   *
   * \param[in]       name    The name of the resource.
   * \param[in]       value   The limit of the resource value.
   *
   * \return          On success a ResourceLimit object.
   *
   * \error           osabstraction::OsabErrc::kApiError     Creating ResourceLimit is not supported.
   *
   * \context         InitPhase
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   *
   * \trace           DSGN-Osab-Process-ProcessResourceLimit
   *
   * \vprivate        Vector component internal API
   */
  static ::amsr::core::Result<ResourceLimit> Create(ResourceLimitName name, ResourceLimitValue const& value) noexcept;

  // Disable copying to avoid dynamic memory allocation at runtime.
  ResourceLimit(ResourceLimit const&) = delete;
  ResourceLimit& operator=(ResourceLimit const&) = delete;

  /*!
   * \brief Move constructs an ability.
   */
  ResourceLimit(ResourceLimit&&) noexcept = default;

  /*!
   * \brief Move assigns an ResourceLimit.
   */
  ResourceLimit& operator=(ResourceLimit&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   */
  ~ResourceLimit() noexcept = default;

  /*!
   * \brief             Returns the ID of the resource limit.
   * \return            resource limit ID.
   * \context           ANY
   * \pre               -
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   * \vprivate          Vector component internal API
   */
  ResourceLimitId GetId() const noexcept { return id_; }

  /*!
   * \brief             Returns the value of the resource limit.
   * \return            resource limit value.
   * \context           ANY
   * \pre               -
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   * \vprivate          Vector component internal API
   */
  ResourceLimitValue GetValue() const noexcept { return value_; }

  /*!
   * \brief             Returns if the resource limit is valid.
   * \details           This information shall be processed during process creation, because
   *                    EM is not able to handle this information properly during OsProcessSettings setup (error would
   *                    be ignored and the process will be started without configured setting).
   * \return            true if valid, false otherwise.
   * \context           ANY
   * \pre               -
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   * \vprivate          Vector component internal API
   */
  // Covered by TCASE: UT__OsProcessSettings__Linux.ResourceLimits
  bool IsValid() const noexcept { return valid_; }  // COV_OSAB_TOOLINGISSUE

 private:
  /*!
   * \brief     Constructs invalid ResourceLimit object.
   */
  ResourceLimit() noexcept = default;

  // VECTOR Next Construct VectorC++-V3.9.2: MD_OSA_V3.9.2_ResourceLimit
  /*!
   * \brief     Constructs valid ResourceLimit object.
   */
  ResourceLimit(ResourceLimitId const& id, ResourceLimitValue const& value) noexcept
      : id_{id}, value_{value}, valid_{true} {};

  ResourceLimitId id_{};       /*!< Stores the ID of the resource limit. */
  ResourceLimitValue value_{}; /*!< Stores the value of the resource limit. */
  bool valid_{false};          /*!< Stores the valid flag of the resource limit. */
};

}  // namespace internal
}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_RESOURCE_LIMIT_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_OSAB_TOOLINGISSUE
//   \ACCEPT XX
//   \REASON The function is already covered by a test case but VCast does not show correct coverage measurements.
// COV_JUSTIFICATION_END
