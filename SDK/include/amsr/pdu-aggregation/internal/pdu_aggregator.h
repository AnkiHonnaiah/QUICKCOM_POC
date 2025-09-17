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
/*!        \file  pdu_aggregator.h
 *        \brief  Null PDU aggregation class.
 *
 *********************************************************************************************************************/

#ifndef LIB_NULL_PDU_AGGREGATION_INCLUDE_AMSR_PDU_AGGREGATION_INTERNAL_PDU_AGGREGATOR_H_
#define LIB_NULL_PDU_AGGREGATION_INCLUDE_AMSR_PDU_AGGREGATION_INTERNAL_PDU_AGGREGATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/pdu-aggregation/internal/pdu_aggregator_interface.h"
#include "ara/core/result.h"
#include "osabstraction/io/io_buffer.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace pdu_aggregation {
namespace internal {

/*!
 * \brief PDU aggregation class.
 * \details This is a dummy PduAggregator to be used when no other PduAggregation library is linked.
 */
class PduAggregator : public PduAggregatorInterface {
 public:
  /*!
   * \copydoc amsr::pdu_aggregation::internal::PduAggregatorInterface::AggregatePdu()
   * \details This is a dummy AggregatePdu method that moves back the same pointer which is passed as an argument.
   * \internal
   * - Move the passed pdu pointer back to the caller.
   * \steady TRUE
   */
  inline ara::core::Result<DataBufferPtr, AggregationError> AggregatePdu(
      DataBufferPtr&& uniquePduPointer) noexcept override {
    ara::core::Result<DataBufferPtr, AggregationError> result{std::move(uniquePduPointer)};

    return result;
  };
};

}  // namespace internal
}  // namespace pdu_aggregation
}  // namespace amsr

#endif  // LIB_NULL_PDU_AGGREGATION_INCLUDE_AMSR_PDU_AGGREGATION_INTERNAL_PDU_AGGREGATOR_H_
