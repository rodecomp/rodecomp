#ifndef RAGNAROK_SOURCE_UTILITY_GGHASH_H
#define RAGNAROK_SOURCE_UTILITY_GGHASH_H

#include <cstdlib>
#include <windows.h>

class CGGHash {
public:
  CGGHash();
  virtual ~CGGHash();

  DWORD GetSize() const;
  DWORD GetError() const;

protected:
  // INLINED
  void Clear() {
    this->m_error = 0;
    this->m_size = 0;
    if (this->m_data) {
      std::free(this->m_data);
      this->m_data = nullptr;
    }
  }

  /* 0004 */ DWORD m_error;
  /* 0008 */ DWORD m_size;
  /* 000C */ BYTE* m_data;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CGGHash) == 16);
#endif

#endif
