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
/*!        \file  find_service_handle.h
 *        \brief  Handle for the FindService request.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_ARA_COM_FIND_SERVICE_HANDLE_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_FIND_SERVICE_HANDLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/socal/internal/instance_specifier_lookup_table.h"

namespace ara {
namespace com {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief  Identifier for a triggered FindService request.
 * \remark If an object of type FindServiceHandle goes out of scope and thus is destroyed, it is no longer possible to
 *         stop the find service activities via a call to StopFindService.
 * \vpublic
 *
 * \trace SPEC-4980258
 * \unit  Socal::Proxy::ProxyServiceDiscovery::AsynchronousProxyFindService
 */
class FindServiceHandle final {  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_CLASS_MEMBERS
 public:
  /*!
   * \brief Internal handle state for synchronization.
   * \vprivate Vector component internal type.
   */
  // VECTOR NC VectorC++-V11.0.3: MD_SOCAL_VectorC++-V11.0.3_public_struct_data_members
  struct State {
    /*!
     * \brief The actual value.
     */
    bool active_{true};

    /*!
     * \brief The lock for the value. This must be recursive due to the fact that application can call StopFindService
     *        inside the FindServiceHandler.
     */
    std::recursive_mutex lock_{};
  };

  /*!
   * \brief Constructor
   * \param[in] service_instances  The searched service instances (represented as lookup table entries).
   * \param[in] state              The shared state. The pointer must not be null.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires state != nullptr; \endspec
   */
  FindServiceHandle(::amsr::socal::internal::InstanceSpecifierLookupTableEntryContainer const& service_instances,
                    std::shared_ptr<State> state) noexcept;

  /*!
   * \brief The copy constructor creates a copy from the provided FindServiceHandle object.
   * \param[in] other  Reference to the object to be copied.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
  FindServiceHandle(FindServiceHandle const& other) = default;

  /*!
   * \brief Use default move constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  FindServiceHandle(FindServiceHandle&&) noexcept = default;  // VCA_SOCAL_CALLING_STL_APIS

  /*!
   * \brief Use default copy assignment.
   * \return Reference of object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980258
   */
  // VCA_SOCAL_CALLING_STL_APIS
  FindServiceHandle& operator=(FindServiceHandle const&) & = default;

  /*!
   * \brief Use default move assignment.
   * \return Reference of object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  FindServiceHandle& operator=(FindServiceHandle&&) & noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
  ~FindServiceHandle() noexcept = default;

  /*!
   * \brief Comparison operator checks if two FindServiceHandle objects are equal.
   * \param[in] rhs  The object to compare with this one.
   * \return true if both FindServiceHandle objects match on proxy_id, instance_id and underlying state; false if any of
   *         these does not match.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980258
   */
  bool operator==(FindServiceHandle const& rhs) const noexcept;

  /*!
   * \brief   Overloads < operator.
   * \details Comparison is done with the following priorities:
   *          1. Proxy ID.
   *          2. The sorted container of service instances using std::set lexicographical_compare: elements compared
   *             sequentially using operator<, stopping at the first occurrence.
   *          3. state
   * \param[in] rhs  The FindServiceHandle to compare against.
   * \return True if *this < rhs else false.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980258
   */
  bool operator<(FindServiceHandle const& rhs) const noexcept;

  /*!
   * \brief   Deactivate the handle.
   * \details The state is used within the proxy to see if the registered callback from StartFindService is still "call-
   *          able". If StopFindService has been called the callback shouldn't be invoked anymore.
   * \pre      -
   * \context  App
   * \vprivate Vector component internal API.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void Deactivate() const noexcept;

  /*!
   * \brief Get the searched service instances.
   * \return Container of searched service instances.
   * \pre      -
   * \context  App
   * \vprivate Vector component internal API.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::socal::internal::InstanceSpecifierLookupTableEntryContainer GetServiceInstances() const noexcept;

 private:
  /*!
   * \brief   Container of service instances searched by the FindServiceHandler.
   * \details Due to required operator< support a sorted set is used for storage of the instances.
   */
  ::amsr::socal::internal::InstanceSpecifierLookupTableEntryContainer service_instances_;

  /*!
   * \brief   State of the handle.
   * \details As soon as the handle is created, the state is set to active. As soon as StopFindService is called, the
   *          handle is set to inactive to signalize that the associated callback must not be called any more.
   */
  std::shared_ptr<State> state_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_FIND_SERVICE_HANDLE_H_

// clang-format off
/*!
 * \exclusivearea ::ara::com::FindServiceHandle::State.lock_
 * Used for protecting the read/write access to the FindServiceHandle state and
 * to synchronize the execution of a FindServiceHandler task against a StopFindService request.
 *
 * \protects ::ara::com::FindServiceHandle::State::active_ is the protected resource of a State object.
 * \usedin ::amsr::socal::internal::ServiceHandleExplorer::ExploreFoundServices
 * \usedin ::amsr::socal::internal::ServiceHandleExplorer::IsActive
 * \usedin ::ara::com::FindServiceHandle::Deactivate
 * \exclude All other methods of a FindServiceHandle object.
 * \length SHORT assigning and reading a variable.
 * \endexclusivearea
 */
// clang-format on
