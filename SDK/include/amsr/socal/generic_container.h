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
/**        \file  generic_container.h
 *        \brief  Generic container implementation.
 *      \details  Wrapper over amsr::core::Map that provides read-only access to the map after construction.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_CONTAINER_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_CONTAINER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <memory>
#include <utility>
#include "amsr/core/abort.h"
#include "amsr/core/map.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/com_error_domain.h"
#include "ara/core/string_view.h"

namespace amsr {
namespace socal {

/*!
 * \brief   Generic container to be used by the generic proxy to store all the configured events, methods and fields.
 * \details Wrapper over amsr::core::Map that provides read-only access to the map after construction.
 * \tparam T  Type of the contained element.
 */
template <typename T>
class AMSR_LG_BETA GenericContainer final {
 public:
  /*!
   * \brief Provides access to the underlying container element.
   */
  class ElementAccess final {
   public:
    /*!
     * \brief Container access result type.
     */
    using ResultType = ::amsr::core::Result<T&>;

    /*!
     * \brief Constructs a container element access object.
     * \param[in] result  Result of the container access.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vprivate    Component Private.
     * \steady FALSE
     */
    explicit ElementAccess(ResultType&& result) noexcept : result_{std::move(result)} {}

    /*!
     * \brief Destroys the container element access object.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, FALSE for different instances.
     * \synchronous TRUE
     * \vpublic
     * \steady FALSE
     */
    ~ElementAccess() noexcept = default;

    /*!
     * \brief Copy-constructs a container element access object.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vpublic
     * \steady FALSE
     */
    ElementAccess(ElementAccess const&) noexcept = default;

    /*!
     * \brief Move-constructs a container element access object.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vpublic
     * \steady FALSE
     */
    ElementAccess(ElementAccess&&) noexcept = default;

    ElementAccess& operator=(ElementAccess const&) = delete;
    ElementAccess& operator=(ElementAccess&&) = delete;

    /*!
     * \brief Check whether the access object is valid.
     * \return True if the access is valid, false otherwise.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, FALSE for different instances.
     * \synchronous TRUE
     * \vpublic
     * \steady TRUE
     * \spec requires true; \endspec
     */
    explicit operator bool() const noexcept { return result_.HasValue(); }

    /*!
     * \brief Indirection operator * for accessing the underlying container element.
     * \return Reference to the underlying container element.
     *         The returned reference is valid only until the corresponding proxy object is valid.
     * \pre         This access object is valid, i.e. bool(*this) == true.
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, FALSE for different instances.
     * \synchronous TRUE
     * \vpublic
     * \steady TRUE
     * \internal
     * - If the access is valid, return the reference to the underlying container element.
     * - Otherwise, abort the application.
     * \endinternal
     */
    T& operator*() noexcept {
      if (not result_.HasValue()) {
        ::amsr::socal::internal::logging::AraComLogger const logger{
            ::amsr::socal::internal::logging::kAraComLoggerContextId,
            ::amsr::socal::internal::logging::kAraComLoggerContextDescription,
            ::amsr::core::StringView{"GenericContainer"}};

        logger.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Attempt to access an invalid generic container element!";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      return result_.Value();  // VCA_SOCAL_RETURNING_VALID_REFERENCE_BASED_ON_CDD
    }

    /*!
     * \brief Member access operator -> for accessing the underlying container element.
     * \return Pointer to the underlying container element.
     *         The returned pointer is valid only until the corresponding proxy object is valid.
     * \pre         This access object is valid, i.e. bool(*this) == true.
     * \context     App
     * \threadsafe  TRUE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, FALSE for different instances.
     * \synchronous TRUE
     * \vpublic
     * \steady TRUE
     * \internal
     * - If the access is valid, return the pointer to the underlying container element.
     * - Otherwise, abort the application.
     * \endinternal
     */
    T* operator->() noexcept {
      if (not result_.HasValue()) {
        ::amsr::socal::internal::logging::AraComLogger const logger{
            ::amsr::socal::internal::logging::kAraComLoggerContextId,
            ::amsr::socal::internal::logging::kAraComLoggerContextDescription,
            ::amsr::core::StringView{"GenericContainer"}};

        logger.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Attempt to access an invalid generic container element!";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      return std::addressof(result_.Value());
    }

   private:
    /*!
     * \brief Result of the container access.
     */
    ResultType result_;
  };

  /*!
   * \brief Underlying container type.
   */
  using ContainerType = ::amsr::core::Map<::amsr::core::StringView, std::unique_ptr<T>>;

  /*!
   * \brief Constructs a generic container with the given map container.
   * \param[in] container  The underlying container object.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  explicit GenericContainer(ContainerType&& container) noexcept : container_{std::move(container)} {}

  /*!
   * \brief Destroys the generic container.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  ~GenericContainer() noexcept = default;

  /*!
   * \brief Move-constructs a generic container.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  GenericContainer(GenericContainer&&) noexcept = default;

  GenericContainer(GenericContainer const&) = delete;
  GenericContainer& operator=(GenericContainer const&) = delete;
  GenericContainer& operator=(GenericContainer&&) = delete;

  /*!
   * \brief Subscript operator to access the underlying container element by its short-name string.
   * \param[in] short_name  The short-name of the element to search.
   * \return Access object containing a valid container element if the given short-name exists,
   *         otherwise an empty access object.
   *         The returned Access object is valid only until the lifetime of the corresponding proxy object. Accessing it
   *         after destroying the proxy object will lead to undefined behavior.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ElementAccess operator[](::ara::core::StringView const& short_name) const noexcept {
    typename ElementAccess::ResultType result{::ara::com::ComErrc::kErrorNotOk};
    typename ContainerType::const_iterator const it{container_.find(short_name)};
    if (it != container_.cend()) {
      assert(it->second != nullptr);
      result.EmplaceValue(*(it->second));  // VCA_SOCAL_PASSING_VALID_REFERENCE_BASED_ON_CDD
    }
    return ElementAccess{std::move(result)};
  }

  /*!
   * \brief Clears the container elements.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  void Clear() noexcept { container_.clear(); }

 private:
  /*!
   * \brief Underlying container object.
   */
  ContainerType container_{};
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_CONTAINER_H_
