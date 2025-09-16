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
 *        \brief  Configuration class for methods.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_METHOD_CONFIG_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_METHOD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/method_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief   Configuration for methods.
 * \details Configuration class storing the configuration parameters for an method.
 *
 * \unit ComTrace::TraceConfig
 */
class MethodConfig final : public MethodConfigInterface {
 public:
  /*!
   * \brief Construct an empty method configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  MethodConfig() noexcept = default;

  /*!
   * \brief Copy-construct a method configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  MethodConfig(MethodConfig const&) noexcept = default;

  auto operator=(MethodConfig const&) noexcept -> MethodConfig& = delete;
  MethodConfig(MethodConfig&&) noexcept = delete;
  auto operator=(MethodConfig&&) noexcept -> MethodConfig& = delete;

  /*!
   * \brief Destroy method configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~MethodConfig() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \copydoc amsr::comtrace::internal::config::MethodConfigInterface::GetShortname()
   *
   * \spec requires true; \endspec
   */
  auto GetShortname() const noexcept -> Shortname final;

  /*!
   * \brief Sets the shortname.
   *
   * \param[in] shortname  The shortname.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetShortname(Shortname const& shortname) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::MethodConfigInterface::IsTraceRequestSendEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceRequestSendEnabled() const noexcept -> bool final;

  /*!
   * \brief Enable or disable tracing of sending requests.
   *
   * \param[in] enabled  'true' for enabling the tracing of send requests, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceRequestSendEnabled(bool enabled) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::MethodConfigInterface::IsTraceRequestReceivedEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceRequestReceivedEnabled() const noexcept -> bool final;

  /*!
   * \brief Enable or disable tracing of received requests.
   *
   * \param[in] enabled  'true' for enabling the tracing of received requests, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceRequestReceivedEnabled(bool enabled) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::MethodConfigInterface::IsTraceResponseSendEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceResponseSendEnabled() const noexcept -> bool final;

  /*!
   * \brief Enable or disable tracing of sending responses.
   *
   * \param[in] enabled  'true' for enabling the tracing of send responses, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceResponseSendEnabled(bool enabled) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::MethodConfigInterface::IsTraceResponseReceivedEnabled()
   *
   * \spec requires true; \endspec
   */
  auto IsTraceResponseReceivedEnabled() const noexcept -> bool final;

  /*!
   * \brief Enable or disable tracing of received responses.
   *
   * \param[in] enabled  'true' for enabling the tracing of received responses, 'false' to disable.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetTraceResponseReceivedEnabled(bool enabled) noexcept -> void;

 private:
  /*! Short name of the method. */
  Shortname shortname_{};

  /*! Trace request send. */
  bool trace_request_send_{false};
  /*! Trace request received. */
  bool trace_request_received_{false};
  /*! Trace response send. */
  bool trace_response_send_{false};
  /*! Trace response received. */
  bool trace_response_received_{false};
};

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_METHOD_CONFIG_H_
