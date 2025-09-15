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
/*!        \file  generic_sample_ptr.h
 *        \brief  Implementation of generic sample pointer that wraps the serialized event sample data.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_SAMPLE_PTR_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_SAMPLE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/generic_memory_cache_interface.h"

namespace amsr {
namespace socal {

/*!
 * \brief   Implementation of generic sample pointer that wraps the serialized event sample data.
 * \details Different APIs of this class are not thread-safe against each other for the same class instance. The thread
 *          safety of individual APIs are defined at the function level.
 * \vpublic
 */
class AMSR_LG_BETA GenericSamplePtr final {
  /*!
   * \brief Type alias for GenericDataType.
   */
  using GenericDataType = ::amsr::socal::GenericDataType;

  /*!
   * \brief Type alias for the cache interface to return the memory wrapper interface pointer to.
   */
  using CacheType = ::amsr::socal::internal::GenericMemoryCacheInterface;

  /*!
   * \brief Type alias for the memory wrapper interface pointer given to construct this GenericSamplePtr.
   */
  using MemoryWrapperInterfacePtr = typename CacheType::GenericMemoryCacheEntryPtr;

 public:
  /*!
   * \brief Default constructs the GenericSamplePtr.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  constexpr GenericSamplePtr() noexcept = default;

  /*!
   * \brief   Default constructs the GenericSamplePtr.
   * \details Semantically equivalent to Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit constexpr GenericSamplePtr(std::nullptr_t) noexcept {}

  /*!
   * \brief Generic constructor for storing the generic sample.
   * \param[in] memory_wrapper_if_ptr  Pointer to the memory wrapper interface which can be used to access the sample
   *                                   data from the corresponding binding. Must not be nullptr.
   * \param[in] cache_ptr              Pointer to the cache interface where the memory_ptr was taken from.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires memory_wrapper_if_ptr != nullptr; \endspec
   */
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_Socal_M3.2.4_FunctionWithoutDefinitionInLgBetaCode
  explicit GenericSamplePtr(MemoryWrapperInterfacePtr memory_wrapper_if_ptr,
                            std::weak_ptr<CacheType> const& cache_ptr) noexcept;

  /*!
   * \brief Destroys the GenericSamplePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~GenericSamplePtr() noexcept;

  GenericSamplePtr(GenericSamplePtr const&) = delete;

  GenericSamplePtr& operator=(GenericSamplePtr const&) = delete;

  /*!
   * \brief Move constructs the GenericSamplePtr.
   * \param[in] other  GenericSamplePtr object which has to be moved.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericSamplePtr(GenericSamplePtr&& other) noexcept = default;

  /*!
   * \brief Move assigns the GenericSamplePtr.
   * \param[in] other  GenericSamplePtr object which has to be move assigned.
   * \return The move assigned GenericSamplePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericSamplePtr& operator=(GenericSamplePtr&& other) & noexcept;

  /*!
   * \brief Assignment of nullptr_t for GenericSamplePtr.
   * \return The null assigned GenericSamplePtr object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericSamplePtr& operator=(std::nullptr_t) & noexcept;

  /*!
   * \brief Indirection operator * for accessing the serialized sample data.
   * \return Reference to the view over underlying serialized sample data buffers.
   *         The returned reference and the view is valid only until the lifetime of *this or until *this is reset.
   * \pre         The underlying pointer is valid, i.e. Get() != nullptr.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericDataType const& operator*() const noexcept;

  /*!
   * \brief Member access operator -> for accessing the serialized sample data.
   * \return Pointer to the view over underlying serialized sample data buffers.
   *         The returned pointer and the view is valid only until the lifetime of *this or until *this is reset.
   * \pre         The underlying pointer is valid, i.e. Get() != nullptr.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericDataType const* operator->() const noexcept;

  /*!
   * \brief Checks if the stored pointer is null.
   * \return True if the stored pointer is valid, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit operator bool() const noexcept;

  /*!
   * \brief   Swaps the managed object.
   * \details Two instances which are related to two different ProxyEvents shall not be swapped.
   * \param[in] other  GenericSamplePtr object which has to be swapped.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void Swap(GenericSamplePtr& other) noexcept;

  /*!
   * \brief Resets the managed object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void Reset(std::nullptr_t) noexcept;

  /*!
   * \brief Gets the pointer to the stored sample.
   * \return Pointer to the underlying serialized sample if valid, otherwise nullptr.
   *         The returned pointer is valid only until the lifetime of *this or until *this is reset.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericDataType const* Get() const noexcept;

 private:
  /*!
   * \brief Releases the held memory pointer.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  void Release() noexcept;

  /*!
   * \brief Gets the read-only view over the serialized data buffers from the underlying memory pointer.
   * \return Read-only view retrieved from the underlying memory pointer.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  GenericDataType GetData() const noexcept;

  /*!
   * \brief Pointer to the memory wrapper interface which can be used to access the underlying sample data.
   */
  MemoryWrapperInterfacePtr memory_ptr_{};

  /*!
   * \brief Cache pointer memory wrapper was taken from.
   */
  std::weak_ptr<CacheType> cache_ptr_{};

  /*!
   * \brief Read-only view over the serialized data buffers extracted from memory_ptr_.
   */
  GenericDataType data_view_{};
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_SAMPLE_PTR_H_
