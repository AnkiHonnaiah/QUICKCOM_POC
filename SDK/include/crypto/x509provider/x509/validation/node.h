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
/*!        \file  node.h
 *        \brief  Class representation of a single node in the certification path.
 *      \details  A node is a collection of CA certificates with identical subject DNs.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_SRC_CRYPTO_X509PROVIDER_X509_VALIDATION_NODE_H_
#define LIB_LIBX509PROVIDER_SRC_CRYPTO_X509PROVIDER_X509_VALIDATION_NODE_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/vector.h"
#include "crypto/x509provider/x509/certificate_impl.h"

namespace crypto {
namespace x509provider {
namespace x509 {
namespace validation {

/*!
 * \brief           Class representation of a single node in the certification path.
 */
class Node final {
 public:
  /*!
   * \brief         Type alias for a shared pointer to a Node.
   */
  using Sptr = std::shared_ptr<Node>;

  /*!
   * \brief         Type alias for a Node element.
   */
  using NodeElement = x509::Certificate;

  /*!
   * \brief         Type alias for a container of Node elements.
   */
  using ElementContainer = ara::core::Vector<std::reference_wrapper<NodeElement const>>;

  /*!
   * \brief         Construct an empty Node.
   */
  Node() noexcept = default;

  /*!
   * \brief         Construct a new Node instance.
   * \param[in]     certificates The certificates contained in this node
   * \context       ANY
   * \pre           All certificates in \c certificates share the same subject DN
   * \vprivate      Component Private
   */
  explicit Node(ElementContainer certificates) noexcept;

  /*!
   * \brief         Destroy this Node instance.
   */
  ~Node() noexcept = default;

  /*!
   * \brief         Copy construct a Node instance.
   */
  Node(Node const&) noexcept = default;

  /*!
   * \brief         Move construct a Node instance.
   */
  Node(Node&&) noexcept = default;

  /*!
   * \brief         Copy assign a Node instance.
   * \return        reference to this
   * \vprivate      Component Private
   */
  auto operator=(Node const&) & noexcept -> Node& = default;

  /*!
   * \brief         Move assign a Node instance.
   * \return        reference to this
   * \vprivate      Component Private
   */
  auto operator=(Node&&) & noexcept -> Node& = default;

  /*!
   * \brief         Access the currently active element of the node.
   * \return        The currently active element of this node
   * \context       ANY
   * \threadsafe    TRUE
   * \error         SecurityErrc::kEmptyContainer If there are no node elements left (<tt>IsEmpty() == true</tt>)
   * \synchronous   TRUE
   * \reentrant     TRUE
   * \vprivate      Component Private
   */
  auto CurrentElement() const noexcept -> amsr::core::Result<NodeElement const&> {
    amsr::core::Result<NodeElement const&> result{ara::crypto::SecurityErrc::kEmptyContainer};

    if (current_element_ != elements_.end()) {
      result.EmplaceValue(*current_element_);
    }

    return result;
  }

  /*!
   * \brief         Drop the currently active element of the node.
   * \details       After this, \c CurrentElement() will return the next element of this node.
   * \context       ANY
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \reentrant     TRUE
   * \vprivate      Component Private
   */
  void DropCurrentElement() noexcept {
    if (current_element_ != elements_.end()) {
      current_element_++;
    }
  }

  /*!
   * \brief         Set the Node preceding this one.
   * \param[in]     node The node to set as previous
   * \context       ANY
   * \threadsafe    FALSE
   * \synchronous   TRUE
   * \reentrant     TRUE
   * \vprivate      Component Private
   */
  void SetPrevious(Sptr node) noexcept { previous_ = std::move(node); }

  // VECTOR NC AutosarC++17_10-M9.3.1: MD_CRYPTO_AutosarC++AutosarC++17_10-M9.3.1_smart_pointer
  /*!
   * \brief         Get the Node preceding this one.
   * \return        The previous node in the current path
   */
  auto GetPrevious() const noexcept -> Sptr { return previous_; }

  /*!
   * \brief         Determine if the Node is empty.
   * \return        Iterator pointing to the current element
   */
  auto IsEmpty() const noexcept -> bool { return current_element_ == elements_.end(); }

  /*!
   * \brief         Determine whether this Node contains a certificate.
   * \param[in]     certificate The certificate to find
   * \return        True if this Node contains given \c certificate
   * \context       ANY
   * \threadsafe    TRUE
   * \synchronous   TRUE
   * \vprivate      Component Private
   */
  auto Contains(NodeElement const& certificate) const noexcept -> bool;

 private:
  /*! The previous node in the current path */
  Sptr previous_;
  /*! The elements that make up this node */
  ElementContainer elements_;
  /*! Iterator pointing to the current element */
  ElementContainer::iterator current_element_;
};

}  // namespace validation
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_SRC_CRYPTO_X509PROVIDER_X509_VALIDATION_NODE_H_
