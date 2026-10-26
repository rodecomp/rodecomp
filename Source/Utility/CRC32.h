#ifndef RAGNAROK_SOURCE_UTILITY_CRC32_H
#define RAGNAROK_SOURCE_UTILITY_CRC32_H

#include <cstddef>
#include <windows.h>

class CRC32 {
public:
  enum Error : DWORD {
    InvalidArgument = 1,
    FileOpenFailed = 2,
    FileReadFailed = 3,
    FileSeekFailed = 4
  };

  /* 0000 */ DWORD m_table[256];
  /* 0400 */ DWORD m_error;

  CRC32();
  ~CRC32();

  bool Update(const BYTE* data, int length, DWORD* checksum);
  bool UpdateFile(const WCHAR* filename, LONG offset, DWORD length, DWORD* checksum);
  DWORD Calculate(const char* text);
  DWORD Calculate(const WCHAR* text);
  DWORD CalculateLowercase(const char* text);
  DWORD CalculateLowercase(const WCHAR* text);
  bool Calculate(const BYTE* data, int length, DWORD* checksum);
  bool CalculateFile(const WCHAR* filename, DWORD* checksum);
};

static_assert(offsetof(CRC32, m_table) == 0x0000);
static_assert(offsetof(CRC32, m_error) == 0x0400);
static_assert(sizeof(CRC32) == 0x0404);

#endif
