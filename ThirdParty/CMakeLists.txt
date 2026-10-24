# zlib 1.2.13, unmodified upstream sources.
add_library(RagnarokZlib STATIC
  zlib/adler32.c
  zlib/crc32.c
  zlib/inflate.c
  zlib/inftrees.c
  zlib/inffast.c
  zlib/uncompr.c
  zlib/zutil.c
  zlib/compress.c
  zlib/deflate.c
  zlib/trees.c
)
target_include_directories(RagnarokZlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/zlib")
target_compile_definitions(RagnarokZlib PRIVATE _CRT_SECURE_NO_WARNINGS)

# libpng1.6.36, original bitmap wrapper version/configured standard API.
add_library(RagnarokPng STATIC
  libpng/png.c
  libpng/pngerror.c
  libpng/pngget.c
  libpng/pngmem.c
  libpng/pngpread.c
  libpng/pngread.c
  libpng/pngrio.c
  libpng/pngrtran.c
  libpng/pngrutil.c
  libpng/pngset.c
  libpng/pngtrans.c
  libpng/pngwio.c
  libpng/pngwrite.c
  libpng/pngwtran.c
  libpng/pngwutil.c
)
target_include_directories(RagnarokPng PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libpng")
target_compile_definitions(RagnarokPng PRIVATE PNG_STATIC _CRT_SECURE_NO_WARNINGS)
target_link_libraries(RagnarokPng PUBLIC RagnarokZlib)


# JsonCpp 0.5.0 lineage, default std::map Value storage and exception semantics.
add_library(RagnarokJson STATIC
  jsoncpp/src/lib_json/json_reader.cpp
  jsoncpp/src/lib_json/json_value.cpp
  jsoncpp/src/lib_json/json_writer.cpp
)
target_include_directories(RagnarokJson PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/include")
target_compile_features(RagnarokJson PUBLIC cxx_std_20)
target_compile_definitions(RagnarokJson PUBLIC JSON_NO_AUTOLINK _ITERATOR_DEBUG_LEVEL=0)
target_compile_definitions(RagnarokJson PRIVATE _CRT_SECURE_NO_WARNINGS NDEBUG)
set_target_properties(RagnarokJson PROPERTIES CXX_EXTENSIONS OFF)

# Lua 5.1.4; source/header identity and x86 chunk format verified separately.
add_library(RagnarokLua STATIC
  lua/src/lapi.c
  lua/src/lauxlib.c
  lua/src/lbaselib.c
  lua/src/lcode.c
  lua/src/ldblib.c
  lua/src/ldebug.c
  lua/src/ldo.c
  lua/src/ldump.c
  lua/src/lfunc.c
  lua/src/lgc.c
  lua/src/linit.c
  lua/src/liolib.c
  lua/src/llex.c
  lua/src/lmathlib.c
  lua/src/lmem.c
  lua/src/loadlib.c
  lua/src/lobject.c
  lua/src/lopcodes.c
  lua/src/loslib.c
  lua/src/lparser.c
  lua/src/lstate.c
  lua/src/lstring.c
  lua/src/lstrlib.c
  lua/src/ltable.c
  lua/src/ltablib.c
  lua/src/ltm.c
  lua/src/lundump.c
  lua/src/lvm.c
  lua/src/lzio.c
)
target_include_directories(RagnarokLua PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lua/src")
target_compile_definitions(RagnarokLua PRIVATE _CRT_SECURE_NO_WARNINGS)
