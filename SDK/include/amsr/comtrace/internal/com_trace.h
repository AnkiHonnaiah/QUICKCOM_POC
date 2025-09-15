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
 *        \brief  ComTrace component main instance.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_COM_TRACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_COM_TRACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

#include "amsr/comtrace/internal/async_client_trace_interface.h"
#include "amsr/comtrace/internal/client_trace_interface.h"
#include "amsr/comtrace/internal/config/trace_config.h"
#include "amsr/comtrace/internal/config/trace_config_interface.h"
#include "amsr/comtrace/internal/logger.h"
#include "amsr/comtrace/types.h"

namespace amsr {
namespace comtrace {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_ComTrace_M7.3.6_StringView_literal
/*!
 * \brief Use string literal for creation of StringViews.
 */
using vac::container::literals::operator""_sv;

/*!
 * \brief   Main instance of ComTrace component.
 * \details Instance lifecycle is managed as Singleton.
 *
 * \unit ComTrace::ComponentLifecycle
 */
class ComTrace final {
 public:
  /*!
   * \brief Type-alias for StringView.
   */
  using StringView = ::amsr::core::StringView;

  /*!
   * \brief Type-alias for TraceConfigInterface.
   */
  using TraceConfigInterface = internal::config::TraceConfigInterface;

  /*!
   * \brief Default file path of the ComTrace configuration JSON file.
   */
  static constexpr StringView kDefaultConfigJsonFilePath{"./etc/comtrace_config.json"_sv};

  // ---- Construction ----

  /*!
   * \brief Get the singleton instance.
   *
   * \return Reference to the singleton instance.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static auto GetInstance() noexcept -> ::amsr::generic::Singleton<ComTrace>&;

  /*!
   * \brief Must not be called. Use GetInstance() instead!.
   *
   * \steady FALSE
   */
  ComTrace() noexcept = default;

  ComTrace(ComTrace const&) = delete;
  auto operator=(ComTrace const&) & noexcept -> ComTrace& = delete;
  ComTrace(ComTrace&&) noexcept = delete;
  auto operator=(ComTrace&&) & noexcept -> ComTrace& = delete;

  ~ComTrace() noexcept = default;  // VCA_COMTRACE_EXTERNAL_SAFE_FUNCTION_WITHOUT_ARGUMENT

  // ---- Public APIs ----

  /*!
   * \brief Initialize the ComTrace component singleton instance using the default config JSON location.
   *
   * \return -
   *
   * \error ComTraceErrc::json_parsing_failure  If JSON parsing failed.
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
  auto Initialize() noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Deinitialize the ComTrace component singleton instance.
   *
   * \return Always a positive result.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto Deinitialize() noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Get read-only access to the configuration model hierarchy.
   *
   * \return Read-only access to the configuration model hierarchy.
   *
   * \pre         Component must be in state 'initialized'.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetTraceConfig() const noexcept -> TraceConfigInterface const&;

  /*!
   * \brief   Register an ara::com internal trace client.
   * \details Trace clients: use global ::amsr::comtrace::internal::RegisterTraceClient for registration.
   *
   * \param[in] name          Name of trace client. Must be ara::com unique name.
   * \param[in] async_tracer  Callback interface for asynchronous trace clients. Pass an empty weak_ptr for synchronous
   *                          trace clients.
   *
   * \return ClientTraceInterface for the registered trace client.
   *
   * \pre         Component must be in state 'initialized'.
   * \pre         Maximum number of registered trace clients must not be reached (65535).
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto RegisterTraceClient(StringView name, std::weak_ptr<AsyncClientTraceInterface> async_tracer) noexcept
      -> std::shared_ptr<ClientTraceInterface>;

  /*!
   * \brief Notify ComTrace about completion of an ongoing asynchronous trace operation.
   *
   * \param[in] context_id  The Context ID / handle of the asynchronous trace operation. Only context IDs previously
   *                        received with any of the asynchronous trace point callouts must be returned.
   *
   * \pre         \p context_id must contain a context ID previously provided with any async trace callout function.
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec requires true; \endspec
   */
  auto OnTraceCompleted(ContextId context_id) const noexcept -> void;

 private:
  /*!
   * \brief Type-alias for TraceConfig.
   */
  using TraceConfig = internal::config::TraceConfig;

  /*!
   * \brief Logger instance.
   */
  using ClientId = ::amsr::comtrace::ClientId;

  /*!
   * \brief Infos about a single ara::com internal trace client.
   */
  struct ClientInfo {  // VCA_COMTRACE_STD_WEAK_PTR
    /*!
     * \brief Name of the registered client.
     */
    StringView name{};

    /*!
     * \brief Client ID mapped by the custom tracer to the client.
     */
    ClientId external_client_id{0};

    /*!
     * \brief Asynchronous client callback. Might be empty for synchronous trace clients.
     */
    std::weak_ptr<AsyncClientTraceInterface> async_client_trace_interface{};
  };

  /*!
   * \brief Type-alias for the registered clients container.
   */
  using RegisteredClientsContainer = ara::core::Vector<ClientInfo>;

  /*!
   * \brief Build the TraceConfig configuration model hierarchy by parsing the config JSON.
   *
   * \param[in] trace_config_json_path  File path of the config JSON to be parsed.
   *
   * \return -
   *
   * \error ComTraceErrc::json_parsing_failure  If JSON parsing failed.
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
  auto BuildTraceConfig(StringView trace_config_json_path) noexcept -> ::ara::core::Result<void>;

  /*!
   * \brief Logger.
   */
  Logger const logger_{"ComTrace"};

  /*!
   * \brief The TraceConfig configuration model hierarchy.
   */
  ::ara::core::Optional<TraceConfig> trace_config_{};

  /*!
   * \brief Container of registered ara::com internal trace clients.
   */
  RegisteredClientsContainer registered_clients_{};
};

}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_COM_TRACE_H_
