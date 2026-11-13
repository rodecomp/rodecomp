#include "Utility/Hash/GGHash.h"

#include "Security/FileSignatureVerifier.h"
#include "Utility/Hash/MD5.h"

#include <cstdint>
#include <cstdlib>

struct GGHashEntry {
  std::uint32_t nameHash;
  std::uint32_t fileHash;
};

static const DWORD* FindHash(const DWORD* table, DWORD count, DWORD value) {
  while (count != 0) {
    const DWORD* mid = &table[count >> 1];
    if (value == *mid) {
      return mid;
    }
    if (value > *mid) {
      table = mid + 1;
      --count;
    }
    count >>= 1;
  }
  return nullptr;
}

// OFFSET: 0x004012B0
// FUNCTION: fn_00109
CGGHash::CGGHash() : m_error(0), m_size(0), m_data(nullptr) {
}

// OFFSET: 0x00401300
// FUNCTION: fn_00110
// VIRTUALIZED: 2022-03-31 0x00401300
BOOL CGGHash::Load(const wchar_t* filename) {
  this->Clear();

  HANDLE file = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    this->m_error = 1;
    return FALSE;
  }

  LARGE_INTEGER fileSize;
  fileSize.QuadPart = 0;
  if (!GetFileSizeEx(file, &fileSize)) {
    this->m_error = 2;
    return FALSE;
  }

  BOOL result = FALSE;
  HANDLE mapping = CreateFileMappingW(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
  if (mapping != nullptr) {
    const DWORD* view = static_cast<const DWORD*>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0));
    if (view != nullptr) {
      BOOL ok = TRUE;
      MD5 md5;

      std::uint32_t seed = view[0];
      DWORD encryptedCount = view[1];
      DWORD encryptedChecksum = view[2];
      const DWORD* src = &view[3];

      unsigned int keyCount = seed % 10 + 6;
      for (unsigned int i = keyCount; i != 0; --i) {
        seed = md5.HashBuffer(reinterpret_cast<const unsigned char*>(&seed), 4);
      }

      DWORD keys[16];
      for (int i = 0; i < static_cast<int>(keyCount); ++i) {
        keys[i] = md5.HashBuffer(reinterpret_cast<const unsigned char*>(&seed), 4);
        seed = keys[i];
      }

      DWORD count = keys[4] ^ encryptedCount;
      if (count != 0) {
        this->m_data = static_cast<BYTE*>(::operator new(4 * (2 * count + 1)));
        DWORD* table = reinterpret_cast<DWORD*>(this->m_data);
        this->m_size = count;

        DWORD checksum = keys[5] ^ encryptedChecksum;
        DWORD running = 0xFFFFFFFF;
        for (DWORD i = 0; i < count; ++i) {
          DWORD value = *src++ ^ keys[i % keyCount];
          running ^= value;
          table[i] = value;
        }

        if (checksum == running) {
          table[count] = ~table[(count - 1) >> 1] ^ checksum;
          for (DWORD i = 0; i < count; ++i) {
            table[count + 1 + i] = *src++ ^ keys[(i + 4) % keyCount];
          }

          FileSignatureVerifier verifier;
          if (!verifier.VerifyBuffer(filename, reinterpret_cast<const BYTE*>(view), fileSize.LowPart, true)) {
            ok = FALSE;
            this->m_error = 5;
          }
        } else {
          ok = FALSE;
          this->m_error = 4;
        }
      } else {
        ok = FALSE;
        this->m_error = 3;
      }

      UnmapViewOfFile(view);
      result = ok;
    }
    CloseHandle(mapping);
  }
  CloseHandle(file);
  return result;
}

// OFFSET: 0x00401640
// FUNCTION: fn_00111
// VIRTUALIZED: 2022-03-31 0x00401640
int CGGHash::Check() {
  DWORD count = this->m_size;
  if (count == 0) {
    this->m_error = 3;
    return 0;
  }

  const DWORD* table = reinterpret_cast<const DWORD*>(this->m_data);
  DWORD running = 0xFFFFFFFF;
  for (DWORD i = 0; i < count; ++i) {
    running ^= table[i];
  }

  if (table[count] != (running ^ ~table[(count - 1) >> 1])) {
    return 0;
  }
  return 1;
}

// OFFSET: 0x00401730
// FUNCTION: fn_00112
// VIRTUALIZED: 2022-03-31 0x004016E0
int CGGHash::VerifyHash(const wchar_t* name, DWORD hash) {
  if (name == nullptr) {
    this->m_error = 1;
    return 0;
  }

  if (this->m_size == 0) {
    this->m_error = 3;
    return 0;
  }

  MD5 md5;
  std::uint32_t nameHash = md5.HashStringLower(name);
  DWORD count = this->m_size;
  const DWORD* table = reinterpret_cast<const DWORD*>(this->m_data);

  if (FindHash(table, count, hash ^ nameHash) != nullptr) {
    return 1;
  }
  return FindHash(&table[count + 1], count, nameHash) != nullptr ? 0 : 2;
}

// OFFSET: 0x00401870
// FUNCTION: fn_00113
DWORD CGGHash::GetSize() const {
  return this->m_size;
}

// OFFSET: 0x00401880
// FUNCTION: fn_00114
DWORD CGGHash::GetError() const {
  return this->m_error;
}

// OFFSET: 0x004018C0
// FUNCTION: fn_00115
BOOL CGGHash::Load(const char* filename) {
  WCHAR wideFileName[260];
  MultiByteToWideChar(CP_ACP, 0, filename, -1, wideFileName, 260);
  return this->Load(wideFileName);
}

// OFFSET: 0x00401920
// FUNCTION: fn_00116
int CGGHash::VerifyBuffer(const wchar_t* name, const unsigned char* buf, unsigned int size) {
  this->m_error = 0;
  if (name == nullptr) {
    this->m_error = 1;
    return 0;
  }

  if (buf == nullptr || size == 0) {
    this->m_error = 2;
    return 0;
  }

  MD5 md5;
  std::uint32_t hash = md5.HashBuffer(buf, size);
  return this->VerifyHash(name, hash);
}

// OFFSET: 0x004019E0
// FUNCTION: fn_00117
int CGGHash::VerifyFile(const wchar_t* filename) {
  this->m_error = 0;
  if (filename == nullptr) {
    this->m_error = 1;
    return 0;
  }

  MD5 md5;
  std::uint32_t hash = md5.HashFile(filename);
  if (hash == 0) {
    this->m_error = 2;
    return 0;
  }

  return this->VerifyHash(filename, hash);
}

// OFFSET: 0x00401AC0
// FUNCTION: fn_00118
CGGHash::~CGGHash() {
  this->Clear();
}

// OFFSET: 0x00401B10
// FUNCTION: fn_00119
int CGGHash::VerifyBuffer(const char* name, const unsigned char* buf, unsigned int size) {
  this->m_error = 0;
  if (name == nullptr) {
    this->m_error = 1;
    return 0;
  }

  if (buf == nullptr || size == 0) {
    this->m_error = 2;
    return 0;
  }

  WCHAR wideName[260];
  MultiByteToWideChar(CP_ACP, 0, name, -1, wideName, 260);
  return this->VerifyBuffer(wideName, buf, size);
}

// OFFSET: 0x00401BB0
// FUNCTION: fn_00120
int CGGHash::VerifyFile(const char* filename) {
  this->m_error = 0;
  if (filename == nullptr) {
    this->m_error = 1;
    return 0;
  }

  WCHAR wideFileName[260];
  MultiByteToWideChar(CP_ACP, 0, filename, -1, wideFileName, 260);
  return this->VerifyFile(wideFileName);
}
