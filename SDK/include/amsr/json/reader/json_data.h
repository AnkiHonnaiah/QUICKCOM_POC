/**********************************************************************************************************************
 *  COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/**       \file     json_data.h
 *        \brief    Representation of JSON data.
 *
 *        \details  Provides handling methods for JSON data.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_JSON_DATA_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_JSON_DATA_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/iostream/input_stream.h"
#include "amsr/json/reader/internal/config/json_reader_cfg.h"
#include "amsr/json/reader/internal/depth_counter.h"
#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/reader/parser_state.h"
#include "amsr/json/reader_fwd.h"
#include "amsr/json/util/types.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace json {
// VECTOR NC Metric-OO.WMC.One: MD_JSON_Metric.OO.WMC.One_json_data
/*!
 * \brief A JSON data representation.
 *
 * \details Handles the state of the data, such as the current position in the file and nesting of JSON tree.
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Reader-Deserialization
 */
class JsonData final {
  /*! \brief Stream buffer. */
  internal::Ref<stream::InputStream> stream_;
  /*! \brief The potentially owned stream. */
  std::unique_ptr<stream::InputStream> owned_stream_{nullptr};
  /*! \brief JSON structure state. */
  internal::DepthCounter depth_counter_{};
  /*! \brief Current key. */
  String current_key_{};
  /*! \brief Current buffer. */
  String current_buffer_{};
  /*! \brief Flag to indicate if the document has a UTF-8 BOM. */
  EncodingType encoding_{EncodingType::kNone};
  /*! \brief Backup of the structure state. */
  internal::DepthCounter depth_counter_backup_{};
  /*! \brief Backup of the document position. */
  std::uint64_t pos_backup_{0U};
  /*! \brief Flag to indicate if a backup state is available. */
  bool has_backup_{false};

 public:
  /*!
   * \brief Class JsonOps must have access to the InputStream but no other classes.
   */
  friend class internal::JsonOps;  // VECTOR SL AutosarC++17_10-A11.3.1: MD_JSON_AutosarC++17_10-A11.3.1_json_ops

  /*!
   * \brief Initializes a JSON data object using a constructed reader.
   * \param input_stream to operate on.
   *
   * \context ANY
   * \pre \c input_stream has to be open.
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Vector product private.
   */
  explicit JsonData(stream::InputStream& input_stream) noexcept;

  /*!
   * \brief Initializes a JSON data object using a constructed reader.
   * \param input_stream to operate on.
   *
   * \context ANY
   * \pre \c input_stream has to be open.
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \vprivate Component private.
   */
  explicit JsonData(std::unique_ptr<stream::InputStream> input_stream) noexcept;

  /*!
   * \brief Initializes a JSON data object from a file.
   * \param path The path to the JSON file.
   * \return A constructed JSON data object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \error{JsonErrc::kStreamFailure, if the file could not be opened.}
   *
   * \vpublic
   */
  static auto FromFile(ara::core::StringView path) noexcept -> Result<JsonData>;

  /*!
   * \brief Initializes a JSON data object from a buffer.
   * \param buffer The buffer containing the JSON value.
   * \return A constructed JSON data object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  static auto FromBuffer(ara::core::StringView buffer) noexcept -> Result<JsonData>;

  /*!
   * \brief Initializes a JSON data object from a buffer.
   * \param buffer The buffer containing the JSON value.
   * \return A constructed JSON data object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  static auto FromBuffer(vac::container::CStringView buffer) noexcept -> Result<JsonData>;

  /*!
   * \brief Initializes a JSON data object from a buffer.
   * \param buffer The buffer containing the JSON value.
   * \return A constructed JSON data object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  static auto FromBuffer(ara::core::Span<char const> buffer) noexcept -> Result<JsonData>;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Move constructor.
   * \param other The moved from object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  JsonData(JsonData&& other) noexcept = default;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move assignment.
   * \return A reference to the moved into object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_VAJSON_MOLE_1298
  auto operator=(JsonData&&) & noexcept -> JsonData& = default;

  /*!
   * \brief Deleted copy constructor.
   */
  JsonData(JsonData const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   */
  auto operator=(JsonData const&) & -> JsonData& = delete;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default destructor.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_VAJSON_MOLE_1298
  ~JsonData() noexcept = default;

  /*!
   * \brief Returns the JSON structure state.
   * \return The current state.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetState() noexcept -> internal::DepthCounter& { return this->depth_counter_; }

  /*!
   * \brief Sets the current key.
   * \param key to store.
   * \return The current key.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  void StoreCurrentKey(StringView key) noexcept {
    // VCA_VAJSON_WITHIN_SPEC
    this->current_key_ = key.ToString(std::allocator<char>());
  }

  /*!
   * \brief Returns the current key.
   * \return The current key.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetCurrentKey() const noexcept -> CStringView { return CStringView::FromString(this->current_key_); }

  /*!
   * \brief Returns the current string.
   * \return The current string.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetCurrentString() const noexcept -> CStringView { return CStringView::FromString(this->current_buffer_); }

  /*!
   * \brief Gets the emptied string buffer.
   * \return The emptied string buffer.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetClearedStringBuffer() noexcept -> String& {
    // VCA_VAJSON_WITHIN_SPEC
    this->current_buffer_.clear();
    return this->current_buffer_;
  }

  /*!
   * \brief Gets the string buffer.
   * \return The string buffer.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetStringBuffer() noexcept -> String& { return this->current_buffer_; }

  /*!
   * \brief Returns the detected encoding type.
   * \return The encoding type.
   * \spec
   *   requires true;
   * \endspec
   * \trace DSGN-JSON-Reader-Encoding
   * \vpublic
   */
  auto GetEncoding() noexcept -> EncodingType& { return this->encoding_; }

  /*!
   * \brief Stores a snapshot of the parser's current state.
   * \return A Result being empty, or the error that occurred.
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Component private.
   */
  auto Snap() noexcept -> Result<void>;

  /*!
   * \brief Restores the parser's state from the previous snapshot.
   * \return A Result being empty, or the error that occurred.
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Component private.
   */
  auto Restore() noexcept -> Result<void>;

 private:
  /*!
   * \brief Returns the stream.
   * \return The stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetStream() noexcept -> stream::InputStream& { return *this->stream_; }

  /*!
   * \brief Returns the stream.
   * \return The stream.
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetStream() const noexcept -> stream::InputStream const& { return *this->stream_; }

  /*!
   * \brief Inspects the document's BOM.
   *
   * \context ANY
   * \pre The read pointer must be at the beginning of the document.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void ParseBom() noexcept;
};
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_JSON_DATA_H_
