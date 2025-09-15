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
 *        \brief  A memory resource consists of a collection of pools that serves requests for different block sizes.
 *         \unit  VaCommonLib::SynchronizedPoolResource
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_SYNCHRONIZED_POOL_RESOURCE_H_
#define LIB_VAC_INCLUDE_ARA_CORE_SYNCHRONIZED_POOL_RESOURCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include "amsr/internal/single_size_pool.h"
#include "ara/core/memory_resource.h"

namespace ara {
namespace core {

/*!
 * \brief           Get the block size for a given number of bytes
 * \param[in]       bytes
 *                  The number of bytes to get the block size for.
 * \returns         The smallest power of 2 that is larger than the given number of bytes.
 */
auto GetBlockSize(std::size_t bytes) noexcept -> std::size_t;

/*! \brief          The maximum number of blocks */
static constexpr std::size_t kMaxBlocksPerChunk{8};

/*!
 * \brief           Struct is a set of constructor options(max_blocks_per_chunk, largest_required_pool_block) for pool
 *                  resources
 */
struct PoolOptions final {
  /*! \brief          The maximum number of blocks that will be allocated at once from the upstream resource */
  std::size_t max_blocks_per_chunk;
  /*! \brief          The largest allocation size that is required to be fulfilled using the pooling mechanism */
  std::size_t largest_required_pool_block;
};

/*!
 * \brief           Memory resource with one or multiple pools of fixed sized blocks
 * \details         An instance of SynchronizedPoolResource can have only pools with the same block size. Pools are
 *                  created by calling Reserve() with the number of blocks and the block size. If Reserve() is called
 *                  multiple times, the same block size must be used.
 * \vprivate        PES Private
 * \threadsafe      TRUE
 * \trace           CREQ-VaCommonLib-SynchronizedPoolResource
 */
class SynchronizedPoolResource : public MemoryResource {
  /*! \brief          declaration for testing purposes */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  FRIEND_TEST(UT__SynchronizedPoolResource, CorrectPoolSize);

  /*! \brief          declaration for testing purposes */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  FRIEND_TEST(UT__SynchronizedPoolResource, Allocate__AllocateFromUpstreamResource);

 public:
  /*!
   * \brief           Constructs a new instance of the Synchronized SingleSizePool Resource class
   * \details         The upstream resource is set to ara::core::GetDefaultResource().
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  SynchronizedPoolResource() noexcept = default;

  /*!
   * \brief           Constructs a new instance of the Synchronized SingleSizePool Resource class
   * \param[in]       upstream_resource
   *                  The upstream resource.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  explicit SynchronizedPoolResource(MemoryResource* upstream_resource) noexcept;

  /*!
   * \brief           Constructs a new instance of the Synchronized SingleSizePool Resource using specified options
   * \param[in]       opts
   *                  A pool_options struct containing the constructor options.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  explicit SynchronizedPoolResource(PoolOptions const& opts) noexcept;

  /*!
   * \brief           Constructs a new instance of the Synchronized SingleSizePool Resource using specified options and
   *                  upstream resource
   * \param[in]       opts
   *                  A pool_options struct containing the constructor options.
   * \param[in]       upstream_resource
   *                  The upstream resource.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  SynchronizedPoolResource(PoolOptions const& opts, MemoryResource* upstream_resource) noexcept;

  SynchronizedPoolResource(SynchronizedPoolResource const&) = delete;

  /*!
   * \brief           Move constructor
   * \details         The upstream resource of \p other will be invalidated after the move.
   * \param[in]       other
   *                  The value to be moved.
   * \pre             -
   * \threadsafe      TRUE
   */
  SynchronizedPoolResource(SynchronizedPoolResource&& other) noexcept;

  auto operator=(SynchronizedPoolResource const&) -> SynchronizedPoolResource& = delete;

  /*!
   * \brief           Move-assignment operator
   * \details         The upstream resource of \p other will be invalidated after the move.
   * \param[in]       other
   *                  The value to be move assigned.
   * \return          *this, containing the contents of other.
   * \pre             -
   * \threadsafe      TRUE
   */
  auto operator=(SynchronizedPoolResource&& other) & noexcept -> SynchronizedPoolResource&;

  /*!
   * \brief           Destructs the instance of SynchronizedPoolResource
   * \details         Steady mode flag is set to false and memory is released to the upstream resource.
   * \context         ANY
   * \pre             All pools in the resource must be empty, i. e. not used.
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  virtual ~SynchronizedPoolResource() noexcept;

  /*!
   * \brief           Release memory to the upstream resource
   * \vprivate        PES Private
   * \context         ANY
   * \pre             synchronized pool resource must not be in steady mode and pool must be empty.
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  void release() noexcept;

  /*!
   * \brief           Returns a pointer to the upstream memory resource
   * \vprivate        PES Private
   * \return          Pointer to the upstream resource.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  auto upstream_resource() const noexcept -> MemoryResource*;

  /*!
   * \brief           Returns the options that controls the pooling behaviour
   * \vprivate        PES Private
   * \return          A copy of the pool options object.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  auto options() const noexcept -> PoolOptions;

 protected:
  /*!
   * \brief           Allocates storage with a size of at least
   * \details         If the size requested is larger than what the largest pool can handle, memory is allocated from
   *                  the upstream resource.
   * \param[in]       bytes
   *                  The number of bytes to allocate.
   * \param[in]       alignment
   *                  The alignment of the allocated storage. Must be a power of two.
   * \return          Pointer to the allocated storage if memory from the pool was available; nullptr otherwise.
   * \threadsafe      TRUE
   * \steady          TRUE
   * \p               bytes bytes from the pool.
   */
  auto DoAllocate(std::size_t bytes, std::size_t alignment) noexcept -> void* final;

  /*!
   * \brief           Deallocates storage by returning it to the pool
   * \details         If \p p is not pointing to any of the pools, nothing will be done.
   * \param[in]       p
   *                  Pointer to the storage to deallocate.
   * \param[in]       bytes
   *                  The number of bytes to deallocate.
   * \param[in]       alignment
   *                  The alignment of the deallocated storage.
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  void DoDeallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept final;

  /*!
   * \brief           Checks whether \p other equals \p this
   * \param[in]       other
   *                  The memory resource to compare.
   * \return          true, if the addresses of the memory resources are equal; false, otherwise.
   * \threadsafe      TRUE
   * \steady          TRUE
   */
  auto DoIsEqual(MemoryResource const& other) const noexcept -> bool final;

 private:
  /*! \brief          Default value for pool options */
  PoolOptions opts_{8U, 0U};

  /*! \brief          Pointer to the upstream memory resource */
  ara::core::MemoryResource* upstream_{GetDefaultResource()};

  /*! \brief          Memory pool of the resource */
  amsr::internal::SingleSizePool* pools_{nullptr};

  /*! \brief          Mutex to ensure thread-safe access to the pools and the upstream resource */
  std::mutex pool_mutex_{};
};

/*!
 * \exclusivearea   ara::core::SynchronizedPoolResource::pool_mutex_
 *                  Protects the pools and the upstream resource.
 * \protects        ara::core::SynchronizedPoolResource::upstream_ ara::core::SynchronizedPoolResource::pools_
 * \usedin          ara::core::SynchronizedPoolResource::operator= ara::core::SynchronizedPoolResource::release
 *                  ara::core::SynchronizedPoolResource::DoAllocate ara::core::SynchronizedPoolResource::DoDeallocate
 * \exclude         When the pool is not empty during calling release(), ara::core::Abort will be triggered. When the
 *                  upstream is a nullptr during calling DoAllocate(), ara::core::Abort will be triggered. When the
 *                  upstream is a nullptr during calling DoDeallocate(), ara::core::Abort will be triggered.
 * \length          LONG
 *                  This exclusive area may be blocked long time by "waiting" functionality
 * \endexclusivearea
 */

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_SYNCHRONIZED_POOL_RESOURCE_H_
