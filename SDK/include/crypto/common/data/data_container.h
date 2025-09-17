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
/*!        \file  data_container.h
 *        \brief  Internal model for external data used by FileReader and FileWriter.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/type_traits.h"
#include "vac/language/compile_time.h"

namespace crypto {
namespace common {
namespace data {

// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Internal model for external data.
 *
 * \details DataContainers are used for the internal representation of the JSON format.
 * DataContainers can contain: bools, floats, uint64_ts, int64_ts, strings, other DataContainers,
 * as well as arrays of the aforementioned types.
 * Each value is identified by a string (key) which can be used to retrieve the value from the DataContainer.
 * \vprivate Component Private
 */
class DataContainer final {
 public:
  /*!
   * \brief Maximum recursion depth for DataContainers containing a payload.
   * This constant must be larger than the maximum depth of the content/content field in the JSON schema.
   * \vprivate Component Private
   */
  static constexpr std::size_t max_payload_depth{2};
  /*!
   * \brief Constructs an empty DataContainer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  DataContainer()
      : bool_{},
        uint64_t_{},
        int64_t_{},
        string_{},
        object_{},
        uint64_t_array_{},
        int64_t_array_{},
        string_array_{},
        object_array_{} {}

  // VECTOR NC AutosarC++17_10-A7.5.2: MD_CRYPTO_AutosarC++17_10-A7.5.2_member
  /*!
   * \brief Copy constructor; copies the content of other DataContainer into this one.
   * \param[in] other Source DataContainer.
   * \vprivate Component Private
   */
  DataContainer(DataContainer const& other);

  /*!
   * \brief Move constructor; moves the content of other DataContainer into this one.
   * \param[in] other Source DataContainer.
   * \vprivate Component Private
   */
  DataContainer(DataContainer&& other) noexcept = default;

  // VECTOR NC AutosarC++17_10-A7.5.2: MD_CRYPTO_AutosarC++17_10-A7.5.2_member
  /*!
   * \brief Default copy assignment operator; copies the content of \c other into this one.
   * \vprivate Component Private
   */
  DataContainer& operator=(DataContainer const& /*other*/) & = default;

  // VECTOR NC AutosarC++17_10-A7.5.2: MD_CRYPTO_AutosarC++17_10-A7.5.2_member
  /*!
   * \brief Default move assignment operator; moves the content of \c other into this one.
   * \vprivate Component Private
   */
  DataContainer& operator=(DataContainer&& /*other*/) & noexcept = default;

  // VECTOR NC AutosarC++17_10-A7.5.2: MD_CRYPTO_AutosarC++17_10-A7.5.2_member
  /*!
   * \brief Destructs this DataContainer.
   * \vprivate Component Private
   */
  ~DataContainer() noexcept = default;

  /*!
   * \brief Deletes a given key from the data container.
   * \param[in] key The key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Delete(amsr::core::String const& key);

  /*!
   * \brief Adds a key-value pair of bool to the data model.
   * \param[in] key The key specifier.
   * \param[in] val The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddBool(amsr::core::String const& key, bool val) noexcept;

  /*!
   * \brief Adds a key-value pair of unsigned integer to the data model.
   * \param[in] key The key specifier.
   * \param[in] num The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddUnsigned(amsr::core::String const& key, std::uint64_t num) noexcept;

  /*!
   * \brief Adds a key-value pair of signed integer to the data model.
   * \param[in] key The key specifier.
   * \param[in] num The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddSigned(amsr::core::String const& key, std::int64_t num) noexcept;

  /*!
   * \brief Adds a key-value pair of string to the data model.
   * \param[in] key The key specifier.
   * \param[in] str The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddString(amsr::core::String const& key, amsr::core::String const& str) noexcept;

  /*!
   * \brief Adds a key-value pair of \c DataContainer to the data model.
   * \details Creates a copy of the \c obj.
   * \param[in] key The key specifier.
   * \param[in] obj The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddObject(amsr::core::String const& key, crypto::common::data::DataContainer const& obj) noexcept;

  /*!
   * \brief Adds a key-value pair of \c DataContainer to the data model.
   * \details Moves from \c obj.
   * \param[in] key The key specifier.
   * \param[in] obj The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddObject(amsr::core::String const& key, crypto::common::data::DataContainer&& obj) noexcept;

  /*!
   * \brief Appends an unsigned integer value to the array identified by \c key.
   * \details If the array for given key does not exist it will be created.
   * \param[in] key The key specifier.
   * \param[in] num The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddUnsignedArray(amsr::core::String const& key, std::uint64_t num) noexcept;

  /*!
   * \brief Appends a signed integer value to the array identified by \c key.
   * \details If the array for given key does not exist it will be created.
   * \param[in] key The key specifier.
   * \param[in] num The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddSignedArray(amsr::core::String const& key, std::int64_t num) noexcept;

  /*!
   * \brief Appends a string to the array identified by \c key.
   * \details If the array for given key does not exist it will be created.
   * \param[in] key The key specifier.
   * \param[in] str The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddStringArray(amsr::core::String const& key, amsr::core::String const& str) noexcept;

  /*!
   * \brief Appends a \c DataContainer to the array identified by \c key.
   * \details If the array for given key does not exist it will be created.
   * \param[in] key The key specifier.
   * \param[in] obj The value to add.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddObjectArray(amsr::core::String const& key, crypto::common::data::DataContainer const& obj) noexcept;

  /*!
   * \brief Creates an empty \c DataContainer array identified by \c key.
   * \details If the array for given key already exists it will be overwritten.
   * \param[in] key The key specifier.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddObjectArray(amsr::core::String const& key) noexcept;

  /*!
   * \brief Returns a bool for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no bool value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<bool> GetBool(amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns an unsigned integer for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<std::uint64_t> GetUnsigned(amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns a signed integer for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<std::int64_t> GetSigned(amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns a string for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<amsr::core::String const&> GetString(amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns a \c DataContainer for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<DataContainer const&> GetObject(amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns an array of unsigned integer for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::core::Vector<std::uint64_t> const&> GetUnsignedArray(
      amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns an array of signed integer for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::core::Vector<std::int64_t> const&> GetSignedArray(
      amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns an array of string for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::core::Vector<std::reference_wrapper<amsr::core::String const>>> GetStringArray(
      amsr::core::String const& key) const noexcept;

  /*!
   * \brief Returns an array of \c DataContainer for given key.
   * \param[in] key The key to the desired value.
   * \return The value for given key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnknownIdentifier if no value for the given key exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::core::Vector<DataContainer> const&> GetObjectArray(
      amsr::core::String const& key) const noexcept;

  /*!
   * \brief Getter for the content map containing booleans.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, bool> const& GetBoolContent() const { return bool_; }
  /*!
   * \brief Getter for the content map containing unsigned integers.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, std::uint64_t> const& GetUnsignedContent() const { return uint64_t_; }
  /*!
   * \brief Getter for the content map containing signed integers.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, std::int64_t> const& GetSignedContent() const { return int64_t_; }
  /*!
   * \brief Getter for the content map containing strings.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, amsr::core::String> const& GetStringContent() const { return string_; }
  /*!
   * \brief Getter for the content map containing objects.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, DataContainer> const& GetObjectContent() const { return object_; }
  /*!
   * \brief Getter for the content map containing unsigned integer arrays.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, ara::core::Vector<uint64_t>> const& GetUnsignedArrayContent() const {
    return uint64_t_array_;
  }
  /*!
   * \brief Getter for the content map containing signed integer arrays.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, ara::core::Vector<int64_t>> const& GetSignedArrayContent() const {
    return int64_t_array_;
  }
  /*!
   * \brief Getter for the content map containing string arrays.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, ara::core::Vector<amsr::core::String>> const& GetStringArrayContent() const {
    return string_array_;
  }
  /*!
   * \brief Getter for the content map containing object arrays.
   * \return Content map for content traversal.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::map<amsr::core::String, ara::core::Vector<DataContainer>> const& GetObjectArrayContent() const {
    return object_array_;
  }

  /*!
   * \brief Wipes out all data of this container and all its child containers up to a maximum depth.
   * \param[in] max_depth Maximum depth for propagation to child containers.
   * \details This method uses an iterative implementation of depth first search for traversing the DataContainer tree.
   * The maximum memory usage is limited by \c max_recursion_depth.
   * \return true iff the Wipe completed without reaching the maximum recursion depth.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool WipeTree(size_t max_depth);

  /*!
   * \brief Compares two DataContainers.
   * \param[in] other Other DataContainer for the comparison.
   * \param[in] max_depth Maximum depth when comparing nested child containers.
   * \details This method uses an iterative implementation of depth first search for traversing the DataContainer tree.
   * The maximum memory usage is limited by \c max_recursion_depth.
   * \return true iff the Wipe completed without reaching the maximum recursion depth.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  bool Equals(DataContainer const& other, size_t max_depth) const;

  /*!
   * \brief Deleted comparison operator. Due to the possible recursion this is implemented in the function Equals(),
   * where a maximum recursion depth can be provided.
   * \vprivate Component Private
   */
  bool operator==(DataContainer const& other) const = delete;

 private:
  /*!
   * \brief Wipes out all data in this container.
   * \details Does not propagate to child containers.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Wipe() noexcept;

  /*!
   * \brief Overwrites all data stored in a string map .
   * \param[out] map Map of strings to wipe.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static void WipeHelper(std::map<amsr::core::String, amsr::core::String>& map) noexcept {
    for (std::pair<amsr::core::String const, amsr::core::String>& val : map) {
      val.second.assign(val.second.capacity(), '0');
    }
  }

  /*!
   * \brief Wipes out all data stored in \c map and replaces it with the dummy given by \c value.
   * \param[in] value The dummy value used to replace all instances in \c map.
   * \param[out] map Map to wipe.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename T,
            std::enable_if_t<(!IsVector<T>::value) && (!std::is_same<T, amsr::core::String>::value), std::int32_t> = 0>
  static void WipeHelper(T value, std::map<amsr::core::String, T>& map) noexcept {
    for (std::pair<amsr::core::String const, T>& val : map) {
      val.second = value;
    }
  }

  /*!
   * \brief Wipes out all data stored in a map of ara::core::Vector<amsr::core::String>.
   * \param[out] map Map to wipe.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static void WipeHelper(std::map<amsr::core::String, ara::core::Vector<amsr::core::String>>& map) noexcept {
    for (std::pair<amsr::core::String const, ara::core::Vector<amsr::core::String>>& array : map) {
      for (amsr::core::String& val : array.second) {
        val.assign(val.capacity(), '0');
      }
    }
  }

  /*!
   * \brief Wipes out all data stored in a map of ara::core::Vectors and replaces it with the dummy given by \c value.
   * \tparam T Any ara::core::Vector specialization except amsr::core::String
   * \param[in] value Dummy value to replace all entries in \c map
   * \param[out] map Map containing the data to wipe.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename T, std::enable_if_t<IsVector<T>::value, std::int32_t> = 0>
  static void WipeHelper(typename RemoveVector<T>::type value, std::map<amsr::core::String, T>& map) noexcept {
    for (std::pair<amsr::core::String const, T>& array : map) {
      for (typename RemoveVector<T>::type& val : array.second) {
        val = value;
      }
    }
  }

  /*!
   * \brief Returns the next child DataContainer of \c parent if the previously visited child is \c child.
   * \param[in] parent Parent DataContainer to search.
   * \param[in] child Current child. Can be nullptr then the function returns the first child of \c parent.
   * \return DataContainer* pointing to the next sibling of child in parent
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kEmptyContainer if no next child exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<DataContainer*> NextChild(DataContainer& parent, DataContainer const* child) noexcept;

  /*!
   * \brief Search \c parent.obeject_ and return the next element after \c child.
   * \param[in] parent Parent DataContainer which should be searched.
   * \param[in] child Previous child found in \c parent.object_.
   * \return Pointer to the next child in \c parent.object_ or first element in \c parent.object_array_ if \c child was
   * the last element in \c parent.object_. \error ara::crypto::SecurityErrc::kEmptyContainer if no next child exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<DataContainer*> NextChildInObjectMap(DataContainer& parent,
                                                                 DataContainer const* child) noexcept;

  /*!
   * \brief Search \c parent.obeject_array_ and return the next element after \c child.
   * \param[in] parent Parent DataContainer which should be searched.
   * \param[in] child Previous child found in \c parent.object_array_.
   * \return Pointer to the next child in \c parent.object_array_
   * \error ara::crypto::SecurityErrc::kEmptyContainer if no next child exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<DataContainer*> NextChildInObjectArray(DataContainer& parent,
                                                                   DataContainer const* child) noexcept;

  /*!
   * \brief Returns the next child DataContainer of \c parent and its corresponding key, for the previously visited
   * child \c child.
   * \details This function behaves very similar to \c NextChild but returns the key alongside the next child.
   * Also it accepts a const reference and pointer to a const object and can thus be used
   * by const functions (i.e. \c Equals) without the need to const_cast.
   * \param[in] parent Parent DataContainer
   * \param[in] child Current child. Can be nullptr then the function returns the first child of \c parent and its key.
   * \return std::pair of a string (key of the next child) and DataContainer* pointing to the next sibling of child in
   * parent.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kEmptyContainer if no next child exists.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static amsr::core::Result<std::pair<amsr::core::String, DataContainer const*>> NextChildAndKey(
      DataContainer const& parent, DataContainer const* child) noexcept;

  /*!
   * \brief Search \c parent.object_ and return the next element after \c child with its key.
   * \param[in] parent Parent DataContainer which should be searched.
   * \param[in] child Previous child found in \c parent.object_.
   * \return Pointer to the next child in \c parent.object_ or first element in \c parent.object_array_ if \c child was
   * the last element in \c parent.object_ with the corresponding key.
   * \error ara::crypto::SecurityErrc::kEmptyContainer
   * if no next child exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<std::pair<amsr::core::String, DataContainer const*>> NextChildAndKeyInObjectMap(
      DataContainer const& parent, DataContainer const* child) noexcept;

  /*!
   * \brief Search \c parent.object_array_ and return the next element after \c child with its key.
   * \param[in] parent Parent DataContainer which should be searched.
   * \param[in] child Previous child found in \c parent.object_array_.
   * \return Pointer to the next child in \c parent.object_array_
   * \error ara::crypto::SecurityErrc::kEmptyContainer if no next child exists.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   */
  static amsr::core::Result<std::pair<amsr::core::String, DataContainer const*>> NextChildAndKeyInObjectArray(
      DataContainer const& parent, DataContainer const* child) noexcept;

  std::map<amsr::core::String, bool> bool_;                                       /*!< Map for booleans */
  std::map<amsr::core::String, std::uint64_t> uint64_t_;                          /*!< Map for uint64 */
  std::map<amsr::core::String, std::int64_t> int64_t_;                            /*!< Map for signed int */
  std::map<amsr::core::String, amsr::core::String> string_;                       /*!< Map for strings */
  std::map<amsr::core::String, crypto::common::data::DataContainer> object_;      /*!< Map for objects */
  std::map<amsr::core::String, ara::core::Vector<std::uint64_t>> uint64_t_array_; /*!< Map for array of unsigned int */
  std::map<amsr::core::String, ara::core::Vector<std::int64_t>> int64_t_array_;   /*!< Map for array of signed int */
  std::map<amsr::core::String, ara::core::Vector<amsr::core::String>> string_array_; /*!< Map for array of strings */
  std::map<amsr::core::String, ara::core::Vector<crypto::common::data::DataContainer>>
      object_array_; /*!< Map for array of objects */
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_CONTAINER_H_
