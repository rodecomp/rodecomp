#include "Archive/MemMapPakFile.h"

#include <bit>

// OFFSET: 0x00AE99D0
// FUNCTION: fn_18111
// OFFSET: 0x00AECAA0
// FUNCTION: fn_18140
CMemMapPakFile::CMemMapPakFile()
    : m_handle(nullptr), m_mapping(nullptr), m_viewSize(0), m_windowSize(0x20000000), m_size(0), m_viewOffset(0),
      m_view(nullptr) {
  SYSTEM_INFO information;
  GetSystemInfo(&information);
  this->m_offsetMask = 0xFFFFFFFF00000000ULL | static_cast<std::uint32_t>(~(information.dwAllocationGranularity - 1));
}

// OFFSET: 0x00AE9850
// FUNCTION: fn_18108
// OFFSET: 0x00AE9950
// FUNCTION: fn_18110
CMemMapPakFile::~CMemMapPakFile() {
  this->Close();
}

// OFFSET: 0x00AEA420
std::uint64_t CMemMapPakFile::GetSize() const {
  return this->m_size;
}

// OFFSET: 0x00AECB10
// FUNCTION: fn_18141
bool CMemMapPakFile::Open(const char* filename) {
  this->m_handle =
    CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (this->m_handle == INVALID_HANDLE_VALUE) {
    this->m_handle = nullptr;
    return false;
  }

  LARGE_INTEGER size;
  GetFileSizeEx(this->m_handle, &size);
  this->m_size = std::bit_cast<std::uint64_t>(size.QuadPart);
  this->m_viewOffset = 0;
  this->m_viewSize = 0;
  this->m_mapping = CreateFileMappingA(this->m_handle, nullptr, PAGE_READONLY, 0, 0, nullptr);
  if (this->m_mapping) {
    return true;
  }
  this->m_windowSize = 0;
  return false;
}

// OFFSET: 0x00AECA50
// FUNCTION: fn_18139
void CMemMapPakFile::Close() {
  if (this->m_windowSize && this->m_view) {
    UnmapViewOfFile(this->m_view);
    this->m_view = nullptr;
  }
  if (this->m_mapping) {
    CloseHandle(this->m_mapping);
    this->m_mapping = nullptr;
  }
  if (this->m_handle) {
    CloseHandle(this->m_handle);
    this->m_handle = nullptr;
  }
}

// OFFSET: 0x00AECBB0
// FUNCTION: fn_18142
std::uint8_t* CMemMapPakFile::Read(std::uint64_t offset, std::uint32_t length) {
  if (this->m_windowSize) {
    if (offset >= this->m_viewOffset && offset + length <= this->m_viewOffset + this->m_viewSize) {
      return this->m_view + static_cast<std::uint32_t>(offset - this->m_viewOffset);
    }
    if (this->m_view) {
      UnmapViewOfFile(this->m_view);
      this->m_view = nullptr;
    }

    while (this->m_windowSize >=
           length + (static_cast<std::uint32_t>(offset) & ~static_cast<std::uint32_t>(this->m_offsetMask))) {
      this->m_viewOffset = offset & this->m_offsetMask;
      this->m_viewSize = this->m_windowSize;
      SIZE_T bytes = this->m_windowSize;
      if (this->m_viewOffset + this->m_windowSize > this->m_size) {
        bytes = 0;
        this->m_viewSize = static_cast<std::uint32_t>(this->m_size - this->m_viewOffset);
      }
      this->m_view = static_cast<std::uint8_t*>(MapViewOfFile(
        this->m_mapping, FILE_MAP_READ, static_cast<DWORD>(this->m_viewOffset >> 32),
        static_cast<DWORD>(this->m_viewOffset), bytes
      ));
      if (this->m_view) {
        return this->m_view + static_cast<std::uint32_t>(offset - this->m_viewOffset);
      }
      this->m_windowSize >>= 1;
      if (!this->m_windowSize) {
        break;
      }
    }

    CloseHandle(this->m_mapping);
    this->m_mapping = nullptr;
    this->m_view = nullptr;
    this->m_viewSize = 0;
    this->m_windowSize = 0;
  }

  if (this->m_viewSize < length) {
    this->m_viewSize = length;
    this->m_buffer.resize(length);
    this->m_view = this->m_buffer.data();
  }
  LARGE_INTEGER position;
  position.QuadPart = std::bit_cast<std::int64_t>(offset);
  SetFilePointerEx(this->m_handle, position, nullptr, FILE_BEGIN);
  DWORD read = 0;
  if (ReadFile(this->m_handle, this->m_buffer.data(), length, &read, nullptr)) {
    // The original accepts a successful short read; it does not compare read.
    this->m_view = this->m_buffer.empty() ? nullptr : this->m_buffer.data();
    return this->m_view;
  }
  return nullptr;
}
