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
/*!        \file  pdu_aggregator_interface.h
 *        \brief  PDU aggregation interface class.
 *
 *********************************************************************************************************************/

#ifndef LIB_NULL_PDU_AGGREGATION_INCLUDE_AMSR_PDU_AGGREGATION_INTERNAL_PDU_AGGREGATOR_INTERFACE_H_
#define LIB_NULL_PDU_AGGREGATION_INCLUDE_AMSR_PDU_AGGREGATION_INTERNAL_PDU_AGGREGATOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "ara/core/result.h"
#include "osabstraction/io/io_buffer.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace pdu_aggregation {
namespace internal {

/*!
 * \brief Aggregation error codes
 */
enum class AggregationError : std::uint8_t { kAggregationNotComplete, kNotOk };

/*!
 * \brief PDU aggregation class.
 */
class PduAggregatorInterface {
 public:
  /*!
   * \brief Constructor
   * \steady TRUE
   */
  inline PduAggregatorInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \steady TRUE
   */
  virtual ~PduAggregatorInterface() noexcept = default;

  PduAggregatorInterface(PduAggregatorInterface&&) = delete;
  PduAggregatorInterface& operator=(PduAggregatorInterface const&) & = delete;
  PduAggregatorInterface& operator=(PduAggregatorInterface&&) & = delete;

  /*!
   * \brief A data buffer type.
   */
  using DataBufferPtr = vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief Process received PDU
   * \param uniquePduPointer A pointer to the PDU to be processed
   * \return A pointer to the result aggregated PDU or an AggregationError
   * \steady TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SomeIpDaemon_M9.3.3_vfunction_cannot_be_declared_const
  virtual ara::core::Result<DataBufferPtr, AggregationError> AggregatePdu(
      DataBufferPtr&& uniquePduPointer) noexcept = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \steady FALSE
   */
  PduAggregatorInterface(PduAggregatorInterface const&) = default;
};

}  // namespace internal
}  // namespace pdu_aggregation
}  // namespace amsr

#endif  // LIB_NULL_PDU_AGGREGATION_INCLUDE_AMSR_PDU_AGGREGATION_INTERNAL_PDU_AGGREGATOR_INTERFACE_H_
