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
/*!        \file  amsr/trace/tracesink.h
 *        \brief  Declaration of the TraceSink interface and AddTraceSink function.
 *
 *         \unit  LogAndTrace::Trace::TraceSink
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACESINK_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACESINK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "amsr/core/vector.h"
#include "amsr/trace/tracer.h"
#include "ara/core/vector.h"

namespace amsr {
namespace trace {
/*!
 * \brief TraceSink abstract class.
 * \vpublic
 */
class TraceSink {
 public:
  /*! \brief Default destructor. */
  virtual ~TraceSink() noexcept = default;
  /*! \brief Default destructor. */
  TraceSink() noexcept = default;
  /*!
   * \brief Get the Id of the TraceSink.
   * \return The id of the TraceSink.
   */
  virtual auto GetId() const noexcept -> amsr::core::StringView = 0;
  /*!
   * \brief  Get the TraceSink's Tracer.
   * \return A reference to the TraceSink's Tracer.
   * \vprivate Component Private
   */
  virtual auto GetTracer() noexcept -> Tracer& = 0;

 protected:
  /*! \brief Default copy constructor. */
  TraceSink(TraceSink const&) noexcept = default;
  /*! \brief Default move constructor. */
  TraceSink(TraceSink&&) noexcept = default;
  /*!
   * \brief Default copy assignment.
   * \return
   */
  auto operator=(TraceSink const&) & noexcept -> TraceSink& = default;
  /*!
   * \brief Default move assignment.
   * \return
   */
  auto operator=(TraceSink&&) & noexcept -> TraceSink& = default;
};

// VECTOR NC AutosarC++17_10-M3.2.4: MD_LOG_AutosarC++17_10-M3.2.4_noDefinitionFalsePositive
/*!
 * \brief   Forward declaration of function that calls AddTraceSink() on all TraceSinks to be used.
 * \details An empty default implementation is provided in LogAndTrace's trace-extension-point library.
 *          There is a cmake switch (see TechRef) that allows providing a custom implementation.
 * \vpublic
 */
void RegisterTraceSinks();

/*!
 * \brief Adds the provided TraceSink to the list of TraceSinks.
 * \spec
 *  return true
 * \endspec
 * \trace DSGN-LogAndTrace-CustomTraceSinks
 * \vpublic
 */
void AddTraceSink(std::unique_ptr<TraceSink>&&) noexcept;
}  // namespace trace
}  // namespace amsr
#endif
