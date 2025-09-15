
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
/**        \file  socal/internal/types_service_discovery.h
 *        \brief  Types used by service discovery APIs
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TYPES_SERVICE_DISCOVERY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TYPES_SERVICE_DISCOVERY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Alias for a StartFindService callback used internally by Socal.
 */
using StartFindServiceCallback = ::ara::com::FindServiceHandlerExt<::amsr::socal::HandleType>;

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TYPES_SERVICE_DISCOVERY_H_
