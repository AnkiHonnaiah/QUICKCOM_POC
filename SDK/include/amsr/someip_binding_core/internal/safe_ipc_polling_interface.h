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
/*!        \file  someip_binding_core/internal/safe_ipc_polling_interface.h
 *        \brief  ara::com SOME/IP Binding Safe Ipc Polling Interface
 *
 *      \details  Interface of "SafeIpcPollingWrapper" or "DummyPollingWrapper" class
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SAFE_IPC_POLLING_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SAFE_IPC_POLLING_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class contains reactor polling function to be called while waiting for a command response (only in
 *        Polling Mode).
 */
class SafeIpcPollingInterface {
 public:
  /*!
   * \brief Default constructor.
   */
  SafeIpcPollingInterface() = default;

  /*!
   * \brief Default destructor.
   */
  virtual ~SafeIpcPollingInterface() = default;

  SafeIpcPollingInterface(SafeIpcPollingInterface const&) = delete;
  SafeIpcPollingInterface(SafeIpcPollingInterface&&) = delete;
  SafeIpcPollingInterface& operator=(SafeIpcPollingInterface const&) & = delete;
  SafeIpcPollingInterface& operator=(SafeIpcPollingInterface&&) & = delete;

  /*!
   * \brief       Reactor polling function to be called while waiting for
   *              a command response (only in Polling Mode).
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate    -
   * \synchronous TRUE
   * \steady      TRUE
   */
  virtual void Poll() noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SAFE_IPC_POLLING_INTERFACE_H_
