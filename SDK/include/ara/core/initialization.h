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
/**        \file  initialization.h
 *        \brief  Provides ara::core::Initialize() and ara::core::Deinitialize().
 *
 *      \details  Provides the global initialization and shutdown functions that initialize resp. deinitialize data
 *                structures and threads of the AUTOSAR Runtime for Adaptive Applications (ARA).
 *         \unit  ApplicationBase::InitializationDeinitialization
 *   \complexity  Calls the initialization and deinitialization functions of respective components.
 *                Complexity of the functions are analyzed in the scope of the components.
 *
 *********************************************************************************************************************/

#ifndef LIB_APPLICATION_BASE_INCLUDE_ARA_CORE_INITIALIZATION_H_
#define LIB_APPLICATION_BASE_INCLUDE_ARA_CORE_INITIALIZATION_H_

#include "amsr/application_base/initialization_error_domain.h"

#include "ara/core/result.h"

namespace ara {
namespace core {

// VECTOR NL AutosarC++17_10-A2.8.2: MD_APP_BASE_A2.8.2_SectionsOfCodeShallNotBeCommentedOut
/*!
 * \brief      (Pre-)Initialization of the ARA Framework.
 * \details    Prior to this call, no interaction with the ARA (Adaptive Runtime for Applications) is allowed with the
 *             exception of constant initializations and types which are trivially destructible. It is required to make
 *             this call in a place where it is guaranteed that static memory initialization has completed
 *             (e.g. inside of main function). When Initialize() has returned successfully, calling it again will return
 *             an error. When Initialize() returns with an error, the application may report the error cause and exit by
 *             using an appropriate return code. Calling Deinitialize() in this case is not necessary.
 * \note       Calling any ARA functions before calling this function may have unforeseen consequences.
 * \remarks    The function body is generated.
 *
 * \code
 *              // ...
 *              #include "ara/core/initialization.h"
 *
 *              int main(int argc, char* argv[]) {
 *                // static variables/objects initialized
 *                // setup signal handling, ...
 *                ara::core::Result<void> init_result {ara::core::Initialize()};
 *                // init_result.HasValue() != true -> ARA can not be used
 *                // init_result.HasValue() == true -> ARA can be used
 *                if(init_result.HasValue()) {
 *                  // ... business logic ...
 *
 *                  // all threads accessing ARA APIs joined
 *                  ara::core::Result<void> deinit_result {ara::core::Deinitialize()};
 *                  deinit_result.InspectError([](ara::core::ErrorCode const& error) {
 *                    std::cerr << "ara::core::Deinitialize() failed!";
 *                    std::cerr << "Result contains: " << error.Message() << ", " << error.UserMessage();
 *                  });
 *                }
 *                return init_result.HasValue() ? 0 : -1;
 *              }  // static variables/objects will be deinitialized
 * \endcode
 *
 * \vpublic
 *
 * \threadsafe FALSE
 * \reentrant  FALSE
 * \error      InitializationErrc::kWrongSequence
 *             Initialize() is called in a wrong sequence (e.g. twice consecutively)
 *
 * \trace      DSGN-ApplicationBase-InitializationDeinitialization
 * \returns    an initialized Result<void> on success.
 */
Result<void> Initialize() noexcept;

// VECTOR NL AutosarC++17_10-A2.8.2: MD_APP_BASE_A2.8.2_SectionsOfCodeShallNotBeCommentedOut
/*!
 * \brief      Shutdown of the ARA Framework.
 * \details    When Initialize() has returned successfully, Deinitialize() has to be called.
 *             After this call, no interaction with the ARA (Adaptive Runtime for Application) is allowed
 *             (with the exception of constant initializations and types which are trivial destructible).
 *             As a prerequisite to calling this API it is expected that the use of ARA interfaces is completed
 *             (with the given exceptions). It is required to make this call in a place where it is guaranteed that the
 *             static initialization has completed and destruction of statically initialized data has not yet started
 *             (e.g. inside of main function).
 * \pre        ara::core::Initialize() has returned successfully.
 * \pre        All threads accessing ARA APIs joined.
 * \note       Calling any ARA functions after calling this function may have unforeseen consequences.
 *             When Deinitialize() returns with an error, it is generally unsafe to call either Deinitialize()
 *             or Initialize() again.
 * \remarks    The function body is generated.
 * \code
 *             // ...
 *             #include "ara/core/initialization.h"
 *
 *             int main(int argc, char* argv[]) {
 *               // static variables/objects initialized
 *               // setup signal handling, ...
 *               ara::core::Result<void> init_result {ara::core::Initialize()};
 *               // init_result.HasValue() != true -> ARA can not be used
 *               // init_result.HasValue() == true -> ARA can be used
 *               if(init_result.HasValue()) {
 *                 // ... business logic ...
 *
 *                 // all threads accessing ARA APIs joined
 *                 ara::core::Result<void> deinit_result {ara::core::Deinitialize()};
 *                 deinit_result.InspectError([](ara::core::ErrorCode const& error) {
 *                   std::cerr << "ara::core::Deinitialize() failed!";
 *                   std::cerr << "Result contains: " << error.Message() << ", " << error.UserMessage();
 *                 });
 *               }
 *             }  // static variables/objects will be deinitialized
 * \endcode
 * \vpublic
 *
 * \threadsafe FALSE
 * \reentrant  FALSE
 * \error      InitializationErrc::kWrongSequence
 *             Deinitialize() is called in a wrong sequence (e.g. Deinitialize() is called prior to Initialize()).
 *
 * \trace      DSGN-ApplicationBase-InitializationDeinitialization
 * \returns    an initialized Result<void> on success.
 */
Result<void> Deinitialize() noexcept;

namespace details {

/*!
 * \brief Defines the reachable initialization states.
 */
enum class InitializationState : std::uint8_t { kUninitialized, kInitialized, kDeinitialized };

/*!
 * \brief The internal initialization state of ara::core.
 */
extern InitializationState g_initialization_state;

}  // namespace details

}  // namespace core
}  // namespace ara

#endif  // LIB_APPLICATION_BASE_INCLUDE_ARA_CORE_INITIALIZATION_H_
