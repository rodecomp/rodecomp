#include "File/CommonObject.h"
#include "Resource/Bitmap/BitmapResource.h"

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
    std::free(buffer);
  }

  this->m_buffers.clear();
}

// OFFSET: 0x005B8810
// FUNCTION: fn_03422
bool CCommonObject::LoadTokens(std::vector<char*>& tokens, const char* name, int mode, ResourceBytes& file) {
  tokens.clear();

  if (tokens.capacity() < 4096) {
    tokens.reserve(4096);
  }

  if (!file.Open(name)) {
    file.Close();
    return false;
  }

  const auto bytes = static_cast<unsigned>(file.Size());
  std::unique_ptr<char, decltype(&std::free)> buffer(static_cast<char*>(std::malloc(static_cast<std::size_t>(bytes) + 1)), &std::free);

  if (!buffer) {
    file.Close();
    throw std::bad_alloc();
  }

  std::memcpy(buffer.get(), file.Data(), bytes);
  buffer.get()[bytes] = '\0';
  file.Close();
  char* start = buffer.get();

  for (unsigned index = 0; index < bytes; ++index) {
    char& value = buffer.get()[index];

    switch (value) {
    case '\n':
      start = buffer.get() + index + 1;
      break;
    case '\r':
      value = '\0';

      if (*start != '\0' && mode != 0 && std::strncmp(start, "//", 2) != 0) {
        tokens.push_back(start);
      }

      start = buffer.get() + index + 1;
      break;
    case '#':
      if (std::strncmp(start, "//", 2) != 0) {
        value = '\0';
        tokens.push_back(start);

        if (mode == 2) {
          tokens.push_back(nullptr);
        }

        start = buffer.get() + index + 1;
      }

      break;
    default:
      break;
    }
  }

  this->m_buffers.push_back(buffer.get());
  buffer.release();
  return true;
}
