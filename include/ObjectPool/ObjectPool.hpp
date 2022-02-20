#pragma once

/**
 * @file ObjectsMemoryPool.hpp
 * 
 * This module declares the ObjectsMemoryPool::ObjectsMemoryPool class.
 * © 2020 by Sanjeev
 */

#include <memory>
#include <ObjectPool/BlockMemoryAllocator.hpp>
#include <ObjectPool/InlineNoInline.hpp>

namespace sys {
  /**
   * This class serves as a memory allocator 
   * support zero-alloc in the hotpath.
   */
  template<class ObjectType, size_t NumberOfObjects = 16>
  class ObjectPool {
   public:
    //Life cycle methods
    ~ObjectPool() = default;
    MAKE_NONCOPYABLE(ObjectPool);
    MAKE_NONMOVABLE(ObjectPool);
   public:
    /**
     * This is the constructor
     */
    ObjectPool()
      : _m_mem_block_size(NumberOfObjects * sizeof(ObjectType)),
        _m_mem_block_allocator(
          std::make_unique<sys::BlockMemoryAllocator>())				
    {
      static_assert(sizeof(ObjectType) > sizeof(ObjectType*));
      replenish();
    }
            
    /**
     * This function allocates and constructs the object of type 
     * ObjectType using the paramaters passed in the argument
     * 
     * @param[in]
     *    __args - Arguments to the constructor of ObjectType
     * @return 
     *      pointer to an allocated object
     */
    template<class... Args>
    FORCE_INLINE(ObjectType*) alloc(Args&&... __args) {
      if (_m_object_ptr == nullptr) {
          replenish();
      }
      ObjectType* retval = reinterpret_cast<ObjectType*>(_m_object_ptr);
      _m_object_ptr = _m_object_ptr->next;
      new(retval) ObjectType(std::forward<__args>...);
      return retval;
    }

    /**
     * This function deallocates an object pointed by the
     * object pointer, technically it reuses the object
     * 
     * @param[in] __ptr
     *      pointer to an object which needs to be deallocated
     * 
     */
    FORCE_INLINE(void) dealloc(ObjectType* __ptr) {
      reinterpret_cast<ObjectLink*>(__ptr)->next = _m_object_ptr;
      _m_object_ptr = reinterpret_cast<ObjectLink*>(__ptr);
    }

    /**
     * This function requests block memory allocator to allocate
     * block of memory and builds the list of objects
     */
    FORCE_NOINLINE(void) replenish() {
      char* start_addr = (char*)_m_mem_block_allocator->allocate(_m_mem_block_size);
      ObjectLink* curr_link = _m_object_ptr = reinterpret_cast<ObjectLink*>(start_addr);
      for(size_t sz = sizeof(ObjectType); sz < _m_mem_block_size; sz += sizeof(ObjectType)) {
        curr_link->next = reinterpret_cast<ObjectLink*>(start_addr + sz);
        curr_link = curr_link->next;
      }
      curr_link->next = nullptr;
    }

   private:
    /**
     * This structure is used to store the list of 
     * allocated objects pointer
     */
    struct ObjectLink {
      /**
       * This holds the reference to start address of 
       * next object memory location
       */
      ObjectLink *next;
    };
    
    /**
     * This is the size is requested from the block mem allocator
     */
    size_t _m_mem_block_size;

    /**
     * This holds the address of the memory location which can
     * be used to construct a new obect of type ObjectType
     */
    ObjectLink *_m_object_ptr;

    /**
     * This is the pointer to the sys::BlockMemoryAllocator
     */
    std::unique_ptr< BlockMemoryAllocator > _m_mem_block_allocator;
  };
}
