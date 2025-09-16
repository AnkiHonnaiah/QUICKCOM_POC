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
/**        \file  generic_data_type.h
 *        \brief  Type definitions for the generic data.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_DATA_TYPE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_DATA_TYPE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "ara/core/span.h"

namespace amsr {
namespace socal {

/*!
 * \brief   A view over a contiguous sequence of serialized byte stream.
 * \details This is just a view over existing memory location, so the underlying memory must be valid at least until
 *          this view is destroyed.
 * \vpublic
 */
using GenericDataFragment = ::ara::core::Span<std::uint8_t const>;

/*!
 * \brief   Collection of one or more GenericDataFragment(s).
 * \details Depending on the memory management schema, the serialized byte stream can be provided in a single or
 *          multiple contiguous memory fragment(s).
 * \vpublic
 */
using GenericDataType = ::ara::core::Span<GenericDataFragment const>;

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_DATA_TYPE_H_
