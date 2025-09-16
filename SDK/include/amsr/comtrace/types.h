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
 *        \brief  Types of the comtrace component.
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TYPES_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "ara/core/error_code.h"
#include "ara/core/span.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace comtrace {

/*!
 * \brief Immutable buffer view type (reference/pointer and size).
 *
 * \vpublic
 */
using ImmutableBufferView = ::ara::core::Span<std::uint8_t const>;

/*!
 * \brief Immutable views to immutable buffer views (I/O vector).
 *
 * \vpublic
 */
using ImmutableBufferViews = ::ara::core::Span<ImmutableBufferView const>;

/*!
 * \brief File descriptor type for registration of memory resources.
 *
 * \vpublic
 */
using FileDescriptor = std::int32_t;

/*!
 * \brief Context ID / handle for a ongoing asynchronous trace operation.
 *
 * \vpublic
 */
using ContextId = std::uint32_t;

/*!
 * \brief Extension point generated unique ID for every registered ara::com trace client.
 *
 * \vpublic
 */
using ClientId = std::uint16_t;

/*!
 * \brief Extension point generated unique ID for every registered ara::com memory resource.
 *
 * \vpublic
 */
using MemoryResourceId = std::uint32_t;

/*!
 * \brief String view type alias.
 *
 * \vpublic
 */
using StringView = ::ara::core::StringView;

/*!
 * \brief Type-alias for ErrorCode.
 */
using ErrorCode = ::ara::core::ErrorCode;

}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_TYPES_H_
