/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief Indirection of ServerBuilder.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_LOGIC_SERVER_BUILDER_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_LOGIC_SERVER_BUILDER_H_

#include "amsr/zero_copy_common/logic/internal/server_builder.h"

namespace amsr {
namespace zero_copy_common {
namespace external {
namespace logic {

/*!
 * \brief Indirection to access ServerBuilder.
 */
using ServerBuilder = ::amsr::zero_copy_common::logic::internal::ServerBuilder;

}  // namespace logic
}  // namespace external
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_LOGIC_SERVER_BUILDER_H_
