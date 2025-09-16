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
/*!        \file  data_writer.h
 *        \brief  Writer interface for serialization of DataContainer objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_WRITER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/data/data_container.h"

namespace crypto {
namespace common {
namespace data {

/*!
 * \brief Writer interface for serialization of DataContainer objects
 * \details In order to persist data represented by the internal data model (DataContainer) it needs to be converted to
 * a binary or textual representation. This interface provides the necessary functions.
 * \vprivate Component Private
 */
class DataWriter {
 public:
  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  virtual ~DataWriter() noexcept = default;

  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  DataWriter() = default;

  /*!
   * \brief Write the data container as JSON to the output buffer.
   * \param[in] container Container to be serialized.
   * \param[out] buffer buffer to write the serialized data to.
   * \return Subspan of the input buffer that was used in the serialization.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual amsr::core::Result<ara::crypto::ReadOnlyMemRegion> WriteDataContainer(
      ::crypto::common::data::DataContainer const& container, ara::crypto::WritableMemRegion buffer) const = 0;

  /*!
   * \brief Calculates the size of the container when being serialized.
   * \param[in] container to be serialized.
   * \return size of the serialized data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual std::size_t CalculateSerializedSize(::crypto::common::data::DataContainer const& container) const = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  DataWriter(DataWriter const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  DataWriter(DataWriter&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  DataWriter& operator=(DataWriter const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \vprivate Component Private
   */
  DataWriter& operator=(DataWriter&& /*other*/) & noexcept = default;
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_WRITER_H_
