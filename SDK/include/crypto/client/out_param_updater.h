/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set
 * out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  out_param_updater.h
 *        \brief  Wrapper for output parameter update used in BaseProxy class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_OUT_PARAM_UPDATER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_OUT_PARAM_UPDATER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include "amsr/core/array.h"
#include "crypto/client/type_traits.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace client {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::operator""_sv;

/*!
 * \brief Wrapper for output parameter update used in BaseProxy class
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation
 */
class OutParamUpdater {
 public:
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_template_functions_with_no_side_effects
  /*!
   * \brief Template for calling a given function with each element of an std::tuple.
   * \param[in] in_tuple    The tuple to be iterated.
   * \param[out] out_tuple  The tuple to update the arguments in.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <std::size_t StartIdx, typename InputTupleType, typename OutputTupleType>
  static void AssignAllOutParameters(InputTupleType& in_tuple, OutputTupleType& out_tuple) noexcept {
    constexpr std::size_t N{std::tuple_size<std::remove_reference_t<OutputTupleType>>::value};
    AssignForEachRecursive<StartIdx>(in_tuple, out_tuple, std::make_index_sequence<N>{});
  }

 private:
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_functions_with_no_side_effects
  // VECTOR NC AutosarC++17_10-A18.1.5: MD_CRYPTO_AutosarC++17_10-A18.1.5_const_unique_ptr
  /*!
   * \brief Dummy implementation.
   * \tparam T Type of the out parameter.
   * \param[in] in_param Object containing the pointer to the source memory object - unused parameter.
   * \param[in] out_param T Type object containing the pointer to the destination memory- unused parameter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename T>
  static void AssignSingleOutParameter(OutParameterDummyType in_param, T const& out_param) noexcept {
    static_cast<void>(in_param);
    static_cast<void>(out_param);
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_template_functions_with_no_side_effects
  /*!
   * \brief This Methods copies the data from the input memory region to the output memory region. As both parameters
   *        are value objects, it updates the memory behind the pointer in the passed objects and does not update
   *        the arguments themselves.
   * \param[in] in_param MemRegion object containing the pointer to the source memory object.
   * \param[in] out_param MemRegion object containing the pointer to the destination memory.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static void AssignSingleOutParameter(ara::crypto::WritableMemRegion in_param,
                                       ara::crypto::WritableMemRegion out_param) noexcept {
    if (in_param.size_bytes() > out_param.size_bytes()) {
      crypto::common::util::Logger::LogFatalAndAbort(common::util::kClientLoggingContext, AMSR_FILE, __LINE__,
                                                     "The destination buffer is smaller than the input buffer"_sv);
    }
    static_cast<void>(std::copy(in_param.begin(), in_param.end(), out_param.begin()));
  }

  /*!
   * \brief Method for updating out parameters. After successful execution of the method, the skeleton
   *        returns a message containing all possibly changed parameters. The parameters are used to assign the
   *        original parameters to mirror the state of output parameters to the proxy class.
   * \details This method handles parameters with reference type.
   * \tparam OutParameterType Template parameter for generic implementation.
   * \param[in] in_param Parameter returned from the skeleton (=server) side, used as new value.
   * \param[out] out_param Parameter to be updated (=client).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename OutParameterType>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  static void AssignSingleOutParameter(OutParameterType const& in_param, OutParameterType& out_param) noexcept {
    out_param = in_param;
  }

  /*!
   * \brief This method copies the data from the input to the output type. As both parameters are pointers objects,
   *        it updates the memory behind the pointer in the passed objects and does not update the arguments themselves.
   * \details This method handles parameters with pointer types.
   * \tparam OutParameterType Template parameter for generic implementation.
   * \param[in] in_param New value.
   * \param[out] out_param Old value to be updated.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename OutParameterType,
            typename std::enable_if<!(common::is_identifiable<OutParameterType>::value), std::int32_t>::type = 0>
  static void AssignSingleOutParameter(OutParameterType* const& in_param, OutParameterType*& out_param) noexcept {
    if ((out_param != nullptr) && (in_param != nullptr)) {
      *out_param = *in_param;
    } else if (out_param != nullptr) {
      out_param = nullptr;
      // VECTOR NC AutosarC++17_10-M6.4.2_false_positive: MD_CRYPTO_AutosarC++17_10-M6.4.2_false_positive
    } else {
      // this else clause is required to satisfy AutosarC++17_10-M6.4.2
    }
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_template_functions_with_no_side_effects
  /*!
   * \brief Dummy implementation.
   * \details This method handles parameters with reference const types (and does nothing because they are const).
   * \tparam T Type of the out parameter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename OutParameterType>
  static void AssignSingleOutParameter(OutParameterType const& /*in_param*/,
                                       OutParameterType const& /*out_param*/) noexcept {}

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_CRYPTO_AutosarC++17_10-M0.1.8_template_functions_with_no_side_effects
  /*!
   * \brief Template (internal recursion) for calling a given function with each element of an std::tuple. Template
   *        takes two tuples and calls the method always with in_tuple(n) and out_tuple(n+1) because the messages
   *        that will be processed contain on the input side all argument values and on the output side first the
   *        return value and the following the updated argument values. This requires the index shift for the out_tuple.
   * \param[in] in_tuple The tuple to be iterated.
   * \param[out] out_tuple The tuple to update the arguments in.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <std::size_t MessageIndexOffset, typename InputTupleType, typename OutputTupleType, std::size_t... Indices>
  static void AssignForEachRecursive(InputTupleType& in_tuple, OutputTupleType& out_tuple,
                                     std::index_sequence<Indices...>) noexcept {
    // Number of elements of the swallow array = number of indices in the the tuples + 1 for the message index offset
    constexpr std::size_t const kNumElements{sizeof...(Indices) + 1};
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using swallow = amsr::core::Array<std::int32_t, kNumElements>;
    // Note: If the input tuple contains the return value as the first argument, the offset is 1 to skip it. In case of
    // void, this offset is zero
    // VECTOR Disable AutosarC++17_10-M5.18.1: MD_CRYPTO_AutosarC++17_10-M5.18.1_comma_operator
    // VECTOR Disable AutosarC++17_10-M5.0.8: MD_CRYPTO_AutosarC++17_10-M5.0.8_static_cast_with_comma_operator
    // VECTOR NC AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    static_cast<void>(swallow{
        static_cast<std::int32_t>(MessageIndexOffset),
        static_cast<std::int32_t>(
            AssignSingleOutParameter(std::get<Indices + MessageIndexOffset>(in_tuple), std::get<Indices>(out_tuple)),
            std::int32_t{})...});
    // VECTOR Enable AutosarC++17_10-M5.18.1
    // VECTOR Enable AutosarC++17_10-M5.0.8
  }
};

}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_OUT_PARAM_UPDATER_H_
