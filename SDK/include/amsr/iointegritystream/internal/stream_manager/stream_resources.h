/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 *                Informatik GmbH. Vector Informatik GmbH grants to you only those rights as
 *                set out in the license conditions. All other rights remain with Vector
 *                Informatik GmbH.
 * \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  stream_resources.h
 *        \brief
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_STREAM_RESOURCES_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_STREAM_RESOURCES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/variant.h"

#include "amsr/iointegritystream/internal/stream/input_integrity_stream.h"
#include "amsr/iointegritystream/internal/stream/output_integrity_stream.h"
#include "amsr/iostream/filestream/input_file_stream.h"
#include "amsr/iostream/filestream/input_output_file_stream.h"
#include "amsr/iostream/stringstream/input_string_stream.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream_manager {

/*!
 * \brief Alias.
 */
using InputStringStream = amsr::stream::stringstream::InputStringStream;

/*!
 * \brief Alias.
 */
using InputOutputFileStream = amsr::stream::filestream::InputOutputFileStream;

/*!
 * \brief The stream variants that iointegritystream supports.
 */
using StreamVariant = amsr::core::Variant<amsr::core::monostate, InputStringStream, InputOutputFileStream,
                                          stream::InputIntegrityStream, stream::OutputIntegrityStream>;

// VECTOR NC VectorC++-V11.0.1: MD_IoIntegrityStream_V11.0.1_struct_access_modifier
// VECTOR NC VectorC++-V11.0.3: MD_IoIntegrityStream_V11.0.3_struct_public_members
/*!
 * \brief StreamResources that hold resources for any integrity stream.
 */
struct StreamResources {  // VCA_IOIS_VALID_PARAMETERS
  /*!
   * \brief Stream buffer used for pre validation.
   */
  ara::core::Vector<char> buffer{};

  /*!
   * \brief Internally created cache buffer if user has not provided one.
   */
  ara::core::Vector<vac::language::byte> cache_buffer_internal{};

  /*!
   * \brief Raw file stream.
   */
  std::unique_ptr<InputOutputFileStream> raw_fs{nullptr};

  /*!
   * \brief An integrity provider.
   */
  std::unique_ptr<stream::IntegrityProvider> integrity_provider{nullptr};

  /*!
   * \brief Factory class to create StreamResources.
   */
  class Factory;

 private:
  /*!
   * \brief Private constructor to be used by factory.
   */
  StreamResources() noexcept {};
};

/*!
 * \brief Factory class to create StreamResources.
 */
class StreamResources::Factory final {
 public:
  /*!
   * \brief Create resources for pre validation.
   * \return An StreamResources instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static std::unique_ptr<StreamResources> PreValCreate() noexcept {
    // VECTOR NL AutosarC++17_10-A18.5.2: MD_IoIntegrityStream_A18.5.2_instance_factory
    return std::unique_ptr<StreamResources>(new StreamResources());
  };

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_IoIntegrityStream_A15.4.2_noexcept_function_without_throw
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_IoIntegrityStream_A15.5.3_noexcept_function_without_throw
  /*!
   * \brief Create resources for post validation.
   * \param[in] validation_data Reference to validation file data.
   * \param[in] cache_buffer User provided cache buffer.
   * \return An StreamResources instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  static amsr::core::Result<std::unique_ptr<StreamResources>> PostValCreate(
      ValidationFileData& validation_data,
      amsr::core::Optional<amsr::core::Span<vac::language::byte>> cache_buffer = amsr::core::nullopt) noexcept {
    amsr::core::Result<std::unique_ptr<StreamResources>> result{std::unique_ptr<StreamResources>(
        new StreamResources())};  // VECTOR SL AutosarC++17_10-A18.5.2: MD_IoIntegrityStream_A18.5.2_instance_factory
    amsr::core::Span<vac::language::byte> buffer_span;
    if (cache_buffer.has_value()) {
      if (cache_buffer->size() < validation_data.file_info.block_length) {
        result.EmplaceError(MakeErrorCode(amsr::stream::StreamErrc::kInsufficientResources,
                                          "Cache buffer cannot be smaller than a single block."));
      } else {
        buffer_span = cache_buffer.value();
      }
    } else {
      // VECTOR NC AutosarC++17_10-A15.4.2: MD_IoIntegrityStream_A15.4.2_noexcept_function_without_throw
      // VECTOR NC AutosarC++17_10-A15.5.3: MD_IoIntegrityStream_A15.5.3_noexcept_function_without_throw
      result.Value()->cache_buffer_internal.resize(         // VCA_IOIS_FUNC_CONTRACT
          validation_data.file_info.block_length);          // VCA_IOIS_VALID_PARAMETERS
      buffer_span = result.Value()->cache_buffer_internal;  // VCA_IOIS_VALID_OBJECT
    }

    if (!result.CheckError(amsr::stream::StreamErrc::kInsufficientResources)) {
      result.Value()->raw_fs =  // VCA_IOIS_VALID_PARAMETERS, VCA_IOIS_VALID_OBJECT
          std::make_unique<InputOutputFileStream>();

      result.Value()->integrity_provider =                                        // VCA_IOIS_VALID_OBJECT
          std::make_unique<stream::IntegrityProvider>(validation_data.file_info,  // VCA_IOIS_VALID_PARAMETERS
                                                      *result.Value()->raw_fs, buffer_span);
    }

    return result;
  };
};

/*!
 * \brief Stream token that contains integrity stream and its dependent resources.
 */
struct IntegrityStreamToken {  // VCA_IOIS_VALID_PARAMETERS
  /*!
   * \brief Stream resources.
   */
  std::unique_ptr<StreamResources> resources{nullptr};

  /*!
   * \brief Variant holds the integrity stream.
   */
  StreamVariant stream{amsr::core::monostate()};

  /*!
   * \brief True if stream was opened read-only, only needed for InputOutputFileStream
   * \detail Using the StreamVariant class assumes that all cases of read, read/write, and write with different
   *         validation strategies result in separate stream types in the variant so the visitor classes know
   *         that to do. But currently this is not true; InputOutputFileStream is used for two different cases,
   *         so we need an extra bool for the visitors to identify what to do.
   *         TODO: This bool shold be removed when VIK-2720 is implemented.
   */
  bool is_read_only{true};
};

}  // namespace stream_manager
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_MANAGER_STREAM_RESOURCES_H_
