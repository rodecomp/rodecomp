#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEBYTEREADER_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEBYTEREADER_H

#include <cstring>

struct ResourceByteReader {
  const unsigned char* data;
  unsigned size;
  unsigned position = 0;

  // INLINED
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
