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
/*!        \file  ara/crypto/common/serializable.h
 *        \brief  Serializable object interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_SERIALIZABLE_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_SERIALIZABLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>

#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/std_api.h"
#include "ara/crypto/cryp/memory_pool.h"
#include "crypto/common/identifiable.h"

namespace ara {
namespace crypto {

/*!
 * \brief Serializable object interface.
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
class Serializable : public virtual ::crypto::common::Identifiable {
 protected:
  /*!
   * \brief Default constructor.
   * \vprivate Component Private
   */
  Serializable() = default;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~Serializable() noexcept override = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  Serializable(Serializable const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  Serializable(Serializable&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment constructor
   * \vprivate Component Private
   */
  Serializable& operator=(Serializable&& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  Serializable& operator=(Serializable const& /*other*/) & = default;

 public:
  /*!
   * \brief A container type for bit-flags of allowed usages of the key.
   * \vpublic
   */
  using FormatId = std::uint32_t;

  static FormatId const kFormatDefault{0L};      /*!< Default format. */
  static FormatId const kFormatRawValueOnly{1L}; /*!< Export only raw value of an object. */
  static FormatId const kFormatDerEncoded{2L};   /*!< Export DER-encoded value of an object. */
  static FormatId const kFormatPemEncoded{3L};   /*!< Export PEM-encoded value of an object. */

  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  // VECTOR NC AutosarC++17_10-A0.1.5: MD_CRYPTO_AutosarC++17_10-A0.1.5_pure_virtual
  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is interested).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity
   *         if (output.empty() == false), but it's capacity is less than required
   * \error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
   * \error SecurityErrorDomain::kUnsupportedFormat
   *         if the specified format ID is not supported for this object type
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual ara::core::Result<std::size_t> ExportPublicly(WritableMemRegion output = WritableMemRegion(),
                                                        FormatId format_id = kFormatDefault) const noexcept = 0;

  /*!
   * \brief Serialize itself publicly.
   * \tparam Alloc custom allocator type of the output container
   * \param[out] output  pre-reserved managed container for the serialization output
   * \param[in] format_id  the Crypto Provider specific identifier of the output format
   * \details This method sets the size of the output container according to actually saved value!
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrorDomain::kInsufficientCapacity
   *         if capacity of the output buffer is less than required
   * \error SecurityErrorDomain::kUnknownIdentifier  if an unknown format ID was specified
   * \error SecurityErrorDomain::kUnsupportedFormat
   *         if the specified format ID is not supported for this object type
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  /*!
   * \internal
   * - Resize the given output buffer
   * - Export the object into the buffer
   *   into the output buffer
   * - If exporting the object succeeds
   *   - Resize the output buffer to the size
         of serialized object
   * - Else fail with error
   * \endinternal
   */
  template <typename Alloc>
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_CRYPTO_AutosarC++17_10-A15.5.3_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  ara::core::Result<void> ExportPublicly(ara::core::Vector<Byte, Alloc>& output,
                                         FormatId format_id = kFormatDefault) const noexcept {
    output.resize(output.capacity());
    WritableMemRegion const out{output};
    ara::core::Result<std::size_t> size{ExportPublicly(out, format_id)};
    ara::core::Result<void> result{};
    if (size) {
      output.resize(size.Value());
    } else {
      result.EmplaceError(size.Error());
    }
    return result;
  }
};

}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_COMMON_SERIALIZABLE_H_
