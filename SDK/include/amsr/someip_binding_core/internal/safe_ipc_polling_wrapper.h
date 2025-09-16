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
/*!        \file  someip_binding_core/internal/safe_ipc_polling_wrapper.h
 *        \brief  Wrapper class for runtime polling mode.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SafeIpcPollingWrapper
 *
 *      \details  This wrapper class contains reactor polling function to be called while waiting for a command response
 *                (only in Polling Mode).
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SAFE_IPC_POLLING_WRAPPER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SAFE_IPC_POLLING_WRAPPER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "ara/core/memory_resource.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief  Class contains reactor polling function to be called while waiting for a command response (only in
 *        Polling Mode).
 *
 */
class SafeIpcPollingWrapper final : public SafeIpcPollingInterface {
 public:
  /*!
   * \brief Type alias for polling function.
   */
  using PollingFunction = vac::language::UniqueFunction<void(), ara::core::PolymorphicAllocator<std::uint8_t>>;

  /*!
   * \brief Default destructor.
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  ~SafeIpcPollingWrapper() final = default;

  SafeIpcPollingWrapper() = delete;
  SafeIpcPollingWrapper(SafeIpcPollingWrapper const&) = delete;
  SafeIpcPollingWrapper(SafeIpcPollingWrapper&&) = delete;
  SafeIpcPollingWrapper& operator=(SafeIpcPollingWrapper const&) & = delete;
  SafeIpcPollingWrapper& operator=(SafeIpcPollingWrapper&&) & = delete;

  /*!
   * \brief Explicit constructor.
   * \param[in] polling_function Reactor polling function to be called while waiting for a command response (only in
   *                             Polling Mode).
   *
   * \pre        -
   * \context    Init
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  explicit SafeIpcPollingWrapper(PollingFunction&& polling_function) noexcept;

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
  void Poll() noexcept final;

 private:
  /*!
   * \brief Reactor polling function to be called while waiting for a command response (only in Polling Mode).
   */
  PollingFunction polling_function_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SAFE_IPC_POLLING_WRAPPER_H_
