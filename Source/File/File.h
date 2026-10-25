#ifndef RAGNAROK_SOURCE_FILE_FILE_H
#define RAGNAROK_SOURCE_FILE_FILE_H

#include <cstdint>
#include <string>
#include <windows.h>

class CRenderSystemBase;

class CFile {
public:
  CFile();
  virtual ~CFile();

  bool OpenWrite(const char* path, CRenderSystemBase* renderSystem);
  bool Open(const char* path) {
    this->m_path = path;
    this->m_handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (this->m_handle == INVALID_HANDLE_VALUE) {
      return false;
    }

    LARGE_INTEGER size;
    GetFileSizeEx(this->m_handle, &size);
    this->m_size = size.LowPart;
    return true;
  }
  // INLINED
  std::uint32_t GetSize() const {
    return this->m_size;
  }
  bool Write(const void* data, std::uint32_t bytes);
  bool Read(void* data, std::uint32_t bytes);
  bool Peek(void* data, std::uint32_t bytes);
  bool Seek(std::uint32_t distance, DWORD origin);
  void* ReadPtr(std::uint32_t bytes);
  bool IsEOF() const;
  void Close();

protected:
  /* 0004 */ HANDLE m_handle;
  /* 0008 */ void* m_data;
  /* 000C */ std::uint32_t m_position;
  /* 0010 */ std::uint32_t m_size;
  /* 0014 */ std::string m_path;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CFile) == 44);
#endif

#endif
