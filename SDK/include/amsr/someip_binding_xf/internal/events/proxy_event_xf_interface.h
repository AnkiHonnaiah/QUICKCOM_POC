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
/**        \file
 *        \brief  Interface for Proxy Event XF
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_PROXY_EVENT_XF_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_PROXY_EVENT_XF_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Configuration class for SOME/IP proxy event manager.
 * \unit SomeIpBinding::SomeIpBindingXf::Communication::Event::ProxyEventXf
 *
 * \tparam SampleType Event Sample Data Type.
 */
template <typename SampleType>
class ProxyEventXfInterface : public ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType> {
 public:
  /*!
   * \brief default ctor
   * \steady FALSE
   */
  ProxyEventXfInterface() = default;

  /*!
   * \brief default dtor
   * \steady FALSE
   */
  virtual ~ProxyEventXfInterface() = default;

  ProxyEventXfInterface(ProxyEventXfInterface const&) = delete;
  ProxyEventXfInterface(ProxyEventXfInterface&&) = delete;
  auto operator=(ProxyEventXfInterface const&) -> ProxyEventXfInterface& = delete;
  auto operator=(ProxyEventXfInterface&&) -> ProxyEventXfInterface& = delete;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_PROXY_EVENT_XF_INTERFACE_H_
