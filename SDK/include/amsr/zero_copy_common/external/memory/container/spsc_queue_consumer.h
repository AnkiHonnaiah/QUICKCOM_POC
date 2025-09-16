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
 *        \brief Indirection of SPSCQueueConsumer.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_MEMORY_CONTAINER_SPSC_QUEUE_CONSUMER_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_MEMORY_CONTAINER_SPSC_QUEUE_CONSUMER_H_

#include "amsr/memory/container/spsc_queue_consumer.h"

namespace amsr {
namespace zero_copy_common {
namespace external {
namespace memory {
namespace container {

/*!
 * \brief Indirection to access SPSCQueueConsumer.
 */
template <typename T>
using SPSCQueueConsumer = ::amsr::memory::container::SPSCQueueConsumer<T>;

}  // namespace container
}  // namespace memory
}  // namespace external
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_EXTERNAL_INCLUDE_AMSR_ZERO_COPY_COMMON_EXTERNAL_MEMORY_CONTAINER_SPSC_QUEUE_CONSUMER_H_
