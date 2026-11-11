#include "Client/Session/Session.h"
#include "Contents/Map/MapInfoMgr.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

// OFFSET: 0x00DE8740
// FUNCTION: fn_23498
bool CSession::LoadMapNameTable() {
  std::vector<char*> tokens;
  CCommonObject& storage = reinterpret_cast<CCommonObject&>(this->minimapTokenStorage[0x30]);
  const char source[] = {'\x52', '\x88', '\x4B', '\xB1', '\x0A', '\x62', '\x9E', '\x43', '\x5E', '\x8B', '\x57', '\x9A', '\x45', '\x7B', '\x83', '\x63', '\x3F'};
  thread_local EncryptedName<sizeof(source), 0x17FB0F6BFF3BE93FULL> name(source);
  const bool loaded = storage.LoadTokens(tokens, name.Text(), 0);

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

// INLINED
static __forceinline void ScaleMapRect(MapPosRect* rect) {
  if (!rect) {
    return;
  }

  const float scaleX = static_cast<float>(g_renderer->viewport.width) / 1280.0f;
  const float scaleY = static_cast<float>(g_renderer->viewport.height - 17) * 0.0009765625f;
  rect->left = static_cast<int>(static_cast<float>(rect->left) * scaleX + 0.5);
  rect->top = static_cast<int>(static_cast<float>(rect->top) * scaleY + 0.5) + 17;
  rect->right = static_cast<int>(static_cast<float>(rect->right) * scaleX + 0.5);
  rect->bottom = static_cast<int>(static_cast<float>(rect->bottom) * scaleY + 0.5) + 17;
}

// OFFSET: 0x00DE4FF0
// FUNCTION: fn_45471
bool CSession::ScaleMapPositions() {
  for (int index = 0; index < this->m_mapPosListCount; ++index) {
    for (std::map<std::string, MapPosEntry>::iterator it = this->m_mapPosLists[index].begin(); it != this->m_mapPosLists[index].end(); ++it) {
      ScaleMapRect(&it->second.rect);
    }
  }

  for (std::map<int, MapPosEntry>::iterator it = this->m_mapPosBounds.begin(); it != this->m_mapPosBounds.end(); ++it) {
    ScaleMapRect(&it->second.rect);
  }

  return true;
}
