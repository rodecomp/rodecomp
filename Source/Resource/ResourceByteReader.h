#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEBYTEREADER_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEBYTEREADER_H

#include <cstring>

// Original CFile memory-backed Read contract over an explicit resource owner.
// Failed reads leave output and cursor unchanged; loaders retain original
// unchecked-read policy rather than inventing format validation.
struct ResourceByteReader {
  const unsigned char* data;
  unsigned size;
  unsigned position = 0;

  bool Read(void* output, unsigned bytes) {
    if (position + bytes > size) {
      return false;
    }

    std::memcpy(output, data + position, bytes);
    position += bytes;
    return true;
  }
};

#endif
