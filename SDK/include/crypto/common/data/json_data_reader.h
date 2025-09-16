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
/*!        \file  json_data_reader.h
 *        \brief  JSON Data reader able to fill internal data model.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_DATA_READER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_DATA_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "crypto/common/data/data_container.h"
#include "crypto/common/data/data_reader.h"
#include "crypto/common/util/logger.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace common {
namespace data {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

// VECTOR NC AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
/*!
 * \brief JSON Data reader able to fill internal data model (DataContainer).
 * \details We use the JSON data format to persist data. This class provides functions to convert JSON data into the
 * internal data model.
 * \vprivate Product Private
 */
class JSONDataReader : public DataReader {
 private:
  crypto::common::util::Logger logger_; /*!< Logging instance */

 public:
  /*!
   * \brief Constructor.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  JSONDataReader() : DataReader{}, logger_{util::kCommonLoggingContext, ""_sv} {};

  /*!
   * \brief Default copy constructor
   * \vprivate Product Private
   */
  JSONDataReader(JSONDataReader const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Product Private
   */
  JSONDataReader(JSONDataReader&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default copy assignment operator
   * \vprivate Product Private
   */
  JSONDataReader& operator=(JSONDataReader const& /*other*/) & = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default move assignment operator
   * \vprivate Product Private
   */
  JSONDataReader& operator=(JSONDataReader&& /*other*/) & noexcept = default;

  /*!
   * \brief Destructor.
   * \vprivate Product Private
   */
  ~JSONDataReader() noexcept override = default;

  /*!
   * \brief Parser method for parsing JSON data from memory.
   * \param[in] data To be parsed into DataContainer in JSON format.
   * \return DataContainer filled with content from JSON.
   * \error ara::crypto::SecurityErrc::kInvalidArgument if \c data is not a valid JSON string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  amsr::core::Result<DataContainer> Parse(ara::crypto::ReadOnlyMemRegion data) const override;
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_DATA_READER_H_
