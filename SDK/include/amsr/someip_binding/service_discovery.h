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
/*!        \file  someip_binding/service_discovery.h
 *        \brief  Public APIs of SOME/IP Binding that are related to ServiceDiscovery
 *         \unit  SomeIpBinding::SomeIpBinding::ServiceDiscovery
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_SERVICE_DISCOVERY_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_SERVICE_DISCOVERY_H_

#include "amsr/generic/activatable_source_code.h"

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace someip_binding {

/*!
 * \brief   Restart service discovery for all required service instances.
 * \details All configured required service instances will be released and then requested again. In case no other
 *          process requests the same required service instances, this will trigger restarting the corresponding client
 *          state machines (and accordingly might trigger sending SD FindService messages).
 *          In case TCP connections existed for the already found services, this API will close all connections for the
 *          corresponding service instances, and try to re-connect.
 *          As closing TCP connection might not be done immediately, depending on the underlying OS, it is not
 *          guaranteed that Re-connecting to servers will be done directly.
 *          Therefore it is recommended to use this API when no connections to servers already exist (Either all
 *          services are configured via UDP or no offers from servers that use TCP have been yet received).
 *
 * \pre SomeIpBinding has been initialized.
 * \pre SomeIpDaemon connection retrial is disabled.
 *
 * \context     APP
 * \threadsafe  FALSE
 * \vpublic
 * \reentrant   FALSE
 * \synchronous TRUE
 */
AMSR_LG_BETA void RestartServiceDiscovery() noexcept;

}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_SERVICE_DISCOVERY_H_
