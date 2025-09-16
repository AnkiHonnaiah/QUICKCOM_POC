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
/*! \file
 *  \brief Public common BindingType type.
 *  \unit IpcServiceDiscovery::Common::Types
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_BINDING_TYPE_H_
#define LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_BINDING_TYPE_H_

#include <cstdint>
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace common {
namespace internal {

class BindingType {
 public:
  /*!
   * \brief Binding enum.
   */
  enum class Binding : std::uint8_t { kZeroCopy, kIpc };

  /*!
   * \brief Construct an BindingType.
   * \param[in] binding Type of binding.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit BindingType(Binding binding) noexcept : binding_(binding) {}

  constexpr BindingType() = default;

  /*!
   * \brief Getter function for the Binding.
   * \return Binding of the BindingType.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto GetBinding() const noexcept -> Binding { return binding_; }

  /*!
   * \brief Check if BindingType is valid.
   * \details Check if Binding enum class is in bounds.
   * \return True if BindingType is valid, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto IsValid() const noexcept -> bool { return binding_ <= Binding::kIpc; }

  /*!
   * \brief Compare the BindingType against another for equality.
   * \param[in] other BindingType to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(BindingType const& other) const noexcept -> bool {
    return other.GetBinding() == GetBinding();
  }

 private:
  Binding binding_{};
};

/*!
 * \brief Log streaming operator for type BindingType.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] binding_type A const reference to the BindingType.
 * \return The reference to the LogStream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
inline auto operator<<(::ara::log::LogStream& log_stream,
                       BindingType const& binding_type) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "BindingType: ";
  log_stream << "[";
  switch (binding_type.GetBinding()) {
    case BindingType::Binding::kZeroCopy:
      log_stream << "kZeroCopy";
      break;
    case BindingType::Binding::kIpc:
      log_stream << "kIpc";
      break;
    default:
      log_stream << "Unknown binding type";
      break;
  }
  return log_stream << "]";
}

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery
}  // namespace amsr

// VECTOR NC AutosarC++17_10-M14.7.3: MD_IPCSERVICEDISCOVERY_M14.7.3_hashFunctorSpecializationNotDeclaredInSameFile
/*!
 * \brief A hash functor injection into namespace std for BindingType.
 * \details Is used by std::unordered_set and std::unordered_map containers
 *          to hash a BindingType for efficient access.
 * \vprivate Component internal.
 */
template <>
class std::hash<amsr::ipc_service_discovery::common::internal::BindingType> {
 public:
  using BindingType = amsr::ipc_service_discovery::common::internal::BindingType;
  auto operator()(BindingType binding_type) const noexcept -> std::size_t {
    return std::hash<typename BindingType::Binding>{}(binding_type.GetBinding());
  }
};

#endif  // LIB_IPC_SERVICE_DISCOVERY_COMMON_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_COMMON_INTERNAL_TYPES_BINDING_TYPE_H_
