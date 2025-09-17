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
/**     \file
 *      \brief      Extension point from which custom log sink variants can be added.
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * FILE REQUIRES USER MODIFICATIONS  Template Scope: Header section marked "Optional," if necessary
 * -------------------------------------------------------------------------------------------------------------------
 * This file includes template code that must be completed and/or adapted during BSW integration.The template code is
 * incomplete and only intended for providing a signature and an empty implementation. It is neither intended nor
 * qualified for use in series production without applying suitable quality measures. The template code must be
 * completed as described in the instructions given within this file and/or in the Technical Reference. The completed
 * implementation must be tested with diligent care and must comply with all quality requirements which are necessary
 * according to the state of the art before its use.
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_EXTENSION_POINT_CUSTOM_SINK_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_EXTENSION_POINT_CUSTOM_SINK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/internal/abort.h"
#include "vac/container/string_literals.h"
// Optional: replace dummy template implementation header with project specific one.
#include "log_sink_custom.h"

namespace amsr {
namespace log {

/*! \brief Alias for a collection of custom log sinks */
using CustomSinks = ara::core::Vector<std::unique_ptr<LogSinkCustom>>;

/*!
 * \brief   Used to provide LogAndTrace with a collection of custom implemented log sinks derived from the LogSinkCustom
 * class.
 *
 * \return  An ara::core::Vector of pointers referencing the implementations of LogSinkCustom.
 * \note    Current limitation is that LogAndTrace can only enable and integrate one single custom sink.
 *          Only the first element in the returned vector will be initialized.
 *
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 *
 * \vpublic
 */
inline CustomSinks RegisterCustomLogSinks() {
  // Optional: Either the user can provide their implementation in the amsr/log/log_sink_custom.cpp file directly,
  // or replace the template implementation with their own implemented class derived from LogSinkCustom class.
  // The second option requires that new headers and external libraries are linked to the build.
  //
  // If providing a proprietary header with an implementation class below statement should be adjusted
  // to return an instance of the given class.
  //

  using vac::container::literals::operator""_sv;
  CustomSinks sinks{};
  // VCA_LOGAPI_CHECKED_CONTRACT
  sinks.emplace_back(std::make_unique<LogSinkCustom>());
  internal::Assert(sinks.size() != 0, "At least one LogSinkCustom derived sink must be returned."_sv);
  return sinks;
}  // namespace log

}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_EXTENSION_POINT_CUSTOM_SINK_H_
