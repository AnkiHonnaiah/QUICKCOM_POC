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
/**        \file  proxy_method.h
 *        \brief  Proxy method implementation
 *
 *      \details  Contains common components used by all user-defined methods
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_METHODS_PROXY_METHOD_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_METHODS_PROXY_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/future.h"
#include "amsr/core/promise.h"  // necessary for Future::SetExecutionContext
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/proxy_fire_and_forget_method_backend_interface.h"
#include "amsr/socal/internal/methods/proxy_method_backend_interface.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace methods {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief Parameter list of a method.
 * \tparam ArgsT  The parameters of this method call.
 */
template <typename... ArgsT>
struct MethodParameters {
  /*!
   * \brief Fire & Forget method including method arguments.
   *
   * \unit Socal::Proxy::ProxyMethod::ProxyFireAndForgetMethod
   */
  class ProxyFireAndForgetMethod final {
   public:
    /*!
     * \brief Creates a fire-and-forget method.
     * \param[in] proxy_method_backend  The proxy method backend interface.
     * \pre      -
     * \context  App
     * \vprivate
     * \steady FALSE
     * \spec requires true; \endspec
     */
    explicit ProxyFireAndForgetMethod(
        ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<ArgsT...>&
            proxy_method_backend) noexcept
        : proxy_method_backend_{proxy_method_backend} {}

    /*!
     * \brief Call the method implementation of the selected communication binding.
     * \param[in] args  The parameters of this method call.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous TRUE
     * \vpublic
     * \steady TRUE
     * \spec requires true; \endspec
     *
     * \trace SPEC-5951256
     */
    void operator()(ArgsT const&... args) const noexcept {
      // PTP-B-Socal-ProxyMethod_ProxyFireAndForgetMethod_WithArgs
      // VCA_SOCAL_PROXY_METHOD_BACKEND_REF, VCA_SOCAL_PROXY_METHOD_BACKEND_REF_HANDLEREQUEST_ARGUMENTS
      proxy_method_backend_.HandleRequest(std::forward<ArgsT const&>(args)...);
      // PTP-E-Socal-ProxyMethod_ProxyFireAndForgetMethod_WithArgs
    }

   private:
    /*!
     * \brief The communication partner towards the binding.
     */
    ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<ArgsT...>& proxy_method_backend_;
  };

  /*!
   * \brief Generic template class for a method called in a proxy.
   * \tparam ConcreteMethod  To identify the method at server-side.
   *
   * \unit Socal::Proxy::ProxyMethod::ProxyMethod
   */
  template <typename ConcreteMethod>
  class ProxyMethod final {
    /*!
     * \brief Type alias for future executor class type.
     */
    using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

   public:
    /*!
     * \brief Return structure for this method, the proxy expects.
     */
    using Output = typename ConcreteMethod::Output;

    /*!
     * \brief Creates a request-response method.
     * \param[in] proxy_method_backend  The proxy method backend interface.
     * \param[in] future_executor       Executor of the callbacks registered in the context of Future::then.
     * \pre      -
     * \context  App
     * \vprivate
     * \steady FALSE
     * \spec future_executor != nullptr; \endspec
     */
    explicit ProxyMethod(
        ::amsr::socal::internal::methods::ProxyMethodBackendInterface<Output, ArgsT...>& proxy_method_backend,
        ExecutorInterfacePtr future_executor) noexcept
        : proxy_method_backend_{proxy_method_backend},
          future_executor_{std::move(future_executor)} {}  // VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT

    /*!
     * \brief The return value of this method is a future object including the output structure for asynchronous
     *        operation.
     */
    using Return = ::amsr::core::Future<Output>;

    /*!
     * \brief Operation will call the method of the ProxyMethodBackendInterface to propagate the call to the binding.
     * \param[in] args  The parameters of this method call.
     * \return A future object that can be used to query the method response. In polling runtime processing mode, it is
     *         not allowed to register a Future::then() callback on the returned future object.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous FALSE
     * \vpublic
     * \steady TRUE
     * \spec requires true; \endspec
     * \internal
     * - Forward the method request to the backend.
     * - Set the future executor context on the returned future.
     * - If the executor context was not set:
     *   - Log a fatal error message and trigger an abort.
     * - Return the future to the caller.
     * \endinternal
     *
     * \trace SPEC-4980380, SPEC-5951256
     */
    Return operator()(ArgsT const&... args) noexcept {
      // PTP-B-Socal-ProxyMethod_ProxyMethod_WithArgs
      // VCA_SOCAL_PROXY_METHOD_BACKEND_REF, VCA_SOCAL_PROXY_METHOD_BACKEND_REF_HANDLEREQUEST_ARGUMENTS
      Return future{proxy_method_backend_.HandleRequest(std::forward<ArgsT const&>(args)...)};
      bool const execution_context_set{
          future.SetExecutionContext(future_executor_)};  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
      if (!execution_context_set) {
        logger_.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Setting a future execution context failed (invalid future or set twice).";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      return future;
      // PTP-E-Socal-ProxyMethod_ProxyMethod_WithArgs
    }

   private:
    /*!
     * \brief The the proxy communication partner towards the binding.
     */
    ::amsr::socal::internal::methods::ProxyMethodBackendInterface<Output, ArgsT...>& proxy_method_backend_;

    /*!
     * \brief The future executor used for callback execution on Future::then() requests.
     */
    ExecutorInterfacePtr future_executor_;

    /*!
     * \brief An instance of a logger.
     */
    ::amsr::socal::internal::logging::AraComLogger logger_{
        ::amsr::socal::internal::logging::kAraComLoggerContextId,
        ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ProxyMethod"_sv};
  };
};

/*!
 * \brief Specialization for no input parameters.
 */
template <>
struct MethodParameters<void> {
  /*!
   * \brief Fire-and-Forget method without any method arguments.
   *
   * \unit Socal::Proxy::ProxyMethod::ProxyFireAndForgetMethod
   */
  class ProxyFireAndForgetMethod final {
   public:
    /*!
     * \brief Creates a fire-and-forget method that has no input arguments.
     * \param[in] proxy_method_backend  The proxy method backend interface.
     * \pre      -
     * \context  App
     * \vprivate
     * \steady FALSE
     * \spec requires true; \endspec
     */
    explicit ProxyFireAndForgetMethod(
        ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<>& proxy_method_backend) noexcept
        : proxy_method_backend_{proxy_method_backend} {}

    /*!
     * \brief Call the method implementation of the selected communication binding.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous TRUE
     * \vpublic
     * \steady TRUE
     * \spec requires true; \endspec
     *
     * \trace SPEC-5951256
     */
    void operator()() const noexcept {
      // PTP-B-Socal-ProxyMethod_ProxyFireAndForgetMethod_ArgLess
      proxy_method_backend_.HandleRequest();  // VCA_SOCAL_PROXY_METHOD_BACKEND_REF
      // PTP-E-Socal-ProxyMethod_ProxyFireAndForgetMethod_ArgLess
    }

   private:
    /*!
     * \brief The communication partner towards the binding.
     */
    ::amsr::socal::internal::methods::ProxyFireAndForgetMethodBackendInterface<>& proxy_method_backend_;
  };

  /*!
   * \brief Generic template class for a method called by a proxy.
   * \tparam ConcreteMethod  To identify the method at server-side.
   *
   * \unit Socal::Proxy::ProxyMethod::ProxyMethod
   */
  template <typename ConcreteMethod>
  class ProxyMethod final {
    /*!
     * \brief Type alias for future executor class type.
     */
    using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

   public:
    /*!
     * \brief Return structure for this method, the proxy expects.
     */
    using Output = typename ConcreteMethod::Output;

    /*!
     * \brief Creates a request-response method that takes no input arguments to invoke the method call.
     * \param[in] proxy_method_backend  The proxy method backend interface.
     * \param[in] future_executor       Executor of the callbacks registered in the context of Future::then.
     * \pre      -
     * \context  App
     * \vprivate
     * \steady FALSE
     * \spec future_executor != nullptr; \endspec
     */
    explicit ProxyMethod(::amsr::socal::internal::methods::ProxyMethodBackendInterface<Output>& proxy_method_backend,
                         ExecutorInterfacePtr future_executor) noexcept
        : proxy_method_backend_{proxy_method_backend},
          future_executor_{std::move(future_executor)} {}  // VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT

    /*!
     * \brief The return value of this method is a future object including the output structure for asynchronous
     *        operation.
     */
    using Return = ::amsr::core::Future<Output>;

    /*!
     * \brief Operation will call the method of the ProxyMethodBackendInterface to propagate the call to the binding.
     * \return A future object that can be used to query the method response. In polling runtime processing mode, it is
     *         not allowed to register a Future::then() callback on the returned future object.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous FALSE
     * \vpublic
     * \steady TRUE
     * \spec requires true; \endspec
     * \internal
     * - Forward the method request to the backend.
     * - Set the future executor context on the returned future.
     * - If the executor context was not set:
     *   - Log a fatal error message and trigger an abort.
     * - Return the future to the caller.
     * \endinternal
     */
    Return operator()() noexcept {
      // PTP-B-Socal-ProxyMethod_ProxyMethod_ArgLess
      Return future{proxy_method_backend_.HandleRequest()};  // VCA_SOCAL_PROXY_METHOD_BACKEND_REF
      bool const execution_context_set{
          future.SetExecutionContext(future_executor_)};  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
      if (!execution_context_set) {
        logger_.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Setting a future execution context failed (invalid future or set twice).";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      return future;
      // PTP-E-Socal-ProxyMethod_ProxyMethod_ArgLess
    }

   private:
    /*!
     * \brief The communication partner towards the binding.
     */
    ::amsr::socal::internal::methods::ProxyMethodBackendInterface<Output>& proxy_method_backend_;

    /*!
     * \brief The future executor used for callback execution on Future::then() requests.
     */
    ExecutorInterfacePtr future_executor_;

    /*!
     * \brief An instance of a logger.
     */
    ::amsr::socal::internal::logging::AraComLogger logger_{
        ::amsr::socal::internal::logging::kAraComLoggerContextId,
        ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ProxyMethod"_sv};
  };
};

}  // namespace methods
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_METHODS_PROXY_METHOD_H_
