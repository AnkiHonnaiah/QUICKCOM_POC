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
/**       \file     object_type_impl.h
 *        \brief    Implementation of methods for object type.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_OBJECT_TYPE_IMPL_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_OBJECT_TYPE_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/json/writer/serializers/structures/generic_value_serializer.h"
#include "amsr/json/writer/serializers/structures/serializer.h"
#include "amsr/json/writer/types/object_type.h"

namespace amsr {
namespace json {
inline namespace types {
/*!
 * \internal
 * - Serialize every key-value pair as a JSON key followed by a JSON value.
 * \endinternal
 * \vprivate component private
 *
 * \spec
 * requires true;
 * \endspec
 */
template <typename Range, typename KeyFn, typename ValueFn>
template <typename KS>
auto PairRangeSerializer<Range, KeyFn, ValueFn>::operator()(KS os) const noexcept -> KS {
  // VECTOR NC AutosarC++17_10-A7.1.5: MD_JSON_AutosarC++17_10-A7.1.5_auto_it
  // VCA_VAJSON_THIS_DEREF
  for (auto const& pair : *this->map_) {
    // VCA_VAJSON_THIS_DEREF
    ObjectSerializerValue osv{std::move(os) << this->key_function_(pair.first)};
    // VECTOR NC AutosarC++17_10-A12.8.3: MD_JSON_read_moved_object
    // VCA_VAJSON_THIS_DEREF
    os = std::move(osv) << this->value_function_(pair.second);
  }
  return os;
}
}  // namespace types
}  // namespace json
}  // namespace amsr
#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_TYPES_OBJECT_TYPE_IMPL_H_
