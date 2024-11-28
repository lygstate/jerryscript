# Design primitive layout

* Optimize `lit_is_utf8_string_pair_magic` by suffix-array
* Optimize `ecma_string_t` with quickjs atom like things

```c

```

## ecma_atom_t

* 4 ECMA_ATOM_NUMBER `31 bit unsigned integer`
* 1 ECMA_ATOM_SYMBOL
* 1 ECMA_ATOM_GLOBAL_SYMBOL
* 1 ECMA_ATOM_STRING
* 1 ECMA_ATOM_PRIVATE

## ecma_type_t

* 1 ECMA_TYPE_INTEGER
  * directly encoded number value `31 bit signed integer`
* 1 ECMA_TYPE_DIRECT
  * directly encoded `simple value`, `extend point for future`
* 1 ECMA_TYPE_STRING
  * `pointer` to description of a `string` , that is `ecma_string_t` (8 byte aligned)
* 1 ECMA_TYPE_SYMBOL
  * `pointer` to description of a `symbol`  that is `ecma_string_t` (8 byte aligned)
* 1 ECMA_TYPE_FLOAT
  * `pointer` to a 64 bit floating point number (8 byte aligned)
* 1 ECMA_TYPE_BIGINT
  * `pointer` to description of a `bigint`  (8 byte aligned)
* 1 ECMA_TYPE_OBJECT
  * `pointer` to (`object`, `function`, `extend point for future`) (8 byte aligned)
* 1 ECMA_TYPE_ERROR
  * pointer to description of an error reference(exception) (only supported by C API)

## brief testing command

```bash
clang-cl --target=i686-pc-windows-msvc -DJERRY_CPOINTER_64_BIT=1 -c test-api.c

clang-cl --target=i686-pc-windows-msvc -DJERRY_CPOINTER_16_BIT=1 -c test-api.c
clang-cl --target=i686-pc-windows-msvc -DJERRY_CPOINTER_32_BIT=1 -c test-api.c
clang-cl --target=x86_64-pc-windows-msvc -DJERRY_CPOINTER_16_BIT=1 -c test-api.c
clang-cl --target=x86_64-pc-windows-msvc -DJERRY_CPOINTER_32_BIT=1 -c test-api.c
clang-cl --target=x86_64-pc-windows-msvc -DJERRY_CPOINTER_64_BIT=1 -c test-api.c
```
