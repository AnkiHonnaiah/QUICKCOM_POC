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
/*!        \file  amsr/someip_binding_core/internal/local_server_manager_interface.h
 *        \brief  ara::com SOME/IP Binding Local Server Manager Interface
 *
 *      \details  Interface of LocalServerManager
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface for a local server manager.
 */
class LocalServerManagerInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  LocalServerManagerInterface() = default;

  /*!
   * \brief Define destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~LocalServerManagerInterface() noexcept = default;

  LocalServerManagerInterface(LocalServerManagerInterface const&) = delete;
  LocalServerManagerInterface(LocalServerManagerInterface&&) = delete;
  LocalServerManagerInterface& operator=(LocalServerManagerInterface const&) & = delete;
  LocalServerManagerInterface& operator=(LocalServerManagerInterface&&) & = delete;

  /*!
   * \brief     Returns ownership of a local server impl to the local server manager.
   *
   * \param[in] service_instance_id    The service instance id.
   * \param[in] local_server_impl      Implementation object to return memory ownership to the manager.
   *
   * \pre       -
   *
   * \context   App
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void Release(ProvidedServiceInstanceId const& service_instance_id,
                       std::unique_ptr<LocalServerInterface> local_server_impl) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_MANAGER_INTERFACE_H_
