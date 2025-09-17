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
/*!        \file  json_parser.h
 *        \brief  JSON parser utilized by respective JSON file reader.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_PARSER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/json/reader.h"
#include "crypto/common/data/data_container.h"

namespace crypto {
namespace common {
namespace data {

/*!
 * \brief JSON parser utilized by respective JSON file reader.
 * \details We use the JSON data format to persist data. In order to parse JSON data we use the JSONParser provided by
 * vajson. This class implements several callback functions which are needed by the vajson Parser interface.
 * \vprivate Product Private
 */
class JSONParser : public amsr::json::Parser<JSONParser> {
  /*!
   * \brief Unqualified access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Unqualified access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

 public:
  /*!
   * \brief Constructor.
   * \param[in] document Document in JSON format which should be parsed.
   * \param[in] data The document is parsed into this DataContainer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  JSONParser(amsr::json::JsonData& document, DataContainer& data) noexcept
      : Parser<JSONParser>(document), data_{data}, array_{false}, array_key_{} {}

  // JSONParser is not copy constructible.
  JSONParser(JSONParser const& /*other*/) = delete;

  // JSONParser is not move constructible.
  JSONParser(JSONParser&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  // JSONParser is not copy assignable.
  JSONParser& operator=(JSONParser const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  // JSONParser is not move assignable.
  JSONParser& operator=(JSONParser&& /*other*/) & noexcept = delete;

  /*!
   * \brief Destructor.
   * \vprivate Product Private
   */
  ~JSONParser() noexcept override = default;

  /*!
   * \brief Callback for bool occurrence.
   * \param[in] val Value of the bool found in the json file.
   * \return The result of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error amsr::json::JsonErrc::kUserValidationFailed If a bool was
   *        encountered inside an array.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ParserResult OnBool(bool val);

  /*!
   * \brief Callback for number occurrence.
   * \param[in] num Value of the key found in the json file.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error amsr::json::JsonErrc::kUserValidationFailed If the parsed number is invalid.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ParserResult OnNumber(amsr::json::JsonNumber num);

  /*!
   * \brief Callback for string occurrence.
   * \param[in] str Value of the string found in the json file.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ParserState OnString(vac::container::CStringView str);

  /*!
   * \brief Callback for key occurrence.
   * \param[in] str String of the key found in the json file.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error amsr::json::JsonErrc::kUserValidationFailed If the key is empty.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static ParserResult OnKey(vac::container::CStringView str);

  /*!
   * \brief Callback for the start of an object.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ParserResult OnStartObject();

  /*!
   * \brief Callback for the end of an object.
   * \param[in] size Size of the parsed object.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static ParserState OnEndObject(std::size_t size);

  /*!
   * \brief Callback for the start of an array.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ParserState OnStartArray();

  /*!
   * \brief Callback for the end of an array.
   * \param[in] size Size of the parsed array.
   * \return The status of the event call used by the reader class.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ParserState OnEndArray(std::size_t size);

  /*!
   * \brief Callback for unexpected events.
   * \return ParserState::kRunning to continue parsing.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  static ParserState OnUnexpectedEvent();

 private:
  DataContainer& data_; /*!< Reference to data container to fill */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  bool array_;                   /*!< Whether the parser is currently in an array */
  amsr::core::String array_key_; /*!< Key of last array parsed */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__JsonParser, OnNumber__AddUnsignedArray);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__JsonParser, OnNumber__AddUnsigned);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__JsonParser, OnNumber__AddSignedArray);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__JsonParser, OnNumber__AddSigned);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__common__data__JsonParser, OnNumber__Error);
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_JSON_PARSER_H_
