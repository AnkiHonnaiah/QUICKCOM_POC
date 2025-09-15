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
/*!        \file  cfg_element.h
 *        \brief  CfgElement acts as a wrapper, which signalizes if a certain value in memory is set or not.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CFG_ELEMENT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CFG_ELEMENT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>

namespace amsr {
namespace someip_daemon_core {
namespace configuration {

/*!
 *\brief Enumeration if a configuration element is set or not.
 */
enum class ElementStatus : std::uint8_t {
  /*!
   * \brief The element's value was not set.
   */
  kNotSet,

  /*!
   * \brief The element's value is set.
   */
  kSet
};

/*!
 * \brief Wrapper to set a specific element of the configuration. If
 * the element's value is set, the element's status is updated and is set
 * from ElementStatus::kNotSet to ElementStatus::kSet.
 * \tparam ElementType of the element which is set. This could be a basic
 * datatype or a complex datatype like a string, array or vector (containers).
 */
template <typename ElementType>
class CfgElement {
 public:
  /*!
   * \brief It's assumed that the element is stored outside of this object
   * and is valid as long as this object lives.
   */
  using ElementTypeRef = ElementType&;

  /*!
   * \brief Const-reference of the element's type.
   */
  using ElementTypeConstRef = ElementType const&;

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Constructor sets the reference to the element's memory for setting the value.
   * \details The reference to valid memory is mandatory. This is why there is no default ctor.
   * \param[in] element Reference to the element's memory, where the value will be stored.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit CfgElement(ElementTypeRef element) noexcept : element_{element}, status_{ElementStatus::kNotSet} {}

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Destructor.
   * \steady FALSE
   */
  virtual ~CfgElement() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   * \steady FALSE
   */
  CfgElement(CfgElement const&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   * \steady FALSE
   */
  CfgElement& operator=(CfgElement const&) & = delete;

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  // VECTOR NC AutosarC++17_10-A12.8.4: MD_SomeIpDaemon_AutosarC++17_10-A12.8.4_UsageExplanation
  /*!
   * \brief Move constructor.
   * \param[in, out] other The other instance to move construct from.
   * \steady FALSE
   */
  CfgElement(CfgElement&& other) noexcept : element_{other.element_}, status_{ElementStatus::kNotSet} {}

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  // VECTOR NL AutosarC++17_10-A2.8.2: MD_SomeIpDaemon_A2.8.2_usage_explanation
  /*!
   * \brief Move assignment operator will reset the object's state.
   * \param[in] other The constructed element to move in.
   * \details This is used for resetting of an object of type CfgElement<T> like this:
   * int val{};
   * CfgElement<int> element1{val};
   * element1.SetElement(1);
   * // ElementStatus of element1 will now be reset.
   * element1 = CfgElement<int>{val};
   *
   * \return the reset object.
   * \steady FALSE
   */
  CfgElement& operator=(CfgElement&& other) & noexcept {
    this->status_ = ElementStatus::kNotSet;
    this->element_ = std::move(other.element_);
    return *this;
  }

  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10-A15.4.2_A15.5.3_STL_exceptions
  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Set the value of the element and the element's status.
   * \param[in] element Value of the element to write.
   * \steady FALSE
   */
  void SetElement(ElementTypeConstRef element) noexcept {
    element_ = element;
    status_ = ElementStatus::kSet;
  }

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Access the element's value.
   * \return The element's value as a const ref.
   * \steady FALSE
   */
  ElementTypeRef GetElement() const noexcept { return element_; }

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Getter if the element's value is set or is not set.
   * \return Whether the element is set or is not set.
   * \steady FALSE
   */
  ElementStatus GetStatus() const noexcept { return status_; }

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Reference to the element's value to write.
   */
  ElementTypeRef element_;

  // VECTOR NC VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_public_attributes
  /*!
   * \brief Status if the element is set or not.
   */
  ElementStatus status_;
};

}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_CFG_ELEMENT_H_
