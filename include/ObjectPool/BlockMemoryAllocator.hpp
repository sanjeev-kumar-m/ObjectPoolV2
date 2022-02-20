#pragma once

/**
 * @file BlockMemoryAllocator.hpp
 * @author Sanjeev Kumar M
 * 
 * This module defines sys::BlockMemoryAllocator class
 */

#include <cstddef>
#include <stdlib.h>
#include <ObjectPool/NonCopyableMovable.hpp>
#include <ObjectPool/InlineNoInline.hpp>


namespace sys {

  class BlockMemoryAllocator {
   public:
    //Life cycle methods
    ~BlockMemoryAllocator() noexcept {
      MemoryBlockLink* prev_block = nullptr;
      while(_m_tail_block != nullptr) {
        prev_block = reinterpret_cast<MemoryBlockLink*>(
              _m_tail_block->prev_block);
        free(_m_tail_block);
        _m_tail_block = prev_block;
      }
    }
    MAKE_NONCOPYABLE(BlockMemoryAllocator);
    MAKE_NONMOVABLE(BlockMemoryAllocator); 
   public:
    /**
     * This is the default constructor
     */
    BlockMemoryAllocator() = default;
    
    /**
     * This function allocates block of memory of size 
     * passed in the parameter
     * 
     * @param[in] number_of_bytes
     *    This is number of bytes to allocated
     *
     * @return 
     *    Starting address of the memory block allocated
     */
    FORCE_NOINLINE(void*) allocate(size_t __size) {
      char* t_new_block = reinterpret_cast<char*>(malloc(__size + sizeof(MemoryBlockLink*)));
      reinterpret_cast<MemoryBlockLink*>(t_new_block)->prev_block
         = reinterpret_cast<MemoryBlockLink*>(_m_tail_block);
      _m_tail_block = reinterpret_cast<MemoryBlockLink*>(t_new_block);
      return reinterpret_cast<void*>(t_new_block + sizeof(MemoryBlockLink*));
    }

   private:
    /**
     * This structure used to maintain list of allocated blocks of 
     * memory, used only when freeing the allocated blocks
     */
    struct MemoryBlockLink {
      /**
       * This stores the address of the prev block
       */
      MemoryBlockLink* prev_block;
    };

    /**
     * This stores the ptr to the recently allocated block
     */
    MemoryBlockLink *_m_tail_block{nullptr};
  };
} // namespace sys
