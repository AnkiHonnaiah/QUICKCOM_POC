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
/*!        \file   someip_binding_lite/raw_data.h
 *         \brief  Data type for a raw buffer.
 *         \unit   SomeIpBinding::SomeIpBindingLite::Communication::Events::SomeIpBindingLiteSamplePtr
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_RAW_DATA_H_
#define LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_RAW_DATA_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/someip_binding_lite/data_types.h"
#include "amsr/someip_binding_lite/internal/data_types.h"
#include "amsr/someip_binding_lite/sample_ptr.h"

namespace amsr {
namespace someip_binding_lite {

/*!
 * \brief Data type for a raw buffer.
 * \vpublic
 */
struct RawData {
  /*!
   * \brief Pointer to the data.
   */
  SamplePtr data_pointer;

  /*!
   * \brief The number of bytes in the buffer.
   */
  std::size_t number_of_bytes;
};

}  // namespace someip_binding_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_LITE_INCLUDE_AMSR_SOMEIP_BINDING_LITE_RAW_DATA_H_
