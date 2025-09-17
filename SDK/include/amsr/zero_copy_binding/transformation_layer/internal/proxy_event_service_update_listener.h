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
/*!        \file
 *         \brief  Interface class used by the ProxyXf to be able to address all events at once.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_SERVICE_UPDATE_LISTENER_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_SERVICE_UPDATE_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   Base class of ProxyEventBackend that is used to notify it about event updates.
 *
 * \unit    ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ProxyEventBackend
 */
class ProxyEventServiceUpdateListener {
 public:
  /*!
   * \brief       Handle a service instance update (service offer) triggered by the service discovery.
   * \details     This function is used by the ProxyXf that has received a service offer from Socal.
   * \context     Reactor
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  virtual void OnServiceInstanceUp() noexcept = 0;

  /*!
   * \brief       Handle a service instance update (service stop offer) triggered by the service discovery.
   * \details     This function is used by the ProxyXf that has received a service stop offer from Socal.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  virtual void OnServiceInstanceDown() noexcept = 0;

  /*!
   * \brief       Check if the ProxyEventServiceUpdateListener is currently connected to a SkeletonEventManager.
   * \return      A boolean indicating whether a connection is currently established.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto IsConnectedToSkeletonEvent() const noexcept -> bool = 0;

  /*!
   * \brief       Default destructor.
   * \vprivate    Component private
   */
  virtual ~ProxyEventServiceUpdateListener() noexcept = 0;

  ProxyEventServiceUpdateListener(ProxyEventServiceUpdateListener const&) = delete;
  auto operator=(ProxyEventServiceUpdateListener const&) -> ProxyEventServiceUpdateListener& = delete;
  ProxyEventServiceUpdateListener(ProxyEventServiceUpdateListener&&) = delete;
  auto operator=(ProxyEventServiceUpdateListener&&) -> ProxyEventServiceUpdateListener& = delete;

 protected:
  /*!
   * \brief       Default constructor.
   * \vprivate    Component private
   */
  ProxyEventServiceUpdateListener() noexcept = default;
};

inline ProxyEventServiceUpdateListener::~ProxyEventServiceUpdateListener() noexcept = default;

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_EVENT_SERVICE_UPDATE_LISTENER_H_
