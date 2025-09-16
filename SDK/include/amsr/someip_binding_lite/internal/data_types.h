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
/*!        \file   someip_binding_lite/internal/data_types.h
 *         \brief  Internal (private) data types.
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLiteDataTypes
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_DATA_TYPES_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_DATA_TYPES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/client_manager_interface.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_core.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace someip_binding_lite {
namespace internal {

/*!
 * \brief Type-alias for the SOME/IP binding core.
 */
using SomeIpBindingCoreType = someip_binding_core::internal::SomeIpBindingCore;

/*!
 * \brief Type-alias for the SOME/IP binding core interface.
 */
using SomeIpBindingCoreInterfaceType = someip_binding_core::internal::SomeIpBindingCoreInterface;

/*!
 * \brief Type-alias for the client manager.
 */
using ClientManagerType = someip_binding_core::internal::ClientManagerInterface;

/*!
 * \brief Type for reactor.
 */
using ReactorType = osabstraction::io::reactor1::Reactor1;

/*!
 * \brief Type for reactor interface.
 */
using ReactorInterfaceType = osabstraction::io::reactor1::Reactor1Interface;

/*!
 * \brief Type for service listener interface.
 */
using ServiceListenerInterfaceType = someip_binding_core::internal::service_discovery::ServiceListenerInterface;

/*!
 * \brief Type for subscription state.
 */
using SubscriptionStateType = someip_protocol::internal::SubscriptionState;

/*!
 * \brief Type for provided instance.
 */
using ProvidedServiceInstanceIdType = someip_binding_core::internal::ProvidedServiceInstanceId;

/*!
 * \brief Type for required instance.
 */
using RequiredServiceInstanceIdType = someip_binding_core::internal::RequiredServiceInstanceId;

}  // namespace internal
}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_INTERNAL_DATA_TYPES_H_
