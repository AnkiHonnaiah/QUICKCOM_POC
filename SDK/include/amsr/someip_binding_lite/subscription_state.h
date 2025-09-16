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
/*!        \file   someip_binding_lite/subscription_state.h
 *         \brief  Enumeration type for subscription states.
 *         \unit   SomeIpBinding::SomeIpBindingLite::SomeIpBindingLiteDataTypes
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SUBSCRIPTION_STATE_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SUBSCRIPTION_STATE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Definitions for subscription states of an event.
 * \vpublic
 */
enum class SubscriptionState : std::uint8_t {
  kSubscribed = 0u,         /*!< Subscribed to an event */
  kNotSubscribed = 1u,      /*!< Not subscribed to an event */
  kSubscriptionPending = 2u /*!< Event subscription pending */
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_SUBSCRIPTION_STATE_H_
