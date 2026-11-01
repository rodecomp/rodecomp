#include "File/CommonObject.h"
#include "Client/Input/InputCharset.h"
#include "File/File.h"

#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>

// INLINED
CCommonObject::~CCommonObject() {
  this->Clear();
}

// OFFSET: 0x005B7C90
// FUNCTION: fn_03417
void CCommonObject::Clear() {
  for (char* buffer : this->m_buffers) {
    delete[] buffer;
  }

  this->m_buffers.clear();
}

// OFFSET: 0x005B8810
// FUNCTION: fn_03422
bool CCommonObject::LoadTokens(std::vector<char*>& tokens, const char* name, int mode) {
  tokens.clear();

  if (tokens.capacity() < 4096) {
    tokens.reserve(4096);
  }

  CFile file;

  if (g_serverLanguage) {
    name = g_serverLanguage->ServiceName(name);
  }

  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  const int size = static_cast<int>(file.GetSize());
  char* buffer = new char[size + 1];
  file.Read(buffer, size);
  buffer[size] = '\0';
  file.Close();
  char* start = buffer;

  for (int index = 0; index < size; ++index) {
    switch (buffer[index]) {
    case '\n':
      start = buffer + index + 1;
      break;
    case '\r':
      buffer[index] = '\0';

      if (start && *start != '\0' && mode != 0 && std::strncmp(start, "//", 2) != 0) {
        tokens.push_back(start);
      }

      start = buffer + index + 1;
      break;
    case '#':
      if (std::strncmp(start, "//", 2) != 0) {
        buffer[index] = '\0';
        tokens.push_back(start);

        if (mode == 2) {
          start = nullptr;
          tokens.push_back(start);
        }

        start = buffer + index + 1;
      }

      break;
    default:
      break;
    }
  }

  this->m_buffers.push_back(buffer);
  return true;
}
