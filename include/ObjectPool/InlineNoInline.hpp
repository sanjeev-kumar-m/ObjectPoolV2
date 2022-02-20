#pragma once 

#define FORCE_INLINE(...) __VA_ARGS__ __attribute__((always_inline))
#define FORCE_NOINLINE(...) __VA_ARGS__ __attribute__((noinline))
