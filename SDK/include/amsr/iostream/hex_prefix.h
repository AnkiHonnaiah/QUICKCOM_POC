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
/*!        \file
 *        \brief  Implements a hex prefix.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_HEX_PREFIX_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_HEX_PREFIX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/iostream/types.h"

namespace amsr {
namespace stream {

/*!
 * \brief Implements a hex prefix.
 * \unit amsr::stream::HexPrefix
 * \vprivate Component Private
 */
class HexPrefix {
 public:
  /*! size of prefix for hexadecimal numbers */
  static constexpr std::uint8_t kHexPrefixSize{2};

  /*!
   * \brief Get string view of character sequence "0x"
   * \return String view of literal "0x".
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  static amsr::core::StringView GetLiteral();
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_HEX_PREFIX_H_
