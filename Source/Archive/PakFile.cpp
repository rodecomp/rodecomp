#include "Archive/PakFile.h"

// OFFSET: 0x005EF660
CPakFile::~CPakFile() = default;

namespace myGPack {
  // OFFSET: 0x005F7790
  // FUNCTION: fn_04046
  CReadPakFile::CReadPakFile() : m_handle(INVALID_HANDLE_VALUE), m_size(0) {
  }

  // INLINED
  CReadPakFile::~CReadPakFile() {
    this->Close();
  }

  // OFFSET: 0x005EF710
  CWritePakFile::~CWritePakFile() = default;

  // OFFSET: 0x005F7780
  // FUNCTION: fn_04045
  std::uint64_t CReadPakFile::GetSize() const {
    return this->m_size;
  }

  // OFFSET: 0x005F7810
  // FUNCTION: fn_04047
  void CReadPakFile::Close() {
    if (this->m_handle) {
      CloseHandle(this->m_handle);
      this->m_handle = nullptr;
    }
  }

  // OFFSET: 0x005F7890
  // FUNCTION: fn_04048
  bool CWritePakFile::Open(const char* filename) {
    this->m_handle = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH | FILE_ATTRIBUTE_NORMAL, nullptr);
    if (this->m_handle == INVALID_HANDLE_VALUE) {
      return false;
    }

    LARGE_INTEGER size;

    GetFileSizeEx(this->m_handle, &size);
    this->m_size = static_cast<std::uint64_t>(size.QuadPart);
    return true;
  }

  // OFFSET: 0x005F78F0
  // FUNCTION: fn_04049
  std::uint8_t* CReadPakFile::Read(std::uint64_t offset, std::uint32_t length) {
    if (offset + length > this->m_size) {
      return nullptr;
    }

    this->m_buffer.resize(length);

    LARGE_INTEGER position;
    position.QuadPart = static_cast<std::int64_t>(offset);

    SetFilePointerEx(this->m_handle, position, nullptr, FILE_BEGIN);

    DWORD read;
    if (ReadFile(this->m_handle, this->m_buffer.data(), length, &read, nullptr) && read == length) {
      return this->m_buffer.data();
    }

    return nullptr;
  }

  // OFFSET: 0x005F79C0
  // FUNCTION: fn_04050
  bool CWritePakFile::Write(std::uint64_t offset, const void* bytes, std::uint32_t length) {
    LARGE_INTEGER position;
    position.QuadPart = static_cast<std::int64_t>(offset);
    SetFilePointerEx(this->m_handle, position, nullptr, FILE_BEGIN);

    DWORD written;
    if (!WriteFile(this->m_handle, bytes, length, &written, nullptr) || written != length) {
      return false;
    }

    std::uint64_t end = offset + length;
    if (this->m_size < end) {
      this->m_size = end;
    }

    return true;
  }
}
