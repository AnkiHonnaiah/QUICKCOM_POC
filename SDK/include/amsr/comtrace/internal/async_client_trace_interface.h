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
 *        \brief  Callback interface for ara::com internal trace clients using asynchronous trace callouts.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_ASYNC_CLIENT_TRACE_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_ASYNC_CLIENT_TRACE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/comtrace/internal/client_types.h"

namespace amsr {
namespace comtrace {
namespace internal {

/*!
 * \brief Callback interface for ara::com internal trace clients using asynchronous trace callouts.
 *
 * \unit ComTrace::ClientTracer
 */
class AsyncClientTraceInterface {
 public:
  AsyncClientTraceInterface(AsyncClientTraceInterface const&) = delete;
  AsyncClientTraceInterface(AsyncClientTraceInterface&&) = delete;
  auto operator=(AsyncClientTraceInterface const&) & -> AsyncClientTraceInterface& = delete;
  auto operator=(AsyncClientTraceInterface&&) & -> AsyncClientTraceInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~AsyncClientTraceInterface() noexcept = default;

  // ---- Asynchronous trace operation handling ----

  /*!
   * \brief Callback to notify the ara::com trace client about completion of an ongoing asynchronous trace operation.
   *
   * \param[in] client_context_id  The trace client Context ID of the asynchronous trace operation. Pass only context
   *                               IDs previously provided with any asynchronous trace point callout.
   *
   * \pre         \p context_id must contain a context ID previously provided with any trace callout function.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  virtual void OnTraceCompleted(ClientContextId client_context_id) noexcept = 0;

 protected:
  /*!
   * \brief Default constructor of the AsyncClientTraceInterface.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  AsyncClientTraceInterface() noexcept = default;
};

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_ASYNC_CLIENT_TRACE_INTERFACE_H_
