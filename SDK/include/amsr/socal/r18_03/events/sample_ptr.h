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
/*!        \file  socal/include/amsr/socal/r18_03/events/sample_ptr.h
 *        \brief  SamplePtr header file.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_SAMPLE_PTR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_SAMPLE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/socal/internal/events/cache_interface.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/e2e_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace r18_03 {
namespace events {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief   SamplePtr implementation.
 * \details Different APIs of this class are not thread-safe against each other for the same class instance. The thread
 *          safety of individual APIs are defined at the function level.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \vpublic
 *
 * \trace SPEC-4980263
 * \unit  Socal::Proxy::ProxyEvent::SamplePtr__r18_03
 */
template <typename SampleType>
class SamplePtr final {
  /*!
   * \brief Base sample type.
   */
  using SampleDataType = typename ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType>::SampleDataType;

  /*!
   * \brief Type alias for the memory wrapper interface pointer given to construct this SamplePtr.
   */
  using MemoryWrapperInterfacePtrType =
      typename ::amsr::socal::internal::events::CacheInterface<SampleType>::CacheEntryType;

  /*!
   * \brief Type alias for the cache interface to return the memory wrapper interface pointer to.
   */
  using CacheType = ::amsr::socal::internal::events::CacheInterface<SampleDataType>;

 public:
  /*!
   * \brief Generic constructor for creating the memory and setting the value of the sample.
   * \param[in] memory_ptr        Pointer to the memory wrapper interface which can be used to access the sample data
   *                              from the corresponding binding. Must be nullptr, if and only if, e2e_check_status
   *                              equals to 'Error'.
   * \param[in] cache_ptr         Pointer to the cache interface where the memory_ptr was taken from.
   * \param[in] e2e_check_status  E2E check status.
   * \pre      -
   * \context  App
   * \vprivate
   * \steady TRUE
   * \spec cache_ptr != nullptr; \endspec
   * \internal
   * - If given memory_ptr is invalid
   *   - Reset the cache_ptr_.
   *
   * - Calls "ara::core::Abort()" if:
   *   - Memory_ptr is nullptr and e2e_check_status is not equal to E2ECheckStatus::Error.
   *   - Memory_ptr is not equal to nullptr and e2e_check_status is equal to E2ECheckStatus::Error.
   * \endinternal
   */
  explicit SamplePtr(MemoryWrapperInterfacePtrType&& memory_ptr, std::weak_ptr<CacheType> cache_ptr,
                     ::ara::com::E2E_state_machine::E2ECheckStatus const e2e_check_status) noexcept
      // VCA_SOCAL_VALID_MOVE_CONSTRUCTION, VCA_SOCAL_CALLING_STL_APIS
      : memory_ptr_{std::move(memory_ptr)},
        // VCA_SOCAL_VALID_MOVE_CONSTRUCTION, VCA_SOCAL_CALLING_STL_APIS
        cache_ptr_{std::move(cache_ptr)},
        // VCA_SOCAL_SCOPED_ENUM_OUT_OF_BOUND, VCA_SOCAL_ACCESSING_ABSOLUTE_ADDRESS_FALSE_POSITIVE,
        // VCA_SOCAL_POSSIBLE_USE_AFTER_FREE_FALSE_POSITIVE, VCA_SOCAL_DEREFERENCING_NULLPTR_FALSE_POSITIVE
        e2e_check_status_{e2e_check_status} {  // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_CONSTRUCTOR
    if (memory_ptr_ == nullptr) {
      // VCA_SOCAL_CALLING_STL_APIS
      cache_ptr_.reset();

      // if memory_ptr_ is nullptr, E2ECheckStatus is expected as Error.
      if (e2e_check_status_ != ::ara::com::E2E_state_machine::E2ECheckStatus::Error) {
        ::amsr::socal::internal::logging::AraComLogger const logger{
            ::amsr::socal::internal::logging::kAraComLoggerContextId,
            ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr18-03"_sv};

        logger.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Creating SamplePtr with nullptr is only allowed, if E2E check status is 'Error'.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else if (e2e_check_status_ ==
               ::ara::com::E2E_state_machine::E2ECheckStatus::Error) {  // Memory Ptr must be nullptr if e2e check
                                                                        // status is Error.
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr18-03"_sv};

      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid construction of SamplePtr with E2E check status 'Error'.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // Do nothing.
    }
  }

  /*!
   * \brief   Copy constructs the SamplePtr.
   * \details This API is not threadsafe to comply with memory-returning mechanism of the backend.
   * \param[in] other  SamplePtr object which will be copied.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  SamplePtr(SamplePtr const& other) noexcept = default;

  /*!
   * \brief   Move constructs the SamplePtr.
   * \details This API is not threadsafe to comply with memory-returning mechanism of the backend.
   * \param[in] other  SamplePtr object which has to be moved.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_ACCESS_IS_POSSIBLY_OUT_OF_BOUND_FALSE_POSITIVE
  // VCA_SOCAL_ACCESSING_ABSOLUTE_ADDRESS_FALSE_POSITIVE
  // VCA_SOCAL_VALID_MOVE_CONSTRUCTION
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
  // VCA_SOCAL_POSSIBLE_USE_AFTER_FREE_FALSE_POSITIVE
  // VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
  SamplePtr(SamplePtr&& other) noexcept = default;

  /*!
   * \brief   Copy assigns the SamplePtr.
   * \details This API is not threadsafe to comply with memory-returning mechanism of the backend, see DSGN-Socal-
   *          Events-UpdateEventCache.
   * \param[in] other  SamplePtr object which will be copy assigned from.
   * \return The copy assigned SamplePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Call Release() to return memory_ptr_ to backend.
   * - Copy-assign all members of other to this element.
   * \endinternal
   */
  SamplePtr& operator=(SamplePtr const& other) & noexcept {
    if (this != &other) {
      Release();
      // VCA_SOCAL_CALLING_STL_APIS
      memory_ptr_ = other.memory_ptr_;
      // VCA_SOCAL_CALLING_STL_APIS
      cache_ptr_ = other.cache_ptr_;
      e2e_check_status_ = other.e2e_check_status_;
    }
    return *this;
  };

  /*!
   * \brief   Move assigns the SamplePtr.
   * \details This API is not threadsafe to comply with memory-returning mechanism of the backend, see DSGN-Socal-
   *          Events-UpdateEventCache.
   * \param[in] other  SamplePtr object which has to be move assigned.
   * \return The move assigned SamplePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Call Release() to return memory_ptr_ to backend.
   * - Move-assign all members of other to this element.
   * \endinternal
   */
  SamplePtr& operator=(SamplePtr&& other) & noexcept {
    if (this != &other) {
      Release();
      // VCA_SOCAL_CALLING_STL_APIS
      memory_ptr_ = std::move(other.memory_ptr_);
      // VCA_SOCAL_CALLING_STL_APIS
      cache_ptr_ = std::move(other.cache_ptr_);
      e2e_check_status_ = other.e2e_check_status_;
    }
    return *this;
  };

  /*!
   * \brief   Destroys the SamplePtr object.
   * \details This API is not threadsafe to comply with memory-returning mechanism of the backend, see DSGN-Socal-
   *          Events-UpdateEventCache.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Call Release() to return memory_ptr_ to backend.
   * \endinternal
   */
  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_CLASS_MEMBERS
  ~SamplePtr() { Release(); }

  /*!
   * \brief E2E Status for this sample.
   * \return the E2E check status for this concrete sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980265, SPEC-4980060, SPEC-4980265
   */
  ::ara::com::E2E_state_machine::E2ECheckStatus GetE2ECheckStatus() const noexcept { return e2e_check_status_; }

  /*!
   * \brief   Indirection operator * for getting the sample value.
   * \details The behavior is undefined if the underlying sample is not valid.
   * \return the actual sample value.
   * \pre         This API must not be called on E2E related \p SamplePtrs if SamplePtr::GetE2ECheckStatus() returns
   *              E2ECheckStatus::Error.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * Calls "ara::core::Abort()" if:
   * - Memory wrapper pointer is nullptr.
   * \endinternal
   */
  SampleType const& operator*() const noexcept {
    if (!memory_ptr_) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr18-03"_sv};

      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Accessing SamplePtr with invalid memory-wrapper-pointer.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
    // VCA_SOCAL_DEREFERENCING_POINTER_WITH_RUNTIME_CHECK
    return memory_ptr_->operator*();
  }

  /*!
   * \brief   Member access operator -> for accessing the values inside the sample.
   * \details The behavior is undefined if the underlying sample is not valid.
   * \return the actual sample value.
   * \pre         This API must not be called on E2E related \p SamplePtrs if SamplePtr::GetE2ECheckStatus() returns
   *              E2ECheckStatus::Error.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * Calls "ara::core::Abort()" if:
   * - Memory wrapper pointer is nullptr.
   * \endinternal
   */
  SampleType* operator->() const noexcept {
    if (!memory_ptr_) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr18-03"_sv};

      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Accessing SamplePtr with invalid memory-wrapper-pointer.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
    // VCA_SOCAL_DEREFERENCING_POINTER_WITH_RUNTIME_CHECK
    return memory_ptr_->operator->();
  }

 private:
  /*!
   * \brief   Releases the held MemoryWrapper.
   * \details No nullptr check is done since a SamplePtr can only be constructed from a valid MemoryWrapperInterface
   *          object. This API is not threadsafe to comply with memory-returning mechanism of the backend, see DSGN-
   *          Socal-Events-UpdateEventCache.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the visible cache object referenced by cache_ptr_ still exists:
   *   - Give the memory_ptr_ back to the backend.
   * - Reset the cache_ptr_.
   * \endinternal
   */
  void Release() {
    // VECTOR NC AutosarC++17_10-A7.1.1: MD_Socal_AutosarC++17_10-A7.1.1_Immutable_shall_be_qualified_const_false_positive
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOCAL_AutosarC++17_10-A18.5.8_FalsePositive
    std::shared_ptr<CacheType> valid_cache_ptr{cache_ptr_.lock()};
    if (valid_cache_ptr) {
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
      // VCA_SOCAL_DEREFERENCING_POINTER_WITH_RUNTIME_CHECK
      valid_cache_ptr->ReturnEntry(std::move(memory_ptr_));
      // VCA_SOCAL_CALLING_STL_APIS
      valid_cache_ptr.reset();
    }
    // VECTOR NC AutosarC++17_10-A12.8.3: MD_Socal_AutosarC++17_10-A12.8.3_read_access_moved_from_object_falsepositive
    // VCA_SOCAL_CALLING_STL_APIS
    cache_ptr_.reset();
    // VCA_SOCAL_CALLING_STL_APIS
  }
  /*!
   * \brief Pointer to the memory wrapper interface which can be used to access the underlying sample data.
   */
  MemoryWrapperInterfacePtrType memory_ptr_{};

  /*!
   * \brief Cache pointer memory wrapper was taken from.
   */
  std::weak_ptr<CacheType> cache_ptr_{};

  /*!
   * \brief E2ECheckStatus of one sample.
   */
  ::ara::com::E2E_state_machine::E2ECheckStatus e2e_check_status_;
};

}  // namespace events
}  // namespace r18_03
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_SAMPLE_PTR_H_
