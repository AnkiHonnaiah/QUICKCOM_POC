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
/*!        \file  primitive_deserializer.h
 *        \brief  Deserializer for data primitives used in message transmission.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PRIMITIVE_DESERIALIZER_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PRIMITIVE_DESERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "amsr/core/error_domain.h"
#include "amsr/core/string.h"
#include "amsr/sec/ipc/message_object_lifetime_manager.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "ara/crypto/common/exceptions.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/x509/x509_dn.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Representation of pointers to remote identifiables.
 * \vprivate Component Private
 */
struct IdentifiablePointer {
  bool is_nullptr{true}; /*!< Indicates if the pointer is null. */
  ProxyId proxy_id{};    /*!< Contains the proxy ID. */
};

// VECTOR NC Metric-OO.WMC.One, Metric-OO.RFC: MD_CRYPTO_Metric-OO.WMC.One, MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Deserializer for data primitives.
 *
 * \details For message transmission we need to be able to serialize and deserialize the arguments and results of
 * function calls. This class contains several functions which provide these deserialization capabilities for different
 * data types.
 * \vprivate Component Private
 */
class PrimitiveDeserializer {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  PrimitiveDeserializer() = default;

  /*!
   * \brief Destructs the PrimitiveDeserializer.
   * \vprivate Component Private
   */
  virtual ~PrimitiveDeserializer() noexcept = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  PrimitiveDeserializer(PrimitiveDeserializer const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  PrimitiveDeserializer(PrimitiveDeserializer&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  PrimitiveDeserializer& operator=(PrimitiveDeserializer const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  PrimitiveDeserializer& operator=(PrimitiveDeserializer&& /*other*/) & noexcept = default;

  /*!
   * \brief Checker function to ensure that the buffer contains enough data for deserialization.
   * \param[in] size Size to be read from the buffer.
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
   * \brief Implements common deserialization for fundamental arithmetic types.
   * \tparam T The fundamental arithmetic type.
   * \param[in] buffer The buffer itself.
   * \param[in,out] buffer_pos The current buffer position.
   * \param[out] out_value Deserialized value.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Assert that size is sufficient
   * - Deserialize the value from \c buffer into \c out_value
   * - Adjust \c buffer_pos
   * \endinternal
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::int32_t> = 0>
  static auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos, T& out_value) noexcept
      -> amsr::core::Result<void> {
    return PrimitiveDeserializer::CheckSize(sizeof(T), buffer, buffer_pos).AndThen([buffer, &buffer_pos, &out_value]() {
      std::memcpy(&out_value, &buffer[buffer_pos], sizeof(T));
      buffer_pos += sizeof(T);
      return amsr::core::Result<void>::FromValue();
    });
  }

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::WritableMemRegion& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::ReadOnlyMemRegion& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::CryptoObjectType& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::cryp::KeyType& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::x509::X509DN::AttributeId& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   amsr::core::String& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::Uuid& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   SerializationTags& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::keys::KeySlotPrototypeProps& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::keys::KeySlotContentProps& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::CryptoObjectUid& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::keys::UserPermissions& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::x509::Certificate::Status& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::x509::AttributeCertificate::Status& out_value) const noexcept
      -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::ProviderType& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::keys::VersionControlType& out_value) const noexcept -> amsr::core::Result<void>;

  /*!
   * \copydoc PrimitiveDeserializer::Deserialize
   * \vprivate Component Private
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::crypto::cryp::CryptoPrimitiveId::Category& out_value) const noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Specific deserialization function for pointers.
   * \details Deserializes pointers to identifiable / proxy objects.
   * \tparam PointerType Type of the pointer
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize serialization tag from \c buffer
   * - If serialization succeeded:
   *    - If tag is tag_normal_pointer:
   *        - Adjust \c buffer_pos to skip proxy id
   * \endinternal
   */
  template <typename PointerType, typename std::enable_if<((std::is_pointer<PointerType>::value) ||
                                                           (::crypto::common::is_shared_pointer<PointerType>::value) ||
                                                           (::crypto::common::is_unique_pointer<PointerType>::value)) &&
                                                              ::crypto::common::is_identifiable<PointerType>::value,
                                                          std::int32_t>::type = 0>
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   PointerType /*dummy_value*/) const noexcept -> amsr::core::Result<void> {
    // Pointers are encoded using a Serialization tag (nullptr / normal_ptr). For normal pointers, after this tag, the
    // object id follows. For nullptrs, nothing follows. So this method must take care for that and move the buffer
    // position correctly.
    SerializationTags tag{};
    return Deserialize(buffer, buffer_pos, tag).AndThen([&buffer_pos, &tag]() {
      if (tag == SerializationTags::tag_normal_pointer) {
        buffer_pos += sizeof(ProxyId);
      }
      return amsr::core::Result<void>::FromValue();
    });
  }

  /*!
   * \brief Specific deserialization function for identifiable pointers.
   * \details Deserializes the information, if a pointer to an identifiable is not null.
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize serialization tag from \c buffer
   * - If serialization succeeded:
   *    - If tag is tag_normal_pointer:
   *        - Set is_nullptr to false
   *        - Deserialize proxy id from \c buffer
   *    - Else:
   *        - Set is_nullptr to true
   * \endinternal
   */
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   IdentifiablePointer& out_value) const noexcept -> amsr::core::Result<void> {
    // Pointers are encoded using a Serialization tag (nullptr / normal_ptr). For normal pointers, after this tag, the
    // object id follows. For nullptrs, nothing follows. So this method must take care for that and move the buffer
    // position correctly.
    SerializationTags tag{};
    return Deserialize(buffer, buffer_pos, tag).AndThen([&buffer, &buffer_pos, &out_value, &tag]() {
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
      auto result = amsr::core::Result<void>::FromValue();
      if (tag == SerializationTags::tag_normal_pointer) {
        out_value.is_nullptr = false;
        result = Deserialize(buffer, buffer_pos, out_value.proxy_id);
      } else {
        out_value.is_nullptr = true;
      }
      return result;
    });
  }

  /*!
   * \brief Specific deserialization function for std::pair.
   * \tparam LeftType First type of the pair.
   * \tparam RightType Second type of the pair.
   * \details Deserializes a pair type.
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize left value from \c buffer
   * - Deserialize right value from \c buffer
   * - If both succeeded: Return pair of {left, right}
   * \endinternal
   */
  template <typename LeftType, typename RightType>
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   std::pair<LeftType, RightType>& out_value) const noexcept -> amsr::core::Result<void> {
    LeftType left{};
    RightType right{};

    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto const deser_left_result = Deserialize(buffer, buffer_pos, left);
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto const deser_right_result = Deserialize(buffer, buffer_pos, right);

    return deser_left_result.And(deser_right_result).AndThen([&out_value, &left, &right]() {
      out_value = {left, right};
      return amsr::core::Result<void>::FromValue();
    });
  }

  /*!
   * \brief Specific deserialization function for vectors of generic elements.
   * \tparam ElementType type of the vector elements
   * \details Deserializes a vector containing a generic element type.
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize vector length from \c buffer
   * - Deserialize vector elements from \c buffer
   * \endinternal
   */
  template <typename ElementType>
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   ara::core::Vector<ElementType>& out_value) const noexcept -> amsr::core::Result<void> {
    static_assert(sizeof(std::size_t) == sizeof(typename ara::core::Vector<ElementType>::size_type),
                  "Sizes of std::size_t and ara::core::Vector::size_type must match!");
    std::uint64_t vector_length{0};
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = Deserialize(buffer, buffer_pos, vector_length);
    // VECTOR NL AutosarC++17_10-A6.5.1: MD_CRYPTO_AutosarC++17_10-A6.5.1_not_a_container_loop
    for (std::size_t i{0}; i < vector_length; i++) {
      // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_default_constructor
      ElementType element{};
      result = result.And(Deserialize(buffer, buffer_pos, element));
      if (result.HasValue()) {
        out_value.emplace_back(element);
      } else {
        break;
      }
    }
    return result;
  }

  /*!
   * \brief Specific deserialization function for spans of generic, arithmetic elements.
   * \tparam ElementType type of the span elements
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize vector length from \c buffer
   * - Deserialize vector elements from \c buffer
   * \endinternal
   */
  template <typename ElementType, std::enable_if_t<std::is_arithmetic<ElementType>::value, std::int32_t> = 0>
  static auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                          amsr::core::Span<ElementType const>& out_value) noexcept -> amsr::core::Result<void> {
    static_assert(sizeof(std::size_t) == sizeof(typename amsr::core::Span<ElementType>::size_type),
                  "Sizes of std::size_t and amsr::core::Span::size_type must match!");
    std::size_t span_length{0};
    return Deserialize(buffer, buffer_pos, span_length).AndThen([buffer, &buffer_pos, &out_value, &span_length]() {
      return PrimitiveDeserializer::CheckSize(span_length, buffer, buffer_pos)
          .AndThen([buffer, &buffer_pos, &out_value, &span_length]() {
            // VECTOR NC AutosarC++17_10-A5.2.4: MD_CRYPTO_AutosarC++17_10-A5.2.4_reinterpret_cast_rpc
            out_value = amsr::core::Span<ElementType const>{
                reinterpret_cast<ElementType const*>(buffer.subspan(buffer_pos).data()), span_length};
            buffer_pos += span_length * sizeof(ElementType);
            return amsr::core::Result<void>::FromValue();
          });
    });
  }

  /*!
   * \brief Specific deserialization function for results.
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize has_value boolean from \c buffer
   * - If serialization succeeded:
   *    - If has_value is true:
   *        - Deserialize value & emplace it into the result on success
   *    - Else:
   *        - Deserialize error code & emplace it into the result on success
   * \endinternal
   */
  template <typename T, std::enable_if_t<!std::is_void<T>::value, std::int32_t> = 0>
  auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                   amsr::core::Result<T>& out_value) const noexcept -> amsr::core::Result<void> {
    bool has_value{false};
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = Deserialize(buffer, buffer_pos, has_value);

    if (result.HasValue()) {
      // VECTOR NL AutosarC++17_10-M0.1.2: MD_CRYPTO_AutosarC++17_10-M0.1.2_branch_dead_false_positive
      if (has_value) {
        T value{};
        result = Deserialize(buffer, buffer_pos, value);
        static_cast<void>(result.Inspect([&out_value, &value]() { out_value.EmplaceValue(value); }));
      } else {
        amsr::core::ErrorDomain::CodeType error_code{};
        result = Deserialize(buffer, buffer_pos, error_code);
        if (result.HasValue()) {
          // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
          out_value.EmplaceError(static_cast<ara::crypto::SecurityErrc>(error_code));
        }
      }
    }

    return result;
  }

  /*!
   * \brief Specific deserialization function for optionals.
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \return Deserialized result.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize has_value boolean from \c buffer
   * - If serialization succeeded:
   *    - If has_value is true:
   *        - Deserialize value & emplace it into the result on success
   * \endinternal
   */
  template <typename T, std::enable_if_t<!std::is_void<T>::value, std::int32_t> = 0>
  static auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                          amsr::core::Optional<T>& out_value) noexcept -> amsr::core::Result<void> {
    bool has_value{false};
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = Deserialize(buffer, buffer_pos, has_value);

    // VECTOR NL AutosarC++17_10-M0.1.2: MD_CRYPTO_AutosarC++17_10-M0.1.2_branch_dead_false_positive
    if ((result.HasValue()) && (has_value)) {
      T value{};
      result = Deserialize(buffer, buffer_pos, value);
      static_cast<void>(result.Inspect([&out_value, &value]() { out_value.emplace(value); }));
    }

    return result;
  }

  /*!
   * \brief Partial template specialization for the type void.
   * \param[in] buffer The buffer holding the data.
   * \param[in,out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \param[out] out_value Deserialized value.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize has_value boolean from \c buffer
   * - If serialization succeeded:
   *    - If has_value is true:
   *        - Emplace value (void)
   *    - Else:
   *        - Deserialize error code & emplace it into the result on success
   * \endinternal
   */
  template <typename T, std::enable_if_t<std::is_void<T>::value, std::int32_t> = 0>
  static auto Deserialize(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos,
                          amsr::core::Result<T>& out_value) noexcept -> amsr::core::Result<void> {
    bool has_value{false};
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = Deserialize(buffer, buffer_pos, has_value);
    if (result.HasValue()) {
      // VECTOR NL AutosarC++17_10-M0.1.2: MD_CRYPTO_AutosarC++17_10-M0.1.2_branch_dead_false_positive
      if (has_value) {
        out_value.EmplaceValue();
      } else {
        amsr::core::ErrorDomain::CodeType error_code{};
        result = Deserialize(buffer, buffer_pos, error_code);
        static_cast<void>(result.Inspect([&out_value, error_code]() {
          // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
          out_value.EmplaceError(static_cast<ara::crypto::SecurityErrc>(error_code));
        }));
      }
    }
    return result;
  }
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_PRIMITIVE_DESERIALIZER_H_
