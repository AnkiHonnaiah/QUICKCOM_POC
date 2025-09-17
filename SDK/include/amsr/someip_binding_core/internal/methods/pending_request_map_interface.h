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
/*!        \file  someip_binding_core/internal/methods/pending_request_map_interface.h
 *        \brief  Method request storage
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::Methods::PendingRequestMap
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/optional.h"
#include "ara/core/promise.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Indirection interface for pending request map.
 * \tparam Output Response result value (wrapped in a Promise) to store a positive response
 */
template <typename Output>
class PendingRequestMapInterface {
 public:
  /*!
   * \brief Value of the map. Objects of this type are stored for each method request in the map.
   */
  using ResponseResultPromise = ::ara::core::Promise<Output>;

  /*!
   * \brief Optional of the response promise type.
   */
  using ResponsePromiseOptional = ::amsr::core::Optional<ResponseResultPromise>;

  /*!
   * \brief Key for identifying stored method requests.
   */
  using RequestKey = ::amsr::someip_protocol::internal::SessionId;

  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  PendingRequestMapInterface() = default;

  PendingRequestMapInterface(PendingRequestMapInterface const&) = delete;
  PendingRequestMapInterface(PendingRequestMapInterface&&) = delete;
  PendingRequestMapInterface& operator=(PendingRequestMapInterface const&) & = delete;
  PendingRequestMapInterface& operator=(PendingRequestMapInterface&&) & = delete;

 protected:
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  ~PendingRequestMapInterface() = default;

 public:
  /*!
   * \brief Fetch a request by the given request key and remove the request entry from the wrapped map.
   *
   * \param[in] request_key Key to fetch the request entry
   * \return An optional of promise. If a request for the passed key is available, it will be stored in the optional
   * to return. If there is no request entry available for the given request key, the returned optional will not
   * have a value.
   *
   * \pre              -
   * \context          App (method cancellation, send error), Reactor (response)
   * \threadsafe       TRUE
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOMEIPBINDING_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_FPBS
  virtual ResponsePromiseOptional MoveOutRequest(RequestKey const request_key) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_INTERFACE_H_
