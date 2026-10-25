#include "Client/Application/ErrorDialog.h"
#include "File/File.h"

#include <cstring>

// INLINED
CFile::CFile() : m_handle(INVALID_HANDLE_VALUE), m_data(nullptr), m_position(0), m_size(0) {
}

// OFFSET: 0x005222F0
// FUNCTION: fn_01124
CFile::~CFile() {
  this->Close();
}

// INLINED
bool CFile::OpenWrite(const char* path, CRenderSystemBase* renderSystem) {
  this->m_path = path;
  this->m_handle = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (this->m_handle == INVALID_HANDLE_VALUE) {
    ShowErrorDialog(renderSystem, this->m_path.c_str(), 0);
    return false;
  }

  LARGE_INTEGER size;
  GetFileSizeEx(this->m_handle, &size);
  this->m_size = size.LowPart;

  return true;
}

// OFFSET: 0x0057C340
// FUNCTION: fn_02601
bool CFile::Write(const void* data, std::uint32_t bytes) {
  DWORD written;
  return this->m_handle != INVALID_HANDLE_VALUE && WriteFile(this->m_handle, data, bytes, &written, nullptr);
}

// OFFSET: 0x0057C260
// FUNCTION: fn_02599
bool CFile::Read(void* data, std::uint32_t bytes) {
  if (this->m_handle != INVALID_HANDLE_VALUE) {
    DWORD read;
    return ReadFile(this->m_handle, data, bytes, &read, nullptr) != FALSE;
  }

  if (!this->m_data || this->m_position + bytes > this->m_size) {
    return false;
  }

  std::memcpy(data, static_cast<const unsigned char*>(this->m_data) + this->m_position, bytes);
  this->m_position += bytes;
  return true;
}

// OFFSET: 0x0057C1E0
// FUNCTION: fn_02598
bool CFile::Peek(void* data, std::uint32_t bytes) {
  if (this->m_handle != INVALID_HANDLE_VALUE) {
    DWORD read;

    if (!ReadFile(this->m_handle, data, bytes, &read, nullptr)) {
      return false;
    }

    LARGE_INTEGER distance;

    distance.QuadPart = static_cast<std::uint32_t>(0u - bytes);
    SetFilePointerEx(this->m_handle, distance, nullptr, FILE_CURRENT);
    return true;
  }

  if (!this->m_data || this->m_position + bytes > this->m_size) {
    return false;
  }

  std::memcpy(data, static_cast<const unsigned char*>(this->m_data) + this->m_position, bytes);
  return true;
}

// OFFSET: 0x0057C2D0
// FUNCTION: fn_02600
bool CFile::Seek(std::uint32_t distance, DWORD origin) {
  if (this->m_handle != INVALID_HANDLE_VALUE) {
    LARGE_INTEGER offset;
    offset.QuadPart = distance;
    SetFilePointerEx(this->m_handle, offset, nullptr, origin);

    return true;
  }

  if (!this->m_data) {
    return false;
  }

  switch (origin) {
  case FILE_BEGIN:
    this->m_position = distance;
    break;
  case FILE_CURRENT:
    this->m_position += distance;
    break;
  case FILE_END:
    this->m_position = this->m_size + distance;
    break;
  }

  if (this->m_position > this->m_size) {
    this->m_position = this->m_size;
    return false;
  }

  return true;
}

// OFFSET: 0x0057B940
// FUNCTION: fn_02592
void CFile::Close() {
  if (this->m_handle == INVALID_HANDLE_VALUE) {
    if (this->m_data) {
      VirtualFree(this->m_data, 0, MEM_RELEASE);
      this->m_data = nullptr;
    }
  } else {
    CloseHandle(this->m_handle);
    this->m_handle = INVALID_HANDLE_VALUE;
  }
}

// OFFSET: 0x0057BB20
// FUNCTION: fn_02593
void* CFile::ReadPtr(std::uint32_t bytes) {
  std::uint32_t offset = this->m_position;
  this->m_position = offset + bytes;
  return static_cast<char*>(this->m_data) + offset;
}

// OFFSET: 0x0057BB40
// FUNCTION: fn_02594
bool CFile::IsEOF() const {
  return this->m_position >= this->m_size;
}
