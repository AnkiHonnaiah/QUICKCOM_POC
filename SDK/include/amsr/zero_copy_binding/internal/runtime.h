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
/*!        \file
 *         \brief  Runtime implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_RUNTIME_H_
#define LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/instance_specifier.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/runtime.h"
#include "amsr/socal/internal/types.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace zero_copy_binding {
namespace internal {

/*!
 * \brief   Runtime to connect to the SOCAL runtime.
 * \unit    ZeroCopyBinding::ZeroCopyBinding::Runtime
 */
class Runtime {
 public:
  /*!
   * \brief         Default constructor of the runtime.
   *
   * \context       ANY
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate Component private
   */
  Runtime() = default;

  /*!
   * \brief         Map an InstanceIdentifier to the corresponding R-Port InstanceSpecifier.
   *
   * \param[in]     instance_specifier The instance specifier which will be the key of the new entry.
   * \param[in]     instance_identifier The instance identifier which will be the value for the new entry.
   *                                    Expected format: "<binding_name>:<binding specific instance ID>".
   * \pre           The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate Component private
   */
  void MapRequiredInstanceSpecifierToInstanceId(::amsr::core::InstanceSpecifier const& instance_specifier,
                                                ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief         Map an InstanceIdentifier to the corresponding P-Port InstanceSpecifier.
   *
   * \param[in]     instance_specifier The instance specifier which will be the key of the new entry.
   * \param[in]     instance_identifier The instance identifier which will be the value for the new entry.
   *                                    Expected format: "<binding_name>:<binding specific instance ID>".
   * \pre           The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate Component private
   */
  void MapProvidedInstanceSpecifierToInstanceId(::amsr::core::InstanceSpecifier const& instance_specifier,
                                                ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief         Removes the given InstanceIdentifier from the required instance specifier table.
   * \param[in]     instance_specifier The instance specifier which will be the key for the entry to be removed.
   * \param[in]     instance_identifier The instance identifier which will be the value for the entry to be removed.
   * \pre           The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context       Shutdown
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate Component private
   */
  void RemoveRequiredInstanceSpecifierEntry(::amsr::core::InstanceSpecifier const& instance_specifier,
                                            ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

  /*!
   * \brief         Removes the given InstanceIdentifier from the provided instance specifier table.
   * \param[in]     instance_specifier The instance specifier which will be the key for the entry to be removed.
   * \param[in]     instance_identifier The instance identifier which will be the value for the entry to be removed.
   * \pre           The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context       Shutdown
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate Component private
   */
  void RemoveProvidedInstanceSpecifierEntry(::amsr::core::InstanceSpecifier const& instance_specifier,
                                            ::ara::com::InstanceIdentifier const& instance_identifier) noexcept;

 private:
  /*!
   * \brief The Socal runtime Singleton to be used for the calls of the Binding runtime.
   */
  ::amsr::generic::SingletonAccess<::amsr::socal::internal::Runtime> socal_runtime_{
      ::amsr::socal::internal::Runtime::getSingletonInstance().GetAccess()};

  /*!
   * \brief The logger instance.
   */
  core::internal::logging::ZeroCopyLogger logger_{"Runtime"};  // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
};

}  // namespace internal
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_RUNTIME_H_
