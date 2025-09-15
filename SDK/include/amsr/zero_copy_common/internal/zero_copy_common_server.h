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
 *        \brief  Declaration of the server-side Zero-Copy Common interface.
 *         \unit  ZeroCopyCommon::MemCon
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_SERVER_H_
#define LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_SERVER_H_

// TODO: unit

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_id.h"
#include "amsr/zero_copy_common/memcon/internal/server/receiver_state.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_builder.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_builder_interface.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_interface.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_state.h"

namespace amsr {
namespace zero_copy_common {
namespace internal {

/*!
 * \brief Enumeration for all error code values of ZeroCopyCommon.
 */
using ZeroCopyCommonErrc = common::ZeroCopyCommonErrc;

/*!
 * \brief Interface of a server.
 */
using ServerInterface = memcon::internal::server::ServerInterface;

/*!
 * \brief Interface of a server builder.
 */
using ServerBuilderInterface = memcon::internal::server::ServerBuilderInterface;

/*!
 * \brief Implementation of ServerBuilderInterface.
 */
using ServerBuilder = memcon::internal::server::ServerBuilder;

}  // namespace internal
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_SERVER_H_
