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
/**       \file     parser_state.h
 *        \brief    Contains the definition of the parser state.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_PARSER_STATE_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_PARSER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"

namespace amsr {
namespace json {
/*!
 * \brief State that is used for every parser event.
 *
 * \vpublic
 */
enum class ParserState : bool { kRunning, kFinished };

/*!
 * \brief Return type of most parser functions.
 *
 * \vpublic
 */
using ParserResult = ara::core::Result<ParserState>;
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_PARSER_STATE_H_
