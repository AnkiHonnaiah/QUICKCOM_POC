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
/**       \file     serializer.h
 *        \brief    Contains common types and forward declarations for JSON serializers.
 *
 *        \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_SERIALIZER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/json/reader/internal/ref.h"
#include "amsr/json/util/types.h"

namespace amsr {
namespace json {
/*!
 * \brief State of the object to be serialized.
 *
 * \details Indicates if the object is empty or not. Commas should only appended if the object is not empty.
 *
 * \vpublic
 */
enum class SerializerState : bool { kEmpty, kNonEmpty };

/*!
 * \brief An empty type that signifies that the serializer has no follow-up state.
 *
 * \vprivate component private
 *
 * \trace DSGN-JSON-Writer-Error-Prevention
 */
class Unit {
 public:
  /*!
   * \brief Constructs a Unit type from an output stream.
   *
   * \details This satisfies the 'Next' state interface for serializers.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  explicit Unit(internal::Ref<amsr::stream::StatefulOutputStream>, SerializerState = SerializerState::kEmpty) noexcept {
  }
};

/*!
 * \brief A marker struct that only tells the GenericValueSerializer to return itself after using operator<<().
 *
 * \vprivate component private
 *
 * \trace DSGN-JSON-Writer-Error-Prevention
 */
class Self {};

/*!
 * \brief Forward declaration for the GenericValueSerializer.
 *
 * \vprivate component private
 */
template <typename Return = Self>
class GenericValueSerializer;

/*!
 * \brief A serializer type for single values.
 *
 * \vprivate component private
 */
using ValueSerializer = GenericValueSerializer<Unit>;

/*!
 * \brief A serializer type for JSON documents.
 *
 * \details Intentionally a using to make it obvious that a JSON document must start with a single value.
 *
 * \vpublic
 */
using DocumentSerializer = ValueSerializer;

/***********************
 * Object Declarations *
 **********************/

/*!
 * \brief Forward declaration for the KeySerializer.
 */
class KeySerializer;

/*!
 * \brief A serializer type for the start of JSON objects.
 *
 * \details Typedef for the initial Object serializer state where only a key is allowed.
 *
 * \vpublic
 */
using ObjectStart = KeySerializer;

/*!
 * \brief A serializer type for JSON objects.
 *
 * \details This class only allows adding a value into the object and the next concatenation will only allow a key.
 *
 * \vprivate component private
 */
using ObjectSerializerValue = GenericValueSerializer<KeySerializer>;

/**********************
 * Tuple Declarations *
 *********************/

/*!
 * \brief A serializer type for JSON arrays.
 *
 * \details Serializes multiple, potentially inhomogeneous values.
 *
 * \vprivate component private
 */
using ArraySerializer = GenericValueSerializer<>;

/*!
 * \brief A serializer type for the start of JSON arrays.
 *
 * \details Typedef for the initial Array serializer state.
 *
 * \vpublic
 */
using ArrayStart = ArraySerializer;

/*!
 * \brief Type of the output writer.
 *
 * \vpublic
 */
using WriterType = internal::Ref<amsr::stream::StatefulOutputStream>;
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_SERIALIZER_H_
