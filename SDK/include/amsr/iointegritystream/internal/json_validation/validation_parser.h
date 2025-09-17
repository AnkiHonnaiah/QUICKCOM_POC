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
/*!     \file         validation_parser.h
 *      \brief        Parses validation json document using vajson library
 *      \unit         amsr::iointegritystream::internal::json_validation::ValidationParser
 *
 *      \complexity   The complexity is increased by many inter module calls which are necessary for the correct
 *                    functionality of this unit and does not introduce any risk.
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_JSON_VALIDATION_VALIDATION_PARSER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_JSON_VALIDATION_VALIDATION_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/filesystem/path.h"
#include "amsr/iointegritystream/internal/common_types.h"
#include "amsr/json/reader.h"
#include "ara/core/vector.h"

namespace amsr {
namespace iointegritystream {
namespace internal {
namespace json_validation {

/*! \brief Type used for keeping track of found keys */
struct IntegrityFoundKeys {
  /*! \brief boolean for found key */
  bool target_found;
  /*! \brief boolean for found key */
  bool format_version_found;
  /*! \brief boolean for found key */
  bool files_found;
  /*! \brief boolean for found key */
  bool filename_found;
  /*! \brief boolean for found key */
  bool checksum_found;
  /*! \brief boolean for found key */
  bool block_length_found;
  /*! \brief boolean for found key */
  bool algorithm_found;
};

/*!
 * \brief Class used for parsing configuration parameters from json file.
 * \trace DSGN-IoIntegrity-Verify-Validation-File
 */
class ValidationParser final : public amsr::json::v2::Parser {
 public:
  /*! \brief ParserResult type of vajson. */
  using ParserResult = json::ParserResult;

  /*! \brief StringView type of vajson. */
  using StringView = json::StringView;

  /*!
   * \brief Constructor.
   * \details Files in validation file will append the path on top of the provided path to the validation file. This is
   * to get the full path to the integrity protected file.
   * \param[in] document The json document to parse.
   * \param[in] validation_file_name Path to the validation file.
   */
  explicit ValidationParser(json::JsonData& document,
                            amsr::core::StringView validation_file_name = amsr::core::StringView{}) noexcept;

  /*!
   * \brief Destruct the parser.
   */
  ~ValidationParser() noexcept final = default;  // VCA_IOIS_DEFAULT_DESTRUCTOR

  // Delete move constructor/assignment
  ValidationParser(ValidationParser&& other) = delete;
  ValidationParser& operator=(ValidationParser&& other) & noexcept = delete;

  // Delete copy constructor/assignment
  ValidationParser(ValidationParser const&) = delete;
  ValidationParser& operator=(ValidationParser const& other) & noexcept = delete;

  /*!
   * \brief Event call for the string value type.
   * \param[in] str The parsed value.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ParserResult OnString(StringView str) noexcept final;

  /*!
   * \brief Event call for the number value type.
   * \param[in] num The parsed value.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ParserResult OnNumber(json::JsonNumber num) noexcept final;

  /*!
   * \brief Event call for the Bool value type.
   * \param[in] value The parsed value.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe TRUE
   * \reentrant FALSE
   */
  ParserResult OnBool(bool value) noexcept final;

  /*!
   * \brief Event call for the start of an object.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ParserResult OnStartObject() noexcept final;

  /*!
   * \brief Event call for the end of an object.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept final;

  /*!
   * \brief Event call for the start of an array.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ParserResult OnStartArray() noexcept final;

  /*!
   * \brief Event call for the end of an array.
   * \param[in] elements Number of array elements processed.
   * \return State of parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ParserResult OnEndArray(std::size_t elements) noexcept final;

  /*!
   * \brief Event call for unexpected events.
   * \return kRunning to continue parsing.
   * \context json::v2::Parser::Parse()
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   */
  ParserResult OnUnexpectedEvent() noexcept final;

  /*!
   * \brief Validation function for parsed data
   * \param[in] expected_target The expected target string
   * \param[in] block_length_max_validation Boolean for block_length validation
   * \return true if validation is ok, else false.
   * \context ANY
   * \pre -
   * \error amsr::json::JsonErrc::kUserValidationFailed Parsing failed
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \trace DSGN-IoIntegrity-Reader, DSGN-IoIntegrity-Writer, DSGN-IoIntegrity-Reader-Writer
   */
  amsr::core::Result<void> Validate(amsr::core::StringView expected_target,
                                    bool block_length_max_validation) const noexcept;

  /*!
   * \brief Get a reference to the parsed validation data.
   * \return The validation data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  ValidationFileData& GetValidationData() noexcept;

 private:
  /*!
   * \brief User-defined struct for the maximum block length.
   * \details Required to avoid Bauhaus issues in ValidateBlockLength() (V3-9-2).
   */
  struct MaxBlockLength {
    std::uint64_t val; /*!< \brief The configured max length. */
  };

  /*!
   * \brief Validate the parsed block length.
   * \param[in] block_length_max_validation Flag indicating whether the validation shall be performed or not.
   * \param[in] max_block_length The max allowed block length
   * \return Result if block length is valid, otherwise error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> ValidateBlockLength(bool block_length_max_validation,
                                               MaxBlockLength max_block_length) const noexcept;

  /*!
   * \brief Validate all parsed checksums to be valid 32-bit values
   * \return Result if all checksums are valid, otherwise error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  amsr::core::Result<void> ValidateCrc32Values() const noexcept;

  /*!
   * \brief Variable holding parsed data
   */
  ValidationFileData integrity_file_element_;

  /*!
   * \brief Name of the opened validation file.
   */
  filesystem::Path const validation_file_name_;

  /*! \brief Possible states of parser. */
  enum class State : std::uint32_t {
    kInit,            /*!< \brief Indicates that the parser is in the initialized state, before parsing any element. */
    kRoot,            /*!< \brief Indicates that the parser is in the root element. */
    kInFilesArray,    /*!< \brief Indicates that the parser is in the files array. */
    kInChecksumArray, /*!< \brief Indicates that the parser is in the files array. */
  };

  /*!
   * \brief State of parser.
   */
  State state_{State::kInit};

  /*!
   * \brief Keeps track of found keys.
   */
  IntegrityFoundKeys integrity_found_keys_;
};

}  // namespace json_validation
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_JSON_VALIDATION_VALIDATION_PARSER_H_
