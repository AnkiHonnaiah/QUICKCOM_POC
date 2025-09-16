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
 *        \brief  Declaration of the client-side Zero-Copy Common interface.
 *         \unit  ZeroCopyCommon::MemCon
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_CLIENT_H_
#define LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_CLIENT_H_

// TODO: unit

#include "amsr/zero_copy_common/common/zero_copy_common_error_domain.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_builder.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_builder_interface.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_interface.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_state.h"

namespace amsr {
namespace zero_copy_common {
namespace internal {

/*!
 * \brief Enumeration for all error code values of ZeroCopyCommon.
 */
using ZeroCopyCommonErrc = common::ZeroCopyCommonErrc;

/*!
 * \brief Interface of a client.
 */
using ClientInterface = memcon::internal::client::ClientInterface;

/*!
 * \brief Interface of a client builder.
 */
using ClientBuilderInterface = memcon::internal::client::ClientBuilderInterface;

/*!
 * \brief Implementation of ClientBuilderInterface.
 */
using ClientBuilder = memcon::internal::client::ClientBuilder;

}  // namespace internal
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_COMMON_INCLUDE_AMSR_ZERO_COPY_COMMON_INTERNAL_ZERO_COPY_COMMON_CLIENT_H_
