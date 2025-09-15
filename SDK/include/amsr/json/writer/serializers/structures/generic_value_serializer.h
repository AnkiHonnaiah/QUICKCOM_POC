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
/**       \file     generic_value_serializer.h
 *        \brief    Serializer for generic JSON value types.
 *
 *        \details  Provides serializers for Null, Bool, Number, String, Array, and Object types.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_GENERIC_VALUE_SERIALIZER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_GENERIC_VALUE_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "amsr/charconv/to_textual.h"
#include "amsr/iostream/output_stream.h"

#include "amsr/json/util/types.h"
#include "amsr/json/writer/serializers/structures/serializer.h"
#include "amsr/json/writer/serializers/util/escaped_json_string.h"
#include "amsr/json/writer/serializers/util/length_serializer.h"
#include "amsr/json/writer/types/array_type.h"
#include "amsr/json/writer/types/basic_types.h"
#include "amsr/json/writer/types/bin_types.h"
#include "amsr/json/writer/types/object_type.h"

namespace amsr {
namespace json {
/*!
 * \brief A serializer for JSON value types.
 * \tparam Return Type of the return value of a << operation. Must be one of the following types:
 *                - Unit: Serializer has no follow-up state (outermost element).
 *                - Self or GenericValueSerializer<T>: Next element is another value (e.g. inside arrays).
 *                - KeySerializer: Next element is a key.
 *
 * \vpublic
 *
 * \trace DSGN-JSON-Writer-Serialization
 */
template <typename Return>
class GenericValueSerializer final {
 public:
  /*!
   * \brief Type of the return value.
   *
   * \details Set the type of the return value to be either its own type GenericValueSerializer (for arrays or a
   *          specified type) or the type specified by Return.
   *
   * \vpublic
   */
  using Next = typename std::conditional_t<std::is_same<Return, Self>::value, GenericValueSerializer, Return>;

  /*!
   * \brief Constructs a GenericValueSerializer from an output stream.
   * \details Do not create an instance of GenericValueSerializer directly, use the vpublic aliases in
   * amsr/json/writer/serializer.h
   *
   * \param[out] os Output stream to write into.
   * \param state of the Serializer.
   * \param bom The BOM type to write.
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
   */
  explicit GenericValueSerializer(WriterType os, SerializerState state = SerializerState::kEmpty,
                                  EncodingType bom = EncodingType::kNone) noexcept
      : os_(os), serializer_state_{state} {
    this->WriteBom(bom);
  }

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
   */
  GenericValueSerializer(GenericValueSerializer&&) noexcept = default;

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
   */
  auto operator=(GenericValueSerializer&&) & noexcept -> GenericValueSerializer& = default;

  // Deleted copy constructor/assignment operator.
  GenericValueSerializer(GenericValueSerializer const&) = delete;
  auto operator=(GenericValueSerializer const&) & -> GenericValueSerializer& = delete;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_JSON_AutosarC++17_10-A15.5.1_explicit_noexcept_missing
  /*!
   * \brief Default DTOR.
   * \vpublic
   */
  ~GenericValueSerializer() noexcept = default;

  /*!
   * \brief Serializes a null value.
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
   */
  auto operator<<(JNullType) && noexcept -> Next {
    return this->Serialize([this]() noexcept {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Write("null"_sv);
    });
  }

  /*!
   * \brief Serializes a boolean value.
   * \param b Boolean value to serialize.
   * \return The succeeding serializer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   *
   * \spec
   * requires true;
   * \endspec
   */
  auto operator<<(JBoolType b) && noexcept -> Next {
    vac::container::CStringView const value{b.value ? "true"_sv : "false"_sv};
    // VECTOR NL AutosarC++17_10-M8.5.1: MD_JSON_AutosarC++17_10-M8.5.1_use_of_possibly_unintialized_value_false_positive
    return this->Serialize([this, value]() noexcept {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Write(value);
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Serializes a number value.
   * \tparam T Type of number.
   * \param number value to serialize.
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
   */
  template <typename T>
  auto operator<<(JNumberType<T> number) && noexcept -> Next {
    return this->Serialize([this, number]() noexcept {
      std::array<char, amsr::charconv::kFPMaxLength> buffer{};
      amsr::charconv::ToTextual<T> to_textual{amsr::core::Span<char>{buffer}, static_cast<T>(number.GetValue())};
      // VCA_VAJSON_EXTERNAL_CALL
      amsr::core::Result<amsr::core::Span<char>> conversion_result{to_textual.Convert()};

      AssertCondition(conversion_result.HasValue(),
                      "GenericValueSerializer: Could not convert number to textual representation.");

      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Write(conversion_result.Value());
    });
  }

  /*!
   * \brief Serializes a string value.
   * \param string value to serialize.
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
   * - Add quotes to the begin and end of the string.
   * - Serialize the escaped string as a JSON string value.
   * \endinternal
   */
  auto operator<<(JStringType string) && noexcept -> Next {
    return this->Serialize([this, string]() noexcept {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put('"');
      // VCA_VAJSON_OUTPUTSTREAM
      *this->os_ << internal::EscapedJsonString(string);
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put('"');
    });
  }

  /*!
   * \brief Serializes a binary string value.
   * \param string value to serialize.
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
   * - Add a 's' to denote the following value as a string.
   * - Serialize the length of the string value as four bytes big endian.
   * - Write the string value.
   * \endinternal
   */
  auto operator<<(JBinStringType string) && noexcept -> Next {
    return this->Serialize([this, string]() noexcept {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put('s');
      // VCA_VAJSON_OUTPUTSTREAM
      internal::SerializeLength(this->os_, string.GetLength());
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Write(string.GetValue());
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Serializes a series of serializable values.
   * \tparam Fn Type of serializer function.
   * \param tuple to serialize.
   * \return The succeeding serializer.
   *
   * \context ANY
   * \pre The function contained in the argument does not throw any exceptions
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vpublic
   *
   * \internal
   * - Add an opening square bracket.
   * - Serialize every value as a JSON value.
   * - Add a closing square bracket.
   * \endinternal
   */
  template <typename Fn>
  auto operator<<(JArrayType<Fn> tuple) && noexcept -> Next {
    return this->Serialize([this, tuple]() {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put('[');
      tuple.fn(ArrayStart(this->os_));
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put(']');
    });
  }

  /*!
   * \brief Serializes a binary value.
   * \param bin Value to serialize.
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
   * - Add a 'b' to denote the following value as binary.
   * - Serialize the length of the binary value as four bytes big endian.
   * - Write the binary value.
   * \endinternal
   */
  auto operator<<(JBinType bin) && noexcept -> Next {
    return this->Serialize([this, bin]() {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put('b');
      // VCA_VAJSON_OUTPUTSTREAM
      internal::SerializeLength(this->os_, bin.GetLength());
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Write(bin.GetValue());
    });
  }

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Serializes an object.
   * \tparam Fn Type of serializer function.
   * \param object to serialize.
   * \return The succeeding serializer.
   * \vpublic
   *
   * \context ANY
   * \pre The function contained in the argument does not throw any exceptions
   * \threadsafe FALSE
   * \reentrant FALSE
   */
  template <typename Fn>
  auto operator<<(JObjectType<Fn> object) && noexcept -> Next;

 private:
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Serializes a value.
   * \tparam Fn Type of function.
   * \param fn Serializer call function.
   * \return The succeeding serializer.
   *
   * \context ANY
   * \pre The passed function does not throw any exceptions
   * \threadsafe FALSE
   * \reentrant FALSE
   *
   * \internal
   * - If another element was serialized before:
   *   - Add a comma.
   * - Execute the given serializer function.
   * \endinternal
   */
  template <typename Fn>
  auto Serialize(Fn&& fn) noexcept -> Next {
    if (this->serializer_state_ == SerializerState::kNonEmpty) {
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Put(',');
    }
    fn();
    return Next(this->os_, SerializerState::kNonEmpty);
  }

  /*!
   * \brief Writes the requested BOM type.
   * \param type The BOM type to write.
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
   * - Write the requested BOM to the stream.
   * \endinternal
   */
  void WriteBom(EncodingType type) noexcept {
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_JSON_AutosarC++17_10-M0.1.2_dead_code
    if (type == EncodingType::kUtf8) {
      constexpr amsr::core::StringView kUtf8Bom{"\xEF\xBB\xBF"_sv};
      // VCA_VAJSON_OUTPUTSTREAM
      this->os_->Write(kUtf8Bom);
    }
  }

  /*! \brief Output stream to write into. */
  WriterType os_;

  /*!
   * \brief Serializer state.
   */
  SerializerState serializer_state_;
};
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_GENERIC_VALUE_SERIALIZER_H_
