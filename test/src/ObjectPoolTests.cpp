#include <gtest/gtest.h>
#include <string.h> 
#include <string>
#include <ObjectPool/ObjectPool.hpp>
#include <iostream>

class ObjectPoolTests 
  : public ::testing::Test {
 protected:
  virtual void SetUp() override {}
  
  virtual void TearDown() override {}
 public:
  ObjectPoolTests()
    : object_pool(new sys::ObjectPool<TestStruct, 4>()) 
  {}
 protected:
  struct TestStruct {
    uint64_t val1;
    double dVal1;
    TestStruct() = default;
    TestStruct(uint64_t t_val1, double t_dVal1) {
      val1 = t_val1;
      dVal1 = t_dVal1;
    };
  };
  sys::ObjectPool<TestStruct, 4> *object_pool;
};

TEST_F(ObjectPoolTests, ShouldAllocateValidObject) {
  TestStruct* struct_ptr = object_pool->alloc();
  EXPECT_NE(struct_ptr, nullptr);
}

TEST_F(ObjectPoolTests, ShouldBeAbleToUseAllocatedObject) {
  TestStruct test_struct(10, 1.0);
  TestStruct* struct_ptr = object_pool->alloc();
  memcpy(struct_ptr, &(test_struct), sizeof(test_struct));
  struct_ptr->val1 = 10;
  struct_ptr->dVal1 = 1.0;
  EXPECT_STREQ((char*)(struct_ptr), (char*)(&test_struct));
}

TEST_F(ObjectPoolTests, ShouldReuseDeallocatedObject) {
  std::vector<TestStruct*> ptr_vec;
  int number_of_objects = 4;
  for(int i = 0; i < number_of_objects; i++) {
    ptr_vec.push_back(object_pool->alloc());
  }
  for(int i= 0; i < number_of_objects; i++) {
    object_pool->dealloc(ptr_vec[i]);
  }
  for(int i = number_of_objects -1; i >= 0; i--) {
    TestStruct *ptr = object_pool->alloc();
    EXPECT_EQ(ptr, ptr_vec[i]);
  }
}

TEST_F(ObjectPoolTests, ShouldAllocateMoreThanAlreadyAllocatedObjects) {
  std::vector<TestStruct*> ptr_vec;
  int number_of_objects = 5;
  for(int i = 0; i < number_of_objects; i++) {
    ptr_vec.push_back(object_pool->alloc());
    EXPECT_NE(ptr_vec[i], nullptr);
  }
  for(int i= 0; i < number_of_objects; i++) {
    object_pool->dealloc(ptr_vec[i]);
  }
  for(int i = number_of_objects -1; i >= 0; i--) {
    TestStruct *ptr = object_pool->alloc();
    EXPECT_EQ(ptr, ptr_vec[i]);
  }
}
