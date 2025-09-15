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
/**        \file  generic_proxy_field.h
 *        \brief  Generic proxy field implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/fields/generic_proxy_field_getter.h"
#include "amsr/socal/fields/generic_proxy_field_notifier.h"
#include "amsr/socal/fields/generic_proxy_field_setter.h"
#include "amsr/socal/internal/fields/generic_proxy_field_backend_interface.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace socal {
namespace fields {

/*!
 * \brief Generic proxy field.
 */
class AMSR_LG_BETA GenericProxyField final {
  /*!
   * \brief Type alias for generic proxy field backend.
   */
  using GenericProxyFieldBackend = ::amsr::socal::internal::fields::GenericProxyFieldBackendInterface;

  /*!
   * \brief Type alias for future executor class type.
   */
  using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

 public:
  /*!
   * \brief Constructs a generic proxy field.
   * \param[in] field_backend    The corresponding field backend. The given reference must be valid at least until the
   *                             lifetime of this object.
   * \param[in] service_name     The shortname path of the service interface this field belongs to.
   * \param[in] field_name       The field name configured in the model.
   * \param[in] instance_id      The instance identifier of the corresponding proxy instance.
   * \param[in] future_executor  Executor that triggers the callbacks registered with Future::then.
   *                             The given pointer must not be null.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires future_executor != nullptr; \endspec
   */
  GenericProxyField(GenericProxyFieldBackend& field_backend, std::string const& service_name,
                    std::string const& field_name, ::ara::com::InstanceIdentifier const& instance_id,
                    ExecutorInterfacePtr future_executor) noexcept;
  /*!
   * \brief Destroys the generic proxy field.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  ~GenericProxyField() noexcept = default;

  GenericProxyField(GenericProxyField const&) = delete;
  GenericProxyField(GenericProxyField&&) = delete;
  GenericProxyField& operator=(GenericProxyField const&) = delete;
  GenericProxyField& operator=(GenericProxyField&&) = delete;

 private:
  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_;

 public:
  /*!
   * \brief Notifier for the field.
   * \vpublic
   */
  GenericProxyFieldNotifier notifier;  // VECTOR SL VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private

  /*!
   * \brief Getter for the field.
   * \vpublic
   */
  GenericProxyFieldGetter get;  // VECTOR SL VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private

  /*!
   * \brief Setter for the field.
   * \vpublic
   */
  GenericProxyFieldSetter set;  // VECTOR SL VectorC++-V11.0.2: MD_SOCAL_VectorC++-V11-0-2_Shall_be_private
};

}  // namespace fields
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_H_
