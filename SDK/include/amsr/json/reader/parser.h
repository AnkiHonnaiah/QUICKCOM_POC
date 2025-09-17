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
 *        \brief    A parser that aborts on unknown values.
 *
 *        \details  Provides pre-defined parsers for JSON elements. If the parser encounters a JSON element for which no
 *                  user callback is implemented by the child parser, it returns a negative Result.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/v1/parser.h"
#include "amsr/json/reader/v2/parser.h"
#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_PARSER_H_
