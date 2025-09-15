/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**     \file       map.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::Map
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MAP_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/core/map.h"
#include "amsr/core/result.h"
#include "amsr/log/internal/abort.h"
namespace amsr {
namespace log {
namespace internal {

/*!
 * \brief   Map with C++17 additions & with transparent comparison function.
 *
 * \tparam  Key     The key type of the map.
 *                  Must implement the CopyConstructible, EqualityComparable and LessThanComparable named requirements.
 * \tparam  Value   The value type of the map.
 *                  Must implement the CopyConstructible and Erasable named requirement.
 *                  If non-const overload of operator[] is to ever be used,
 *                  Value must also implement the DefaultConstructible named requirement.
 *                  Must meet the requirements of ara::core::Result<Value>.
 *
 * \details Uses std::less<void> for transparent comparison.
 */
template <typename Key, typename Value>
// VCA_LOGAPI_MOLE_1298
class Map : public core::Map<Key, Value, std::less<>> {
 public:
  /*! The iterator for this map. */
  using iterator = typename core::Map<Key, Value, std::less<>>::iterator;
  /*! The const_iterator for this map. */
  using const_iterator = typename core::Map<Key, Value, std::less<>>::const_iterator;

  /*!
   * \brief   Accesses the map and creates the element in case it's not available.
   *
   * \details This overload replaces std::map<Key, Value>::operator[](Key&&).
   *
   * \remark  This function impose additional requirements for the template parameter Value.
   *          Value must implement the DefaultConstructible named requirement.
   *
   * \tparam  K   The key type that's convertable to Key.
   *              Must not be of the same type as Key.
   *              Note that const and/or references of Key are not considered the same as Key (e.g., Key and Key&).
   *
   * \param   key The key to look for.
   *
   * \return  A reference to the value for that key.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A10.2.1: MD_LOG_AutosarC++17_10-A10.2.1_false_positive
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
  // VECTOR NC AutosarC++17_10-A13.5.1: MD_LOG_AutosarC++17_10-A13.5.1_false_positive
  template <typename K, typename std::enable_if_t<!std::is_same<K, Key>::value, std::uint8_t> = 0>
  auto operator[](K&& key) & noexcept -> Value& {
    iterator it{this->lower_bound(key)};
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
    if (not CheckLowerBound(it, key)) {
      Key owned_key{std::forward<K>(key)};
      // VCA_LOGAPI_CHECKED_CONTRACT
      it = this->emplace_hint(it, std::piecewise_construct, std::forward_as_tuple(std::move(owned_key)),
                              std::forward_as_tuple(Value{}));
    }
    // VCA_LOGAPI_VALID_OBJECT
    return it->second;
  }

  /*!
   * \brief    Accesses the map and terminates in case it's not available.
   *
   * \details  This overload replaces std::map<Key, Value>::operator[](Key const&).
   *           This overload is only when the amsr::log::internal::Map<Key, Value> is const.
   *
   * \tparam   K    The key type that's convertable to Key.
   *                Must not be of the same type as Key.
   *
   * \param    key  The key to look for.
   *
   * \return   A reference to the value for that key.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A10.2.1: MD_LOG_AutosarC++17_10-A10.2.1_false_positive
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
  template <typename K, typename std::enable_if_t<!std::is_same<K, Key>::value, std::uint8_t> = 0>
  auto operator[](K const& key) const& noexcept -> Value const& {
    using vac::container::operator""_sv;
    // VECTOR NC AutosarC++17_10-A7.1.1: MD_LOG_AutosarC++17_10-A7.1.1_false_positive
    // VCA_LOGAPI_VALID_OBJECT
    const_iterator it{this->find(key)};
    // VCA_LOGAPI_VALID_OBJECT
    internal::Assert(it != this->end(), "Map does not contain key"_sv);
    // VCA_LOGAPI_VALID_OBJECT
    return it->second;
  }

  /*!
   * \brief   Accesses the map and returns value as a pointer.
   *
   * \tparam  K   The key type that's convertable to Key.
   *
   * \param   key The key to look for.
   *
   * \return  A pointer to the value for that key or nullptr.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename K>
  auto GetPtr(K const& key) & noexcept -> Value* {
    using vac::container::operator""_sv;
    Value* ret_val{nullptr};
    // VECTOR NC AutosarC++17_10-A7.1.1: MD_LOG_AutosarC++17_10-A7.1.1_false_positive
    iterator it{this->find(key)};
    if (it != this->end()) {
      ret_val = &it->second;
    }
    return ret_val;
  }

  /*!
   * \brief   Accesses the map and returns value as a pointer.
   *
   * \tparam  K   The key type that's convertable to Key.
   *
   * \param   key The key to look for.
   *
   * \return  A pointer to the value for that key or nullptr.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename K>
  auto GetPtr(K const& key) const& noexcept -> Value const* {
    using vac::container::operator""_sv;
    Value const* ret_val{nullptr};
    // VECTOR NC AutosarC++17_10-A7.1.1: MD_LOG_AutosarC++17_10-A7.1.1_false_positive
    // VCA_LOGAPI_CHECKED_CONTRACT
    const_iterator it{this->find(key)};
    // VCA_LOGAPI_OBJECT_IS_VALID
    if (it != this->end()) {
      ret_val = &it->second;
    }
    return ret_val;
  }

  /*!
   * \brief   Assigns the value to the key. Creates it if needed.
   *
   * \tparam  K     The key type that's convertable to Key.
   * \tparam  V     The value type that's convertable to Value.
   *
   * \param   key   The key to look for.
   * \param   value The value to assign.
   *
   * \return  The iterator to the key-value-pair and if the value is new.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_LOG_AutosarC++17_10-A15.4.4
  template <typename K, typename V>
  auto InsertOrAssign(K&& key, V&& value) noexcept -> std::pair<iterator, bool> {
    iterator it{this->lower_bound(key)};
    bool inserted{false};
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
    if (this->CheckLowerBound(it, key)) {
      // VCA_LOGAPI_VALID_OBJECT
      it->second = std::forward<V>(value);
    } else {
      Key owned_key{std::forward<K>(key)};
      // VCA_LOGAPI_CHECKED_CONTRACT
      it = this->emplace_hint(it, std::piecewise_construct, std::forward_as_tuple(std::move(owned_key)),
                              std::forward_as_tuple(std::forward<V>(value)));
      inserted = true;
    }
    return std::make_pair(it, inserted);
  }

  /*!
   * \brief   Gets the value to the key. Creates it if needed.
   *
   * \tparam  K     The key type that's convertable to Key.
   * \tparam  V     The value type that's convertable to Value.
   *
   * \param   key   The key to look for.
   * \param   value The value to assign.
   *
   * \return  A reference to the value.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_LOG_AutosarC++17_10-A15.4.4
  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  template <typename K, typename V>
  auto GetOrAssign(K&& key, V&& value) noexcept -> Value const& {
    Value const* ret_val{nullptr};
    iterator it{this->lower_bound(key)};
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
    if (CheckLowerBound(it, key)) {
      ret_val = &it->second;
    } else {
      Key owned_key{std::forward<K>(key)};
      // VCA_LOGAPI_CHECKED_CONTRACT
      it = this->emplace_hint(it, std::piecewise_construct, std::forward_as_tuple(std::move(owned_key)),
                              std::forward_as_tuple(std::forward<V>(value)));
      ret_val = &it->second;
    }
    // VCA_LOGAPI_VALID_RETURN_VALUE
    return *ret_val;
  }

  /*!
   * \brief   Gets the value to the key or the passed default.
   *
   * \tparam  K     The key type that's convertable to Key.
   * \tparam  V     The value type that's convertable to Value.
   *
   * \param   key   The key to look for.
   * \param   value The value to return on failure.
   *
   * \return  A reference to the value.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_LOG_AutosarC++17_10-A15.4.4
  template <typename K, typename V>
  auto GetOr(K&& key, V&& value) const noexcept -> Value {
    Value ret_val{std::forward<V>(value)};
    // VCA_LOGAPI_VALID_OBJECT
    const_iterator const it{this->find(key)};
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
    // VCA_LOGAPI_VALID_OBJECT
    if (it != this->end()) {
      // VCA_LOGAPI_VALID_OBJECT
      ret_val = it->second;
    }
    return ret_val;
  }

  /*!
   * \brief   Returns true in case the key is in the map.
   *
   * \tparam  K   The key type.
   *              Must be convertible to Key.
   *
   * \param   key The key to look for.
   *
   * \return  True in case the key is in the map.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename K>
  auto Has(K const& key) const noexcept -> bool {
    // VCA_LOGAPI_CHECKED_CONTRACT
    const_iterator const it{this->find(key)};
    return it != this->end();
  }

  /*!
   * \brief   Returns the value of the map.
   *
   * \tparam  K       The key type.
   *                  Must be convertible to Key.
   * \tparam  Errors  The types of the ErrorCode constructor used.
   *
   * \param   key     The key to look for.
   * \param   errors  The arguments for the ErrorCode constructor.
   *
   * \return  The value in case the key is in the map.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename K, typename... Errors>
  auto Get(K const& key, Errors&&... errors) const noexcept -> core::Result<Value const&> {
    using RetVal = core::Result<Value const&>;
    // VCA_LOGAPI_CHECKED_CONTRACT
    const_iterator const it{this->find(key)};
    // VCA_LOGAPI_VALID_OBJECT
    return (it != this->end()) ? RetVal::FromValue(it->second) : RetVal::FromError(std::forward<Errors>(errors)...);
  }

  /*!
   * \brief   Returns true in case the key is in the map.
   *
   * \tparam  K       The key type.
   * \tparam  Errors  The types of the ErrorCode constructor used.
   *
   * \param   key     The key to look for.
   * \param   errors  The arguments for the ErrorCode constructor.
   *
   * \return  True in case the key is in the map.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename K, typename... Errors>
  auto CheckFor(K const& key, Errors&&... errors) const noexcept -> core::Result<void> {
    return this->Has(key) ? core::Result<void>::FromValue()
                          : core::Result<void>::FromError(std::forward<Errors>(errors)...);
  }

 private:
  /*!
   * \brief   Checks if lower_bound has found the element in the map.
   *
   * \tparam  K   Type of the key to compare.
   *              Must be equality comparable with Key.
   *
   * \param   it  The iterator returned by lower_bound.
   * \param   key The key that was searched for.
   *
   * \return  True in case the key was in the map.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename K>
  auto CheckLowerBound(iterator it, K const& key) noexcept -> bool {
    // VECTOR NC AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
    // VCA_LOGAPI_VALID_OBJECT
    return (it != this->end()) && (it->first == key);
  }
};

/*!
 * \brief The mutable iterator for a container.
 *
 * \tparam Container The container type.
 */
template <typename Container>
using IterMut = typename Container::iterator;

/*!
 * \brief The constant iterator for a container.
 *
 * \tparam Container The container type.
 */
template <typename Container>
using Iter = typename Container::const_iterator;

/*!
 * \brief The mutable iterator for the Map type.
 *
 * \tparam Key   The key type of the map.
 * \tparam Value The value type of the map.
 */
template <typename Key, typename Value>
using MapIterMut = IterMut<Map<Key, Value>>;

/*!
 * \brief The constant iterator for the Map type.
 *
 * \tparam Key   The key type of the map.
 * \tparam Value The value type of the map.
 */
template <typename Key, typename Value>
using MapIter = Iter<Map<Key, Value>>;

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MAP_H_
