#include "Utility/GGHash.h"

// OFFSET: 0x004012B0
// FUNCTION: fn_00109
CGGHash::CGGHash() : m_error(0), m_size(0), m_data(nullptr) {
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

// OFFSET: 0x00401AC0
// FUNCTION: fn_00118
CGGHash::~CGGHash() {
  this->Clear();
}
