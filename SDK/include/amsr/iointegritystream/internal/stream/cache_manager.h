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
 *        \brief  Declaration of the CacheManager class.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_CACHE_MANAGER_H_
#define LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_CACHE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <list>
#include <unordered_map>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/vector.h"
#include "vac/language/byte.h"
namespace amsr {
namespace iointegritystream {
namespace internal {
namespace stream {

/*!
 * \brief           Class that manages the cache memory when reading/writing to an integrity file
 *
 * \details         Caches pages of data using the LRU (Least Recently Used) algorithm to a block identifier (block ID).
 *                  The user provides the cache buffer and decides how large each block shall be (the pages in the cache
 *                  will have the same size as the block size).
 *
 *                  E.g., if a buffer of size 4096 is provided, it's possible to divide the cache into several blocks
 *                  and sizes: 1 block of 4096 bytes, 2 blocks of 2048 bytes, 4 blocks of 1024 bytes, etc.
 *
 *                  The CacheManager will try to make as many blocks as possible that can fit into the given buffer.
 */
class CacheManager final {
 public:
  /*!
   * \brief           Constant block ID used for unoccupied cache pages
   */
  static constexpr size_t kNoBlockId{std::numeric_limits<size_t>::max()};

  /*!
   * \brief           Information about a cached block
   */
  struct BlockInfo {
    /*!
     * \brief           The page index of the cached block
     */
    std::size_t page_index{0};
    /*!
     * \brief           The memory span where the block data is stored
     */
    amsr::core::Span<vac::language::byte> span{};
    /*!
     * \brief           The amount of valid data in the block
     * \detail          Initialized to 0 when block is loaded, but can be modified
     *                  by IntegrityEngine after reading file data into the page.
     */
    std::size_t used_size{0};
    /*!
     * \brief           Flag indicating if page has been modified and needs flushing to disk
     * \detail          Initialized to false for new loaded blocks, but can be set
     *                  directly by IntegrityEngine.
     */
    bool dirty{false};
    /*!
     * \brief           Flag indicating if the page was already in the cache before the call. or not
     * \detail          Initialized to false for newly loaded blocks, but can be set
     *                  by IntegrityEngine when data is written to the page.
     */
    bool just_loaded{true};
    /*!
     * \brief           ID of evicted block
     * \detail          Set to the the ID of the block using the page before,
     *                  allowing IntegrityEngine to flush modified data. The span member should still
     *                  be valid fror the evicted block, and evicted_used_size and evicted_dirty members
     *                  are set to the previous values of used_size_ and dirty. If page was previously
     *                  unused, this member is set to kNoBlockId.
     */
    std::size_t evicted_block_id{kNoBlockId};
    /*!
     * \brief           The amount of data in the block being evicted
     * \detail          Will be set to 0 if page was previously unused.
     */
    std::size_t evicted_used_size{0};
    /*!
     * \brief           Indicates if the block being evicted was dirty
     * \detail          Will be set to false if page was previously unused.
     */
    bool evicted_dirty{false};
  };

  /*!
   * \brief           Destructor for CacheManager
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  ~CacheManager() noexcept = default;  // VCA_IOIS_FUNC_CONTRACT

  /*!
   * \brief           Move constructor for CacheManager
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  CacheManager(CacheManager&&) noexcept = default;  // VCA_IOIS_FUNC_CONTRACT

  auto operator=(CacheManager&&) -> CacheManager& = delete;

  // CacheManager shall not be copied
  CacheManager(CacheManager const&) = delete;
  auto operator=(CacheManager const&) -> CacheManager& = delete;

  /*!
   * \brief           Creates an instance of CacheManager
   * \details         Aborts if the block size is invalid.
   * \param[in]       cache_buffer
   *                  Buffer to hold cached data. User is responsible for keeping the buffer alive.
   * \param[in]       block_size
   *                  The size of one data block. (A page in the cache will be the same size.)
   * \return          Instance of a CacheManager.
   * \context         ANY
   * \pre             The input cache buffer size must be equal or greater than the block size.
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  static auto Create(amsr::core::Span<vac::language::byte> cache_buffer, size_t block_size) noexcept -> CacheManager;

  /*!
   * \brief           Loads a page from the cache
   * \details         Always loads a page from the cache even if the block ID is found in the cache or not. If it is
   *                  found, the corresponding page will be loaded. If not, a page will be linked to the new block ID
   *                  and loaded. The least recently used (LRU) algorithm is used to replace pages in the cache if
   *                  needed.
   * \param[in]       block_id
   *                  ID of block of data to be stored in the cache.
   * \return          BlockInfo of the data in the page. Also includes information about block previously using the
   *                  page; caller is responsible for flushing evicted block to disk if needed.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \spec
   * requires true;
   * \endspec
   */
  auto LoadPageFromCache(std::size_t block_id) noexcept -> BlockInfo const;

  /*!
   * \brief           Remove a page from the cache
   * \param[in]       block_id
   *                  ID of block of data to be removed.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void RemovePageFromCache(size_t block_id) noexcept;

  /*!
   * \brief           Flushes dirty pages in the cache
   * \detail          The flushed blocks will remain in the cache, but the dirty flag is cleared.
   * \param[in]       func
   *                  Callback function to actually write a dirty page to disk and updata CRC.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  template <typename Func>
  auto Flush(Func&& func) noexcept -> amsr::core::Result<void> {
    amsr::core::Result<void> result;
    amsr::core::Result<void> tmp_result;
    for (auto& page : cache_pages_) {
      if ((page.block_id != kNoBlockId) && page.dirty) {
        tmp_result = func(page.block_id, page.span, page.used_size);  // VCA_IOIS_FUNC_CONTRACT
        if (tmp_result) {
          page.dirty = false;  // VCA_IOIS_ACCESS_OUT_OF_BOUNDS
        }
        result = result.And(tmp_result);
      }
    }
    return result;
  }

  /*!
   * \brief           Calculates remaining buffer space in cache.
   * \detail          Calculates the remaining space assuming the caller will continue to read or write,
   *                  without seeking. Returns ramaining space in current page plus size of all free pages.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  auto GetRemainingBufferSize(std::uint64_t current_stream_pos) const noexcept -> amsr::core::Span<char>::index_type;

  /*!
   * \brief           Updates the used size of the specified page.
   * \detail          IntegrityEngine calls this to update size after write to or loading data from file into the page.
   * \param[in]       page_index Index of the page to update.
   * \param[in]       used_size
   *                  The new used size for the page.
   * \context         ANY
   * \pre             page_index should always be taken from BlockInfo.page_index returned from a previous call to
   *                  LoadPageFromCache().
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetPageUsedSize(std::size_t page_index, std::size_t used_size);

  /*!
   * \brief           Sets the dirty bit of the specified page.
   * \detail          IntegrityEngine calls this after writing to the page.
   * \param[in]       page_index Index of the page to update.
   * \context         ANY
   * \pre             page_index should always be taken from BlockInfo.page_index returned from a previous call to
   *                  LoadPageFromCache().
   * \threadsafe      FALSE
   * \reentrant       FALSE
   */
  void SetPageDirty(std::size_t page_index);

 private:
  /*!
   * \brief           Constructor
   * \param[in]       cache_buffer
   *                  Buffer to hold cached data. User is responsible for keeping buffer alive.
   * \param[in]       block_size
   *                  Maximum amount of data (in bytes), depending on CRC32/CRC64.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \spec
   * requires true;
   * \endspec
   */
  CacheManager(amsr::core::Span<vac::language::byte> cache_buffer, std::size_t block_size) noexcept;

  /*!
   * \brief           The cache buffer
   */
  amsr::core::Span<vac::language::byte> cache_buffer_;

  /*!
   * \brief           Size of a single block of data (and therefore the size of a page in the cache)
   */
  std::size_t const block_size_;

  /*!
   * \brief           Number of pages in the cache
   */
  std::size_t const number_of_cache_pages_;

  /*!
   * \brief           Data held for each page in the cache
   */
  struct CachePage {
    /*!
     * \brief           The block currently loaded in the page, kNoBlockId if page is unused
     */
    size_t block_id{kNoBlockId};
    /*!
     * \brief           The memory span where the block data is stored, subspan of cache_buffer_
     */
    amsr::core::Span<vac::language::byte> span{};
    /*!
     * \brief           The amount of valid data in the page
     * \detail          Initialized to 0 when block is loaded, but can be modified
     *                  by IntegrityEngine after reading file data into the page.
     */
    size_t used_size{0};
    /*!
     * \brief           Indicates if the page has been modified and needs flushing to disk
     * \detail          Initialized to false for newly loaded blocks, but can be set
     *                  by IntegrityEngine when data is written to the page.
     */
    bool dirty{false};
    /*!
     * \brief           Iterator to corresponding entry in LRU list
     */
    std::list<std::size_t>::iterator lru_pos{};
  };

  /*!
   * \brief           Array of pages in the cache
   */
  ara::core::Vector<CachePage> cache_pages_;

  /*!
   * \brief           List to keep track of LRU (Least Recently Used) blocks. The first element is the most recently
   *                  used, and the last element is the least recently used. The cached pages holds iterators to the
   *                  elements so they can be easily reordered without any new memory allocations
   */
  std::list<std::size_t> lru_list_;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoIntegrityStream_A11.3.1_friend_class
  /*!
   * \brief           Friend test
   */
  friend auto IsBlockInCache(CacheManager const& cache_manager, std::size_t block_id) -> bool;

  /*!
   * \brief           Friend test
   */
  FRIEND_TEST(UT__CacheManager_P, LoadBlock__LoadAllPages);

  /*!
   * \brief           Friend test
   */
  FRIEND_TEST(UT__CacheManager_P, LoadBlock__BlocksFound);

  /*!
   * \brief           Friend test
   */
  FRIEND_TEST(UT__CacheManager_P, LoadBlock__CacheFull);
};

}  // namespace stream
}  // namespace internal
}  // namespace iointegritystream
}  // namespace amsr

#endif  // LIB_IOINTEGRITYSTREAM_INCLUDE_AMSR_IOINTEGRITYSTREAM_INTERNAL_STREAM_CACHE_MANAGER_H_
