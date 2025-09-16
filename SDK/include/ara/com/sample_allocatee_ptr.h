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
/*!        \file  ara/com/sample_allocatee_ptr.h
 *        \brief  SampleAllocateePtr holding a pointer to the data sample allocated by the bindings.
 *                Used by the application during the skeleton event transmission.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_SAMPLE_ALLOCATEE_PTR_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_SAMPLE_ALLOCATEE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/socal/internal/events/allocated_sample_interface.h"
#include "amsr/unique_ptr.h"

namespace ara {
namespace com {

/*!
 * \brief SampleAllocateePtr implementation.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 *
 * \unit Socal::Skeleton::SkeletonEvent::SampleAllocateePtr
 */
template <typename SampleType>
class SampleAllocateePtr final {
  /*!
   * \brief AllocatedSampleInterface type.
   */
  using AllocatedSampleInterface = ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType>;

  /*!
   * \brief Base sample data type.
   */
  using SampleDataType = typename AllocatedSampleInterface::SampleDataType;

  /*!
   * \brief The underlying allocated sample pointer type.
   */
  using AllocatedSamplePointer = amsr::UniquePtr<AllocatedSampleInterface>;

 public:
  /*!
   * \brief Default constructs the SampleAllocateePtr.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  constexpr SampleAllocateePtr() noexcept = default;

  /*!
   * \brief   Default constructs the SampleAllocateePtr.
   * \details Semantically equivalent to Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit constexpr SampleAllocateePtr(std::nullptr_t) noexcept {}

  /*!
   * \brief Constructs the SampleAllocateePtr with preallocated memory.
   * \param[in] allocated_ptr  Pointer to the data sample allocated by the bindings. This must not be a nullptr.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec allocated_ptr != nullptr; \endspec
   */
  explicit constexpr SampleAllocateePtr(AllocatedSamplePointer allocated_ptr) noexcept
      // VCA_SOCAL_PASSING_POINTERS_OR_REFERENCES_RECEIVED_BASED_ON_FUNCTION_CONTRACT
      : allocated_sample_ptr_{std::move(allocated_ptr)} {}

  /*!
   * \brief Destroys the SampleAllocateePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~SampleAllocateePtr() noexcept = default;  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_CLASS_MEMBERS

  SampleAllocateePtr(SampleAllocateePtr const&) = delete;
  SampleAllocateePtr& operator=(SampleAllocateePtr const&) = delete;

  /*!
   * \brief Move constructs the SampleAllocateePtr.
   * \param[in] other  SampleAllocateePtr object which has to be moved.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  SampleAllocateePtr(SampleAllocateePtr&& other) noexcept = default;

  /*!
   * \brief Move assigns the SampleAllocateePtr.
   * \param[in] other  SampleAllocateePtr object which is used to move-assign from.
   * \return The move assigned SampleAllocateePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_CALLING_STL_APIS
  SampleAllocateePtr& operator=(SampleAllocateePtr&& other) & noexcept = default;

  /*!
   * \brief Assignment of nullptr_t for SampleAllocateePtr.
   * \return The SampleAllocateePtr object that was reset.
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
  SampleAllocateePtr& operator=(std::nullptr_t) & noexcept {
    reset(nullptr);
    return *this;
  }

  /*!
   * \brief   Indirection operator * for getting the sample value.
   * \details The behavior is undefined if the underlying sample is not valid, i.e. get() == nullptr. The
   *          SampleAllocateePtr(s) returned by the SkeletonEvent::Allocate API will always contain a valid sample, but
   *          it is still unsafe to call this function after the SampleAllocateePtr has been reset.
   * \return Reference to the preallocated event sample.
   * \pre         SampleAllocateePtr::get() must return a valid pointer.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT,
  // VCA_SOCAL_POSSIBLY_ACCESSING_NULLPTR_RETRIEVED_FROM_EXTERNAL_FUNCTION
  SampleDataType& operator*() const noexcept { return allocated_sample_ptr_->operator*(); }

  /*!
   * \brief   Member access operator -> for accessing the values inside the sample.
   * \details The behavior is undefined if the underlying sample is not valid, i.e. get() == nullptr. The
   *          SampleAllocateePtr(s) returned by the SkeletonEvent::Allocate API will always contain a valid sample, but
   *          it is still unsafe to call this function after the SampleAllocateePtr has been reset.
   * \return Pointer to the preallocated event sample.
   * \pre         SampleAllocateePtr::get() must return a valid pointer.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT,
  // VCA_SOCAL_POSSIBLY_ACCESSING_NULLPTR_RETRIEVED_FROM_EXTERNAL_FUNCTION
  SampleDataType* operator->() const noexcept { return allocated_sample_ptr_->operator->(); }

  /*!
   * \brief Resets the managed object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void reset(std::nullptr_t = nullptr) noexcept { allocated_sample_ptr_.reset(); }  // VCA_SOCAL_CALLING_STL_APIS

  /*!
   * \brief Swaps the managed object.
   * \param[in] other  SampleAllocateePtr object which has to be swapped.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void swap(SampleAllocateePtr& other) noexcept { allocated_sample_ptr_.swap(other.allocated_sample_ptr_); }

  /*!
   * \brief Gets the pointer to the stored sample.
   * \return Pointer to the stored sample if valid, otherwise nullptr.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the underlying sample is valid, get the data pointer from the bindings.
   * - Otherwise, return nullptr.
   * \endinternal
   */
  SampleDataType* get() const noexcept {
    SampleDataType* sample_ptr{nullptr};
    if (allocated_sample_ptr_) {
      // VCA_SOCAL_DEREFERENCING_POINTER_WITH_RUNTIME_CHECK
      sample_ptr = allocated_sample_ptr_->operator->();
    }
    return sample_ptr;
  }

  /*!
   * \brief Checks whether the stored pointer is null.
   * \return True if the stored pointer is valid, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit operator bool() const noexcept { return static_cast<bool>(allocated_sample_ptr_); }

  /*!
   * \brief Compares the SampleAllocateePtr with nullptr.
   * \return True if the stored pointer is not nullptr, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool operator!=(std::nullptr_t) const noexcept { return (allocated_sample_ptr_ != nullptr); }

  /*!
   * \brief Gets the underlying preallocated sample pointer.
   * \return The underlying preallocated sample pointer.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  AllocatedSamplePointer GetAllocatedSamplePtr() noexcept { return std::move(allocated_sample_ptr_); }

 private:
  /*!
   * \brief Preallocated sample pointer given by the bindings.
   */
  AllocatedSamplePointer allocated_sample_ptr_{};
};

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_SAMPLE_ALLOCATEE_PTR_H_
