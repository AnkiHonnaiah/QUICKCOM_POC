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
 *         \brief  Provider for monotonically increasing unique numbers.
 *
 *       \details
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_UNIQUE_NUMBER_PROVIDER_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_UNIQUE_NUMBER_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief  Provider for monotonically increasing unique numbers.
 * \unit   ZeroCopyBinding::UniqueNumberProvider
 * \vprivate Component private
 */
class UniqueNumberProvider final {
 public:
  using NumberType = std::size_t;
  /*!
   * \brief         Constructor of UniqueNumberProvider.
   * \pre           -
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  UniqueNumberProvider() noexcept = default;

  /*!
   * \brief         Default destructor of UniqueNumberProvider.
   * \pre           -
   * \context       Shutdown
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  ~UniqueNumberProvider() noexcept = default;

  UniqueNumberProvider(UniqueNumberProvider const&) = delete;
  auto operator=(UniqueNumberProvider const&) -> UniqueNumberProvider& = delete;
  UniqueNumberProvider(UniqueNumberProvider&&) = delete;
  auto operator=(UniqueNumberProvider&&) -> UniqueNumberProvider& = delete;

  /*!
   * \brief         Get the next unique value.
   * \return        The next unique value.
   * \context       Any
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  auto GetNextValue() noexcept -> NumberType;

 private:
  /*!
   * \brief The atomic counter.
   */
  std::atomic_size_t counter_{0U};
};

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_UNIQUE_NUMBER_PROVIDER_H_
