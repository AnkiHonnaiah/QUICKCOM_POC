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
/*!        \file  amsr/pool_resource.h
 *        \brief  A memory resource with a pool of same sized memory blocks.
 *         \unit  VaCommonLib::MemoryManagement::PoolResource
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_POOL_RESOURCE_H_
#define LIB_VAC_INCLUDE_AMSR_POOL_RESOURCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include "amsr/core/result.h"
#include "amsr/vector.h"
#include "ara/core/memory_resource.h"

namespace amsr {

namespace internal {
/*!
 * \brief Machine Word size.
 */
using Word = std::uint64_t;

/*!
 * \brief Bitset for which the size is set at runtime.
 * \vprivate Component Private
 */
class Bitset final {
 public:
  /*!
   * \brief Constructs a new instance of the Bitset.
   * \param[in] p Pointer to the memory for the bitset.
   * \param[in] num_bits Number of bits in the bitset.
   * \context ANY
   * \pre The memory pointed to by \p p needs to be large enough for \p num_bits bits. \p num_bits must be larger than
   * zero.
   * \steady TRUE
   */
  Bitset(void* const p, std::size_t const num_bits) noexcept;

  /*!
   * \brief Default destructs the instance of Bitset.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  ~Bitset() noexcept = default;

  // Deleted copy constructor as it shall be avoided that two bitsets write to the same memory.
  Bitset(Bitset const&) = delete;

  /*!
   * \brief Move constructs a new instance of the Bitset class.
   * \param[in] other The bitset to move.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  Bitset(Bitset&& other) noexcept = default;

  // Deleted copy assignment as it shall be avoided that two bitsets write to the same memory.
  auto operator=(Bitset const&) -> Bitset& = delete;

  /*!
   * \brief Move assigns a bitset.
   * \param[in] other The bitset to move.
   * \return reference to the bitset.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto operator=(Bitset&& other) & noexcept -> Bitset& = default;

  /*!
   * \brief Returns the size of the bitset.
   * \return The number of bits in the bitset.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Size() const noexcept -> std::size_t;

  /*!
   * \brief Returns the number of free blocks (unset bits).
   * \return The number of unset bits.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Free() const noexcept -> std::size_t;

  /*!
   * \brief Checks whether the bitset is full, i. e., all bits are set.
   * \return True if there are no unset bits, false otherwise.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Full() const noexcept -> bool;

  /*!
   * \brief Checks whether the bitset is empty (no bits set).
   * \return True, if no bits are set, false otherwise.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Empty() const noexcept -> bool;

  /*!
   * \brief Resets the bitset.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  void Reset() noexcept;

  /*!
   * \brief Gets the bit at position \p n.
   * \param[in] n The index of the bit. Must be smaller than the number of bits in the bitset.
   * \return True, if the bit is set; false, otherwise.
   * \context ANY
   * \steady TRUE
   */
  auto operator[](std::size_t n) const noexcept -> bool;

  /*!
   * \brief Gets the first unset bit and flips it.
   * \details The caller has to make sure that the bitset is not yet full.
   * \return Position of the first unset bit if the bitset is not full; std::size_t(-1) otherwise.
   * \context ANY
   * \pre There is at least one unset bit in the bitset.
   * \steady TRUE
   */
  auto GetFirstUnset() noexcept -> std::size_t;

  /*!
   * \brief Set bit at position \p n.
   * \param[in] n The position of the bit to set.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  void Set(std::size_t n) noexcept;

  /*!
   * \brief Clears a single bit.
   * \param[in] n The position of the bit to clear.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  void Clear(std::size_t n) noexcept;

  /*!
   * \brief Returns the number of words used to store the bits.
   * \return The word count.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto WordCount() const noexcept -> std::size_t;

  /*!
   * \brief Gets the pointer to the data of the bitset.
   * \return The pointer to the data of the bitset.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Data() const noexcept -> void const*;

 private:
  /*!
   * \brief Updates the index of the next word with an unset bit.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  void UpdateNextWord() noexcept;

  /*!
   * \brief Get the number of set bits in \p word.
   * \param[in] word The word.
   * \return The number of set bits.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  static auto GetSetBitsInWord(Word word) noexcept -> std::size_t;

  /*!
   * \brief Get the first unset bit in a word starting from the least significant bit.
   * \param[in] word The word.
   * \return The position of the first unset bit.
   * \context ANY
   * \pre There is an unset bit in \p word.
   * \steady TRUE
   */
  static auto GetFirstUnsetBit(Word word) noexcept -> std::size_t;

  /*!
   * \brief Maximum number of bits in the bitset.
   * \details The value is arbitrary. Since continuous memory chunks for the pool with the same number of block and the
   * bitset will be needed, it should not be too large.
   */
  static constexpr std::size_t kMaxNumBits{524288U};

  /*!
   * \brief Pointer to the words.
   */
  Word* words_{nullptr};

  /*!
   * \brief Number of bits in the bitset.
   */
  std::size_t num_of_bits_;

  /*!
   * \brief Index of the first word with unset bits.
   */
  std::size_t next_word_{0U};
};

/*!
 * \brief A memory pool with fixed sized blocks for a PoolResource.
 * \vprivate
 */
class Pool {
 public:
  /*!
   * \brief Constructs a new instance of the Pool class.
   * \param[in] p Pointer to the memory.
   * \param[in] n Number of blocks in the pool.
   * \param[in] block_size Size of a single block in bytes.
   * \param[in] total_pool_size Size of all blocks and the bitset in bytes.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  Pool(void* p, std::size_t n, std::size_t block_size, std::size_t total_pool_size) noexcept;

  /*!
   * \brief Resets the pool.
   * \details The pool is empty then.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  void Reset() noexcept;

  /*!
   * \brief Get a free block from the pool.
   * \details If there are no free blocks left, a nullptr is returned.
   * \return Pointer to the block.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto GetFreeBlock() noexcept -> void*;

  /*!
   * \brief Checks whether the address is part of the pool.
   * \param[in] p The address to check.
   * \return true, if the address is in the pool; false, otherwise.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Owns(void const* const p) const noexcept -> bool;

  /*!
   * \brief Returns a block to the pool.
   * \param[in] p The pointer to the block.
   * \context ANY
   * \pre Pool owns \p p.
   * \steady TRUE
   */
  void ReturnBlock(void const* p) noexcept;

  /*!
   * \brief Sets the address of the pool.
   * \return p The pointer to the beginning of the pool.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstNonConstPointer
  auto GetAddress() noexcept -> void*;

  /*!
   * \brief Gets the number of blocks in the pool.
   * \return The block count.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto GetBlockCount() const noexcept -> std::size_t;

  /*!
   * \brief Gets the size of a single block in the pool.
   * \return The block size.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto GetBlockSize() const noexcept -> std::size_t;

  /*!
   * \brief Gets the total size of the pool including the bitset in bytes.
   * \return The total pool size.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto GetTotalPoolSize() const noexcept -> std::size_t;

  /*!
   * \brief Checks whether the pool is empty, i. e., no blocks are used.
   * \return True, if the pool is empty; false, otherwise.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto Empty() const noexcept -> bool;

 private:
  /*!
   * \brief Initialize the bitset for managing which blocks are used/free.
   * \param[in] p The address of the pool.
   * \param[in] n The number of blocks in the pool.
   * \param[in] block_size The block size.
   * \return The bitset.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  static auto InitializeBitset(void* p, std::size_t n, std::size_t block_size) noexcept -> Bitset;

  /*!
   * \brief Pointer to the beginning of the memory pool.
   */
  char* start_;

  /*!
   * \brief Number of bytes in a block.
   */
  std::size_t block_size_;

  /*!
   * \brief Total size of the pool including the bitset in bytes.
   */
  std::size_t total_pool_size_;

  /*!
   * \brief Information on which block in the pool is used.
   */
  Bitset used_blocks_;
};

}  // namespace internal

/*!
 * \brief Memory resource with one or multiple pools of fixed sized blocks.
 * \details An instance of PoolResource can have only pools with the same block size.
 * Pools are created by calling Reserve() with the number of blocks and the block size. If Reserve() is called
 * multiple times, the same block size must be used.
 * After reserving the pool(s), the steady mode flag has to be set by calling SetSteadyMode(true) if no further heap
 * operations shall occur. Otherwise, allocating memory might lead to creation of new pools if the available pools are
 * all fully used.
 * \trace CREQ-VaCommonLib-PoolResource
 * \vpublic
 */
class PoolResource final : public ara::core::MemoryResource {
 public:
  /*!
   * \brief Constructs a new instance of the Pool Resource class.
   * \details The upstream resource is set to ara::core::GetDefaultResource().
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  PoolResource() noexcept;

  /*!
   * \brief Constructs a new instance of the Pool Resource class.
   * \param[in] upstream_resource The upstream resource.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  explicit PoolResource(ara::core::MemoryResource* upstream_resource) noexcept;

  PoolResource(PoolResource const&) = delete;

  /*!
   * \brief     Move constructor.
   * \details   The upstream resource of \p other will be invalidated after the move.
   * \param[in] other The value to be moved.
   * \pre       -
   */
  PoolResource(PoolResource&& other) noexcept;

  /*!
   * \brief Destructs the instance of PoolResource.
   * \details Steady mode flag is set to false and memory is released to the upstream resource.
   * \context ANY
   * \pre All pools in the resource must be empty, i. e. not used.
   * \steady TRUE
   */
  ~PoolResource() noexcept final;

  auto operator=(PoolResource const&) -> PoolResource = delete;

  /*!
   * \brief     Move-assignment operator.
   * \details   The upstream resource of \p other will be invalidated after the move.
   * \param[in] other The value to be move assigned.
   * \return    *this, containing the contents of other.
   * \pre       -
   */
  auto operator=(PoolResource&& other) & noexcept -> PoolResource&;

  /*!
   * \brief Reserves memory for \p n blocks.
   * \details When calling Reserve() multiple times, the same block size shall be used. If a smaller block size is used,
   * the pool resource will still allocate blocks of the previous larger size. If a larger block size is used, the
   * application will be aborted.
   *
   * The total memory for the pool is larger than \p n multiplied with \p block_size. Padding might be added to
   * each block to achieve the required alignment. After the blocks, additional memory for \p n bits is added to keep
   * track of used and unused blocks. Padding might be added between the last block in the pool and the bitset for
   * alignment reasons.
   * \param[in] n The number of blocks to reserve. Range: [1, 524.288]
   * \param[in] block_size The size of a single block in bytes.
   * \param[in] alignment The alignment of the blocks in bytes. Must be a power of two. Defaults to alignment of void*,
   *            i.e., the natural machine alignment.
   * \return Result containing nothing on success; an error code otherwise.
   * \error amsr::generic::GenErrc::kRuntimeResourceOutOf if allocation from the upstream resource failed.
   * \error amsr::ContainerErrorCode::kAllocationError if allocation of memory for internally used vector was not
   *        successful.
   * \context ANY
   * \pre Pool resource must not be in steady mode. To avoid wrap around of the unsigned integers during calculations,
   *      the total memory size needs to fit in an integer of type std::size_t.
   *      Both, \p n and \p block_size must be larger than zero.
   * \steady TRUE
   */
  auto Reserve(std::size_t n, std::size_t block_size, std::size_t alignment = alignof(void*)) noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Release memory to the upstream resource.
   * \context ANY
   * \pre Pool resource must not be in steady mode and pool must be empty.
   * \steady TRUE
   */
  void Release() noexcept;

  /*!
   * \brief Activates or deactivates steady mode.
   * \param[in] steady The steady mode state.
   * \details If \p steady is true, no more memory can be allocated from or released to the upstream resource.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  void SetSteadyMode(bool steady) noexcept;

  /*!
   * \brief Checks whether the pool resource is in steady mode.
   * \return true, if resource is in steady mode; false, otherwise.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto IsInSteadyMode() const noexcept -> bool;

  /*!
   * \brief Returns a pointer to the upstream memory resource.
   * \return Pointer to the upstream resource.
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  auto UpstreamResource() const noexcept -> ara::core::MemoryResource*;

 protected:
  /*!
   * \brief Allocates storage with a size of at least \p bytes bytes from the pool.
   * \details Allocation will fail if \p bytes is larger than the block size of the memory pool. It will also fail if
   * all blocks are used and the resource is in steady mode.
   * \param[in] bytes The number of bytes to allocate.
   * \param[in] alignment The alignment of the allocated storage. Must be a power of two.
   * \return Pointer to the allocated storage if memory from the pool was available; nullptr otherwise.
   * \steady TRUE
   */
  auto DoAllocate(std::size_t bytes, std::size_t alignment) noexcept -> void* final;

  /*!
   * \brief Deallocates storage by returning it to the pool.
   * \details If \p p is not pointing to any of the pools, nothing will be done.
   * \param[in] p Pointer to the storage to deallocate.
   * \steady TRUE
   */
  void DoDeallocate(void* p, std::size_t, std::size_t) noexcept final;

  /*!
   * \brief Checks whether \p other equals \p this.
   * \param[in] other The memory resource to compare.
   * \return true, if the addresses of the memory resources are equal; false, otherwise.
   * \steady TRUE
   */
  auto DoIsEqual(ara::core::MemoryResource const& other) const noexcept -> bool final;

 private:
  /*!
   * \brief Pointer to the upstream memory resource.
   */
  ara::core::MemoryResource* upstream_resource_;

  /*!
   * \brief Memory pool of the resource.
   */
  amsr::Vector<internal::Pool, ara::core::PolymorphicAllocator<internal::Pool>> pools_;

  /*!
   * \brief The number of blocks to create the next pool with.
   */
  std::size_t next_number_of_blocks_{2U};

  /*!
   * \brief The block size in the pools.
   */
  std::size_t block_size_{0U};

  /*!
   * \brief The alignment for de/allocation of the upstream resource.
   */
  std::size_t alignment_{0U};

  /*!
   * \brief Indicates whether the steady mode is active.
   * \details If set to true, memory cannot be allocated from the upstream resource.
   */
  bool steady_mode_active_{false};
};

}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_POOL_RESOURCE_H_
