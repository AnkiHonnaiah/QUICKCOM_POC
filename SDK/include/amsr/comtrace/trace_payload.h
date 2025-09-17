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
 *        \brief  Trace payload.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_PAYLOAD_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_PAYLOAD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/optional.h"

#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {

/*!
 * \brief   Views on traced payload buffers.
 * \details Traced payload might be separated into multiple data chunks / pages. Therefore a view of views to the trace
 *          payload is provided. Also known as "I/O Vector" or "vectored I/O".
 *
 * \unit ComTrace::TraceData
 */
class TracePayload final {
 public:
  /*!
   * \brief Descriptor for payloads that are bound to a memory resource.
   */
  struct MemoryResourceDescriptor {
    /*!
     * \brief The ID of the memory resource.
     */
    MemoryResourceId id{0};

    /*!
     * \brief A view to the memory resource beginning at the start of the memory region and ending at the end.
     */
    ImmutableBufferView view{};
  };

  // ---- Construction / Destruction ----

  /*!
   * \brief Construct a new TracePayload.
   *
   * \param[in] data                        Views to buffer views where the serialized payload is located.
   * \param[in] memory_resource_descriptor  An optional MemoryResourceDescriptor containing information regarding the
   *                                        memory resource the payload resides in.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  explicit TracePayload(ImmutableBufferViews data,
                        ::ara::core::Optional<MemoryResourceDescriptor> memory_resource_descriptor = {}) noexcept;

  /*!
   * \brief Default copy-constructor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  TracePayload(TracePayload const&) noexcept = default;

  /*!
   * \brief Default copy-assignment operator.
   *
   * \return Reference of the copy-assigned instance.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto operator=(TracePayload const&) & noexcept -> TracePayload& = default;

  /*!
   * \brief Default move-constructor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  TracePayload(TracePayload&&) noexcept = default;

  /*!
   * \brief Default move-assignment operator.
   *
   * \return Reference of the moved-assigned instance.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  auto operator=(TracePayload&&) & noexcept -> TracePayload& = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  ~TracePayload() noexcept = default;

  // ---- Public APIs ----

  /*!
   * \brief Get the views of buffer views to the serialized payload.
   *
   * \return Views of buffer views to the serialized payload. Attention: The returned views might only be valid in a
   *         limited scope.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  auto Data() const noexcept -> ImmutableBufferViews;

  /*!
   * \brief Get the optional memory resource descriptor of the payload.
   *
   * \return An optional containing a MemoryResourceDescriptor if it has been set for that payload.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady      TRUE
   */
  auto GetMemoryResourceDescriptor() const noexcept -> ::ara::core::Optional<MemoryResourceDescriptor>;

 private:
  /*!
   * \brief The views of buffer views to the serialized payload (I/O Vector).
   */
  ImmutableBufferViews data_;

  /*!
   * \brief An optional descriptor for payloads that are bound to memory resources.
   */
  ::ara::core::Optional<MemoryResourceDescriptor> memory_resource_descriptor_{};
};

}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TRACE_PAYLOAD_H_
