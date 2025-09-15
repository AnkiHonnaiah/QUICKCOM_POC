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
/**     \file       app_base.h
 *      \brief      Application Base singleton.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_APP_BASE_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_APP_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/application_base/initialization_error_domain.h"
#include "amsr/application_base/internal/default_resource.h"
#include "amsr/application_base/internal/default_resource_config.h"
#include "amsr/application_base/internal/utils.h"
#include "amsr/generic/singleton_wrapper.h"
#include "ara/core/optional.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace application_base {
namespace internal {
/*!
 * \brief The ApplicationBase singleton.
 */
class AppBase final {
  /*!
   * \brief Default Resource instance.
   */
  Optional<DefaultResource> memory_resource_{};

  /*!
   * \brief Flag to indicate if singleton is initalized.
   */
  bool is_initialized_{false};

  /*!
   * \brief Pointer to the previous default memory resource.
   */
  ara::core::MemoryResource* old_memory_resource_{nullptr};

 public:
  /*!
   * \brief   Initialize the singleton.
   *
   * \details Creates a DefaultResource if the environment variables for a DefaultResource are correctly configured.
   *          Sets the DefaultResource as the default memory resource in LibVac.
   *
   * \return Nothing on success.
   */
  auto Initialize() noexcept -> Result<void> {
    Result<void> result{};

    if (is_initialized_) {
      result.EmplaceError(
          MakeErrorCode(InitializationErrc::kWrongSequence, {},
                        "amsr::application_base::internal::AppBase::Initialize: Singleton is already initialized."));
    } else {
      static_cast<void>(ParseDefaultResourceConfig().Consume([this](DefaultResourceConfig config) {
        memory_resource_.emplace(config.size, config.expanding);
        old_memory_resource_ = ara::core::SetDefaultResource(memory_resource_->AsMemoryResource());
      }));

      is_initialized_ = true;
    }

    return result;
  }

  /*!
   * \brief   Deinitialize the singleton.
   *
   * \details If the DefaultResouce is configured, release the memory of the DefaultResource and
   *          reset the default memory resource in LibVac.
   *
   * \return Nothing on success.
   */
  auto Deinitialize() noexcept -> Result<void> {
    Result<void> result{};

    if (not is_initialized_) {
      result.EmplaceError(
          MakeErrorCode(InitializationErrc::kWrongSequence, {},
                        "amsr::application_base::internal::AppBase::Deinitialize: Singleton is not initalized."));
    } else {
      if (memory_resource_) {
        static_cast<void>(ara::core::SetDefaultResource(old_memory_resource_));

        // Release memory
        memory_resource_.reset();
      }

      old_memory_resource_ = nullptr;
      is_initialized_ = false;
    }

    return result;
  }

  /*!
   * \brief Gets the size of the MonotonicBufferResource of the DefaultResource.
   *
   * \return Optional with the size or an empty Optional if no resource was configured.
   */
  auto GetDefaultResourceSize() const noexcept -> Optional<std::size_t> {
    Optional<std::size_t> size{};
    if (memory_resource_) {
      size.emplace(memory_resource_->Size());
    }
    return size;
  }

 private:
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__AppBase__WithResource, InitializeSucceeds);
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__AppBase__WithResource, DeinitializeSucceeds);
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__AppBase__NoResource, InitializeSucceeds);
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__AppBase__NoResource, DeinitializeSucceeds);
};
namespace detail {
/*!
 * \brief Application Base singleton wrapper.
 */
using AppBaseSingleton = generic::Singleton<AppBase>;

/*!
 * \brief Get the singleton of ApplicationBase.
 */
inline auto GetSingleton() noexcept -> AppBaseSingleton& {
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_APP_BASE_AutosarC++17_10-A3.3.2_needed_singleton
  static AppBaseSingleton g_appbase{};
  return g_appbase;
}
}  // namespace detail

/*!
 * \brief Application Base singleton access.
 */
using AppBaseInstance = generic::SingletonAccess<AppBase>;

/*!
 * \brief   Get the instance of the Application Base Singleton.
 *
 * \pre     The ApplicationBase singleton must have been created.
 *
 * \return  Access to the singleton.
 */
inline auto GetInstance() noexcept -> AppBaseInstance { return detail::GetSingleton().GetAccess(); }
}  // namespace internal
}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_APP_BASE_H_