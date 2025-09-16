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
/*!        \file  socal/include_r20_11/ara/com/sample_ptr.h
 *        \brief  SamplePtr header file.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_R20_11_ARA_COM_SAMPLE_PTR_H_
#define LIB_SOCAL_INCLUDE_R20_11_ARA_COM_SAMPLE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/r20_11/events/sample_ptr.h"

namespace ara {
namespace com {

/*!
 * \brief Type alias for the R20-11 SamplePtr implementation.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \vpublic
 *
 * \trace SPEC-4980263
 */
template <typename SampleType>
using SamplePtr = ::amsr::socal::r20_11::events::SamplePtr<SampleType>;

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_R20_11_ARA_COM_SAMPLE_PTR_H_
