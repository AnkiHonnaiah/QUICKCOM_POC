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
 *         \brief  A class defining the provided instance provider the factory shall use.
 *
 *       \details  For the default variant the SkeletonXfFactory is selected, for the binding_if_legacy variant the
 *                 ServiceDiscoverySkeletonXf is selected.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROVIDED_SERVICE_INSTANCE_PROVIDER_TYPE_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROVIDED_SERVICE_INSTANCE_PROVIDER_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/zero_copy_binding/transformation_layer/internal/skeleton_xf_factory.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief Alias declaration for the provided service instance provider to be used on the default binding interface.
 */
template <typename SkeletonXf, typename Skeleton>
using ProvidedInstanceProviderType = SkeletonXfFactory<SkeletonXf, Skeleton>;

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROVIDED_SERVICE_INSTANCE_PROVIDER_TYPE_H_
