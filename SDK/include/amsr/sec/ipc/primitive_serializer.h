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
/*!        \file  primitive_serializer.h
 *        \brief  Serializer for data primitives used message transmission.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PRIMITIVE_SERIALIZER_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PRIMITIVE_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/sec/ipc/basic_message.h"
#include "ara/crypto/common/guid.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_object.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/user_permissions.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/x509_dn.h"
#include "crypto/common/identifiable.h"
#include "crypto/common/type_traits.h"
#include "crypto/common/util/logger.h"
#include "vac/language/location.h"
#include "vac/language/remove_shared_ptr.h"

namespace amsr {
namespace sec {
namespace ipc {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

using BufferPosition = std::size_t; /*!< Position inside a buffer. */

// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Serializer for single value objects which are contained in a message.
 * \details For message transmission we need to be able to serialize and deserialize the arguments and results of
 * function calls. This class contains several functions for serialization of different data types.
 * \vprivate Component Private
 */
class PrimitiveSerializer {
 public:
  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~PrimitiveSerializer() noexcept = default;

  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  PrimitiveSerializer() = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  PrimitiveSerializer(PrimitiveSerializer const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  PrimitiveSerializer(PrimitiveSerializer&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  PrimitiveSerializer& operator=(PrimitiveSerializer const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  PrimitiveSerializer& operator=(PrimitiveSerializer&& /*other*/) & noexcept = default;

  /*!
   * \brief Checker function to ensure that the buffer is large enough to hold the serialized data.
   * \param[in] size Size to write to the buffer.
   * \param[in] buffer The buffer itself.
   * \param[in] buffer_pos The current buffer position.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto CheckSize(std::size_t size, ara::crypto::ReadOnlyMemRegion const& buffer,
                        BufferPosition buffer_pos) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Implements common deserialization for fundamental types.
   * \tparam T The primitive type.
   * \param[in] value Size to be read from the buffer.
   * \param[in,out] buffer_pos The current buffer position.
   * \param[in] buffer The buffer itself.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Assert that buffer size is sufficient
   * - Serialize the value from \c value into \c buffer
   * - Adjust \c buffer_pos
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::int32_t> = 0>
  static auto Serialize(T const& value, BufferPosition& buffer_pos,
                        ara::crypto::WritableMemRegion const& buffer) noexcept -> amsr::core::Result<void> {
    return PrimitiveSerializer::CheckSize(sizeof(T), buffer, buffer_pos).AndThen([buffer, &buffer_pos, &value]() {
      std::memcpy(&buffer[buffer_pos], &value, sizeof(T));
      buffer_pos += sizeof(T);
      return amsr::core::Result<void>::FromValue();
    });
  }

  /*!
   * \brief Templated function to serialize all kinds of pointers.
   * \details Serialization of pointers will first add a serialization tag which indicates if the pointer is null or if
   * it has a value. Afterwards the serializer of the type is called. \tparam T Type of the pointer to be processed.
   * \param[in] value Pointer to be serialized. Nullptr will be serialized to a nullptr-tag.
   * \param[out] buffer_pos position of the buffer.
   * \param[out] buffer Buffer to write the data into.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - If pointer is nullptr: Serialize tag_nullptr  into \c buffer
   * - Else: Serialize tag_normal_pointer into \c buffer and then serialize pointee into \c buffer
   * \endinternal
   */
  template <typename T,
            typename std::enable_if<((std::is_pointer<T>::value) || (::crypto::common::is_shared_pointer<T>::value) ||
                                     (::crypto::common::is_unique_pointer<T>::value)) &&
                                        (!::crypto::common::is_identifiable<T>::value) &&
                                        (!std::is_same<T, char const*>::value),
                                    std::int32_t>::type = 0>
  auto Serialize(T const& value, BufferPosition& buffer_pos, ara::crypto::WritableMemRegion buffer) const noexcept
      -> amsr::core::Result<void> {
    return (value == nullptr)
               ? Serialize(SerializationTags::tag_nullptr, buffer_pos, buffer)
               : Serialize(SerializationTags::tag_normal_pointer, buffer_pos, buffer)
                     .AndThen([this, &value, &buffer_pos, &buffer]() { return Serialize(*value, buffer_pos, buffer); });
  }

  /*!
   * \brief Templated function to serialize all kinds of pointers.
   * \details Serialization of pointers will first add a serialization tag which indicates if the pointer is null or if
   * it has a value. Afterwards the serializer of the type is called. \tparam T Type of the pointer to be processed.
   * \param[in] value Pointer to be serialized. Nullptr will be serialized to a nullptr-tag.
   * \param[out] buffer_pos position of the buffer.
   * \param[out] buffer Buffer to write the data into.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - If pointer is nullptr: Serialize tag_nullptr  into \c buffer
   * - Else: Serialize tag_normal_pointer into \c buffer and then serialize proxy id into \c buffer
   * \endinternal
   */
  template <typename T,
            typename std::enable_if<(std::is_pointer<T>::value && ::crypto::common::is_identifiable<T>::value),
                                    std::int32_t>::type = 0>
  auto Serialize(T const& value, BufferPosition& buffer_pos, ara::crypto::WritableMemRegion buffer) const noexcept
      -> amsr::core::Result<void> {
    return (value == nullptr) ? Serialize(SerializationTags::tag_nullptr, buffer_pos, buffer)
                              : Serialize(SerializationTags::tag_normal_pointer, buffer_pos, buffer)
                                    .AndThen([&value, &buffer, &buffer_pos]() {
                                      amsr::sec::ipc::ProxyId const proxy_id{amsr::sec::ipc::GetProxyId(value)};
                                      return Serialize(proxy_id, buffer_pos, buffer);
                                    });
  }

  // VECTOR NC AutosarC++17_10-A18.1.5: MD_CRYPTO_AutosarC++17_10-A18.1.5_const_unique_ptr
  /*!
   * \copydoc Serialize
   * \tparam T Shared / unique pointer type.
   * \details Serializes shared and unique pointers to identifiable objects.
   * \vprivate Component Private
   */
  template <typename T, typename std::enable_if<(::crypto::common::is_unique_pointer<T>::value ||
                                                 ::crypto::common::is_shared_pointer<T>::value) &&
                                                    ::crypto::common::is_identifiable<T>::value,
                                                std::int32_t>::type = 0>
  auto Serialize(T const& value, BufferPosition& buffer_pos, ara::crypto::WritableMemRegion buffer) const noexcept
      -> amsr::core::Result<void> {
    return Serialize(value.get(), buffer_pos, buffer);
  }

  /*!
   * \brief Serialize single value of the message.
   * \param[in] value  value to be serialized.
   * \param[out] buffer_pos  current position of the buffer, will be updated after serialization.
   * \param[out] buffer Output buffer to store serialized data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::cryp::CryptoObject::Type const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(amsr::core::String const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(amsr::core::StringView const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::Uuid const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::ReadOnlyMemRegion const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::WritableMemRegion const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::cryp::CryptoPrimitiveId::Category const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(::crypto::common::Identifiable const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(SerializationTags const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::keys::KeySlotPrototypeProps const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::keys::KeySlotContentProps const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::CryptoObjectUid const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::cryp::KeyType const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::x509::X509DN::AttributeId const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::ProviderType const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::x509::Certificate::Status const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::x509::AttributeCertificate::Status const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::keys::VersionControlType const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \vprivate Component Private
   */
  auto Serialize(ara::crypto::keys::UserPermissions const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \tparam LeftType Type of first pair element.
   * \tparam RightType Type of second pair element.
   * \vprivate Component Private
   * \internal
   * - Serialize first element into \c buffer
   * - If first element serialized successfully: serialize second element into \c buffer
   * \endinternal
   */
  template <typename LeftType, typename RightType>
  auto Serialize(std::pair<LeftType, RightType> const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void> {
    return Serialize(value.first, buffer_pos, buffer).AndThen([this, &value, &buffer_pos, buffer]() {
      return Serialize(value.second, buffer_pos, buffer);
    });
  }

  /*!
   * \copydoc PrimitiveSerializer::Serialize
   * \tparam ElementType type of the vector elements
   * \details Serializes a vector containing a generic element type.
   * \vprivate Component Private
   * \internal
   * - Serialize vector length into \c buffer
   * - If vector length serialized successfully: serialize vector elements \c buffer
   * \endinternal
   */
  template <typename ElementType>
  auto Serialize(ara::core::Vector<ElementType> const& value, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{ara::crypto::SecurityErrc::kRuntimeFault};
    static_assert(sizeof(std::size_t) == sizeof(typename ara::core::Vector<ElementType>::size_type),
                  "Sizes of std::size_t and ara::core::Vector::size_type must match!");
    std::uint64_t const vector_length{value.size()};
    if (!Serialize(vector_length, buffer_pos, buffer).HasValue()) {
      logger_.LogError(AMSR_FILE, __LINE__) << "Unable to serialize length.";
      result.EmplaceError(ara::crypto::SecurityErrc::kRpcInsufficientCapacity);
    } else {
      result.EmplaceValue();

      for (std::size_t i{0}; i < vector_length; ++i) {
        if (!Serialize(value[i], buffer_pos, buffer).HasValue()) {
          logger_.LogError(AMSR_FILE, __LINE__) << "Unable to serialize length.";
          result.EmplaceError(ara::crypto::SecurityErrc::kRpcInsufficientCapacity);
          break;
        }
      }
    }

    return result;
  }

  /*!
   * \brief Serialize raw data to destination.
   * \tparam InputIterator Type of iterator.
   * \param[in] start Start of data to be copied.
   * \param[in] end End of data to be copied.
   * \param[in,out] buffer_pos Position of the buffer.
   * \param[out] buffer Destination buffer.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Check if buffer size is sufficient to fit iterator range
   * - If size is sufficient: copy range into buffer
   * - Else: return kRpcInsufficientCapacity error
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_CRYPTO_AutosarC++17_10-A8.4.4_in_out_param
  template <typename InputIterator>
  auto Serialize(InputIterator start, InputIterator end, BufferPosition& buffer_pos,
                 ara::crypto::WritableMemRegion buffer) const noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result{ara::crypto::SecurityErrc::kRpcInsufficientCapacity};
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using DifferenceType = typename std::iterator_traits<InputIterator>::difference_type;
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using ValueType = typename std::iterator_traits<InputIterator>::value_type;
    DifferenceType const element_count{std::distance(start, end)};
    std::size_t const bytes{static_cast<std::size_t>(element_count) * sizeof(ValueType)};
    if (static_cast<std::size_t>(buffer.size()) >= bytes) {
      static_cast<void>(std::copy(start, end, &buffer[buffer_pos]));
      buffer_pos += bytes;
      result.EmplaceValue();
    } else {
      logger_.LogError(AMSR_FILE, __LINE__) << "Buffer too small to hold data.";
    }
    return result;
  }

  /*!
   * \brief Templated function to serialize all kinds of results except void.
   * \details Serialization of results will first add a boolean which indicates if the result holds a value or an error.
   * Afterwards it adds the value itself or the error code.
   * \tparam T Value type contained by the result.
   * \param[in] value Result to be serialized.
   * \param[in,out] buffer_pos position of the buffer.
   * \param[out] buffer Buffer to write the data into.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - If result has value: serialize HasValue() ("true") & value into \c buffer
   * - Else: serialize HasValue() ("false") & error into \c buffer
   * \endinternal
   */
  template <typename T, std::enable_if_t<!std::is_void<T>::value, std::int32_t> = 0>
  inline auto Serialize(amsr::core::Result<T> const& value, BufferPosition& buffer_pos,
                        ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void> {
    bool const has_value{value.HasValue()};
    return (has_value) ? Serialize(has_value, buffer_pos, buffer).AndThen([this, &value, &buffer_pos, buffer]() {
      return Serialize(value.Value(), buffer_pos, buffer);
    })
                       : Serialize(has_value, buffer_pos, buffer).AndThen([this, &value, &buffer_pos, buffer]() {
                           return Serialize(value.Error().Value(), buffer_pos, buffer);
                         });
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_function_cannot_be_made_static_false_positive
  /*!
   * \brief Partial template specialization of the result serialization method.
   * \param[in] value Result to be serialized.
   * \param[in,out] buffer_pos position of the buffer.
   * \param[out] buffer Buffer to write the data into.
   * \return
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Check if buffer size is sufficient to fit iterator range
   * - If size is sufficient: copy range into buffer
   * - Else: return kRpcInsufficientCapacity error
   * \endinternal
   */
  template <typename T, std::enable_if_t<std::is_void<T>::value, std::int32_t> = 0>
  inline auto Serialize(amsr::core::Result<T> const& value, BufferPosition& buffer_pos,
                        ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void> {
    return (value.HasValue())
               ? Serialize(value.HasValue(), buffer_pos, buffer)
               : Serialize(value.HasValue(), buffer_pos, buffer).AndThen([this, &value, &buffer_pos, buffer]() {
                   return Serialize(value.Error().Value(), buffer_pos, buffer);
                 });
  }

  /*!
   * \brief Templated function to serialize all kinds of Optionals.
   * \details Serialization of results will first add a boolean which indicates if the Optional holds a value or not.
   * Afterwards it adds the value itself.
   * \tparam T Value type contained by the result.
   * \param[in] value Result to be serialized.
   * \param[in,out] buffer_pos position of the buffer.
   * \param[out] buffer Buffer to write the data into.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - If result has value: serialize has_value() ("true") & value into \c buffer
   * - Else: serialize has_value() ("false") into \c buffer
   * \endinternal
   */
  template <typename T>
  inline auto Serialize(amsr::core::Optional<T> const& value, BufferPosition& buffer_pos,
                        ara::crypto::WritableMemRegion const& buffer) const noexcept -> amsr::core::Result<void> {
    return (value.has_value())
               ? Serialize(value.has_value(), buffer_pos, buffer).AndThen([this, &value, &buffer_pos, buffer]() {
                   return Serialize(value.value(), buffer_pos, buffer);
                 })
               : Serialize(value.has_value(), buffer_pos, buffer);
  }

 private:
  /*! Logger instance */
  ::crypto::common::util::Logger logger_{::crypto::common::util::kCommonLoggingContext, ""_sv};
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PRIMITIVE_SERIALIZER_H_
