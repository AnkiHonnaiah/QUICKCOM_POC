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
/*!       \file infinity_size_t.h
 *        \brief Size T that can sefely represent infinity. Done because ara::core::Result is not correctly constexpr.
 *        \unit SomeIpProtocol::Serdes::InfSizeT
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SIZE_T_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SIZE_T_H_

#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief A class that is a size_t that can represent and retain infinity.
 * \details Its goal is not to be mathematically correct (e.g when multiplying
 *          two infinities having inf^2), just being able to saturate at infinity when
 *          doing serialization calculations.
 */
class InfSizeT final {
 public:
  /*!
   * \brief     Constructor.
   * \param[in] value Value of new instance.
   * \pre       -
   * \context   ANY
   */
  // VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  constexpr explicit InfSizeT(std::size_t value) noexcept : value_{value} {};

  /*!
   * \brief   Default Constructor.
   * \pre     -
   * \context ANY
   */
  constexpr InfSizeT() noexcept = default;

  /*!
   * \brief     Copy constructor.
   * \param[in] value Value of new instance.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT(InfSizeT const& value) noexcept = default;

  /*!
   * \brief     Move constructor.
   * \param[in] value Value of new instance.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT(InfSizeT&& value) noexcept = default;

  /*!
   * \brief     Copy assignment operator.
   * \param[in] value Value of new instance.
   * \return    Instance of InfSizeT.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT& operator=(InfSizeT const& value) & noexcept = default;

  /*!
   * \brief     Move assignment operator.
   * \param[in] value Value of new instance.
   * \return    Instance of InfSizeT.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT& operator=(InfSizeT&& value) & noexcept = default;

  /*!
   * \brief   Default destructor
   * \pre     -
   * \context ANY
   */
  ~InfSizeT() = default;

  /*!
   * \brief   Create an instance set to infinity.
   * \return  The value, if not infinity. Otherwise std::size_t max.
   * \pre     -
   * \context ANY
   */
  static constexpr InfSizeT Infinity() noexcept {
    InfSizeT r{0u};
    r.is_infinity_ = true;
    return r;
  }

  /*!
   * \brief   Get the numeric value.
   * \return  The numeric value, if not infinity. If infinity, std::size_t max.
   * \pre     -
   * \context ANY
   */
  // VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  constexpr std::size_t Value() const noexcept { return !IsInfinity() ? value_ : ~std::size_t{}; }

  /*!
   * \brief   Check if infinity.
   * \return  True if infinity. False otherwise.
   * \pre     -
   * \context ANY
   */
  constexpr bool IsInfinity() const noexcept { return is_infinity_; }

  /*!
   * \brief     Operator += for InfSizeT.
   * \param[in] rhs Left hand value.
   * \return    This InfSizeT object, with value increased by rhs.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT& operator+=(InfSizeT rhs) & noexcept {
    value_ += rhs.value_;
    is_infinity_ = is_infinity_ || rhs.is_infinity_;
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
  /*!
   * \brief     Operator += for size_t.
   * \param[in] rhs Left hand value.
   * \return    This InfSizeT object, with value increased by rhs.
   * \pre       -
   * \context   ANY
   */
  // VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  constexpr InfSizeT& operator+=(std::size_t rhs) & noexcept {
    *this += InfSizeT{rhs};
    return *this;
  }

  /*!
   * \brief     Operator + for InfSizeT.
   * \param[in] rhs Left hand value.
   * \return    New InfSizeT object that is the result of the multiplication.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT operator+(InfSizeT rhs) const& noexcept {
    InfSizeT r{*this};
    r += rhs;
    return r;
  }

  /*!
   * \brief     Operator -= for InfSizeT.
   * \param[in] rhs Left hand value.
   * \return    This InfSizeT object, with value lowered by rhs.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT& operator-=(InfSizeT rhs) & noexcept {
    value_ -= rhs.value_;
    is_infinity_ = is_infinity_ || rhs.is_infinity_;
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
  /*!
   * \brief     Operator -= for size_t.
   * \param[in] rhs Left hand value.
   * \return    This InfSizeT object, with value subtracted by rhs.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT& operator-=(std::size_t rhs) & noexcept {
    *this -= InfSizeT{rhs};
    return *this;
  }

  /*!
   * \brief     Operator - for InfSizeT.
   * \param[in] rhs Left hand value.
   * \return    New InfSizeT object that is equal to lhs minus rhs.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT operator-(InfSizeT rhs) const& noexcept {
    InfSizeT r{*this};
    r -= rhs;
    return r;
  }

  /*!
   * \brief     Operator *= for InfSizeT.
   * \param[in] rhs Left hand value.
   * \return    This InfSizeT object, multiplied by rhs.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT& operator*=(InfSizeT rhs) & noexcept {
    value_ *= rhs.value_;
    is_infinity_ = is_infinity_ || rhs.is_infinity_;
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
  /*!
   * \brief     Operator *= for size_t.
   * \param[in] rhs Left hand value.
   * \return    This InfSizeT object, multiplied by rhs.
   * \pre       -
   * \context   ANY
   */
  // VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  constexpr InfSizeT& operator*=(std::size_t rhs) & noexcept {
    *this *= InfSizeT{rhs};
    return *this;
  }

  /*!
   * \brief     Operator * for InfSizeT.
   * \param[in] rhs Left hand value.
   * \return    New InfSizeT object that is the result of the multiplication.
   * \pre       -
   * \context   ANY
   */
  constexpr InfSizeT operator*(InfSizeT rhs) const& noexcept {
    InfSizeT r{*this};
    r *= rhs;
    return r;
  }

 private:
  /*!
   * \brief Size value.
   */
  // VECTOR AV NC Architecture-ApiAnalysis Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  std::size_t value_{0u};

  /*!
   * \brief Infinity state.
   */
  bool is_infinity_{false};
};

// VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
/*!
 * \brief     Operator = for size_t and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is the result of the addition.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr InfSizeT operator+(std::size_t lhs, InfSizeT rhs) noexcept { return InfSizeT{lhs} + rhs; }

// VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
/*!
 * \brief     Operator + for InfSizeT and size_t.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is the result of the addition.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr InfSizeT operator+(InfSizeT lhs, std::size_t rhs) noexcept { return lhs + InfSizeT{rhs}; }

// VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
/*!
 * \brief     Operator - for size_t and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is equal to lhs minus rhs.
 * \pre       -
 * \context   ANY
 */
inline constexpr InfSizeT operator-(std::size_t lhs, InfSizeT rhs) noexcept { return InfSizeT{lhs} - rhs; }

// VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
/*!
 * \brief     Operator - for InfSizeT and size_t.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is equal to lhs minus rhs.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr InfSizeT operator-(InfSizeT lhs, std::size_t rhs) noexcept { return lhs - InfSizeT{rhs}; }

// VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
/*!
 * \brief     Operator * for size_t and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is the result of the multiplication.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr InfSizeT operator*(std::size_t lhs, InfSizeT rhs) noexcept { return InfSizeT{lhs} * rhs; }

// VECTOR Next Construct AutosarC++17_10-M5.17.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.17.1_missing_binary_overload
/*!
 * \brief     Operator * for InfSizeT and size_t.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is the result of the multiplication.
 * \pre       -
 * \context   ANY
 */
inline constexpr InfSizeT operator*(InfSizeT lhs, std::size_t rhs) noexcept { return lhs * InfSizeT{rhs}; }

/*!
 * \brief     Operator == for InfSizeT and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    True if equal. False otherwise.
 * \pre       -
 * \context   ANY
 */
inline constexpr bool operator==(InfSizeT lhs, InfSizeT rhs) {
  return (lhs.IsInfinity() == rhs.IsInfinity()) && (lhs.Value() == rhs.Value());
}

/*!
 * \brief     Operator == for size_t and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    True if equal. False otherwise.
 * \pre       -
 * \context   ANY
 */
inline constexpr bool operator==(std::size_t lhs, InfSizeT rhs) { return InfSizeT{lhs} == rhs; }

/*!
 * \brief     Operator == for InfSizeT and size_t.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    True if equal. False otherwise.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr bool operator==(InfSizeT lhs, std::size_t rhs) { return lhs == InfSizeT{rhs}; }

/*!
 * \brief     Operator <= for InfSizeT and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    True if lhs is equal or less than rhs. False otherwise.
 * \pre       -
 * \context   ANY
 */
inline constexpr bool operator<=(InfSizeT lhs, InfSizeT rhs) {
  return ((!lhs.IsInfinity()) && rhs.IsInfinity()) || (lhs.Value() <= rhs.Value());
}

/*!
 * \brief     Operator <= for std::size_t and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    True if lhs is equal or less than rhs. False otherwise.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr bool operator<=(std::size_t lhs, InfSizeT rhs) {
  InfSizeT const is{lhs};
  return ((!is.IsInfinity()) && rhs.IsInfinity()) || (is.Value() <= rhs.Value());
}

/*!
 * \brief     Operator <= for InfSizeT and std::size_t.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    True if lhs is equal or less than rhs. False otherwise.
 * \pre       -
 * \context   ANY
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
inline constexpr bool operator<=(InfSizeT lhs, std::size_t rhs) {
  InfSizeT const is{rhs};
  return ((!lhs.IsInfinity()) && is.IsInfinity()) || (lhs.Value() <= is.Value());
}

/*!
 * \brief     Find maximum of two given InfSizeT and InfSizeT.
 * \param[in] lhs Left hand value.
 * \param[in] rhs Right hand value.
 * \return    New InfSizeT object that is equal to the maximum value of lhs and rhs.
 * \pre       -
 * \context   ANY
 */
inline constexpr InfSizeT Max(InfSizeT lhs, InfSizeT rhs) noexcept { return (lhs <= rhs) ? rhs : lhs; }

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SIZE_T_H_
