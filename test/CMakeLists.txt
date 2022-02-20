set(This ObjectPoolTests)

set(Sources
    src/BlockMemoryAllocatorTests.cpp
    src/ObjectPoolTests.cpp)

add_executable(${This} ${Sources})

target_link_libraries(${This}
  gtest_main
  ObjectPool
)

add_test(
  NAME ${This}
  COMMAND ${This}
)