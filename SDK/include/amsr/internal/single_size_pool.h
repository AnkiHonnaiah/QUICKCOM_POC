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
/*!        \file  amsr/internal/single_size_pool.h
 *        \brief  A memory pool containing multiple blocks of the same block size.
 *         \unit  VaCommonLib::SingleSizePool
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_INTERNAL_SINGLE_SIZE_POOL_H_
#define LIB_VAC_INCLUDE_AMSR_INTERNAL_SINGLE_SIZE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"

namespace amsr {
namespace internal {

/*!
 * \brief           A chunk of memory
 * \details         A chunk contains memory for multiple blocks in a pool.
 */
class Chunk {
 public:
  /*!
   * \brief           Constructs a new chunk
   * \param[in]       size
   *                  Size in bytes.
   * \param[in]       align
   *                  The alignment of the chunk.
   * \param[in]       next
   *                  Pointer to the next chunk.
   * \param[in]       start
   *                  Pointer to the start of the chunk.
   */
  Chunk(std::size_t size, std::size_t align, Chunk* next, void* start)
      : size_(size),        // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        alignment_(align),  // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        next_(next),        // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        start_(start) {}    // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK, VCA_VAC_VALID_CONSTRUCTOR_CALLED

  /*!
   * \brief           Get the size of the chunk
   * \return          The size of the chunk.
   */
  auto GetSize() const noexcept -> std::size_t;

  /*!
   * \brief           Get the alignment of the chunk
   * \return          The alignment of the chunk.
   */
  auto GetAlignment() const noexcept -> std::size_t;

  /*!
   * \brief           Get the next chunk in the linked list
   * \return          The next chunk in the linked list.
   */
  auto GetNext() const noexcept -> Chunk*;

  /*!
   * \brief           Get the start address of the chunk
   * \return          The start address of the chunk.
   */
  void* GetStart() const noexcept;

 private:
  /*! \brief          The size of the chunk */
  std::size_t size_;

  /*! \brief          The alignment of the chunk */
  std::size_t alignment_;

  /*! \brief          The next chunk in the linked list */
  Chunk* next_{nullptr};

  /*! \brief          The start address of the chunk */
  void* start_;
};

/*! \brief          A single memory block in an amsr::internal::SingleSizePool */
class Block {
 public:
  /*!
   * \brief           Constructs a new instance of the Block class
   * \param[in]       next
   *                  Pointer to the next block.
   */
  explicit Block(Block* next) : next_(next) {}  // VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK, VCA_VAC_VALID_CONSTRUCTOR_CALLED

  /*! \brief          Get the next block in the linked list */
  auto GetNext() const noexcept -> Block*;

  /*!
   * \brief           Set the next block in the linked list
   * \param[in]       next
   *                  The next block in the linked list.
   */
  void SetNext(Block* next) noexcept;

 private:
  /*! \brief          The next block in the linked list */
  Block* next_;
};

/*!
 * \brief           A memory pool containing multiple blocks of the same block size
 * \details         The memory is allocated from an upstream resource in chunks with enough memory for several blocks.
 *                  Blocks are stored in linked lists. Depending on the list they are stored in, they are currently
 *                  available or used. If all blocks are used, the pool is replenished with another chunk.
 * \trace           CREQ-VaCommonLib-SynchronizedPoolResource
 */
class SingleSizePool final {
  /*! \brief          declaration for testing purposes */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  FRIEND_TEST(UT__SingleSizePool, CreateChunkAbort);

  /*! \brief          declaration for testing purposes */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  FRIEND_TEST(UT__SingleSizePool, CreateChunkFailed);

  /*! \brief          declaration for testing purposes */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  FRIEND_TEST(UT__SingleSizePool, CreateChunkSuccess);

 public:
  // Deleted default constructor
  SingleSizePool() = delete;

  /*!
   * \brief           Constructs a new instance of the SingleSizePool class
   * \param[in]       upstream_resource
   *                  The upstream memory resource.
   * \param[in]       block_size
   *                  Size of a single block in bytes.
   * \param[in]       alignment
   *                  Alignment of the blocks.
   * \param[in]       blocks_per_chunk
   *                  Maximum number of blocks per chunk.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  SingleSizePool(ara::core::MemoryResource* upstream_resource, std::size_t block_size, std::size_t alignment,
                 std::size_t blocks_per_chunk) noexcept;

  /*! \brief          Move constructor */
  SingleSizePool(SingleSizePool&& other) noexcept;

  /*! \brief          Move assignment */
  auto operator=(SingleSizePool&& other) & noexcept -> SingleSizePool&;

  // Deleted copy constructor
  SingleSizePool(SingleSizePool const& other) = delete;
  // Deleted copy assignment
  auto operator=(SingleSizePool const& other) -> SingleSizePool& = delete;

  /*! \brief          Destructs the SingleSizePool class */
  ~SingleSizePool() noexcept;

  /*!
   * \brief           Releases all memory chunks to the upstream resource
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  void Release() noexcept;

  /*!
   * \brief           Get a free block from the pool
   * \details         If there are no free blocks left, another chunk of memory is requested from the upstream resource.
   *                  If no memory can be obtained from upstream, an error is returned.
   * \return          Pointer to the block on success, an error code otherwise.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf if allocation from the upstream resource failed.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto GetFreeBlock() noexcept -> ara::core::Result<void*>;

  /*!
   * \brief           Checks whether the pool owns the \p block
   * \param[in]       block
   *                  The block to check.
   * \return          true, if the block is used; false, otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto Owns(void const* const block) const noexcept -> bool;

  /*!
   * \brief           Checks whether \p block is part of the used list
   * \param[in]       block
   *                  The block to check.
   * \return          true, if the block is used; false, otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto IsUsed(void const* const block) const noexcept -> bool;

  /*!
   * \brief           Returns a block to the single size pool
   * \param[in]       block
   *                  The block to return.
   * \context         ANY
   * \pre             SingleSizePool owns \p block.
   * \steady          TRUE
   */
  void ReturnBlock(void const* block) noexcept;

  /*!
   * \brief           Gets the size of a block from the single size pool
   * \return          The size of a block from the single size pool.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto GetBlockSize() const noexcept -> std::size_t;

  /*!
   * \brief           Gets the alignment of the blocks
   * \return          The alignment of the blocks
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto GetAlignment() const noexcept -> std::size_t;

  /*!
   * \brief           Checks whether the pool is unused, i. e., no blocks are used
   * \return          True, if the pool is unused; false, otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto Unused() const noexcept -> bool;

  /*!
   * \brief           Get the next pool
   * \return          The next pool.
   */
  auto GetNext() const noexcept -> SingleSizePool*;

  /*!
   * \brief           Set the next pool
   * \param[in]       next
   *                  The next pool.
   */
  void SetNext(SingleSizePool* next) noexcept;

 private:
  /*!
   * \brief           Create a Chunk of blocks
   * \param[in]       blocks
   *                  Number of blocks.
   * \return          Return void if successful; otherwise an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceNotAvailable if the upstream resource is a nullptr.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf if allocation from the upstream resource failed.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto CreateChunk(std::size_t blocks) noexcept -> ara::core::Result<void>;

  /*!
   * \brief           Gets the start address of a block
   * \param[in]       block
   *                  The pointer to the block.
   * \return          The start address of the block.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto GetStartOfBlock(void* block) const noexcept -> void*;

  /*!
   * \brief           Checks whether the address of \p block is contained in the linked list \p block_list
   * \param[in]       block
   *                  The address to check.
   * \param[in]       block_list
   *                  The linked list to search in.
   * \return          true, if the address is in the given list.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  auto IsContainedBy(void const* const block, Block* block_list) const noexcept -> bool;

  /*! \brief          Number of bytes in a block */
  std::size_t block_size_;

  /*! \brief          Alignment of the blocks */
  std::size_t alignment_;

  /*! \brief          Upstream memory resource */
  ara::core::MemoryResource* upstream_resource_{nullptr};

  /*! \brief          Number of blocks per chunk */
  std::size_t blocks_per_chunk_{};

  /*! \brief          Pointer to the beginning of the list of chunks */
  Chunk* chunks_{nullptr};

  /*! \brief          Pointer to the beginning of the list of available blocks */
  Block* available_blocks_{nullptr};

  /*! \brief          Pointer to the beginning of the list of used blocks */
  Block* used_blocks_{nullptr};

  /*! \brief          Pointer to the next pool in the list */
  SingleSizePool* next_{nullptr};

  /*! \brief          Aligned block size without size of the control block */
  std::size_t padded_extended_block_size_without_control_block_{0};
};

}  // namespace internal
}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_INTERNAL_SINGLE_SIZE_POOL_H_
