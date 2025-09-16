/**********************************************************************************************************************
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
/*!        \file  json_writer.h
 *        \brief  Writer for serialization of DataContainer objects to JSON data.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_WRITER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/data/data_container.h"
#include "crypto/common/data/data_writer.h"

namespace crypto {
namespace common {
namespace data {

/*!
 * \brief Writer for serialization of DataContainer objects to JSON data
 * \details We use the JSON data format to persist data. This class provides functions to convert the internal data
 * format (DataContainer) into a JSON string.
 * \vprivate Product Private
 */
class JsonWriter : public DataWriter {
 public:
  /*!
   * \brief Maximum length for JSON files during write operations
   */
  static constexpr std::uint64_t kMaximumJsonFileSize{10000};

  /*!
   * \brief StringOutputBuffer type
   */
  using StringOutputBuffer = std::array<char, kMaximumJsonFileSize>;

  /*!
   * \brief Default constructor
   * \vprivate Product Private
   */
  JsonWriter() = default;

  /*!
   *  \brief Default destructor
   *  \vprivate Product Private
   */
  ~JsonWriter() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Product Private
   */
  JsonWriter(JsonWriter const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Product Private
   */
  JsonWriter(JsonWriter&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default copy assignment operator
   * \vprivate Product Private
   */
  JsonWriter& operator=(JsonWriter const& /*other*/) & = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default move assignment operator
   * \vprivate Product Private
   */
  JsonWriter& operator=(JsonWriter&& /*other*/) & noexcept = default;

  /*!
   * \brief Write the data container as JSON to the output buffer.
   * \param[in] container Container to be serialized
   * \param[out] buffer buffer to write the serialized data to
   * \return Subspan of the input buffer that was used in the serialization
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the container is invalid
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity If the buffer is to small
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto WriteDataContainer(::crypto::common::data::DataContainer const& container,
                          ara::crypto::WritableMemRegion buffer) const noexcept
      -> amsr::core::Result<ara::crypto::ReadOnlyMemRegion> final;

  /*!
   * \brief Write the data container as JSON to the output buffer.
   * \param[in] container Container to be serialized.
   * \param[out] output_buffer buffer to write the JSON string data to.
   * \return StringView onto the resulting JSON string inside the output_buffer or empty StringView in case an error
   * occurred
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static auto DataContainerToJsonString(::crypto::common::data::DataContainer const& container,
                                        StringOutputBuffer& output_buffer) noexcept -> amsr::core::StringView;

  /*!
   * \brief Calculates the size of the container serialized to JSON.
   * \param[in] container to be serialized.
   * \return size of the JSON or 0 in case an error occurred
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto CalculateSerializedSize(::crypto::common::data::DataContainer const& container) const noexcept
      -> std::size_t final;
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_WRITER_H_
