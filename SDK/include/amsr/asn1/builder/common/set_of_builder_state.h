/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   amsr/asn1/builder/common/set_of_builder_state.h
 *        \brief  ASN.1 Builder for SET OF.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SET_OF_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SET_OF_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/builder/constructed_type_builder_state.h"
#include "sequence_of_builder_state.h"

namespace amsr {
namespace asn1 {
namespace builder {

// Note: Both SET OF and SEQUENCE OF are dynamic structures translated as C++ vectors. The fact that ASN.1 SETs are
//  not ordered is not reflected in the built data. The type tags are passed in a constructor call. Hence,
//  there is no physical difference between a SetOfBuilderState and a SequenceOfBuilderState.

/*!
 * \brief Alias name for SET OF builder.
 */
template <typename... Args>
using SetOfBuilderState = SequenceOfBuilderState<Args...>;

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_COMMON_SET_OF_BUILDER_STATE_H_
