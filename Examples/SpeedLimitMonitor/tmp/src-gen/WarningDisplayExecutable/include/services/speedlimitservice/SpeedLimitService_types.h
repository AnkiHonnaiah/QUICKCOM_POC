
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/include/services/speedlimitservice/SpeedLimitService_types.h
 *        \brief  Input and output structures for methods, fields and application errors of service 'SpeedLimitService'
 *
 *      \details  Definition of common input-/output structs used for simplified argument / marshalling handling. For
 *                all elements like methods, events fields structs with the related in-/output arguments are generated.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_socal
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_TYPES_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/handle_type.h"
#include "services/speedlimitservice/SpeedLimitService_datatypes.h"
#include "services/speedlimitservice/speedlimitservice_common.h"

namespace services {
namespace speedlimitservice {
namespace internal {

/*!
 * \brief Proxy HandleType for the Service 'SpeedLimitService'.
 *
 * \trace SPEC-4980259
 */
class SpeedLimitServiceHandleType final : public ::amsr::socal::HandleType {
 public:
  /*!
   * \brief Inherit constructor.
   */
  using HandleType::HandleType;
};

}  // namespace internal
}  //  namespace speedlimitservice
}  //  namespace services

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_TYPES_H_

