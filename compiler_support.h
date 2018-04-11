#ifdef _MSC_VER
// #pragma message ("MSVC")
#define VECTORIZE_FUNCTION
#define ASSUME_ALIGNED(ptr, align) (__assume((uintptr_t)ptr % align == 0))
#elif defined(__INTEL_COMPILER)
// #warning ICC
#define ASSUME_ALIGNED(ptr, align) __assume_aligned(ptr, align)
#define VECTORIZE_FUNCTION
#elif defined(__clang__)
// #warning clang
// Clang doesn't seem to support target_clones attribute (yet?)
#define VECTORIZE_FUNCTION

// Clang verison >= 3.6
#if __clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 6)
#define ASSUME_ALIGNED(ptr, align)                                             \
  (ptr = (decltype(ptr))__builtin_assume_aligned((void *)ptr, align))
#else
// #warning clang too old for assume_aligned
#define ASSUME_ALIGNED(ptr, align)
#endif

#elif defined(__GNUC__)
// #warning GCC
// GCC version >= 4.5
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define ASSUME_ALIGNED(ptr, align)                                             \
  (ptr = (decltype(ptr))__builtin_assume_aligned((void *)ptr, align))
#else
// #warning gcc too old for assume_aligned
#define ASSUME_ALIGNED(ptr, align)
#endif
// GCC version >= 6.0

#if (defined(__x86_64) || defined(__i386)) && __GNUC__ >= 6
// #warning using default x86/x86_64 vectorize list
#define VECTORIZE_FUNCTION                                                     \
  __attribute__((target_clones("avx2", "arch=atom", "default")))
#else
// #warning disabling function vectorization
#define VECTORIZE_FUNCTION
#endif
#else
// #warning no compiler detected
#define ASSUME_ALIGNED(ptr, align)
#define VECTORIZE_FUNCTION
#endif

#if defined(__GNUC__)
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#else
#warning No likely/unlikely
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#if defined(__GNUC__)
#define UNREACHABLE() __builtin_unreachable()
#else
#warning no unreachable
#define UNREACHABLE()
#endif
