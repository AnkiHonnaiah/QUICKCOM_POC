/**********************************************************************************************************************
 * COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 * \verbatim
 * Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *               This software is copyright protected and proprietary to Vector Informatik GmbH.
 *               Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *               All other rights remain with Vector Informatik GmbH.
 * \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 * FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *       \brief  Implementation of an offer state.
 *
 *     \details
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_OFFER_STATE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_OFFER_STATE_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief   The OfferState.
 * \details The offer state can have the following states:
 *          - True for is offered.
 *          - False for is not offered.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonInstance
 */
class OfferState final {
 public:
  /*!
   * \brief Construct a offer state.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  OfferState() = default;

  /*!
   * \brief Destruct the offer state.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~OfferState() = default;

  OfferState(OfferState const&) noexcept = delete;
  auto operator=(OfferState const&) noexcept -> OfferState& = delete;
  OfferState(OfferState&& other) noexcept = delete;
  auto operator=(OfferState&& other) noexcept -> OfferState& = delete;

  /*!
   * \brief Set the stet to offered.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void Offer();

  /*!
   * \brief Set the stet to stopped.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void StopOffer();

  /*!
   * \brief Conversion operator to bool.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  explicit operator bool() const;

 private:
  /*! The offered state, true when offered, false when stopped. */
  bool is_offered_{};
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_OFFER_STATE_H_
