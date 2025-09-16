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
/**        \file  generic_data_ptr.h
 *        \brief  Wrapper over serialized byte stream to support ownership transfer to the user.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_DATA_PTR_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_DATA_PTR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/generic_memory_wrapper_interface.h"

namespace amsr {
namespace socal {

/*!
 * \brief Wrapper over serialized byte stream to support ownership transfer to the user.
 */
class AMSR_LG_BETA GenericDataPtr final {
  /*!
   * \brief Type alias for GenericDataType.
   */
  using GenericDataType = ::amsr::socal::GenericDataType;

  /*!
   * \brief Type alias for the memory wrapper interface pointer.
   */
  using MemoryWrapperInterfacePtr = typename std::unique_ptr<::amsr::socal::internal::GenericMemoryWrapperInterface>;

 public:
  /*!
   * \brief Constructs the generic data pointer.
   * \param[in] memory_wrapper_if_ptr  Pointer to the memory wrapper interface. Must not be nullptr.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires memory_wrapper_if_ptr != nullptr; \endspec
   */
  explicit GenericDataPtr(MemoryWrapperInterfacePtr memory_wrapper_if_ptr) noexcept;

  /*!
   * \brief Destroys the generic data pointer.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~GenericDataPtr() noexcept = default;

  /*!
   * \brief Move constructs the generic data pointer.
   * \param[in] other  GenericDataPtr object which has to be moved.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericDataPtr(GenericDataPtr&& other) noexcept = default;

  /*!
   * \brief Move assigns the generic data pointer.
   * \param[in] other  GenericDataPtr object which has to be moved.
   * \return The move assigned GenericDataPtr object.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericDataPtr& operator=(GenericDataPtr&& other) & noexcept = default;

  GenericDataPtr(GenericDataPtr const&) = delete;
  GenericDataPtr& operator=(GenericDataPtr const&) & = delete;

  /*!
   * \brief Member access operator -> for accessing the underlying data buffer.
   * \return A pointer to the view over the underlying data buffer.
   *         The returned pointer and the view is valid only until the lifetime of *this.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   */
  GenericDataType const* operator->() const noexcept;

  /*!
   * \brief Member indirection operator * for accessing the underlying data buffer.
   * \return A view over the underlying data buffer.
   *         The returned reference and the view is valid only until the lifetime of *this.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  GenericDataType const& operator*() const noexcept;

  /*!
   * \brief Checks if the stored pointer is valid.
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

 private:
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
   * \brief Pointer to the memory wrapper interface.
   */
  MemoryWrapperInterfacePtr memory_ptr_{};

  /*!
   * \brief Read-only view over the serialized data buffers extracted from memory_ptr_.
   */
  GenericDataType data_view_{};
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_GENERIC_DATA_PTR_H_
