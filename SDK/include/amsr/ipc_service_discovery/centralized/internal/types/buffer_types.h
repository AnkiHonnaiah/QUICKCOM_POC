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
/*! \file
 *  \brief Common types of buffers for SafeIpc.
 *  \unit IpcServiceDiscovery::Centralized::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_BUFFER_TYPES_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_BUFFER_TYPES_H_

#include "ara/core/span.h"
#include "ara/log/logger.h"
#include "osabstraction/io/io_buffer.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Type for the elements in a buffer.
 */
using BufferElementType = std::uint8_t;

/*!
 * \brief View to a const buffer.
 */
using ConstBufferView = ::ara::core::Span<BufferElementType const>;

/*!
 * \brief Const view to a const buffer.
 */
using ConstBufferConstView = ::ara::core::Span<BufferElementType const> const;

/*!
 * \brief View to a mutable buffer.
 */
using MutableBufferView = ::ara::core::Span<BufferElementType>;

/*!
 * \brief Const view to a mutable buffer.
 */
using MutableBufferConstView = ::ara::core::Span<BufferElementType> const;

/*!
 * \brief Container for a single const IO buffer.
 */
using ConstIOBufferContainer = std::array<osabstraction::io::ConstIOBuffer, 1u>;

/*!
 * \brief View to a const IO buffer container.
 */
using ConstIOBufferView = ::ara::core::Span<osabstraction::io::ConstIOBuffer>;

/*!
 * \brief Container for a single mutable IO buffers.
 */
using MutableIOBufferContainer = std::array<osabstraction::io::MutableIOBuffer, 1u>;

/*!
 * \brief View to a mutable IO buffer container.
 */
using MutableIOBufferView = ::ara::core::Span<osabstraction::io::MutableIOBuffer>;

/*!
 * \brief Log streaming operator for type ConstBufferView.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] buffer_view A reference to the ConstBufferConstView.
 * \return The reference to the LogStream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
inline auto operator<<(::ara::log::LogStream& log_stream,
                       ::amsr::ipc_service_discovery::centralized::internal::ConstBufferConstView& buffer_view
                       ) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "BufferView: ";
  log_stream << "[ ";
  for (BufferElementType const buffer_element : buffer_view) {
    log_stream << "0x" << ::ara::log::HexFormat(buffer_element) << " ";
  }
  return log_stream << "]";
}
}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_TYPES_BUFFER_TYPES_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
