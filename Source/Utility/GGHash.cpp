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

// OFFSET: 0x00401BB0
