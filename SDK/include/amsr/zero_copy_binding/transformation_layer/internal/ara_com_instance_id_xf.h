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
 *         \brief  Provides utilities to map between ara::com InstanceIdentifier and the ServiceInstanceIdentifier
 *                 from the Ipc service discovery.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingTransformationLayer::AraComInstanceIdXf
 *
 *   \complexity  The complexity is increased by many inter module calls to ::ara::com, ::ara::core and
 *                ::amsr::charconv. Those are necessary for the correct functionality of the AraComInstanceIdXf and
 *                cannot be omitted as the unit's task is to map an identifier from another module to a ZeroCopyBinding
 *                identifier and vice versa. These inter-dependencies are by design and thoroughly tested.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ARA_COM_INSTANCE_ID_XF_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ARA_COM_INSTANCE_ID_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <type_traits>

#include "amsr/core/result.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "ara/com/instance_identifier.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_ZEROCOPYBINDING_M7.3.6_usingDirective
using ::vac::container::literals::operator""_sv;

/*!
 * \brief AraComInstanceIdXf class to translate between ::ara::com::InstanceIdentifier and
 *        ::amsr::ipc_service_discovery::internal::ServiceInstanceIdentifier.
 */
class AraComInstanceIdXf final {
 public:
  AraComInstanceIdXf() = delete;
  AraComInstanceIdXf(AraComInstanceIdXf const&) = delete;
  AraComInstanceIdXf(AraComInstanceIdXf&&) = delete;
  auto operator=(AraComInstanceIdXf const&) -> AraComInstanceIdXf& = delete;
  auto operator=(AraComInstanceIdXf&&) -> AraComInstanceIdXf& = delete;
  ~AraComInstanceIdXf() = delete;

  /*!
   * \brief Type-alias for the ServiceInstanceIdentifier from the Ipc service discovery.
   */
  using ServiceInstanceIdentifier =
      ::amsr::zero_copy_binding::common::internal::service_discovery::ServiceInstanceIdentifier;

  /*!
   * \brief       Parse the ServiceInstanceIdentifier from a Socal InstanceIdentifier.
   *
   * \param[in]   instance_identifier The InstanceIdentifier to be parsed.
   * \return      The parsed ServiceInstanceIdentifier.
   * \error       ::amsr::generic::GenErrc::kRuntime in case the InstanceIdentifier format is not correct.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  static auto BuildServiceInstanceIdentifier(::ara::com::InstanceIdentifier const& instance_identifier) noexcept
      -> ::amsr::core::Result<ServiceInstanceIdentifier>;

  /*!
   * \brief       Builder function for an instance identifier string.
   * \details     The function prepares and returns an instance identifier string from a given instance id.
   *
   * \param[in]   service_instance_identifier Instance identifier from the Ipc service discovery.
   * \return      Instance identifier in the format:
   *              "ZeroCopy:<Service ID>:<Instance ID>:<Major Version>.<Minor Version>".
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  static auto BuildAraComInstanceIdentifier(ServiceInstanceIdentifier const& service_instance_identifier) noexcept
      -> ::ara::com::InstanceIdentifier;

 private:
  using ServiceIdType = common::internal::config::ServiceId::Type;
  using InstanceIdType = common::internal::config::InstanceId::Type;
  using MajorVersionType = common::internal::config::MajorVersion::Type;
  using MinorVersionType = common::internal::config::MinorVersion::Type;

  /*!
   * \brief ZeroCopyBinding specific string prefix for ara::com InstanceIdentifier.
   */
  static constexpr ::amsr::core::StringView kBindingPrefix{"ZeroCopy:"_sv};

  /*!
   * \brief The separators that are used in the ara::com InstanceIdentifier.
   */
  static constexpr ::amsr::core::StringView separators{"::."_sv};

  /*!
   * \brief   A helper that determines the maximum number of digits an integer of type T can have in decimal format.
   * \tparam  T  An integer type.
   */
  template <typename T>
  struct GetMaxDigits {
    static constexpr std::size_t value{
        std::numeric_limits<std::enable_if_t<std::numeric_limits<T>::is_integer, T>>::digits10 + 1U};
  };

  /*!
   * \brief Maximal amount of digits of the used integer type of the ZeroCopyBinding ServiceId in decimal system.
   */
  static constexpr std::size_t kMaxDigitsServiceId{GetMaxDigits<ServiceIdType>::value};

  /*!
   * \brief Maximal amount of digits of the used integer type of the ZeroCopyBinding InstanceId in decimal system.
   */
  static constexpr std::size_t kMaxDigitsInstanceId{GetMaxDigits<InstanceIdType>::value};

  /*!
   * \brief Maximal amount of digits of the used integer type of the ZeroCopyBinding MajorVersion in decimal system.
   */
  static constexpr std::size_t kMaxDigitsMajorVersion{GetMaxDigits<MajorVersionType>::value};

  /*!
   * \brief Maximal amount of digits of the used integer type of the ZeroCopyBinding MinorVersion in decimal system.
   */
  static constexpr std::size_t kMaxDigitsMinorVersion{GetMaxDigits<MinorVersionType>::value};
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ARA_COM_INSTANCE_ID_XF_H_
