# Common header files used in C++ code

- `dcore/Uni.hpp`: should be included in most of the files
  - Provides:
    * `DCORE_OWN` - annotation for pointers (`void DCORE_OWN *ptr`), means
                  that the pointer is managed by the class that contains it.
                  *Note: used rarely*
    * `DCORE_REF` - annotation for pointers (`void DCORE_OWN *ptr`), means
                  that the pointer is a reference to some object.
                  *Note: used rarely*
    
    * `sbyte` - type representing an 8-bit signed number
    * `ubyte` - type representing an 8-bit unsigned number
    * `byte = ubyte`
  - Includes:
    * `cinttypes` - *stdlib* common integer types like `uint32_t`, `uint16_t`
                    *Note: use `dstd::UIntN` types instead.*

- `dcore/Core/Type.hpp`: common types used across all of the modules
  - Provides:
    - `namespace dstd`:
      - `USize` - pointer size
      - `UInt64` - unsigned 64bit integer
      - `UInt32` - unsigned 32bit integer
      - `UInt16` - unsigned 16bit integer
      - `UInt8` - unsigned 8bit integer
      - `Byte = UInt8`

- `dcore/Core/Memory.hpp`: common memory functions used across all of the modules
  - Provides:
    - `namespace dstd`:
      - `CopyBuffer(USize n, T *dst, const U *src)` - copies `n` `U`'s from `src` to `dst`
      - TODO

