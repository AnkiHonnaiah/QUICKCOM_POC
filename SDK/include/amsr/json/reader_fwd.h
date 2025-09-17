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
/**       \file
 *        \brief    Forward declarations of public JSON types.
 *
 *        \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_FWD_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_FWD_H_

namespace amsr {
namespace json {
/*! \brief Forward declaration for JsonData. */
class JsonData;

namespace internal {
/*! \brief Forward declaration for JsonOps. */
class JsonOps;
}  // namespace internal

/*! \brief Forward declaration for JsonParser. */
class JsonParser;

namespace v2 {
/*! \brief Forward declaration for SingleArrayParser. */
class SingleArrayParser;
/*! \brief Forward declaration for SingleObjectParser. */
class SingleObjectParser;
/*! \brief Forward declaration for Parser. */
class Parser;
}  // namespace v2
}  // namespace json
}  // namespace amsr
#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_FWD_H_
