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

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_PARSER_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/internal/parsers/composition_parser.h"
#include "amsr/json/reader/internal/parsers/virtual_parser.h"

namespace amsr {
namespace json {
namespace v2 {
/*!
 * \brief A parser that aborts on unknown values.
 *
 * \details The VirtualParser implements a interface for parsers and is based on dynamic polymorphism.
 *          The derived parser can implement callbacks for all types of elements it expects to appear. If the parser
 *          encounters any other type, it calls OnUnexpectedEvent. If this callback is not defined by the derived
 *          parser, the default implementation aborts parsing.
 *
 * \vpublic
 */
// VCA_VAJSON_MOLE_1298
class Parser : public internal::CompositionParser<internal::VirtualParser> {
 public:
  /*!
   * \brief Constructor.
   */
  using internal::CompositionParser<internal::VirtualParser>::CompositionParser;
};
}  // namespace v2
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_V2_PARSER_H_
