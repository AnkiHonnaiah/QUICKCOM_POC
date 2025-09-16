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
/**        \file  internal/instance_identifier_string.h
 *        \brief  Define a string for binding independent storage of the instance IDs.
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_IDENTIFIER_STRING_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_IDENTIFIER_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief   String for binding-independent storage of the instance IDs.
 * \details This string must not contain any other meta-data of the instance identifier (e.g. the binding type). Example
 *          for SOME/IP Binding: "23"
 */
using InstanceIdentifierString = std::string;

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INSTANCE_IDENTIFIER_STRING_H_
