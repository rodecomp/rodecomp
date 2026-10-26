#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKCRYPTO_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKCRYPTO_H

#include <windows.h>

namespace myGPack {
  int DecryptPakData(const BYTE* secret, DWORD secretSize, const BYTE* input, BYTE* output, DWORD dataSize);
}

#endif
