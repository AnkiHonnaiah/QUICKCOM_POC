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
/*!        \file  amsr/socal/r20_11/events/sample_ptr.h
 *        \brief  Contains SamplePtr implementation as defined by the AUTOSAR_SWS_CommunicationManagement R20-11 with
 *                configurable extensions for handling time stamps in samples.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_SAMPLE_PTR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_SAMPLE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>
#include <type_traits>
#include <utility>

#include "amsr/socal/internal/events/cache_interface.h"
#include "amsr/socal/internal/events/timestamp_config.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/e2e_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace r20_11 {
namespace events {
namespace internal {

/*!
 * \brief ConditionalTimeStamp is a template base class for handling conditional Timestamps in the SamplePtr.
 * \tparam IsTimeStampEnabled  True if the SamplePtr is extended with a timestamp (extension to AUTOSAR), False
 *                             otherwise.
 *
 * \unit Socal::Proxy::ProxyEvent::SamplePtr__r20_11
 */
template <bool IsTimeStampEnabled>
class ConditionalTimeStamp;

/*!
 * \brief ConditionalTimeStamp<true> is a base class for handling conditional Timestamps of the SamplePtr, when
 *        Timestamps are enabled.
 *
 * \unit Socal::Proxy::ProxyEvent::SamplePtr__r20_11
 */
template <>
class ConditionalTimeStamp<true> {
  /*!
   * \brief Type alias for time stamp type.
   */
  using TimeStamp = ::amsr::socal::internal::events::TimeStamp;

 protected:
  /*!
   * \brief Constructor for setting the value of the timestamp.
   * \param[in] time_stamp  Timestamp value for the sample.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit ConditionalTimeStamp(TimeStamp const& time_stamp) noexcept : time_stamp_{time_stamp} {}

  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  constexpr ConditionalTimeStamp() = default;

  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~ConditionalTimeStamp() noexcept = default;

  /*!
   * \brief   Default constructs the ConditionalTimeStamp.
   * \details Semantically equivalent to Default constructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit constexpr ConditionalTimeStamp(std::nullptr_t) noexcept {}

  ConditionalTimeStamp(ConditionalTimeStamp const&) noexcept = delete;

  ConditionalTimeStamp& operator=(ConditionalTimeStamp const&) & noexcept = delete;

  /*!
   * \brief Move constructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ConditionalTimeStamp(ConditionalTimeStamp&&) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object.
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ConditionalTimeStamp& operator=(ConditionalTimeStamp&&) & noexcept = default;

  /*!
   * \brief Time stamp for when receiving a message.
   */
  // VECTOR NL VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private
  TimeStamp time_stamp_{};
};

/*!
 * \brief ConditionalTimeStamp<false> is a base class for handling conditional Timestamps of the SamplePtr, when
 *        Timestamps are disabled.
 *
 * \unit Socal::Proxy::ProxyEvent::SamplePtr__r20_11
 */
template <>
class ConditionalTimeStamp<false> {
 protected:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  constexpr ConditionalTimeStamp() = default;

  /*!
   * \brief   Default constructs the ConditionalTimeStamp.
   * \details Semantically equivalent to Default constructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit constexpr ConditionalTimeStamp(std::nullptr_t) noexcept {}

  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~ConditionalTimeStamp() noexcept = default;

  ConditionalTimeStamp(ConditionalTimeStamp const&) noexcept = delete;

  ConditionalTimeStamp& operator=(ConditionalTimeStamp const&) & noexcept = delete;

  /*!
   * \brief Move constructor.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ConditionalTimeStamp(ConditionalTimeStamp&&) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object.
   * \pre     -
   * \context App
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ConditionalTimeStamp& operator=(ConditionalTimeStamp&&) & noexcept = default;
};

}  // namespace internal

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief   SamplePtr implementation as defined by the AUTOSAR_SWS_CommunicationManagement AP R20-11.
 * \details Different APIs of this class are not thread-safe against each other for the same class instance. The thread
 *          safety of individual APIs are defined at the function level.
 * \tparam SampleType              Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimestampConfiguration  Controls if the SamplePtr is extended with a timestamp (extension to AUTOSAR).
 * \vpublic
 *
 * \trace SPEC-4980263
 * \unit  Socal::Proxy::ProxyEvent::SamplePtr__r20_11
 */
template <typename SampleType, typename TimestampConfiguration = ::amsr::socal::internal::events::TimeStampDisabled>
class SamplePtr final : public internal::ConditionalTimeStamp<TimestampConfiguration::IsEnabled> {
 public:
  /*!
   * \brief Base sample data type.
   */
  using SampleDataType = std::remove_cv_t<std::remove_reference_t<SampleType>>;

  /*!
   * \brief Type alias for the memory wrapper interface pointer given to construct this SamplePtr.
   */
  using MemoryWrapperInterfacePtrType =
      typename ::amsr::socal::internal::events::CacheInterface<SampleType>::CacheEntryType;

  /*!
   * \brief Type alias for the cache interface to return the memory wrapper interface pointer to.
   */
  using CacheType = ::amsr::socal::internal::events::CacheInterface<SampleDataType>;

  /*!
   * \brief Type alias for the E2E protection check status.
   */
  using ProfileCheckStatus = ::ara::com::e2e::ProfileCheckStatus;

  /*!
   * \brief Type alias for the TimeStamp value.
   */
  using TimeStamp = ::amsr::socal::internal::events::TimeStamp;

  /*!
   * \brief Inheriting the base class constructor.
   */
  using internal::ConditionalTimeStamp<TimestampConfiguration::IsEnabled>::ConditionalTimeStamp;

  /*!
   * \brief Type alias for the Base class.
   */
  using BaseType = internal::ConditionalTimeStamp<TimestampConfiguration::IsEnabled>;

  /*!
   * \brief Default constructs the SamplePtr.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  constexpr SamplePtr() = default;

  /*!
   * \brief   Default constructs the SamplePtr.
   * \details Semantically equivalent to Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit constexpr SamplePtr(std::nullptr_t) noexcept : BaseType{nullptr} {}

  /*!
   * \brief Generic constructor for storing the deserialized sample and E2E check status.
   * \tparam    Config                    Configures if time stamp is enabled or disabled.
   * \param[in] memory_wrapper_if_ptr     Pointer to the memory wrapper interface which can be used to access the sample
   *                                      data from the corresponding binding. Must be nullptr, if and only if,
   *                                      e2e_check_status equals to 'Error'.
   * \param[in] cache_ptr                 Pointer to the cache interface where the memory_ptr was taken from.
   * \param[in] e2e_profile_check_status  E2E profile check status for the sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec cache_ptr != nullptr; \endspec
   * \internal
   * - If given memory_ptr is invalid,
   *   - Reset the cache_ptr_.
   *
   * - Calls "ara::core::Abort()" if:
   *   - Memory_ptr is nullptr and e2e_check_status is not equal to ProfileCheckStatus::Error.
   *   - Memory_ptr is not equal to nullptr and e2e_check_status is equal to ProfileCheckStatus::Error.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_SOCAL_AutosarC++17_10-A12.1.5_delegating_constructor_for_common_init
  explicit SamplePtr(
      MemoryWrapperInterfacePtrType&& memory_wrapper_if_ptr, std::weak_ptr<CacheType> cache_ptr,
      std::enable_if_t<Config::IsEnabled == false, ProfileCheckStatus> const e2e_profile_check_status) noexcept
      : BaseType{},
        memory_ptr_{std::move(memory_wrapper_if_ptr)},
        // VCA_SOCAL_VALID_MOVE_CONSTRUCTION
        cache_ptr_{std::move(cache_ptr)},
        e2e_profile_check_status_{e2e_profile_check_status} {
    if (memory_ptr_ == nullptr) {
      cache_ptr_.reset();  // VCA_SOCAL_CALLING_STL_APIS

      if (e2e_profile_check_status_ != ProfileCheckStatus::Error) {
        ::amsr::socal::internal::logging::AraComLogger const logger{
            ::amsr::socal::internal::logging::kAraComLoggerContextId,
            ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr20-11"_sv};

        logger.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Creating SamplePtr with nullptr is only allowed, if E2E profile check status is 'Error'.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else if (e2e_profile_check_status_ == ProfileCheckStatus::Error) {  // Memory Ptr must be nullptr if e2e profile
                                                                          // check status is Error.
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr20-11"_sv};

      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid construction of SamplePtr with E2E profile check status 'Error'.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // Do nothing.
    }
  }

  /*!
   * \brief Generic constructor for storing the deserialized sample along with E2E check status and time stamp.
   * \tparam    Config                    Configures if time stamp is enabled or disabled.
   * \param[in] memory_wrapper_if_ptr     Pointer to the memory wrapper interface which can be used to access the sample
   *                                      data from the corresponding binding. Must be nullptr, if and only if,
   *                                      e2e_check_status equals to 'Error'.
   * \param[in] cache_ptr                 Pointer to the cache interface where the memory_ptr was taken from.
   * \param[in] e2e_profile_check_status  E2E profile check status for the sample.
   * \param[in] time_stamp                Timestamp value for the sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec cache_ptr != nullptr; \endspec
   * \internal
   * - If given memory_ptr is invalid,
   *   - Reset the cache_ptr_.
   *
   * - Calls "ara::core::Abort()" if:
   *   - Memory_ptr is nullptr and e2e_check_status is not equal to ProfileCheckStatus::Error.
   *   - Memory_ptr is not equal to nullptr and e2e_check_status is equal to ProfileCheckStatus::Error.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  explicit SamplePtr(MemoryWrapperInterfacePtrType&& memory_wrapper_if_ptr, std::weak_ptr<CacheType> cache_ptr,
                     ProfileCheckStatus const e2e_profile_check_status,
                     std::enable_if_t<Config::IsEnabled, TimeStamp> const& time_stamp) noexcept
      : BaseType{time_stamp},
        memory_ptr_{std::move(memory_wrapper_if_ptr)},
        // VCA_SOCAL_VALID_MOVE_CONSTRUCTION
        cache_ptr_{std::move(cache_ptr)},
        e2e_profile_check_status_{e2e_profile_check_status} {
    if (memory_ptr_ == nullptr) {
      cache_ptr_.reset();  // VCA_SOCAL_CALLING_STL_APIS

      if (e2e_profile_check_status_ != ProfileCheckStatus::Error) {
        ::amsr::socal::internal::logging::AraComLogger const logger{
            ::amsr::socal::internal::logging::kAraComLoggerContextId,
            ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr20-11"_sv};

        logger.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Creating SamplePtr with nullptr is only allowed, if E2E profile check status is 'Error'.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else if (e2e_profile_check_status_ == ProfileCheckStatus::Error) {  // Memory Ptr must be nullptr if e2e profile
                                                                          // check status is Error.
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr20-11"_sv};

      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid construction of SamplePtr with E2E profile check status 'Error'.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // Do nothing.
    }
  }

  /*!
   * \brief Destroys the SamplePtr object.
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

  SamplePtr(SamplePtr const&) = delete;

  SamplePtr& operator=(SamplePtr const&) = delete;

  /*!
   * \brief Move constructs the SamplePtr.
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
   * \brief Move assigns the SamplePtr.
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
   * - Swap content of this SamplePtr with other.
   * \endinternal
   */
  SamplePtr& operator=(SamplePtr&& other) & noexcept {
    if (this != &other) {
      Swap(other);
      other.Reset(nullptr);
    }
    return *this;
  };

  /*!
   * \brief Assignment of nullptr_t for SamplePtr.
   * \return The null assigned SamplePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Reset the managed object.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.2.1: MD_SOCAL_A10.2.1_redefinitionOfNonVirtualFunction
  SamplePtr& operator=(std::nullptr_t) & noexcept {
    Reset(nullptr);
    return *this;
  }

  /*!
   * \brief   Indirection operator * for getting the sample value.
   * \details The behavior is undefined if the underlying sample is not valid, i.e. Get() == nullptr or
   *          SamplePtr::GetProfileCheckStatus() returns ProfileCheckStatus::Error. It is unsafe to call this function
   *          after the given SamplePtr has been reset.
   * \return The actual sample value.
   * \pre         Must not be called.
   *              - On \p SamplePtrs created via default constructor prior to being swapped with a valid \p SamplePtr,
   *              - On \p SamplePtrs created via explicit constructor taking a nullptr, Reset \p SamplePtrs, E2E related
   *                \p SamplePtrs if SamplePtr::GetProfileCheckStatus() returns ProfileCheckStatus::Error.
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
  SampleDataType const& operator*() const noexcept {
    if (!memory_ptr_) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr20-11"_sv};

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
   * \details The behavior is undefined if the underlying sample is not valid, i.e. Get() == nullptr or
   *          SamplePtr::GetProfileCheckStatus() returns ProfileCheckStatus::Error. It is unsafe to call this function
   *          after the given SamplePtr has been reset.
   * \return Pointer to the underlying sample.
   * \pre         Must not be called on \p SamplePtrs created via default constructor prior to being swapped with a
   *              valid \p SamplePtr, on \p SamplePtrs created via explicit constructor taking a nullptr, Reset \p
   *              SamplePtrs, E2E related \p SamplePtrs if SamplePtr::GetProfileCheckStatus() returns
   *              ProfileCheckStatus::Error.
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
  SampleDataType const* operator->() const noexcept {
    if (!memory_ptr_) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SamplePtr20-11"_sv};

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

  /*!
   * \brief Checks if the stored pointer is null.
   * \return True if the stored pointer is valid, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit operator bool() const noexcept { return static_cast<bool>(memory_ptr_); }

  /*!
   * \brief   Swaps the managed object.
   * \details Two instances which related to two different ProxyEvents shall not be swapped.
   * \tparam    Config  Configures if the time stamp is enabled or disabled.
   * \param[in] other   SamplePtr object which has to be swapped.
   * \return -
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Swap the contents of memory_ptr_ and e2e_profile_check_status_ with the given SamplePtr object.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  auto Swap(SamplePtr& other) noexcept -> std::enable_if_t<Config::IsEnabled != true, void> {
    memory_ptr_.swap(other.memory_ptr_);
    cache_ptr_.swap(other.cache_ptr_);
    std::swap(e2e_profile_check_status_, other.e2e_profile_check_status_);
  }

  /*!
   * \brief   Swaps the managed object including time stamp.
   * \details Two instances which related to two different ProxyEvents shall not be swapped.
   * \tparam    Config  Configures if the time stamp is enabled or disabled. Defaults to TimeStampDisabled.
   * \param[in] other   SamplePtr object which has to be swapped.
   * \return -
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Swap the contents of memory_ptr_, e2e_profile_check_status_ and time stamp with the given SamplePtr object.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  auto Swap(SamplePtr& other) noexcept -> std::enable_if_t<Config::IsEnabled, void> {
    memory_ptr_.swap(other.memory_ptr_);
    cache_ptr_.swap(other.cache_ptr_);
    std::swap(e2e_profile_check_status_, other.e2e_profile_check_status_);
    std::swap(internal::ConditionalTimeStamp<Config::IsEnabled>::time_stamp_,
              other.internal::ConditionalTimeStamp<Config::IsEnabled>::time_stamp_);
  }

  /*!
   * \brief Gets the timestamp value of the stored sample, if enabled.
   * \tparam Config  Configures if the time stamp is enabled or disabled.
   * \return The timestamp of the stored sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  template <typename Config = TimestampConfiguration>
  auto GetTimeStamp() const noexcept -> std::enable_if_t<Config::IsEnabled, TimeStamp> {
    return internal::ConditionalTimeStamp<Config::IsEnabled>::time_stamp_;
  }

  /*!
   * \brief Resets the managed object without time stamp.
   * \tparam Config  Configures if the time stamp is enabled or disabled. Defaults to TimeStampDisabled.
   * \return -
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
   * - Assign nullptr to memory_ptr.
   * - Reset the state of e2e_profile_check_status_.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  auto Reset(std::nullptr_t) noexcept -> std::enable_if_t<Config::IsEnabled == false, void> {
    Release();
    memory_ptr_ = nullptr;  // VCA_SOCAL_CALLING_STL_APIS
    e2e_profile_check_status_ = ProfileCheckStatus::NotAvailable;
  }

  /*!
   * \brief Resets the managed object with time stamp.
   * \tparam Config  Configures if the time stamp is enabled or disabled. Defaults to TimeStampDisabled.
   * \return -
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
   * - Assign nullptr to memory_ptr.
   * - Reset the states e2e_profile_check_status_ and time_stamp_.
   * \endinternal
   */
  template <typename Config = TimestampConfiguration>
  auto Reset(std::nullptr_t) noexcept -> std::enable_if_t<Config::IsEnabled, void> {
    Release();
    memory_ptr_ = nullptr;  // VCA_SOCAL_CALLING_STL_APIS
    e2e_profile_check_status_ = ProfileCheckStatus::NotAvailable;
    internal::ConditionalTimeStamp<Config::IsEnabled>::time_stamp_ = TimeStamp::min();
  }

  /*!
   * \brief Gets the pointer to the stored sample.
   * \return Pointer to the stored sample if valid, otherwise nullptr.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If memory_ptr_ is valid
   *   - Return a pointer to the stored sample.
   * - Otherwise
   *   - Return a nullptr.
   * \endinternal
   */
  SampleDataType const* Get() const noexcept {
    SampleDataType const* sample_ptr{nullptr};
    if (memory_ptr_) {
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
      // VCA_SOCAL_DEREFERENCING_POINTER_WITH_RUNTIME_CHECK
      sample_ptr = memory_ptr_->operator->();
    }
    return sample_ptr;
  }

  /*!
   * \brief Gets the end 2 end protection check result of the stored sample.
   * \return The E2E profile check status of the stored sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980265, SPEC-13650595, SPEC-13650595
   */
  ProfileCheckStatus GetProfileCheckStatus() const noexcept { return e2e_profile_check_status_; }

 private:
  /*!
   * \brief Releases the held MemoryWrapper.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If memory_ptr_ is not a nullptr
   *   - If the visible cache object referenced by cache_ptr_ still exists:
   *     - Give the memory_ptr_ back to the backend.
   *   - Reset the cache_ptr_.
   * \endinternal
   */
  void Release() {
    // VECTOR NC AutosarC++17_10-A7.1.1: MD_Socal_AutosarC++17_10-A7.1.1_Immutable_shall_be_qualified_const_false_positive
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOCAL_AutosarC++17_10-A18.5.8_FalsePositive
    if (memory_ptr_) {
      std::shared_ptr<CacheType> valid_cache_ptr{cache_ptr_.lock()};
      if (valid_cache_ptr) {
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
        // VCA_SOCAL_DEREFERENCING_POINTER_WITH_RUNTIME_CHECK
        valid_cache_ptr->ReturnEntry(std::move(memory_ptr_));
        // VCA_SOCAL_CALLING_STL_APIS
        valid_cache_ptr.reset();
      }
      // VECTOR NC AutosarC++17_10-A12.8.3: MD_Socal_AutosarC++17_10-A12.8.3_read_access_moved_from_object_falsepositive
      cache_ptr_.reset();  // VCA_SOCAL_CALLING_STL_APIS
      // VCA_SOCAL_CALLING_STL_APIS
    }
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
   * \brief E2E profile check status of the stored sample.
   */
  ProfileCheckStatus e2e_profile_check_status_{ProfileCheckStatus::NotAvailable};
};

}  // namespace events
}  // namespace r20_11
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_SAMPLE_PTR_H_
