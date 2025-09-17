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
/**        \file      deser_tlv.h
 *        \brief      Deserializer functionality for SOME/IP TLV format.
 *        \unit       SomeIpProtocol::Serdes::TLV
 *        \complexity High metrics are expected for complex data structures due to the nature of the recursive design.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_TLV_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_TLV_H_
#define LIB_SOMEIP_PROTOCOL_TLV_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_TLV_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>
#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/deserialization/deser_forward.h"
#include "someip-protocol/internal/deserialization/deser_lengthfield.h"
#include "someip-protocol/internal/deserialization/deser_wrapper.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"
namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief DataIdStruct Structure
 */
struct DataIdStruct {
  /*!
   * \brief Type alias for Data id.
   */
  using DataIdType = std::uint16_t;
  /*!
   * \brief Data id
   */
  DataIdType data_id;
};
/*!
 * \brief Reads a length field with a size (determined by WireType).
 * \tparam LengthEndian Endianness of the length field.
 * \param[in,out] r Reader holding the buffer view on the serialized data.
 * \param[in] size Size of the length field in bytes.
 * \return The length field size is 1, 2 or 4 Bytes. Empty otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - If reader contains at least the number of bytes to represent the length field
 *   - If size of length field is 1,2 or 4 bytes
 *     - Read length field value according to the size
 * \endinternal
 */
template <typename LengthEndian>
inline ara::core::Optional<std::uint32_t> ReadLengthFieldWithSize(Reader& r, std::uint8_t size) noexcept {
  ara::core::Optional<std::uint32_t> ret_val{};

  // Check if the TLV length can be consumed.
  if (r.VerifySize(size)) {
    if (size == 1U) {
      typename UintRead<1U, LengthEndian>::type len{};
      // clang-format off
      r.ReadUintOfSize<1U, LengthEndian>(len); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
      ret_val.emplace(len); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
    } else if (size == 2U) {
      typename UintRead<2U, LengthEndian>::type len{};
      r.ReadUintOfSize<2U, LengthEndian>(len); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
      ret_val.emplace(len); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
    } else if (size == 4U) {
      typename UintRead<4U, LengthEndian>::type len{};
      r.ReadUintOfSize<4U, LengthEndian>(len); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
      ret_val.emplace(len); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
      // clang-format on
    } else {
      // Unknown or unsupported length field size. Fall-through and return an empty optional.
    }
  }
  return ret_val;
}

/*!
 * \brief  Wrapper to hold the data Id and the reader object.
 */
class DataIdBuffer {
 public:
  /*!
   * \brief Constructor.
   * \param[in, out] reader Reader object.
   * \param[in] id Data Id.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  DataIdBuffer(Reader reader, DataId id) : r{reader}, data_id{id} {};
  /**
   * \brief   Getter for DataIdBuffer reader.
   * \return  The reader of the DataIdBuffer.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  Reader& GetReader() { return r; }
  /**
   * \brief   Getter for DataIdBuffer data id.
   * \return  The data id of the DataIdBuffer.
   * \pre     -
   * \context Reactor|App
   * \spec
   *   requires true;
   * \endspec
   */
  DataId const& GetDataID() const { return data_id; }

 private:
  /*!
   * \brief Reader holding the buffer view on the serialized value.
   */
  Reader r;

  /*!
   * \brief The data id.
   */
  DataId data_id;
};

/*!
 * \brief Reads a TLV buffer by returning a DataId and its corresponding sub buffer.
 * \details Unpacks wire type and data id according to SOME/IP protocol
 * \tparam Endian Endianness of the length field.
 * \param[in,out] r Reader holding the buffer view on the serialized data.
 * \param[in] default_length_size Default length size in case of wire type 4 (size according to data definition).
 * \return Data Id and and reader holding a view on the value. Empty if not enough bytes are left to read the tag.
 * \pre It has been verified that the serialized buffer contains at least the number of bytes
 *      which represent a tag.
 * \context Reactor|App
 * \reentrant TRUE
 *
 * \internal
 * - Read tag.
 * - Extract wire type and data id from the tag.
 * - If wire type is 0..3
 *   - Set size to 1, 2, 4 or 8 bytes correspondingly.
 * - Else if wire type is 5..7
 *   - Read length field value with the size of 1,2 or 4 bytes from serialized buffer.
 * - Else read the length field value with size given in the model from the serialized buffer (wire type 4).
 * - If length field value can be verified
 *   - Create a DataIdBuffer holding the DataId and a reader object with the number of bytes
 *     according to the length field value.
 * \endinternal
 */
template <typename Endian>
ara::core::Optional<DataIdBuffer> ReadDataIdBuffer(Reader& r, std::uint8_t const default_length_size) noexcept {
  /*!
   * \brief Type definition for the wire type used in the TAG field in TLV context.
   */
  using WireType = std::uint8_t;

  /*!
   * \brief Define available TLV wire types.
   * \details The wire types are defining the length field size and availablity.
   * \trace SPEC-8053389
   */
  enum class WireTypeClass : WireType {

    /*!
     * \brief 8 Bit Data Base data type
     */
    kType0 = 0,

    /*!
     * \brief  16 Bit Data Base data type
     */
    kType1 = 1,

    /*!
     * \brief  32 Bit Data Base data type
     */
    kType2 = 2,

    /*!
     * \brief  64 Bit Data Base data type
     */
    kType3 = 3,

    /*!
     * \brief  Complex Data Type: Array, Struct, String, Union with length field size according model data definitions
     */
    kType4 = 4,

    /*!
     * \brief  Complex Data Type: Array, Struct, String, Union with length field size 1 byte (ignore static definition)
     */
    kType5 = 5,

    /*!
     * \brief  Complex Data Type: Array, Struct, String, Union with length field size 2 byte (ignore static definition)
     */
    kType6 = 6,

    /*!
     * \brief  Complex Data Type: Array, Struct, String, Union with length field size 4 byte (ignore static definition)
     */
    kType7 = 7
  };

  ara::core::Array<std::uint8_t, 2U> tag{};
  Result result{true};  // An empty optional is not an error
  ara::core::Optional<DataIdBuffer> ret_val{};

  // Read tag
  r.ReadArray(tag);

  // unpacks wire type and data id according to SOME/IP protocol
  WireType const wire_type{static_cast<WireType>((tag[0] & 0x70U) >> 4)};
  DataId const data_id{
      static_cast<DataId>((static_cast<std::uint16_t>(static_cast<std::uint16_t>(tag[0]) << 8U) & 0x0F00U) |
                          static_cast<std::uint16_t>(static_cast<std::uint16_t>(tag[1]) & 0x00FFU))};
  std::uint32_t size{0};

  static constexpr WireType wire_type_4{static_cast<std::underlying_type_t<WireTypeClass>>(WireTypeClass::kType4)};
  if (wire_type < wire_type_4) {
    // Wire type is 0 to 3.
    size = static_cast<std::uint32_t>(1UL << wire_type);
  } else if (wire_type > wire_type_4) {
    // Wire type is 5 to 7.
    static constexpr WireType wire_type_5{static_cast<std::underlying_type_t<WireTypeClass>>(WireTypeClass::kType5)};
    std::uint8_t const length_size{static_cast<std::uint8_t>(1U << static_cast<std::uint8_t>(wire_type - wire_type_5))};
    ara::core::Optional<std::uint32_t> const opt_size{ReadLengthFieldWithSize<Endian>(r, length_size)};
    if (opt_size) {
      size = *opt_size;
    } else {
      result = false;
    }
  } else {
    // If wire type is 4 then we take the Length field size from ARXML configuration.
    ara::core::Optional<std::uint32_t> const opt_size{ReadLengthFieldWithSize<Endian>(r, default_length_size)};
    if (opt_size) {
      size = *opt_size;
    } else {
      result = false;
    }
  }

  // Check if the length field value can be consumed
  if (result) {
    if (r.VerifySize(size)) {
      ret_val.emplace(DataIdBuffer{r.ConsumeSubStream(size), data_id});
    }
  }

  return ret_val;
}

// Utility functions used by TlvDeserializer.AddId(...).
namespace detail {
/*!
 * \brief Extends the given array of data Ids with the given Id.
 * \tparam T Data Id / bool.
 * \tparam Is Data Ids.
 * \param[in,out] arr Array to be appended.
 * \param[in] tail Data id to appended at the end.
 * \return Extended array.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Append the data ID to the end of the array.
 * \endinternal
 */
template <typename T, std::size_t... Is>
constexpr auto ExtendArrayImpl(ara::core::Array<T, sizeof...(Is)> const& arr, T tail,
                               std::index_sequence<Is...>) noexcept -> ara::core::Array<T, sizeof...(Is) + 1> {
  return {arr[Is]..., tail};
}

/*!
 * \brief Extends the given array of Data Ids with the given Id.
 * \tparam T Data Id / bool.
 * \tparam N Number of Data Ids in the input array.
 * \param[in,out] arr Array to be extended.
 * \param[in] tail  Data id to appended at the end.
 * \return Extended array.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, std::size_t N>
constexpr auto ExtendArray(ara::core::Array<T, N> const& arr, T tail) noexcept -> ara::core::Array<T, N + 1> {
  return ExtendArrayImpl(arr, tail, std::make_index_sequence<N>{});  // VCA_SOMEIPPROTOCOL_FIXED_SIZE_ARRAY
}

/*!
 * \brief Extends the tuple with given input data.
 * \tparam Is Tuple indices.
 * \tparam Ts Type of tuple elements.
 * \tparam T Type of data to be appended.
 * \param[in,out] tup Tuple to be extended.
 * \param[in] tail Data to appended at the end of the tuple.
 * \return Extended tuple.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <std::size_t... Is, typename... Ts, typename T>
constexpr auto ExtendTupleImpl(std::tuple<Ts...> const& tup, T const& tail, std::index_sequence<Is...>) noexcept
    -> std::tuple<Ts..., T> {
  // clang-format off
  return {std::get<Is>(tup)..., tail}; // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
  // clang-format on
}

/*!
 * \brief Extends the tuple with given input data.
 * \tparam Ts Type of tuple elements.
 * \tparam T Type of data to be appended.
 * \param[in,out] tup Tuple to be extended.
 * \param[in] tail Data to appended at the end of the tuple.
 * \return Extended tuple.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE
 * \spec
 *   requires true;
 * \endspec
 */
template <typename... Ts, typename T>
constexpr auto ExtendTuple(std::tuple<Ts...> const& tup, T const& tail) noexcept -> std::tuple<Ts..., T> {
  return ExtendTupleImpl(tup, tail, std::make_index_sequence<sizeof...(Ts)>{});
}

/*!
 * \brief Primary template to call the deserialization functor.
 * \details Uses recursive linear search with DataId to find and call a deserialization function.
 * \tparam ReverseIndex The reverse index (End Index of the array for the first call).
 * \spec
 *   requires true;
 * \endspec
 */
template <std::size_t ReverseIndex>
class DataIdSelectCallImpl {
 public:
  /*!
   * \brief Call the deserialization function for the given Data Id.
   * \details It is assumed the deserialization function to be called is at the same index position
   *          within the tuple as the Data Id is within the Data Id Array.
   * \tparam Ts Type of tuple element.
   * \param[in] ids Array of Data Ids.
   * \param[in] t Tuple holding the deserialization functions.
   * \param[in,out] r Buffer Reader provided to the deserialization function.
   * \param[in] id Data Id.
   * \param[in,out] field_present Array containing information if subelements are present or not
   * \return True if the value was correctly deserialized for the given Data Id. False otherwise.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   *
   * \internal
   * - If DataId matches the stored data Id
   *   - Mark the element as present.
   *   - Call the stored deserialization lambda function.
   * \endinternal
   */
  template <typename... Ts>
  static Result Call(ara::core::Array<DataId, sizeof...(Ts)> const& ids, std::tuple<Ts...> const& t, Reader& r,
                     DataId const id, ara::core::Array<bool, sizeof...(Ts)>& field_present) noexcept {
    constexpr std::size_t N{sizeof...(Ts) - ReverseIndex - 1};
    Result result{};
    if (id == ids[N]) {
      field_present[N] = true;
      result = std::get<N>(t)(r);
    } else {
      // VECTOR NL AutosarC++17_10-M5.19.1:MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.19.1_OverflowOrUnderflowOfConstExpr
      result = DataIdSelectCallImpl<ReverseIndex - 1>::Call(ids, t, r, id, field_present);
    }

    return result;
  }
};

/*!
 * \brief Specialized template class that builds the recursion tail.
 */
template <>
class DataIdSelectCallImpl<std::numeric_limits<std::size_t>::max()> {
 public:
  /*!
   * \brief Recursion Tail for std::size_t::max as reverse Index (see primary template).
   * \tparam Ts Type of tuple element.
   * \param[in] ids Array of Data Ids.
   * \param[in] t Tuple holding the deserialization functions.
   * \param[in,out] r Buffer Reader provided to the deserialization function.
   * \param[in] id Data Id.
   * \param[in,out] field_present Array containing information if subelements are present or not
   * \return Always return true for recursion tail.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename... Ts>
  static Result Call(ara::core::Array<DataId, sizeof...(Ts)> const& ids, std::tuple<Ts...> const& t, Reader const& r,
                     DataId const id, ara::core::Array<bool, sizeof...(Ts)> const& field_present) noexcept {
    static_cast<void>(ids);
    static_cast<void>(t);
    static_cast<void>(r);
    static_cast<void>(id);
    static_cast<void>(field_present);
    // Recursion tail. No further action required here. An unknown DataId is gracefully ignored.
    return true;
  }
};
}  // namespace detail

/*!
 * \brief Call the deserialization function for the given Data Id.
 * \details It is assumed the deserialization function to be called is at the same index position
 *          within the tuple as the Data Id is within the Data Id Array.
 * \tparam N Number of Data Ids stored in the input array.
 * \tparam Ts Type of tuple element.
 * \param[in] ids Array of Data Ids.
 * \param[in] t Tuple holding the deserialization functions.
 * \param[in,out] r Buffer Reader provided to the deserialization function.
 * \param[in] id Data Id.
 * \param[in,out] field_present Array containing information if subelements are present or not
 * \return true if the value was correctly deserialized for the given Data Id, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Call the deserialization function for the given Data Id.
 * \endinternal
 */
template <std::size_t N, typename... Ts>
Result DataIdSelectCall(ara::core::Array<DataId, N> const& ids, std::tuple<Ts...> const& t, Reader& r, DataId const id,
                        ara::core::Array<bool, N>& field_present) noexcept {
  return detail::DataIdSelectCallImpl<sizeof...(Ts) - 1>::Call(ids, t, r, id, field_present);
}

/*!
 * \brief Primary template declaration to create a lambda for deserialization.
 * \tparam IsPrimitive True in case of a primitive.
 * \tparam TpPack Tranformation properties
 * \tparam Confs Transformation properties configurations for the sub-elements.
 */
template <bool IsPrimitive, typename TpPack, typename... Confs>
class CreateTlvDeserializationLambda;

/*!
 * \brief Specialized template class to create a lambda function for deserialization for primitive data.
 * \tparam TpPack Transformation properties.
 * \tparam Confs Transformation properties configurations for the sub-elements.
 */
template <typename TpPack, typename... Confs>
class CreateTlvDeserializationLambda<true, TpPack, Confs...> {
 public:
  /*!
   * \brief Creates a lambda function for deserialization that takes the reader as input value.
   * \tparam T Type of primitive data.
   * \param[out] t Variable the deserialized value shall be stored into.
   * \return Deserialization lambda function.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Return deserialization lambda function that
   *   - verifies the static size of the primitive
   *   - deserializes the data, if verification was successful.
   * \endinternal
   */
  template <typename T>
  // VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
  static auto Create(T& t) noexcept {
    return [&t](Reader& r) {
      constexpr std::size_t static_size{SomeIpProtocolGetStaticSize<TpPack, Confs...>(SizeToken<T>{})};
      Result result{r.VerifySize(static_size)};
      if (result) {
        // clang-format off
        result = SomeIpProtocolDeserialize<TpPack, Confs...>(r, t); // VCA_SOMEIPPROTOCOL_READER_REFERENCE
        // clang-format on
      }
      return result;
    };
  }
};

/*!
 * \brief Specialized template class to create a lambda function for deserialization for data elements with a length
 *        field.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration holding endianness and length field size information.
 * \tparam Confs Transformation properties configurations for the sub-elements.
 */
template <typename TpPack, typename LengthConf, typename... Confs>
class CreateTlvDeserializationLambda<false, TpPack, LengthConf, Confs...> {
 public:
  /*!
   * \brief Creates a lambda function for deserialization that takes the reader as input value.
   * \tparam T Type of data that shall be deserialized.
   * \param[out] t Variable the deserialized value shall be stored into.
   * \return Deserialization lambda function.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Return deserialization lambda function that
   *   - verifies the static size of the data itself (without length field)
   *   - deserializes the data, if verification was successful.
   * \endinternal
   */
  template <typename T>
  // VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
  static auto Create(T& t) noexcept {
    // Set length field size to 0 because of length field of this non-primitive datatype is already deserialized by
    // the TLV tag deserializer. In case of non-TLV datatypes inside a TLV hierarchy (struct/method argument),
    // the length field of the TLV tag is used.
    return [&t](Reader& r) {
      constexpr std::size_t static_size{
          SomeIpProtocolGetStaticSize<TpPack, LengthSize<0, typename LengthConf::endian>, Confs...>(SizeToken<T>{})};
      Result result{r.VerifySize(static_size)};
      if (result) {
        // clang-format off
        result = SomeIpProtocolDeserialize<TpPack, LengthSize<0, typename LengthConf::endian>, Confs...>(r, t); // VCA_SOMEIPPROTOCOL_READER_REFERENCE
        // clang-format on
      }
      return result;
    };
  }
};

/*!
 * \brief Constexpr object with Deserialize(...) function.
 * \details This class can be used in a fluent api. Initial object shall be created through the builder.
 * \tparam TpPack Transformation properties.
 * \tparam N Number of Data Ids.
 * \tparam Fs Deserialization functions parameter pack.
 */
template <typename TpPack, std::size_t N, typename... Fs>
class TlvDeserializer {
  /*!
   * \brief Container type of registered TLV data IDs.
   */
  using Ids = ara::core::Array<DataId, N>;

  /*!
   * \brief Container type of registered TLV sub-element deserialization lambdas.
   */
  using Funs = std::tuple<Fs...>;

  /*!
   * \brief Container type of TLV sub-element presence states (sub-element optionality).
   */
  using SubelementPresentList = ara::core::Array<bool, N>;

 public:
  /*!
   * \brief Constructor.
   * \param[in] ids Input array holding N Data Ids.
   * \param[in] funs Tuple holding the deserialization functor.
   * \param[in] is_subelement_present Array containing information if subelements are present or not.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr TlvDeserializer(Ids const& ids, Funs const& funs, SubelementPresentList const& is_subelement_present)
      : ids_{ids}, funs_{funs}, is_subelement_present_{is_subelement_present} {}

  /*!
   * \brief Extends table of DataId and deserialization functors. Specialization for non-optional TLV struct members.
   * \tparam Confs Transformation properties of the TLV sub-elements.
   * \tparam T Type of data to be deserialized.
   * \param[in] data Object of structure DataIdStruct.
   * \param[out] t Variable the value shall be deserialized into.
   * \return Extended Tlv Deserializer object.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a new TlvDeserializer with the extended DataId table & deserialization functors table.
   * - Mark DataId as absent.
   * \endinternal
   */
  template <typename... Confs, typename T>
  // VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
  constexpr auto AddId(DataIdStruct const data, T& t) const noexcept {
    constexpr bool is_primitive_or_enum{IsPrimitiveType<T>::value || IsEnumType<T>::value};
    // VECTOR NC AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.5.2_falsepositive
    // clang-format off
    auto f = CreateTlvDeserializationLambda<is_primitive_or_enum, TpPack, Confs...>::Create(t); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
    // VECTOR NC AutosarC++17_10-A5.1.7: MD_SOMEIPPROTOCOL_AutosarC++17_10-A5.1.7_UnderlyingLambdaTypeShallNotBeUsed
    return TlvDeserializer<TpPack, N + 1, Fs..., std::decay_t<decltype(f)>>{ // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        detail::ExtendArray(ids_, data.data_id), detail::ExtendTuple(funs_, std::move(f)), // VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        // clang-format on
        // Set false for non optional members here and we make it true when we actually find them in byte stream so that
        // we can later find out if any mandatory sub-element is missing.
        detail::ExtendArray(is_subelement_present_, false)};  // VCA_SOMEIPPROTOCOL_FIXED_SIZE_ARRAY
  }

  /*!
   * \brief Extends table of DataId and deserialization functors. Specialization for optional TLV struct members.
   * \details Specialization necessary for IsPrimitive check of the datatype.
   * \tparam Confs Transformation properties of the TLV sub-elements.
   * \tparam T Type of data to be deserialized.
   * \param[in] data Object of structure DataIdStruct.
   * \param[out] t Optional variable the value shall be deserialized into.
   * \return Extended Tlv Deserializer object.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a new TlvDeserializer with the extended DataId table & deserialization functors table.
   * - Mark DataId as present.
   * \endinternal
   */
  template <typename... Confs, typename T>
  // VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
  constexpr auto AddId(DataIdStruct const data, ara::core::Optional<T>& t) const noexcept {
    constexpr bool is_primitive_or_enum{IsPrimitiveType<T>::value || IsEnumType<T>::value};
    // VECTOR NC AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.5.2_falsepositive
    // clang-format off
    auto f = CreateTlvDeserializationLambda<is_primitive_or_enum, TpPack, Confs...>::Create(t); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
    // VECTOR NC AutosarC++17_10-A5.1.7: MD_SOMEIPPROTOCOL_AutosarC++17_10-A5.1.7_UnderlyingLambdaTypeShallNotBeUsed
    return TlvDeserializer<TpPack, N + 1, Fs..., std::decay_t<decltype(f)>>{ // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        detail::ExtendArray(ids_, data.data_id), detail::ExtendTuple(funs_, std::move(f)), // VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        // clang-format on
        // Set true for optional members already as we do not want to check at the end if all optional members are
        // deserialized or not.
        detail::ExtendArray(is_subelement_present_, true)};  // VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
  }

  /*!
   * \brief Register a new TLV sub-element identified by its data-ID with sub-element specific
            transformation properties. Specialization for TLV method arguments.
   * \details Transformation properties of the TlvDeserializer are ignored by this API.
   *          Use the TlvDeserializerBuilder() without transformation properties parameter to construct the
   *          initial empty TlvDeserializer instance.
   * \tparam TpPackOfId Transformation properties of the TLV element.
   * \tparam Confs  Transformation properties of the TLV sub-elements.
   * \tparam T Type of data to be deserialized.
   * \param[in] data Object of structure DataIdStruct.
   * \param[out] t Variable the value shall be deserialized into.
   * \return Extended Tlv Deserializer object.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a new TlvDeserializer with the extended DataId table, deserialization functors table and the
   *   element-specific transformation properties.
   * - Mark DataId as absent.
   * \endinternal
   */
  template <typename TpPackOfId, typename... Confs, typename T>
  // VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
  constexpr auto AddIdWithTransformationProps(DataIdStruct const data, T& t) const noexcept {
    constexpr bool is_primitive_or_enum{IsPrimitiveType<T>::value || IsEnumType<T>::value};
    // VECTOR NC AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.5.2_falsepositive
    // clang-format off
    auto f = CreateTlvDeserializationLambda<is_primitive_or_enum, TpPackOfId, Confs...>::Create(t); // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
    // VECTOR NC AutosarC++17_10-A5.1.7: MD_SOMEIPPROTOCOL_AutosarC++17_10-A5.1.7_UnderlyingLambdaTypeShallNotBeUsed
    return TlvDeserializer<TpPackOfId, N + 1, Fs..., std::decay_t<decltype(f)>>{ // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT, VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        detail::ExtendArray(ids_, data.data_id), detail::ExtendTuple(funs_, std::move(f)), // VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        // clang-format on
        // Set false for non optional members here and we make it true when we actually find them in byte stream so that
        // we can later find out if any mandatory sub-element is missing.
        detail::ExtendArray(is_subelement_present_, false)};  // VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
  }

  /*!
   * \brief Deserialize the byte stream containing the registered TLV elements (struct members / method arguments).
   * \details This function is supposed to be called on the fully extended TlvDeserializer object.
   * \tparam StaticLengthConf  Static length field configuration.
   * \param[in,out] r Buffer Reader holding the view on the serialized data.
   * \return true if the deserialization is successful, false otherwise.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE for different reader objects.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - While the byte stream contains at least the number of bytes required to represent a Tag
   *   - Read Data Id from Tag
   * \endinternal
   */
  template <typename StaticLengthConf>
  Result Deserialize(Reader& r) const noexcept {
    Result result{true};
    SubelementPresentList present_subelements{is_subelement_present_};

    // Iterate until data elements are present.
    while (r.VerifySize(2U)) {  // Check if the Tag can be consumed.
      ara::core::Optional<DataIdBuffer> opt_data_id_buffer{
          // clang-format off
          ReadDataIdBuffer<typename StaticLengthConf::endian>(r, static_cast<std::uint8_t>(StaticLengthConf::value))}; // VCA_SOMEIPPROTOCOL_READER_REFERENCE
      // clang-format on
      if (opt_data_id_buffer.has_value()) {
        Reader& sub_reader{opt_data_id_buffer->GetReader()};
        // clang-format off
        result = DataIdSelectCall(ids_, funs_, sub_reader, opt_data_id_buffer->GetDataID(), present_subelements);  // VCA_SOMEIPPROTOCOL_DESER_TLV_REFERENCE
        // clang-format on
      } else {
        result = false;
      }

      // If reading of the last TLV element was not successful, we shall terminate here and shall not read any further.
      if (!result) {
        break;
      }
    }

    // If the deserializer cannot find a required (i.e. non-optional) member/argument defined in its data definition in
    // the serialized byte stream, the deserialization shall be aborted
    if (!std::all_of(present_subelements.cbegin(), present_subelements.cend(),
                     [](bool is_present) { return is_present; })) {
      result = false;
    }
    return result;
  }

 private:
  /*!
   * \brief Container of registered TLV data IDs.
   */
  Ids ids_;

  /*!
   * \brief Container of registered TLV sub-element deserialization lambdas.
   */
  Funs funs_;

  /*!
   * \brief Container type of TLV sub-element presence states (sub-element optionality).
   * \details Initially all optional elements are marked as present.
   */
  SubelementPresentList is_subelement_present_;
};

/*!
 * \brief Wrapper function to create an empty TlvDeserializer using common TransformatioProperties.
 * \tparam TpPack Transformation properties.
 * \return TlvDeserializer object with empty tables.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Construct and return an empty TlvDeserializer using the passed transformation properties.
 * \endinternal
 */
template <typename TpPack>
// VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
constexpr auto TlvDeserializerBuilder() noexcept {
  return TlvDeserializer<TpPack, 0>{{}, std::make_tuple(), {}};
}

/*!
 * \brief Wrapper function to create an empty TlvDeserializer.
 * \details Sub-element specific transformation properties must be registered using AddIdWithTransformationProps API.
 * \return TlvDeserializer object with empty tables.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 * \internal
 * - Construct and return an empty TlvDeserializer using an empty transformation properties configuration.
 * \endinternal
 */
// VECTOR NL AutosarC++17_10-A7.1.5: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.1.5_UseOfC++11autoTypeSpecifier
constexpr auto TlvDeserializerBuilder() noexcept {
  return TlvDeserializer<internal::TpPack<>, 0>{{}, std::make_tuple(), {}};
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_TLV_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_TLV_H_
