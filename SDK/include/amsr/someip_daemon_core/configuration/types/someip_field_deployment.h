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
/**        \file  someip_field_deployment.h
 *        \brief  SOME/IP Field deployment
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_FIELD_DEPLOYMENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_FIELD_DEPLOYMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/someip_event_deployment.h"
#include "amsr/someip_daemon_core/configuration/types/someip_method_deployment.h"
#include "ara/core/optional.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {

/*!
 * \brief SOME/IP configuration settings for a Field.
 */
struct SomeIpFieldDeployment {
  /*!
   * \brief This aggregation represents the setting of the get method.
   */
  ::ara::core::Optional<SomeIpMethodDeployment> get;
  /*!
   * \brief This aggregation represents the setting of the set method.
   */
  ::ara::core::Optional<SomeIpMethodDeployment> set;
  /*!
   * \brief This aggregation represents the setting of the notifier.
   */
  ::ara::core::Optional<SomeIpEventDeployment> notifier;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_SOMEIP_FIELD_DEPLOYMENT_H_
