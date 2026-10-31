#include "Client/Session.h"
#include "Contents/MapInfoMgr.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

class TableBytes : public ResourceBytes {
public:
  CFile file;

  // INLINED
  bool Open(const char* name) override {
    return this->file.Open(name, 0, true, false);
  }

  // INLINED
  const void* Data() const override {
    return this->file.GetData();
  }

  // INLINED
  int Size() const override {
    return static_cast<int>(this->file.GetSize());
  }

  // INLINED
  void Close() override {
    this->file.Close();
  }
};

}

// OFFSET: 0x00DE8740
// FUNCTION: fn_23498
bool CSession::LoadMapNameTable() {
  std::vector<char*> tokens;
  CCommonObject& storage = reinterpret_cast<CCommonObject&>(this->unknown_5400[0x30]);
  const std::uint8_t source[] = {0x52, 0x88, 0x4B, 0xB1, 0x0A, 0x62, 0x9E, 0x43, 0x5E,
                                 0x8B, 0x57, 0x9A, 0x45, 0x7B, 0x83, 0x63, 0x3F};
  thread_local EncryptedName<sizeof(source), 0x17FB0F6BFF3BE93FULL> name(source);
  TableBytes file;
  const bool loaded = storage.LoadTokens(tokens, name.Text(), 0, file);

  for (auto token = tokens.begin(); token != tokens.end(); token += 2) {
    g_mapInfoMgr->AddMapDisplayName(token[0], token[1], false);
  }

  return loaded;
}

// OFFSET: 0x00DE8970
// FUNCTION: fn_23499
bool CSession::LoadMapPosTable() {
  this->m_mapPosLists = nullptr;
  this->m_mapPositions.clear();
  this->m_mapPosBounds.clear();

  CFile file;
  if (!file.Open("mapPosTable.txt", 0, true, false)) {
    return false;
  }

  const char* cursor = static_cast<const char*>(file.GetData());
  const char* end = cursor + file.GetSize();
  int currentId = 0;
  std::string name;
  std::vector<std::string> fields;
  std::string token;
  int boundsValid = 0;
  MapPosRect bounds = {};
  const char* start = cursor;
  for (; cursor < end; ++cursor) {
    switch (*cursor) {
    case '\n':
    case '\r':
      start = cursor + 1;
      break;
    case '@':
      if (this->m_mapPosListCount == -1) {
        this->m_mapPosListCount = std::atoi(std::string(start, cursor).c_str());
        this->m_mapPosLists = new std::map<std::string, MapPosEntry>[this->m_mapPosListCount];
      }
      break;
    case '#':
      if (token.size() == 0) {
        token.assign(start, cursor - start);
        const int id = std::atoi(std::string(start, cursor).c_str());
        if (currentId != id && boundsValid != 0) {
          MapPosEntry& entry = this->m_mapPosBounds[currentId];
          entry.rect = bounds;
          entry.valid = boundsValid;
          bounds = {};
          boundsValid = 0;
          currentId = id;
        }
      } else if (name.size() == 0) {
        name.assign(start, cursor - start);
      } else if (fields.size() < 3) {
        fields.push_back(std::string(start, cursor));
      } else {
        const std::string last(start, cursor);
        fields.push_back(last);
        const int bottom = std::atoi(fields[3].c_str());
        const int right = std::atoi(fields[2].c_str());
        const int top = std::atoi(fields[1].c_str());
        MapPosRect rect;
        rect.left = std::atoi(fields[0].c_str());
        rect.top = top;
        rect.right = right;
        rect.bottom = bottom;

        MapPosEntry& listed = this->m_mapPosLists[currentId][name];
        listed.rect = rect;
        listed.valid = 1;
        MapPosEntry& position = this->m_mapPositions[name];
        position.rect = rect;
        position.valid = 1;

        if (boundsValid == 0) {
          bounds = rect;
          boundsValid = 1;
        } else {
          if (bounds.right < right) {
            bounds.right = right;
          }
          if (bounds.bottom < bottom) {
            bounds.bottom = bottom;
          }
          if (rect.left < bounds.left) {
            bounds.left = rect.left;
          }
          if (top < bounds.top) {
            bounds.top = top;
          }
        }

        token.clear();
        name.clear();
        fields.clear();
      }
      start = cursor + 1;
      break;
    }
  }

  if (boundsValid != 0) {
    MapPosEntry& entry = this->m_mapPosBounds[currentId];
    entry.rect = bounds;
    entry.valid = boundsValid;
  }

  file.Close();
  return true;
}
