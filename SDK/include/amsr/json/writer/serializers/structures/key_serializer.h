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
/**       \file     key_serializer.h
 *        \brief    Serializer for JSON keys.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_KEY_SERIALIZER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_KEY_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/writer/serializers/structures/serializer.h"
#include "amsr/json/writer/serializers/util/escaped_json_string.h"
#include "amsr/json/writer/serializers/util/length_serializer.h"

#include "amsr/json/util/types.h"
#include "amsr/json/writer/types/array_type.h"
#include "amsr/json/writer/types/basic_types.h"
#include "amsr/json/writer/types/object_type.h"

namespace amsr {
namespace json {
/*!
 * \brief A serializer for JSON keys.
 *
 * \details This class only allows adding a key into the object and always returns a value serializer to only allow a
 *          value for the next concatenation operation.
 *
 * \vpublic
 */
class KeySerializer final {
 public:
  /*!
   * \brief Serializer state after adding a key.
   * \vpublic
   */
  using Next = ObjectSerializerValue;

  /*!
   * \brief Constructs a KeySerializer from an output stream.
   * \details Do not create an instance of KeySerializer directly, use the vpublic aliases in
   * amsr/json/writer/serializer.h
   *
   * \param[out] os Output stream to write into.
   * \param state of the Serializer.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   *
   * \spec
   * requires true;
   * \endspec
   *
   */
  explicit KeySerializer(WriterType os, SerializerState state = SerializerState::kEmpty) noexcept
      : os_(os), serializer_state_{state} {}

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move constructor.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   *
   * \spec
   * requires true;
   * \endspec
   *
   */
  KeySerializer(KeySerializer&&) noexcept = default;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default move assignment.
   * \return A reference to the moved into object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   *
   * \spec
   * requires true;
   * \endspec
   *
   */
  auto operator=(KeySerializer&&) & noexcept -> KeySerializer& = default;

  // Deleted copy constructor copy assignment operator.
  KeySerializer(KeySerializer const&) = delete;
  auto operator=(KeySerializer const&) & -> KeySerializer& = delete;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief   Default DTOR.
   * \vpublic
   */
  ~KeySerializer() noexcept = default;

  /*!
   * \brief Serializes a key.
   * \param key to serialize.
   * \return The succeeding serializer.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   *\vpublic
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \internal
   * - Add a comma, if necessary.
   * - Serialize the key.
   * \endinternal
   */
  auto operator<<(JKeyType key) && noexcept -> Next {
    this->WriteComma();

    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Put('"');
    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Write(internal::EscapedJsonString(key).GetValue());
    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Write(R"(":)"_sv);

    return Next(this->os_);
  }

  /*!
   * \brief Serializes a binary key.
   * \param key to serialize.
   * \return The succeeding serializer.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \internal
   * - Add a comma, if necessary.
   * - Serialize the length of the key as four bytes big endian.
   * - Write the key.
   * \endinternal
   */
  auto operator<<(JBinKeyType key) && noexcept -> Next {
    this->WriteComma();

    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Put('k');
    // VCA_VAJSON_OUTPUTSTREAM
    internal::SerializeLength(this->os_, key.GetLength());
    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Write(key.GetValue());

    return Next(this->os_);
  }

 private:
  /*!
   * \brief Adds a comma to the stream, if necessary.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \spec
   * requires true;
   * \endspec
   *
   * \internal
   * - If another element was serialized before, add a comma.
   * \endinternal
   */
  void WriteComma() noexcept {
    if (this->serializer_state_ == SerializerState::kNonEmpty) {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put(',');
    }
  }

  /*!
   * \brief Output stream to write into.
   */
  WriterType os_;

  /*!
   * \brief Serializer state.
   */
  SerializerState serializer_state_;
};
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_KEY_SERIALIZER_H_
