/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/include/datatypes/impl_type_overlimitenum.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_modelleddatatypes_api
 *         Commit ID: 24bd7fa8a3a5d079286c3008b376d3fafd47f4f6
 *********************************************************************************************************************/

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_DATATYPES_IMPL_TYPE_OVERLIMITENUM_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_DATATYPES_IMPL_TYPE_OVERLIMITENUM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace datatypes {

// VCA Disable [LNG-01,LNG-03,SLC-11,SLC-12,SLC-14,SLC-16,SPC-15] : VCA_MODELLEDDATATYPEGENERATOR_DATA_TYPE_DECLARATION
/*!
 * \brief Type OverLimitEnum.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /Datatypes/OverLimitEnum
 */
enum class OverLimitEnum : std::uint8_t {
  NOT_OVER = 0,
  OVER_LOW = 1,
  OVER_HIGH = 2
};
// VCA Enable : VCA_MODELLEDDATATYPEGENERATOR_DATA_TYPE_DECLARATION

}  // namespace datatypes

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_DATATYPES_IMPL_TYPE_OVERLIMITENUM_H_
