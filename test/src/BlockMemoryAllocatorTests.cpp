#include <gtest/gtest.h>
#include <string.h> 
#include <string>
#include <ObjectPool/BlockMemoryAllocator.hpp>



TEST(BlockMemoryAllocatorTests, AllocatesMemory) {
  sys::BlockMemoryAllocator blk_mem_allctr;
  char *block_memory = (char*)blk_mem_allctr.allocate(16);
  EXPECT_NE(block_memory, nullptr);
}

TEST(BlockMemoryAllocatorTests, AllocatesValidMemory) {
  sys::BlockMemoryAllocator blk_mem_allctr;
  char *block_memory = (char*)blk_mem_allctr.allocate(16);
  std::string test_string1_ = "abcdefghijklmnop";
  memcpy(block_memory, test_string1_.c_str(), 16);
}

TEST(BlockMemoryAllocatorTests, AllocateMultipleBlocks) {
  sys::BlockMemoryAllocator blk_mem_allctr;
  char *block_memory1 = (char*)blk_mem_allctr.allocate(16);
  char *block_memory2 = (char*)blk_mem_allctr.allocate(24);
  std::string test_string1_ = "abcdefghijklmnop";
  std::string test_string2_ = "abcdefghijklmnopqrstuvwx";
  EXPECT_NE(block_memory1, nullptr);
  EXPECT_NE(block_memory2, nullptr);
  memcpy(block_memory1, test_string1_.c_str(), 16);
  memcpy(block_memory2, test_string2_.c_str(), 24);
}
