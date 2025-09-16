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
/*!        \file  someip_binding_xf/internal/methods/skeleton_method_request_xf.h
 *         \brief SkeletonMethodRequestXf represents a SomeIp method request.
 *         \unit SomeIpBinding::SomeIpBindingXf::Communication::Methods::SkeletonMethodRequestXf
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_XF_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include "amsr/socal/internal/methods/skeleton_method_request_interface.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_request_deserializer_interface.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief RequestXf represents a SomeIp method request to be processed by a server.
 *        It provides a way to retrieve the method arguments which are contained in the packet.
 *
 * \tparam InputArgs A variadic list of data types that correspond to the types of the elements of InputType.
 */
template <typename... InputArgs>
// VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
class SkeletonMethodRequestXf final
    : public ::amsr::socal::internal::methods::SkeletonMethodRequestInterface<InputArgs...> {
  /*!
   * \brief Deserializer which is used to create an InputType object from a provided Packet
   */
  using Deserializer = ::amsr::someip_binding_xf::internal::ser_deser::MethodRequestDeserializerInterface<InputArgs...>;

  /*!
   * \brief Type alias for SkeletonMethodRequestInterface.
   */
  using SkeletonMethodRequestInterface = ::amsr::socal::internal::methods::SkeletonMethodRequestInterface<InputArgs...>;

 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] packet_buffer_ptr The memory buffer pointer contains header + payload which will be deserialized.
   * \param[in] deserializer      The Deserializer which deserializes the method arguments.
   *
   * \pre           -
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   */
  SkeletonMethodRequestXf(::amsr::someip_binding_core::internal::MemoryBufferPtr packet_buffer_ptr,
                          Deserializer& deserializer)
      : SkeletonMethodRequestInterface{},
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        packet_buffer_ptr_{std::move(packet_buffer_ptr)},
        // [MALLOC][small] uses malloc to allocate memory
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
        // VCA_SOMEIPBINDING_POSSIBLY_ACCESSING_NULLPTR_RETRIEVED_FROM_EXTERNAL_FUNCTION
        packet_view_{packet_buffer_ptr_->GetView(0U)},
        deserializer_{deserializer} {}

 private:
  /*!
   * \brief         Retrieves request argument(s).
   * \details       Arguments should be provided in a form that can be used directly by the method call (e.g.
   *                deserialized). It is intended to be invoked right before the method would be called.
   * \param[out]    args Tuple containing the default-initialized method request arguments.
   * \return        True if all the arguments are successfully deserialized, False otherwise.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \steady        TRUE
   * \spec
   *   requires true;
   * \endspec
   * \vprivate Product Private
   *
   * \internal
   * - deserialize the given method request into struct data type
   * - if deserialization is successful
   *   - copy content of struct data type into provided tuple
   * \endinternal
   */
  bool GetArgs(std::tuple<InputArgs...>& args) const noexcept final {
    bool deserialization_result{false};
    std::size_t constexpr kSomeIpHeaderSize{::amsr::someip_protocol::internal::kHeaderSize};

    // Prepare Reader
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
    ::amsr::someip_protocol::internal::deserialization::BufferView const body_view{
        // VCA_SOMEIPBINDING_POSSIBLY_ACCESSING_NULLPTR_RETRIEVED_FROM_EXTERNAL_FUNCTION
        packet_view_.data(), packet_buffer_ptr_->GetView(0U).size()};

    // Skip the header
    ::amsr::someip_protocol::internal::deserialization::BufferView const buffer_view{
        body_view.subspan(kSomeIpHeaderSize, body_view.size() - kSomeIpHeaderSize)};

    // Deserialize Payload
    ::amsr::someip_protocol::internal::deserialization::Reader reader{buffer_view};
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    deserialization_result = deserializer_.Deserialize(reader, args);

    return deserialization_result;
  }

  /*!
   * \brief Buffer which contains a SomeIp method request to be deserialized.
   */
  ::amsr::someip_binding_core::internal::MemoryBufferPtr packet_buffer_ptr_;

  /*!
   * \brief MemoryBufferView to the buffer which is used during deserialization.
   */
  ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView packet_view_;

  /*!
   * \brief Deserializer.
   */
  Deserializer& deserializer_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_XF_H_
