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
/**       \file     generic_value_serializer_impl.h
 *        \brief    Implementation of methods for generic value serializer type.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_GENERIC_VALUE_SERIALIZER_IMPL_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_GENERIC_VALUE_SERIALIZER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/writer/serializers/structures/generic_value_serializer.h"

namespace amsr {
namespace json {
/*!
 * \brief  Serializes another object into the output stream
 * \vprivate component private
 *
 * \internal
 * - Assert that the current state allows adding an object.
 * - Add an opening curly bracket.
 * - Serialize the object as a JSON object.
 * - Add a closing curly bracket.
 * \endinternal
 *
 * \spec
 * requires true;
 * \endspec
 */
template <typename Return>
template <typename Fn>
auto GenericValueSerializer<Return>::operator<<(JObjectType<Fn> object) && noexcept
    -> GenericValueSerializer<Return>::Next {
  return this->Serialize([this, &object]() {
    /*!
     * \brief Defines the return value type.
     */
    using ReturnType = decltype(object.fn(ObjectStart(this->os_)));
    static_assert(std::is_same<ReturnType, KeySerializer>::value, "Cannot close object in current state");

    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Put('{');
    // VCA_VAJSON_OUTPUTSTREAM
    static_cast<void>(object.fn(ObjectStart(this->os_)));
    // VCA_VAJSON_OUTPUTSTREAM
    this->os_->Put('}');
  });
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_STRUCTURES_GENERIC_VALUE_SERIALIZER_IMPL_H_
