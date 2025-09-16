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
/**        \file  socal/internal/types.h
 *        \brief  Types used by ara::com API
 *
 *      \details  Specific types needed by the ara::com API for proxy, skeleton and the runtime interface.
 *        \trace  SPEC-4980255
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TYPES_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <string>

#include "amsr/core/string_view.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/events/timestamp_config.h"
#include "ara/com/instance_identifier.h"
#include "vac/container/static_list.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
// VECTOR NL AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Alias for the service shortname path.
 */
using ServiceShortNamePath = ::amsr::core::StringView;

/*!
 * \brief Type alias for shortning of assignment name.
 */
using ThreadPoolAssignmentIdentifier = ::amsr::core::StringView;

/*!
 * \brief Assignment name to identify the r port default thread pool.
 */
static constexpr ThreadPoolAssignmentIdentifier kAssignmentNameRPortDefaultThreadPool{"RPortDefaultThreadPool "_sv};

/*!
 * \brief Assignment name to identify the p port default thread pool.
 */
static constexpr ThreadPoolAssignmentIdentifier kAssignmentNamePPortDefaultThreadPool{"PPortDefaultThreadPool "_sv};

/*!
 * \brief Assignment name to identify vComContinuation thread pool.
 */
static constexpr ThreadPoolAssignmentIdentifier kAssignmentNameContinuationThreadPool{"ComContinuationThreadPool"_sv};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TYPES_H_
