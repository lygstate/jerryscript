#include <assert.h>
#include <stdint.h>

#define JERRY_STATIC_ASSERT_GLUE_(a, b, c) a##b##_##c
#define JERRY_STATIC_ASSERT_GLUE(a, b, c)  JERRY_STATIC_ASSERT_GLUE_ (a, b, c)
#define JERRY_STATIC_ASSERT(x, msg)                                                  \
  enum                                                                               \
  {                                                                                  \
    JERRY_STATIC_ASSERT_GLUE (static_assertion_failed_, __LINE__, msg) = 1 / (!!(x)) \
  }

#if defined(JERRY_CPOINTER_64_BIT) && defined(JERRY_CPOINTER_32_BIT)
#error "Do not define JERRY_CPOINTER_64_BIT JERRY_CPOINTER_32_BIT at the same time"
#endif /* defined(JERRY_CPOINTER_64_BIT) && defined(JERRY_CPOINTER_32_BIT) */

#if defined(JERRY_CPOINTER_64_BIT) && defined(JERRY_CPOINTER_16_BIT)
#error "Do not define JERRY_CPOINTER_64_BIT JERRY_CPOINTER_16_BIT at the same time"
#endif /* defined(JERRY_CPOINTER_64_BIT) && defined(JERRY_CPOINTER_16_BIT) */

#if defined(JERRY_CPOINTER_32_BIT) && defined(JERRY_CPOINTER_16_BIT)
#error "Do not define JERRY_CPOINTER_32_BIT JERRY_CPOINTER_16_BIT at the same time"
#endif /* defined(JERRY_CPOINTER_32_BIT) && defined(JERRY_CPOINTER_16_BIT) */

#if 0
#define JERRY_CPOINTER_64_BIT 1
#define JERRY_CPOINTER_32_BIT 1
#define JERRY_CPOINTER_16_BIT 1
#endif
#if !defined(JERRY_CPOINTER_64_BIT) && !defined(JERRY_CPOINTER_32_BIT) && !defined(JERRY_CPOINTER_16_BIT)
#define JERRY_CPOINTER_16_BIT 1
#endif /* !defined(JERRY_CPOINTER_64_BIT) && !defined(JERRY_CPOINTER_32_BIT) && !defined(JERRY_CPOINTER_16_BIT) */

#ifndef JERRY_CPOINTER_64_BIT
#define JERRY_CPOINTER_64_BIT 0
#endif /* JERRY_CPOINTER_64_BIT */

#ifndef JERRY_CPOINTER_32_BIT
#define JERRY_CPOINTER_32_BIT 0
#endif /* JERRY_CPOINTER_32_BIT */

#ifndef JERRY_CPOINTER_16_BIT
#define JERRY_CPOINTER_16_BIT 0
#endif /* JERRY_CPOINTER_16_BIT */

#if JERRY_CPOINTER_64_BIT && UINTPTR_MAX <= UINT32_MAX
#warning "Do not support JERRY_CPOINTER_64_BIT on 32bit system"
#endif /* JERRY_CPOINTER_64_BIT && UINTPTR_MAX <= UINT32_MAX */

#if (JERRY_CPOINTER_64_BIT + JERRY_CPOINTER_32_BIT + JERRY_CPOINTER_16_BIT) == 0
#error "One of JERRY_CPOINTER_64_BIT, JERRY_CPOINTER_32_BIT, JERRY_CPOINTER_16_BIT should be defined"
#endif /* (JERRY_CPOINTER_64_BIT + JERRY_CPOINTER_32_BIT + JERRY_CPOINTER_16_BIT) == 0 */

#if JERRY_CPOINTER_64_BIT
typedef uint64_t jerry_uintptr_t;
#define JERRY_UINTPTR_MAX UINT64_MAX
#elif JERRY_CPOINTER_32_BIT
typedef uint32_t jerry_uintptr_t;
#define JERRY_UINTPTR_MAX UINT32_MAX
#elif JERRY_CPOINTER_16_BIT
typedef uint16_t jerry_uintptr_t;
#define JERRY_UINTPTR_MAX UINT16_MAX
#endif /* JERRY_CPOINTER_64_BIT */

#if JERRY_CPOINTER_16_BIT
typedef uint16_t lit_string_hash_t;
typedef uint16_t ecma_tag_t;
#else /* !JERRY_CPOINTER_16_BIT */
typedef uint32_t lit_string_hash_t;
typedef uint32_t ecma_tag_t;
#endif /* JERRY_CPOINTER_16_BIT */

typedef enum
{
  ECMA_STRING_SIZE_TYPE_EXTERNAL = 0,
  ECMA_STRING_SIZE_TYPE_COMPACT = 0x9,
} ecma_string_size_type_t;

typedef struct
{
#if JERRY_CPOINTER_16_BIT
  ecma_tag_t refs : 11;
#else
  ecma_tag_t refs : 27;
#endif
  ecma_tag_t utf16 : 1; /**< 0: ASCII string, 1: UTF16 string */

  /**
   * ecma_compact_string_t:
   *  ECMA_STRING_SIZE_TYPE_COMPACT <= size_type
   * ecma_internal_string_t:
   *  ECMA_STRING_SIZE_TYPE_EXTERNAL < size_type < ECMA_STRING_SIZE_TYPE_COMPACT
   * ecma_external_string_t:
   *  size_type == ECMA_STRING_SIZE_TYPE_EXTERNAL
   */
  ecma_tag_t size_type : 4;
} ecma_string_t;

/**
 * Actual data of ecma_compact_string_t depending on `tag::utf16` `tag::size_type`)
 *  the allocated size for ecma_compact_string_t always be 8
 *  the hash is not stored in header, should be calculated when retrieve hash
 *  size = size_type - ECMA_STRING_SIZE_TYPE_COMPACT
 */
typedef struct
{
  ecma_string_t tag;
  union
  {
    uint8_t ascii[8 - sizeof (ecma_string_t)];
    uint16_t utf16[(8 - sizeof (ecma_string_t)) / 2];
  } u;
} ecma_compact_string_t;
JERRY_STATIC_ASSERT (sizeof (ecma_compact_string_t) == 8, sizeof_ecma_compact_string_t_is_8);

/**
 * Actual data of ecma_internal_string_t depending on `tag::utf16` and `tag::size_type` field)
 *  the hash is stored in header
 *  size_bits = `tag::size_type` << (3 + utf16)
 *  size is stored in ecma_internal_string_t::u::size(length is `tag::size_type << utf16` bytes)
 */
typedef union
{
  struct
  {
    ecma_string_t tag;
    lit_string_hash_t hash;
    uint8_t size[8 - sizeof (ecma_string_t) - sizeof (lit_string_hash_t)];
  } u;
  uint64_t value;
} ecma_internal_string_t;
JERRY_STATIC_ASSERT (sizeof (ecma_internal_string_t) == 8, sizeof_ecma_internal_string_t_is_8);

typedef struct
{
  union
  {
    uint8_t *ascii;
    uint16_t *utf16;
  } u;
  void *user_p; /**< user pointer passed to the callback when the string is freed */
} ecma_external_string_body_t;

/**
 * Actual data of ecma_string_t depending on`tag::utf16` field)
 *  the hash is stored in header
 *  size is stored in ecma_external_string_t::size
 */
typedef struct
{
  ecma_string_t tag;
  lit_string_hash_t hash;
#if JERRY_CPOINTER_16_BIT
  uint32_t size;
#else
  uintptr_t size;
#endif
  ecma_external_string_body_t body;
} ecma_external_string_t;

#if JERRY_CPOINTER_64_BIT
JERRY_STATIC_ASSERT (sizeof (ecma_external_string_t) == 32, sizeof_ecma_external_string_t_equal_32);
#elif JERRY_CPOINTER_32_BIT
#if UINTPTR_MAX > UINT32_MAX
JERRY_STATIC_ASSERT (sizeof (ecma_external_string_t) == 32, sizeof_ecma_external_string_t_equal_32);
#else
JERRY_STATIC_ASSERT (sizeof (ecma_external_string_t) == 20, sizeof_ecma_external_string_t_equal_20);
#endif /* UINTPTR_MAX > UINT32_MAX */
#elif JERRY_CPOINTER_16_BIT
#if UINTPTR_MAX > UINT32_MAX
JERRY_STATIC_ASSERT (sizeof (ecma_external_string_t) == 24, sizeof_ecma_external_string_t_equal_24);
#else
JERRY_STATIC_ASSERT (sizeof (ecma_external_string_t) == 16, sizeof_ecma_external_string_t_equal_16);
#endif /* UINTPTR_MAX > UINT32_MAX */
#else /* ! JERRY_CPOINTER_16_BIT */
#error "JERRY_CPOINTER is not configured"
#endif /* JERRY_CPOINTER_64_BIT */

typedef struct
{
  void *origin;
  uint8_t *str;
  uint64_t len;
} ecma_string_tracked_utf8_t;

typedef struct
{
  void *origin;
  uint8_t *str;
  uint64_t len;
} ecma_string_tracked_cesu8_t;

typedef struct
{
  void *origin;
  uint16_t *str;
  uint64_t len;
} ecma_string_tracked_utf16_t;

typedef enum
{
  ECMA_ATOM_TYPE_STRING,
  ECMA_ATOM_TYPE_GLOBAL_SYMBOL,
  ECMA_ATOM_TYPE_SYMBOL,
  ECMA_ATOM_TYPE_PRIVATE
} ecma_atom_type_t;

typedef struct
{
  ecma_string_t *str;
  union
  {
    struct
    {
      uint32_t type : 3; /* ecma_atom_type_t */
      uint32_t next_index : 29;
    } tag;
    uint32_t value;
  } u;
} ecma_atom_hash_entry_t;

typedef uint32_t ecma_atom_value_t;

typedef union
{
  struct
  {
    uint32_t is_string : 1;
    uint32_t padding : 31;
  } tag;
  struct
  {
    uint32_t is_string : 1;
    uint32_t value : 31;
  } i;
  struct
  {
    uint32_t is_string : 1;
    uint32_t type : 2; /* ecma_atom_type_t */
    uint32_t index : 29;
  } s;
  ecma_atom_value_t value;
} ecma_atom_t;

JERRY_STATIC_ASSERT (sizeof (ecma_atom_t) == 4, ecma_atom_t_size_is_four);
JERRY_STATIC_ASSERT (sizeof (ecma_atom_t) == sizeof (ecma_atom_value_t), ecma_atom_t_size_equal_ecma_atom_value_size);

int
main (void)
{
  return 0;
}
