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
/*!        \file  amsr/someip_binding_core/internal/server_manager_interface.h
 *        \brief  ara::com SOME/IP Binding Server Manager Interface
 *
 *      \details  Interface of ServerManager
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/methods/skeleton_method_handler_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Realizes a concrete Binding for the skeleton communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
class ServerManagerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ServerManagerInterface() = default;
  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ServerManagerInterface() = default;

  ServerManagerInterface(ServerManagerInterface const&) = delete;
  ServerManagerInterface(ServerManagerInterface&&) = delete;
  ServerManagerInterface& operator=(ServerManagerInterface const&) & = delete;
  ServerManagerInterface& operator=(ServerManagerInterface&&) & = delete;

  /*!
   * \brief       Register one service binding on construction of a ServiceSkeletonXf.
   * \param[in]   psid Uniquely identify a service binding with a service id, instance id, major_version,
   *              minor_version.
   * \param[in]   server The service provider.
   *              On method request / event request, a look-up through the registered skeleton bindings happens, to
   *              delegate the request to the correct service binding object.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  virtual void RegisterProvidedServiceInstance(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& psid,
      ::amsr::someip_binding_core::internal::methods::SkeletonMethodHandlerInterface& server) = 0;

  /*!
   * \brief       Actively de-register one service binding (e.g. after StopOfferService gets called).
   * \param[in]   psid The key to search for and the associated value to erase from the list.
   * \pre         service is already registered
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  virtual void DeregisterProvidedServiceInstance(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& psid) = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SERVER_MANAGER_INTERFACE_H_
