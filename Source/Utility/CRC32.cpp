#include "Utility/CRC32.h"

#include <cstring>
#include <cwchar>

// FUNCTION: fn_00142
CRC32::CRC32() {
  for (int i = 0; i < 256; ++i) {
    DWORD value = i;
    for (int bit = 0; bit < 8; ++bit) {
      if (value & 1) {
        value = (value >> 1) ^ 0xEDB88320;
      } else {
        value >>= 1;
      }
    }
    m_table[i] = value;
  }
}

// FUNCTION: fn_00143
CRC32::~CRC32() {
}

// FUNCTION: fn_00144
bool CRC32::Update(const BYTE* data, int length, DWORD* checksum) {
  if (data == nullptr) {
    m_error = InvalidArgument;
    return false;
  }

  const BYTE* end = data + length;
  while (data < end) {
    *checksum = m_table[static_cast<BYTE>(*checksum) ^ *data++] ^ (*checksum >> 8);
  }
  return true;
}

// FUNCTION: fn_00145
bool CRC32::UpdateFile(const WCHAR* filename, LONG offset, DWORD length, DWORD* checksum) {
  BYTE buffer[65536];
  DWORD bytesRead;

  if (filename == nullptr || *filename == 0) {
    m_error = InvalidArgument;
    return false;
  }

  HANDLE file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    m_error = FileOpenFailed;
    return false;
  }

  if (SetFilePointer(file, offset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
    m_error = FileSeekFailed;
    CloseHandle(file);
    return false;
  }

  BOOL readResult = ReadFile(file, buffer, 0xFFFF, &bytesRead, nullptr);
  while (readResult && bytesRead != 0) {
    DWORD chunkLength;
    if (length > bytesRead) {
      chunkLength = bytesRead;
      length -= bytesRead;
    } else {
      chunkLength = length;
      length = 0;
    }
    Update(buffer, chunkLength, checksum);
    if (length == 0) {
      break;
    }
    readResult = ReadFile(file, buffer, 0xFFFF, &bytesRead, nullptr);
  }

  if (!readResult) {
    m_error = FileReadFailed;
  }
  CloseHandle(file);
  return true;
}

// FUNCTION: fn_00146
DWORD CRC32::Calculate(const char* text) {
  DWORD checksum = 0xFFFFFFFF;
  Update(reinterpret_cast<const BYTE*>(text), std::strlen(text), &checksum);
  return ~checksum;
}

// FUNCTION: fn_00147
DWORD CRC32::Calculate(const WCHAR* text) {
  char converted[MAX_PATH];
  WideCharToMultiByte(CP_ACP, 0, text, -1, converted, MAX_PATH, nullptr, nullptr);
  DWORD checksum = 0xFFFFFFFF;
  Update(reinterpret_cast<const BYTE*>(converted), std::strlen(converted), &checksum);
  return ~checksum;
}

// FUNCTION: fn_00148
DWORD CRC32::CalculateLowercase(const char* text) {
  char lowercase[MAX_PATH];
  strcpy_s(lowercase, MAX_PATH, text);
  _strlwr_s(lowercase, MAX_PATH);
  DWORD checksum = 0xFFFFFFFF;
  Update(reinterpret_cast<const BYTE*>(lowercase), std::strlen(lowercase), &checksum);
  return ~checksum;
}

// FUNCTION: fn_00149
DWORD CRC32::CalculateLowercase(const WCHAR* text) {
  WCHAR lowercase[MAX_PATH];
  wcscpy_s(lowercase, MAX_PATH, text);
  _wcslwr_s(lowercase, MAX_PATH);
  return Calculate(lowercase);
}

// FUNCTION: fn_00150
bool CRC32::Calculate(const BYTE* data, int length, DWORD* checksum) {
  *checksum = 0xFFFFFFFF;
  Update(data, length, checksum);
  *checksum = ~*checksum;
  return true;
}

// FUNCTION: fn_00151
bool CRC32::CalculateFile(const WCHAR* filename, DWORD* checksum) {
  if (filename == nullptr || *filename == 0) {
    m_error = InvalidArgument;
    return false;
  }

  HANDLE file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    m_error = FileOpenFailed;
    return false;
  }

  LARGE_INTEGER fileSize;
  if (GetFileSizeEx(file, &fileSize)) {
    if (fileSize.QuadPart > 500000000) {
      fileSize.QuadPart = 500000000;
    }

    HANDLE mapping = CreateFileMappingW(file, nullptr, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, nullptr);
    if (mapping != nullptr) {
      const BYTE* data = static_cast<const BYTE*>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, fileSize.LowPart));
      if (data != nullptr) {
        Calculate(data, fileSize.LowPart, checksum);
        UnmapViewOfFile(data);
      }
      CloseHandle(mapping);
    }
    CloseHandle(file);
  }
  return true;
}
