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
 *        \brief  Utility for encoding / decoding of context IDs.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONTEXT_ID_UTIL_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONTEXT_ID_UTIL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/comtrace/internal/client_types.h"
#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {
namespace internal {

/*!
 * \brief   Utility for encoding / decoding of context IDs.
 * \details Layout of the context ID:
 *          <pre>
 *          +----------------(32bit)---------------------------------------+
 *          | Client ID (16bit) | Trace client specific context ID (16bit) |
 *          +--------------------------------------------------------------+
 *          </pre>
 *
 * \unit ComTrace::ContextIdUtil
 */
class ContextIdUtil final {
 public:
  // ---- Construction -------------------------------------------------------------------------------------------------

  ContextIdUtil() noexcept = delete;

  ContextIdUtil(ContextIdUtil const&) noexcept = delete;
  auto operator=(ContextIdUtil const&) & noexcept -> ContextIdUtil& = delete;
  ContextIdUtil(ContextIdUtil&&) noexcept = delete;
  auto operator=(ContextIdUtil&&) & noexcept -> ContextIdUtil& = delete;

  /*!
   * \brief Destroy the custom tracer lifecycle manager.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ContextIdUtil() noexcept = default;

  // ---- Static APIs --------------------------------------------------------------------------------------------------

  /*!
   * \brief Decode the ara::com internal trace client ID from a Context ID.
   *
   * \param[in] context_id  The context ID to be decoded.
   *
   * \return Decoded internal client ID.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static auto DecodeInternalClientId(ContextId context_id) noexcept -> InternalClientId;

  /*!
   * \brief Decode the trace client specific context ID from a Context ID.
   *
   * \param[in] context_id  The context ID to be decoded.
   *
   * \return Decoded client context ID.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static auto DecodeClientContextId(ContextId context_id) noexcept -> ClientContextId;

  /*!
   * \brief Encode a ContextId.
   *
   * \param[in] client_id          The trace client ID.
   * \param[in] client_context_id  The trace client context ID.
   *
   * \return Encoded context ID.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static auto EncodeContextId(InternalClientId client_id, ClientContextId client_context_id) noexcept -> ContextId;
};

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONTEXT_ID_UTIL_H_
