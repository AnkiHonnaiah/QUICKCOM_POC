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
 *         \brief  Class for analyzing meta data of received slots and for validating metadata.
 *
 *       \details
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_SLOT_ANALYZER_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_SLOT_ANALYZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstddef>
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/generic_error_domain.h"
#include "amsr/zero_copy_binding/core/internal/slot_content.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief    Represents analysis result.
 * \unit
 * \vprivate Component private
 */
struct SlotAnalysis {
  std::uint64_t dropped_messages{0U};
};

/*!
 * \brief    Analyses slots for their contained meta data.
 *
 * \unit
 * \vprivate Component private
 */
class SlotAnalyzer final {
  /*!
   * \brief ErrorCode for the slot analysis.
   */
  using Errc = ::amsr::generic::GenErrc;

 public:
  /*!
   * \brief         Analyze the given SlotContent.
   * \details       Detects any dropped messages if the previous call to Analyze() contained a sequence counter
   *                which was smaller by more than one.
   * \param[in]     slot_content The SlotContent to analyze.
   * \return        A result containing the SlotAnalysis or an error.
   * \error         GenErrc::kProtocolInvalidParameter The meta data contains a timestamp or a sequence counter
   *                                                   which is smaller than the previously analyzed slot.
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component Private
   *
   * \internal
   * - If slot contains valid meta data:
   * -   Count any lost messages by comparing sequence counter to previous one.
   * -   Emplace SlotAnalysis.
   * -   Save meta data for next analysis.
   * - Else:
   * -   Emplace kProtocolInvalidParameter.
   * - Return result.
   * \endinternal
   */
  auto Analyze(SlotContentHeader const& slot_content_header) noexcept -> ::amsr::core::Result<SlotAnalysis> {
    auto analysis_result(::amsr::core::Result<SlotAnalysis>::FromError(
        Errc::kProtocolInvalidParameter,
        ::amsr::core::StringView{"Slot metadata contains either invalid timestamp or invalid sequence counter."}));
    MetaData const meta_data{slot_content_header.meta_data};
    MetaData::SequenceCounter const current_sequence_counter{slot_content_header.meta_data.sequence_counter};

    if ((meta_data.time_stamp >= previous_meta_data_.time_stamp) &&
        (current_sequence_counter > previous_meta_data_.sequence_counter)) {
      MetaData::SequenceCounter const sequence_diff{current_sequence_counter - previous_meta_data_.sequence_counter};
      SlotAnalysis slot_analysis{};
      if (sequence_diff > 1U) {
        slot_analysis.dropped_messages = sequence_diff - 1U;
      }
      analysis_result.EmplaceValue(slot_analysis);
      previous_meta_data_ = slot_content_header.meta_data;
    }
    return analysis_result;
  }

 private:
  /*!
   * \brief The previously analyzed meta data.
   */
  MetaData previous_meta_data_{std::chrono::steady_clock::now(), 0U};
};
}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_SLOT_ANALYZER_H_
