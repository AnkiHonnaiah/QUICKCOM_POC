
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
/*!        \file
 *        \brief  Generic proxy request serializer class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_REQUEST_SERIALIZER_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_REQUEST_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_type.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Used to directly write already serialized method parameters to a packet. Normally, serialization would take
 *        place here, but as the parameters are already serialized, it will just write them directly to the packet that
 *        is being constructed.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyMethod
 */
class AMSR_LG_BETA GenericProxyRequestSerializer {
 public:
  /*!
   * \brief Returns the combined size of all spans in the GenericDataType.
   *
   * \param[in] parameters  The generic parameters to serialize.
   *
   * \return Combined size of the buffers contained in parameters.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  static auto GetRequiredBufferSize(socal::GenericDataType parameters) noexcept -> size_t;

  /*!
   * \brief Writes all the parameters to the given writer.
   *
   * \param[in,out] writer      Destination where the parameters will be written to.
   * \param[in]     parameters  List of spans to write to the writer.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  static void Serialize(someip_protocol::internal::serialization::Writer& writer, socal::GenericDataType parameters);
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_REQUEST_SERIALIZER_H_
