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
/*!        \file  someip_binding_core/internal/dummy_polling_wrapper.h
 *        \brief  Wrapper class for runtime event driven mode.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SafeIpcPollingWrapper
 *
 *      \details  This wrapper class contains dummy polling function for event driven mode.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_DUMMY_POLLING_WRAPPER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_DUMMY_POLLING_WRAPPER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Class contains dummy polling function to be called while waiting for a command response (only in
 *        event driven mode).
 *
 */
class DummyPollingWrapper final : public SafeIpcPollingInterface {
 public:
  /*!
   * \brief Default constructor.
   *
   */
  DummyPollingWrapper() = default;

  /*!
   * \brief Default destructor.
   *
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~DummyPollingWrapper() final = default;

  DummyPollingWrapper(DummyPollingWrapper const&) = delete;
  DummyPollingWrapper(DummyPollingWrapper&&) = delete;
  DummyPollingWrapper& operator=(DummyPollingWrapper const&) & = delete;
  DummyPollingWrapper& operator=(DummyPollingWrapper&&) & = delete;

  /*!
   * \brief                  Dummy polling function for runtime event driven mode.
   * \pre                    -
   * \context                App
   * \threadsafe             FALSE
   * \reentrant              FALSE
   * \vprivate               -
   * \synchronous            TRUE
   * \internal steady TRUE \endinternal
   */
  void Poll() noexcept final;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_DUMMY_POLLING_WRAPPER_H_
