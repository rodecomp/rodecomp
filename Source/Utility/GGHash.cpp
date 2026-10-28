#include "Utility/GGHash.h"

#include "Utility/FileSignatureVerifier.h"
#include "Utility/MD5.h"

#include <cstdint>
#include <cstdlib>

struct GGHashEntry {
  std::uint32_t nameHash;
  std::uint32_t fileHash;
};

// OFFSET: 0x004012B0
// FUNCTION: fn_00109
CGGHash::CGGHash() : m_error(0), m_size(0), m_data(nullptr) {
}

// OFFSET: 0x00401300

// OFFSET: 0x00401640

// OFFSET: 0x00401730

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

// OFFSET: 0x004019E0

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
