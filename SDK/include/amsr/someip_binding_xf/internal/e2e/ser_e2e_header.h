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
/*!        \file  ser_e2e_header.h
 *        \brief  This header file includes the serialization class of E2E header.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::E2EHeaderSerializer
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_SER_E2E_HEADER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_SER_E2E_HEADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/e2e/profiles/profile_protector.h"
#include "amsr/e2e/transformers/client_side_transformer.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_xf/internal/e2e/e2e_cfg.h"
#include "amsr/someip_binding_xf/internal/e2e/server_side_transformer.h"
#include "ara/com/e2e_types.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace e2e {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Writes in primitive data types to a buffer view.
 */
using Writer = ::amsr::someip_protocol::internal::serialization::Writer;

/*!
 * \brief Buffer view to wrap the data to inspect.
 */
using Buffer = ::amsr::someip_protocol::internal::serialization::BufferView;

// ---- E2E protection serializer --------------------------------------------------------------

/*!
 * \brief Serialization class of E2E protection header.
 * \details An object of this class is meant to be created, used and destroyed within
 * the same function context. It is not meant to be locally stored, copied, moved or shared.
 */
class E2EHeaderSerializer final {
 private:
  /*!
   * \brief Type alias for server side transformer.
   */
  using TransformerType = ::amsr::someip_binding_xf::internal::e2e::ServerSideTransformer;

 public:
  /*!
   * \brief The constructor of E2EHeaderSerializer
   *
   * \param[in, out] writer            Points to the correct place within the buffer where the data
   *                                   should be serialized.
   * \param[in, out] transformer       E2E transformer, protects the payload and writes the E2E header.
   * \param[in, out] buffer            The buffer which holds the serialized data.
   * \param[in]      protected_offset  Byte offset in the packet buffer to the payload which shall
   *                                   get protected by E2E.
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   *
   * \internal steady TRUE \endinternal
   */
  explicit E2EHeaderSerializer(Writer& writer, TransformerType& transformer, Buffer& buffer,
                               std::uint8_t const protected_offset = 0U)
      : writer_{writer}, transformer_{transformer}, buffer_{buffer}, protected_offset_{protected_offset} {
    // Allocate the E2E header in packet. Header will be written in context of Close()
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    writer_.skip(transformer_.GetHeaderSize());
  }

  /*!
   * \brief Close the E2E header serializer
   *
   * \internal
   * - If the result of the serialization is kOk
   *   - Protect the payload byte stream and update the E2E header
   *   - If the protection is not successful, set the result to kNotOk
   * - Return the result of the serialization
   * \endinternal
   *
   * \pre          -
   * \context      App
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \trace SPEC-13650577
   * \trace SPEC-4980044
   * \spec
   *   requires true;
   * \endspec
   */
  void Close() {
    // Protect the payload byte stream and update the E2E header
    ::amsr::e2e::profiles::ProfileProtector::ProtectReturnType const protect_return{
        transformer_.Protect(buffer_, protected_offset_)};
    if (!protect_return.HasValue()) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"E2EHeaderSerializer"},
              [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
                s << "Serialization of event sample failed because of an E2E protection failure.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief Reference to used writer class which writes primitives to the byte buffer.
   */
  Writer& writer_;

  /*!
   * \brief Reference to used transformer object which handles E2E protection and update of E2E header.
   */
  TransformerType& transformer_;

  /*!
   * \brief Reference to the byte buffer which holds the serialized data.
   */
  Buffer& buffer_;

  /*!
   * \brief Byte offset in the packet buffer to the payload which shall get protected by E2E.
   */
  std::size_t const protected_offset_;
};

}  // namespace e2e
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_E2E_SER_E2E_HEADER_H_
