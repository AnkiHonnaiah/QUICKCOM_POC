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
 *        \brief  Interface for Proxy Method XF
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_METHOD_XF_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_METHOD_XF_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/methods/proxy_method_backend_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief Configuration class for SOME/IP proxy method manager.
 * \unit SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyMethodXf
 *
 * \tparam OutputType The output type for the method.
 * \tparam ArgsT      The types of the input arguments for the method.
 */
template <typename OutputType, typename... ArgsT>
class ProxyMethodXfInterface
    : public ::amsr::socal::internal::methods::ProxyMethodBackendInterface<OutputType, ArgsT...> {
 public:
  /*!
   * \brief default ctor
   */
  ProxyMethodXfInterface() = default;

  /*!
   * \brief default dtor
   */
  virtual ~ProxyMethodXfInterface() = default;

  ProxyMethodXfInterface(ProxyMethodXfInterface const&) = delete;
  ProxyMethodXfInterface(ProxyMethodXfInterface&&) = delete;
  auto operator=(ProxyMethodXfInterface const&) -> ProxyMethodXfInterface& = delete;
  auto operator=(ProxyMethodXfInterface&&) -> ProxyMethodXfInterface& = delete;

  /*!
   * \brief Get the current count of pending requests from the request map.
   * \details this is only used for testing
   * \return pending request count.
   * \context     Testing
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual std::size_t GetPendingRequestCount() const noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_PROXY_METHOD_XF_INTERFACE_H_
