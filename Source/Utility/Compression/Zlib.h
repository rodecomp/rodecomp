#ifndef RAGNAROK_SOURCE_UTILITY_COMPRESSION_ZLIB_H
#define RAGNAROK_SOURCE_UTILITY_COMPRESSION_ZLIB_H

extern "C" int __cdecl uncompress(unsigned char* destination, unsigned long* destinationLength, const unsigned char* source, unsigned long sourceLength);

#endif
