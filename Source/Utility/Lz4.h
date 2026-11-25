#ifndef RAGNAROK_SOURCE_UTILITY_LZ4_H
#define RAGNAROK_SOURCE_UTILITY_LZ4_H

extern "C" int __cdecl LZ4_decompress_safe(const char* source, char* destination, int compressedSize, int maxDecompressedSize);

#endif
