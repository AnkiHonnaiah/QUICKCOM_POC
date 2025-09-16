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
/*!        \file  data_reader.h
 *        \brief  Reader interface for parsing serialized data.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_READER_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "crypto/common/data/data_container.h"

namespace crypto {
namespace common {
namespace data {

/*!
 * \brief Reader interface for parsing serialized data
 * \details Data loaded from files needs to be converted into the internal data representation (DataContainer) to be
 * usable. This interface provides the necessary functions for this conversion.
 * \vprivate Product Private
 */
class DataReader {
 public:
  /*!
   * \brief Destructor.
   * \vprivate Product Private
   */
  virtual ~DataReader() noexcept = default;

  /*!
   * \brief Default constructor
   * \vprivate Product Private
   */
  DataReader() = default;

  /*!
   * \brief Parses input data and writes its content into the internal data model.
   * \param[in] data The data to be read file.
   * \return DataContainer representing the information in \c data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual amsr::core::Result<DataContainer> Parse(ara::crypto::ReadOnlyMemRegion data) const = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Product Private
   */
  DataReader(DataReader const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Product Private
   */
  DataReader(DataReader&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \vprivate Product Private
   */
  DataReader& operator=(DataReader const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \vprivate Product Private
   */
  DataReader& operator=(DataReader&& /*other*/) & noexcept = default;
};

}  // namespace data
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_DATA_DATA_READER_H_
