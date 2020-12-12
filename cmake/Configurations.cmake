macro(SAKURA_DEF def default_value)
    set(${def} ${default_value})
    if(${def})
        add_definitions(-D "${def}")
    endif(${def})
endmacro(SAKURA_DEF)

macro(SAKURA_REMOVE_DEF def)
    set(${def} FALSE)
    remove_definitions("-D ${def}")
endmacro()

## Host Device.
SAKURA_DEF(SAKURA_HOST TRUE)

## debug mode as default.
SAKURA_DEF(SAKURA_DEBUG TRUE)

## release mode.
SAKURA_DEF(SAKURA_RELEASE FALSE)

## shipping mode.
SAKURA_DEF(SAKURA_SHIPPING FALSE)

## Use Intel ISPC.
SAKURA_DEF(SAKURA_USE_ISPC FALSE)

## Uses SSE.
SAKURA_DEF(SAKURA_USE_SSE TRUE)

## Forces Static Link.
SAKURA_DEF(SAKURA_FORCE_STATIC_LINK TRUE)

## Use Microsoft MIMALLOC.
SAKURA_DEF(SAKURA_USE_MIMALLOC TRUE)

## Use Microsoft DXMath.
SAKURA_DEF(SAKURA_USE_DXMATH TRUE)

## sse2-i32x4, sse2-i32x8, sse4-i8x16, sse4-i16x8, sse4-i32x4, sse4-i32x8
## avx1-i32x4, avx1-i32x8, avx1-i32x16, avx1-i64x4
## avx2-i8x32, avx2-i16x16, avx2-i32x4, avx2-i32x8, avx2-i32x16, avx2-i64x4
## avx512knl-i32x16, avx512skx-i32x8, avx512skx-i32x16, avx512skx-i8x64, avx512skx-i16x32
## neon-i8x16, neon-i16x8, neon-i32x4, neon-i32x8
## wasm-i32x4
set(TARGET_SSE_AVX_NEON_LEVEL sse2,sse4,avx2,avx512knl-i32x16)


set(android 0)
set(linux 0)
set(macos 0)
set(ios 0)
set(windows 0)
set(web 0)